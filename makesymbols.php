<?php
$handle = @fopen("nsmb.idc", "r");
if (!$handle) die("Error opening file");

while (($buffer = fgets($handle, 4096)) !== false)
{
//	MakeName	(0X20A2C80,	"StageScene_onExecute");
//	echo($buffer);
	if(preg_match('/MakeName.*\(0X([0-9A-F]+),	"(.*)"/', $buffer, $matches))
		echo preg_replace("/[^0-9a-zA-Z_]/i", '', $matches[2]), " = 0x", $matches[1], ";\n";
}

fclose($handle);

