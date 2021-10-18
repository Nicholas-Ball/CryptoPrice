#include <iostream>
#include <fstream>
#include "../crypto/coin.hpp"
#include "../crypto/nlohmann/json.hpp"

int main()
{
  Coin c("btc");
  Coin x("xmr");

  auto graph = c.GetMarketCapGraph();

  std::cout<<"Low: "<<x.GetTodaysLow()<<std::endl;
  std::cout<<"High: "<<x.GetTodaysHigh()<<std::endl;
  
  return 0;
}