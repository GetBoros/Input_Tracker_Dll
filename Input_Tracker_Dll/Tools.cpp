//------------------------------------------------------------------------------------------------------------
module;

module Tools;
//------------------------------------------------------------------------------------------------------------
import <Windows.h>;
import <sstream>;
import <array>;
import <thread>;
import <filesystem>;
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
HANDLE AsTools::Fmpeg_Process = 0;
HANDLE AsTools::Fmpeg_Stdin = 0;
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
void AsTools::FFmpeg_Command_Run(const wchar_t *str)
{
   // https://edge11-waw.live.mmcdn.com/live-edge/amlst:re_jeena-sd-c21eb72bf628013d5a6e3599fabd4f66ac03d3ebcdf4264b086b061c886994c9_trns_h264/chunklist_w6517537_b5128000_t64RlBTOjMwLjA=.m3u8
   const wchar_t *text_chank_lists = Handle_Clipboard();
   int yy = 0;
   const wchar_t *file_name_starts = wcsstr(text_chank_lists, L"amlst:") + wcslen(L"amlst:");
   const wchar_t *file_name_ending = wcsstr(text_chank_lists, L"-sd-");
   std::wstring_view file_name(file_name_starts, file_name_ending - file_name_starts);

   HANDLE hInputRead = 0;
   HANDLE hInputWrite = 0;
   std::wstring check_file (file_name);
   check_file += +L".mp4";
   std::filesystem::path file_path = check_file;
   if (std::filesystem::exists(file_path) )
      yy++;  // if exist create new name
  
   std::wstring command = L"ffmpeg -i \"" + std::wstring(text_chank_lists) + L"\" -c:v libx264 -preset veryfast -crf 23 -c:a aac -b:a 128k " + check_file;
   SECURITY_ATTRIBUTES sa = { sizeof(sa) };
   STARTUPINFO si = { sizeof(si) };

   sa.bInheritHandle = true;
   CreatePipe(&hInputRead, &hInputWrite, &sa, 0);

   si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
   si.wShowWindow = SW_HIDE;
   si.hStdInput = hInputRead;
   si.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
   si.hStdError = GetStdHandle(STD_ERROR_HANDLE);

   PROCESS_INFORMATION pi;
   std::array<wchar_t, 512> cmd{};

   // !!! TASK Get from clipboard wstring | extract name to save | check if doesn`t exist already save | if exist add _00 or _01

   wcsncpy_s(cmd.data(), cmd.size(), command.c_str(), cmd.size() - 1);

   if (CreateProcessW(0, cmd.data(), 0, 0, true, 0, 0, 0, &si, &pi))
   {
      CloseHandle(pi.hThread);  // Close thread handle
      CloseHandle(hInputRead);  // Close unused read end

      Fmpeg_Process = pi.hProcess;  // Save process handle
      Fmpeg_Stdin = hInputWrite;  // Save write pipe

      std::thread([&pi]() { WaitForSingleObject(pi.hProcess, INFINITE); } ).detach();
   }
   else
   {
      CloseHandle(hInputRead);
      CloseHandle(hInputWrite);
      AsTools::Throw();  // Error handling
   }
}
//------------------------------------------------------------------------------------------------------------
void AsTools::FFmpeg_Stop()
{
   if (Fmpeg_Stdin != 0)  // Explicit check
   {
      DWORD bytesWritten = 0;
      WriteFile(Fmpeg_Stdin, "q\n", 2, &bytesWritten, 0);  // Send 'q' command
      CloseHandle(Fmpeg_Stdin);                             // Close pipe
      Fmpeg_Stdin = 0;                                      // Reset handle
   }

   if (Fmpeg_Process != 0)  // Explicit check
   {
      WaitForSingleObject(Fmpeg_Process, 5000);  // Wait max 5 seconds
      CloseHandle(Fmpeg_Process);                // Force close if still running
      Fmpeg_Process = 0;                         // Reset handle
   }
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
