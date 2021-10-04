#include <iostream>
#include <fstream>
#include "coin.hpp"
#include "nlohmann/json.hpp"

int main()
{
    Coin c("btc");

    c.SetCoin("btc");

    return 0;
}