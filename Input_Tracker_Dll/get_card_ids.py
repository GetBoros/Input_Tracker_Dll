from playwright.sync_api import sync_playwright
import json
import os
from urllib.parse import urljoin  # Добавьте этот импорт

CONFIG = {
    "COOKIES_FILE": "auth_cookies.json",
    "BASE_URL": "https://asstars.tv",
    "USER_URL": "/user/Integra_Prime/cards/?rank=a",
    "OUTPUT_FILE": "card_ids.json",
    "HEADLESS": True,
    "TIMEOUT": 15000
}

def get_card_ids():
    with sync_playwright() as p:
        browser = p.chromium.launch(headless=CONFIG["HEADLESS"])
        context = browser.new_context()
        page = context.new_page()

        # Загрузка куков
        if os.path.exists(CONFIG["COOKIES_FILE"]):
            with open(CONFIG["COOKIES_FILE"], "r") as f:
                context.add_cookies(json.load(f))

        # Парсинг карточек
        page.goto(
            urljoin(CONFIG["BASE_URL"], CONFIG["USER_URL"]),
            timeout=CONFIG["TIMEOUT"],
            wait_until="domcontentloaded"
        )

        cards = page.locator(".anime-cards__item[data-id]").all()
        card_ids = [card.get_attribute("data-id") for card in cards]

        with open(CONFIG["OUTPUT_FILE"], "w") as f:
            json.dump({"card_ids": card_ids}, f)

        browser.close()
        print(f"Найдено {len(card_ids)} карточек. ID сохранены в {CONFIG['OUTPUT_FILE']}")

if __name__ == "__main__":
    get_card_ids()