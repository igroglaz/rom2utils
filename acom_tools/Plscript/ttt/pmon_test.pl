#use File::Listing qw(parse_dir);

# Импорт модуля XML::Simple.
#
#use XML::Simple;


$logfile=">>pmon.log";
$lockfile="pmon.lck";
$args= "pmon " . time();
for($i=0;$i<=$#ARGV;$i++)
	{
 	 $args=$args . " " . $ARGV[$i];
 	}
open(LOG,$logfile);
print LOG "start ", $args, "\n";
close(LOG);
#if( open(LOCK,$lockfile)) {die "locked";}
#else {open(LOCK,">" . $lockfile);
#   print LOCK time();close(LOCK);}


use Encode qw(from_to);
my @arr;
my $keysec;
my @levels=("","Легкий","Средний","Сложный","Ужас");
my %type=();
$type{"COOPERATIVE"}="Кооперативный";
$type{"ARENA"}="Арена";
$type{"TEAMPLAY"}="Командная";
my %visprt=();
$visprt{0}="";
$visprt{1}="Invisible";



my @sps = (
  "None",                  # incorrect spell
  "Fire Arrow",            # огненная стрела
  "Fire Ball",             # огненный шар
  "Fire Wall",             # огненная стена
  "Protection from Fire",  # защита от магии огня
  "Ice Missile",           # ледяная стрела
  "Poison Cloud",          # ядовитый туман
  "Blizzard",              # град
  "Protection from Water", # защита от магии воды
  "Acid Stream",           # кислотная атака
  "Lightning",             # молния
  "Prismatic Spray",       # радужная молния
  "Invisibility",          # невидимость
  "Protection from Air",   # защита от магии воздуха
  "Darkness",              # тьма
  "Light",                 # свечение
  "Diamond Dust",          # каменная стрела
  "Wall of Earth",         # каменная стена
  "Stone Curse",           # каменное проклятие
  "Protection from Earth", # защита от магии огня
  "Bless",          # благословение
  "Haste",          # ускорение
  "Control Spirit", # перерождение
  "Teleport",      # телепорт
  "Heal",          # исцеление
  "Summon",        # зов
  "Drain Life",    # вампиризм
  "Shield",        # магический щит
  "Curse",         # проклятие
  "Slow");           # замедление


sub readil($) {
    my $idx = shift;
    $a = @arr[$idx];
    $b = @arr[$idx+1];
    $c = @arr[$idx+2];
    $d = @arr[$idx+3];

#print sprintf("%x %x %x %x \n",$d, $c, $b, $a);
    return $a | ($b << 8) | ($c << 16) | ($d << 24);
}

sub readwl($) {
    my $idx = shift;
    $a = @arr[$idx];
    $b = @arr[$idx+1];
    return $a | ($b << 8);
}

sub readbl($) {
    my $idx = shift;
    $a = @arr[$idx];
    return $a;
}

sub readi($) {
    my $f = shift;
    read($f, $a, 1);
    read($f, $b, 1);
    read($f, $c, 1);
    read($f, $d, 1);
    return ord($a) | (ord($b) << 8) | (ord($c) << 16) | (ord($d) << 24);
}
		   
sub readb($) {
    my $f = shift;
    read($f, $a, 1);
    return ord($a);
}

sub decrypt {
    $keysec = (($keysec >> 0x10) & 0xFFFF);
    $k = $keysec | ($keysec << 0x10);
#    print($kh);
    $c = 0;
    foreach $i(@arr) {
	$i = $i & 0xFF;
#        $ih1 = sprintf("0x%2.2X", $i);
	$i = (($k >> 0x10) ^ $i) & 0xFF;
	$k = ($k << 1) & 0xFFFFFFFF;
	if (($c & 0x0F) == 0x0F) {
	    $k |= $keysec;
	}
	$c+=1;
#        $kh = sprintf("0x%8.8X", $k);
#        $ih = sprintf("0x%2.2X", $i);
#	print("$ih1 $ih $kh\n");
    }
}

sub testchecksum {
    my $ch = 0;
    foreach $i(@arr) {
	$ch = (($ch << 1) & 0xFFFFFFFF) + ($i & 0xFF);
#	print "ch=$ch, i=$i\n";
    }
    return $ch & 0xFFFFFFFF;
}

sub readpack {
    $idx = 9;
    $size = scalar(@arr);
    while ($idx < $size) {
	$itid = readwl($idx); $idx+=2;
	$b0 = readbl($idx); $idx++;
        $IDh = sprintf("%.4Xh", $itid);
	if (($b0 & 0x80) == 0x80) {
	    $cc = $b0-0x80;
	    print("  found simple object: ID=$IDh, count=$cc\n");
	} elsif (($b0 & 0x20) == 0x20) {
	    $price = readil($idx); $idx+=4;
	    $ccc = $b0 & 0xF;
	    print("  found magic object: ID=$IDh, price=$price gold\n");
	    $i = 0;
	    while ($i++ < $ccc) {
		$eid = readbl($idx); $idx++;
	        $eIDh = sprintf("%.2Xh", $eid);
		$evl = readbl($idx); $idx++;
		print("   effect ID=$eIDh, value=$evl\n");
#		if ($eid == 0x29 || ($eid >= 0x2C && $eid <=0x30)) {
#		    $ccc--;
#		}
	    }
	} elsif (!$b0) {
	    $ccc = readwl($idx); $idx+=2;
	    print("  found simple object: ID=$IDh, count=$ccc\n");
	}
    }
}

my %server_list=();
#print "C:\\Games\\Allods\\PLScript\\status.ini\n";
open(INI,"C:\\Games\\Allods\\PLScript\\status.ini");
$hat_cfg_name=<INI>;
chomp $hat_cfg_name;
close(INI);
#print $hat_cfg_name . "\n";
open(HAT_CFG,$hat_cfg_name);
  while(<HAT_CFG>)
   {
    chomp;
	if(/chr_base=(.*)\\?/)
        	  {
	  $logged_in="C:\\Games\\Allods\\" . $1 . "\\logged_in\\";
	 }
#	if(/server=\d+\.\d+\.\d+\.\d+:\d+,(.+),(.*)/) 
	if(/server=.+:\d+,(.+),(.*)/) 
	  {
	    $server_list{$1}=$2;
#	    print "$1, $2\n";
	   }
   }
   
open(OUT_ADM,">C:\\Games\\Allods\\status.xml");   
print OUT_ADM "<?xml version=\"1.0\" encoding=\"Windows-1251\"?>";
print OUT_ADM "<status>";

#print "C:\\Games\\Allods\\PLScript\\scan.ini\n";
$servern=0;
open(SERVLIST, "C:\\Games\\Allods\\PLScript\\scan.ini");
$section="";
while(<SERVLIST>)
 { 
   $servern=$servern+1;
   print OUT_ADM "<server>";

   my %logs=();
   chomp;
   $name=$_;
   open(LISTING,"dir /B $name|");
   $name=~s/\*\.$//;
   $name=~/server(\d+)/;
   $server_id=$1;
   $server_cfg=$name ."..\\server.cfg";
   open(SVR_CFG,$server_cfg);
   while(<SVR_CFG>)
    {

	 if(/GameType=([a-zA-Z]*)/) { $GameType=uc $1;}
	}
   
   $map_info=$name . "info.map";
   open(MAP_FILE,$map_info) || next;
    $map_file=<MAP_FILE>;
	close(MAP_FILE);
	chomp $map_file;
    $time_info=$name . "..\\time.info"; 
	if(open(TIME_FILE,$time_info))
	  {
	    $map_old=<TIME_FILE>;chomp $map_old;
	    $time_old=<TIME_FILE>;chomp $time_old;
	    close(TIME_FILE);
	$tmp=$map_file;
	$tmp=~s/\\/\\\\/;
	$tmp=~s/\./\\\./;
	
#	    print "$map_old| |$map_file|\n";
	    if($map_old=~/$tmp/) 
		{$time_cur=time()-$time_old;}
	      else
		{
		  $time_cur=0;
		  open(TIME_FILE, ">" . $time_info); 
		  print TIME_FILE "$map_file\n";
		  print TIME_FILE time(); 
		  print TIME_FILE "\n";
		}
	  }
	  else
		{
		  $time_cur=0;
		  open(TIME_FILE, ">" . $time_info); 
		  print TIME_FILE "$map_file\n";
		  print TIME_FILE time(); 
		  print TIME_FILE "\n";
		}
#	print $time_cur . "\n";
	$map_file=$name . "..\\" . $map_file;	
    open(MAP_FILE,$map_file);
     binmode MAP_FILE;
	 read(MAP_FILE,$a,0x28);
	 read(MAP_FILE,$b,8);
	 @arr=unpack("c*",$b);
#	 $X_size=((@arr[3]*256+@arr[2])*256+@arr[1])*256+@arr[0]-16;
#	 $Y_size=((@arr[7]*256+@arr[6])*256+@arr[5])*256+@arr[4]-16;
	 $X_size=(((@arr[3]&0xFF)*256+(@arr[2]&0xFF))*256+(@arr[1]&0xFF))*256+(@arr[0]&0xFF)-16;
	 $Y_size=(((@arr[7]&0xFF)*256+(@arr[6]&0xFF))*256+(@arr[5]&0xFF))*256+(@arr[4]&0xFF)-16;
 	read(MAP_FILE, $a, 0x3c);
	read(MAP_FILE, $b, 0x40);
	@arr=unpack("c*", $b);
		  	$idxxx = 0;
	$map_name = "";
  	while ($c = readbl($idxxx++)) {
		$map_name = $map_name . chr($c & 0xFF);
	}
	 read(MAP_FILE,$b,5);
	  @arr=unpack("c*", $b);
	  $map_level=@arr[4];
	close(MAP_FILE);
#	$tmp=sprintf("%u",$server_id);
	
	

	 
	 $viscount=0;
   while(<LISTING>)
    {
	  chomp;
	  $lgn=$_;
	  $lgn_file=$name . $_;
	  open(C, $lgn_file)|| die "can't open $lgn\n";
	  binmode C;
	  
	  $sig=readi(C);

while($sigsec=readi(C) & 0xFFFFFFFF) {
    $sizsec=readi(C) & 0xFFFFFFFF;
    $keysec=readi(C) & 0xFFFFFFFF;
    $crcsec=readi(C) & 0xFFFFFFFF;
    $sigh = sprintf("%.8Xh", $sigsec);
#    print("section found: $sigh\n size: $sizsec bytes\n");
    if ($sigsec == 0xAAAAAAAA) {
#	print("  main:\n");
        read(C, $body, $sizsec);
        @arr = unpack("c*", $body);
        decrypt();
	$id1 = readil(0);
	$id2 = readil(4);          
	if(($id2&0x3F000800) == 0x3F000800)
		{$visible=1;
		 $viscount++;}
	  else
		{$visible=0;}
#	$visible=sprintf("%.8Xh ", $id2) . $visible;
#	print("   ID=$id1/$id2\n");
	$idxxx = 0;
	$s = "";
	while ($c = readbl(12+$idxxx++)) {
		$s = $s . chr($c & 0xFF);
	}
	$octets = Encode::decode("cp866", $s);
#	$s_enc=from_to($s,"cp866", "cp866");
	$s_enc=Encode::encode("cp1251",$octets);
	$logs{$lgn}=$s_enc;
	$vis{$lgn}=$visible;
	if ($crcsec!=testchecksum()) { die "bad chksum!\n"; }
	last;
    }
#    elsif ($sigsec == 0x55555555) {
#	print("  additional:\n");
#        read(C, $body, $sizsec);
#        @arr = unpack("c*", $body);
#        decrypt();
#	if ($crcsec!=testchecksum()) { die "bad chksum!\n"; }
#    }
#    elsif ($sigsec == 0x40A40A40) {
#	print("  memory:\n");
#        read(C, $body, $sizsec);
#        @arr = unpack("c*", $body);
#        decrypt();
#	if ($crcsec!=testchecksum()) { die "bad chksum!\n"; }
#    }
#    elsif ($sigsec == 0x3A5A3A5A) {
#	print("  in-the-pack:\n");
#        read(C, $body, $sizsec);
#        @arr = unpack("c*", $body);
#        decrypt();
#	if ($crcsec!=testchecksum()) { die "bad chksum!\n"; }
#	readpack();
#    }
#    elsif ($sigsec == 0xDE0DE0DE) {
#	print("  on-the-body:\n");
#        read(C, $body, $sizsec);
#        @arr = unpack("c*", $body);
#	decrypt();
#	if ($crcsec!=testchecksum()) { die "bad chksum!\n"; }
#	readpack();
#    }
#    elsif ($sigsec == 0x41392521) {
#	$keysec = $keysec >> 0x10;
#	if ($keysec & 0x0001) { $tmp=readb(C);}
#	$monsters_kills=(readi(C) ^ 0x1529251) & 0xFFFFFFFF;
#	if ($keysec & 0x0002) {$tmp=readb(C);}
#	$players_kills=(readi(C)+ $monsters_kills *5+0x13141516) & 0xFFFFFFFF;
#	if ($keysec & 0x0004) {$tmp=readb(C);}
#	$frags = (readi(C) + $players_kills * 7 + 0xABCDEF) & 0xFFFFFFFF;
#	if ($keysec & 0x0008) {$tmp=readb(C);}
#	$deaths = (readi(C) ^ 0x17FF12AA ) & 0xFFFFFFFF;
#	if ($keysec & 0x0010) {$tmp=readb(C);}
#	$money = (readi(C) + $monsters_kills *3 - 0x21524542) & 0xFFFFFFFF;
#	if ($keysec & 0x0020) {$tmp=readb(C);}
#	$body_level = (readb(C) + $money * 0x11 + $monsters_kills * 0x13) & 0xFF;
#	if ($keysec & 0x0040) {$tmp=readb(C);}
#	$reaction_level = (readb(C) + $body_level*3) & 0xFF;
#	if ($keysec & 0x0080) {$tmp=readb(C);}
#	$mind_level = (readb(C) + $body_level + $reaction_level*5) & 0xFF;
#	if ($keysec & 0x0100) {$tmp=readb(C);}
#	$spirit_level = (readb(C) + $body_level * 7 + $mind_level*9) & 0xFF;
#	if ($keysec & 0x4000) {$tmp=readb(C);}
#	$spells = (readi(C) - 0x10121974) & 0xFFFFFFFF;
#	if ($keysec & 0x2000) {$tmp=readb(C);}
#	$active_spell = (readi(C)) & 0xFFFFFFFF;
#	if ($keysec & 0x0200) {$tmp=readb(C);}
#	$exp_fire_blade = (readi(C) ^ 0xDADEDADE) & 0xFFFFFFFF;
#	if ($keysec & 0x0400) {$tmp=readb(C);}
#	$exp_water_axe = (readi(C) - $exp_fire_blade*0x771) & 0xFFFFFFFF;
#	if ($keysec & 0x0800) {$tmp=readb(C);}
#	$exp_air_bludgeon = (readi(C) - $exp_water_axe*0x771) & 0xFFFFFFFF;
#	if ($keysec & 0x1000) {$tmp=readb(C);}
#	$exp_earth_pike = (readi(C) - $exp_air_bludgeon*0x771) & 0xFFFFFFFF;
#	if ($keysec & 0x2000) {$tmp=readb(C);}
#	$exp_astral_shooting = (readi(C) - $exp_earth_pike*0x771) & 0xFFFFFFFF;
#	$idxx = 0;
#	print("  other settings:\n");
#	print("   spell-list:\n");
#	foreach $i (@sps) {
#		if ($spells & (1 << $idxx)) {
#			print("    $i\n");
#		}
#		$idxx++;
#	}
f#	print("   fire/blade exp: $exp_fire_blade\n");
#	print("   water/axe exp: $exp_water_axe\n");
#	print("   air/bludgeon exp: $exp_air_bludgeon\n");
#	print("   earth/pike exp: $exp_earth_pike\n");
#	print("   astral/shooting exp: $exp_astral_shooting\n");
#    }
#    else {
#	die "invalid section's signature!\n";
#    }
}
 

	  
 	}
	@tmp=keys %logs;
	print OUT_ADM "<server_id>";  
        print OUT_ADM $server_id;
	print OUT_ADM "</server_id>";

        print OUT_ADM "<server_name>";
        print OUT_ADM $server_list{$servern};
        print OUT_ADM "</server_name>";

        my ($sec,$min,$hour,$h1,$h2,$h3,$h4,$h5,$h6)=localtime($time_cur+21*60*60);
	if($min<10){$min="0" .$min;}
        print  OUT_ADM "<server_time>";
	print  OUT_ADM "$hour:$min";
        print  OUT_ADM "</server_time>";

	print  OUT_ADM "<map_name>"; 
	print  OUT_ADM "$map_name";
	print  OUT_ADM "</map_name>";
        
        print  OUT_ADM "<map_level>";
	print  OUT_ADM @levels[$map_level];
        print  OUT_ADM "</map_level>";
        
        print  OUT_ADM "<map_size>";
	print  OUT_ADM $X_size . "x" . $Y_size;
        print  OUT_ADM "</map_size>";

        print  OUT_ADM "<game_type>";
	print  OUT_ADM $type{$GameType};
        print  OUT_ADM "</game_type>";
        

        print  OUT_ADM "<visible_count>";
	print  OUT_ADM $viscount;
        print  OUT_ADM "</visible_count>";

        print  OUT_ADM "<all_count>";
	print  OUT_ADM $#tmp+1;
        print  OUT_ADM "</all_count>";

        if($#tmp>-1) 
        {

            print OUT_ADM "<players>";
            while ( my ($key, $value) = each(%logs) )
            {
            $lin_name=$logged_in . $key;

            if(open(LIN,$lin_name))
            {
                ($lin_ip,$lin_time)=<LIN>;
                close(LIN);
                $lin_ip=~/^(\d+\.\d+\.\d+\.\d+:\d+)/;
                $lin_ip=$1;
                chomp $lin_time;
            }
            else
            { 
                ($lin_ip,$lin_time)=("0.0.0.0:0","0.0.0 0:0:0.0");
            }
            my $SymbLine;
            foreach $SymbLine (&HTMLSymb) 
            {
		($ascii, $html) = split(/\s\s*/,$SymbLine);
		$value =~ s/$ascii/$html/g;
	    }
	    $value =~ s/\s/&#160;/g;

                print OUT_ADM "<player>";

                print OUT_ADM "<link_to_details>";
                print OUT_ADM "href=http://hat.allods.net:8030/Scripts/stat/show.php?a2c=".$name.$key;
                print OUT_ADM "</link_to_details>";

                print OUT_ADM "<name>";
                print OUT_ADM $value;
                print OUT_ADM "</name>";


                print OUT_ADM "<login>";
                print OUT_ADM $key;
                print OUT_ADM "</login>";
                
                print OUT_ADM "<ip>";
                print OUT_ADM $lin_ip;
                print OUT_ADM "</ip>";
                
                print OUT_ADM "<time>";
                print OUT_ADM $lin_time;
                print OUT_ADM "</time>";
                
                print OUT_ADM "<visibility>";
                print OUT_ADM $visprt{$vis{$key}};
                print OUT_ADM "</visibility>";

                print OUT_ADM "</player>";
            }
            print OUT_ADM "</players>";
         }
   print OUT_ADM "</server>";
 }
print OUT_ADM "</status>";
close OUT_ADM;

open(LOG,$logfile);
print LOG "stop ", $args, "\n";
close(LOG);

#unlink $lockfile;


# for (parse_dir(`dir`)) {
# print "1\n";
#     ($name, $type, $size, $mtime, $mode) = ;
#	 print "$name $type $size $mtime $mode\n";@$_
#    # next if $type ne 'f'; # plain file
#     #...
# }


# HTML Codes
sub HTMLSymb {
	return (
	"&	&#38;",
	"\"	&quot;",
	"<	&lt;",
	">	&gt;",
	"©	&copy;",
	"®	&reg;",
#	"Ж	&AElig;",
#	"Б	&Aacute;",
#	"В	&Acirc;",
#	"А	&Agrave;",
#	"Е	&Aring;",
#	"Г	&Atilde;",
#	"Д	&Auml;",
#	"З	&Ccedil;",
#	"Р	&ETH;",
#	"Й	&Eacute;",
#	"К	&Ecirc;",
#	"И	&Egrave;",
#	"Л	&Euml;",
#	"Н	&Iacute;",
#	"О	&Icirc;",
#	"М	&Igrave;",
#	"П	&Iuml;",
#	"С	&Ntilde;",
#	"У	&Oacute;",
#	"Ф	&Ocirc;",
#	"Т	&Ograve;",
#	"Ш	&Oslash;",
#	"Х	&Otilde;",
#	"Ц	&Ouml;",
#	"Ю	&THORN;",
#	"Ъ	&Uacute;",
#	"Ы	&Ucirc;",
#	"Щ	&Ugrave;",
#	"Ь	&Uuml;",
#	"Э	&Yacute;",
#	"б	&aacute;",
#	"в	&acirc;",
#	"ж	&aelig;",
#	"а	&agrave;",
#	"е	&aring;",
#	"г	&atilde;",
#	"д	&auml;",
#	"з	&ccedil;",
#	"й	&eacute;",
#	"к	&ecirc;",
#	"и	&egrave;",
#	"р	&eth;",
#	"л	&euml;",
#	"н	&iacute;",
#	"о	&icirc;",
#	"м	&igrave;",
#	"п	&iuml;",
#	"с	&ntilde;",
#	"у	&oacute;",
#	"ф	&ocirc;",
#	"т	&ograve;",
#	"ш	&oslash;",
#	"х	&otilde;",
#	"ц	&ouml;",
#	"Я	&szlig;",
#	"ю	&thorn;",
#	"ъ	&uacute;",
#	"ы	&ucirc;",
#	"щ	&ugrave;",
#	"ь	&uuml;",
#	"э	&yacute;",
#	"я	&yuml;",
#	" 	&#160;",
	"Ў	&#161;",
	"ў	&#162;",
	"Ј	&#163;",
	"Ґ	&#165;",
	"¦	&#166;",
	"§	&#167;",
	"Ё	&#168;",
	"©	&#169;",
	"Є	&#170;",
	"«	&#171;",
	"¬	&#172;",
	"­	&#173;",
	"®	&#174;",
	"Ї	&#175;",
	"°	&#176;",
	"±	&#177;",
	"І	&#178;",
	"і	&#179;",
	"ґ	&#180;",
	"µ	&#181;",
	"¶	&#182;",
	"·	&#183;",
	"ё	&#184;",
	"№	&#185;",
	"є	&#186;",
	"»	&#187;",
	"ј	&#188;",
	"Ѕ	&#189;",
	"ѕ	&#190;",
	"ї	&#191;",
#	"Ч	&#215;",
#	"Ю	&#222;",
#	"ч	&#247;"
)
}
