var inputField, c, maxx, maxy, maxw, points, samples, slopes, curve, eqn;

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

function render(){
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
			if(/*slopes[i][j] == 'nan' || slopes[i][j] == '-nan' ||*/ slopes[i][j] == 'inf' || slopes[i][j] == '-inf')
				slopes[i][j] = 9999;
			drawSlope(maxy / (points + 3), slopes[i][j] * 1, (i + 1)/(points + 1) * maxy + adj, (j + 1)/(points + 1) * maxy);
		}
	}
	console.log(maxx, maxy);

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
	}
}

function doSlope(data){
	var raw = data.split('\n');
	slopes = [];
	for(var i = 0; i < points; i++){
		var temp = [];
		for(var j = 0; j < points; j++)
			temp.push(raw[i * points + j]);
		slopes.push(temp);
	}
	console.log(slopes);
	render();
}

function getSlopes(data){
	if(!points || !maxw) return;

	eqn = data;
	console.log(data);

	var s = '0\n';
	s += data + '\n';
	s += '-' + maxw + '\n' + maxw + '\n-' + maxw + '\n' + maxw + '\n' + points + '\n' + points + '\n';

	$.get('request.php',
		{
			type: 'slope',
			data: s
		},
		doSlope
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

function doCurve(data){
	var raw = data.split('\n');
	curve = [];

	for(var i = 0; i < samples; i++){
		var temp = raw[i].split(' ');
		curve.push(temp);
	}
	render();
}

function getCurve(x, y){
	if(!eqn) return;
	var adj = (maxx - maxy) / 2;

	x -= adj;
	if(x < 0 || x > maxy) return;
	x = (x / maxy * 2 - 1) * maxw;
	y = ((maxy - y) / maxy * 2 - 1) * maxw;

	var s = '1\n' + eqn + '\n-' + maxw + '\n' + maxw + '\n-' + maxw + '\n' + maxw + '\n' + x + '\n' + y + '\n' + samples + '\n200';
	$.get("request.php",
		{
			type: "curve",
			data : s
		},
		doCurve
	);
}


$(document).ready(function(){
	var canvas = document.getElementById('canvas');
	var graph = document.getElementById('graph');
	canvas.width = graph.clientWidth;
	canvas.height = graph.clientHeight;
	maxx = canvas.width;
	maxy = canvas.height;

	var MQ = MathQuill.getInterface(2);

	var p = document.getElementById('prompt');
	MQ.StaticMath(p);

	var input = document.getElementById('input');
	inputField = MQ.MathField(input);

	$('#submit').click(getPolish);
	$(document).keydown(function(e){
		if(e.which == 13) getPolish();
	});

	c = $('#canvas');

	$('#canvas').mousemove(function(e){
		getCurve(e.pageX - $(this).offset().left, e.pageY - $(this).offset().top);
	});

	$(document).dblclick(function(){
		var ele = $('div');
		for(var i = 0; i < ele.length; i++){
			$(ele[i]).css('background-color', 'hsla(0, 0%, 80%, 0.1)');
			$(ele[i]).css('border', '1px solid black');
		}
	});

	$('#points').val(20);
	points = 20;
	$('#points-text').text(points);

	$('#window').val(5);
	maxw = 5;
	$('#window-text').text('From -' + maxw + ' to ' + maxw);
	
	$('#samples').val(200);
	samples = 200;
	$('#samples-text').text(samples);

	$('#points').change(function(){
		points = $('#points').val() * 1;
		$('#points-text').text(points);
		getPolish();
	});
	$('#window').change(function(){
		maxw = $('#window').val() * 1;
		$('#window-text').text('From -' + maxw + ' to ' + maxw);
		getPolish();
	});
	$('#samples').change(function(){
		samples = $('#samples').val() * 1;
		$('#samples-text').text(samples);
		getPolish();
	});
});
