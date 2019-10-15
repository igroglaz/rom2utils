Utils to repack data for Rage of Mages 2

In 'hats' folder there is data from different servers. 

Credits:
*** convert.php by Anotomix ***
*** customized by Tangar ***


Help (example of commands, change paths to make it work):

=======

res.exe ar "C:\games\Allods2\world.res" world\data "C:\games\Allods2\edit\world\data"

res.exe ar "C:\games\Allods2\world.res" world\data ai.reg data.bin itemname.bin itemname.pkt map.reg

databin2xml --compress "C:\games\Allods2\edit\original.xml" "C:\games\Allods2\edit\world\data\data.bin"

databin2xml "C:\games\Allods2\edit\world\data\data.bin" "C:\games\Allods2\edit\original.xml"

res.exe x "C:\games\Allods2\edit\world.res" world\data "C:\games\Allods2\edit\world\data"

res.exe l world.res


========

C:\games\Allods2\edit>databin2xml.exe
databin2xml 1.02, (c) Vladimir Chebotarev
usage: databin2xml file.bin file.xml
   or: databin2xml [--compress] file.xml file.bin

======




 Usage: RES <command> <resource file> <resource dir>{[filename]|[@filelist]...}
 a: Add files to archive:
    RES a C:\graphics.res graphics file1.bmp file2.bmp
     - adds to the root directory files file1.bmp and file2.bmp
    RES a C:\graphics.res graphics\bbb @filelist
     - adds to the resource directory bbb files containing in the filelist
 ar: likes a. Add files to archive recoursively
 d: Delete files from archive:
    RES d S:\artworks\graphics graphics\terrain\bitmap.bmp
    RES d S:\artworks\graphics.res @filelist 
     - deletes from resource entries containing in the filelist
 x: eXtract files with full pathname
    RES x S:\artworks\graphics graphics\terrain C:\pict
     - extracts files from resource directory terrain to the directoty C:\pict
 l: directory List
    RES l S:\artworks\graphics
     - gets list of all files in resource
    RES l S:\artworks\graphics graphics\terrain
     - gets list of all files in the resource directory terrain
 vr: make Release version			c: command mode
 vd: make Debug version				t: Test integrity of archive
 ?,h: get this Help