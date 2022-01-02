// node_modules 에 있는 express 관련 파일을 가져온다.
var express = require('express')

// express 는 함수이므로, 반환값을 변수에 저장한다.
var app = express()

// app.get('/', function(req, res){
//     res.send("<h1>hi~~~~ GIGA Genie</h1>")
// })

app.get('/', (req, res) => {
    
    res.redirect('/main')
})

app.get('/main', (req, res) => {
    res.sendFile(__dirname + '/main.html')
})

app.get('/alarm', (req, res) =>{
    res.sendFile(__dirname + '/alarm.html')
})

app.get('/cody', (req, res) =>{
    res.sendFile(__dirname + '/cody.html')
})

app.get('/mask', (req, res) =>{
    res.sendFile(__dirname + '/mask.html')
})



// 3000 포트로 서버 오픈
app.listen(3000, function() {
    console.log("start! express server on port 3000")
})


const {PythonShell} = require('python-shell');
const { fstat } = require('fs');

var options = {
  mode: 'text',
  pythonPath: '', //없어도 정상 실행 파이썬 설치경로가 환경변수로 잡혔기 때문
  pythonOptions: ['-u'],
  scriptPath: '',
  args: ['value1'],
  //args: ['value1', 'value2', 'value3'],
  encoding: null
};

PythonShell.run('weather_data_API.py', options, function (err, results) {

  if (err) throw err;
  console.log('results: %j',results);
  console.log('max_ondo:: '+ results[0]);
  console.log('min_ondo:: '+ results[1]);
  console.log('avg_ondo:: '+ results[2]); //string[]
  console.log('wind_power:: '+ results[3]);
  console.log('humidity:: '+ results[4]);
  console.log('우산:: ' + results[5]);
});