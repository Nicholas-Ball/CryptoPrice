#include <iostream>
#include <fstream>
#include "coin.hpp"
#include "nlohmann/json.hpp"

int main()
{
  Coin c("btc");
  auto graph = c.GetMarketCapGraph();

  

  /*
  for(int i = 0; i != graph.size();i++)
  {
    std::cout<<graph[i]<<" ";
  }*/

  return 0;
}