//------------------------------------------------------------------------------------------------------------
module;

export module Tools;
//------------------------------------------------------------------------------------------------------------
import <sstream>;
import <Windows.h>;
//------------------------------------------------------------------------------------------------------------



//------------------------------------------------------------------------------------------------------------
export class AsTools
{
public:
	~AsTools();
	AsTools();

	void Throw();

	// TEMP || If last click don`t save cords, or if need save |
	void Enable_Hook();
	void Disable_Hook();
	void FFmpeg_Chank_List_Record();
	void FFmpeg_Stop();

	static void Array_Clear();

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
	X - Maybe move all file to folder if many re_jenna_00 - 99 it`s good
		- how to
	X - Make UI and start big refactoring
	X - Can add button to store .m3u8 url just create button with nick what store those url and when press start record?
	X -
*/
#pragma endregion
