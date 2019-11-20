<?php
	class MemStream
	{
		var $file;
		var $fileptr;
		var $size;
		public $bad;
		public $error;
		public $loaded;
	
		public function ReadDword()
		{
			if(!$this->loaded) return false;
			$pos = $this->fileptr;
			if($pos >= $this->size) return false;
			$string = $this->file[$pos].$this->file[$pos+1].$this->file[$pos+2].$this->file[$pos+3];
			$retv = unpack("V", $string);
			$this->fileptr += 4;
			return intval($retv[1]);
		}
		
		public function ReadWord()
		{
			if(!$this->loaded) return false;
			$pos = $this->fileptr;
			if($pos >= $this->size) return false;
			$string = $this->file[$pos].$this->file[$pos+1];
			$retv = unpack("v", $string);
			$this->fileptr += 2;
			return intval($retv[1]);
		}
		
		public function ReadByte()
		{
			if(!$this->loaded) return false;
			$pos = $this->fileptr;
			if($pos >= $this->size) return false;
			$retv = 0x00; // но наху€? хз
			$retv = ord($this->file[$pos]);
			$this->fileptr++;
			return intval($retv);
		}
		
		public function ReadString($maxcount)
		{
			if(!$this->loaded) return false;
			$pos = $this->fileptr;
			if($pos >= $this->size) return false;
			$string = "";
			for($i = 0; $i < $maxcount; $i++)
			{
				if((ord($this->file[$pos]) != 0) && ($pos < $this->size))
				{
					$string .= $this->file[$pos];
					$pos++;
				}
				else break;
			}
			$this->fileptr = $pos;
			return $string;
		}
		
		public function WriteByte($byte)
		{
			if(!$this->loaded) return false;
			$pos = $this->fileptr;
			if($pos >= $this->size) return false;
			$this->file[$pos] = chr($byte);
			$this->fileptr++;
			return $byte;
		}
		
		public function Seek($pos = 0, $type = 0)
		{
			if(!$this->loaded) return false;
			switch($type)
			{
				case 0:
					if($pos != 0) $this->fileptr = $pos;
					break;
				case 1:
					$this->fileptr += $pos;
					break;
				case 2:
					$this->fileptr -= $pos;
					break;
				case 3:
					$this->fileptr = $this->size-$pos;
					break;
			}
			return $this->fileptr;
		}
		
		public function Size()
		{
			if(!$this->loaded) return false;
			return $this->size;
		}
		
		function Error($string)
		{
			$this->bad = true;
			$this->error = $string;
		}
		
		function __construct()
		{
			$this->bad = false;
			$this->loaded = false;
		}
		
		public function FromFile($filename)
		{
			if(file_exists($filename))
			{
				$this->file = file_get_contents($filename);
				$this->size = filesize($filename);
				if($this->file === false || $this->size === false)
				{
					$this->Error("FILE_LOAD_ERROR");
					return;
				}
				$this->fileptr = 0;
			}
			else
			{
				$this->Error("FILE_NOT_FOUND");
			}
			$this->loaded = true;
		}
	}

	class A2C
	{
		public $bad;
		public $error;
		
		public $Id1;
		public $Id2;
		public $HatId;
		public $Nick;
		public $Clan;
		public $Sex;
		public $Picture;
		public $Color;
		public $Flags;
		public $MainSkill;
		
		// additional
		public $MonstersKills;
		public $PlayersKills;
		public $Frags;
		public $Deaths;
		public $Money;
		public $Body;
		public $Reaction;
		public $Mind;
		public $Spirit;
		public $Spells;
		public $ActiveSpell;
		public $ExpFireBlade;
		public $ExpWaterAxe;
		public $ExpAirBludgeon;
		public $ExpEarthPike;
		public $ExpAstralShooting;
		
		// items on-body
		public $Dress;
		
		// items in bag
		public $Bag;
		
		var $file;
		
		function ParseItems($pos, $size)
		{
			$this->file->Seek($pos+9, 0);

			$items = array();
			while($this->file->Seek() < $pos+$size)
			{
				$item_id = 0;
				$item_type = 0;
				$item_effects = array();
				$item_magic = false;
				$item_effectcnt = 0;
				$item_price = 0;
				$item_count = 0;
				/// begin code
				$item_id = $this->file->ReadWord();
				$item_type = $this->file->ReadByte();
				if(($item_type & 0x80) == 0x80) // несколько простых вещей
				{
					$item_count = $item_type-0x80;
					$item_magic = false;
					$item_price = 0;
				}
				else if(($item_type & 0x20) == 0x20) // магическа€ вещь (всегда 1)
				{
					$item_price = $this->file->ReadDword();
					$item_count = 1;
					$item_magic = true;
					$item_effectcnt = $item_type & 0x0F;
					for($i = 0; $i < $item_effectcnt; $i++)
					{
						$item_effects[$i]['id1'] = $this->file->ReadByte();
						$item_effects[$i]['value1'] = $this->file->ReadByte();
						if($item_effects[$i]['id1'] == 0x29 || (($item_effects[$i]['id1'] >= 0x2C) && ($item_effects[$i]['id1'] <= 0x30)))
						{
							$item_effects[$i]['id2'] = $this->file->ReadByte();
							$item_effects[$i]['value2'] = $this->file->ReadByte();
							$item_effectcnt--;
						}
					}
				}
				else if($item_type == 0x00) // много простых вещей
				{
					$item_count = $this->file->ReadWord();
					$item_magic = false;
					$item_price = 0;
				}
				else
				{
					$this->Error("Wrong item structure (strange item type ".$item_type.")!");
					return false;
				}
				
				$items[] = array('price' => $item_price,
								'count' => $item_count,
								'magic' => $item_magic,
								'effects' => $item_effects,
								'id' => $item_id);
			}
			return $items;
		}
		
		function FindSection($id)
		{
			$pos = 4;
			if($id == 0xAAAAAAAA) return $pos; // главна€ секци€ не ищетс€, неебу почему
			while($pos <= $this->file->Size())
			{
				$this->file->Seek($pos);
				$ssig = $this->file->ReadDword();
				$ssiz = $this->file->ReadDword();
				if($ssig == $id)
				{
					return $pos;
				}
				else
				{
					$pos += 0x10+$ssiz;
				}
			}
			return false;
		}
		
		function DecryptSection($position, $size, $key) // tested 100%, не трогать бл€ть
		{
			$k = $key | ($key << 0x10);
			for($i = 0; $i < $size; $i++)
			{
				$this->file->Seek($position+$i);
				$byte = $this->file->ReadByte();
				$byte = (($k >> 0x10) ^ $byte) & 0xFF;
				$k = ($k << 1);
				if(($i & 0x0F) == 0x0F) $k |= $key;
				$this->file->Seek($position+$i);
				$this->file->WriteByte($byte);
			}
		}
		
		function Error($string)
		{
			$this->bad = true;
			$this->error = $string;
		}
		
		function __construct($filename)
		{
			$this->bad = false;
			$this->file = new MemStream;
			$this->file->FromFile($filename);
			if($this->file->bad)
			{
				$this->Error("FILE ERR: ".$this->file->error);
				return;
			}
			$this->file->Seek(0);
			$sig = $this->file->ReadDword();
			$ssig1 = $this->file->ReadDword();
			
			if($sig == 0x04507989 && $ssig1 == intval(0xAAAAAAAA)) // ебаный пшп с его signed int
																   // не может в числа больше 0x7FFFFFFF
																   // без этого костыл€
			{
				$pos = $this->FindSection(intval(0xAAAAAAAA));
				if($pos === false)
				{
					$this->Error("Section ID=AAAAAAAAh (main) not found.");
					return;
				}
				$this->file->Seek($pos+4);
				$ssiz = $this->file->ReadDword();
				$this->file->Seek($pos+10);
				$skey = $this->file->ReadWord();
				$this->DecryptSection($pos+0x10, $ssiz, $skey);
				$this->file->Seek($pos+0x10);
				$this->Id1 = $this->file->ReadDword();
				$this->Id2 = $this->file->ReadDword();
				$this->HatId = $this->file->ReadDword();
				$this->Nick = $this->file->ReadString(32);
				$clanpos = strpos($this->Nick, "|");
				if(false !== $clanpos)
				{
					$this->file->Seek($pos+0x1C);
					$this->Nick = $this->file->ReadString($clanpos);
					$this->file->Seek(1, 1); // skip the '|'
					$this->Clan = $this->file->ReadString(32);
				}
				$this->file->Seek($pos+0x3C);
				$this->Sex = $this->file->ReadByte();
				$this->Picture = $this->file->ReadByte();
				$this->MainSkill = $this->file->ReadByte();
				$this->Flags = $this->file->ReadByte();
				$this->Color = $this->file->ReadByte();
				// end reading basic chars
				
				$pos = $this->FindSection(intval(0x41392521));
				if($pos === false)
				{
					$this->Error("Section ID=41392521h (additional chars) not found.");
					return;
				}
				$this->file->Seek($pos+10);
				$skey = $this->file->ReadWord();
				$this->file->Seek($pos+0x10);
				if($skey & 0x0001) $this->file->Seek(1, 1); // +1 byte
				$this->MonstersKills = $this->file->ReadDword() ^ 0x01529251;
				if($skey & 0x0002) $this->file->Seek(1, 1);
				$this->PlayersKills = $this->file->ReadDword() + $this->MonstersKills * 5 + 0x13141516;
				if($skey & 0x0004) $this->file->Seek(1, 1);
				$this->Frags = $this->file->ReadDword() + $this->PlayersKills * 7 + 0x00ABCDEF;
				if($skey & 0x0008) $this->file->Seek(1, 1);
				$this->Deaths = $this->file->ReadDword() ^ 0x17FF12AA;
				if($skey & 0x0010) $this->file->Seek(1, 1);
				$this->Money = $this->file->ReadDword() + $this->MonstersKills * 3 - 0x21524542;
				if($skey & 0x0020) $this->file->Seek(1, 1);
				$this->Body = ($this->file->ReadByte() + $this->Money * 17 + $this->MonstersKills * 19) & 0xFF;
				if($skey & 0x0040) $this->file->Seek(1, 1);
				$this->Reaction = ($this->file->ReadByte() + $this->Body * 3) & 0xFF;
				if($skey & 0x0080) $this->file->Seek(1, 1);
				$this->Mind = ($this->file->ReadByte() + $this->Body + $this->Reaction * 5) & 0xFF;
				if($skey & 0x0100) $this->file->Seek(1, 1);
				$this->Spirit = ($this->file->ReadByte() + $this->Body * 7 + $this->Mind * 9) & 0xFF;
				if($skey & 0x4000) $this->file->Seek(1, 1);
				$this->Spells = $this->file->ReadDword() - 0x10121974;
				if($skey & 0x2000) $this->file->Seek(1, 1);
				$this->ActiveSpell = $this->file->ReadDword(); // OMG
				if($skey & 0x0200) $this->file->Seek(1, 1);
				$this->ExpFireBlade = $this->file->ReadDword() ^ 0xDADEDADE;
				if($skey & 0x0400) $this->file->Seek(1, 1);
				$this->ExpWaterAxe = $this->file->ReadDword() - $this->ExpFireBlade * 0x771;
				if($skey & 0x0800) $this->file->Seek(1, 1);
				$this->ExpAirBludgeon = $this->file->ReadDword() - $this->ExpWaterAxe * 0x771;
				if($skey & 0x1000) $this->file->Seek(1, 1);
				$this->ExpEarthPike = $this->file->ReadDword() - $this->ExpAirBludgeon * 0x771;
				if($skey & 0x2000) $this->file->Seek(1, 1);
				$this->ExpAstralShooting = $this->file->ReadDword() - $this->ExpEarthPike * 0x771;
				// end reading additional chars
				
				$pos = $this->FindSection(intval(0xDE0DE0DE));
				if($pos === false)
				{
					$this->Error("Section ID=DE0DE0DEh (on-body items) not found.");
					return;
				}
				$this->file->Seek($pos+4);
				$ssiz = $this->file->ReadDword();
				$this->file->Seek($pos+10);
				$skey = $this->file->ReadWord();
				$this->DecryptSection($pos+0x10, $ssiz, $skey);
				$this->Dress = $this->ParseItems($pos+0x10, $ssiz);
				// end reading on-body items

				$pos = $this->FindSection(intval(0x3A5A3A5A));
				if($pos !== false)
				{
					$this->file->Seek($pos+4);
					$ssiz = $this->file->ReadDword();
					$this->file->Seek($pos+10);
					$skey = $this->file->ReadWord();
					$this->DecryptSection($pos+0x10, $ssiz, $skey);
					$this->Bag = $this->ParseItems($pos+0x10, $ssiz);
				}
				// end reading pack
			}
			else
			{
				$this->Error("Not a character file.");
				return;
			}
		}
	}
?>