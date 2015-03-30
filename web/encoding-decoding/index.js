var query = 'Apple Watch Apps Begin Showing Up in the App Store Ahead of Apple Watch Launch';

var anagramica = require('anagramica');

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
}

// query = query.replace(/[0-9a-zA-Z ]./, '');
// console.log(query);
var words = query.split(' ');

var arrayQuery = [];
var shuffledQuery = [];
var anagramicas = [];
var result = [];


for(var i = 0; i < words.length; i++)(function(_i){
  arrayQuery.push(words[_i].shuffle());

  console.log(arrayQuery);
  setTimeout(function(){

  anagramica.all(arrayQuery[_i], function(error, response) {
    if (error) {
      throw error;
      // console.log('eee');
    }else{
         arrayQuery[_i] = response.all[0];
    }
    if(_i == words.length-1){
      console.log(arrayQuery);

    }
  });
}, 2000);


}(i));
//
