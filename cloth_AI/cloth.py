#날씨 API 받아오기
import requests
import json
import datetime
import pickle
import numpy as np

#아래 url은 받은 api 페이지 참고문서에 보면 적혀있습니다.
weather_URL = 'http://apis.data.go.kr/1360000/VilageFcstInfoService_2.0/getVilageFcst'
service_key = 'C3f1VJ8VEgQNFvd+fBrLMuqEf3hboln4Y+EEFwqvAV84D0HF+xj3cZjSYxkYMMt/ITx+ELxAgIq9S9/lfOoEoQ=='
today = datetime.datetime.today()
base_date = today.strftime("%Y%m%d")
base_time = "0800"

nx = "63"
ny = "122"

#요청변수
params ={'serviceKey' : service_key, 'pageNo' : '1', 'numOfRows' : '1000', 'dataType' : 'JSON', 'base_date' : base_date, 'base_time' : '0800', 'nx' : nx, 'ny' : ny }

response = requests.get(weather_URL, params=params)

items = response.json().get('response').get('body').get('items')

data = dict()
data['data'] = base_date

weather_data = dict()

#items를 조건문으로 비교하며 각각의 list에 분류해줌
tmp_data = list()
wsd_data = list()
reh_data = list()

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
      
items['item']
import numpy

max_TMP =max(tmp_data)
min_TMP = min(tmp_data)
range_TMP = round(max_TMP-min_TMP,2)
max_WSD = max(wsd_data)
range_REH = round(numpy.mean(reh_data),2)


#저장된 학습모델 불러오기
with open("saved_model.pkl", 'rb') as f:
    model = pickle.load(f)


#받아온 변수 값들을 불러온 학습 모델에 넣은 후 예측값 구하기
arr= np.array([[max_TMP, min_TMP , range_TMP, max_WSD, range_REH]])
model_pred = model.predict(arr)

print(model_pred[0])
