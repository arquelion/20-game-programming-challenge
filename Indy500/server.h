#pragma once
class TcpConnection;
class TcpServer;

#include "clock.h"
#include "object2D.h"

class TcpConnection
    : public std::enable_shared_from_this<TcpConnection>
{
public:
    typedef std::shared_ptr<TcpConnection> pointer;

    static pointer create(boost::asio::io_context& ioContext);

    boost::asio::ip::tcp::socket& socket()
    {
        return socket_;
    }

    void start();

private:
    TcpConnection(boost::asio::io_context& ioContext)
        : socket_(ioContext)
    {
    }

    void handleWrite(const boost::system::error_code& /*error*/,
        size_t /*bytes_transferred*/);

    boost::asio::ip::tcp::socket socket_;
    std::string message_;
};

class TcpServer
{
public:
    TcpServer(boost::asio::io_context& ioContext)
        : ioContext_(ioContext),
        acceptor_(ioContext, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 13))
    {
        startAccept();
    }

    void update();

private:
    void startAccept();
    void handleAccept(TcpConnection::pointer newConnection,
        const boost::system::error_code& error);

    void startGame();
    void sendUpdate(TcpConnection::pointer player);
    void processObjects();

    boost::asio::io_context& ioContext_;
    boost::asio::ip::tcp::acceptor acceptor_;

    std::vector<TcpConnection::pointer> players;
    std::vector<Object2D> cars;
};
