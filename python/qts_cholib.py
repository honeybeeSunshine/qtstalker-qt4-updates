# several helper functions to interact with the chart object database directly


from bsddb3 import db
import struct
import re
import os


HOME_DIR = os.path.expanduser("~")


##read colour codes into a dictionary
colour_dict = {'black': '#000000', 'white': '#FFFFFF', 'red': '#FF0000', 'lime': '#00FF00', 'blue': '#0000FF', 'yellow': '#FFFF00', 'cyan': '#00FFFF', 'magenta': '#FF00FF', 'silver': '#C0C0C0', 'grey': '#808080', 'maroon': '#800000', 'olive': '#808000', 'green': '#008000', 'purple': '#800080', 'teal': '#008080', 'navy': '#000080', 'dark red': '#8B0000', 'brown': '#A52A2A', 'firebrick': '#B22222', 'crimson': '#DC143C', 'tomato': '#FF6347', 'coral': '#FF7F50', 'indian red': '#CD5C5C', 'light coral': '#F08080', 'dark salmon': '#E9967A', 'salmon': '#FA8072', 'light salmon': '#FFA07A', 'orange red': '#FF4500', 'dark orange': '#FF8C00', 'orange': '#FFA500', 'gold': '#FFD700', 'dark golden rod': '#B8860B', 'golden rod': '#DAA520', 'pale golden rod': '#EEE8AA', 'dark khaki': '#BDB76B', 'khaki': '#F0E68C', 'yellow green': '#9ACD32', 'dark olive green': '#556B2F', 'olive drab': '#6B8E23', 'lawn green': '#7CFC00', 'chart reuse': '#7FFF00', 'green yellow': '#ADFF2F', 'dark green': '#006400', 'forest green': '#228B22', 'lime green': '#32CD32', 'light green': '#90EE90', 'pale green': '#98FB98', 'dark sea green': '#8FBC8F', 'medium spring green': '#00FA9A', 'spring green': '#00FF7F', 'sea green': '#2E8B57', 'medium aqua marine': '#66CDAA', 'medium sea green': '#3CB371', 'light sea green': '#20B2AA', 'dark slate gray': '#2F4F4F', 'dark cyan': '#008B8B', 'aqua': '#00FFFF', 'light cyan': '#E0FFFF', 'dark turquoise': '#00CED1', 'turquoise': '#40E0D0', 'medium turquoise': '#48D1CC', 'pale turquoise': '#AFEEEE', 'aqua marine': '#7FFFD4', 'powder blue': '#B0E0E6', 'cadet blue': '#5F9EA0', 'steel blue': '#4682B4', 'corn flower blue': '#6495ED', 'deep sky blue': '#00BFFF', 'dodger blue': '#1E90FF', 'light blue': '#ADD8E6', 'sky blue': '#87CEEB', 'light sky blue': '#87CEFA', 'midnight blue': '#191970', 'dark blue': '#00008B', 'medium blue': '#0000CD', 'royal blue': '#4169E1', 'blue violet': '#8A2BE2', 'indigo': '#4B0082', 'dark slate blue': '#483D8B', 'slate blue': '#6A5ACD', 'medium slate blue': '#7B68EE', 'medium purple': '#9370DB', 'dark magenta': '#8B008B', 'dark violet': '#9400D3', 'dark orchid': '#9932CC', 'medium orchid': '#BA55D3', 'thistle': '#D8BFD8', 'plum': '#DDA0DD', 'violet': '#EE82EE', 'magenta / fuchsia': '#FF00FF', 'orchid': '#DA70D6', 'medium violet red': '#C71585', 'pale violet red': '#DB7093', 'deep pink': '#FF1493', 'hot pink': '#FF69B4', 'light pink': '#FFB6C1', 'pink': '#FFC0CB', 'antique white': '#FAEBD7', 'beige': '#F5F5DC', 'bisque': '#FFE4C4', 'blanched almond': '#FFEBCD', 'wheat': '#F5DEB3', 'corn silk': '#FFF8DC', 'lemon chiffon': '#FFFACD', 'light golden rod yellow': '#FAFAD2', 'light yellow': '#FFFFE0', 'saddle brown': '#8B4513', 'sienna': '#A0522D', 'chocolate': '#D2691E', 'peru': '#CD853F', 'sandy brown': '#F4A460', 'burly wood': '#DEB887', 'tan': '#D2B48C', 'rosy brown': '#BC8F8F', 'moccasin': '#FFE4B5', 'navajo white': '#FFDEAD', 'peach puff': '#FFDAB9', 'misty rose': '#FFE4E1', 'lavender blush': '#FFF0F5', 'linen': '#FAF0E6', 'old lace': '#FDF5E6', 'papaya whip': '#FFEFD5', 'sea shell': '#FFF5EE', 'mint cream': '#F5FFFA', 'slate gray': '#708090', 'light slate gray': '#778899', 'light steel blue': '#B0C4DE', 'lavender': '#E6E6FA', 'floral white': '#FFFAF0', 'alice blue': '#F0F8FF', 'ghost white': '#F8F8FF', 'honeydew': '#F0FFF0', 'ivory': '#FFFFF0', 'azure': '#F0FFFF', 'snow': '#FFFAFA', 'dim grey': '#696969', 'gray': '#808080', 'dark gray': '#A9A9A9', 'light gray': '#D3D3D3', 'gainsboro': '#DCDCDC', 'white smoke': '#F5F5F5'}



#list chart objects for given symbol
def cho_list(symbol,chotype='all',plot='all',bar='all',colour='all'):

	if (colour != 'all') and ('#' not in colour): #look up colours automatically as we don't know the codes
		colour = colour_dict[colour]
		print(colour)

	myDB = db.DB()
	filename = HOME_DIR + '/.qtstalker/data1/index/co.db'
	myDB.open(filename, None, db.DB_BTREE)
	# cur = myDB.cursor()

	key = (symbol+'_LIST\x00').encode()
	cholist = myDB.get(key)
	if not cholist:
		print ('cholist empty, no objects or no entry in index')
		return
	cholist = cholist.decode()
	cholist = cholist.split('\x00')[0]
	cholist = cholist.split(',')

	print(cholist)

	returnlist = []
	for x in cholist:
		key = (symbol+x+'\x00').encode()
		cho = myDB.get(key)
		cho = cho.decode()

		if plot == 'all' or cho_value('Plot', cho) == plot:
			if chotype == 'all' or cho_value('Type',cho) == chotype:
				if colour == 'all' or cho_value('Color',cho) == colour:
					if 'Bar Length' in cho:
						if bar == 'all' or cho_value('Bar Length', cho) == bar:
							returnlist.append(cho)
							# print(cho)
					else:
						returnlist.append(cho)
						# print(cho)
						
	myDB.close()

	i = 0
	for item in returnlist:
		print(str(i)+': '+cho_value('Type',item))
		print(item)
		print(' ')
		i += 1

	return returnlist


#prep chart object data to insert into database, i.e. pass returned value to cho_write
def cho_make (chotype,startdate=False,enddate=False,value=False,value2=False,label=False,plot=False,colour=False,usebar=False):

	if startdate:
		if len(startdate) == 8:
			startdate = startdate + '000000'
			# startdate = startdate + '234500'
		elif len(startdate) != 14:
			print('wrong startdate length')
			return
	if enddate:
		if len(enddate) == 8:
			enddate = enddate + '000000'
			# enddate = enddate + '234500'
		elif len(enddate) != 14:
			print('wrong enddate length')
			return	

	if chotype == 'VerticalLine':
		cho = 'Type=VerticalLine|Name=%s|Date='+startdate+'|Plot='+plot

	elif chotype == 'HorizontalLine':
		if not label: label = ''
		cho = 'Type=HorizontalLine|Name=%s|Text='+label+': |Value='+value+'|Plot='+plot

	elif chotype == 'Text':
		cho = 'Type=Text|Name=%s|Font=DejaVu Sans 10 50|Value='+str(value)+'|Date='+startdate+'|Label='+label+'|Plot='+plot

	elif chotype == 'TrendLine':
		cho = 'Extend Line=1|Use Bar=0|End Date='+enddate+'|Auto Adjust Line=0|Bar Length=26|Type=TrendLine|Start Date='+startdate+'|Name=%s|Start Value='+str(value)+'|End Value='+str(value2)+'|Draw Fan=1|Plot='+plot+'|Bar Field=High-Low'


	if colour:
		cho += '|Color='+colour_dict[colour]
	else:
		cho += '|Color=#55aaff'

	return cho + '\x00'



#write an object to the chart object database, use chomake first
def cho_write (symbol, cho):

	myDB = db.DB()
	filename = HOME_DIR + '/.qtstalker/data1/index/co.db'
	myDB.open(filename)

	#get list of chart objects for this symbol
	key = (symbol+'_LIST\x00').encode()
	cholist = myDB.get(key)
	if (cholist == b'\x00'): ##no objects in database, so call the first one 0
		i = 0
		cholist = b'0\x00'
	else:
		cholist = cholist.decode()
		cholist = cholist.split('\x00')[0]
		#find first missing integer in cholist to use as name and index of this new chart object
		choints = cholist.split(',')
		choints = [int(x) for x in choints]
		choints.sort()
		i = 0
		for c in choints:
			if c == i:
				i += 1

		cholist = cholist + ',' + str(i) + '\x00'
		cholist = cholist.encode()

	#update list of chart objects
	myDB.put(key, cholist)

	#now put the actual chart object into the db
	key = (symbol+str(i)+'\x00').encode()
	val = cho % str(i)
	val = val.encode()
	myDB.put(key, val)

	myDB.sync()
	myDB.close()




#delete an object from the chart object database
def cho_drop (symbol, choidx):

	myDB = db.DB()
	filename = HOME_DIR + '/.qtstalker/data1/index/co.db'
	myDB.open(filename)

	# #delete chart object with specified index
	key = (symbol+str(choidx)+'\x00').encode()
	myDB.delete(key)

	#remove index from list of indices
	#decompose and recompose the list of chart objects indices
	key = (symbol+'_LIST\x00').encode()
	cholist = myDB.get(key)
	cholist = cholist.decode()
	cholist = cholist.split('\x00')[0]
	cholist = cholist.split(',')
	cholist = [x for x in cholist if x != str(choidx)]
	cholist = ','.join([str(x) for x in cholist])
	cholist = cholist + '\x00' #remove last

	myDB.put(key, cholist.encode())
	myDB.sync()
	myDB.close()




#delete all chart objects matching these criteria
def cho_clear (symbol, plot='all', chotype='HorizontalLine', colour='all'):
	cholist = cho_list(symbol, plot=plot, chotype=chotype, colour=colour)
	for cho in cholist:
		cho_drop(symbol, cho_value('Name',cho))
	return




#return bool as to whether key exists in price database, or return price value
def check_db (symbol, date, get=False):
	myDB = db.DB()
	filename = HOME_DIR + '/.qtstalker/data1/data/Stocks/'+symbol
	myDB.open(filename)
	key = (date + '\x00').encode()
	exists = myDB.exists(key)

	if get:
		val = myDB.get(key)
		val = struct.unpack('<5di4x', val)
		if get == 'open':		val = val[0]
		elif get == 'high':		val = val[1]
		elif get == 'low':		val = val[2]
		elif get == 'close':	val = val[3]
		elif get == 'volume':	val = val[4]
		exists = val

	myDB.close()
	return exists



#return start and end date values for a chart object
def cho_dates(cho):
	parsedate = lambda x : x[0:4] + '/' + x[4:6] + '/' + x[6:8]
	end_date = parsedate(cho_value('End Date',cho))
	start_date = parsedate(cho_value('Start Date',cho))
	return [start_date,end_date]


#return the value
def cho_value(key, cho):
	value = re.search(key+'\=([^\|]+)',cho).group(1)
	if (('Date' in key) or ('Name' in key)):
		return value
	else:
		try:
			float(value)
			return float(value)
		except:
			return value