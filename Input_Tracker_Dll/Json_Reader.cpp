//---------------------------------------------------------------------------
module;
#include <nlohmann/json.hpp> // Пока не поддерживается в модулях C++20
#include <fstream>      // Для std::ifstream
module Json_Reader;
//---------------------------------------------------------------------------




// AJson_Reader
AJson_Reader::AJson_Reader()
 : Card_Data_List()
{

}
//---------------------------------------------------------------------------
bool AJson_Reader::Load_From_File(const std::filesystem::path &file_path)
{
   nlohmann::json json_data;

   if (!std::filesystem::exists(file_path) )
      return false;

   std::ifstream test(file_path);
   if (!test.is_open() )
      return false;

   test >> json_data;
   for (const auto &item : json_data)
   {
      SCard_Data card;
      card.Card_ID = item["card_id"].get<std::string>();
      card.Lock_ID = item["lock_id"].get<std::string>();
      Card_Data_List.push_back(card);
   }
   return true;
}
//---------------------------------------------------------------------------
