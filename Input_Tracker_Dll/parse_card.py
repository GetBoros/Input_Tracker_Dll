from playwright.sync_api import sync_playwright
import json
import os
from urllib.parse import urljoin
import time

# Глобальные настройки (можно легко менять)
CONFIG = {
    "COOKIES_FILE": "auth_cookies.json",  # Файл для сохранения куков
    "BASE_URL": "https://asstars.tv",     # Базовый URL сайта
    "CARD_ID": 31450,                     # ID карточки для парсинга
    "MAX_PAGES": 100,                     # Максимальное количество страниц
    "DELAY": 0.4,                         # Задержка между страницами (в секундах)
    "HEADLESS": True                     # Режим браузера (False - видимый)
}

def scrape_all_card_pages():
    with sync_playwright() as p:
        # Инициализация браузера
        browser = p.chromium.launch(headless=CONFIG["HEADLESS"])
        context = browser.new_context()
        page = context.new_page()

        # Формируем URL карточки
        card_url = f"{CONFIG['BASE_URL']}/cards/{CONFIG['CARD_ID']}/users/need/"

        # Авторизация
        if os.path.exists(CONFIG["COOKIES_FILE"]):
            with open(CONFIG["COOKIES_FILE"], "r", encoding="utf-8") as f:
                cookies = json.load(f)
            context.add_cookies(cookies)
            print("Загружены сохраненные куки. Пробуем автоматический вход...")

            page.goto(CONFIG["BASE_URL"])
            if "зарегистрированным" not in page.content():
                print("Автоматический вход успешен!")
            else:
                print("Куки устарели. Требуется ручной вход.")
                page.goto(f"{CONFIG['BASE_URL']}/login/")
                input("Войдите вручную и нажмите Enter в консоли...")
                with open(CONFIG["COOKIES_FILE"], "w", encoding="utf-8") as f:
                    json.dump(context.cookies(), f, ensure_ascii=False, indent=2)
        else:
            print("Сохраненных куков не найдено. Требуется ручной вход.")
            page.goto(f"{CONFIG['BASE_URL']}/login/")
            input("Войдите вручную и нажмите Enter в консоли...")
            with open(CONFIG["COOKIES_FILE"], "w", encoding="utf-8") as f:
                json.dump(context.cookies(), f, ensure_ascii=False, indent=2)

        # Основной цикл парсинга
        all_users = []
        current_page = 1

        while current_page <= CONFIG["MAX_PAGES"]:
            print(f"Обрабатывается страница {current_page}...")
            page.goto(card_url)

            # Проверка авторизации
            if "зарегистрированным" in page.content():
                print("Ошибка: Требуется авторизация!")
                page.screenshot(path="debug_auth_error.png")
                break

            # Сбор пользователей
            user_elements = page.locator(".profile__friends-item").all()
            all_users.extend({
                "id": user.get_attribute("data-user-id"),
                "name": user.text_content().strip()
            } for user in user_elements)

            # Поиск следующей страницы
            pagination = page.locator("#pagination")
            if not pagination.count():
                break

            next_page_link = None
            active_page = pagination.locator("span").first.text_content()

            # Проверяем кнопку "вперед"
            next_btn = pagination.locator(".pagination__pages-btn a")
            if next_btn.count():
                next_page_link = urljoin(CONFIG["BASE_URL"], next_btn.get_attribute("href"))
            else:
                # Ищем следующую числовую страницу
                for link in pagination.locator("a").all():
                    href = link.get_attribute("href")
                    if href and f"/page/{int(active_page)+1}/" in href:
                        next_page_link = urljoin(CONFIG["BASE_URL"], href)
                        break

            if not next_page_link:
                break

            card_url = next_page_link
            current_page += 1
            time.sleep(CONFIG["DELAY"])  # Задержка между запросами

        # Сохранение результатов
        results = {
            "card_id": CONFIG["CARD_ID"],
            "total_pages": current_page - 1,
            "users_count": len(all_users),
            "users": all_users
        }

        output_file = f"card_{CONFIG['CARD_ID']}_users.json"
        with open(output_file, "w", encoding="utf-8") as f:
            json.dump(results, f, ensure_ascii=False, indent=2)

        browser.close()
        print(f"Готово! Сохранено {len(all_users)} пользователей в {output_file}")

if __name__ == "__main__":
    scrape_all_card_pages()