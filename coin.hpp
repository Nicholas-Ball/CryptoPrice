#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif
#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <vector>
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
        if(list[i]["symbol"] == coin) 
          return i;
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
        //information about the coin
        double Price;
        std::string Symbol;
        int MarketCap;
        int MarketCapRank;
        double HighToday;
        double LowToday;

        //graphs
        std::vector<double> PriceGraph;
        std::vector<double> VolumeGraph;
        std::vector<double> MarketCapGraph;
      
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
              throw std::runtime_error("Invaild Symbol");
          }

          //set coin data
          n.Get("https://api.coingecko.com/api/v3/coins/"+list[index]["id"].get<std::string>()+"?localization=false&tickers=false&community_data=false&developer_data=false&sparkline=false");
          auto j = nlohmann::json::parse(n.Response());

          //set parms
          this->Price = j["market_data"]["current_price"]["usd"];
          this->MarketCap = j["market_data"]["market_cap"]["usd"];
          this->MarketCapRank = j["market_data"]["market_cap_rank"];
          this->HighToday = j["market_data"]["high_24h"]["usd"];
          this->LowToday = j["market_data"]["low_24h"]["usd"];
        }


        //price accssor in dollars
        double GetPrice()
        {
          return this->Price;
        }  
        //Market cap accssor in dollars
        double GetMarketCap()
        {
          return this->MarketCap;
        }  
        //Market cap rank accssor
        int GetMarketCapRank()
        {
          return this->MarketCapRank;
        }  
        //Todays high accssor in dollars
        double GetTodaysHigh()
        {
          return this->HighToday;
        }  
        //Today's low accssor in dollars
        double GetTodaysLow()
        {
          return this->LowToday;
        }

        void Graph()
        {
          //get coin history
          this->n.Get("https://api.coingecko.com/api/v3/coins/bitcoin/market_chart?vs_currency=usd&days=365");
          auto j = nlohmann::json::parse(n.Response());

          //add to gragh array in reverse order (current day at index 0)
          for(int i = (j["prices"].size()-1); i != -1;i--)
          {
            this->PriceGraph.push_back(j["prices"][i][1]);
            this->VolumeGraph.push_back(j["total_volumes"][i][1]);
            this->MarketCapGraph.push_back(j["market_caps"][i][1]);
            std::cout<<j["prices"][i][1]<<std::endl;
          }
        }  

        //constructor
        Coin(std::string CoinSymbol)
        {

          this->Symbol = CoinSymbol;
          int index = SymbolToIndex(CoinList(),CoinSymbol);

          if(index != -1)
          {
            SetCoin(CoinSymbol);
          }else{
            throw std::runtime_error("Unknown Coin Symbol \""+CoinSymbol+"\"");
          }
        }

};