<html>
<body>



<?php 

session_start();
$author = $_SESSION['author'];

$cmd = './post ' . $author . ' *' . $_POST['stream'] . ' @"' . $_POST['text'] . '"';
exec($cmd, $output, $status);

if ($status) {
	echo 'post failed';
}
else {
	if ($output == NULL) {
		echo '<h5>Added Post<h5> from: '. $_SESSION['author'];
		echo ' to: '. $_POST['stream'];
		echo '<br>';
		echo 'Text: ' . $_POST['text']; 
	}
	else {
		echo 'ERROR: Author may not have permission to stream <br>';
	}
}


$cmd = './a3 backToMenu.wpml';
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
