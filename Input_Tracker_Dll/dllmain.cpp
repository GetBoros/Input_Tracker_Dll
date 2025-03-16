//------------------------------------------------------------------------------------------------------------
#include "dllmain.h"
//------------------------------------------------------------------------------------------------------------
import Tools;


// ATemplate
ATemplate::~ATemplate()
{
	delete[] Arra_Cords;
}
//------------------------------------------------------------------------------------------------------------
ATemplate::ATemplate()
 : Temp_Array_Ptr(0)
{
	Arra_Cords = new int[10] {};
	Temp_Array_Ptr = Arra_Cords;
}
//------------------------------------------------------------------------------------------------------------
int ATemplate::Init()
{
	return 0;
}
//------------------------------------------------------------------------------------------------------------
wchar_t *ATemplate::Handle_Clipboard()
{
	wchar_t *format_wstring = AsTools::Handle_Clipboard();  // !!! must return string with cpy data

	// !!! do something with string

	return format_wstring;
}
//------------------------------------------------------------------------------------------------------------
void ATemplate::Enable_Hook(bool is_enable)
{
	AsTools tools;
	if (is_enable)
		tools.Enable_Hook();
	else
		tools.Disable_Hook();
}
//------------------------------------------------------------------------------------------------------------
void ATemplate::Get_XY_Cords(int **ptr_x, int **ptr_y)
{
	*ptr_x = AsTools::Array_X_Cords;
	*ptr_y = AsTools::Array_Y_Cords;
}
//------------------------------------------------------------------------------------------------------------
void ATemplate::Reset_Tools()
{
	AsTools::Array_Clear();
}
//------------------------------------------------------------------------------------------------------------
