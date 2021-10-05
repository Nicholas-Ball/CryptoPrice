#include <iostream>
#include <fstream>
#include "coin.hpp"
#include "nlohmann/json.hpp"

int main()
{
  Coin c("btc");

  c.Graph();

  std::cout<<"\n";

  return 0;
}