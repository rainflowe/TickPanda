#include <iostream>
#include "OrderBook.h"

using namespace std;

int main()
{
    OrderBook orderbook;
    orderbook.Add(10, 10, true);
    orderbook.Add(20, 10, true);
    orderbook.Add(30, 10, true);
    orderbook.Add(40, 10, true);

    orderbook.Add(15, 10, false);
    orderbook.Add(25, 10, false);
    orderbook.Add(35, 10, false);
    orderbook.Add(45, 10, false);

    orderbook.Print();

    orderbook.Modify(20, 15, true);
    orderbook.Modify(35, 5, false);

    orderbook.Print();

    orderbook.Add(10, 10, true);
    orderbook.Delete(25, false);

    orderbook.Print();

    cout << orderbook.BestBid()->GetPrice() << endl;
    cout << orderbook.BestAsk()->GetPrice() << endl;

    list<OrderBookPriceLevel*> bids = orderbook.TopBids(3);
    for(OrderBookPriceLevel* bid : bids) { cout << "price: " << bid->GetPrice() << "; quantity: " << bid->GetQuantity() << endl; }
    list<OrderBookPriceLevel*> asks = orderbook.TopAsks(2);
    for(OrderBookPriceLevel* ask : asks) { cout << "price: " << ask->GetPrice() << "; quantity: " << ask->GetQuantity() << endl; }

    return 0;
}
