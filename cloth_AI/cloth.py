#날씨 API 받아오기
import requests
import json
import datetime
import pickle
import numpy as np
import sys

if len(sys.argv) > 1:
  output = sys.argv[1]
else:
  output = "no argument found"


#아래 url은 받은 api 페이지 참고문서에 보면 적혀있습니다.
weather_URL = 'http://apis.data.go.kr/1360000/VilageFcstInfoService_2.0/getVilageFcst'
service_key = 'C3f1VJ8VEgQNFvd+fBrLMuqEf3hboln4Y+EEFwqvAV84D0HF+xj3cZjSYxkYMMt/ITx+ELxAgIq9S9/lfOoEoQ=='
today = datetime.datetime.today()
base_date = today.strftime("%Y%m%d")
base_time = "0800"

nx = "63"
ny = "122"

#요청변수
params ={'serviceKey' : service_key, 'pageNo' : '1', 'numOfRows' : '1000', 'dataType' : 'JSON', 'base_date' : base_date, 'base_time' : base_time, 'nx' : nx, 'ny' : ny }

response = requests.get(weather_URL, params=params)

items = response.json().get('response').get('body').get('items')

data = dict()
data['data'] = base_date

weather_data = dict()

for item in items['item']:
    # 기온
      if item['category'] == 'TMP':
          weather_data['tmp'] = item['fcstValue']  

#items를 조건문으로 비교하며 각각의 list에 분류해줌
tmp_data = list()
wsd_data = list()
reh_data = list()
pop_data = list()

for i in range(len(items['item'])):
  if items['item'][i]['fcstDate'] == base_date:
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
 
#items['item']

import numpy

max_TMP =max(tmp_data)
min_TMP = min(tmp_data)
range_TMP = round(max_TMP-min_TMP,2)
max_WSD = max(wsd_data)
range_REH = round(numpy.mean(reh_data),2)
this_tmp = tmp_data[0]

#저장된 학습모델 불러오기
with open("./cloth_AI/saved_model.pkl", 'rb') as f:
    model = pickle.load(f)


#받아온 변수 값들을 불러온 학습 모델에 넣은 후 예측값 구하기
arr= np.array([[max_TMP, min_TMP , range_TMP, max_WSD, range_REH]])
model_pred = model.predict(arr)

print(str(max_TMP))
print(str(min_TMP))
print(str(range_TMP))
print(str(max_WSD))
print(str(range_REH))

if max(pop_data) <= 0.4 :
  print(1000) #우산 안챙겨
else:
  print(1004) #우산 챙기기 유도

print(model_pred[0])


print(str(this_tmp)) #현재온도
