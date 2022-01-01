var http = require('http'); 

var server = http.createServer(function(request,response){ 

    request.pipe(response);
});

server.listen(8080, function(){ 
    console.log('Server is running...');
});