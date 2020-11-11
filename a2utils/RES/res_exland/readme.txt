Author: Nival Interactive
Categories: RoM 2, For developers

Tool for modifying .res files.
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
 vr: make Release version                       c: command mode
 vd: make Debug version                         t: Test integrity of archive
 ?,h: get this Help