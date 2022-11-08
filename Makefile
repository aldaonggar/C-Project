main: main.c datamgr.c lib/dplist.c
	mkdir -p build
	gcc -Wall -Werror -o build/datamgr main.c datamgr.c lib/dplist.c

file_creator: file_creator.c
	mkdir -p build
	gcc -Wall -Werror -o build/file_creator file_creator.c

clean: 
	rm -r build