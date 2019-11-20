#!/usr/bin/perl

#(C)LAS 2011

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
	} elsif (($b0 & 0x20) == 0x20) {
	    $price = readil($idx); $idx+=4;
	    $ccc = $b0 & 0xF;
	    $i = 0;
	    $count=1;	
	    while ($i++ < $ccc) {
		$eid = readbl($idx); $idx++;
	        $eIDh = sprintf("%.2Xh", $eid);
		$evl = readbl($idx); $idx++;
		$effects{$eIDh}=$evl;
	    }
	} elsif (!$b0) {
	    $ccc = readwl($idx); $idx+=2;
	    $count=$ccc;
	}
	$enable=$enable+validate();
    }
}

%disable=(
#"812D"=>0, #"Uncommon Wood Staff", #маг - палка
#"810D"=>0, #"Common Wood Staff", #волшебница - палка
#"F82B"=>0, #"Uncommon None Cloak", #плащ маг/волшебница
#"F72D"=>0, #"Uncommon None Robe", #роба маг/волшебница
#"0C1E"=>0, #"Common Iron Plate Boots", #ноги воин/амазонка
#"0A1A"=>0, #"Common Iron Scale Gauntlets", #перчи воин/амазонка
#"0916"=>0, #"Common Iron Plate Bracers", #наручи
#"0813"=>0, #"Common Iron Plate Cuirass", #кираса
#"B70F"=>0, #"Common Hard Leather Mail", #подкира
#"010F"=>0, #"Common Iron Pike", #копа
#"0109"=>0, #"Common Iron Mace", #дубина
#"0104"=>0, #"Common Iron Long Sword", #меч
#"0112"=>0, #"Common Iron Axe", #топор

#"0E01"=>0, #"Book Air",
#"0E05"=>0, #"Book Astral",
#"0E04"=>0, #"Book Earth",
#"0E03"=>0, #"Book Fire",
#"0E02"=>0, #"Book Water",

#"F8AB"=>0, #"Bad None Cloak",
#"F7AE"=>0, #"Bad None Dress",
#"F7AD"=>0, #"Bad None Robe",
#"1502"=>0, #"Common Bronze Amulet",
#"1914"=>0, #"Common Bronze Bracers",
#"1812"=>0, #"Common Bronze Cuirass",
#"1606"=>0, #"Common Bronze Helm",
#"1C1E"=>0, #"Common Bronze Plate Boots",
#"1916"=>0, #"Common Bronze Plate Bracers",
#"1813"=>0, #"Common Bronze Plate Cuirass",
#"160A"=>0, #"Common Bronze Plate Helm",
#"1A1A"=>0, #"Common Bronze Scale Gauntlets",
#"BC1C"=>0, #"Common Hard Leather Boots",
#"B915"=>0, #"Common Hard Leather Bracers",
#"BA18"=>0, #"Common Hard Leather Gauntlets",
#"B607"=>0, #"Common Hard Leather Helm",
#"0502"=>0, #"Common Iron Amulet",
#"0914"=>0, #"Common Iron Bracers",
#"0812"=>0, #"Common Iron Cuirass",
#"0606"=>0, #"Common Iron Helm",
#"060A"=>0, #"Common Iron Plate Helm",
#"AC1C"=>0, #"Common Leather Boots",
#"A915"=>0, #"Common Leather Bracers",
#"AA18"=>0, #"Common Leather Gauntlets",
#"A607"=>0, #"Common Leather Helm",
#"A70F"=>0, #"Common Leather Mail",
#"F604"=>0, #"Common None Cap",
#"F80C"=>0, #"Common None Cape",
#"F80B"=>0, #"Common None Cloak",
#"F70E"=>0, #"Common None Dress",
#"FA17"=>0, #"Common None Gloves",
#"F603"=>0, #"Common None Hat",
#"F605"=>0, #"Common None Low Hat",
#"F70D"=>0, #"Common None Robe",
#"FC1B"=>0, #"Common None Shoes",
#"6582"=>0, #"Elven Adamantium Amulet",
#"6790"=>0, #"Elven Adamantium Chain Mail",
#"6689"=>0, #"Elven Adamantium Full Helm",
#"6686"=>0, #"Elven Adamantium Helm",
#"6481"=>0, #"Elven Adamantium Ring",
#"4582"=>0, #"Elven Gold Amulet",
"7582"=>0, #"Elven Meteoric Amulet",
"7790"=>0, #"Elven Meteoric Chain Mail",
"7689"=>0, #"Elven Meteoric Full Helm",
"7686"=>0, #"Elven Meteoric Helm",
"7481"=>0, #"Elven Meteoric Ring",
#"5582"=>0, #"Elven Mithrill Amulet",
#"5790"=>0, #"Elven Mithrill Chain Mail",
#"5689"=>0, #"Elven Mithrill Full Helm",
#"5686"=>0, #"Elven Mithrill Helm",
#"5481"=>0, #"Elven Mithrill Ring",
#"F684"=>0, #"Elven None Cap",
#"F88C"=>0, #"Elven None Cape",
#"F78E"=>0, #"Elven None Dress",
#"FA97"=>0, #"Elven None Gloves",
#"F683"=>0, #"Elven None Hat",
#"F685"=>0, #"Elven None Low Hat",
#"FC9B"=>0, #"Elven None Shoes",
#"46C9"=>0, #"Good Gold Full Helm",
#"46C6"=>0, #"Good Gold Helm",
#"48D3"=>0, #"Good Gold Plate Cuirass",
#"58D2"=>0, #"Good Mithrill Cuirass",
#"56C9"=>0, #"Good Mithrill Full Helm",
#"F6C4"=>0, #"Good None Cap",
#"F8CB"=>0, #"Good None Cloak",
#"F7CE"=>0, #"Good None Dress",
#"6542"=>0, #"Rare Adamantium Amulet",
#"6954"=>0, #"Rare Adamantium Bracers",
#"6C5D"=>0, #"Rare Adamantium Chain Boots",
#"6A59"=>0, #"Rare Adamantium Chain Gauntlets",
#"6648"=>0, #"Rare Adamantium Chain Helm",
#"6750"=>0, #"Rare Adamantium Chain Mail",
#"6852"=>0, #"Rare Adamantium Cuirass",
#"6649"=>0, #"Rare Adamantium Full Helm",
#"6646"=>0, #"Rare Adamantium Helm",
#"6C5E"=>0, #"Rare Adamantium Plate Boots",
#"6956"=>0, #"Rare Adamantium Plate Bracers",
#"6853"=>0, #"Rare Adamantium Plate Cuirass",
#"664A"=>0, #"Rare Adamantium Plate Helm",
#"6441"=>0, #"Rare Adamantium Ring",
#"6A5A"=>0, #"Rare Adamantium Scale Gauntlets",
#"6751"=>0, #"Rare Adamantium Scale Mail",
#"BC5C"=>0, #"Rare Hard Leather Boots",
#"5542"=>0, #"Rare Mithrill Amulet",
#"5954"=>0, #"Rare Mithrill Bracers",
#"5C5D"=>0, #"Rare Mithrill Chain Boots",
#"5A59"=>0, #"Rare Mithrill Chain Gauntlets",
#"5648"=>0, #"Rare Mithrill Chain Helm",
#"5750"=>0, #"Rare Mithrill Chain Mail",
#"5852"=>0, #"Rare Mithrill Cuirass",
#"5649"=>0, #"Rare Mithrill Full Helm",
#"5646"=>0, #"Rare Mithrill Helm",
#"5C5E"=>0, #"Rare Mithrill Plate Boots",
#"5956"=>0, #"Rare Mithrill Plate Bracers",
#"5853"=>0, #"Rare Mithrill Plate Cuirass",
#"564A"=>0, #"Rare Mithrill Plate Helm",
#"5441"=>0, #"Rare Mithrill Ring",
#"5A5A"=>0, #"Rare Mithrill Scale Gauntlets",
#"5751"=>0, #"Rare Mithrill Scale Mail",
#"F644"=>0, #"Rare None Cap",
#"F84C"=>0, #"Rare None Cape",
#"F84B"=>0, #"Rare None Cloak",
#"F74E"=>0, #"Rare None Dress",
#"FA57"=>0, #"Rare None Gloves",
#"F643"=>0, #"Rare None Hat",
#"F645"=>0, #"Rare None Low Hat",
#"F74D"=>0, #"Rare None Robe",
#"FC5B"=>0, #"Rare None Shoes",
#"3542"=>0, #"Rare Silver Amulet",
#"3441"=>0, #"Rare Silver Ring",
#"1934"=>0, #"Uncommon Bronze Bracers",
#"1832"=>0, #"Uncommon Bronze Cuirass",
#"1629"=>0, #"Uncommon Bronze Full Helm",
#"1626"=>0, #"Uncommon Bronze Helm",
#"1C3E"=>0, #"Uncommon Bronze Plate Boots",
#"1936"=>0, #"Uncommon Bronze Plate Bracers",
#"1833"=>0, #"Uncommon Bronze Plate Cuirass",
#"162A"=>0, #"Uncommon Bronze Plate Helm",
#"1A3A"=>0, #"Uncommon Bronze Scale Gauntlets",
#"1731"=>0, #"Uncommon Bronze Scale Mail",
#"BC3C"=>0, #"Uncommon Hard Leather Boots",
#"B935"=>0, #"Uncommon Hard Leather Bracers",
#"BA38"=>0, #"Uncommon Hard Leather Gauntlets",
#"B627"=>0, #"Uncommon Hard Leather Helm",
#"B72F"=>0, #"Uncommon Hard Leather Mail",
#"AC3C"=>0, #"Uncommon Leather Boots",
#"A935"=>0, #"Uncommon Leather Bracers",
#"AA38"=>0, #"Uncommon Leather Gauntlets",
#"A627"=>0, #"Uncommon Leather Helm",
#"A72F"=>0, #"Uncommon Leather Mail",
#"F624"=>0, #"Uncommon None Cap",
#"F82C"=>0, #"Uncommon None Cape",
#"F72E"=>0, #"Uncommon None Dress",
#"FA37"=>0, #"Uncommon None Gloves",
#"F623"=>0, #"Uncommon None Hat",
#"F625"=>0, #"Uncommon None Low Hat",
#"FC3B"=>0, #"Uncommon None Shoes",
#"3522"=>0, #"Uncommon Silver Amulet",
#"3421"=>0, #"Uncommon Silver Ring",
#"2934"=>0, #"Uncommon Steel Bracers",
#"2C3D"=>0, #"Uncommon Steel Chain Boots",
#"2A39"=>0, #"Uncommon Steel Chain Gauntlets",
#"2628"=>0, #"Uncommon Steel Chain Helm",
#"2730"=>0, #"Uncommon Steel Chain Mail",
#"2832"=>0, #"Uncommon Steel Cuirass",
#"2629"=>0, #"Uncommon Steel Full Helm",
#"2626"=>0, #"Uncommon Steel Helm",
#"2C3E"=>0, #"Uncommon Steel Plate Boots",
#"2936"=>0, #"Uncommon Steel Plate Bracers",
#"2833"=>0, #"Uncommon Steel Plate Cuirass",
#"262A"=>0, #"Uncommon Steel Plate Helm",
#"2A3A"=>0, #"Uncommon Steel Scale Gauntlets",
#"2731"=>0, #"Uncommon Steel Scale Mail",
#"6562"=>0, #"Very Rare Adamantium Amulet",
#"6872"=>0, #"Very Rare Adamantium Cuirass",
#"6669"=>0, #"Very Rare Adamantium Full Helm",
#"6666"=>0, #"Very Rare Adamantium Helm",
#"6C7E"=>0, #"Very Rare Adamantium Plate Boots",
#"6976"=>0, #"Very Rare Adamantium Plate Bracers",
#"6873"=>0, #"Very Rare Adamantium Plate Cuirass",
#"666A"=>0, #"Very Rare Adamantium Plate Helm",
#"6461"=>0, #"Very Rare Adamantium Ring",
#"6A7A"=>0, #"Very Rare Adamantium Scale Gauntlets",
#"6771"=>0, #"Very Rare Adamantium Scale Mail",
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
#"F664"=>0, #"Very Rare None Cap",
#"F86C"=>0, #"Very Rare None Cape",
#"F76E"=>0, #"Very Rare None Dress",
#"FA77"=>0, #"Very Rare None Gloves",
#"F663"=>0, #"Very Rare None Hat",
#"F665"=>0, #"Very Rare None Low Hat",
#"FC7B"=>0, #"Very Rare None Shoes",
#"0E40"=>0, #"Potion Antipoison",
#"0E47"=>0, #"Potion Big Healing",
#"0E4A"=>0, #"Potion Big Mana",
#"0E4B"=>0, #"Potion Fighter Bonus",
#"0E45"=>0, #"Potion Health Regeneration",
#"0E4C"=>0, #"Potion Mage Bonus",
#"0E48"=>0, #"Potion Mana Regeneration",
#"0E46"=>0, #"Potion Medium Healing",
#"0E49"=>0, #"Potion Medium Mana",
"0E41"=>0, #"Potion Body",
"0E43"=>0, #"Potion Mind",
"0E42"=>0, #"Potion Reaction",
"0E44"=>0, #"Potion Spirit",
#"0E54"=>0, #"Quest Amulet",
#"0E4E"=>0, #"Quest Banner",
#"0E4F"=>0, #"Quest Crown1",
#"0E50"=>0, #"Quest Crown12",
#"0E51"=>0, #"Quest Crown13",
#"0E4D"=>0, #"Quest Documents",
#"0E56"=>0, #"Quest Head",
#"0E52"=>0, #"Quest Ingredient",
#"0E55"=>0, #"Quest Map",
#"0E58"=>0, #"Quest Meta1",
#"0E59"=>0, #"Quest Meta2",
#"0E5A"=>0, #"Quest Meta3",
#"0E5B"=>0, #"Quest Meta4",
#"0E60"=>0, #"Quest Ring",
#"0E5C"=>0, #"Quest RuneA",
#"0E5D"=>0, #"Quest RuneE",
#"0E5E"=>0, #"Quest RuneF",
#"0E5F"=>0, #"Quest RuneW",
#"0E57"=>0, #"Quest Stone",
#"0E53"=>0, #"Quest Treasure",
"0E0E"=>50, #"Scroll Acid Stream",
"0E19"=>50, #"Scroll Bless",
"0E0C"=>50, #"Scroll Blizzard",
"0E1B"=>50, #"Scroll Control Spirit",
"0E21"=>50, #"Scroll Curse",
"0E13"=>50, #"Scroll Darkness",
"0E15"=>50, #"Scroll Diamond Dust",
"0E1F"=>50, #"Scroll Drain Life",
"0E06"=>50, #"Scroll Fire Arrow",
"0E07"=>50, #"Scroll Fire Ball",
"0E08"=>50, #"Scroll Fire Wall",
"0E1A"=>50, #"Scroll Haste",
"0E1D"=>50, #"Scroll Heal",
"0E0A"=>50, #"Scroll Ice Missile",
"0E11"=>50, #"Scroll Invisibility",
"0E14"=>50, #"Scroll Light",
"0E0F"=>50, #"Scroll Lightning",
"0E0B"=>50, #"Scroll Poison Cloud",
"0E10"=>50, #"Scroll Prismatic Spray",
"0E12"=>50, #"Scroll Protection from Air",
"0E18"=>50, #"Scroll Protection from Earth",
"0E09"=>50, #"Scroll Protection from Fire",
"0E0D"=>50, #"Scroll Protection from Water",
"0E20"=>50, #"Scroll Shield",
"0E22"=>50, #"Scroll Slow",
"0E17"=>50, #"Scroll Stone Curse",
"0E1E"=>50, #"Scroll Summon",
"0E1C"=>50, #"Scroll Teleport",
"0E16"=>50, #"Scroll Wall of Earth",
"0E2B"=>10, #"SuperScroll Acid Stream",
"0E36"=>10, #"SuperScroll Bless",
"0E29"=>10, #"SuperScroll Blizzard",
"0E38"=>10, #"SuperScroll Control Spirit",
"0E3E"=>10, #"SuperScroll Curse",
"0E30"=>10, #"SuperScroll Darkness",
"0E32"=>10, #"SuperScroll Diamond Dust",
"0E3C"=>10, #"SuperScroll Drain Life",
"0E23"=>10, #"SuperScroll Fire Arrow",
"0E24"=>10, #"SuperScroll Fire Ball",
"0E25"=>10, #"SuperScroll Fire Wall",
"0E37"=>10, #"SuperScroll Haste",
"0E3A"=>10, #"SuperScroll Heal",
"0E27"=>10, #"SuperScroll Ice Missile",
"0E2E"=>10, #"SuperScroll Invisibility",
"0E31"=>10, #"SuperScroll Light",
"0E2C"=>10, #"SuperScroll Lightning",
"0E28"=>10, #"SuperScroll Poison Cloud",
"0E2D"=>10, #"SuperScroll Prismatic Spray",
"0E2F"=>10, #"SuperScroll Protection from Air",
"0E35"=>10, #"SuperScroll Protection from Earth",
"0E26"=>10, #"SuperScroll Protection from Fire",
"0E2A"=>10, #"SuperScroll Protection from Water",
"0E3D"=>10, #"SuperScroll Shield",
"0E3F"=>10, #"SuperScroll Slow",
"0E34"=>10, #"SuperScroll Stone Curse",
"0E3B"=>10, #"SuperScroll Summon",
"0E39"=>10, #"SuperScroll Teleport",
"0E33"=>10, #"SuperScroll Wall of Earth",
#"22A5"=>0, #"Bad Steel Large Shield",
#"1201"=>0, #"Common Bronze Buckler",
#"1202"=>0, #"Common Bronze Small Shield",
#"B206"=>0, #"Common Hard Leather Large Shield",
#"B203"=>0, #"Common Hard Leather Small Shield",
#"0201"=>0, #"Common Iron Buckler",
#"0202"=>0, #"Common Iron Small Shield",
#"A206"=>0, #"Common Leather Large Shield",
#"A203"=>0, #"Common Leather Small Shield",
#"8207"=>0, #"Common Wood Large Shield",
#"8204"=>0, #"Common Wood Small Shield",
#"6245"=>0, #"Rare Adamantium Large Shield",
#"6248"=>0, #"Rare Adamantium Tower Shield",
#"9247"=>0, #"Rare Magic Wood Large Shield",
#"9244"=>0, #"Rare Magic Wood Small Shield",
#"9249"=>0, #"Rare Magic Wood Tower Shield",
#"5245"=>0, #"Rare Mithrill Large Shield",
#"5242"=>0, #"Rare Mithrill Small Shield",
#"5248"=>0, #"Rare Mithrill Tower Shield",
#"1222"=>0, #"Uncommon Bronze Small Shield",
#"1228"=>0, #"Uncommon Bronze Tower Shield",
#"B226"=>0, #"Uncommon Hard Leather Large Shield",
#"B223"=>0, #"Uncommon Hard Leather Small Shield",
#"A226"=>0, #"Uncommon Leather Large Shield",
#"A223"=>0, #"Uncommon Leather Small Shield",
#"9227"=>0, #"Uncommon Magic Wood Large Shield",
#"9224"=>0, #"Uncommon Magic Wood Small Shield",
#"9229"=>0, #"Uncommon Magic Wood Tower Shield",
#"2225"=>0, #"Uncommon Steel Large Shield",
#"2222"=>0, #"Uncommon Steel Small Shield",
#"2228"=>0, #"Uncommon Steel Tower Shield",
#"8227"=>0, #"Uncommon Wood Large Shield",
#"8224"=>0, #"Uncommon Wood Small Shield",
#"8229"=>0, #"Uncommon Wood Tower Shield",
#"6265"=>0, #"Very Rare Adamantium Large Shield",
"C266"=>0, #"Very Rare Dragon Leather Large Shield",
"C263"=>0, #"Very Rare Dragon Leather Small Shield",
#"01B2"=>0, #"Bad Iron Axe",
#"21A3"=>0, #"Bad Steel Short Sword",
#"81AE"=>0, #"Bad Wood Shaman Staff",
#"1112"=>0, #"Common Bronze Axe",
#"1102"=>0, #"Common Bronze Dagger",
#"1111"=>0, #"Common Bronze Lance",
#"1104"=>0, #"Common Bronze Long Sword",
#"1109"=>0, #"Common Bronze Mace",
#"110A"=>0, #"Common Bronze Morning Star",
#"110B"=>0, #"Common Bronze Pick Hammer",
#"110F"=>0, #"Common Bronze Pike",
#"1103"=>0, #"Common Bronze Short Sword",
#"1108"=>0, #"Common Bronze Spiked Club",
#"1113"=>0, #"Common Bronze Two Handed Axe",
#"1106"=>0, #"Common Bronze Two Handed Sword",
#"110C"=>0, #"Common Bronze War Hammer",
#"0102"=>0, #"Common Iron Dagger",
#"0111"=>0, #"Common Iron Lance",
#"010A"=>0, #"Common Iron Morning Star",
#"010B"=>0, #"Common Iron Pick Hammer",
#"0103"=>0, #"Common Iron Short Sword",
#"0108"=>0, #"Common Iron Spiked Club",
#"0113"=>0, #"Common Iron Two Handed Axe",
#"0106"=>0, #"Common Iron Two Handed Sword",
#"010C"=>0, #"Common Iron War Hammer",
#"8107"=>0, #"Common Wood Club",
#"8116"=>0, #"Common Wood Crossbow",
#"8115"=>0, #"Common Wood Long Bow",
#"810E"=>0, #"Common Wood Shaman Staff",
#"8114"=>0, #"Common Wood Short Bow",
#"6182"=>0, #"Elven Adamantium Dagger",
#"6184"=>0, #"Elven Adamantium Long Sword",
#"6183"=>0, #"Elven Adamantium Short Sword",
#"6186"=>0, #"Elven Adamantium Two Handed Sword",
#"9195"=>0, #"Elven Magic Wood Long Bow",
#"9194"=>0, #"Elven Magic Wood Short Bow",
#"918D"=>0, #"Elven Magic Wood Staff",
"7182"=>0, #"Elven Meteoric Dagger",
"7184"=>0, #"Elven Meteoric Long Sword",
"7183"=>0, #"Elven Meteoric Short Sword",
#"7186"=>0, #"Elven Meteoric Two Handed Sword",
#"5182"=>0, #"Elven Mithrill Dagger",
#"5184"=>0, #"Elven Mithrill Long Sword",
#"5183"=>0, #"Elven Mithrill Short Sword",
#"5186"=>0, #"Elven Mithrill Two Handed Sword",
#"91D5"=>0, #"Good Magic Wood Long Bow",
#"91CD"=>0, #"Good Magic Wood Staff",
#"31C6"=>0, #"Good Silver Two Handed Sword",
#"21C4"=>0, #"Good Steel Long Sword",
#"81CD"=>0, #"Good Wood Staff",
#"6152"=>0, #"Rare Adamantium Axe",
#"6145"=>0, #"Rare Adamantium Bastard Sword",
#"6156"=>0, #"Rare Adamantium Crossbow",
#"6142"=>0, #"Rare Adamantium Dagger",
#"6150"=>0, #"Rare Adamantium Halberd",
#"6144"=>0, #"Rare Adamantium Long Sword",
#"6149"=>0, #"Rare Adamantium Mace",
#"614A"=>0, #"Rare Adamantium Morning Star",
#"614F"=>0, #"Rare Adamantium Pike",
#"6143"=>0, #"Rare Adamantium Short Sword",
#"6153"=>0, #"Rare Adamantium Two Handed Axe",
#"6146"=>0, #"Rare Adamantium Two Handed Sword",
#"614C"=>0, #"Rare Adamantium War Hammer",
#"9155"=>0, #"Rare Magic Wood Long Bow",
#"914E"=>0, #"Rare Magic Wood Shaman Staff",
#"9154"=>0, #"Rare Magic Wood Short Bow",
#"5152"=>0, #"Rare Mithrill Axe",
#"5145"=>0, #"Rare Mithrill Bastard Sword",
#"5156"=>0, #"Rare Mithrill Crossbow",
#"5142"=>0, #"Rare Mithrill Dagger",
#"5150"=>0, #"Rare Mithrill Halberd",
#"5144"=>0, #"Rare Mithrill Long Sword",
#"5149"=>0, #"Rare Mithrill Mace",
#"514A"=>0, #"Rare Mithrill Morning Star",
#"514F"=>0, #"Rare Mithrill Pike",
#"5143"=>0, #"Rare Mithrill Short Sword",
#"5153"=>0, #"Rare Mithrill Two Handed Axe",
#"5146"=>0, #"Rare Mithrill Two Handed Sword",
#"514C"=>0, #"Rare Mithrill War Hammer",
#"3142"=>0, #"Rare Silver Dagger",
#"1132"=>0, #"Uncommon Bronze Axe",
#"1122"=>0, #"Uncommon Bronze Dagger",
#"1124"=>0, #"Uncommon Bronze Long Sword",
#"1129"=>0, #"Uncommon Bronze Mace",
#"112A"=>0, #"Uncommon Bronze Morning Star",
#"112B"=>0, #"Uncommon Bronze Pick Hammer",
#"112F"=>0, #"Uncommon Bronze Pike",
#"1123"=>0, #"Uncommon Bronze Short Sword",
#"1128"=>0, #"Uncommon Bronze Spiked Club",
#"1133"=>0, #"Uncommon Bronze Two Handed Axe",
#"1126"=>0, #"Uncommon Bronze Two Handed Sword",
#"112C"=>0, #"Uncommon Bronze War Hammer",
#"9127"=>0, #"Uncommon Magic Wood Club",
#"9135"=>0, #"Uncommon Magic Wood Long Bow",
#"912E"=>0, #"Uncommon Magic Wood Shaman Staff",
#"9134"=>0, #"Uncommon Magic Wood Short Bow",
#"912D"=>0, #"Uncommon Magic Wood Staff",
#"2132"=>0, #"Uncommon Steel Axe",
#"2125"=>0, #"Uncommon Steel Bastard Sword",
#"2136"=>0, #"Uncommon Steel Crossbow",
#"2122"=>0, #"Uncommon Steel Dagger",
#"2130"=>0, #"Uncommon Steel Halberd",
#"2124"=>0, #"Uncommon Steel Long Sword",
#"2129"=>0, #"Uncommon Steel Mace",
#"212A"=>0, #"Uncommon Steel Morning Star",
#"212B"=>0, #"Uncommon Steel Pick Hammer",
#"212F"=>0, #"Uncommon Steel Pike",
#"2123"=>0, #"Uncommon Steel Short Sword",
#"2128"=>0, #"Uncommon Steel Spiked Club",
#"2133"=>0, #"Uncommon Steel Two Handed Axe",
#"2126"=>0, #"Uncommon Steel Two Handed Sword",
#"212C"=>0, #"Uncommon Steel War Hammer",
#"8127"=>0, #"Uncommon Wood Club",
#"8135"=>0, #"Uncommon Wood Long Bow",
#"812E"=>0, #"Uncommon Wood Shaman Staff",
#"8134"=>0, #"Uncommon Wood Short Bow",
#"6172"=>0, #"Very Rare Adamantium Axe",
#"6176"=>0, #"Very Rare Adamantium Crossbow",
#"6162"=>0, #"Very Rare Adamantium Dagger",
#"6170"=>0, #"Very Rare Adamantium Halberd",
#"6164"=>0, #"Very Rare Adamantium Long Sword",
#"616F"=>0, #"Very Rare Adamantium Pike",
#"6163"=>0, #"Very Rare Adamantium Short Sword",
#"6173"=>0, #"Very Rare Adamantium Two Handed Axe",
#"6166"=>0, #"Very Rare Adamantium Two Handed Sword",
"E172"=>0, #"Very Rare Crystal Axe",
"E164"=>0, #"Very Rare Crystal Long Sword",
"E169"=>0, #"Very Rare Crystal Mace",
"E16A"=>0, #"Very Rare Crystal Morning Star",
"E16F"=>0, #"Very Rare Crystal Pike",
"E173"=>0, #"Very Rare Crystal Two Handed Axe",
"E166"=>0, #"Very Rare Crystal Two Handed Sword",
"E16C"=>0, #"Very Rare Crystal War Hammer",
#"4162"=>0, #"Very Rare Gold Dagger",
#"916E"=>0, #"Very Rare Magic Wood Shaman Staff",
#"916D"=>0, #"Very Rare Magic Wood Staff",
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


 @sps_disable = ( #сила заклов в вещах 0-100
  0,                  # incorrect spell
  100,            # огненная стрела 
  100,             # огненный шар
  0,             # огненная стена
  15,#"Protection from Fire",  # защита от магии огня
  100,#"Ice Missile",           # ледяная стрела
  0,#"Poison Cloud",          # ядовитый туман
  0,#"Blizzard",              # град
  15,#"Protection from Water", # защита от магии воды
  0,#"Acid Stream",           # кислотная атака
  100,#"Lightning",             # молния
  100,#"Prismatic Spray",       # радужная молния
  0,#"Invisibility",          # невидимость
  15,#"Protection from Air",   # защита от магии воздуха
  0,#"Darkness",              # тьма
  0,#"Light",                 # свечение
  100,#"Diamond Dust",          # каменная стрела
  0,#"Wall of Earth",         # каменная стена
  50,#"Stone Curse",           # каменное проклятие
  15,#"Protection from Earth", # защита от магии земли
  0,#"Bless",          # благословение
  0,#"Haste",          # ускорение
  0,#"Control Spirit", # перерождение
  0,#"Teleport",      # телепорт
  15,#"Heal",          # исцеление
  0,#"Summon",        # зов
  100,#"Drain Life",    # вампиризм
  0,#"Shield",        # магический щит
  0,#"Curse",         # проклятие
  0);#"Slow");           # замедление

 @book_disable = ( #ИЗУЧЕННЫЕ ЗАКЛЫ  0 - запрет, 1 - можно 
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
 while (my ($key,$value)=each(%disable))
  {
    if(($IDh=~/^$key/)&&($count>$value)) { return 1;}
  }
$spell_type=-1;
$spell_skill=-1;
 while ( my ($key, $value) = each(%effects) )
 {  	 
	if(($key=~/02h/)&&($value>1))	{return 1}; # сила
	if(($key=="08h")&&($value>100)) {return 1}; # healthregeneration
	if(($key=="0Bh")&&($value>200)) {return 1}; # manaregeneration
	if(($key=="29h")) { # spellinside
		if($spell_skill==-1)
		{
			if($sps_disable[$value]==0){return 1;}
			else { $spell_type=$value; }
              		}
              		elsif($spell_skill>$sps_disable[$value]) {$spell_type=-1;$spell_skill=-1;return 1;}
                	else{$spell_type=-1;$spell_skill=-1;}
	 }
	if(($key=="32h")) # spellskill
	{
           		if($spell_type<0) { $spell_skill=$value; }
	     	elsif($spell_skill>$sps_disable[$spell_type]) {$spell_type=-1;$spell_skill=-1;return 1;}
                	else{$spell_type=-1;$spell_skill=-1;}
                 } 
	if(($key=="2Ah")&&($book_disable[$value]<0))  # book
	{
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
$test = 0;
if (!$fn)
{
    $test = 1;
    $fn = "test.lgn";
}
###
#open(F,">>log.txt");
###
#print F "$fn\n";
open(C, $fn) || die "Can't open character file: $!!\n";
binmode(C);

$id1 = shift;
$id2 = shift;
if (!$id1 && !$id2) 
{
	$id1 = 4155455435;
	$id2 = 24;
}

seek(C, 0x104, 0);
my $c = readi(C);
$found = 0;

for ($i = 0; $i < $c; ++$i) 
{
	$i1 = readi(C);
	$i2 = readi(C);
	$i1h = sprintf("0x%.8X",$i1);
	$i2h = sprintf("0x%.8X",$i2);
	$id1h = sprintf("0x%.8X",$id1);
	$id2h = sprintf("0x%.8X",$id2);
###
#	print("$id1h, $id2h -- $i1h, $i2h   $i,$c\n");
	if (($i1 == $id1) && ($i2 == $id2)) {
		$found = 1;
		last;
	}
}

if (!$found) { die "Character ($id1, $id2) not found!\n"; }

seek(C, 0x188, 0);

$offs = 0x234;

for ($j = 0; $j < $i; ++$j) 
{
	$offs += readi(C);
}
$sz = readi(C);

seek(C, $offs, 0);

$enable=0;

$sig=readi(C);

while($sigsec=readi(C) & 0xFFFFFFFF) 
{
    $sizsec=readi(C) & 0xFFFFFFFF;
    $keysec=readi(C) & 0xFFFFFFFF;
    $crcsec=readi(C) & 0xFFFFFFFF;
    $sigh = sprintf("%.8Xh", $sigsec);
###
#    print("section found: $sigh\n size: $sizsec bytes\n");
    if ($sigsec == 0xAAAAAAAA) {
###
#	print(F "  main:\n");
        read(C, $body, $sizsec);
        @arr = unpack("c*", $body);
        decrypt();
	$id1 = readil(0);
	$id2 = readil(4);          
###
#	print(F "   ID=$id1/$id2\n");
	$idxxx = 0;
	$s = "";
	while ($c = readbl(12+$idxxx++)) 
	{
		$s = $s . chr($c & 0xFF);
	}
###
#	print(F "   nick: '$s'\n");
	if ($crcsec!=testchecksum()) { die "bad chksum!\n"; }
    }
    elsif ($sigsec == 0x55555555) {
###
#	print(F "  additional:\n");
        read(C, $body, $sizsec);
        @arr = unpack("c*", $body);
        decrypt();
	if ($crcsec!=testchecksum()) { die "bad chksum!\n"; }
    }
    elsif ($sigsec == 0x40A40A40) {
###
#	print(F "  memory:\n");
        read(C, $body, $sizsec);
        @arr = unpack("c*", $body);
        decrypt();
	if ($crcsec!=testchecksum()) { die "bad chksum!\n"; }
    }
    elsif ($sigsec == 0x3A5A3A5A) {
###
#	print(F "  in-the-pack:\n");
        read(C, $body, $sizsec);
        @arr = unpack("c*", $body);
        decrypt();
	if ($crcsec!=testchecksum()) { die "bad chksum!\n"; }
	readpack();
    }
    elsif ($sigsec == 0xDE0DE0DE) {
###
#	print(F "  on-the-body:\n");
        read(C, $body, $sizsec);
        @arr = unpack("c*", $body);
	decrypt();
	if ($crcsec!=testchecksum()) { die "bad chksum!\n"; }
	readpack();
    }
    elsif ($sigsec == 0x41392521) 
    {
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

#	if ($exp_fire_blade>5800) {$enable=$enable+1; }
#	if ($exp_water_axe>5800) {$enable=$enable+1; }
#	if ($exp_air_bludgeon>5800) {$enable=$enable+1; }
#	if ($exp_earth_pike>5800) {$enable=$enable+1; }
#	if ($exp_astral_shooting>1600) {$enable=$enable+1; }
	if ($money>100000000) {$enable=$enable+1; }
	if ($body_level>44) {$enable=$enable+1; }
	if ($reaction_level>44) {$enable=$enable+1; }
	if ($mind_level>44) {$enable=$enable+1; }
	if ($spirit_level>44) {$enable=$enable+1; }
 	if ($spirit_level+$body_level+$reaction_level+$mind_level>136) {$enable=$enable+1; }
	foreach $i (@sps) 
	{
		if ($spells & (1 << $idxx)) 
		{
		    if(@book_disable[$idxx]==0) {$enable=$enable+1; } 
		}
		$idxx++;
	}
    }
}
$log_dir = "C:\\Allods2\\plscript\\pvm_log\\";
$newname = sprintf("%s%d-%d.a2c", $log_dir, time(), $enable);

open CC, ">$newname";
binmode CC;
seek(C, $offs, 0);
read(C, $data, $sz);
print CC $data;
close CC;

if($enable>0){$enable=1;}
#close F;
if ($test)
{
   print "$newname\n";
   print "$enable\n";
   sleep 1;
   exit;
}
exit $enable;

