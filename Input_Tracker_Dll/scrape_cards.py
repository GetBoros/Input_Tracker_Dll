# # [numthreads(1, 1, 1)]
# # void main( uint3 DTid : SV_DispatchThreadID )
# # {
# # }

# from playwright.sync_api import sync_playwright
# import json

# def scrape_cards():
#     results = []
    
#     with sync_playwright() as p:
#         browser = p.chromium.launch(headless=False)  # Для дебагу: headless=False
#         page = browser.new_page()
        
#         # 1. Перехід на сторінку з картками
#         page.goto("https://asstars.tv/cards/?rank=e")
        
#         # 2. Очікування завантаження карток (можливо, потрібен скролл)
#         page.wait_for_selector(".card-item")  # Замінити на реальний селектор
        
#         # 3. Збір посилань на картки
#         card_links = page.query_selector_all("a.card-link")  # Замінити селектор
#         for link in card_links:
#             card_url = link.get_attribute("href")
#             card_id = card_url.split("/")[2]  # Наприклад, "/cards/35176/users/" → 35176
            
#             # 4. Перехід на сторінку "потрібно" для картки
#             need_page = browser.new_page()
#             need_page.goto(f"https://asstars.tv/cards/{card_id}/users/need/")
            
#             # 5. Збір користувачів
#             users = []
#             user_elements = need_page.query_selector_all(".profile__friends-item")
#             for user_element in user_elements:
#                 username = user_element.text_content().strip()
#                 user_id = user_element.get_attribute("data-user-id")
#                 users.append({"id": user_id, "username": username})
            
#             # 6. Збереження даних
#             results.append({
#                 "card_id": card_id,
#                 "users_count": len(user_elements),
#                 "users": users
#             })
#             need_page.close()
        
#         browser.close()
    
#     # Зберегти результати у JSON
#     with open("cards_data.json", "w", encoding="utf-8") as f:
#         json.dump(results, f, ensure_ascii=False)

# if __name__ == "__main__":
#     scrape_cards()

from playwright.sync_api import sync_playwright
import json
import random  # Для случайных задержек
import time    # Для timestamp в имени файла

def scrape_cards():
    results = []
    max_cards = 10  # Тестовый режим (10 карточек). Для полного парсинга замените на 3033 или удалите.

    with sync_playwright() as p:
        # Настройка браузера с user-agent
        browser = p.chromium.launch(headless=False)  # headless=False для отладки
        page = browser.new_page(user_agent="Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/91.0.4472.124 Safari/537.36")
        
        # 1. Переход на страницу карточек
        page.goto("https://asstars.tv/cards/?rank=e")
        page.wait_for_timeout(random.randint(2000, 5000))  # Случайная задержка 2-5 сек.

        # 2. Поиск всех карточек (селектор из вашего HTML)
        card_links = page.locator("a[href^='/cards/'][href*='/users/']").all()
        
        # 3. Ограничение количества карточек для теста
        for link in card_links[:max_cards]:
            card_url = link.get_attribute("href")
            card_id = card_url.split("/")[2]  # Из URL /cards/35176/users/ → 35176
            
            print(f"Парсим карточку {card_id}...")
            
            # 4. Переход на страницу "Нужно" с задержкой
            need_page = browser.new_page(user_agent=page.context.browser.user_agent)  # Тот же user-agent
            need_page.goto(f"https://asstars.tv/cards/{card_id}/users/need/")
            need_page.wait_for_timeout(random.randint(1000, 3000))  # Задержка 1-3 сек.

            # 5. Сбор пользователей (ваш HTML-пример)
            users = []
            user_elements = need_page.locator(".profile__friends-item").all()
            for user in user_elements:
                users.append({
                    "username": user.text_content().strip(),
                    "user_id": user.get_attribute("data-user-id")
                })

            # 6. Сохранение данных
            results.append({
                "card_id": card_id,
                "users_count": len(user_elements),
                "users": users  # Можно удалить, если нужен только счетчик
            })
            
            need_page.close()
            page.wait_for_timeout(random.randint(1000, 2000))  # Пауза перед следующей карточкой
        
        browser.close()
    
    # Сохранение в JSON с timestamp (чтобы не перезаписать файл)
    filename = f"cards_data_{int(time.time())}.json"
    with open(filename, "w", encoding="utf-8") as f:
        json.dump(results, f, ensure_ascii=False, indent=2)
    print(f"Готово! Данные в {filename}")

if __name__ == "__main__":
    scrape_cards()