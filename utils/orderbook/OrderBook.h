#include <cstdint>
#include <list>
#include <map>

using namespace std;

/* a price level in order book */
class OrderBookPriceLevel {
public:	
    OrderBookPriceLevel(int64_t price, uint64_t quantity, uint64_t id = 0)
    {
        _price = price;
	_quantity = quantity;
	if(id != 0) { _id = id; }
    }

    int64_t GetPrice() { return _price; }

    uint64_t GetQuantity() { return _quantity; }

    uint64_t GetId() { return _id; }

    void SetPrice(int64_t price) { _price = price; }

    void SetQuantity(uint64_t quantity) { _quantity = quantity; }

    void AddQuantity(uint64_t quantity) { _quantity += quantity; }

    void SetId(uint64_t id) { _id = id; }

private:
    int64_t _price;
    uint64_t _quantity;
    /* optional */
    uint64_t _id = 0;
};

/* data structure used to pass order book info to trading system */
template <int MAX_LEVELS>
class MarketData {
public:
    MarketData(int levels, list<OrderBookPriceLevel*> bids, list<OrderBookPriceLevel*> asks)
    {
        _levelsUsed = levels;

	int currLevel = 0;
       	for(OrderBookPriceLevel* bid : bids) { _bids[currLevel++] = bid; }
       	currLevel = 0;
	for(OrderBookPriceLevel* ask : asks) { _asks[currLevel++] = ask; }
    }

private:	
    char	         _tradingDay[13];	 //交易日
    char  	         _instrumentID[31];      //合约代码
    char   	         _exchangeID[9];         //交易所代码
    char  	         _exchangeInstID[64];    //合约在交易所的代码
    int64_t  		 _lastPrice;             //最新价
    int64_t  		 _preSettlementPrice;    //上次结算价
    int64_t  		 _preClosePrice;         //昨收盘
    int64_t  		 _preOpenInterest;       //昨持仓量
    int64_t              _openPrice;             //今开盘
    int64_t              _highestPrice;          //最高价
    int64_t  	         _lowestPrice;           //最低价
    uint64_t 		 _volume;                //数量
    int64_t  		 _turnover;              //成交金额
    int64_t  		 _openInterest;          //持仓量
    int64_t  		 _closePrice;            //今收盘
    int64_t  		 _settlementPrice;       //本次结算价
    int64_t  		 _upperLimitPrice;       //涨停板价
    int64_t  	         _lowerLimitPrice;       //跌停板价
    int64_t  	         _preDelta;              //昨虚实度
    int64_t  	         _currDelta;             //今虚实度
    char  		 _updateTime[13];        //最后修改时间
    uint64_t 		 _updateMillisec;        //最后修改毫秒
    int  		 _levelsUsed;		 //实际使用档位数
    OrderBookPriceLevel* _bids[MAX_LEVELS];	 //申买价/量
    OrderBookPriceLevel* _asks[MAX_LEVELS];	 //申卖价/量
};

class OrderBook {
public:
    /* modify the quantity of an existing price level, or create a new one */
    void Modify(const int64_t price, const uint64_t quantity, bool is_bid)
    {
        if(is_bid)
	{
            /* remove price level if existing */
	    if(quantity == 0) { _bids.erase(price); }
	    else
	    {
	        /* modify exsiting price level */
	        if(_bids.find(price) != _bids.end()) { _bids[price]->SetQuantity(quantity); }
	        /* add new price level */
	        else { _bids[price] = new OrderBookPriceLevel(price, quantity); }
            }
	}
        else
	{
            if(quantity == 0) { _asks.erase(price); }
	    else
	    {
	        if(_asks.find(price) != _asks.end()) { _asks[price]->SetQuantity(quantity); }
	        else { _asks[price] = new OrderBookPriceLevel(price, quantity); }
	    }
	}
    }

    /* add new quantity to an existing price level, or create a new one */
    void Add(const int64_t price, const uint64_t quantity, bool is_bid)
    {
        if(quantity == 0) { return; }

        if(is_bid)
        {
            if(_bids.find(price) != _bids.end()) { _bids[price]->AddQuantity(quantity); }
	    else { _bids[price] = new OrderBookPriceLevel(price, quantity); }
	}
	else
	{
            if(_asks.find(price) != _asks.end()) { _asks[price]->AddQuantity(quantity); }
	    else { _asks[price] = new OrderBookPriceLevel(price, quantity); }
	}
    }

    /* delete a price level */
    void Delete(const int64_t price, bool is_bid) { Modify(price, 0, is_bid); }

    /* get the highest bid */
    OrderBookPriceLevel* BestBid() { return _bids.begin()->second; }

    /* get the lowest ask */
    OrderBookPriceLevel* BestAsk() { return _asks.begin()->second;}

    /* get the highest N bids */
    list<OrderBookPriceLevel*> TopBids(int N)
    {
        if(N <= 0) { /* safety check */ }

	list<OrderBookPriceLevel*> bids;

        /* return all the bids if N is larger than number of bids */
        for(const pair<int64_t, OrderBookPriceLevel*>& bid : _bids)
	{
	    if(--N < 0) { break; }
            bids.push_back(bid.second);
	}

	return bids;
    }

    /* get the lowest N asks */
    list<OrderBookPriceLevel*> TopAsks(int N)
    {
        if(N <= 0) { /* safety check */ }

        list<OrderBookPriceLevel*> asks;

        for(const pair<int64_t, OrderBookPriceLevel*>& ask : _asks)
	{
            if(--N < 0) { break; }
            asks.push_back(ask.second);
	}

	return asks;
    }

    /* get the sum quantity of bids at and above the price
       or the sum quantity of asks at and below the price */
    uint64_t Depth(uint64_t price, bool is_bid);

    /* get the sum volume (price * quantity) of bids at and above
       the price or the sum volume of asks at and below the price */
    int64_t Volumn(uint64_t price, bool is_bid);

    MarketData<10>* SendUpdate(int levels)
    {
        /* suppose MAX_LEVELS equals 10 */
	return new MarketData<10>(levels, TopBids(levels), TopAsks(levels));
    }

    /* for testing */
    void Print()
    {
        cout << "Bids:" << endl;
        for(pair<int64_t, OrderBookPriceLevel*> tuple : _bids)
	{
            cout << "price: " << (tuple.second)->GetPrice() << "; quantity: " << (tuple.second)->GetQuantity() << endl;
	}

        cout << "Asks:" << endl;
	for(pair<int64_t, OrderBookPriceLevel*> tuple : _asks)
	{
	    cout << "price: " << (tuple.second)->GetPrice() << "; quantity: " << (tuple.second)->GetQuantity() << endl;
	}
    }

private:
    /* all the _bids in order of decreasing price */
    map<int64_t, OrderBookPriceLevel*, greater<int64_t> > _bids;
    /* all the _asks in order of increasing price */
    map<int64_t, OrderBookPriceLevel*> _asks;	
};
