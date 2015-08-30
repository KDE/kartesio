#
# Regular cron jobs for the kartesio package
#
0 4	* * *	root	[ -x /usr/bin/kartesio_maintenance ] && /usr/bin/kartesio_maintenance
