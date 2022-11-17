#prama once

class EventLoop;
class Socket;
class Acceptor;
class Server
{
private:
    EventLoop* loop;
    Acceptor *acceptor;
public:
    Server(EventLoop* loop);
    ~Server();

    void handleReadEvent(int);
    void newConnection(Socket *serv_sock);
};
