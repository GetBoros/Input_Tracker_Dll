//------------------------------------------------------------------------------------------------------------
module;

export module Tools;
//------------------------------------------------------------------------------------------------------------
export import <sstream>;
import <Windows.h>;
import <filesystem>;
import <regex>;
import <optional>;
import Coroutine;
//------------------------------------------------------------------------------------------------------------
class AValueMonitor
{
public:
	void Check_Value_Async(const int* ValuePtr, int Target)
	{
		CurrentWaiter = AIntWaiter(ValuePtr, Target);
		CurrentWaiter.operator co_await();
	}

private:
	AIntWaiter CurrentWaiter;
};
//------------------------------------------------------------------------------------------------------------
export class AsTools
{
public:
	~AsTools();
	AsTools();

	void Throw();

	// TEMP || If last click don`t save cords, or if need save |
	void Read_Json();
	void Hook_Enable();
	void Hook_Disable();
	void Click_Point_Save();

	std::optional<bool> Compare_Images_Ssim(std::wstring_view old_file, std::wstring_view new_file, double threshold = 0.95);
	void FFmpeg_Make_Screen_Shot();

	bool FFmpeg_Chank_List_Record(wchar_t **file_name_result);
	void FFmpeg_Chank_List_Stop();
	void Curl_Examples();  // !!! TASK Login and create coockies | Run tor bundler and receive our IP for feature examples

	void Clicker_Handler();

	static constexpr int Array_Length = 5;
	static int *Ptr_X_Cords;
	static int *Ptr_Y_Cords;
	static int *Array_X_Cords;
	static int *Array_Y_Cords;

	static HANDLE Fmpeg_Process;
	static HANDLE Fmpeg_Stdin;
	// TEMP END

	static wchar_t *Handle_Clipboard();  // !!! mem leak, need free memory or use smurf pointers

private:
	// !!! Create class what have only import filesystem to handle windows filesystem
	void Get_File_Unique_Name(const wchar_t *text_chank_lists, std::wstring &file_name);
	void Get_Ffmpeg_Cons_Command();
};
//------------------------------------------------------------------------------------------------------------

#pragma region TASKS
/*
	X - Five buttons:
		V - Save chunks
		X - Start recording desktop
		X - Make screen shoots
	X - Maybe move all file to folder if many re_jenna_00 - 99 it`s good
		- how to
	X - Make UI and start big refactoring
	X - Can add button to store .m3u8 url just create button with nick what store those url and when press start record?
	X - Please make every 10 second make screen shoot with ffmpeg thanks

*/
#pragma endregion
