#!/usr/local/env python3.6
# -*- coding: UTF-8 -*-

import pandas as pd
import time
from RepeatedlyMacdChangeQuota_List import *
import pymysql
from pandas import DataFrame as dp



def run_test():
    conn = pymysql.connect(host='cd-cdb-na7tl8h1.sql.tencentcdb.com', port=63889, user='root', passwd='5!Pd!Lgy',
                           db='stock_data', charset='utf8')
    # 创建游标
    cursor = conn.cursor()
    cursor.execute("select date,close,low from exc_hist_data limit 0,10000")
    data = dp(cursor)
    columns = data.columns.size - 1
    # dp.plot.area([0, 1])
    data_array = data.values
    dates = data_array[:, 0]
    close = data_array[:, 1].astype("float64")
    low = data.values[:, 2].astype("float64")
    quota = PluguisRepeatedlyMacdChangeQuotaForList(500)
    start = time.clock()
    for i in range(0, len(close)):
        _start_time = time.clock()
        quota.calculate(close[i], low[i], dates[i])
        print("now index is %s,status code is %s,time: %s" % (i, quota.status, (time.clock() - _start_time)))
        if quota.status == 0:
            array = quota.out_put()
            print(array)
            data1 = pd.DataFrame(array)
            data1.to_csv('data1.csv')
            return
    end = time.clock()
    print end - start


if __name__ == "__main__":
    run_test()

