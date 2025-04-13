from playwright.sync_api import sync_playwright
import json
import os
from urllib.parse import urljoin

COOKIES_FILE = "auth_cookies.json"

def scrape_all_card_pages():
    with sync_playwright() as p:
        browser = p.chromium.launch(headless=False)
        context = browser.new_context()
        page = context.new_page()

        # Авторизация (как в предыдущем коде)
        if os.path.exists(COOKIES_FILE):
            with open(COOKIES_FILE, "r", encoding="utf-8") as f:
                cookies = json.load(f)
            context.add_cookies(cookies)
            print("Загружены сохраненные куки. Пробуем автоматический вход...")

            page.goto("https://asstars.tv/")
            if "зарегистрированным" not in page.content():
                print("Автоматический вход успешен!")
            else:
                print("Куки устарели. Требуется ручной вход.")
                page.goto("https://asstars.tv/login/")
                input("Войдите вручную и нажмите Enter в консоли...")
                cookies = context.cookies()
                with open(COOKIES_FILE, "w", encoding="utf-8") as f:
                    json.dump(cookies, f, ensure_ascii=False, indent=2)
                print("Новые куки сохранены.")
        else:
            print("Сохраненных куков не найдено. Требуется ручной вход.")
            page.goto("https://asstars.tv/login/")
            input("Войдите вручную и нажмите Enter в консоли...")
            cookies = context.cookies()
            with open(COOKIES_FILE, "w", encoding="utf-8") as f:
                json.dump(cookies, f, ensure_ascii=False, indent=2)
            print("Куки сохранены для будущих сеансов.")

        base_url = "https://asstars.tv"
        card_url = "https://asstars.tv/cards/22824/users/need/"
        all_users = []
        current_page = 1
        max_pages = 100  # Максимальное количество страниц для защиты от бесконечного цикла

        while current_page <= max_pages:
            print(f"Обрабатывается страница {current_page}...")
            page.goto(card_url)

            # Проверка авторизации
            if "зарегистрированным" in page.content():
                print("Ошибка: Требуется авторизация!")
                page.screenshot(path="debug_auth_error.png")
                break

            # Сбор пользователей с текущей страницы
            user_elements = page.locator(".profile__friends-item").all()
            page_users = [
                {
                    "id": user.get_attribute("data-user-id"),
                    "name": user.text_content().strip()
                }
                for user in user_elements
            ]
            all_users.extend(page_users)

            # Проверка пагинации - новый улучшенный способ
            pagination = page.locator("#pagination")
            if not pagination.count():
                print("Пагинация не найдена, завершаем сбор данных.")
                break

            # Получаем текущий номер страницы из активного элемента
            active_page = pagination.locator("span").first.text_content().strip()
            print(f"Текущая страница: {active_page}")

            # Ищем следующую страницу
            next_page_link = None

            # Вариант 1: Кнопка "вперед" (стрелка вправо)
            next_btn = pagination.locator(".pagination__pages-btn a")
            if next_btn.count():
                next_page_link = urljoin(base_url, next_btn.get_attribute("href"))
            else:
                # Вариант 2: Ищем следующую числовую ссылку
                all_links = pagination.locator("a").all()
                current_page_num = int(active_page)

                for link in all_links:
                    href = link.get_attribute("href")
                    if href and "/page/" in href:
                        page_num = int(href.split("/page/")[1].strip("/"))
                        if page_num == current_page_num + 1:
                            next_page_link = urljoin(base_url, href)
                            break

            if not next_page_link:
                print("Достигнута последняя страница, завершаем сбор данных.")
                break

            card_url = next_page_link
            current_page += 1

        # Сохранение результатов
        results = {
            "card_id": 22824,
            "total_pages": current_page - 1,
            "users_count": len(all_users),
            "users": all_users
        }

        with open("card_35176_users_all_pages.json", "w", encoding="utf-8") as f:
            json.dump(results, f, ensure_ascii=False, indent=2)

        browser.close()
        print(f"Успех! Собрано {len(all_users)} пользователей с {current_page-1} страниц.")

if __name__ == "__main__":
    scrape_all_card_pages()