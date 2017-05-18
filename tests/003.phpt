--TEST--
Hooking fopen and returning a value
--SKIPIF--
<?php if (!extension_loaded("sqreen")) print "skip"; ?>
--FILE--
<?php
// No log 
$infile = fopen("test.text", "r") or die("Unable to open file!");
echo fread($infile,filesize("test.text"));
fclose($infile);

// Log 
sqreenon();
$infile = fopen("test.text", "r") or die("Unable to open file!");
echo fread($infile,filesize("test.text"));
fclose($infile);

// No log 
sqreenoff();
$infile = fopen("test.text", "r") or die("Unable to open file!");
echo fread($infile,filesize("test.text"));
fclose($infile);
?>
--EXPECT--
