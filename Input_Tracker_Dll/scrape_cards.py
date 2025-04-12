from playwright.sync_api import sync_playwright
import json
import random  # Randow delay
import time  # timestamp in file name

def scrape_cards():
    results = []
    max_cards = 10  # Test mode. only for N card || Delete to full

    with sync_playwright() as p:

        # 1.0. Browser Setting || maybe change to castom login
        browser = p.chromium.launch(headless=False)  # headless=False для отладки
        page = browser.new_page(user_agent="Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/91.0.4472.124 Safari/537.36")
        
        # 1.1. Go to main page
        page.goto("https://asstars.tv/cards/?rank=e")
        page.wait_for_timeout(random.randint(2000, 5000))  # Случайная задержка 2-5 сек.

        # 1.2. Try to find all cards, if not work change selector
        card_links = page.locator("a[href^='/cards/'][href*='/users/']").all()
        
        # 1.3. While useing limeted cards try to get ID
        for link in card_links[:max_cards]:
            card_url = link.get_attribute("href")
            card_id = card_url.split("/")[2]  # Из URL /cards/35176/users/ → 35176
            
            print(f"Парсим карточку {card_id}...")
            
            # 1.4. Go to page where we can find all peoples needed cards
            need_page = browser.new_page(user_agent=page.context.browser.user_agent)  # Тот же user-agent
            need_page.goto(f"https://asstars.tv/cards/{card_id}/users/need/")
            need_page.wait_for_timeout(random.randint(1000, 3000))  # Задержка 1-3 сек.

            # 1.5. Try to get all user names and ID
            users = []
            user_elements = need_page.locator(".profile__friends-item").all()
            for user in user_elements:
                users.append({
                    "username": user.text_content().strip(),
                    "user_id": user.get_attribute("data-user-id")
                })

            # 1.6. Save data result
            results.append({
                "card_id": card_id,
                "users_count": len(user_elements),
                "users": users
            })
            
            need_page.close()
            page.wait_for_timeout(random.randint(1000, 2000))  # Make pause, if don`t whant ban
        
        browser.close()
    
    # Save to JSON with timestamp (need don`t rewrite file, append to file)
    filename = f"cards_data_{int(time.time())}.json"
    with open(filename, "w", encoding="utf-8") as f:
        json.dump(results, f, ensure_ascii=False, indent=2)
    print(f"Готово! Данные в {filename}")

if __name__ == "__main__":
    scrape_cards()