#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif
#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include "net.hpp"
#include "nlohmann/json.hpp"


//get index of coin name
int NameToIndex(nlohmann::json list,std::string coin)
{
    for(int i =0; i != list.size();i++)
    {
        //if found coin, return index
        if(list[i]["name"] == coin) return i;
    }
    //if can't find coin, return -1
    return -1;
}

//get index of coin symbol
int SymbolToIndex(nlohmann::json list,std::string coin)
{
    for(int i =0; i != list.size();i++)
    {
        //if found coin, return index
        if(list[i]["symbol"] == coin) return i;
    }
    //if can't find coin, return -1
    return -1;
}

//checks if file exits
inline bool exists (const std::string& name) {
  struct stat buffer;   
  return (stat (name.c_str(), &buffer) == 0); 
}

class Coin
{
    private:
        double Price;
        std::string Symbol;

        net n;
    public:

        //refresh CoinList
        void RefreshList()
        {   
            //get cryptocoin list
            n.Get("https://api.coingecko.com/api/v3/coins/list");
            
            //save list to json file
            std::ofstream file("CoinList.json");
            file << n.Response();
        }

        //get json of list of coins
        nlohmann::json CoinList()
        {
            //if coin list file exists, return parse version of it, if not, make the list then return parse json
            if(exists("CoinList.json")) return nlohmann::json::parse(std::ifstream("CoinList.json"));
            else{
                RefreshList();
                return CoinList();
            }
        }

        void SetCoin(std::string symbol)
        {
            auto list = CoinList();
            int index = SymbolToIndex(list,symbol);
            if (index == -1)
            {
                throw "Invaild Symbol";
            }

            //set coin data
            n.Get("https://api.coingecko.com/api/v3/coins/"+list[index]["id"].get<std::string>()+"?localization=false&tickers=false&community_data=false&developer_data=false&sparkline=false");
            auto j = nlohmann::json::parse(n.Response());

            std::cout<<j<<std::endl;

        }  

        //constructor
        Coin(std::string CoinSymbol)
        {
            this->Symbol = CoinSymbol;
            int index = SymbolToIndex(CoinList(),CoinSymbol);
            if(index != -1)
            {

            }
        }
};