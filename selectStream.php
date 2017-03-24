<html>
<body>

<?php 

$cmd = './a3 selectStream.wpml';
exec($cmd, $output, $status);

if ($status)
	echo 'exec failed';
else {
	foreach ($output as $line) {
		echo $line;
	}
}

session_start();
echo 'Streams for Author: ' . $_SESSION['author'];

$cmd2 = './view.py ' . $_SESSION['author'];
exec($cmd2, $output2, $status2);


if ($status2)
	echo 'exec failed';
else {
	echo "<form method=\"POST\" action=\"view.php\">\n";
	foreach ($output2 as $line2) {
		if ($line2 != NULL)
			echo "<input type=\"radio\" name=\"stream\" value=\"" . $line2 . "\"/> ". $line2 . "\n";
	}
	echo "<input type=\"submit\" value=\"Submit\"/>\n";
	echo "</form>\n";
}

?>

</body>
</html> 