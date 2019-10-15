<?php

error_reporting(0);

// defines

define(ORIGINAL_XML, "original.xml");
define(OUTPUT_XML, "output.xml");
define(COEFFICIENT, 0.3);

// functions

function matchRULES($out) {
	
	if(preg_match("<treasureGoldMin>", $out)) {
		$search = str_replace(array("<treasureGoldMin>", "</treasureGoldMin>"), "", $out);
		$int = intval($search);
		$newint = ($int != "-1") ? (COEFFICIENT * $int) : $int;
		echo "Replacing: ".$int."->".$newint.PHP_EOL;
		$out = str_replace($int, $newint, $out);
	}
	
	if(preg_match("<treasureGoldMax>", $out)) {
		$search = str_replace(array("<treasureGoldMax>", "</treasureGoldMax>"), "", $out);
		$int = intval($search);
		$newint = ($int != "-1") ? (COEFFICIENT * $int) : $int;
		echo "Replacing: ".$int."->".$newint.PHP_EOL;
		$out = str_replace($int, $newint, $out);
	}
	
	return $out;
	
}

function parseDATAxml($file, $msg = "Parsing data.bin xml...") {

	$output = "";

    echo $msg.PHP_EOL;

    $read_thread = fopen($file, "r");

    while ($buffer = fgets($read_thread, 128)) {
      $output .= matchRULES($buffer);
    }

    if (!feof($read_thread)) {
       echo "Error: unexpected fgets() fail" . PHP_EOL;
    }

    fclose($read_thread);

	writeDATAxml(OUTPUT_XML, $output);

    unset($buffer);
	unset($output);
	
}

function writeDATAxml($file, $buffer, $msg = "Writing new data.bin xml...") {
    
	echo $msg.PHP_EOL;
	
	$write_thread = fopen($file, "w");
	fwrite($write_thread, $buffer, strlen($buffer));
	fclose($write_thread);
	
}

// run app

parseDATAxml(ORIGINAL_XML);
 
echo PHP_EOL.'Done.';

?>