<html>
<body>



<?php 

session_start();
$author = $_SESSION['author'];

$cmd = './addauthor ' . $author . ' *' . $_POST['streams'];
exec($cmd, $output, $status);

if ($status) {
	echo 'addauthor failed';
}
else {
	if ($output == NULL) {
		echo 'Added: '. $author;
		echo ' to: '. $_POST['streams'];
	}
	else
		echo 'ERROR: author may already exist <br>';
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
