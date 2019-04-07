

#include <WC_DECLARE.h>
#include <KfLog.h>
#include "CoinPairWhiteList.h"



WC_NAMESPACE_START

        void CoinPairWhiteList::ReadWhiteLists(const json& j_config, std::string whiteListDefination)
        {
            std::cout << "[readWhiteLists]"<< std::endl;

            if(j_config.find(whiteListDefination) != j_config.end()) {
                std::cout << "[readWhiteLists] found whiteLists"<< std::endl;
                //has whiteLists
                json whiteLists = j_config[whiteListDefination].get<json>();
                if(whiteLists.is_object())
                {
                    for (json::iterator it = whiteLists.begin(); it != whiteLists.end(); ++it)
                    {
                        std::string strategy_coinpair = it.key();
                        std::string exchange_coinpair = it.value();
                        std::cout <<  "[readWhiteLists] (strategy_coinpair) " << strategy_coinpair << " (exchange_coinpair) " << exchange_coinpair<< std::endl;
                        keyIsStrategyCoinpairWhiteList.insert(std::pair<std::string, std::string>(strategy_coinpair, exchange_coinpair));
                    }
                }
            }
        }

        int CoinPairWhiteList::Size()
        {
            return keyIsStrategyCoinpairWhiteList.size();
        }

        void CoinPairWhiteList::Debug_print()
        {
            debug_print(keyIsStrategyCoinpairWhiteList);
        }

        void CoinPairWhiteList::debug_print(std::unordered_map<std::string, std::string> &keyIsStrategyCoinpairWhiteList)
        {
            std::unordered_map<std::string, std::string>::iterator map_itr;
            map_itr = keyIsStrategyCoinpairWhiteList.begin();
            std::cout << "[debug_print] keyIsExchangeSideWhiteList (count) " << keyIsStrategyCoinpairWhiteList.size() << std::endl;
            while(map_itr != keyIsStrategyCoinpairWhiteList.end())
            {
                std::cout << "[debug_print] keyIsExchangeSideWhiteList (strategy_coinpair) " << map_itr->first << " (md_coinpair) "<< map_itr->second << std::endl;
                map_itr++;
            }
        }

        std::string CoinPairWhiteList::GetKeyByValue(std::string exchange_coinpair)
        {
            std::unordered_map<std::string, std::string>::iterator map_itr;
            map_itr = keyIsStrategyCoinpairWhiteList.begin();
            while(map_itr != keyIsStrategyCoinpairWhiteList.end())
            {
                //TODO  std::string == std::string 与 strcmp(str.c_str, s.c_str()) == 0 谁更快
                if(exchange_coinpair == map_itr->second)
                {
//                    std::cout << "[GetKeyByValue] found (strategy_coinpair) " <<
//                              map_itr->first << " (exchange_coinpair) " << map_itr->second << std::endl;

                    return map_itr->first;
                }
                map_itr++;
            }
//            std::cout << "[getWhiteListCoinpairFrom] not found (exchange_coinpair) " << exchange_coinpair << std::endl;
            return "";
        }


        std::string CoinPairWhiteList::GetValueByKey(std::string strategy_coinpair)
        {
            std::unordered_map<std::string, std::string>::iterator map_itr;
            map_itr = keyIsStrategyCoinpairWhiteList.begin();
            while(map_itr != keyIsStrategyCoinpairWhiteList.end())
            {
                if(strategy_coinpair == map_itr->first)
                {
//                    std::cout << "[GetValueByKey] found (strategy_coinpair) " <<
//                              map_itr->first << " (exchange_coinpair) " << map_itr->second << std::endl;

                    return map_itr->second;
                }
                map_itr++;
            }
//            std::cout <<  "[GetValueByKey] not found (strategy_coinpair) " << strategy_coinpair << std::endl;
            return "";
        }


        std::unordered_map<std::string, std::string>& CoinPairWhiteList::GetKeyIsStrategyCoinpairWhiteList()
        {
            return keyIsStrategyCoinpairWhiteList;
        }


WC_NAMESPACE_END