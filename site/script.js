var inputField, c, maxx, maxy, maxw, points, samples, slopes, curve, eqn, samples2, slength;

function drawSlope(length, slope, xc, yc){
	c.drawLine({
		strokeStyle: 'grey',
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

	c.drawLine({
		strokeStyle: 'black',
		strokeWidth: 3,
		x1: maxx/2, y1: 10,
		x2: maxx/2, y2: maxy - 10,
		startArrow: true,
		endArrow: true,
		arrowRadius: 10,
		arrowAngle: 90
	});

	c.drawLine({
		strokeStyle: 'black',
		strokeWidth: 3,
		x1: adj + 10, y1: maxy/2,
		x2: maxx - adj - 10, y2: maxy/2,
		startArrow: true,
		endArrow: true,
		arrowRadius: 10,
		arrowAngle: 90
	});

	c.drawText({
		fillStyle: 'black',
		x: maxx - adj + 5,
		y: maxy/2,
		fontSize: 25,
		fontFamily: 'Monospace',
		fontStyle: 'italic',
		text: 'x'
	});

	c.drawText({
		fillStyle: 'black',
		x: maxx/2 + 15,
		y: 17,
		fontSize: 25,
		fontFamily: 'Monospace',
		fontStyle: 'italic',
		text: 'y'
	});

	var tick = Math.floor(Math.sqrt(maxw));
	var len = Math.floor(maxw/tick) - 1;

	for(var i = -len; i <= len; i++){
		if(i == 0) continue;

		c.drawLine({
			strokeStyle: 'black',
			strokeWidth: 2,
			x1: i * tick / maxw / 2 * maxy + adj + maxy/2,
			x2: i * tick / maxw / 2 * maxy + adj + maxy/2,
			y1: maxy/2 - 10,
			y2: maxy/2 + 10
		});
		
		c.drawText({
			fillStyle: 'black',
			x: i * tick / maxw / 2 * maxy + adj + maxy/2,
			y: maxy/2 - 17,
			fontSize: 15,
			fontFamily: 'Monospace',
			text: '' + (i * tick),
			fromCenter: true
		});

		c.drawLine({
			strokeStyle: 'black',
			strokeWidth: 2,
			y1: i * tick / maxw / 2 * maxy + maxy/2,
			y2: i * tick / maxw / 2 * maxy + maxy/2,
			x1: maxy/2 - 10 + adj,
			x2: maxy/2 + 10 + adj
		});
		
		c.drawText({
			fillStyle: 'black',
			y: i * tick / maxw / 2 * maxy + maxy/2,
			x: maxy/2 + 20 + adj,
			fontSize: 15,
			fontFamily: 'Monospace',
			text: '' + (-i * tick),
			fromCenter: true
		});
	}

	for(var i = 0; i < points; i++){
		for(var j = 0; j < points; j++){
			if(slopes[i][j] == 'inf' || slopes[i][j] == '-inf')
				slopes[i][j] = 9999;
			drawSlope(maxy / (points + 3), slopes[i][j] * 1, (i + 1)/(points + 1) * maxy + adj, (j + 1)/(points + 1) * maxy);
		}
	}

	var adj = (maxx - maxy)/2;

	for(var i = 0; i < samples2 - 1; i++){
		var x1 = curve[i][0] * 1;
		var y1 = curve[i][1] * 1;
		var x2 = curve[i + 1][0] * 1;
		var y2 = curve[i + 1][1] * 1;

		x1 = (x1 + 1) / 2 * maxy + adj;
		y1 = maxy - ((y1 + 1) / 2 * maxy);
		x2 = (x2 + 1) / 2 * maxy + adj;
		y2 = maxy - ((y2 + 1) / 2 * maxy);

		if(x1 < adj + 10 && x1 > maxx - adj - 10) continue;
		if(x2 < adj + 10 && x2 > maxx - adj - 10) continue;
		if(y1 < 10 && y1 > maxy - 10) continue;
		if(y2 < 10 && y2 > maxy - 10) continue;

		c.drawLine({
			strokeStyle: 'red',
			strokeWidth: 2,
			x1: x1,
			y1: y1,
			x2: x2,
			y2: y2
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

	window.setTimeout(function(){
		getCurve($('#pointx').val(), $('#pointy').val(), true);
	}, 100);
}

function doCurve(data){
	var raw = data.split('\n');

	curve = [];

	samples2 = raw.length

	for(var i = 0; i < samples2; i++){
		var temp = raw[i].split(' ');
		curve.push(temp);
	}
	render();
}

function getCurve(x, y, r){
	if(!eqn) return;
	var adj = (maxx - maxy) / 2;

	console.log(x, y);
	if(!r){
		x -= adj;
		if(x < 0 || x > maxy) return;
		x = (x / maxy * 2 - 1) * maxw;
		y = ((maxy - y) / maxy * 2 - 1) * maxw;
	}
	else{
		x *= 1;
		y *= 1;
	}

	$('#pointx').val('' + x*1);
	$('#pointy').val('' + y*1);

	console.log(slength);
	var s = '1\n' + eqn + '\n-' + maxw + '\n' + maxw + '\n-' + maxw + '\n' + maxw + '\n' + x + '\n' + y + '\n' + (samples * slength) + '\n' + slength;
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

	$('#canvas').click(function(e){
		getCurve(e.pageX - $(this).offset().left, e.pageY - $(this).offset().top, false);
	});

	/*
	$(document).dblclick(function(){
		var ele = $('div');
		for(var i = 0; i < ele.length; i++){
			$(ele[i]).css('background-color', 'hsla(0, 0%, 80%, 0.1)');
			$(ele[i]).css('border', '1px solid black');
		}
	});
	*/

	$('#points').val(20);
	points = 20;
	$('#points-text').text(points + ' points per axis ');

	$('#window').val(5);
	maxw = 5;
	$('#window-text').text('From x = -' + maxw + ' to x = ' + maxw);
	
	$('#samples').val(20);
	samples = 20;
	$('#samples-text').text(samples + ' points per unit');

	slength = 100;
	$('#length-text').text(slength + ' units');

	$('#points').change(function(){
		points = $('#points').val() * 1;
		$('#points-text').text(points + ' points per axis ');
		getPolish();
	});
	$('#window').change(function(){
		maxw = $('#window').val() * 1;
		$('#window-text').text('From x = -' + maxw + ' to x = ' + maxw);
		getPolish();
	});
	$('#samples').change(function(){
		samples = $('#samples').val() * 1;
		$('#samples-text').text(samples + ' points per unit');
		getPolish();
	});
	$('#length').change(function(){
		slength = $('#length').val() * 1;
		$('#length-text').text(slength + ' units');
		getPolish();
	});

	/*
	$('#pointx, #pointy').keydown(function(e){
	});
	*/
	$(window).on('wheel', function(e){
		var x = e.originalEvent.wheelDelta;

		maxw += Math.floor(x / 100);
		if(maxw < 2) maxw = 2;
		if(maxw > 20) maxw = 20;
		$('#window').val(maxw);

		$('#window-text').text('From x = -' + maxw + ' to x = ' + maxw);
		getPolish();
	});
	getPolish();
});
