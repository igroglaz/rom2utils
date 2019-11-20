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

$fn = shift;
if (!$fn) {
    $fn = "test.a2c";
}
open(C, $fn) || die "Can't open character file: $!!\n";
binmode C;

$sig=readi(C);

while($sigsec=readi(C) & 0xFFFFFFFF) {
    $sizsec=readi(C) & 0xFFFFFFFF;
    $keysec=readi(C) & 0xFFFFFFFF;
    $crcsec=readi(C) & 0xFFFFFFFF;
    $sigh = sprintf("%.8Xh", $sigsec);
    print("section found: $sigh\n size: $sizsec bytes\n");
    if ($sigsec == 0xAAAAAAAA) {
	print("  main:\n");
        read(C, $body, $sizsec);
        @arr = unpack("c*", $body);
        decrypt();
	$id1 = readil(0);
	$id2 = readil(4);          
	print("   ID=$id1/$id2\n");
	$idxxx = 0;
	$s = "";
	while ($c = readbl(12+$idxxx++)) {
		$s = $s . chr($c & 0xFF);
	}
	print("   nick: '$s'\n");
	if ($crcsec!=testchecksum()) { die "bad chksum!\n"; }
    }
    elsif ($sigsec == 0x55555555) {
	print("  additional:\n");
        read(C, $body, $sizsec);
        @arr = unpack("c*", $body);
        decrypt();
	if ($crcsec!=testchecksum()) { die "bad chksum!\n"; }
    }
    elsif ($sigsec == 0x40A40A40) {
	print("  memory:\n");
        read(C, $body, $sizsec);
        @arr = unpack("c*", $body);
        decrypt();
	if ($crcsec!=testchecksum()) { die "bad chksum!\n"; }
    }
    elsif ($sigsec == 0x3A5A3A5A) {
	print("  in-the-pack:\n");
        read(C, $body, $sizsec);
        @arr = unpack("c*", $body);
        decrypt();
	if ($crcsec!=testchecksum()) { die "bad chksum!\n"; }
	readpack();
    }
    elsif ($sigsec == 0xDE0DE0DE) {
	print("  on-the-body:\n");
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
	print("  other settings:\n");
	print("   spell-list:\n");
	foreach $i (@sps) {
		if ($spells & (1 << $idxx)) {
			print("    $i\n");
		}
		$idxx++;
	}
	print("   fire/blade exp: $exp_fire_blade\n");
	print("   water/axe exp: $exp_water_axe\n");
	print("   air/bludgeon exp: $exp_air_bludgeon\n");
	print("   earth/pike exp: $exp_earth_pike\n");
	print("   astral/shooting exp: $exp_astral_shooting\n");
    }
    else {
	die "invalid section's signature!\n";
    }
}

