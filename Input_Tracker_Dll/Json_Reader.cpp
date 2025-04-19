//---------------------------------------------------------------------------
module;
#include <nlohmann/json.hpp> // Пока не поддерживается в модулях C++20
module Json_Reader;
//---------------------------------------------------------------------------
import <fstream>;
//---------------------------------------------------------------------------
using json = nlohmann::json;
//---------------------------------------------------------------------------

// AJson_Reader
AJson_Reader::AJson_Reader()
 : Card_Data_List()
{

}
//---------------------------------------------------------------------------
bool AJson_Reader::Load_From_File(const std::filesystem::path &file_path)
{
   json json_data;

   if (!std::filesystem::exists(file_path) )
      return false;

   std::ifstream file(file_path);
   if (!file.is_open() )
      return false;

   file >> json_data;
   for (const auto &item : json_data)
   {
      SCardData card;
      card.Card_ID = item["card_id"].get<std::string>();
      card.Lock_ID = item["lock_id"].get<std::string>();
      Card_Data_List.push_back(card);
   }

   return true;
}
//---------------------------------------------------------------------------
const std::vector<AJson_Reader::SCardData> &AJson_Reader::Get_Card_Data() const
{
   return Card_Data_List;
}
//---------------------------------------------------------------------------