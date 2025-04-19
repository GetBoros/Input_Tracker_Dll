//------------------------------------------------------------------------------------------------------------
module;
#include <Windows.h>
#include <curl/curl.h>  // !!! Send to other file
module Tools;
//------------------------------------------------------------------------------------------------------------
import <Windows.h>;
import <array>;
import <thread>;
import <filesystem>;
//------------------------------------------------------------------------------------------------------------
import Json_Reader;
//------------------------------------------------------------------------------------------------------------
   



// Global
HHOOK Hook_Mouse = 0;
//------------------------------------------------------------------------------------------------------------
static long long CALLBACK Hook_Mouse_Proc(int n_code, WPARAM w_param, LPARAM l_param)
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
static size_t WriteCallback(void *contents, size_t size, size_t nmemb, std::string *output)
{
   size_t total_size = size * nmemb;
   output->append( (char *)contents, total_size);
   return total_size;
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
void AsTools::Read_Json()
{
   SCard_Data Card_Data;
   AJson_Reader json_reader;

   std::filesystem::path dir_curr = std::filesystem::current_path();
   std::filesystem::path dir_json = dir_curr / "Json";
   if(!std::filesystem::exists(dir_json) )
      return;
   std::filesystem::path dir_json_file = dir_json / "card_ids.json";
   if (!std::filesystem::exists(dir_json_file) )
      return;

   if (!json_reader.Load_From_File(dir_json_file) )
      return;
   const std::vector<SCard_Data> &card_array = json_reader.Card_Data_List;
}
//------------------------------------------------------------------------------------------------------------//------------------------------------------------------------------------------------------------------------
void AsTools::Hook_Enable()
{
   constexpr int wh_mouse_ll = 14;
   if (Hook_Mouse != 0)
      return;
   Hook_Mouse = SetWindowsHookEx(wh_mouse_ll, Hook_Mouse_Proc, GetModuleHandleW(0), 0);
}
//------------------------------------------------------------------------------------------------------------
void AsTools::Hook_Disable()
{
   delete[] Array_X_Cords;  // !!! Need Check if work good
   delete[] Array_Y_Cords;
   Ptr_X_Cords = 0;
   Ptr_Y_Cords = 0;

   if (Hook_Mouse)
      UnhookWindowsHookEx(Hook_Mouse);
   Hook_Mouse = 0;
}
//------------------------------------------------------------------------------------------------------------
void AsTools::Click_Point_Save()
{
   const int *click_points_array[] { AsTools::Array_X_Cords, AsTools::Array_Y_Cords };
}
//------------------------------------------------------------------------------------------------------------
bool AsTools::FFmpeg_Chank_List_Record(wchar_t **file_name_result)
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
      return false;
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

   if (CreateProcessW(0, cmd.data(), 0, 0, true, 0, 0, 0, &si, &pi) )
   {
      CloseHandle(pi.hThread);  // Close thread handle
      CloseHandle(handle_input_read);  // Close unused read end

      Fmpeg_Process = pi.hProcess;  // Save process handle
      Fmpeg_Stdin = handle_input_write;  // Save write pipe

      std::thread( [&pi]() { WaitForSingleObject(pi.hProcess, INFINITE); } ).detach();
   }
   else
   {
      CloseHandle(handle_input_read);
      CloseHandle(handle_input_write);
      AsTools::Throw();  // Error handling
   }

   delete[] text_chank_lists;

   *file_name_result = new wchar_t[wcslen(file_name.c_str() ) + 1] { };
   wcsncpy_s(*file_name_result, wcslen(file_name.c_str() ) + 1, file_name.c_str(), wcslen(file_name.c_str() ) );
   return true;
}
//------------------------------------------------------------------------------------------------------------
void AsTools::FFmpeg_Chank_List_Stop()
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
void AsTools::Curl_Examples()
{
   int yy = 0;
   CURL *curl = 0;
   CURLcode res;
   std::string response_data;

   // Инициализация libcurl
   curl = curl_easy_init();
   if (curl)
   {
      curl_easy_setopt(curl, CURLOPT_URL, "https://api.ipify.org?format=json");   // Устанавливаем URL для проверки IP (используем api.ipify.org)

      // Указываем Tor-прокси (SOCKS5)
      /*
         // just execute befor
         C:\Tor\Tor\tor.exe
      */
      curl_easy_setopt(curl, CURLOPT_PROXY, "socks5h://127.0.0.1:9050");
      
      curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);  // Устанавливаем callback для записи ответа
      curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_data);

      res = curl_easy_perform(curl);  // Выполняем запрос

      if (res != CURLE_OK)  // Проверяем на ошибки
         yy++;  // error
      else
         response_data;  // receive data

      curl_easy_cleanup(curl);
   }
   else
      yy++;  // error
}
//------------------------------------------------------------------------------------------------------------
void AsTools::Clicker_Handler()
{
   constexpr int delay_ms = 150;  // give site time to response next 150 ms or less? || ( 8 card 150 ) ( 3 card 100)
   constexpr int input_mouse = 0;
   constexpr int mouse_eventf_leftdown = 0x0002;
   constexpr int mouse_eventf_leftup = 0x0004;
   constexpr int vk_control = 0x11;

   INPUT inputs[2] { };

   inputs[0].type = input_mouse;
   inputs[0].mi.dwFlags = mouse_eventf_leftdown;

   inputs[1].type = input_mouse;
   inputs[1].mi.dwFlags = mouse_eventf_leftup;

   auto perform_action = [](int x, int y, INPUT *input_type, size_t input_count, int timer_ms)
      {
         SetCursorPos(x, y);  // Move cursor to cord location
         SendInput(static_cast<UINT>(input_count), input_type, sizeof(INPUT) );
         std::this_thread::sleep_for(std::chrono::milliseconds(timer_ms) );
      };

   auto key_combination = [](const int modifer, const int key)
      {
         return (GetAsyncKeyState(modifer) & 0x8000) && (GetAsyncKeyState(key) & 0x8000);
      };

   while (!key_combination(vk_control, 'Q') )  // if holding ctrl + q
      perform_action(1183, 770, inputs, 2, delay_ms);  // Click to sacrifice card and wait delay_ms
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
