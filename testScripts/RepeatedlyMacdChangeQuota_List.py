#!/usr/local/env python3.6
# -*- coding: UTF-8 -*-


import talib as ta
import numpy as np
import Constants
import PoJo

'''
计算macd
参数：价格序列
返回值：diff macd dea
'''


class PluguisRepeatedlyMacdChangeQuotaForList:

    def __init__(self, length):
        self.length = length
        # 价格序列
        self.close_price_array = []
        # 价格序列
        self.low_price_array = []
        # 价格序列
        self.date_array = []

    def calculate(self, close, low, date):
        self.close_price_array.append(close)
        self.low_price_array.append(low)
        self.date_array.append(date)
        if len(self.close_price_array) < self.length:
            self.status = Constants.Constants.LESS_POINT
        else:
            _start_index = len(self.close_price_array) - self.length
            _close_price_array_tmp = []
            for i in range(_start_index, len(self.close_price_array)):
                _close_price_array_tmp.append(self.close_price_array[i])
            self.close_price_array = _close_price_array_tmp
            self.do_calculate()
            self.status = Constants.Constants.SUCCESS_CODE
        return self

    def do_calculate(self):
        close_price_array = self.close_price_array
        # macd值序列
        self.macd_array = np.zeros(len(close_price_array))
        # diff值序列
        self.diff_array = np.zeros(len(close_price_array))
        # 价格拐点序列
        self.price_change_point_array = np.zeros(len(close_price_array))
        # macd拐点序列
        self.macd_change_point_array = np.zeros(len(close_price_array))
        # macd金叉死叉序列
        self.macd_cross_point_array = np.zeros(len(close_price_array))
        # macd背离类型
        self.macd_reversion_point_type_array = np.zeros(len(close_price_array))
        # macd背离标记
        self.macd_reversion_point_sign_array = np.zeros(len(close_price_array))
        # macd背离与发生背离点相差周期数
        self.macd_reversion_point_count_array = np.zeros(len(close_price_array))
        # macd背离点标准点
        self.macd_reversion_point_stand_point_array = np.zeros(len(close_price_array))
        # macd背离点预背离点
        self.macd_reversion_ready_point_array = np.zeros(len(close_price_array))
        # macd背离幅度
        self.macd_reversion_point_percent_array = np.zeros(len(close_price_array))
        # 多次背离标志
        self.repeatedly_macd_reversion_point_sign_array = np.zeros(len(close_price_array))
        # 多次背离幅度
        self.repeatedly_macd_reversion_point_percent_array = np.zeros(len(close_price_array))
        # 多次背离次数
        self.repeatedly_macd_reversion_point_count_array = np.zeros(len(close_price_array))
        # 多次背离基准点
        self.repeatedly_macd_reversion_point_stand_point_array = np.zeros(len(close_price_array))
        # 多次背离类型
        self.repeatedly_macd_reversion_point_type_array = np.zeros(len(close_price_array))

        '''
        初始化计算
        '''
        self.calculate_macd()
        self.calculate_price_change_point()
        self.calculate_macd_change_point()
        self.calculate_macd_down_reversion_point()
        self.calculate_macd_top_reversion_point()
        return


    '''
    计算macd diff dea
    '''

    def calculate_macd(self):
        macd, macdsignal, macdhist = ta.MACD(np.array(self.close_price_array, dtype=float))
        self.macd_array = macd
        self.diff_array = macdhist
        # print(macd)
        return

    '''
    计算价格拐点
    '''

    def calculate_price_change_point(self):
        price_array = self.close_price_array
        for index in range(1, len(price_array) - 1):
            if price_array[index - 1] < price_array[index] and price_array[index] >= price_array[index + 1]:
                self.price_change_point_array[index] = 1
            elif price_array[index - 1] >= price_array[index] and price_array[index] < price_array[index + 1]:
                self.price_change_point_array[index] = -1
            else:
                self.price_change_point_array[index] = None
        return

    '''
    获取区间最小值
    '''
    def findMinValue(self, array, start, end):
        tmp_array = sorted(array[start:end])
        return tmp_array[len(tmp_array) - 1]
    '''
    获取区间最大值
    '''
    def findMaxValue(self, array, start, end):
        tmp_array = sorted(array[start:end])
        return tmp_array[0]

    '''
    标记金叉死叉
    '''

    def find_cross_points(self):
        macd_value_list = self.macd_array
        result = {}
        for i in range(1, len(self.macd_array) - 1):
            value = macd_value_list[i]
            before_value = macd_value_list[i - 1]
            if value <= 0 < before_value:
                # 死叉
                result[i] = Constants.Constants.MACD_DEAD_FORK
                self.macd_cross_point_array[i] = Constants.Constants.MACD_DEAD_FORK
            elif before_value <= 0 < value:
                # 金叉
                result[i] = Constants.Constants.MACD_GOLD_FORK
                self.macd_cross_point_array[i] = Constants.Constants.MACD_GOLD_FORK
            else:
                self.macd_cross_point_array[i] = None
        return result

    '''
    计算macd拐点
    '''

    def calculate_macd_change_point(self):
        price_array = self.macd_array
        for index in range(1, len(price_array) - 1):
            if price_array[index - 1] < price_array[index] and price_array[index] >= price_array[index + 1]:
                self.macd_change_point_array[index] = 1
            elif price_array[index - 1] >= price_array[index] and price_array[index] < price_array[index + 1]:
                self.macd_change_point_array[index] = -1
            else:
                self.macd_change_point_array[index] = None
        # print (self)
        return

    '''
    底背离
    '''
    def calculate_macd_down_reversion_point(self):
        macd_change_point_array = self.macd_change_point_array
        macd_value_array = self.macd_array
        low_array = self.low_price_array
        close_array = self.close_price_array

        cross_dict = self.find_cross_points()
        keys_array = sorted(cross_dict.keys())

        for i in range(1, len(self.macd_change_point_array) - 1):
            change_type = macd_change_point_array[i]
            break_flag = False
            # 寻找底拐点
            if change_type == Constants.Constants.MACD_BOTTOM_NOT_BREAK or change_type == Constants.Constants.MACD_BOTTOM_BREAK:
                # 反向查找上一个顶拐点
                for j in range(i - 1, 0, -1):
                    if break_flag:
                        break

                    lastTopChangeType = macd_change_point_array[j]
                    # 得到第一个顶拐点
                    if lastTopChangeType == Constants.Constants.MACD_TOP_NOT_BREAK or lastTopChangeType == Constants.Constants.MACD_TOP_BREAK:
                        # 反向查找底拐点
                        for k in range(j-1, 0, -1):
                            if break_flag:
                                break

                            lastDownChangeType = macd_change_point_array[k]
                            # 得到第一个底拐点(对比的底拐点必须是macd为负的底拐点)
                            if lastDownChangeType == Constants.Constants.MACD_BOTTOM_BREAK or lastDownChangeType == Constants.Constants.MACD_BOTTOM_NOT_BREAK:
                                # System.out.println("上一底拐点日期:" + sf.format(df.get(k, getColumns("date"))))
                                # 满足第一个条件，当前macd的值大于前一个拐点的值
                                kValue = macd_value_array[k]
                                iValue = macd_value_array[i]
                                if kValue < iValue:
                                    # 最后一个波段统计区间(j->i)
                                    start = j - 1
                                    end = i
                                    lowListFirPrice = self.findMinValue(low_array, start, end)
                                    closeListFirPrice = self.findMinValue(close_array, start, end)
                                    # 前一个波段统计区间(k->j)
                                    start = k - 1
                                    end = j
                                    lowListSecPrice = self.findMinValue(low_array, start, end)
                                    closeListSecPrice = self.findMinValue(close_array, start, end)

                                    # 背离确认
                                    if None != lowListFirPrice and None != closeListFirPrice and None != lowListSecPrice and None != closeListSecPrice:
                                        if lowListFirPrice < lowListSecPrice or closeListFirPrice < closeListSecPrice:
                                            # 确认背离类型(顺序：k--> j --> i)
                                            for l in range(0, len(keys_array) - 1):
                                                if break_flag:
                                                    break
                                                # 确认起点死叉位置
                                                if (keys_array[l] < k < keys_array[l + 1]) or (
                                                        keys_array[l] < k <= keys_array[l + 1]):
                                                    for m in range(l, len(keys_array) - 1):
                                                        if break_flag:
                                                            break
                                                        # 确认终点死叉位置
                                                        if (keys_array[m] <= i < keys_array[m + 1]) or (
                                                                keys_array[m] < i <= keys_array[m + 1]):
                                                            '''
                                                            * 单波段背离 0
                                                            * 未破线背离 1
                                                            * 标准背离 2
                                                            '''
                                                            type = Constants.Constants.MACD_BOTTOM_SINGLE_POINT
                                                            if m - l == 1:
                                                                type = Constants.Constants.MACD_BOTTOM_NOT_BREAK_POINT
                                                            elif m - l == 2:
                                                                type = Constants.Constants.MACD_BOTTOM_STANDARD_POINT

                                                            percent = macd_value_array[i] / macd_value_array[k]

                                                            self.macd_change_point_array[i] = type
                                                            self.macd_reversion_point_percent_array[i] = percent
                                                            self.macd_reversion_point_stand_point_array[i] = i + 1
                                                            self.macd_reversion_point_count_array[i] = i - k

                                                            for n in range(i, j, -1):
                                                                if close_array[n] < close_array[k]:
                                                                    self.macd_reversion_ready_point_array[n] = 1
                                                            break_flag = True
                                # 底拐点不能向前移动，不成功则放弃
                                break_flag = True
        # print("down")
        return

    '''
    顶背离
    '''
    def calculate_macd_top_reversion_point(self):
        macd_change_point_array = self.macd_change_point_array
        macd_value_array = self.macd_array
        low_array = self.low_price_array
        close_array = self.close_price_array

        cross_dict = self.find_cross_points()
        keys_array = sorted(cross_dict.keys())

        for i in range(1, len(self.macd_change_point_array) - 1):
            change_type = macd_change_point_array[i]
            break_flag = False
            # 寻找顶拐点
            if change_type == Constants.Constants.MACD_TOP_NOT_BREAK or change_type == Constants.Constants.MACD_TOP_BREAK:

                for j in range(i - 1, 0, -1):
                    if break_flag:
                        break
                    # 反向查找上一个底拐点
                    last_down_change_type = macd_change_point_array[j]
                    # 得到第一个顶拐点
                    if last_down_change_type == Constants.Constants.MACD_BOTTOM_BREAK or last_down_change_type == Constants.Constants.MACD_BOTTOM_NOT_BREAK:
                        # 反向查找顶拐点
                        for k in range(j-1, 0, -1):
                            if break_flag:
                                break

                            last_top_change_type = macd_change_point_array[k]
                            # 得到第一个底拐点(对比的底拐点必须是macd为负的底拐点)
                            if last_top_change_type == Constants.Constants.MACD_TOP_NOT_BREAK or last_top_change_type == Constants.Constants.MACD_TOP_BREAK:
                                # System.out.println("上一底拐点日期:" + sf.format(df.get(k, getColumns("date"))))
                                # 满足第一个条件，当前macd的值大于前一个拐点的值
                                kValue = macd_value_array[k]
                                iValue = macd_value_array[i]
                                if kValue < iValue:
                                    # 最后一个波段统计区间(j->i)
                                    start = j - 1
                                    end = i
                                    lowListFirPrice = self.findMaxValue(low_array, start, end)
                                    closeListFirPrice = self.findMaxValue(close_array, start, end)
                                    # 前一个波段统计区间(k->j)
                                    start = k - 1
                                    end = j
                                    lowListSecPrice = self.findMaxValue(low_array, start, end)
                                    closeListSecPrice = self.findMaxValue(close_array, start, end)

                                    # 背离确认
                                    if None != lowListFirPrice and None != closeListFirPrice and None != lowListSecPrice and None != closeListSecPrice:
                                        if lowListFirPrice < lowListSecPrice or closeListFirPrice < closeListSecPrice:
                                            # 确认背离类型(顺序：k--> j --> i)
                                            for l in range(0, len(keys_array) - 1):
                                                if break_flag:
                                                    break
                                                # 确认起点死叉位置
                                                if (keys_array[l] < k < keys_array[l + 1]) or (
                                                        keys_array[l] < k <= keys_array[l + 1]):
                                                    for m in range(l, len(keys_array) - 1):
                                                        if break_flag:
                                                            break
                                                        # 确认终点死叉位置
                                                        if (keys_array[m] <= i < keys_array[m + 1]) or (
                                                                keys_array[m] < i <= keys_array[m + 1]):
                                                            '''
                                                             * 单波段背离 3
                                                             * 未破线背离 4
                                                             * 标准背离 5
                                                            '''
                                                            type = Constants.Constants.MACD_TOP_SINGLE_POINT
                                                            if m - l == 1:
                                                                type = Constants.Constants.MACD_TOP_NOT_BREAK_POINT
                                                            elif m - l == 2:
                                                                type = Constants.Constants.MACD_TOP_STANDARD_POINT

                                                            percent = macd_value_array[i] / macd_value_array[k]

                                                            self.macd_change_point_array[i] = type
                                                            self.macd_reversion_point_percent_array[i] = percent
                                                            self.macd_reversion_point_stand_point_array[i] = i + 1
                                                            self.macd_reversion_point_count_array[i] = i - k

                                                            for n in range(i, j, -1):
                                                                if close_array[n] < close_array[k]:
                                                                    self.macd_reversion_ready_point_array[n] = 2
                                                            break_flag = True
                                # 顶拐点不能向前移动，不成功则放弃
                                break_flag = True
        # print("down")
        return

#===================================================多次背离计算==================================================================


    def find_bottom_macd_points(self):
        rsList = []
        closeArray = self.close_price_array
        lowArray = self.low_price_array
        macdArray = self.macd_array
        dateArray = self.date_array
        macdChangePoint = self.macd_change_point_array

        for i in range(0,len(macdArray)):
            o = macdChangePoint[i]
            if None != o == Constants.Constants.MACD_BOTTOM_BREAK:
                macd = macdArray[i]
                price = closeArray[i]
                lowPrice = lowArray[i]
                date = dateArray[i]
                type = PoJo.MacdChangePoint(i,lowPrice,price,macd,o,date)
                rsList.append(type)

        return rsList
    '''
    统计数组中大于0的数量
    '''
    def findBigThanZoroInArray(self , array , start , end):
        return len(np.maximum(array[start , end],0))

    '''
    统计数组中大于0的数量
    '''
    def findSmallThanZoroInArray(self , array , start , end):
        return len(np.minimum(array[start , end],0))

    '''
    *查找多次底背离点
    * @ param
    local
    * @ param
    extStorage
    * @ return
    '''
    def findButtomRange(self):

        rsList = []
        # 是否是标准背离
        is_stand_change = True

        list = self.find_bottom_macd_points()
        if len(list) == 0:
            return rsList

        standPoint = list[0]
        allChangeCount = 0
        standChangeCount = 0
        subRsList = []
        for i in range(1,len(list)):
            point = list[i]
            refIndex = i - 1
        if len(subRsList) > 0:
            dataIndex = subRsList[subRsList.size()-1].getIndex()
            for j in range(i,(i-20),-1):
                if list[j].getIndex() == dataIndex:
                    refIndex = j
                    break

        refPoint = list[refIndex]
        # 查找MACD底基准，要求基准之后的每一次type为0或者为2的背离，上一拐点的Price > 当前拐点Price，且MACD基准 < MACD当前拐点
        # testChange = refPoint.getPrice() > point.getPrice() and standPoint.getMacd() < point.getMacd()
        testPrice = refPoint.getPrice() > point.getPrice() or refPoint.getLowPrice() > point.getLowPrice()
        testMacd = standPoint.getMacd() < point.getMacd()
        testDiff = True
        if self.findMaxValue(self.diff_array, standPoint.getIndex(), point.getIndex()) < 0:
            testDiff = False

        if testMacd and testPrice:
            # 非标准背离
            if refPoint.getMacd() >= point.getMacd():
                is_stand_change = False

            if is_stand_change:
                standChangeCount += 1
            allChangeCount+=1
            change_type = Constants.Constants.MACD_REPEATEDLY_POINT
            if is_stand_change:
                change_type = Constants.Constants.MACD_REPEATEDLY_STANDARD_POINT
            percent = point.getMacd() / standPoint.getMacd()
            result = (point.getIndex(), change_type, standChangeCount, allChangeCount, percent,
                 self.findBigThanZoroInArray(self.macd_array, standPoint.getIndex(), point.getIndex()),
                 self.diff_array[point.getIndex()], standPoint.getDate())
            subRsList.append(result)
    
        # 寻找下一个区间
        elif testDiff or bool(1-testMacd):
            # 标准背离标志刷新
            is_stand_change = True
            # 重置数量
            allChangeCount = 0
            standChangeCount = 0

            '''
            * 重置缓存
            '''
            minDiff = 10000
            for j in range(0,len(subRsList)):
                tmpDiff = subRsList[j].getMinDiff()
                if tmpDiff < minDiff:
                    minDiff = tmpDiff
                else:
                    subRsList[j].setMinDiff(minDiff)




            rsList.extend(subRsList)
            subRsList = []

            '''
            *基准点更新
            '''

            standPoint = point
        return rsList

    def out_put(self):
        array = np.array([
            self.close_price_array,
            self.low_price_array,
            self.macd_array,
            self.diff_array,
            self.price_change_point_array,
            self.macd_change_point_array,
            self.macd_cross_point_array,
            self.macd_reversion_point_type_array,
            self.macd_reversion_point_sign_array,
            self.macd_reversion_point_count_array,
            self.macd_reversion_point_stand_point_array,
            self.macd_reversion_ready_point_array,
            self.macd_reversion_point_percent_array,
            self.repeatedly_macd_reversion_point_sign_array,
            self.repeatedly_macd_reversion_point_percent_array,
            self.repeatedly_macd_reversion_point_count_array,
            self.repeatedly_macd_reversion_point_stand_point_array,
            self.repeatedly_macd_reversion_point_type_array
        ])
        return array