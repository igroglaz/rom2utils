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
;  3) stand next to a tavern, hover your mouse cursor over it and hit "/" button
;  5) wait while the script scrolls and declines the quests
;  6) found a quest you're happy with? hit "\" immediately to terminate the script :)
;  Problems? Press "\" and the script terminates.
;  log file "a2q.log" is written into the directory where you run the script
;  configuration file "a2q.log" is written into the directory where you run the script. Feel free to edit the values