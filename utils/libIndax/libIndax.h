#include <iostream>
using namespace std;

class libIndax{
public:
    libIndax();
    libIndax(string _k, string _s);
    ~libIndax();

    string getTickerC(string _t);
    string getTradesC(string _t);
    string getDepthC(string _t);
    
    string getInfoC();
    string transHistoryC();
    string tradeHistoryC(string _pair);
    string openOrdersC();
    string orderHistoryC(string _pair);
    string getOrderC(string _pair, string _orderid);

    int tradeC(string _pair, string _type, string _price, string _amount);
    int cancelOrderC(string _pair, string _orderid, string _type);

    int withdrawCoinC();
private:
    string secret;
    string key;
};