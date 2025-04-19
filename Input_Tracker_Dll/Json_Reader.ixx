//---------------------------------------------------------------------------
module;
//---------------------------------------------------------------------------
export module Json_Reader;
//---------------------------------------------------------------------------
import <filesystem>;
import <vector>;
import <string>;
//---------------------------------------------------------------------------
export class AJson_Reader
{
public:
   AJson_Reader();
   bool Load_From_File(const std::filesystem::path &file_path);

   struct SCardData
   {
      std::string Card_ID;
      std::string Lock_ID;
   };

   const std::vector<SCardData> &Get_Card_Data() const;

private:
   std::vector<SCardData> Card_Data_List;
};
//---------------------------------------------------------------------------
