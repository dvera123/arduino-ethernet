$(document).ready(function() {

	var menu = $('.menu');
	var button = $('.button');
	var message = $('.message h3');

	function both (evt){
		if (button.hasClass("open")){
			menu.animate({right: 0});  button.addClass('close'); button.removeClass('open'); message.html('Menu Open');
			}

		else if (button.hasClass("close")){
			menu.animate({right: -400});  button.addClass('open'); button.removeClass('close'); message.html('Menu Closed');
			}

		}

	if (annyang) {

		var go = function() {
	      $.ajax({
			  type: 'GET',	
			  data: { do: "go"}
			}).done(function(content) {
				message.html('Go');
				console.log("direction: go");
			});
	      
	    };

	    var reverse = function() {
	      $.ajax({
			  type: 'GET',	
			  data: { do: "reverse"}
			}).done(function(content) {
				message.html('Reverse');
				console.log("direction: reverse");
			});
	      
	    };

		var commands = {
	     'go': go,
	     'reverse': reverse
	    };

	    
	    annyang.init(commands);
	    annyang.start();
	}	

});
