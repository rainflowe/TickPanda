#include <document.h>
#include <iostream>
#include <string>
#include <sstream>
#include <stdio.h>
#include <assert.h>
#include <string>
#include <cpr/cpr.h>

#include "../../utils/crypto/openssl_util.h"
#include "../../longfist/longfist/LFDataStruct.h"
#include "../../longfist/longfist/LFConstants.h"
#include "../../longfist/longfist/LFPrintUtils.h"


using cpr::Get;
using cpr::Url;
using cpr::Parameters;
using cpr::Payload;
using cpr::Post;

using rapidjson::Document;
using rapidjson::SizeType;
using rapidjson::Value;
using std::string;
using std::to_string;
using std::stod;
using std::stoi;
using utils::crypto::hmac_sha256;

namespace {
  
  std::string GetResponse(const std::string& symbol, int limit) 
  {
    const auto static url = "https://api.binance.com/api/v1/depth";
    const auto response = Get(Url{url}, Parameters{{"symbol", symbol},
                                                        {"limit",  to_string(limit)}});
    return response.text;
  }
  
  std::string GetTradeResponse(const std::string& symbol, int limit) 
  {
    const auto static url = "https://api.binance.com/api/v1/trades";
    const auto response = Get(Url{url}, Parameters{{"symbol", symbol},
                                                        {"limit",  to_string(limit)}});
    return response.text;
  }

  void ParseResponse(const std::string& response) {
    Document d;
    d.Parse(response.c_str());

    LFMarketDataField* lf_md = new LFMarketDataField;
    lf_md->BidPrice1 = stod(d["bids"].GetArray()[0][0].GetString());
    lf_md->BidVolume1 = stoi(d["bids"].GetArray()[0][1].GetString());
    lf_md->BidPrice2 = stod(d["bids"].GetArray()[1][0].GetString());
    lf_md->BidVolume2 = stoi(d["bids"].GetArray()[1][1].GetString());
    lf_md->BidPrice3 = stod(d["bids"].GetArray()[2][0].GetString());
    lf_md->BidVolume3 = stoi(d["bids"].GetArray()[2][1].GetString());
    lf_md->BidPrice4 = stod(d["bids"].GetArray()[3][0].GetString());
    lf_md->BidVolume4 = stoi(d["bids"].GetArray()[3][1].GetString());
    lf_md->BidPrice5 = stod(d["bids"].GetArray()[4][0].GetString());
    lf_md->BidVolume5 = stoi(d["bids"].GetArray()[4][1].GetString());

    lf_md->AskPrice1 = stod(d["asks"].GetArray()[0][0].GetString());
    lf_md->AskVolume1 = stoi(d["asks"].GetArray()[0][1].GetString());
    lf_md->AskPrice2 = stod(d["asks"].GetArray()[1][0].GetString());
    lf_md->AskVolume2 = stoi(d["asks"].GetArray()[1][1].GetString());
    lf_md->AskPrice3 = stod(d["asks"].GetArray()[2][0].GetString());
    lf_md->AskVolume3 = stoi(d["asks"].GetArray()[2][1].GetString());
    lf_md->AskPrice4 = stod(d["asks"].GetArray()[3][0].GetString());
    lf_md->AskVolume4 = stoi(d["asks"].GetArray()[3][1].GetString());
    lf_md->AskPrice5 = stod(d["asks"].GetArray()[4][0].GetString());
    lf_md->AskVolume5 = stoi(d["asks"].GetArray()[4][1].GetString());

    std::cout << PRINT_MD(lf_md);
  }
} /* TestMarketData */

void TestMarketData(const std::string& symbol, int limit) {
  std::cout << "---------------------------------------------------\n";
  std::cout << "Test get order book, endpoint: api/v1/depth, symbol: " << symbol
    << ", limit: " << limit << std::endl;

  const auto response = GetResponse(symbol, limit); 
  /*
  const auto response = "{\"lastUpdateId\":67625893,\"bids\":[[\"0.01700000\",\"1.96000000\",[]],[\"0.01697200\",\"9.61000000\",[]],[\"0.01697100\",\"193.73000000\",[]],[\"0.01697000\",\"40.79000000\",[]],[\"0.01696700\",\"2.93000000\",[]]],\"asks\":[[\"0.01700100\",\"2.00000000\",[]],[\"0.01700400\",\"0.80000000\",[]],[\"0.01700500\",\"0.30000000\",[]],[\"0.01700900\",\"2.61000000\",[]],[\"0.01701100\",\"2.06000000\",[]]]}";
  */

  std::cout << "raw response: " << response << std::endl;

  ParseResponse(response);
}

void TestTrade(const std::string& symbol, int limit) {
  std::cout << "---------------------------------------------------\n";
  std::cout << "Test get order book, endpoint: api/v1/trades, symbol: " << symbol
    << ", limit: " << limit << std::endl;

  const auto response = GetTradeResponse(symbol, limit); 
  Document d;
  d.Parse(response.c_str());
  
  if(d.IsArray())
  {
    for(int i = 0; i < d.Size(); ++i)
    {
    	const auto& ele = d[i];
	if(ele.HasMember("id") && ele.HasMember("price") && ele.HasMember("qty"))
        {
	    std::cout << "found a trade: ";
	    if(ele["id"].IsInt64())
            {
                std::cout << ele["id"].GetInt64();
            }
 	    
	    if(ele["price"].IsString())
		std::cout << " " << ele["price"].GetString();
		
	    if(ele["qty"].IsString())
		std::cout << " " << ele["qty"].GetString(); 

            std::cout << std::endl;
        }
    }
  }
  std::cout << "raw trade response: " << response << std::endl;
}


namespace {

  std::string GetSide(const LfDirectionType& input) {
    if (LF_CHAR_Buy == input) {
      return "BUY";
    } else if (LF_CHAR_Sell == input) {
      return "SELL";
    } else {
      return "UNKNOWN";
    }
  }

  std::string GetType(const LfOrderPriceTypeType& input) {
    if (LF_CHAR_LimitPrice == input) {
      return "LIMIT";
    } else if (LF_CHAR_AnyPrice == input) {
      return "MARKET";
    } else {
      return "UNKNOWN";
    }
  }

  std::string GetTimeInForce(const LfTimeConditionType& input) {
    if (LF_CHAR_IOC == input) {
      return "IOC";
    } else if (LF_CHAR_GTC == input) {
      return "GTC";
    } else if (LF_CHAR_FOK == input) {
      return "FOK";
    } else {
      return "UNKNOWN";
    }
  }

  std::string GetNewOrderData(const LFInputOrderField* order) {
    std::stringstream ss;
    ss << "symbol=" << order->InstrumentID << "&";
    ss << "side=" << GetSide(order->Direction) << "&";
    ss << "type=" << GetType(order->OrderPriceType) << "&";
    ss << "timeInForce=" << GetTimeInForce(order->TimeCondition) << "&";
    ss << "quantity=" << order->Volume << "&";
    ss << "price=" << order->LimitPrice << "&";
    ss << "recvWindow=5000&timestamp=1499827319559";
    return ss.str();
  }

 
  void PostOrder(const std::string& order_data) 
  {
    const auto static url = "https://api.binance.com/api/v3/order";
    const auto response = Post(url, Payload{});
    /*return response.text;*/
  }

} /* TestNewOrder */

std::string
TranslateNewOrder(const LFInputOrderField* order, const std::string& secret) {
  std::cout << "---------------------------------------------------\n";
  std::cout << "Test translate new order with secret: " << secret << std::endl;
  std::cout << PRINT_ORDER(order);

  std::string data = GetNewOrderData(order);
  std::string signature = hmac_sha256(secret.c_str(), data.c_str());
  std::string translated = data + "&signature=" + signature;

  std::cout << "Translate order data is: " << translated << std::endl;

  /* send order out via rest api */
  return "{\n"
    "  \"symbol\": \"LTCBTC\",\n"
    "  \"orderId\": 28,\n"
    "  \"clientOrderId\": \"6gCrw2kRUAF9CvJDGP16IP\",\n"
    "  \"transactTime\": 1507725176595\n"
    "}";
}

void TranslateOrderAck(const std::string& order_ack,
    LFRtnOrderField* translated_order_ack) {
  Document d;
  d.Parse(order_ack.c_str());

  const std::string symbol = d["symbol"].GetString();
  memcpy(translated_order_ack->InstrumentID, symbol.c_str(), symbol.size());

  const std::string order_id = std::to_string(d["orderId"].GetInt());
  memcpy(translated_order_ack->OrderRef, order_id.c_str(), order_id.size());
}

void TranslateOrderExec(const std::string& order_exec,
    LFRtnOrderField* translated_order_exec) {
  Document d;
  d.Parse(order_exec.c_str());

  const std::string symbol = d["symbol"].GetString();
  memcpy(translated_order_exec->InstrumentID, symbol.c_str(), symbol.size());

  const std::string order_id = std::to_string(d["orderId"].GetInt());
  memcpy(translated_order_exec->OrderRef, order_id.c_str(), order_id.size());

  /* TODO: fill the remaining details. */
}

int main() {
  const std::string symbol = "BTCUSDT";
  const int limit = 5;

  /* test market data */
  TestMarketData(symbol, limit);

  TestTrade(symbol, limit);

  /* test sample new order */
  LFInputOrderField* sample_new = new LFInputOrderField();
  memcpy(sample_new->InstrumentID, symbol.c_str(), symbol.size());
  sample_new->Direction = LF_CHAR_Buy;
  sample_new->OrderPriceType = LF_CHAR_LimitPrice;
  sample_new->TimeCondition = LF_CHAR_GTC;
  sample_new->Volume = 1;
  sample_new->LimitPrice = 0.1;
  const std::string order_ack = TranslateNewOrder(sample_new,
      "NhqPtmdSJYdKjVHjA7PZj4Mge3R5YNiP1e3UZjInClVN65XAbvqqM6A7H5fATj0j");

  /* translate order ack */
  LFRtnOrderField* translated_order_ack = new LFRtnOrderField();
  TranslateOrderAck(order_ack, translated_order_ack);

  /* translate order exec */
  const std::string order_exec = "{\n"
    "  \"symbol\": \"BTCUSDT\",\n"
    "  \"orderId\": 28,\n"
    "  \"clientOrderId\": \"6gCrw2kRUAF9CvJDGP16IP\",\n"
    "  \"transactTime\": 1507725176595,\n"
    "  \"price\": \"0.00000000\",\n"
    "  \"origQty\": \"10.00000000\",\n"
    "  \"executedQty\": \"10.00000000\",\n"
    "  \"status\": \"FILLED\",\n"
    "  \"timeInForce\": \"GTC\",\n"
    "  \"type\": \"MARKET\",\n"
    "  \"side\": \"SELL\",\n"
    "  \"fills\": [\n"
    "    {\n"
    "      \"price\": \"4000.00000000\",\n"
    "      \"qty\": \"1.00000000\",\n"
    "      \"commission\": \"4.00000000\",\n"
    "      \"commissionAsset\": \"USDT\"\n"
    "    },\n"
    "    {\n"
    "      \"price\": \"3999.00000000\",\n"
    "      \"qty\": \"5.00000000\",\n"
    "      \"commission\": \"19.99500000\",\n"
    "      \"commissionAsset\": \"USDT\"\n"
    "    },\n"
    "    {\n"
    "      \"price\": \"3998.00000000\",\n"
    "      \"qty\": \"2.00000000\",\n"
    "      \"commission\": \"7.99600000\",\n"
    "      \"commissionAsset\": \"USDT\"\n"
    "    },\n"
    "    {\n"
    "      \"price\": \"3997.00000000\",\n"
    "      \"qty\": \"1.00000000\",\n"
    "      \"commission\": \"3.99700000\",\n"
    "      \"commissionAsset\": \"USDT\"\n"
    "    },\n"
    "    {\n"
    "      \"price\": \"3995.00000000\",\n"
    "      \"qty\": \"1.00000000\",\n"
    "      \"commission\": \"3.99500000\",\n"
    "      \"commissionAsset\": \"USDT\"\n"
    "    }\n"
    "  ]\n"
    "}";
  LFRtnOrderField* translated_order_exec = new LFRtnOrderField();
  TranslateOrderExec(order_exec, translated_order_exec);

  return 0;
}


