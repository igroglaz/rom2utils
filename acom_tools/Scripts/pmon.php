<?php
	require "a2character.class.php";
	error_reporting(0);

#	$file_status = "/var/www/api/status.xml";
#	$file_record = "/var/www/api/record.txt";
#	$filepub_record = "/var/www/pub_info/record.txt";
	$file_status = "C:\\wamp\\www\\api\\status.xml";
	$file_record = "C:\\wamp\\www\\api\\record.txt";
	$filepub_record = "C:\\wamp\\www\\pub_info\\record.txt";

	function to_long($str)
	{
		$c0 = ord($str[3]) & 0xFF;
		$c1 = ord($str[2]) & 0xFF;
		$c2 = ord($str[1]) & 0xFF;
		$c3 = ord($str[0]) & 0xFF;
		$cx = $c0;
		$cx |= ($c1 >> 8);
		$cx |= ($c2 >> 16);
		$cx |= ($c3 >> 24);
		return $cx;
	}

	function sec2hms ($sec, $padHours = false) 
	  {

	    // start with a blank string
	    $hms = "";
	    
	    // do the hours first: there are 3600 seconds in an hour, so if we divide
	    // the total number of seconds by 3600 and throw away the remainder, we're
	    // left with the number of hours in those seconds
	    $hours = intval(intval($sec) / 3600); 

	    // add hours to $hms (with a leading 0 if asked for)
	    $hms .= ($padHours) 
		  ? str_pad($hours, 2, "0", STR_PAD_LEFT). ":"
		  : $hours. ":";
	    
	    // dividing the total seconds by 60 will give us the number of minutes
	    // in total, but we're interested in *minutes past the hour* and to get
	    // this, we have to divide by 60 again and then use the remainder
	    $minutes = intval(($sec / 60) % 60); 

	    // add minutes to $hms (with a leading 0 if needed)
	    $hms .= str_pad($minutes, 2, "0", STR_PAD_LEFT);//. ":";

	    // seconds past the minute are found by dividing the total number of seconds
	    // by 60 and using the remainder
/*	    $seconds = intval($sec % 60); 

	    // add seconds to $hms (with a leading 0 if needed)
	    $hms .= str_pad($seconds, 2, "0", STR_PAD_LEFT);*/

	    // done!
	    return $hms;
	    
	  }


	$servlist = fopen("scan.ini", 'r');
	while(false !== $line = fgets($servlist))
	{
		$servers[]['directory'] = preg_replace("/\/chr\/\*/", "", trim($line));
	}
	fclose($servlist);

	$hat_config = trim(file_get_contents("status.ini"));

	$hatconf = fopen($hat_config, 'r');
	$serv = 0;
	while(false !== $line = fgets($hatconf))
	{
		if(preg_match("/server=.*/", $line))
		{
			if($serv >= count($servers)) break;
			$arrl = explode(',', $line);

			$servers[trim($arrl[1])-1]['name'] = trim($arrl[2]);
			
			$serv++;
		}
	}
	fclose($hatconf);

	foreach($servers as $num => $data)
	{
		printf("read server info: %d, %s, \"%s\"\n", $num, $data['directory'], $data['name']);
	}

	//sleep(10); // while servers are starting

	while(true)
	{
		$out = fopen($file_status, 'w');

		fprintf($out, "<?xml version=\"1.0\" encoding=\"Windows-1251\"?>\n<status>\n");

		foreach($servers as $num => $data)
		{
			$data['time'] = sec2hms(time()-filemtime($data['directory']."/chr/info.map"));
			printf("processing server %d ...\n", $num);
			fprintf($out, "<server>\n");
			fprintf($out, "\t<server_id>%d</server_id>\n", $num+1);
			fprintf($out, "\t<server_name>%s</server_name>\n", $data['name']);
			fprintf($out, "\t<server_time>%s</server_time>\n", $data['time']);
			$mapfile = file_get_contents($data['directory']."/chr/info.map");
			$mapfile = explode("\n", $mapfile); $mapfile = str_replace('\\', '/', trim($mapfile[0])); // first line
			$map = fopen($data['directory']."/".$mapfile, 'r');
			fseek($map, 0x6C);
			$mapnameraw = fread($map, 0x20);
			$null = false;
			$mapname = "";
			for($i = 0; $i < 32; $i++)
			{
				if(ord($mapnameraw[$i]) == 0) break;
				$mapname .= $mapnameraw[$i]; // could be done better ?..
			}
			fprintf($out, "\t<map_name>%s</map_name>\n", $mapname);

			fseek($map, 0xB0);
			$char = fread($map, 1);
			$level = ord($char[0]);

			switch($level)
			{
			case 1:
				$levelstr = "Easy"; break;
			case 2:
				$levelstr = "Medium"; break;
			case 3:
				$levelstr = "Hard"; break;
			case 4:
				$levelstr = "Horror"; break;
			default:
				$levelstr = "Dead beef"; break;
			}

			fprintf($out, "\t<map_level>%s</map_level>\n", $levelstr);

			fseek($map, 0x28);
			$char = fread($map, 8);
			$dim = unpack("V2dim", $char);

			fprintf($out, "\t<map_size>%dx%d</map_size>\n", $dim['dim1']-16, $dim['dim2']-16);

			fclose($map);

			$servercfg = fopen($data['directory']."/server.cfg", 'r');
			$gametype = "Dead beef";
			while(false !== $line = fgets($servercfg))
			{
				if(preg_match("/gametype=.*/i", $line))
				{
					$gametype = preg_replace("/gametype=(.*);.*/i", '$1', $line);
				}
			}
			fclose($servercfg);

			$gametype = trim($gametype);
			$gametype = strtoupper($gametype);

			switch($gametype)
			{
			case "COOPERATIVE":
				$gametype = "Cooperative"; break;
			case "ARENA":
				$gametype = "Arena"; break;
			case "DEATHMATCH":
				$gametype = "Deathmatch"; break;
			default:
				$gametype = "Dead beef"; break;
			}

			fprintf($out, "\t<game_type>%s</game_type>\n", $gametype);

			// now players

			$players = array();
			$playercount = 0;
			$visiblecount = 0;
			$dir = opendir($data['directory']."/chr");
			while(false !== $dirent = readdir($dir))
			{
				if($dirent != '.' &&
					$dirent != '..' &&
					$dirent != 'info.map' &&
					!preg_match("/.*\.sck$/i", $dirent))
				{ // character
					$character = new A2C($data['directory']."/chr/".$dirent);
					if(!$character->bad)
					{
						$pdata['login'] = $dirent;
						$pdata['nickname'] = iconv('866', 'windows-1251', $character->Nick.(strlen($character->Clan) ? "|".$character->Clan : ""));
						$pdata['nickname'] = str_replace('&', "&amp;", $pdata['nickname']);
						$pdata['nickname'] = str_replace('<', "&lt;", $pdata['nickname']);
						$pdata['nickname'] = str_replace('>', "&gt;", $pdata['nickname']);
						$id2 = $character->Id2 & 0x3F000800;
						if($id2 == 0x3F000800) $pdata['visibility'] = "Invisible";
						else $pdata['visibility'] = "Visible";
						$pdata['link_to_details'] = "show.php?a2c=".$data['directory']."/chr/".$dirent;

						$hatinfo = fopen($data['directory']."/../chr/logged_in/".$dirent, 'r');
						$pdata['ip'] = trim(fgets($hatinfo));
						$pdata['time'] = trim(fgets($hatinfo));
						fclose($hatinfo);

						$players[] = $pdata;
						$playercount++;
						if($pdata['visibility'] == "Invisible") $visiblecount++;
					}
				}
			}

			fprintf($out, "\t<visible_count>%d</visible_count>\n", $visiblecount);
			fprintf($out, "\t<all_count>%d</all_count>\n", $playercount);
			if($playercount)
			{
				fprintf($out, "\t<players>\n", $playercount);
				foreach($players as $key => $pdata)
				{
					fprintf($out, "\t\t<player>\n");
					fprintf($out, "\t\t\t<link_to_details>%s</link_to_details>\n", $pdata['link_to_details']);
					fprintf($out, "\t\t\t<name>%s</name>\n", $pdata['nickname']);
					fprintf($out, "\t\t\t<login>%s</login>\n", $pdata['login']);
					fprintf($out, "\t\t\t<ip>%s</ip>\n", $pdata['ip']);
					fprintf($out, "\t\t\t<time>%s</time>\n", $pdata['time']);
					fprintf($out, "\t\t\t<visibility>%s</visibility>\n", $pdata['visibility']);
					fprintf($out, "\t\t</player>\n");
				}
				fprintf($out, "\t</players>\n", $playercount);
			}

			fprintf($out, "</server>\n");
		}

		fprintf($out, "</status>\n");

		fclose($out);

		copy("C:\\Allods2\\record.txt", $file_record);
		copy("C:\\Allods2\\record.txt", $filepub_record);
#		copy("/home/allods2/Documents/Allods2/record.txt", $file_record);
#		copy("/home/allods2/Documents/Allods2/record.txt", $filepub_record);

		sleep(30);
	}

	fclose($out);
?>
