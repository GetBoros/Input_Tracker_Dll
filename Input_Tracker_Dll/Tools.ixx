//------------------------------------------------------------------------------------------------------------
module;

export module Tools;
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
	
	static void Array_Clear();

	static constexpr int Array_Length = 5;
	static int *Ptr_X_Cords;
	static int *Ptr_Y_Cords;
	static int *Array_X_Cords;
	static int *Array_Y_Cords;
	// TEMP END

	static wchar_t *Handle_Clipboard();
};
//------------------------------------------------------------------------------------------------------------
