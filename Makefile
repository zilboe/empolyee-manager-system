all:
	gcc ./server/*.c -o Server -l sqlite3
	gcc ./client/*.c -o Client
clean:
	rm -rf Server
	rm -rf Client
