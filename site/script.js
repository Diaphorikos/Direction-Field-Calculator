var inputField, c, maxx, maxy, maxw, points, samples;

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

	/*

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

	*/

	var adj = (maxx - maxy) / 2;

	for(var i = 0; i < points; i++){
		for(var j = 0; j < points; j++){
			if(/*slopes[i][j] == 'nan' || slopes[i][j] == '-nan' ||*/ slopes[i][j] == 'inf' || slopes[i][j] == '-inf')
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
	s += '-4\n4\n-4\n4\n40\n40\n';
	maxw = 3;
	points = 40;

	$.get('request.php',
		{
			type: 'slope',
			data: s
		},
		draw
	);
}

function getPolish(){
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

function drawCurve(data){
	var canvas = document.getElementById('canvas');
	var graph = document.getElementById('graph');
	canvas.width = graph.clientWidth;
	canvas.height = graph.clientHeight;
	maxx = canvas.width;
	maxy = canvas.height;

	var raw = data.split('\n');
	var curve = [];

	for(var i = 0; i < samples; i++){
		var temp = raw[i].split(' ');
		curve.push(temp);
	}

	var adj = (maxx - maxy)/2;

	for(var i = 0; i < samples - 1; i++){
		var x1 = curve[i][0] * 1;
		var y1 = curve[i][1] * 1;
		var x2 = curve[i + 1][0] * 1;
		var y2 = curve[i + 1][1] * 1;

		c.drawLine({
			strokeStyle: 'black',
			strokeWidth: 2,
			x1: (x1 + 1) / 2 * maxy + adj,
			y1: maxy - ((y1 + 1) / 2 * maxy),
			x2: (x2 + 1) / 2 * maxy + adj,
			y2: maxy - ((y2 + 1) / 2 * maxy)
		});

//		console.log(
//			(x1 / maxw + 1) / 2 * maxy,
//			maxy - ((y1 / maxw + 1) / 2 * maxy),
//			(x2 / maxw + 1) / 2 * maxy,
//			maxy - ((y2 / maxw + 1) / 2 * maxy)
//		);
	}
}

function getCurve(){
	samples = 1000;
	var s = '1\n( 0 x - ) y / \n-4\n4\n-4\n4\n1\n-1\n1000\n200';
	
	$.get("request.php",
		{
			type: "curve",
			data : s
		},
		drawCurve
	);
}


$(document).ready(function(){
	var MQ = MathQuill.getInterface(2);

	var p = document.getElementById('prompt');
	MQ.StaticMath(p);

	var input = document.getElementById('input');
	inputField = MQ.MathField(input);

	$('#submit').click(getPolish);
	$(document).keydown(function(e){
		if(e.which == 13) getPolish();
	});

	c = $('canvas');
	maxw = 3;

	getCurve();

	$(document).dblclick(function(){
		var ele = $('div');
		for(var i = 0; i < ele.length; i++){
			$(ele[i]).css('background-color', 'hsla(0, 0%, 80%, 0.1)');
			$(ele[i]).css('border', '1px solid black');
		}
	});
});
