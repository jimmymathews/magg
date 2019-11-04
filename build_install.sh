gcc -g magg.c -o magg

if test -f "/usr/local/share/man/man1/magg.1.gz";
then
	sudo rm /usr/local/share/man/man1/magg.1.gz
fi

sudo cp magg.1 /usr/local/share/man/man1/
sudo gzip /usr/local/share/man/man1/magg.1

if test -f "magg";
then
	sudo cp magg /usr/local/bin/
fi
