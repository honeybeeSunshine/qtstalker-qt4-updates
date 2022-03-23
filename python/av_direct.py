#an example to fetch and/or update from AlphaVantage into QtStalker, enter your api Key below

import sys
import os
import requests
from datetime import datetime

#modules to interact directly with database
from bsddb3 import db
import struct
myDB = db.DB()


symbol = sys.argv[1]
HOME_DIR = os.path.expanduser("~")
BASE_PATH = HOME_DIR + "/.qtstalker/data1/data/Stocks/"
filename = BASE_PATH + symbol


key = 'YOUR KEY HERE'
url = 'https://www.alphavantage.co/query?function=TIME_SERIES_DAILY&symbol=%s&outputsize=full&apikey=%s' % (symbol,key)



head_ts = False

###open database, creating one if it doesn't exist
if os.path.exists(filename):
    myDB.open(filename, None, db.DB_BTREE)
    try:
        #get latest datapoint from database
        cur = myDB.cursor()
        kvpair = cur.last()
        head_ts = datetime.strptime(kvpair[0].decode().split('\x00')[0],'%Y%m%d%H%M%S')
        print("last db entry: " + str(head_ts))
        print("fetch 100 data points only")
        url = url.replace('outputsize=full','outputsize=compact')
    except:
        print(symbol + ": database appears empty, fetching full data")

else:
    myDB.open(filename, None, db.DB_BTREE, db.DB_CREATE)
    print("file doesn't exist, creating: " + filename)
    #for new files, need to create an entry in index.db so chart objects will work
    a1 = [symbol, '', '', '', filename, '', '', '']
    a2 = [x.encode() for x in a1]
    a3 = [True]
    a3.extend(a2)
    val = struct.pack('<?15s10s90s10s100s15s5s2s', *a3)
    key = (symbol + '\x00').encode()
    idxDB = db.DB()
    idxDB.open(HOME_DIR + "/.qtstalker/data1/index/index.db", None, db.DB_BTREE)
    idxDB.put(key, val)
    idxDB.sync()
    idxDB.close()
    print("added entry in index.db")


##fetch the data
data = requests.get(url).json()
data = data['Time Series (Daily)']

##put the data in the database
extra_flag = False
for day in data:
    if head_ts:
        if datetime.strptime(day,'%Y-%m-%d') < head_ts:
            extra_flag = True
            continue

    o = float(data[day]['1. open'])
    h = float(data[day]['2. high'])
    l = float(data[day]['3. low'])
    c = float(data[day]['4. close'])
    v = float(data[day]['5. volume'])

    key = (day.replace('-','') + '000000' + '\x00').encode()
    val = struct.pack('5di4x', *[o,h,l,c,v,0])
    myDB.put(key,val)

if extra_flag == False:
    print("no extra data, could be data gap")

myDB.sync()
myDB.close()
