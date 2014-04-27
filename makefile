sharemounter: sharemounter.c packages
	gcc -o sharemounter -ansi sharemounter.c  

packages:
	apt-get install afpfs-ng

clean:
	($RM) sharemounter
