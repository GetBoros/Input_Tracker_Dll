#pragma once

//------------------------------------------------------------------------------------------------------------
#ifdef INPUTTRACKERDLL_EXPORTS
#define DLL_API __declspec(dllexport)
#else
#define DLL_API __declspec(dllimport)
#endif
//------------------------------------------------------------------------------------------------------------
enum class EEvent_State
{
	EES_Hook_Enable,
	EES_Clicker_Handler,
	EES_Clipboard_Handler,
	EES_Chunk_List_Record_Start,
	EES_Chunk_List_Record_Stop
};
//------------------------------------------------------------------------------------------------------------
class DLL_API AEvent_Handler  // Interface api | Rename
{
public:
	~AEvent_Handler();
	AEvent_Handler();

	bool Update_State(const EEvent_State event_state, wchar_t **file_name);

	int Init();
	wchar_t *Handle_Clipboard();
	void Enable_Hook(bool is_enable);  // !!!
	void Get_XY_Cords(int **ptr_x, int **ptr_y);
	void Reset_Tools();  // !!!
	int *Temp_Array_Ptr;
	int *Arra_Cords;

private:
	void Clicker_Handler();
	bool FFmpeg_Chank_List_Record(wchar_t **file_name);
	void FFmpeg_Chank_List_Stop();


};
//------------------------------------------------------------------------------------------------------------

