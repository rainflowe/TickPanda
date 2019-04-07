#ifndef KUNGFU_COINPAIRWHITELIST_H
#define KUNGFU_COINPAIRWHITELIST_H

#include <iostream>
#include <unordered_map>
#include <document.h>
#include "../../yijinjing/utils/json.hpp"
#include "../../yijinjing/log/KfLog.h"
#include "WC_DECLARE.h"

WC_NAMESPACE_START

using rapidjson::Document;
using nlohmann::json;


//可以参考kungfu.json.simple

class CoinPairWhiteList {

public:
    void ReadWhiteLists(const json& j_config, std::string whiteLists);
    int Size();

    std::string GetKeyByValue(std::string exchange_coinpair);
    std::string GetValueByKey(std::string strategy_coinpair);
    void Debug_print();

    std::unordered_map<std::string, std::string>& GetKeyIsStrategyCoinpairWhiteList();

private:
    void debug_print(std::unordered_map<std::string, std::string> &keyIsStrategyCoinpairWhiteList);

private:
    //"strategy_coinpair(base_quote)":"exchange_coinpair",
    std::unordered_map<std::string, std::string> keyIsStrategyCoinpairWhiteList;
};

WC_NAMESPACE_END
#endif //KUNGFU_COINPAIRWHITELIST_H
