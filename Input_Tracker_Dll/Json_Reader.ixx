//---------------------------------------------------------------------------
module;
//---------------------------------------------------------------------------
export module Json_Reader;
//---------------------------------------------------------------------------
import <filesystem>;
import <vector>;
import <string>;
//---------------------------------------------------------------------------
export struct SCard_Data
{
   std::string Card_ID;
   std::string Lock_ID;
};
//---------------------------------------------------------------------------
export class AJson_Reader
{
public:
   AJson_Reader();

   bool Load_From_File(const std::filesystem::path &file_path);

   std::vector<SCard_Data> Card_Data_List;
};
//---------------------------------------------------------------------------
