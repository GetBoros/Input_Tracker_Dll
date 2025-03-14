#pragma once

//------------------------------------------------------------------------------------------------------------
#ifdef INPUTTRACKERDLL_EXPORTS
#define DLL_API __declspec(dllexport)
#else
#define DLL_API __declspec(dllimport)
#endif
//------------------------------------------------------------------------------------------------------------
struct SHUD_Settings
{
	int HUD_Width;
	int HUD_Height;
	int X_Cord;
	int Y_Cord;
	wchar_t *Draw_String;
};
//------------------------------------------------------------------------------------------------------------
class DLL_API ATemplate  // Interface api
{
public:
	~ATemplate();
	ATemplate();

	int Init();
	wchar_t *Handle_Clipboard();
	void Enable_Hook(bool is_enable);
	void Get_XY_Cords(int **ptr_x, int **ptr_y);

	int *Temp_Array_Ptr;
	int *Arra_Cords;
};
//------------------------------------------------------------------------------------------------------------

