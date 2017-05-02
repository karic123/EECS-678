STUDENT_ID=XXXXXXX

all: client server

%: %.c
	gcc -g $^ -o $@ -lm

test: client server
	bash -c "./server & sleep 1; ./client"

clean:
	rm -f client server mysock

zip: clean
	mkdir $(STUDENT_ID)-sockets-lab
	cp client.c server.c Makefile $(STUDENT_ID)-sockets-lab/
	zip -r $(STUDENT_ID)-sockets-lab.zip $(STUDENT_ID)-sockets-lab
	rm -rf $(STUDENT_ID)-sockets-lab

.PHONY: all test clean zip
