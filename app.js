// node_modules 에 있는 express 관련 파일을 가져온다.
var express = require('express')

// express 는 함수이므로, 반환값을 변수에 저장한다.
var app = express()

app.use('/img', express.static('img'));
//app.set('view engine','ejs');

app.get('/', (req, res) => {
    res.redirect('/main')
})

//메인 쿠키(서버에 페이지가 담고 있는 날씨정보) 초기화
let pythonDataM = "py";
let pythonDatam = "py";
let pythonDataa = "py";
let pythonDataw = "py";
let pythonDatah = "py";
let pythonDatau = "py";
let pythonDatac = "py";

app.get('/main', (req, res) => {
    //console.log(pythonData);
    
    res.cookie('max', pythonDataM, 1);
    res.cookie('min', pythonDatam, 1);
    res.cookie('avg', pythonDataa, 1);
    res.cookie('wind', pythonDataw, 1);
    res.cookie('hum', pythonDatah, 1);
    res.cookie('umb', pythonDatau, 1);
    res.cookie('img', pythonDatac, 1);
    //console.log(req.cookies);
    setCookie("expend", "true");
    res.sendFile(__dirname + '/main.html')
})

app.get('/cody', (req, res) =>{
    res.sendFile(__dirname + '/cody.html')
})

app.get('/mask', (req, res) =>{
    res.sendFile(__dirname + '/mask.html') // mask, iotmakers
})


// 3000 포트로 서버 오픈
app.listen(3000, function() {
    console.log("start! express server on port 3000");
    
})

const fs = require('fs');
// const jsdom = require('jsdom');
// const { JSDOM } = jsdom;

const {PythonShell} = require('python-shell');

var options = {
  mode: 'text',
  pythonPath: '', //없어도 정상 실행 파이썬 설치경로가 환경변수로 잡혔기 때문
  pythonOptions: ['-u'],
  scriptPath: '',
  args: ['value1'],
  //args: ['value1', 'value2', 'value3'],
  encoding: null
};

PythonShell.run('./cloth_AI/cloth.py', options, function (err, results) {

  if (err) throw err;
  console.log('results: %j',results);
  console.log('max_ondo:: '+ results[0]);
  console.log('min_ondo:: '+ results[1]);
  console.log('avg_ondo:: '+ results[2]);
  console.log('wind_power:: '+ results[3]);

  console.log('humidity:: '+ results[4]);
  console.log('Umbrella:: '+ results[5]);
  console.log('image:: '+ results[6]);

  pythonDataM = results[0];
  pythonDatam = results[1];
  pythonDataa = results[2];
  pythonDataw = results[3];
  pythonDatah = results[4];
  pythonDatau = results[5];
  pythonDatac = results[6];
  //file에 저장할 내용

  var cont = ".";
  
  saveFile(cont);

});


//세션이나 쿠키로 넘기는 내용
var setCookie = function(name, value) {
    // var content;
    // fs.readFile('./main.html', function read(err, data) {
    //     if (err) {
    //         throw err;
    //     }
    //     content = data;
    // });
    
    // const dom = new JSDOM(content);
    // dom.window.document.cookie="result=8"
    // console.log(dom.window.document.cookie);
    
};
    

function saveFile(content){
    fs.writeFileSync('weather.txt', content);
}

