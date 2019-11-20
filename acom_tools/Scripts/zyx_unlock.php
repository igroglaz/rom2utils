<?php
// Скрипт проходит по базе логов, ищет логины с байтом +1C8h установленным в 1, ставит его же в 0, таким образом лог разблокируется.

$vars['chr_base'] = "C:/rom_hat/chr";

	$dir = opendir($vars['chr_base']);
	while(false !== $dirent = readdir($dir))
	{
		if(is_dir($vars['chr_base'].'/'.$dirent) && $dirent[0] != '.')
		{
			$chdirs[] = $dirent;
		}
	}
	closedir($dir);

	for($i = 0; $i < count($chdirs); $i++)
	{
		$cdir = $vars['chr_base'].'/'.$chdirs[$i].'/';
		$dir = opendir($cdir);
		while(false !== $dirent = readdir($dir))
		{
			if(!is_dir($cdir.$dirent) && preg_match('/.*\.lgn/', $dirent))
			{
				$fh = fopen($cdir.$dirent, 'r+');
				{
					fseek($fh, 0x1C8);
					$thisl = ord(fread($fh, 1));
					if($thisl == 1)
					{
						echo 'Broken login: '.$dirent."\n";
						fseek($fh, 0x1C8);
						fwrite($fh, "\x00", 1);
					}
				}
				fclose($fh);
			}
		}
	}
?>