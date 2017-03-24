<html>
<body>


<?php 

session_start();
$_SESSION['author'] = NULL;

$cmd = './a3 index.wpml';
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