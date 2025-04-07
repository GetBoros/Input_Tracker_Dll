//------------------------------------------------------------------------------------------------------------
#include "dllmain.h"
//------------------------------------------------------------------------------------------------------------
import Tools;
//------------------------------------------------------------------------------------------------------------





// AEvent_Handler
AEvent_Handler::~AEvent_Handler()
{
	delete[] Arra_Cords;
}
//------------------------------------------------------------------------------------------------------------
AEvent_Handler::AEvent_Handler()
 : Temp_Array_Ptr(0)
{
	Arra_Cords = new int[10] {};
	Temp_Array_Ptr = Arra_Cords;
}
//------------------------------------------------------------------------------------------------------------
bool AEvent_Handler::Update_State(const EEvent_State event_state, wchar_t **file_name)
{

	switch (event_state)
	{
	case EEvent_State::EES_Hook_Enable:
		break;

	case EEvent_State::EES_Clicker_Handler:
		Clicker_Handler();
		break;

	case EEvent_State::EES_Clipboard_Handler:
		break;

	case EEvent_State::EES_Chunk_List_Record_Start:
		return FFmpeg_Chank_List_Record(file_name);
		break;

	case EEvent_State::EES_Chunk_List_Record_Stop:
		FFmpeg_Chank_List_Stop();
		break;

	case EEvent_State::EES_Curl_Examples:
		Curl_Example();
		break;

	default:
		break;
	}

	return true;
}
//------------------------------------------------------------------------------------------------------------
int AEvent_Handler::Init()
{
	return 0;
}
//------------------------------------------------------------------------------------------------------------
wchar_t *AEvent_Handler::Handle_Clipboard()
{
	wchar_t *format_wstring = AsTools::Handle_Clipboard();  // !!! must return string with cpy data

	// !!! do something with string

	return format_wstring;
}
//------------------------------------------------------------------------------------------------------------
void AEvent_Handler::Enable_Hook(bool is_enable)
{
	AsTools tools;
	if (is_enable)
		tools.Enable_Hook();
	else
		tools.Disable_Hook();
}
//------------------------------------------------------------------------------------------------------------
void AEvent_Handler::Get_XY_Cords(int **ptr_x, int **ptr_y)
{
	*ptr_x = AsTools::Array_X_Cords;
	*ptr_y = AsTools::Array_Y_Cords;
}
//------------------------------------------------------------------------------------------------------------
void AEvent_Handler::Reset_Tools()
{
	AsTools::Array_Clear();
}
//------------------------------------------------------------------------------------------------------------
bool AEvent_Handler::FFmpeg_Chank_List_Record(wchar_t **file_name)
{
	return AsTools().FFmpeg_Chank_List_Record(file_name);
}
//------------------------------------------------------------------------------------------------------------
void AEvent_Handler::FFmpeg_Chank_List_Stop()
{
	AsTools tools;
	tools.FFmpeg_FFmpeg_Chank_List_Stop();
}
//------------------------------------------------------------------------------------------------------------
void AEvent_Handler::Curl_Example()
{
	AsTools().Curl_Examples();
}
//------------------------------------------------------------------------------------------------------------
void AEvent_Handler::Clicker_Handler()
{
	AsTools().Clicker_Handler();
}
//------------------------------------------------------------------------------------------------------------
