import os
import json
import traceback
from playwright.sync_api import sync_playwright

COOKIES_FILE = "cookies.json"
URL = "https://asstars.tv/cards_remelt/"

def save_cookies (context):
    cookies = context.cookies ()
    with open (COOKIES_FILE, "w", encoding="utf-8") as f:
        json.dump (cookies, f, indent=4)

def load_cookies (context):
    if not os.path.exists (COOKIES_FILE):
        return False
    with open (COOKIES_FILE, "r", encoding="utf-8") as f:
        cookies = json.load (f)
        context.add_cookies (cookies)
    return True

def open_browser_and_click_card ():
    with sync_playwright () as p:
        browser = p.chromium.launch (headless=False)
        context = browser.new_context ()
        page = context.new_page ()
        
        if load_cookies (context):
            print ("✅ Загружены cookies")
            page.goto (URL)
        else:
            print ("🟡 Cookies не найдены — открой браузер и залогинься вручную")
            page.goto ("https://asstars.tv/login/")
            page.wait_for_timeout (60000)
            save_cookies (context)
            print ("✅ Cookies сохранены")
            page.goto (URL)

        page.wait_for_selector (".remelt__inventory-item", timeout=15000)
        cards = page.query_selector_all (".remelt__inventory-item")

        if len (cards) > 0:
            print (f"✅ Найдена {len(cards)} карта. Кликаем первую.")
            cards[0].click ()
        else:
            print ("❌ Карточки не найдены.")

        print ("🕒 Ожидаем закрытие окна вручную.")
        input ("Нажми Enter чтобы закрыть браузер...")

        browser.close ()

try:
    print ("[Login.py] Запуск скрипта...")
    open_browser_and_click_card ()
except Exception as e:
    with open ("error_log.txt", "w", encoding="utf-8") as f:
        traceback.print_exc (file=f)
