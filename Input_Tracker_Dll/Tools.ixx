//------------------------------------------------------------------------------------------------------------
module;

export module Tools;
//------------------------------------------------------------------------------------------------------------
export import <sstream>;
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
	bool FFmpeg_Chank_List_Record(wchar_t **file_name_result);
	void FFmpeg_FFmpeg_Chank_List_Stop();
	void Curl_Examples();  // !!! TASK Login and create coockies | Run tor bundler and receive our IP for feature examples

	void Clicker_Handler();

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
	X - Five buttons:
		V - Save chunks
		X - Start recording desktop
		X - Make screen shoots
	X - Maybe move all file to folder if many re_jenna_00 - 99 it`s good
		- how to
	X - Make UI and start big refactoring
	X - Can add button to store .m3u8 url just create button with nick what store those url and when press start record?
	X -
*/
#pragma endregion

#pragma region TASKS_SAVE_DESTOP
/*
	X - Every 5 seconds save desktops
*/
#pragma endregion


#pragma region FFMPEG
//
/*
	ffplay -hwaccel qsv -i "https://edge6-hel.live.mmcdn.com/live-edge/amlst:sincere_shyness-sd-c1f6b213faeaf4d2aca39c22606c277265abb4fae1a8ac2e8c53deca2294dca5_trns_h264/chunklist_w1074660170_b5128000_t64RlBTOjMwLjA=.m3u8"
	
*/
#pragma endregion


#pragma region FFPROB_TEST_EXAMPLES
// HELP Used to get || scale | bitrate | codec | length | all in file | meta data ||
/*
	
	ffprob input.webp
	ffprobe -v quiet -print_format json -show_format -show_streams input.webp
	ffprobe -v error -select_streams v:0 -show_entries stream=width,height,bit_rate,codec_name -of default=noprint_wrappers=1 ""

*/
#pragma endregion

#pragma region FFPLAY_TEST_EXAMPLES
//
/*
	ffplay -user_agent "Boros" "https://edge6-hel.live.mmcdn.com/live-edge/amlst:sincere_shyness-sd-c1f6b213faeaf4d2aca39c22606c277265abb4fae1a8ac2e8c53deca2294dca5_trns_h264/chunklist_w1074660170_b5128000_t64RlBTOjMwLjA=.m3u8"

*/
#pragma endregion
