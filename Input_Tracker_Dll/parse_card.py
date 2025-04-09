from playwright.sync_api import sync_playwright
import json

def scrape_single_card():
    with sync_playwright() as p:
        browser = p.chromium.launch(headless=False)  # Stay visible browser || Show me browser
        page = browser.new_page()  # Open new page
        
        # 1.0. Login/Password
        page.goto("https://asstars.tv/login/")  # Autho by myself and press enter in console
        input("Войдите вручную и нажмите Enter в консоли...")
        
        # 1.1. Go to card
        card_url = "https://asstars.tv/cards/35176/users/need/"  # Move to card | Hardcoded
        page.goto(card_url)
        
        # 1.2. Check if seccusses
        if "зарегистрированным" in page.content():  # If something went wrong close app
            print("Ошибка: Требуется авторизация!")
            page.screenshot(path="debug_auth_error.png")
            browser.close()
            return
        
        # 1.3. Store Data
        user_elements = page.locator(".profile__friends-item").all()  # Count row ?
        results = {
            "card_id": 35176,
            "users_count": len(user_elements),
            "users":
            [
                {
                    "id": user.get_attribute("data-user-id"),
                    "name": user.text_content().strip()  # Из <a>DDD</a> → "DDD"
                }
                for user in user_elements
            ]        
        }
        
        # 1.4. Save result to json
        with open("card_35176_users.json", "w", encoding="utf-8") as f:
            json.dump(results, f, ensure_ascii=False, indent=2)
        
        browser.close()
        print("Успех! Данные сохранены.")

if __name__ == "__main__":
    scrape_single_card()