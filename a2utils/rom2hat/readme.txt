rom_hat
Author: Vladimir Chebotarev aka ex-lend
Categories: RoM 2, Common
Version: 0.92.47 BETA

RoM II master server (hat).

Features:

automatic registration possibility
works with all versions of protocol (7EN/7RU/8RU/10RU)
IP filtering
ban history storing
list of players online
working behind NAT possibility (using map_addr, see below)

Configuration file:
Default file's name is rom_hat.conf. One can use another name of configuration file, specifying it in first command line argument of rom_hat. List of options and default values are described below.

#server = id | name | addr [| map_addr] # add server to list of servers
#silent = 0                             # be silent (use log only)
#hat_id = 1000                          # hat identifier
#log_path = ./rom_hat.log               # log file name
#stat_path = ./rom_hat.stat             # status file name
#servers_addr = 0.0.0.0                 # address for servers
#servers_port = 8001                    # port for servers
#clients_addr = 0.0.0.0                 # address for clients
#clients_port = 8000                    # port for clients
#hat_version = 10                       # hat version (7/8/9)
#hat_language = russian                 # language (ru/rus/russian, en/eng/english)
#any_version = 1                        # allow to connect using all versions and languages (for debugging)
#output_codepage = koi8-r               # output codepage (log and stdout)(koi8/koi8r/koi8-r, 866/cp866/dos, 1251/cp1251/win)
#chr_base_path = ./chr_base/            # character base directory
#ctl_path = ./ctl/                      # control directory
#autoreg_mode = 0                       # automatic registration mode

It seems it is quite clear, but it's worth while mentioning add server arguments:

id - server id (number)
name - server name (for server list)
addr - address, which is used by server to connect to hat (server's address, not hat's)
map_addr - address of server, that'd listen for clients

File example:

any_version = 0
output_codepage = cp1251
hat_version = 7
hat_language = russian
chr_base_path = ./chr/

server = 101 | Server 1 | 192.168.0.5:8001
server = 102 | Server 2 | 192.168.0.5:8003 | 192.168.0.5:8002
server = 103 | Server 3 | 192.168.0.2:8003 | ex-lend.homeunix.org:8002

autoreg_mode = 1

Control directory

If you place file in this directory, server will run some action, depending of file name.

File can have any extension, but name should be following one:

shutdown - shuts hat down, contents of file makes no difference
createaccount - creates account, first line is login name, second if password
changepassword - changes password, first line is login name, second if old password, third is new password
removeaccount - deletes account, first line is login name, second if password

If command fails, hat will create filebad_<oldfilename> in this dir.

Status file

Status file is named rom_hat.stat by default. Each line of it contains info about player in following format:

server_id log_in_time char_id_1 char_id_2 login_name char_name

server_id - server id (number)
log_in_time - log in time
(char_id_1, char_id_2) - character id
login_name - name of login
char_name - name of character

Ban files

File names: <chr_base>/l/login_name.ban, where login_name - name of login.
Contains list of bans of player. Each line has following format:

DD.MM.YYYY HH:MM:SS;PERIOD[;REASON]

DD.MM.YYYY HH:MM:SS - ban time
PERIOD - ban period in days, or -1 for forever ban
REASON - ban reason, optional

IP filter files

File names: <chr_base>/default.ipf, <chr_base>/l/login_name.ipf, where login_name - name of login.
First file sets global IP filter, if it doesn't match IP, second file won't be used at all.
Each line of these files has following format:

[-/+]AA.BB.CC.DD[/CIDR]

[-/+] means DENY (+) or ALLOW (-) access to this host or subnetwork. + is default policy.

AA.BB.CC.DD[/CIDR] - IP address + number of specified bits in it (CIDR). CIDR is 32 by default (1 host).

Files are read line by line, and if current line matches IP, server stops reading it.

+Notes about rom_hat from James (King APK) (origin):
rom_hat_info.rtf