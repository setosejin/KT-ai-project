import requests
import json
import datetime
import numpy
import os
import sys

if len(sys.argv) > 1:
  output = sys.argv[1]
else:
  output = "no argument found"

#print("Hello World!!!")
#print(output)

weather_URL = 'http://apis.data.go.kr/1360000/VilageFcstInfoService_2.0/getVilageFcst'
service_key = 'C3f1VJ8VEgQNFvd+fBrLMuqEf3hboln4Y+EEFwqvAV84D0HF+xj3cZjSYxkYMMt/ITx+ELxAgIq9S9/lfOoEoQ=='
today = datetime.datetime.today()
#today = datetime.date.today()-datetime.timedelta(1) # 새벽에 값을 못 받아와서 전날로 바꿔버림 
base_date = today.strftime("%Y%m%d")
base_time = "1400"

nx = "63"
ny = "122"

params ={'serviceKey' : service_key, 'pageNo' : '1', 'numOfRows' : '1000', 'dataType' : 'JSON', 'base_date' : base_date, 'base_time' : base_time, 'nx' : nx, 'ny' : ny }

response = requests.get(weather_URL, params=params)

items = response.json().get('response').get('body').get('items')
#print(items)
data = dict()
data['data'] = base_date

weather_data = dict()

for item in items['item']:
    # 기온
      if item['category'] == 'TMP':
          weather_data['tmp'] = item['fcstValue']  
weather_data

tmp_data = list()
wsd_data = list()
reh_data = list()
pop_data = list()

for i in range(len(items['item'])):
  if items['item'][i]['fcstDate'] == '20220102':
    if items['item'][i]['category'] == 'TMP':
      tmp = float(items['item'][i]['fcstValue'])
      tmp_data.append(tmp)
    if items['item'][i]['category'] == 'WSD':
      wsd = float(items['item'][i]['fcstValue'])
      wsd_data.append(wsd)
    if items['item'][i]['category'] == 'REH':
      reh = float(items['item'][i]['fcstValue'])
      reh_data.append(reh)
    if items['item'][i]['category'] == 'POP':
      pop = float(items['item'][i]['fcstValue'])
      pop_data.append(pop)
      

max_TMP = max(tmp_data)
min_TMP = min(tmp_data)
range_TMP = round(numpy.mean(tmp_data),2)
max_WSD = max(wsd_data)
range_REH = round(numpy.mean(reh_data),2)


print(str(max_TMP))
print(str(min_TMP))
print(str(range_TMP))
print(str(max_WSD))
print(str(range_REH))


if max(pop_data) <= 0.4 :
  print(1000) #우산 안챙겨
else:
  print(1004) #우산 챙기기 유도