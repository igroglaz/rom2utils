To protect reg form in web server:

1) add right full path to your folder in .htaccess:
 AuthUserFile /home/*********/public_html/.htpasswd
2) htaccess got such structure:
login:password_in_md5
to make password - google "md5 .htaccess"