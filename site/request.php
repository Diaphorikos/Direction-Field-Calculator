<?php
	$type = $_GET['type'];
	$data = $_GET['data'];

	$f = fopen("buffer.in", "w");
	fwrite($f, $data);
	fclose($f);

	if($type == 'slope' || $type == 'curve')
		exec("../math/makepoints < buffer.in > buffer.out");
	elseif($type == 'parse')
		exec("../math/parser < buffer.in > buffer.out");

	$f = fopen("buffer.out", "r");
	$res = fread($f, filesize("buffer.out"));
	fclose($f);

	echo $res;
?>
