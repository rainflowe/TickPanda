#include<iostream>
#include<cpr/cpr.h>

#include<ctime>
#include<hmac.h>
//#include<sha512.h>

#include "./openssl_util.h"
#include "./libIndax.h"
using namespace std;

/*
g++ -L/home/luw/Documents/libIndax/cpr -I. -std=c++11 *.cpp -o test.o -lcpr -lcurl -lssl -lcrypto
#API Key: 
key = "IJRJJ3JC-5QNFRX6K-LMLCYWT9-FLFLXZPA-4NCKI0Z6"
#Secret Key: 
secret = "9975d2f277de72c1b464af9a418b9ffba990554085c8509f8a3ec8b74f7d11a9092efef331b8ccad"

import time,hmac,hashlib,requests,json
from urllib import urlencode

values = {}

url = 'https://vip.bitcoin.co.id/tapi'
values['method'] = "getInfo"
values['nonce'] = str(int(time.time()))
body = urlencode(values)
signature = hmac.new(secret, body, hashlib.sha512).hexdigest()
headers = {
    'Content-Type': 'application/x-www-form-urlencoded',
    'Key': key,
    'Sign': signature
}

req = requests.post(url,data=values,headers=headers)
json.loads(req.text)

*/

string sha512(std::string input);

int main(){
    string key = "IJRJJ3JC-5QNFRX6K-LMLCYWT9-FLFLXZPA-4NCKI0Z6";
    string secret = "9975d2f277de72c1b464af9a418b9ffba990554085c8509f8a3ec8b74f7d11a9092efef331b8ccad";
    map<string, string> values = {};
    values["method"] = "getInfo";

    std::time_t c_time = std::time(nullptr);
    string s_time = to_string(c_time);
    values["nonce"] = s_time;

    string body = "method=getInfo&nonce=" + s_time;
    libIndax lIn(secret, key);

    //cout << lIn.getTickerC("btc_idr") << endl;
    //cout << lIn.getTradesC("btc_idr") << endl;
    //cout << lIn.getDepthC("btc_idr") << endl;

    cout << lIn.getInfoC() << endl;
    char cont;
    // cout << lIn.transHistoryC() << endl;
    // cin >> cont;
    // cout << lIn.tradeHistoryC("btc_idr") << endl;
    // cin >> cont;
    // cout << lIn.openOrdersC() << endl;
    // cin >> cont;
    // cout << lIn.orderHistoryC("btc_idr") << endl;
    // cin >> cont;
    //cout << lIn.openOrdersC() << endl;
    //cin >> cont;
    cout << lIn.tradeC("btc_idr", "buy", "9000000", "90000") << endl;
    cin >> cont;

    //unsigned char c_body[100] = "nonce=1523538473&method=getInfo";
    //unsigned char c_body[100] = "nonce=1523538473&method=getInfo";
    //unsigned char c_secret[300] = "9975d2f277de72c1b464af9a418b9ffba990554085c8509f8a3ec8b74f7d11a9092efef331b8ccad";
    //unsigned char c_secret[300] = "abc";
    //body.copy((char*)c_body, 100);
    //secret.copy((char*)c_secret, 300);
    //cout << c_body << endl;
    //cout << c_secret << endl;

    //string signature = sha512(body);
    unsigned char out[300];
    
    //HMAC h(c_body, body.size(), c_secret, secret.size(), out);
    //HMAC h(c_body, 3, c_secret, 3, out);
    //string signature = hmac_sha512(secret.c_str(), body.c_str());
    //cout << out[0] << endl;
    //cout << signature << endl;
    
    /*
    url = 'https://vip.bitcoin.co.id/tapi'
    values['method'] = "getInfo"
    values['nonce'] = str(int(time.time()))
    body = urlencode(values)
    signature = hmac.new(secret, body, hashlib.sha512).hexdigest()
    headers = {
        'Content-Type': 'application/x-www-form-urlencoded',
        'Key': key,
        'Sign': signature
    }

    req = requests.post(url,data=values,headers=headers)
    json.loads(req.text)
    */
    // auto r = cpr::Post(cpr::Url{"https://vip.bitcoin.co.id/tapi"},
    //                 cpr::Body{
    //                     body
    //                 },
    //                 cpr::Header{
    //                             {"Content-Type", "application/x-www-form-urlencoded"},
    //                             {"Key", key},
    //                             {"Sign", signature}
    //                             }
    //                 );

    // //cout << r.text << endl;

    // std::cout << r.text << std::endl;

    // cin >> out;
}