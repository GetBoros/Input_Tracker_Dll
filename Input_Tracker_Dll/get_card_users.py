from playwright.sync_api import sync_playwright
import json
import os
import time
from urllib.parse import urljoin
from collections import OrderedDict

CONFIG = {
    "COOKIES_FILE": "auth_cookies.json",
    "BASE_URL": "https://asstars.tv",
    "INPUT_FILE": "card_ids.json",
    "OUTPUT_FILE": "card_users.json",
    "HEADLESS": True,
    "DELAY": 0.3,
    "TIMEOUT": 15000
}

def get_users_count():
    print("🚀 Начало работы скрипта")
    with sync_playwright() as p:
        # Инициализация браузера
        print("🛠️ Запуск браузера...")
        browser = p.chromium.launch(headless=CONFIG["HEADLESS"])
        context = browser.new_context()
        page = context.new_page()

        # Загрузка куков
        if os.path.exists(CONFIG["COOKIES_FILE"]):
            print("🔑 Загрузка сохраненных куков...")
            with open(CONFIG["COOKIES_FILE"], "r") as f:
                context.add_cookies(json.load(f))

        # Загрузка ID карточек
        print("📂 Чтение файла с ID карточек...")
        with open(CONFIG["INPUT_FILE"], "r") as f:
            card_ids = json.load(f)["card_ids"]
        
        total_cards = len(card_ids)
        print(f"🔢 Найдено {total_cards} карточек для обработки")

        result = {}
        
        for i, card_id in enumerate(card_ids, 1):
            try:
                print(f"\n🔄 Обработка карточки {i}/{total_cards} (ID: {card_id})")
                url = urljoin(CONFIG["BASE_URL"], f"/cards/{card_id}/users/need/")
                
                print(f"🌐 Переход на страницу: {url}")
                page.goto(url, timeout=CONFIG["TIMEOUT"], wait_until="domcontentloaded")
                
                users = page.locator(".profile__friends-item").all()
                user_count = len(users)
                result[card_id] = user_count
                print(f"👥 Найдено пользователей: {user_count}")
                
                time.sleep(CONFIG["DELAY"])
            
            except Exception as e:
                print(f"❌ Ошибка при обработке карточки {card_id}: {str(e)}")
                result[card_id] = -1  # Помечаем ошибку

        # Сортировка результатов (по убыванию количества пользователей)
        print("\n🔃 Сортировка результатов...")
        sorted_result = OrderedDict(
            sorted(result.items(), key=lambda item: item[1], reverse=True)
        )

        # Сохранение результатов
        with open(CONFIG["OUTPUT_FILE"], "w") as f:
            json.dump(sorted_result, f, indent=2)
        print(f"💾 Результаты сохранены в {CONFIG['OUTPUT_FILE']}")

        browser.close()
        print("✅ Работа скрипта завершена")

if __name__ == "__main__":
    get_users_count()