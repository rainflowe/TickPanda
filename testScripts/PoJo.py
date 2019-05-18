#!/usr/local/env python3.6
# -*- coding: UTF-8 -*-


class MacdChangePoint:

    def __init__(self, index, lowPrice, price, macd, macdType, date):
        self.index = index
        self.lowPrice = lowPrice
        self.price = price
        self.macd = macd
        self.macdType = macdType
        self.date = date

    def getDate(self):
        return self.date

    def setDate(self, date):
        self.date = date

    def getIndex(self):
        return self.index

    def setIndex(self , index):
        self.index = index

    def getPrice(self):
        return self.price

    def setPrice(self , price):
        self.price = price

    def getMacd(self):
        return self.macd

    def setMacd(self , macd):
        self.macd = macd

    def getMacdType(self):
        return self.macdType

    def setMacdType(self , macdType):
        self.macdType = macdType

    def getLowPrice(self):
        return self.lowPrice

    def setLowPrice(self,lowPrice):
        self.lowPrice = lowPrice