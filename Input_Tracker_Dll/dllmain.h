#pragma once

//------------------------------------------------------------------------------------------------------------
#ifdef INPUTTRACKERDLL_EXPORTS
#define DLL_API __declspec(dllexport)
#else
#define DLL_API __declspec(dllimport)
#endif
//------------------------------------------------------------------------------------------------------------
class DLL_API AEvent_Handler  // Interface api | Rename
{
public:
	~AEvent_Handler();
	AEvent_Handler();

	int Init();
	wchar_t *Handle_Clipboard();
	void Enable_Hook(bool is_enable);  // !!!
	void Get_XY_Cords(int **ptr_x, int **ptr_y);
	void Reset_Tools();  // !!!
	void FFmpeg_Chank_List_Record(bool &is_record);
	void FFmpeg_Chank_List_Stop();
	void Clicker();

	int *Temp_Array_Ptr;
	int *Arra_Cords;
};
//------------------------------------------------------------------------------------------------------------

