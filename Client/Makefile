LIBS=$(wildcard Libs/*)
INCLUDE_PATH=Libs/


CC=g++
CFLAGS= -c -std=c++11 -Wall -g

client: libraries
	$(CC) main.cpp $(wildcard libraries/*) -I$(INCLUDE_PATH) -o file_client

libraries:
	mkdir libraries/
	for dir in $(LIBS); do \
		cd $$dir; \
		$(CC) $(CFLAGS) *.cpp -I../; \
		mv *.o ../../libraries; \
		cd -; \
	done

clean: 
	rm -rf libraries/ server
