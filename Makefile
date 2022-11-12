main: main.c datamgr.c lib/dplist.c
	mkdir -p build
	gcc -Wall -Werror -DSET_MAX_TEMP=29 -DSET_MIN_TEMP=21 -o build/datamgr main.c datamgr.c lib/dplist.c

file_creator: file_creator.c
	mkdir -p build
	gcc -Wall -Werror -DDEBUG -o build/file_creator file_creator.c

clean: 
	rm -r build