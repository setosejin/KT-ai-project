//3팀의 코드입니다.
const http = require('http'); // options에 HTTP 요청에 사용할 옵션 값들을 세팅한다. const options = { hostname: '127.0.0.1', port: 9876, path: '/', method: 'POST' }; // request 함수를 호출한다. 앞서 선언한 options 값과 각 요청이 어떻게 동작할지를 명시한 콜백 함수를 지정한다. const req = http.request(options, function (res) { console.log('STATUS: ' + res.statusCode); console.log('HEADERS: ' + JSON.stringify(res.headers)); res.setEncoding('utf8'); res.on('data', function (chunk) { console.log('BODY: ' + chunk); }); }); req.on('error', function (e) { console.log('Problem with request: ' + e.message); }); // write data to request body req.write('hello'); req.write('bye'); // 요청 본문(body)에 써질게 없다할지라도 반드시 'req.end()'를 호출하여 HTTP 요청이 끝났다는 것을 명시해야 한다. req.end();
const options = { 
        hostname: '127.0.0.1', 
        port: 8080, 
        path: '/', 
        method: 'POST' 
    };

const req = http.request(options, function (res) { 
    console.log('STATUS: ' + res.statusCode); 
    console.log('HEADERS: ' + JSON.stringify(res.headers)); 
    res.setEncoding('utf8'); 
    res.on('data', function (chunk) { 
        console.log('BODY: ' + chunk); 
    }); 
}); 

req.on('error', function (e) { 
    console.log('Problem with request: ' + e.message); 
});

req.write('hello'); 
req.write('bye');
req.end();