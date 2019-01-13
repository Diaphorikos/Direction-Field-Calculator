var inputField, c, maxx, maxy, points;

function drawSlope(length, slope, xc, yc){
	c.drawLine({
		strokeStyle: 'black',
		strokeWidth: 1.2,
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

	var adj = (maxx - maxy) / 2;

	for(var i = 0; i < points; i++){
		for(var j = 0; j < points; j++){
			if(slopes[i][j] == 'nan' || slopes[i][j] == '-nan' || slopes[i][j] == 'inf' || slopes[i][j] == '-inf')
				slopes[i][j] = 9999;
			drawSlope(maxy / (points + 3), slopes[i][j] * 1, adj + (j + 1)/(points + 1) * maxy, (i + 1)/(points + 1) * maxy);
		}
	}

	console.log(maxx, maxy);
}

function getSlopes(data){
	console.log(data);

	var s = '0\n';
	s += data + '\n';
	s += '-2\n2\n-2\n2\n39\n39\n';
	points = 39;

	$.get('request.php',
		{
			type: 'slope',
			data: s
		},
		draw
	);
}

function submit(){
	var s = inputField.latex();
	s = s.split('\\ ').join('');
	console.log(s);
	
	$.get("request.php",
		{
			type: "parse",
			data : s
		},
		getSlopes
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
