<?php
	$type = $_GET['type'];
	$data = $_GET['data'];

	if($type == 'slope'){
		$f = fopen("buffer.in", "w");
		fwrite($f, $data);
		fclose($f);

		exec("../math/makepoints < buffer.in > buffer.out");

		$f = fopen("buffer.out", "r");
		$res = fread($f, filesize("buffer.out"));
		fclose($f);
	}

	echo $res;
?>
