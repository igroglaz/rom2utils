#use File::Listing qw(parse_dir);

$logfile=">>itemlog.log";
$lockfile="itemlog.lck";
$args= "itemlog " . time();
@timedata=localtime(time);
#print join(" ",@timedata);
$year=$timedata[5]+1900;
$month=$timedata[4]+1;
$curtime= $timedata[3] . "/" . $month  . "/" . $year  . " $timedata[2]:$timedata[1]:$timedata[0]";
print "$curtime\n";
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

my %item_penalty=(
"0E0E"=>"50",#"Scroll Acid Stream",
"0E19"=>"1",#"Scroll Bless",
"0E0C"=>"50",#"Scroll Blizzard",
"0E1B"=>"50",#"Scroll Control Spirit",
"0E21"=>"5",#"Scroll Curse",
"0E13"=>"1",#"Scroll Darkness",
"0E15"=>"1",#"Scroll Diamond Dust",
"0E1F"=>"1",#"Scroll Drain Life",
"0E06"=>"1",#"Scroll Fire Arrow",
"0E07"=>"5",#"Scroll Fire Ball",
"0E08"=>"10",#"Scroll Fire Wall",
"0E1A"=>"1",#"Scroll Haste",
"0E1D"=>"1",#"Scroll Heal",
"0E0A"=>"1",#"Scroll Ice Missile",
"0E11"=>"30",#"Scroll Invisibility",
"0E14"=>"1",#"Scroll Light",
"0E0F"=>"1",#"Scroll Lightning",
"0E0B"=>"30",#"Scroll Poison Cloud",
"0E10"=>"50",#"Scroll Prismatic Spray",
"0E12"=>"5",#"Scroll Protection from Air",
"0E18"=>"5",#"Scroll Protection from Earth",
"0E09"=>"5",#"Scroll Protection from Fire",
"0E0D"=>"5",#"Scroll Protection from Water",
"0E20"=>"1",#"Scroll Shield",
"0E22"=>"1",#"Scroll Slow",
"0E17"=>"200",#"Scroll Stone Curse",
"0E1E"=>"50",#"Scroll Summon",
"0E1C"=>"50",#"Scroll Teleport",
"0E16"=>"10",#"Scroll Wall of Earth",
"0E2B"=>"50",#"SuperScroll Acid Stream",
"0E36"=>"1",#"SuperScroll Bless",
"0E29"=>"50",#"SuperScroll Blizzard",
"0E38"=>"50",#"SuperScroll Control Spirit",
"0E3E"=>"5",#"SuperScroll Curse",
"0E30"=>"1",#"SuperScroll Darkness",
"0E32"=>"1",#"SuperScroll Diamond Dust",
"0E3C"=>"1",#"SuperScroll Drain Life",
"0E23"=>"1",#"SuperScroll Fire Arrow",
"0E24"=>"5",#"SuperScroll Fire Ball",
"0E25"=>"10",#"SuperScroll Fire Wall",
"0E37"=>"1",#"SuperScroll Haste",
"0E3A"=>"1",#"SuperScroll Heal",
"0E27"=>"1",#"SuperScroll Ice Missile",
"0E2E"=>"30",#"SuperScroll Invisibility",
"0E31"=>"1",#"SuperScroll Light",
"0E2C"=>"1",#"SuperScroll Lightning",
"0E28"=>"30",#"SuperScroll Poison Cloud",
"0E2D"=>"50",#"SuperScroll Prismatic Spray",
"0E2F"=>"5",#"SuperScroll Protection from Air",
"0E35"=>"5",#"SuperScroll Protection from Earth",
"0E26"=>"5",#"SuperScroll Protection from Fire",
"0E2A"=>"5",#"SuperScroll Protection from Water",
"0E3D"=>"1",#"SuperScroll Shield",
"0E3F"=>"1",#"SuperScroll Slow",
"0E34"=>"200",#"SuperScroll Stone Curse",
"0E3B"=>"50",#"SuperScroll Summon",
"0E39"=>"50",#"SuperScroll Teleport",
"0E33"=>"10");#"SuperScroll Wall of Earth"


my @sps_penalty = (
0,#  "None",                  # incorrect spell
1,#  "Fire Arrow",            # огненная стрела
5,#  "Fire Ball",             # огненный шар
10,#  "Fire Wall",             # огненная стена
5,#  "Protection from Fire",  # защита от магии огня

1,#  "Ice Missile",           # ледяная стрела
30,#  "Poison Cloud",          # ядовитый туман
50,#  "Blizzard",              # град
5,#  "Protection from Water", # защита от магии воды

50,#  "Acid Stream",           # кислотная атака
1,#  "Lightning",             # молния
50,#  "Prismatic Spray",       # радужная молния
30,#  "Invisibility",          # невидимость
5,#  "Protection from Air",   # защита от магии воздуха

1,#  "Darkness",              # тьма
1,#  "Light",                 # свечение
1,#  "Diamond Dust",          # каменная стрела
10,#  "Wall of Earth",         # каменная стена
200,#  "Stone Curse",           # каменное проклятие
5,#  "Protection from Earth", # защита от магии огня

1,#  "Bless",          # благословение
1,#  "Haste",          # ускорение
50,#  "Control Spirit", # перерождение
50,#  "Teleport",      # телепорт

1,#  "Heal",          # исцеление
50,#  "Summon",        # зов
1,#  "Drain Life",    # вампиризм
1,#  "Shield",        # магический щит

5,#  "Curse",         # проклятие
1);#  "Slow");           # замедлени

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
#	    print("  found simple object: ID=$IDh, count=$cc\n");
	    print(OUT_STD " ID=$IDh,count=$cc;");
	} elsif (($b0 & 0x20) == 0x20) {
	    $price = readil($idx); $idx+=4;
	    $ccc = $b0 & 0xF;
	    print(OUT_STD "ID=$IDh,price=$price {");
	    $i = 0;
	    while ($i++ < $ccc) {
		$eid = readbl($idx); $idx++;
	        $eIDh = sprintf("%.2Xh", $eid);
		$evl = readbl($idx); $idx++;
		print(OUT_STD "$eIDh,$evl:");
                if($eid==2) {$body_pack=$body_pack+$evl;}
                if($eid==3) {$mind_pack=$mind_pack+$evl;}
                if($eid==4) {$reaction_pack=$reaction_pack+$evl;}
                if($eid==5) {$spirit_pack=$spirit_pack+$evl;}
		if($eid==41) {$res=$res+@sps_penalty[$evl];}
#		if ($eid == 0x29 || ($eid >= 0x2C && $eid <=0x30)) {
#		    $ccc--;
#		}
	    }
	    print OUT_STD  "};";
	} elsif (!$b0) {
	    $ccc = readwl($idx); $idx+=2;
	    print(OUT_STD "ID=$IDh,count=$ccc;");
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
	if(/server=\d+\.\d+\.\d+\.\d+:\d+,(.+),(.*)/) 
	  {
	    $server_list{$1}=$2;
#	    print "$1, $2\n";
	   }
   }
   
open(OUT_STD,">>C:\\Games\\Allods\\logs\\itemlog.log");   
#open(OUT_ADM,">C:\\Games\\Allods\\monitoradm.txt");   

#print "C:\\Games\\Allods\\PLScript\\scan.ini\n";
$servern=0;
open(SERVLIST, "C:\\Games\\Allods\\PLScript\\scan.ini");
$section="";
while(<SERVLIST>)
 { 
   $servern=$servern+1;
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

$body_total=0;
$mind_total=0;
$reaction_total=0;
$spirit_total=0;
$res_total=0;
$propitost=0;
$indexPK=0;


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
	print OUT_STD "$curtime SERVER $server_id $lgn " . $s_enc . "\n";
	$vis{$lgn}=$visible;
	if ($crcsec!=testchecksum()) { die "bad chksum!\n"; }
#	last;
       }
    elsif ($sigsec == 0x55555555) {
#	print("  additional:\n");
        read(C, $body, $sizsec);
        @arr = unpack("c*", $body);
        decrypt();
	if ($crcsec!=testchecksum()) { die "bad chksum!\n"; }
    }
    elsif ($sigsec == 0x40A40A40) {
#	print("  memory:\n");
        read(C, $body, $sizsec);
        @arr = unpack("c*", $body);
        decrypt();
	if ($crcsec!=testchecksum()) { die "bad chksum!\n"; }
    }
    elsif ($sigsec == 0x3A5A3A5A) {
	print(OUT_STD "  in-the-pack:\n");
        read(C, $body, $sizsec);
        @arr = unpack("c*", $body);
        decrypt();
	if ($crcsec!=testchecksum()) { die "bad chksum!\n"; }
	       $body_pack=0;
               $mind_pack=0;
               $reaction_pack=0;
               $spirit_pack=0;
		$res=0;
	readpack();
	$body_total=$body_total+$body_pack;
	$mind_total=$mind_total+$mind_pack;
	$reaction_total=$reaction_total+$reaction_pack;
	$spirit_total=$spirit_total+$spirit_pack;
	$res_total=$res_total+$res;
	print OUT_STD  "\n";
    }
    elsif ($sigsec == 0xDE0DE0DE) {
	print(OUT_STD "  on-the-body:\n");
        read(C, $body, $sizsec);
        @arr = unpack("c*", $body);
	decrypt();
	if ($crcsec!=testchecksum()) { die "bad chksum!\n"; }
	
	       $body_pack=0;
               $mind_pack=0;
               $reaction_pack=0;
               $spirit_pack=0;
	       $res=0;
	readpack();
	$body_total=$body_total+$body_pack;
	$mind_total=$mind_total+$mind_pack;
	$reaction_total=$reaction_total+$reaction_pack;
	$spirit_total=$spirit_total+$spirit_pack;
	$res_total=$res_total+$res;
	print OUT_STD  "\n";
    }
    elsif ($sigsec == 0x41392521) {
	$keysec = $keysec >> 0x10;
	if ($keysec & 0x0001) { $tmp=readb(C);}
	$monsters_kills=(readi(C) ^ 0x1529251) & 0xFFFFFFFF;
	if ($keysec & 0x0002) {$tmp=readb(C);}
	$players_kills=(readi(C)+ $monsters_kills *5+0x13141516) & 0xFFFFFFFF;
	if ($keysec & 0x0004) {$tmp=readb(C);}
	$frags = (readi(C) + $players_kills * 7 + 0xABCDEF) & 0xFFFFFFFF;
	if ($keysec & 0x0008) {$tmp=readb(C);}
	$deaths = (readi(C) ^ 0x17FF12AA ) & 0xFFFFFFFF;
	if ($keysec & 0x0010) {$tmp=readb(C);}
	$money = (readi(C) + $monsters_kills *3 - 0x21524542) & 0xFFFFFFFF;
	if ($keysec & 0x0020) {$tmp=readb(C);}
	$body_level = (readb(C) + $money * 0x11 + $monsters_kills * 0x13) & 0xFF;
	if ($keysec & 0x0040) {$tmp=readb(C);}
	$reaction_level = (readb(C) + $body_level*3) & 0xFF;
	if ($keysec & 0x0080) {$tmp=readb(C);}
	$mind_level = (readb(C) + $body_level + $reaction_level*5) & 0xFF;
	if ($keysec & 0x0100) {$tmp=readb(C);}
	$spirit_level = (readb(C) + $body_level * 7 + $mind_level*9) & 0xFF;
	if ($keysec & 0x4000) {$tmp=readb(C);}
	$spells = (readi(C) - 0x10121974) & 0xFFFFFFFF;
	if ($keysec & 0x2000) {$tmp=readb(C);}
	$active_spell = (readi(C)) & 0xFFFFFFFF;
	if ($keysec & 0x0200) {$tmp=readb(C);}
	$exp_fire_blade = (readi(C) ^ 0xDADEDADE) & 0xFFFFFFFF;
	if ($keysec & 0x0400) {$tmp=readb(C);}
	$exp_water_axe = (readi(C) - $exp_fire_blade*0x771) & 0xFFFFFFFF;
	if ($keysec & 0x0800) {$tmp=readb(C);}
	$exp_air_bludgeon = (readi(C) - $exp_water_axe*0x771) & 0xFFFFFFFF;
	if ($keysec & 0x1000) {$tmp=readb(C);}
	$exp_earth_pike = (readi(C) - $exp_air_bludgeon*0x771) & 0xFFFFFFFF;
	if ($keysec & 0x2000) {$tmp=readb(C);}
	$exp_astral_shooting = (readi(C) - $exp_earth_pike*0x771) & 0xFFFFFFFF;
	$idxx = 0;
#	print("  other settings:\n");
#	print("   spell-list:\n");
#	foreach $i (@sps) {
#	if ($spells & (1 << $idxx)) {
#			print("    $i\n");
#		}
#		$idxx++;
#	}
#	print("   fire/blade exp: $exp_fire_blade\n");
#	print("   water/axe exp: $exp_water_axe\n");
#	print("   air/bludgeon exp: $exp_air_bludgeon\n");
#	print("   earth/pike exp: $exp_earth_pike\n");
#	print("   astral/shooting exp: $exp_astral_shooting\n");
#
	$body_total=$body_total+$body_level;
	$mind_total=$mind_total+$mind_level;
	$reaction_total=$reaction_total+$reaction_level;
	$spirit_total=$spirit_total+$spirit_level;
	foreach $i (@sps) {
	
		if ($spells & (1 << $idxx)) {
			$res=$res+@sps_penalty[$idxx];
#		    if(@book_disable[$idxx]==0) {$enable=$enable+1; } 
		}
                
		$idxx++;
	}
		$res_total=$res_total+$res;

    }
    else {
	die "invalid section's signature!\n";
    }
}
 

	  if($body_total>86) {$body_total=85;} 
if($reaction_total>55) {$reaction_total=54;} 
if($mind_total>55) {$mind_total=54;} 
if($spirit_total>60) {$spirit_total=60;} 
if($res_total>600) {$res_total=600;}
$propitost=$body_total*4 +$reaction_total +$mind_total +$spirit_total;
$index_pk= ($res_total+1+($body_total-15)/3) * ( 1 + 5*($propitost - 170+34*2)/(244+81*2 - 170-34*2));
	
print OUT_STD "Money=$money\n";
print OUT_STD "INDEXPK=$index_pk\n";

 	}
	print $server_id . "\n";
 }
close OUT_STD;

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
	"&	&amp;",
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
