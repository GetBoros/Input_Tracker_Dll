//------------------------------------------------------------------------------------------------------------
#include "dllmain.h"
//------------------------------------------------------------------------------------------------------------
import Tools;
//------------------------------------------------------------------------------------------------------------





// AEvent_Handler
AEvent_Handler::~AEvent_Handler()
{

}
//------------------------------------------------------------------------------------------------------------
AEvent_Handler::AEvent_Handler()
{

}
//------------------------------------------------------------------------------------------------------------
bool AEvent_Handler::Update_State(const EEvent_State event_state, wchar_t **file_name)
{
	switch (event_state)
	{
	case EEvent_State::EES_Hook_Enable:
		AsTools().Hook_Enable();
		break;

	case EEvent_State::EES_Hook_Disable:
		AsTools().Hook_Disable();
		break;

	case EEvent_State::EES_Clicker_Handler:
		AsTools().Clicker_Handler();
		break;

	case EEvent_State::EES_Click_Point_Save:
		AsTools().Click_Point_Save();
		break;

	case EEvent_State::EES_Clipboard_Handler:
		break;

	case EEvent_State::EES_Chunk_List_Record_Start:
		return AsTools().FFmpeg_Chank_List_Record(file_name);
		break;

	case EEvent_State::EES_Chunk_List_Record_Stop:
		AsTools().FFmpeg_Chank_List_Stop();
		break;

	case EEvent_State::EES_Curl_Examples:
		AsTools().Curl_Examples();
		break;

	case EEvent_State::EES_Read_Json_Examples:
		AsTools().Read_Json();
		break;

	default:
		AsTools().Throw();
	}

	return true;
}
//------------------------------------------------------------------------------------------------------------
