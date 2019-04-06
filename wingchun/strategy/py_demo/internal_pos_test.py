'''
Copyright [2017] [taurus.ai]

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
'''

'''
first demo for new wingchun strategy system.
you may run this program by:
wingchun strategy -n my_test -p basic_usage.py
'''

def initialize(context):
    context.add_md(source=SOURCE.CTP)
    context.ticker = 'rb1801'
    context.exchange_id = EXCHANGE.SHFE
    context.buy_price = -1
    context.sell_price = -1
    context.order_rid = -1
    # add ctp trade engine
    context.add_td(source=SOURCE.CTP)
    context.subscribe(tickers=[context.ticker], source=SOURCE.CTP)

def on_tick(context, market_data, source, rcv_time):
    if market_data.InstrumentID == context.ticker:
        context.buy_price = market_data.AskPrice1
        context.sell_price = market_data.BidPrice1
        if context.order_rid < 0:
            context.order_rid = context.insert_limit_order(source=SOURCE.CTP,
                                                         ticker=context.ticker,
                                                         price=context.buy_price,
                                                         exchange_id=context.exchange_id,
                                                         volume=1,
                                                         direction=DIRECTION.Buy,
                                                         offset=OFFSET.Open)

def on_error(context, error_id, error_msg, request_id, source, rcv_time):
    print 'on_error:', error_id, error_msg

def on_rtn_trade(context, rtn_trade, order_id, source, rcv_time):
    print '----on rtn trade----'
    context.print_pos(context.get_pos(source=SOURCE.CTP))
    context.req_rid = context.req_pos(source=SOURCE.CTP)

def on_pos(context, pos_handler, request_id, source, rcv_time):
    if request_id == -1:
        if pos_handler is not None:
            print '----init----'
            context.print_pos(pos_handler)
    elif request_id == context.req_rid:
        print '----rsp pos----'
        context.print_pos(pos_handler)
        context.stop()
