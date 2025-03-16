//------------------------------------------------------------------------------------------------------------
module;

module Tools;
//------------------------------------------------------------------------------------------------------------
import <Windows.h>;
//------------------------------------------------------------------------------------------------------------




// Global
HHOOK Hook_Mouse = 0;
//------------------------------------------------------------------------------------------------------------
static LRESULT CALLBACK Hook_Mouse_Proc(int n_code, WPARAM w_param, LPARAM l_param)
{
   if (n_code >= 0)
   {
      if (w_param == WM_LBUTTONDOWN)  // Save mouse cords
      {
         MSLLHOOKSTRUCT *mouse_struct = (MSLLHOOKSTRUCT *)l_param;
         
         if (!AsTools::Array_X_Cords)
         {
            AsTools::Array_X_Cords = new int[AsTools::Array_Length] {};
            AsTools::Array_Y_Cords = new int[AsTools::Array_Length] {};
            AsTools::Ptr_X_Cords = AsTools::Array_X_Cords;
            AsTools::Ptr_Y_Cords = AsTools::Array_Y_Cords;
         }

         *AsTools::Ptr_X_Cords = mouse_struct->pt.x;
         *AsTools::Ptr_Y_Cords = mouse_struct->pt.y;

         AsTools::Ptr_X_Cords++;
         AsTools::Ptr_Y_Cords++;
      }
   }
   return CallNextHookEx(Hook_Mouse, n_code, w_param, l_param);
}
//------------------------------------------------------------------------------------------------------------
void Mouse_Hook_Enable()
{
   if (Hook_Mouse != 0)
      return;
   Hook_Mouse = SetWindowsHookEx(WH_MOUSE_LL, Hook_Mouse_Proc, GetModuleHandle(0), 0);
}
//------------------------------------------------------------------------------------------------------------
void Mouse_Hook_Remove()
{
   if (Hook_Mouse)
      UnhookWindowsHookEx(Hook_Mouse);
   Hook_Mouse = 0;
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
void AsTools::Throw()
{
   return throw 13;
}
//------------------------------------------------------------------------------------------------------------
void AsTools::Enable_Hook()
{
   Mouse_Hook_Enable();
}
//------------------------------------------------------------------------------------------------------------
void AsTools::Disable_Hook()
{
   Mouse_Hook_Remove();
}
//------------------------------------------------------------------------------------------------------------
void AsTools::Array_Clear()
{
   delete[] Array_X_Cords;
   delete[] Array_Y_Cords;
   Ptr_X_Cords = 0;
   Ptr_Y_Cords = 0;
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
