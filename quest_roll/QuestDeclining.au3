;;;;;====version 0.003========;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; 25/01/2020
;  Added support for English version of Rage of Mages. Thanks to player BNegetive for feedback!
;  Fixed script self-termination
;
;;;;;====version 0.002========;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; 22/01/2020
;  WARNING! Hot keys have been changed!
;  Press "/" when standing next to tavern and hovering mouse over it to start
;  The script will scroll through quests automatically
;  If you haven't made a selection it will decline all quests and start scrolling again
;
;  Press "\" once you're happy with the quest (the script will terminate itself)
;
;
; Added configuration file to adjust timeout settings
; If it's not present, then it's created automatically
;
; If you exit Allods2 the script will terminate itself
;
;==--==--==--==--
;
;
;
; 15/01/2020 This is an AutoIt script designed to pick up
; and then decline quests in Allods 2 (Rage of Mages) network client
; Requirements:
; screen resolution: 1024x768
; It's been tested using tangar's hat (http://tangar.info/allods2)
;
; License: free to be distributed, modified, do anything you want with it,
; but please provide it along with the source code.
;
; The script is provided as is. Use it at your own risk!
; The author doesn't take any responsibility for the result of its work.
; You may lose your data and anything else can happen.
; I never forced you to use this script :)
;
;
;  How to use
;  0) download and install AutoIt (https://www.autoitscript.com/site/autoit/downloads/)
;  1) open Allods2 client
;  2) open QuestDeclining.au3 in AutoIt Script Editor. Hit "F5"  ("Go" to start the script). It will bring up Allods window automatically.
;  3) hover your mouse cursor over the tavern and hit "/" button
;  5) wait while the script scrolls and declines the quests
;  6) found a quest you're happy with? hit "\" immediately to terminate the script :)
;  Problems? Press "\" and the script terminates.
;  log file "a2q.log" is written into the directory where you run the script
;  configuration file "a2q.log" is written into the directory where you run the script. Feel free to edit the values

; definitions
#include <AutoItConstants.au3>
#include <Misc.au3>

   _Singleton("test", 0)

  ;hot key to run record down coordinates and color
  Const $WriteCoordsKey="{x}"

  ;Hot key to terminate the scipt
  Const $ScriptExitKey="{\}"

  ;Hot key to count quests
  ;Const $CountQuestsKey="{v}"
  Const $GetTavernKey="{/}"

  ; log file name
  ;Global $sFileName=@TempDir & "\a2q.log"
  ;Global Const $sFileName= "C:\Temp\a2q.log"
  Global Const $sFileName= @ScriptDir & "\a2q.log"

  Global Const $sConfFile= @ScriptDir & "\a2q.cfg"

  ; Tavern coordiantes. These are populated when being outside the tavern
  ; and hovering mouse over it.
  Global $tavernX=-1, $tavernY=-1
; RusScript=true
  Global const $bDebug=true
  ; amount of quests to decline
  ;Global const $amount=4
; scrWidth=1024
; scrHeight=768



	; default values
	Global $MouseSpeed=5
	Global $TavEntryDelay=2000
	Global $TavExitDelay=2000
	Global $TavQuestsDelay=2000



	; quest icons coordinates
	Global const $q6x=634, $q6y=592
	Global const $q5x=586, $q5y=592
	Global const $q4x=537, $q4y=592
	Global const $q3x=489, $q3y=592
	Global const $q2x=442, $q2y=592
	Global const $q1x=391, $q1y=592


	;Big monster avatar portrait 3 pixels coordinates to check
	Global const $nPortr1x=274, $nPortr1y=496
	Global const $nPortr2x=280, $nPortr2y=528
	Global const $nPortr3x=270, $nPortr3y=464

	; Pixels to check if we're inside the tavern
	Global const $nTopBtn1x=708, $nTopBtn1y=202
	Global const $nTopBtn2x=748, $nTopBtn2y=219
	Global const $nTopBtn3x=772, $nTopBtn3y=198
	Global $nColor1=0,$nColor2=0,$nColor3=0

HotKeySet($ScriptExitKey, "Terminate")
;HotKeySet($WriteCoordsKey, "WriteCoords")
;HotKeySet($CountQuestsKey, "CountQuests")
HotKeySet($GetTavernKey,"GetIntoTavern")
;HotKeySet("{f}","DenyQuests")

;HotKeySet("{6}","DenyQuests6")
;HotKeySet("{5}","DenyQuests5")
;HotKeySet("{4}","DenyQuests4")
;HotKeySet("{3}","DenyQuests3")
;HotKeySet("{2}","DenyQuests2")
;HotKeySet("{1}","DenyQuests1")

;-=-=-=-========---------------
;  end of definitions section
;-=-=-=------=-=-===========-=-=

If $bDebug Then
	WriteToLog("Starting the script")
EndIf


ConfigRead($sConfFile)

; if Allods2 client is not running then kill the script
Allods2Running()

If WinActivate("Аллоды II: Повелитель Душ", "")<>0 Then
	$hAllods2=WinGetHandle("Аллоды II: Повелитель Душ", "")
Else
	WinActivate("Rage of Mages 2: Necromancer", "")
	$hAllods2=WinGetHandle("Rage of Mages 2: Necromancer", "")
EndIf

while 1
	sleep(100)
	; if Allods2 client is not running then kill the script
	Allods2Running()
Wend


;;----Terminate()------
;    Writing to log and exiting script
;
;    hot key "e"
;
;-----------------------
Func Terminate()
    If $bDebug Then
		WriteToLog("Exiting script")
	EndIf
	Exit 0
EndFunc

;;------CountQuests()-------
;  returns the amount of quests in the tavern (6 is maximum!!!)
;
;
;
;  no hot key set
;
;--------------
Func CountQuests()
	; if Allods2 client is not running then kill the script
	Allods2Running()

	; Check how empty avatar looks like
	$EmptyColor= Hex(PixelGetColor($nPortr1x, $nPortr1y, $hAllods2),6)
	$EmptyColor2=Hex(PixelGetColor($nPortr2x, $nPortr2y, $hAllods2),6)
	$EmptyColor3=Hex(PixelGetColor($nPortr3x, $nPortr3y, $hAllods2),6)

	;move mouse cursor to quest 6
	$Check=fCheckQ($q6x,$q6y,$EmptyColor,$EmptyColor2,$EmptyColor3,6)
	If $Check<>0 Then
		Return $Check
	EndIf
	;=======

	;move mouse cursor to quest 5
	$Check=fCheckQ($q5x, $q5y,$EmptyColor,$EmptyColor2,$EmptyColor3,5)
	If $Check<>0 Then
		Return $Check
	EndIf

	;=======

	;move mouse cursor to quest 4
	$Check=fCheckQ($q4x, $q4y,$EmptyColor,$EmptyColor2,$EmptyColor3,4)
	If $Check<>0 Then
		Return $Check
	EndIf
	;=======

	;move mouse cursor to quest 3
	$Check=fCheckQ($q3x, $q3y,$EmptyColor,$EmptyColor2,$EmptyColor3,3)
	If $Check<>0 Then
		Return $Check
	EndIf
	;=======

	;move mouse cursor to quest 2
	$Check=fCheckQ($q2x, $q2y,$EmptyColor,$EmptyColor2,$EmptyColor3,2)
	If $Check<>0 Then
		Return $Check
	EndIf
	;=======

	;move mouse cursor to quest 1
	$Check=fCheckQ($q1x, $q1y,$EmptyColor,$EmptyColor2,$EmptyColor3,1)
	If $Check<>0 Then
		Return $Check
	EndIf
	;=======


	If $bDebug Then
		WriteToLog("end of Counting quests function")
	EndIf

	Return 0
EndFunc


;;----WriteCoords()----
;   writes coordinates and color to the log file
;   hot key
;
;-----------------
Func WriteCoords()
	; if Allods2 client is not running then kill the script
	Allods2Running()

	Local $aPos=MouseGetPos()
	$x=$aPos[0]
	$y=$aPos[1]
	WriteToLog("Mouse x, y: " & $x & ", " & $y & " colour: " & Hex(PixelGetColor($x,$y,$hAllods2),6)   )
EndFunc


;-----WriteToLog($sText)------
;
;  appends text to the log file ($sFileName) with current date and time in the front
;  when fails an error Message Box pops up
;=============================
Func WriteToLog($sText)
	$hFile=FileOpen($sFileName,1)
	If $hFile<>-1 Then
		$sTime=@MDAY & "/" & @MON & "/" & @YEAR & " " & @HOUR & ":" & @MIN & ":" & @SEC & " "
		$res=FileWriteLine($hFile,$sTime & $sText)

		If $res<>0 Then
			FileClose($hFile)
			;MsgBox(0,"Success","Success")
		Else
			MsgBox(16,"Error writing","Error writing to log file")
		EndIf

	EndIf
EndFunc

;;-------GetTavernCoords()------------
;
;    Get coordinates of the tavern next to the player
;    The player has to be outside hovering the mouse cursor over  the tavern.
;    Warning!!!  Map shouldn't be scrolled! Mouse cursor shouldn't be moved!
;;-------------------
Func GetTavernCoords()
	Local $aPos=MouseGetPos()
	$tavernX=$aPos[0]
	$tavernY=$aPos[1]
EndFunc

;;-------EnterTavern()------------
;   Moves mouse cursor over the tavern using coordinates obtained from GetTavernCoords() function
;	Clicks on the tavern
;
;	Warning!!!  Map shouldn't be scrolled! Mouse cursor shouldn't be moved!
;;---------------------------
Func EnterTavern()
	; if Allods2 client is not running then kill the script
	Allods2Running()

	MouseMove($tavernX,$tavernY,$MouseSpeed)
	MouseClick($MOUSE_CLICK_LEFT)

	Sleep($TavEntryDelay)
	If $nColor1=0 Then
		; colours of the pixels of Top button in the tavern. obtained once
		$nColor1=Hex(PixelGetColor($nTopBtn1x, $nTopBtn1y, $hAllods2),6)
		$nColor2=Hex(PixelGetColor($nTopBtn2x, $nTopBtn2y, $hAllods2),6)
		$nColor3=Hex(PixelGetColor($nTopBtn3x, $nTopBtn3y, $hAllods2),6)
	EndIf
EndFunc

;;------GetIntoTavern()--------------
;   Combination of  GetTavernCoords() and EnterTavern()
;
;   hot key "/"
;
;   Warning!!!  Map shouldn't be scrolled! Mouse cursor shouldn't be moved!
;;-----------------------------------
Func GetIntoTavern()
	; if Allods2 client is not running then kill the script
	Allods2Running()

	If $nColor1<>0 Then
		; we've entered the tavern before

		; compare colours of the pixels
		If 	$nColor1 <> Hex(PixelGetColor($nTopBtn1x, $nTopBtn1y, $hAllods2),6) And _
			$nColor2 <> Hex(PixelGetColor($nTopBtn2x, $nTopBtn2y, $hAllods2),6) And _
			$nColor3 <> Hex(PixelGetColor($nTopBtn3x, $nTopBtn3y, $hAllods2),6) Then
				; we're outside and good to get in
				GetTavernCoords()
				EnterTavern()

				; eternal cycle. We won't get here anyways
				While True
					$amount=CountQuests()
					ScrollQuests($amount)
				Wend
		Else
				; we're inside the tavern. exit function
		EndIf
	Else
		; entering tavern for the first time

		GetTavernCoords()
		EnterTavern()

		; eternal cycle
		While True
			$amount=CountQuests()
			ScrollQuests($amount)
		Wend
	EndIf
EndFunc
;;----fCheckQ($x,$y,$ECol1,$ECol2,$ECol3,$q)
; $x     - x coordinate of a quest icon
; $y     - y coordinate of a quest icon
; $ECol1 - empty avatar color of the 1st pixel
; $ECol2 - empty avatar color of the 2nd pixel
; $ECol3 - empty avatar color of the 3nd pixel
; $q     - quest icon number to check
;===================================
Func fCheckQ($x,$y,$ECol1,$ECol2,$ECol3,$q)
	; if Allods2 client is not running then kill the script
	Allods2Running()
	; select a quest icon
	MouseMove($x, $y, $MouseSpeed)
	MouseClick($MOUSE_CLICK_LEFT)

	; Getting colors of 3 pixels on the monster avatar
	$CurrentColor= Hex(PixelGetColor($nPortr1x, $nPortr1y, $hAllods2),6)
	$CurrentColor2=Hex(PixelGetColor($nPortr2x, $nPortr2y, $hAllods2),6)
	$CurrentColor3=Hex(PixelGetColor($nPortr3x, $nPortr3y, $hAllods2),6)

	;Compare pixel colors with an empty portrait
	If $ECol1=$CurrentColor and $ECol2=$CurrentColor2 and $ECol3=$CurrentColor3 Then
		;quest is empty
		return 0
	Else
		If $bDebug Then
			WriteToLog("" & $q & " quests found")
		EndIf

		Return $q
	EndIf
EndFunc


Func DenyQuests1()
	DenyQuests(1)
EndFunc

Func DenyQuests2()
	DenyQuests(2)
EndFunc

Func DenyQuests3()
	DenyQuests(3)
EndFunc

Func DenyQuests4()
	DenyQuests(4)
EndFunc

Func DenyQuests5()
	DenyQuests(5)
EndFunc

Func DenyQuests6()
	DenyQuests(6)
EndFunc
;------
; DenyQuests function
; one needs to be inside the tavern
;
; parameter - amount of quests to decline
;
; -----
Func DenyQuests($nQuests)
	; if Allods2 client is not running then kill the script
	Allods2Running()

	For $i=1 to $nQuests
		If $bDebug Then
			WriteToLog("iteration " & $i & " start")
		EndIf

		;Pick up left quest
		MouseMove($q1x, $q1y, $MouseSpeed)
		MouseClick($MOUSE_CLICK_LEFT)
		MouseClick($MOUSE_CLICK_LEFT)

		ExitTavern()

		EnterTavern()

		;Pick up left quest
		MouseMove($q1x, $q1y, $MouseSpeed)
		MouseClick($MOUSE_CLICK_LEFT)
		MouseClick($MOUSE_CLICK_LEFT)

		ExitTavern()

		EnterTavern()

		If $bDebug Then
			WriteToLog("iteration " & $i & " end")
		EndIf
	Next

EndFunc


;------
; ConfigRead() function
; reads config file inside the script folder
;
; or creates one with default settings
;
; parameter - path to the configuration file
;
; -----
Func ConfigRead($sFile)
	; if config file exists then read values into Global variables
	If FileExists($sFile) Then
		If $bDebug Then
			WriteToLog("Trying to read configuration file: " & $sFile)
		EndIf
		; opening read-only
		$res=FileOpen($sFile,0)
		If $res=-1 Then
			WriteToLog("Failed to open configuration file")
			Terminate()
		EndIf

		; Read the current script file into an array using the filepath.
		Local $aArray = FileReadToArray($sFile)
		Local $iLineCount = @extended
		If @error OR $iLineCount < 4 Then
			WriteToLog("Failed to open configuration file. Possible reason: amount of lines less, than 4")
			Terminate()
		Else
			For $i = 0 To $iLineCount - 1 ; Loop through the array. UBound($aArray) can also be used.
				;$aArray[$i]
				If StringInStr($aArray[$i],"MouseSpeed=")<>0 Then
					Local $aArr=StringSplit($aArray[$i],"=")
					If StringIsDigit($aArr[2]) Then
						$MouseSpeed=$aArr[2]

						If $bDebug Then
							WriteToLog("MouseSpeed parameter read successfully=" & $MouseSpeed)
						EndIf
					EndIf
				EndIf

				If StringInStr($aArray[$i],"TavEntryDelay=")<>0 Then
					Local $aArr=StringSplit($aArray[$i],"=")
					If StringIsDigit($aArr[2]) Then
						$TavEntryDelay=$aArr[2]

						If $bDebug Then
							WriteToLog("TavEntryDelay parameter read successfully=" & $TavEntryDelay)
						EndIf
					EndIf
				EndIf

				If StringInStr($aArray[$i],"TavExitDelay=")<>0 Then
					Local $aArr=StringSplit($aArray[$i],"=")
					If StringIsDigit($aArr[2]) Then
						$TavExitDelay=$aArr[2]

						If $bDebug Then
							WriteToLog("TavExitDelay parameter read successfully=" & $TavExitDelay)
						EndIf
					EndIf
				EndIf

				If StringInStr($aArray[$i],"TavQuestsDelay=")<>0 Then
					Local $aArr=StringSplit($aArray[$i],"=")
					If StringIsDigit($aArr[2]) Then
						$TavQuestsDelay=$aArr[2]

						If $bDebug Then
							WriteToLog("TavQuestsDelay parameter read successfully=" & $TavQuestsDelay)
						EndIf
					EndIf
				 EndIf
			Next    ; go reading next string

		EndIf


	Else
		; create new configuration file overwriting contents
		$res=FileOpen($sFile,2)
		If $res=-1 Then
			WriteToLog("Failed to create configuration file from scratch")
			Terminate()
		EndIf

		If $bDebug Then
			WriteToLog("Creating configuration file from scratch: " & $sFile)
		EndIf

		; Filling in default configuration settings
		FileWriteLine($res,"; mouse speed (slowest=5, fastest=1) default: 1")
		FileWriteLine($res,"MouseSpeed=1")
		FileWriteLine($res,"; tavern entering delay (milliseconds) default: 1800")
		FileWriteLine($res,"TavEntryDelay=1800")
		FileWriteLine($res,"; tavern exit delay (milliseconds) default: 1800")
		FileWriteLine($res,"TavExitDelay=1800")
		FileWriteLine($res,"; Delay between displaying available tavern quests (milliseconds) default: 2000")
		FileWriteLine($res,"TavQuestsDelay=2000")

		FileClose($res)

		If $bDebug Then
			WriteToLog("Configuration file has been created: " & $sFile)
		EndIf
	EndIf
EndFunc

;------
; ScrollQuests() function
; Clicks on available quests one by one allowing time for player to choose
;
;click on the very right one (=amount of quests)
;delay
;click on the one to the left of (=amount of quests-1)
;delay
;is that the very left quest? ( ?=1 )
;yes? then 1) decline "amount of quests" ; 2) get inside 3) start scrolling again
;
;
; parameter - $nAmount  (amount of quests to scroll through)
;
; -----
Func ScrollQuests($nAmount)
	; if Allods2 client is not running then kill the script
	Allods2Running()

	Switch $nAmount
		Case 1
			Sleep($TavQuestsDelay)
			DenyQuests(1)
		Case 2
			Sleep($TavQuestsDelay)
			MouseMove($q1x, $q1y, $MouseSpeed)
			MouseClick($MOUSE_CLICK_LEFT)
			Sleep($TavQuestsDelay)
			DenyQuests(2)
		Case 3
			Sleep($TavQuestsDelay)
			MouseMove($q2x, $q2y, $MouseSpeed)
			MouseClick($MOUSE_CLICK_LEFT)

			Sleep($TavQuestsDelay)
			MouseMove($q1x, $q1y, $MouseSpeed)
			MouseClick($MOUSE_CLICK_LEFT)

			Sleep($TavQuestsDelay)
			DenyQuests(3)
		Case 4
			Sleep($TavQuestsDelay)
			MouseMove($q3x, $q3y, $MouseSpeed)
			MouseClick($MOUSE_CLICK_LEFT)
			Sleep($TavQuestsDelay)
			MouseMove($q2x, $q2y, $MouseSpeed)
			MouseClick($MOUSE_CLICK_LEFT)
			Sleep($TavQuestsDelay)
			MouseMove($q1x, $q1y, $MouseSpeed)
			MouseClick($MOUSE_CLICK_LEFT)
			Sleep($TavQuestsDelay)
			DenyQuests(4)
		Case 5
			Sleep($TavQuestsDelay)
			MouseMove($q4x, $q4y, $MouseSpeed)
			MouseClick($MOUSE_CLICK_LEFT)
			Sleep($TavQuestsDelay)
			MouseMove($q3x, $q3y, $MouseSpeed)
			MouseClick($MOUSE_CLICK_LEFT)
			Sleep($TavQuestsDelay)
			MouseMove($q2x, $q2y, $MouseSpeed)
			MouseClick($MOUSE_CLICK_LEFT)
			Sleep($TavQuestsDelay)
			MouseMove($q1x, $q1y, $MouseSpeed)
			MouseClick($MOUSE_CLICK_LEFT)
			Sleep($TavQuestsDelay)
			DenyQuests(4)
		Case 6
			Sleep($TavQuestsDelay)
			MouseMove($q5x, $q5y, $MouseSpeed)
			MouseClick($MOUSE_CLICK_LEFT)
			Sleep($TavQuestsDelay)
			MouseMove($q4x, $q4y, $MouseSpeed)
			MouseClick($MOUSE_CLICK_LEFT)
			Sleep($TavQuestsDelay)
			MouseMove($q3x, $q3y, $MouseSpeed)
			MouseClick($MOUSE_CLICK_LEFT)
			Sleep($TavQuestsDelay)
			MouseMove($q2x, $q2y, $MouseSpeed)
			MouseClick($MOUSE_CLICK_LEFT)
			Sleep($TavQuestsDelay)
			MouseMove($q1x, $q1y, $MouseSpeed)
			MouseClick($MOUSE_CLICK_LEFT)
			Sleep($TavQuestsDelay)
			DenyQuests(6)
	EndSwitch
EndFunc

Func ExitTavern()
		;Exit tavern
		Send("{ESC}")
		; delay when exiting a tavern
		Sleep($TavExitDelay)
EndFunc


;------
; Allods2Running() function
;
; Checks if Allods2 client is running
; if not - terminates the script
;
; parameter
;
; -----
Func Allods2Running()
	If WinExists("Аллоды II: Повелитель Душ", "")=1 OR WinExists("Rage of Mages 2: Necromancer", "")=1 Then
		; Allods2 client is running. Let the script live.
	Else
		; Allods2 client window can't be found. Terminate the script.
		If $bDebug Then
			WriteToLog("Exiting script, because Allods2 window does not exist")
		EndIf
		Terminate()
	EndIf
EndFunc