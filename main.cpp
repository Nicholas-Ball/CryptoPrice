#include <iostream>
#include <fstream>
#include "coin.hpp"
#include "nlohmann/json.hpp"

int main()
{
  Coin c("btc");

  std::cout<<c.GetTodaysHigh()<<"\n";

  return 0;
}