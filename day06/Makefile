server:
	g++ util.cpp client.cpp -o client -g && \
        g++ util.cpp server.cpp Epoll.cpp InetAddress.cpp Socket.cpp Channel.cpp -o server -g

clean:
	rm server && rm client
