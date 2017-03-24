<html>
<body>

<?php 

$cmd = './a3 addauthor.wpml';
exec($cmd, $output, $status);

if ($status)
	echo 'exec failed';
else {
	foreach ($output as $line) {
		echo $line;
	}
}

?>


</body>
</html> 
