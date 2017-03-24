<html>
<body>



<?php 

session_start();
$author = $_SESSION['author'];

$cmd = './addauthor -r ' . $author . ' *' . $_POST['streams'];
exec($cmd, $output, $status);

if ($status) {
	echo 'remove author failed';
}
else {
	if ($output == NULL) {
		echo 'Removed: '. $author;
		echo ' from: '. $_POST['streams'];
	}
	else {
		echo 'ERROR: author may not exist in stream entered <br>';
	}
}


$cmd2 = './a3 backToMenu.wpml';
exec($cmd2, $output2, $status2);

if ($status2)
	echo 'exec failed';
else {
	foreach ($output2 as $line2) {
		echo $line2;
	}
}

?>


</body>
</html> 
