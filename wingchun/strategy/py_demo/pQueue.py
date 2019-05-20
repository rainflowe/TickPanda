#!/usr/bin/python
# -*- coding: UTF-8 -*-
 
import Queue
import threading
import time
 
exitFlag = 0
queueLock = threading.Lock()

class workDictionary:
    def __init__(self):
        self.dic = {}

    def update(self, key, value):
        queueLock.acquire()
        self.dic[key] = value
        queueLock.release()

    def get(self, key):
        return self.dic.get(key)

    def delete(self, key):
        queueLock.acquire()
        if key in self.dic :
            del self.dic[key]
            print("删除成功！")

        print("item not exists!")
        return

    def getKeys(self):
        return self.dic.keys()

class myOrder:
	def __init__(self, name, time, request_id):
		self.name = name
		self.time = time
		self.request_id = request_id

class myThread (threading.Thread):
    def __init__(self, name, ctx, workQueue):
        threading.Thread.__init__(self)
        self.ctx = ctx
        self.dict = workQueue
        self.name = name

    def run(self):
        print "Starting ==============================" + self.name
        while True:
            time.sleep(1)
            print ("data process begin, num: %d, time: %d" % (self.ctx.md_num, int(time.time())))
            for key in self.dict.getKeys():
                print ("KEY %s, value  %s" % (key, self.dict.get(key).name))

        time.sleep(1)
        print "Exiting " + self.name

class threadManager():
    def __init__(self, ctx):
        self.ctx = ctx
        self.workQueue = workDictionary()
        queueLock = threading.Lock()
    	thread = myThread('testThread', self.ctx, self.workQueue)
        #thread.setDaemon(True)
    	thread.start()

    def updateOrder(self, rid, rname):
        print "add order " + rid
    	order = myOrder(rname, int(time.time()), rid)
    	self.workQueue.update(rid, order)
        print "update workQueue " + rid

#mgr = threadManager("abcd")
#mgr.updateOrder('xiaon', "1234")

'''
threadList = ["Thread-1", "Thread-2", "Thread-3"]
nameList = ["One", "Two", "Three", "Four", "Five"]
queueLock = threading.Lock()
workQueue = Queue.Queue(10)
threads = []
threadID = 1

# 创建新线程
for tName in threadList:
    thread = myThread(threadID, tName, workQueue)
    thread.start()
    threads.append(thread)
    threadID += 1

# 填充队列
queueLock.acquire()
#for word in nameList:
#    workQueue.put(word)
order = myOrder("order1", int(time.time()), "r1234")
workQueue.put(order)
queueLock.release()

# 等待队列清空
while not workQueue.empty():
    pass

# 通知线程是时候退出
exitFlag = 1

# 等待所有线程完成
for t in threads:
    t.join()
print "Exiting Main Thread"
'''