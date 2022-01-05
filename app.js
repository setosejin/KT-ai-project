//실행 방법 : node app.js
// node_modules 에 있는 express 관련 파일을 가져온다.
var express = require('express')

// express 는 함수이므로, 반환값을 변수에 저장한다.
var app = express()

//img 폴더에 있는 파일들을 사용한다.
app.use('/img', express.static('img'));

//localhost:3000으로 넘어오면 바로 localhost:3000/main으로 넘긴다.
app.get('/', (req, res) => {
    res.redirect('/main')
})

//메인 쿠키(서버에 페이지가 담고 있는 날씨정보) 초기화
//빈 값으로 초기화하면 실행 안 됨.
let pythonDataM = "py";
let pythonDatam = "py";
let pythonDataa = "py";
let pythonDataw = "py";
let pythonDatah = "py";
let pythonDatau = "py";
let pythonDatac = "py";
let pythonDatat = "py";

//localhost:3000/main으로 들어오면 동작하는 것들.
app.get('/main', (req, res) => {
    //클라이언트(로컬웹페이지)에 쿠키를 설정한다.
    res.cookie('max', pythonDataM, 1);
    res.cookie('min', pythonDatam, 1);
    res.cookie('avg', pythonDataa, 1);
    res.cookie('wind', pythonDataw, 1);
    res.cookie('hum', pythonDatah, 1);
    res.cookie('umb', pythonDatau, 1);
    res.cookie('img', pythonDatac, 1);
    res.cookie('now', pythonDatat, 1);

    //현재 디렉토리의 main.html 파일을 화면에 표시한다.
    res.sendFile(__dirname + '/main.html')
})

//localhost:3000/cody으로 들어오면 동작하는 것들.
app.get('/cody', (req, res) =>{
    //현재 디렉토리의 cody.html 파일을 화면에 표시한다.
    res.sendFile(__dirname + '/cody.html')
})

//localhost:3000/mask으로 들어오면 동작하는 것들.
app.get('/mask', (req, res) =>{
    //현재 디렉토리의 mask.html 파일을 화면에 표시한다.
    res.sendFile(__dirname + '/mask.html')
})

// 3000 포트로 서버 오픈
app.listen(3000, function() {
    console.log("start! express server on port 3000");
})

//파일 시스템을 fs로 불러와서 사용한다.
const fs = require('fs');

//python-shell을 PythonShell로 불러와서 사용한다.
const {PythonShell} = require('python-shell');

//변수 설정 : python-shell을 동작시키기 위해 필요한 기본 옵션들.
var options = {
  mode: 'text',
  pythonPath: '', //없어도 정상 실행 파이썬 설치경로가 환경변수로 잡혔기 때문
  pythonOptions: ['-u'],
  scriptPath: '',
  args: ['value1'],
  encoding: null
};

// /cloth_AI 폴더에 있는 cloth.py 파일을 실행시킨다.
// (node.js 내에서 .py파일 실행시키는 방법 : python-shell)
PythonShell.run('./cloth_AI/cloth.py', options, function (err, results) {

    //error handling
    if (err) throw err;

    //node를 실행시키는 console창에 출력(확인용)
    console.log('results: %j',results);
    console.log('max_ondo:: '+ results[0]);
    console.log('min_ondo:: '+ results[1]);
    console.log('avg_ondo:: '+ results[2]);
    console.log('wind_power:: '+ results[3]);
    console.log('humidity:: '+ results[4]);
    console.log('Umbrella:: '+ results[5]);
    console.log('image:: '+ results[6]);
    console.log('now_ondo:: ' + results[7])

    // .py 파일을 실행시키고 나서 출력된 결과들을 쿠키 변수들에 넣어준다.
    pythonDataM = results[0];
    pythonDatam = results[1];
    pythonDataa = results[2];
    pythonDataw = results[3];
    pythonDatah = results[4];
    pythonDatau = results[5];
    pythonDatac = results[6];
    pythonDatat = results[7];
});
