#!/usr/bin/perl

my @arr;
my $keysec;

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
%effects=();
    $idx = 9;
    $size = scalar(@arr);
    while ($idx < $size) {
	%effects=();
	$itid = readwl($idx); $idx+=2;
	$b0 = readbl($idx); $idx++;
        $IDh = sprintf("%.4Xh", $itid);
	if (($b0 & 0x80) == 0x80) {
	    $cc = $b0-0x80;
	    $count=$cc;
#	    print("  found simple object: ID=$IDh, count=$cc\n");
	} elsif (($b0 & 0x20) == 0x20) {
	    $price = readil($idx); $idx+=4;
	    $ccc = $b0 & 0xF;
#	    print("  found magic object: ID=$IDh, price=$price gold\n");
	    $i = 0;
	    $count=1;	
	    while ($i++ < $ccc) {
		$eid = readbl($idx); $idx++;
	        $eIDh = sprintf("%.2Xh", $eid);
		$evl = readbl($idx); $idx++;
#		print("   effect ID=$eIDh, value=$evl\n");
		$effects{$eIDh}=$evl;
#		if ($eid == 0x29 || ($eid >= 0x2C && $eid <=0x30)) {
#		    $ccc--;
#		}
	    }
	} elsif (!$b0) {
	    $ccc = readwl($idx); $idx+=2;
#	    print("  found simple object: ID=$IDh, count=$ccc\n");
	    $count=$ccc;
	}
	$enable=$enable+validate();
#	print "enable=$enable\n";

    }
}

%disable=(
#"6582"=>"Elven Adamantium Amulet",
#"6790"=>"Elven Adamantium Chain Mail",
#"6689"=>"Elven Adamantium Full Helm",
#"6686"=>"Elven Adamantium Helm",
#"6481"=>"Elven Adamantium Ring",
#"4582"=>"Elven Gold Amulet",
"7582"=>0, #"Elven Meteoric Amulet",
"7790"=>0, #"Elven Meteoric Chain Mail",
"7689"=>0, #"Elven Meteoric Full Helm",
"7686"=>0, #"Elven Meteoric Helm",
"7481"=>0, #"Elven Meteoric Ring",
#"46C9"=>"Good Gold Full Helm",
#"46C6"=>"Good Gold Helm",
#"48D3"=>"Good Gold Plate Cuirass",
"6542"=>01, #"Rare Adamantium Amulet",
"6954"=>01, #"Rare Adamantium Bracers",
"6C5D"=>01, #"Rare Adamantium Chain Boots",
"6A59"=>01, #"Rare Adamantium Chain Gauntlets",
"6648"=>01, #"Rare Adamantium Chain Helm",
"6750"=>01, #"Rare Adamantium Chain Mail",
"6852"=>01, #"Rare Adamantium Cuirass",
"6649"=>01, #"Rare Adamantium Full Helm",
"6646"=>01, #"Rare Adamantium Helm",
"6C5E"=>01, #"Rare Adamantium Plate Boots",
"6956"=>01, #"Rare Adamantium Plate Bracers",
"6853"=>01, #"Rare Adamantium Plate Cuirass",
"664A"=>01, #"Rare Adamantium Plate Helm",
"6441"=>01, #"Rare Adamantium Ring",
"6A5A"=>01, #"Rare Adamantium Scale Gauntlets",
"6751"=>01, #"Rare Adamantium Scale Mail",
#"3542"=>"Rare Silver Amulet",
#"3441"=>"Rare Silver Ring",
#"3522"=>"Uncommon Silver Amulet",
#"3421"=>"Uncommon Silver Ring",
"6562"=>01, #"Very Rare Adamantium Amulet",
"6872"=>01, #"Very Rare Adamantium Cuirass",
"6669"=>01, #"Very Rare Adamantium Full Helm",
"6666"=>01, #"Very Rare Adamantium Helm",
"6C7E"=>01, #"Very Rare Adamantium Plate Boots",
"6976"=>01, #"Very Rare Adamantium Plate Bracers",
"6873"=>01, #"Very Rare Adamantium Plate Cuirass",
"666A"=>01, #"Very Rare Adamantium Plate Helm",
"6461"=>01, #"Very Rare Adamantium Ring",
"6A7A"=>01, #"Very Rare Adamantium Scale Gauntlets",
"6771"=>01, #"Very Rare Adamantium Scale Mail",
"E562"=>0, #"Very Rare Crystal Amulet",
"E872"=>0, #"Very Rare Crystal Cuirass",
"EC7E"=>0, #"Very Rare Crystal Plate Boots",
"E976"=>0, #"Very Rare Crystal Plate Bracers",
"E873"=>0, #"Very Rare Crystal Plate Cuirass",
"E66A"=>0, #"Very Rare Crystal Plate Helm",
"E461"=>0, #"Very Rare Crystal Ring",
"EA7A"=>0, #"Very Rare Crystal Scale Gauntlets",
"CC7C"=>0, #"Very Rare Dragon Leather Boots",
"C975"=>0, #"Very Rare Dragon Leather Bracers",
"CA78"=>0, #"Very Rare Dragon Leather Gauntlets",
"C667"=>0, #"Very Rare Dragon Leather Helm",
"C76F"=>0, #"Very Rare Dragon Leather Mail",
#"4562"=>0, #"Very Rare Gold Amulet",
#"4461"=>0, #"Very Rare Gold Ring",
"7562"=>0, #"Very Rare Meteoric Amulet",
"7872"=>0, #"Very Rare Meteoric Cuirass",
"7669"=>0, #"Very Rare Meteoric Full Helm",
"7666"=>0, #"Very Rare Meteoric Helm",
"7C7E"=>0, #"Very Rare Meteoric Plate Boots",
"7976"=>0, #"Very Rare Meteoric Plate Bracers",
"7873"=>0, #"Very Rare Meteoric Plate Cuirass",
"766A"=>0, #"Very Rare Meteoric Plate Helm",
"7461"=>0, #"Very Rare Meteoric Ring",
"7A7A"=>0, #"Very Rare Meteoric Scale Gauntlets",
"7771"=>0, #"Very Rare Meteoric Scale Mail",
#"0E01"=>"Book Air",
#"0E05"=>"Book Astral",
#"0E04"=>"Book Earth",
#"0E03"=>"Book Fire",
#"0E02"=>"Book Water",
#"0E40"=>"Potion Antipoison",
#"0E47"=>"Potion Big Healing",
#"0E4A"=>"Potion Big Mana",
"0E41"=>0, #"Potion Body",
#"0E4B"=>"Potion Fighter Bonus",
#"0E45"=>"Potion Health Regeneration",
#"0E4C"=>"Potion Mage Bonus",
#"0E48"=>"Potion Mana Regeneration",
#"0E46"=>"Potion Medium Healing",
#"0E49"=>"Potion Medium Mana",
"0E43"=>0, #"Potion Mind",
"0E42"=>0, #"Potion Reaction",
"0E44"=>0, #"Potion Spirit",
"0E0E"=>2, #"Scroll Acid Stream",
"0E19"=>50, #"Scroll Bless",
"0E0C"=>2, #"Scroll Blizzard",
"0E1B"=>2, #"Scroll Control Spirit",
"0E21"=>50, #"Scroll Curse",
"0E13"=>10, #"Scroll Darkness",
"0E15"=>50, #"Scroll Diamond Dust",
"0E1F"=>0, #"Scroll Drain Life",
"0E06"=>50, #"Scroll Fire Arrow",
"0E07"=>50, #"Scroll Fire Ball",
"0E08"=>2, #"Scroll Fire Wall",
"0E1A"=>50, #"Scroll Haste",
"0E1D"=>50, #"Scroll Heal",
"0E0A"=>50, #"Scroll Ice Missile",
"0E11"=>5, #"Scroll Invisibility",
"0E14"=>10, #"Scroll Light",
"0E0F"=>50, #"Scroll Lightning",
"0E0B"=>2, #"Scroll Poison Cloud",
"0E10"=>2, #"Scroll Prismatic Spray",
"0E12"=>50, #"Scroll Protection from Air",
"0E18"=>50, #"Scroll Protection from Earth",
"0E09"=>50, #"Scroll Protection from Fire",
"0E0D"=>50, #"Scroll Protection from Water",
"0E20"=>50, #"Scroll Shield",
"0E22"=>0, #"Scroll Slow",
"0E17"=>0, #"Scroll Stone Curse",
"0E1E"=>2, #"Scroll Summon",
"0E1C"=>7, #"Scroll Teleport",
"0E16"=>0, #"Scroll Wall of Earth",
"0E2B"=>0, #"SuperScroll Acid Stream",
"0E36"=>15, #"SuperScroll Bless",
"0E29"=>2, #"SuperScroll Blizzard",
"0E38"=>0, #"SuperScroll Control Spirit",
"0E3E"=>3, #"SuperScroll Curse",
"0E30"=>10, #"SuperScroll Darkness",
"0E32"=>50, #"SuperScroll Diamond Dust",
"0E3C"=>0, #"SuperScroll Drain Life",
"0E23"=>50, #"SuperScroll Fire Arrow",
"0E24"=>15, #"SuperScroll Fire Ball",
"0E25"=>0, #"SuperScroll Fire Wall",
"0E37"=>15, #"SuperScroll Haste",
"0E3A"=>50, #"SuperScroll Heal",
"0E27"=>50, #"SuperScroll Ice Missile",
"0E2E"=>0, #"SuperScroll Invisibility",
"0E31"=>10, #"SuperScroll Light",
"0E2C"=>50, #"SuperScroll Lightning",
"0E28"=>0, #"SuperScroll Poison Cloud",
"0E2D"=>0, #"SuperScroll Prismatic Spray",
"0E2F"=>0, #"SuperScroll Protection from Air",
"0E35"=>0, #"SuperScroll Protection from Earth",
"0E26"=>0, #"SuperScroll Protection from Fire",
"0E2A"=>0, #"SuperScroll Protection from Water",
"0E3D"=>2, #"SuperScroll Shield",
"0E3F"=>0, #"SuperScroll Slow",
"0E34"=>0, #"SuperScroll Stone Curse",
"0E3B"=>0, #"SuperScroll Summon",
"0E39"=>0, #"SuperScroll Teleport",
"0E33"=>0, #"SuperScroll Wall of Earth",
#"6245"=>"Rare Adamantium Large Shield",
#"6248"=>"Rare Adamantium Tower Shield",
#"6265"=>"Very Rare Adamantium Large Shield",
"C266"=>0, #"Very Rare Dragon Leather Large Shield",
"C263"=>0, #"Very Rare Dragon Leather Small Shield",
#"6182"=>"Elven Adamantium Dagger",
#"6184"=>"Elven Adamantium Long Sword",
#"6183"=>"Elven Adamantium Short Sword",
#"6186"=>"Elven Adamantium Two Handed Sword",
"7182"=>0, #"Elven Meteoric Dagger",
"7184"=>0, #"Elven Meteoric Long Sword",
"7183"=>0, #"Elven Meteoric Short Sword",
"7186"=>0, #"Elven Meteoric Two Handed Sword",
#"31C6"=>"Good Silver Two Handed Sword",
#"6152"=>"Rare Adamantium Axe",
#"6145"=>"Rare Adamantium Bastard Sword",
#"6156"=>"Rare Adamantium Crossbow",
#"6142"=>"Rare Adamantium Dagger",
#"6150"=>"Rare Adamantium Halberd",
#"6144"=>"Rare Adamantium Long Sword",
#"6149"=>"Rare Adamantium Mace",
#"614A"=>"Rare Adamantium Morning Star",
#"614F"=>"Rare Adamantium Pike",
#"6143"=>"Rare Adamantium Short Sword",
#"6153"=>"Rare Adamantium Two Handed Axe",
#"6146"=>"Rare Adamantium Two Handed Sword",
#"614C"=>"Rare Adamantium War Hammer",
#"3142"=>"Rare Silver Dagger",
#"6172"=>"Very Rare Adamantium Axe",
#"6176"=>"Very Rare Adamantium Crossbow",
#"6162"=>"Very Rare Adamantium Dagger",
#"6170"=>"Very Rare Adamantium Halberd",
#"6164"=>"Very Rare Adamantium Long Sword",
#"616F"=>"Very Rare Adamantium Pike",
#"6163"=>"Very Rare Adamantium Short Sword",
#"6173"=>"Very Rare Adamantium Two Handed Axe",
#"6166"=>"Very Rare Adamantium Two Handed Sword",
"E172"=>0, #"Very Rare Crystal Axe",
"E164"=>0, #"Very Rare Crystal Long Sword",
"E169"=>0, #"Very Rare Crystal Mace",
"E16A"=>0, #"Very Rare Crystal Morning Star",
"E16F"=>0, #"Very Rare Crystal Pike",
"E173"=>0, #"Very Rare Crystal Two Handed Axe",
"E166"=>0, #"Very Rare Crystal Two Handed Sword",
"E16C"=>0, #"Very Rare Crystal War Hammer",
#"4162"=>"Very Rare Gold Dagger",
"7172"=>0, #"Very Rare Meteoric Axe",
"7176"=>0, #"Very Rare Meteoric Crossbow",
"7162"=>0, #"Very Rare Meteoric Dagger",
"7170"=>0, #"Very Rare Meteoric Halberd",
"7164"=>0, #"Very Rare Meteoric Long Sword",
"7169"=>0, #"Very Rare Meteoric Mace",
"716A"=>0, #"Very Rare Meteoric Morning Star",
"716F"=>0, #"Very Rare Meteoric Pike",
"7163"=>0, #"Very Rare Meteoric Short Sword",
"7173"=>0, #"Very Rare Meteoric Two Handed Axe",
"7166"=>0, #"Very Rare Meteoric Two Handed Sword",
"716C"=>0, #"Very Rare Meteoric War Hammer"
);


 @sps_disable = (
  0,                  # incorrect spell

  100,            # огненная стрела
  0,             # огненный шар
  0,             # огненная стена
  0,#"Protection from Fire",  # защита от магии огня
  100,#"Ice Missile",           # ледяная стрела
  0,#"Poison Cloud",          # ядовитый туман
  0,#"Blizzard",              # град
  0,#"Protection from Water", # защита от магии воды
  0,#"Acid Stream",           # кислотная атака
  100,#"Lightning",             # молния

  50,#"Prismatic Spray",       # радужная молния
  0,#"Invisibility",          # невидимость
  0,#"Protection from Air",   # защита от магии воздуха
  0,#"Darkness",              # тьма
  0,#"Light",                 # свечение
  100,#"Diamond Dust",          # каменная стрела
  0,#"Wall of Earth",         # каменная стена
  0,#"Stone Curse",           # каменное проклятие
  0,#"Protection from Earth", # защита от магии огня
  0,#"Bless",          # благословение

  0,#"Haste",          # ускорение
  0,#"Control Spirit", # перерождение
  0,#"Teleport",      # телепорт
  0,#"Heal",          # исцеление
  0,#"Summon",        # зов
  100,#"Drain Life",    # вампиризм
  0,#"Shield",        # магический щит
  0,#"Curse",         # проклятие
  0);#"Slow");           # замедление

 @book_disable = (
  1, #"None",                  # incorrect spell

  1, #"Fire Arrow",            # огненная стрела
  1, #"Fire Ball",             # огненный шар
  0, #"Fire Wall",             # огненная стена
  1, #"Protection from Fire",  # защита от магии огня
  1, #"Ice Missile",           # ледяная стрела
  0, #"Poison Cloud",          # ядовитый туман
  0, #"Blizzard",              # град
  1, #"Protection from Water", # защита от магии воды
  0, #"Acid Stream",           # кислотная атака
  1, #"Lightning",             # молния

  0, #"Prismatic Spray",       # радужная молния
  0, #"Invisibility",          # невидимость
  1, #"Protection from Air",   # защита от магии воздуха
  0, #"Darkness",              # тьма
  0, #"Light",                 # свечение
  1, #"Diamond Dust",          # каменная стрела
  0, #"Wall of Earth",         # каменная стена
  0, #"Stone Curse",           # каменное проклятие
  1, #"Protection from Earth", # защита от магии огня
  1, #"Bless",          # благословение

  1, #"Haste",          # ускорение
  0, #"Control Spirit", # перерождение
  0, #"Teleport",      # телепорт
  1, #"Heal",          # исцеление
  0, #"Summon",        # зов
  1, #"Drain Life",    # вампиризм
  1, #"Shield",        # магический щит
  0, #"Curse",         # проклятие
  0); #"Slow");           # замедление


sub validate
 {
#  my $IDh=shift;
#  my $price=shift;
#  my %effects=shift;
#print "$IDh\n";
#print "$IDh $count\n";
 while (my ($key,$value)=each(%disable))
  {
    if(($IDh=~/^$key/)&&($count>$value)) { return 1;}
  }
$spell_type=-1;
$spell_skill=-1;
 while ( my ($key, $value) = each(%effects) )
       {  	 if(($key=~/02h/)&&($value>1))	{return 1}; 
	 if(($key=="08h")&&($value>150))	{return 1}; 
	 if(($key=="0Bh")&&($value>150))	{return 1}; 
	 if(($key=="29h")) {
           if($spell_skill==-1) {
              if(@sps_disable[$value]==0){return 1;}
                else {
	          $spell_type=$value;
                }
              }
              elsif($spell_skill>@sps_disable[$value]) {$spell_type=-1;$spell_skill=-1;return 1;}
                else{$spell_type=-1;$spell_skill=-1;}
	   }
	 if(($key=="32h")){
           if($spell_type<0) {
             $spell_skill=$value;           
             }
	     elsif($spell_skill>@sps_disable[$spell_type]) {$spell_type=-1;$spell_skill=-1;return 1;}
                else{$spell_type=-1;$spell_skill=-1;}
           } 
	 if(($key=="2Ah")&&(@book_disable[$value]<0)) {
           return 1;
           }
	}

   return 0;
 }


sub readi($) {
    my $f = shift;
	my $a= 0;
	my $b=0;
	my $c=0;
	my $d=0;
    read($f, $a, 1);
    read($f, $b, 1);
    read($f, $c, 1);
    read($f, $d, 1);
    return ord($a) | (ord($b) << 8) | (ord($c) << 16) | (ord($d) << 24);
}
		   
$fn = shift;
if (!$fn) {
    $fn = "test.lgn";
}
open(C, $fn) || die "Can't open character file: $!!\n";
binmode(C);

$id1 = shift;
$id2 = shift;

if (!$id1 && !$id2) {
	$id1 = 0xFE72C55B;
	$id2 = 0;
}

seek(C, 0x104, 0);
my $c = readi(C);
$found = 0;
#print("$c characters total\n");

for ($i = 0; $i < $c; ++$i) {
	$i1 = readi(C);
	$i2 = readi(C);
	$i1h = sprintf("0x%.8X",$i1);
	$i2h = sprintf("0x%.8X",$i2);
	$id1h = sprintf("0x%.8X",$id1);
	$id2h = sprintf("0x%.8X",$id2);
#	print("$id1h, $id2h -- $i1h, $i2h   $i,$c\n");
	if (($i1 == $id1) && ($i2 == $id2)) {
		$found = 1;
		last;
	}
}

if (!$found) { die "Character ($id1, $id2) not found!\n"; }

seek(C, 0x188, 0);

$offs = 0x234;

for ($j = 0; $j < $i; ++$j) {
	$offs += readi(C);
}
$sz = readi(C);

seek(C, $offs, 0);





$enable=0;

#$fn = shift;
#if (!$fn) {
#    $fn = "test.a2c";
#}
#my $C=1;
#binmode(STDIN);
#$fn="<&STDIN";
#open(C, $fn) || die "Can't open character file: $!!\n";
#binmode C;

$sig=readi(C);
#print ;

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
#	print("   ID=$id1/$id2\n");
	$idxxx = 0;
	$s = "";
	while ($c = readbl(12+$idxxx++)) {
		$s = $s . chr($c & 0xFF);
	}
#	print("   nick: '$s'\n");
	if ($crcsec!=testchecksum()) { die "bad chksum!\n"; }
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
#	print("  in-the-pack:\n");
        read(C, $body, $sizsec);
        @arr = unpack("c*", $body);
        decrypt();
	if ($crcsec!=testchecksum()) { die "bad chksum!\n"; }
	readpack();
    }
    elsif ($sigsec == 0xDE0DE0DE) {
#	print("  on-the-body:\n");
        read(C, $body, $sizsec);
        @arr = unpack("c*", $body);
	decrypt();
	if ($crcsec!=testchecksum()) { die "bad chksum!\n"; }
	readpack();
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
#	print("Body: $body_level\n");
	if ($body_level>44) {$enable=$enable+1;}
#	print("Reaction: $reaction_level\n");
	if ($reaction_level>44) {$enable=$enable+1;}
#	print("Mind: $mind_level\n");
	if ($mind_level>44) {$enable=$enable+1;}
#	print("Spirit: $spirit_level\n");
	if ($spirit_level>44) {$enable=$enable+1;}
        if ($spirit_level+$body_level+$reaction_level+$mind_level>138) {$enable=$enable+1;}
#	print("  other settings:\n");
#	print("   spell-list:\n");
	foreach $i (@sps) {
		if ($spells & (1 << $idxx)) {
#			print("    $i\n");
		    if(@book_disable[$idxx]==0) {$enable=$enable+1; } 
		}
		$idxx++;
	}
#	print("   fire/blade exp: $exp_fire_blade\n");
#	print("   water/axe exp: $exp_water_axe\n");
#	print("   air/bludgeon exp: $exp_air_bludgeon\n");
#	print("   earth/pike exp: $exp_earth_pike\n");
#	print("   astral/shooting exp: $exp_astral_shooting\n");
    }
    else {
#	die "invalid section's signature!\n";
    }
}
#print $enable . "\n";
if($enable>0){$enable=1;}
exit $enable;

