all:
	gcc register.c -o register
	gcc chat_server.c -o chat_server
	gcc chat.c -o chat
clean:
	rm register chat_server
