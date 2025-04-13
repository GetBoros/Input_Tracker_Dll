from playwright.sync_api import sync_playwright
import json
import os

COOKIES_FILE = "auth_cookies.json"

def scrape_single_card():
    with sync_playwright() as p:
        browser = p.chromium.launch(headless=False)
        context = browser.new_context()
        page = context.new_page()

        # 1.0. Попробовать загрузить сохраненные куки
        if os.path.exists(COOKIES_FILE):
            with open(COOKIES_FILE, "r", encoding="utf-8") as f:
                cookies = json.load(f)
            context.add_cookies(cookies)
            print("Загружены сохраненные куки. Пробуем автоматический вход...")

            # Проверяем, работает ли автоматический вход
            page.goto("https://asstars.tv/")
            if "зарегистрированным" not in page.content():
                print("Автоматический вход успешен!")
            else:
                print("Куки устарели. Требуется ручной вход.")
                page.goto("https://asstars.tv/login/")
                input("Войдите вручную и нажмите Enter в консоли...")
                # Сохраняем новые куки после ручного входа
                cookies = context.cookies()
                with open(COOKIES_FILE, "w", encoding="utf-8") as f:
                    json.dump(cookies, f, ensure_ascii=False, indent=2)
                print("Новые куки сохранены.")
        else:
            print("Сохраненных куков не найдено. Требуется ручной вход.")
            page.goto("https://asstars.tv/login/")
            input("Войдите вручную и нажмите Enter в консоли...")
            # Сохраняем куки после первого входа
            cookies = context.cookies()
            with open(COOKIES_FILE, "w", encoding="utf-8") as f:
                json.dump(cookies, f, ensure_ascii=False, indent=2)
            print("Куки сохранены для будущих сеансов.")

        # 1.1. Переход к карточке
        card_url = "https://asstars.tv/cards/35176/users/need/"
        page.goto(card_url)

        # 1.2. Проверка успешности входа
        if "зарегистрированным" in page.content():
            print("Ошибка: Требуется авторизация!")
            page.screenshot(path="debug_auth_error.png")
            browser.close()
            return

        # 1.3. Сбор данных
        user_elements = page.locator(".profile__friends-item").all()
        results = {
            "card_id": 35176,
            "users_count": len(user_elements),
            "users": [
                {
                    "id": user.get_attribute("data-user-id"),
                    "name": user.text_content().strip()
                }
                for user in user_elements
            ]
        }

        # 1.4. Сохранение результатов
        with open("card_35176_users.json", "w", encoding="utf-8") as f:
            json.dump(results, f, ensure_ascii=False, indent=2)

        browser.close()
        print("Успех! Данные сохранены.")

if __name__ == "__main__":
    scrape_single_card()