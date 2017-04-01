<html>
<body>

<?php 

$cmd = './a3 view.wpml';
exec($cmd, $output, $status);

if ($status)
	echo 'exec failed';
else {
	foreach ($output as $line) {
		echo $line;
	}
}



session_start();
$author = $_SESSION['author'];

echo 'Viewing Stream: ' . $_SESSION['stream'] . ' <br><br>';
foreach($_POST as $key => $value) { 
  	$key = $key;
  	$value = $value;
}

$mode = "a";
if ($key == "Sort_Stream_By_Name") {
	echo "Sorted stream by author name! <br>";
	$mode = "o";
}
else if ($key == "Next_Post")
	$mode = "+";
else if ($key == "Previous_Post")
	$mode = "-";
else if ($key == "Mark_All_Read") {
	echo "Marked all posts as read! <br>";
	$mode = "m";
}
else if ($key == "Check_for_New_Posts") {
	echo "Checked for new posts! <br>";
	$mode = "c";
}

$cmd2 = './view.py ' . $author . ' *' . $_SESSION['stream'] . ' @' . $mode;
$output2 = shell_exec($cmd2);

echo "<pre>" . $output2 . "</pre>";

?>

</body>
</html> 