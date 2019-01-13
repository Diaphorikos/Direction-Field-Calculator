var inputField, c, maxx, maxy, points;

function submit(){
	var s = inputField.latex(), t = "";
	for(var i = 0; i < s.length; i++)
		if(s[i] != '\\') t += s[i];
	
	$.get("request.php",
		{ type: "asdf", data : t },
		function(data){ console.log(data); }
	);
}

function drawSlope(length, slope, num, xc, yc){
	c.drawLine({
		strokeStyle: 'black',
		strokeWidth: length/Math.sqrt(num),
		x1: -length/(2 * Math.sqrt(slope * slope + 1)) + xc,
		x2: length/(2 * Math.sqrt(slope * slope + 1)) + xc,
		y1: maxy - (-slope*length/(2 * Math.sqrt(slope * slope + 1)) + yc),
		y2: maxy - (slope*length/(2 * Math.sqrt(slope * slope + 1)) + yc),
	});
}

function draw(data){
	var raw = data.split('\n');
	var slopes = [];
	for(var i = 0; i < points; i++){
		var temp = [];
		for(var j = 0; j < points; j++)
			temp.push(raw[i * points + j]);
		slopes.push(temp);
	}
	console.log(slopes);

	var canvas = document.getElementById('canvas');
	var graph = document.getElementById('graph');
	canvas.width = graph.clientWidth;
	canvas.height = graph.clientHeight;
	maxx = canvas.width;
	maxy = canvas.height;

	c.drawRect({
		fillStyle: 'white',
		x: 0, y: 0,
		width: maxx, height: maxy,
		fromCenter: false
	});

	var small = Math.min(maxx, maxy);

	for(var i = 0; i < points; i++){
		for(var j = 0; j < points; j++){
			
		}
	}
}

function getSlopes(){
	var s = '0\n';
	s += 'x y + \n';
	s += '-10\n10\n-10\n10\n19\n19\n';
	points = 19;

	$.get('request.php',
		{
			type: 'slope',
			data: s
		},
		draw
	);
}

$(document).ready(function(){
	var MQ = MathQuill.getInterface(2);

	var p = document.getElementById('prompt');
	MQ.StaticMath(p);

	var input = document.getElementById('input');
	inputField = MQ.MathField(input);

	$('#submit').click(submit);
	$(document).keydown(function(e){
		if(e.which == 13) submit();
	});

	c = $('canvas');

	$(document).dblclick(function(){
		var ele = $('div');
		for(var i = 0; i < ele.length; i++){
			$(ele[i]).css('background-color', 'hsla(0, 0%, 80%, 0.1)');
			$(ele[i]).css('border', '1px solid black');
		}
	});
});
