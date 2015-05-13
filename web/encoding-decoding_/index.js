//Imports
var http = require('http'), url = require("url"), anagramica = require('anagramica'), fs = require('fs');

var testServer = function(request,response){
    try{
        request.on('error', function (err) {
            sendError('Request error ' + err, ERRORS.REQUEST);
        });

        response.on('error', function (err) {
            sendError('Response error ' + err, ERRORS.RESPONSE);
        });

        var urlParts = url.parse(request.url, true);
        var query = urlParts.query;
        if(query.sentence != null){
            var sentence = query.sentence;
            var words = sentence.split(' ');
            var arrayQuery = [];
            getAnagrams(words,function(anagramResponse){
                response.end(anagramResponse.join(" "));
            });
        } else {
            var index = fs.readFileSync('index.html');
            response.writeHead(200, {'Content-Type': 'text/html'});
            response.end(index);
        }
    } catch(err) {
        sendError('General error ' + err);
    }
}

var getAnagrams = function(wordArray,callback){
    var computed = 0;
    var anagramArray = [];
    for(var i = 0; i < wordArray.length ; i++)(function(_i){
        anagramica.all(wordArray[_i].shuffle(), function(err,anagramicaResponse){
           if(err){
               throw(err);
           } else {
               console.log(anagramicaResponse);
                anagramArray[_i] = (anagramicaResponse.all[0]);
               console.log(anagramArray);
           }
            if(++computed == wordArray.length){
                console.log(anagramArray);
                callback(anagramArray);
            }
        });
    })(i);
}

String.prototype.shuffle = function () {
    var a = this.split(""),
        n = a.length;

    for(var i = n - 1; i > 0; i--) {
        var j = Math.floor(Math.random() * (i + 1));
        var tmp = a[i];
        a[i] = a[j];
        a[j] = tmp;
    }
    return a.join("");
};

sendOk = function () {
    console.log("OK");
};

sendError = function (err) {
    console.trace(err);
    process.exit(-1);
};

var serv =  http.createServer(testServer).listen(8000);
