<html>
<body>


<?php 

session_start();
if (is_null($_SESSION['author']))
	$_SESSION['author'] = $_POST['author'];

echo 'Logged in as: ' . $_SESSION['author'];

$cmd = './a3 indexDone.wpml';
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