#!/usr/local/env python3.6
# -*- coding: UTF-8 -*-


class Constants:

    def __init__(self):
        return

    '''
     空值，占位
     '''
    NULL_VALUE = None
    '''
     空值，占位
     '''
    MACD_VALUE = "macd_value"
    '''
     死叉
     '''
    MACD_DEAD_FORK = 1
    '''
     金叉
     '''
    MACD_GOLD_FORK = -1
    '''
     未破线底点
     '''
    MACD_BOTTOM_NOT_BREAK = -11
    '''
     底点
     '''
    MACD_BOTTOM_BREAK = -1
    '''
     未破线顶点
     '''
    MACD_TOP_NOT_BREAK = 11
    '''
     顶点
     '''
    MACD_TOP_BREAK = 1

    '''
     单波段底背离点
     '''
    MACD_BOTTOM_SINGLE_POINT = 0
    '''
     未破线底背离点
     '''
    MACD_BOTTOM_NOT_BREAK_POINT = 1
    '''
     标准背离点
     '''
    MACD_BOTTOM_STANDARD_POINT = 2

    '''
     单波段背离点
     '''
    MACD_TOP_SINGLE_POINT = 3
    '''
     未破线顶背离点
     '''
    MACD_TOP_NOT_BREAK_POINT = 4
    '''
     标准顶背离点
     '''
    MACD_TOP_STANDARD_POINT = 5
    '''
     非标准多次背离点
     '''
    MACD_REPEATEDLY_POINT = 1
    '''
     标准多次背离点
     '''
    MACD_REPEATEDLY_STANDARD_POINT = 0

    '''
    状态码
    '''
    LESS_POINT = -1
    CATCH_EXCEPTION = -2
    SUCCESS_CODE = 0




