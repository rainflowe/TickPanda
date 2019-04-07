#include<libIndax.h>
#include<cpr/cpr.h>
#include<iostream>
#include<string>
#include<./openssl_util.h>
using namespace std;

libIndax::libIndax(){

}
libIndax::libIndax(string _s, string _k){
    this->secret = _s;
    this->key = _k;
}

libIndax::~libIndax(){

}

string libIndax::getTickerC(string _t){
    string response;
    string _url;
    char _curl[100];

    sprintf(_curl, "https://vip.bitcoin.co.id/api/%s/ticker", _t.c_str());
    _url = string(_curl);

    cout << _url << endl;
    auto r = cpr::Get(cpr::Url{_url});
    return r.text;
}

string libIndax::getTradesC(string _t){
    string response;
    string _url;
    char _curl[100];

    sprintf(_curl, "https://vip.bitcoin.co.id/api/%s/trades", _t.c_str());
    _url = string(_curl);

    cout << _url << endl;
    auto r = cpr::Get(cpr::Url{_url});
    return r.text;
    return response;
}


string libIndax::getDepthC(string _t){
    string response;
    string _url;
    char _curl[100];

    sprintf(_curl, "https://vip.bitcoin.co.id/api/%s/depth", _t.c_str());
    _url = string(_curl);

    cout << _url << endl;
    auto r = cpr::Get(cpr::Url{_url});
    return r.text;
    return response;
}

string libIndax::getInfoC(){
    cout << __FUNCTION__ << endl;
    std::time_t c_time = std::time(nullptr);
    string s_time = to_string(c_time);
    //values["nonce"] = s_time;

    string body = "method=getInfo&nonce=" + s_time;
    
    string signature = hmac_sha512(this->secret.c_str(), body.c_str());
    // cout << body << endl;
    // cout << key << endl;
    // cout << secret << endl;
    // cout << signature << endl;
    
    auto r = cpr::Post(cpr::Url{"https://vip.bitcoin.co.id/tapi"},
                    cpr::Body{
                        body
                    },
                    cpr::Header{
                                {"Content-Type", "application/x-www-form-urlencoded"},
                                {"Key", key},
                                {"Sign", signature}
                                }
                    );

    //cout << r.text << endl;

    std::cout << r.text << std::endl;
    return r.text;
}
string libIndax::transHistoryC(){
    cout << __FUNCTION__ << endl;

    std::time_t c_time = std::time(nullptr);
    string s_time = to_string(c_time);
    //values["nonce"] = s_time;

    string body = "method=transHistory&nonce=" + s_time;
    
    string signature = hmac_sha512(this->secret.c_str(), body.c_str());
    
    auto r = cpr::Post(cpr::Url{"https://vip.bitcoin.co.id/tapi"},
                    cpr::Body{
                        body
                    },
                    cpr::Header{
                                {"Content-Type", "application/x-www-form-urlencoded"},
                                {"Key", key},
                                {"Sign", signature}
                                }
                    );

    //cout << r.text << endl;

    std::cout << r.text << std::endl;
    return r.text;
}

string libIndax::tradeHistoryC(string _pair){
    cout << __FUNCTION__ << endl;
    std::time_t c_time = std::time(nullptr);
    string s_time = to_string(c_time);
    //values["nonce"] = s_time;

    string body = "method=tradeHistory&nonce=" + s_time;
    body = body + "&pair=" + _pair;
    string signature = hmac_sha512(this->secret.c_str(), body.c_str());
    
    auto r = cpr::Post(cpr::Url{"https://vip.bitcoin.co.id/tapi"},
                    cpr::Body{
                        body
                    },
                    cpr::Header{
                                {"Content-Type", "application/x-www-form-urlencoded"},
                                {"Key", key},
                                {"Sign", signature}
                                }
                    );

    //cout << r.text << endl;

    std::cout << r.text << std::endl;
    return r.text;
}

string libIndax::openOrdersC(){
    cout << __FUNCTION__ << endl;
    std::time_t c_time = std::time(nullptr);
    string s_time = to_string(c_time);
    //values["nonce"] = s_time;

    string body = "method=openOrders&nonce=" + s_time;
    //body = body + "&pair=" + _pair;
    string signature = hmac_sha512(this->secret.c_str(), body.c_str());
    
    auto r = cpr::Post(cpr::Url{"https://vip.bitcoin.co.id/tapi"},
                    cpr::Body{
                        body
                    },
                    cpr::Header{
                                {"Content-Type", "application/x-www-form-urlencoded"},
                                {"Key", key},
                                {"Sign", signature}
                                }
                    );

    //cout << r.text << endl;

    std::cout << r.text << std::endl;
    return r.text;
}

string libIndax::orderHistoryC(string _pair){
    cout << __FUNCTION__ << endl;
    std::time_t c_time = std::time(nullptr);
    string s_time = to_string(c_time);
    //values["nonce"] = s_time;

    string body = "method=orderHistory&nonce=" + s_time;
    body = body + "&pair=" + _pair;
    string signature = hmac_sha512(this->secret.c_str(), body.c_str());
    
    auto r = cpr::Post(cpr::Url{"https://vip.bitcoin.co.id/tapi"},
                    cpr::Body{
                        body
                    },
                    cpr::Header{
                                {"Content-Type", "application/x-www-form-urlencoded"},
                                {"Key", key},
                                {"Sign", signature}
                                }
                    );

    //cout << r.text << endl;

    std::cout << r.text << std::endl;
    return r.text;
}

string libIndax::getOrderC(string _pair, string _orderid){
    cout << __FUNCTION__ << endl;
    std::time_t c_time = std::time(nullptr);
    string s_time = to_string(c_time);
    //values["nonce"] = s_time;

    string body = "method=getOrder&nonce=" + s_time;
    body = body + "&pair=" + _pair;
    body = body + "&order_id=" + _orderid;

    string signature = hmac_sha512(this->secret.c_str(), body.c_str());
    
    auto r = cpr::Post(cpr::Url{"https://vip.bitcoin.co.id/tapi"},
                    cpr::Body{
                        body
                    },
                    cpr::Header{
                                {"Content-Type", "application/x-www-form-urlencoded"},
                                {"Key", key},
                                {"Sign", signature}
                                }
                    );

    //cout << r.text << endl;

    std::cout << r.text << std::endl;
    return r.text;
}

int libIndax::tradeC(string _pair, string _type, string _price, string _amount){
    cout << __FUNCTION__ << endl;
    std::time_t c_time = std::time(nullptr);
    string s_time = to_string(c_time);
    //values["nonce"] = s_time;

    string body = "method=trade&nonce=" + s_time;
    body = body + "&pair=" + _pair;
    body = body + "&type=" + _type;
    body = body + "&price=" + _price;
    if(_type=="buy"){
        body = body + "&idr=" + _amount;
    }else{
        body = body + "&btc=" + _amount;
    }

    string signature = hmac_sha512(this->secret.c_str(), body.c_str());
    
    auto r = cpr::Post(cpr::Url{"https://vip.bitcoin.co.id/tapi"},
                    cpr::Body{
                        body
                    },
                    cpr::Header{
                                {"Content-Type", "application/x-www-form-urlencoded"},
                                {"Key", key},
                                {"Sign", signature}
                                }
                    );

    //cout << r.text << endl;

    std::cout << r.text << std::endl;
    
    return 1;
}
int libIndax::cancelOrderC(string _pair, string _orderid, string _type){
    cout << __FUNCTION__ << endl;
    std::time_t c_time = std::time(nullptr);
    string s_time = to_string(c_time);
    //values["nonce"] = s_time;

    string body = "method=cancelOrder&nonce=" + s_time;
    body = body + "&pair=" + _pair;
    body = body + "&order_id=" + _orderid;
    body = body + "&type=" + _type;

    string signature = hmac_sha512(this->secret.c_str(), body.c_str());
    
    auto r = cpr::Post(cpr::Url{"https://vip.bitcoin.co.id/tapi"},
                    cpr::Body{
                        body
                    },
                    cpr::Header{
                                {"Content-Type", "application/x-www-form-urlencoded"},
                                {"Key", key},
                                {"Sign", signature}
                                }
                    );

    //cout << r.text << endl;

    std::cout << r.text << std::endl;
    //return r.text;
    return 1;
}

int libIndax::withdrawCoinC(){
    cout << __FUNCTION__ << endl;
    return 1;
}