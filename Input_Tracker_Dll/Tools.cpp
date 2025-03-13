//------------------------------------------------------------------------------------------------------------
module;

module Tools;
//------------------------------------------------------------------------------------------------------------
import <Windows.h>;
//------------------------------------------------------------------------------------------------------------




//------------------------------------------------------------------------------------------------------------
HHOOK g_MouseHook = 0;
//------------------------------------------------------------------------------------------------------------
static LRESULT CALLBACK MouseHookProc(int nCode, WPARAM wParam, LPARAM lParam)
{// Обработчик хука
   int yy = 0, temp = 0;

   if (nCode >= 0)
   {
      if (wParam == WM_LBUTTONDOWN)  // Save mouse cords
      {
         if (!AsTools::Array_X_Cords)
         {
            AsTools::Array_X_Cords = new int[5] {};  // !!! free mem
            AsTools::Ptr_X_Cords = AsTools::Array_X_Cords;
            AsTools::Array_Y_Cords = new int[5] {};  // !!! free mem
            AsTools::Ptr_Y_Cords = AsTools::Array_Y_Cords;
         }

         MSLLHOOKSTRUCT *mouse_struct = 0;
         
         mouse_struct = (MSLLHOOKSTRUCT *)lParam;

         // !!! 1.0. Save X_Cord | Need check if not < 5
         *AsTools::Ptr_X_Cords = mouse_struct->pt.x;
         AsTools::Ptr_X_Cords++;

         // 1.1. Save Y_Cord
         *AsTools::Ptr_Y_Cords = mouse_struct->pt.y;
         AsTools::Ptr_Y_Cords++;
      }
   }
   return CallNextHookEx(g_MouseHook, nCode, wParam, lParam);
}
//------------------------------------------------------------------------------------------------------------
void SetGlobalMouseHook()
{// Установка хука

   if (g_MouseHook != 0)
      return;
   g_MouseHook = SetWindowsHookEx(WH_MOUSE_LL, MouseHookProc, GetModuleHandle(0), 0);  // afk
   if (g_MouseHook == nullptr)
      MessageBox(nullptr, L"Failed to set mouse hook!", L"Error", MB_ICONERROR);
}
//------------------------------------------------------------------------------------------------------------
void RemoveGlobalMouseHook()
{// Удаление хука

   if (g_MouseHook)
   {
      UnhookWindowsHookEx(g_MouseHook);
      g_MouseHook = nullptr;
   }
}
//------------------------------------------------------------------------------------------------------------




// AsTools
int *AsTools::Ptr_X_Cords = 0;
int *AsTools::Ptr_Y_Cords = 0;
int *AsTools::Array_X_Cords = 0;
int *AsTools::Array_Y_Cords = 0;
//------------------------------------------------------------------------------------------------------------
AsTools::~AsTools()
{

}
//------------------------------------------------------------------------------------------------------------
AsTools::AsTools()
{

}
//------------------------------------------------------------------------------------------------------------
void AsTools::Enable_Hook()
{
   SetGlobalMouseHook();
}
//------------------------------------------------------------------------------------------------------------
void AsTools::Disable_Hook()
{
   RemoveGlobalMouseHook();
}
//------------------------------------------------------------------------------------------------------------
wchar_t *AsTools::Handle_Clipboard()
{
	int length = 0;
	wchar_t *psz_text = 0;  // !!! mem leak
	wchar_t *copy_text = 0;
	HANDLE handle_data {};

	if (!OpenClipboard(0) )
		return 0;

	handle_data = GetClipboardData(CF_UNICODETEXT);
	if (!handle_data)
   {
      CloseClipboard();
      return 0;
   }

	// 1.1. get ptr at data
	if ( !(psz_text = static_cast<WCHAR *>(GlobalLock(handle_data) ) ) )  // Save to psz_text from clipboard
		return 0;

	length = static_cast<int>(wcslen(psz_text) + 1);
	copy_text = new wchar_t[length];

	wcscpy_s(copy_text, length, psz_text);  // copy from clipboard to copy_text
   GlobalUnlock(handle_data);  // can again use ctrl + c
   CloseClipboard();

   return copy_text;
}
//------------------------------------------------------------------------------------------------------------
