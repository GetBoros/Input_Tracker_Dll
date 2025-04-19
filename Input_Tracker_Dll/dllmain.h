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
	EES_Hook_Disable,
	EES_Clicker_Handler,
	EES_Click_Point_Save,
	EES_Clipboard_Handler,
	EES_Chunk_List_Record_Start,
	EES_Chunk_List_Record_Stop,
	EES_Curl_Examples,
	EES_Read_Json_Examples
};
//------------------------------------------------------------------------------------------------------------
class DLL_API AEvent_Handler  // Interface api | Rename
{
public:
	~AEvent_Handler();
	AEvent_Handler();

	bool Update_State(const EEvent_State event_state, wchar_t **file_name);
};
//------------------------------------------------------------------------------------------------------------

