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
void AsTools::FFmpeg_Chank_List_Record()
{
   wchar_t *text_chank_lists = 0;
   HANDLE handle_input_read = 0;
   HANDLE handle_input_write = 0;
   std::wstring file_name;
   std::wstring cons_command;
   SECURITY_ATTRIBUTES sa = { sizeof(sa) };
   STARTUPINFO si = { sizeof(si) };

   constexpr int startf_use_show_window = 0x00000001;
   constexpr int startf_use_std_handles = 0x00000100;
   constexpr int sw_hide = 0;
   constexpr int std_output_handle = ( (DWORD) - 11);  // STD_OUTPUT_HANDLE
   constexpr int std_error_handle = ( (DWORD) - 12);  // STD_ERROR_HANDLE

   text_chank_lists = Handle_Clipboard();
   Get_File_Unique_Name(text_chank_lists, file_name);
   if (file_name[0] == 0)  // file name == 0 when it`s not a chat bate url so exit
   {
      delete[] text_chank_lists;
      return;
   }
   cons_command = L"ffmpeg -i \"" + std::wstring(text_chank_lists) + L"\" -c:v libx264 -preset veryfast -crf 23 -c:a aac -b:a 128k " + file_name;

   // Call console command
   sa.bInheritHandle = true;
   CreatePipe(&handle_input_read, &handle_input_write, &sa, 0);

   si.dwFlags = startf_use_show_window | startf_use_std_handles;
   si.wShowWindow = sw_hide;
   si.hStdInput = handle_input_read;
   si.hStdOutput = GetStdHandle(std_output_handle);
   si.hStdError = GetStdHandle(std_error_handle);

   PROCESS_INFORMATION pi;
   std::array<wchar_t, 512> cmd{};

   wcsncpy_s(cmd.data(), cmd.size(), cons_command.c_str(), cmd.size() - 1);

   if (CreateProcessW(0, cmd.data(), 0, 0, true, 0, 0, 0, &si, &pi))
   {
      CloseHandle(pi.hThread);  // Close thread handle
      CloseHandle(handle_input_read);  // Close unused read end

      Fmpeg_Process = pi.hProcess;  // Save process handle
      Fmpeg_Stdin = handle_input_write;  // Save write pipe

      std::thread([&pi]() { WaitForSingleObject(pi.hProcess, INFINITE); } ).detach();
   }
   else
   {
      CloseHandle(handle_input_read);
      CloseHandle(handle_input_write);
      AsTools::Throw();  // Error handling
   }

   delete[] text_chank_lists;
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
void AsTools::Get_File_Unique_Name(const wchar_t *text_chank_lists, std::wstring &file_name)
{
   int index = 0;

   constexpr wchar_t prefix[] = L"amlst:";
   constexpr wchar_t suffix[] = L"-sd-";
   const wchar_t *file_name_starts = 0;
   const wchar_t *file_name_ending = 0;
   std::wstring unique_file_name;

   file_name_starts = wcsstr(text_chank_lists, prefix);
   if (!file_name_starts != 0)
      return;
   file_name_starts += wcslen(prefix);

   file_name_ending = wcsstr(text_chank_lists, suffix);
   if (!file_name_ending != 0)
      return;
   
   file_name.assign(file_name_starts, file_name_ending - file_name_starts);

   do
   {
      unique_file_name = file_name + L"_" + (index < 10 ? L"0" : L"") + std::to_wstring(index) + L".mp4";
      index++;
   } while (std::filesystem::exists(unique_file_name) );

   file_name = unique_file_name;
}
//------------------------------------------------------------------------------------------------------------
void AsTools::Get_Ffmpeg_Cons_Command()
{

}
//------------------------------------------------------------------------------------------------------------
