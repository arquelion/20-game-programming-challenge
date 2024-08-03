#pragma once
class TcpConnection;
class TcpServer;

#include "clock.h"
#include "object2D.h"

struct NetCommand
{
    enum class CommandType : uint32_t
    {
        MOVE,
    };

    CommandType type;
    uint32_t remainingLength;

    union
    {
        uint32_t number;
    };
};

class TcpConnection
    : public std::enable_shared_from_this<TcpConnection>
{
public:
    typedef std::shared_ptr<TcpConnection> pointer;

    static pointer create(boost::asio::io_context& ioContext);
    static pointer connect(boost::asio::io_context& ioContext, std::string hostname);

    boost::asio::ip::tcp::socket& socket()
    {
        return socket_;
    }

    void start();
    std::string read();

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

class TcpServer : std::enable_shared_from_this<TcpServer>
{
public:
    static const boost::asio::ip::port_type PORT_NUM = 50013;

    TcpServer(boost::asio::io_context& ioContext)
        : ioContext_(ioContext),
        acceptor_(ioContext, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), PORT_NUM))
    {
        startAccept();
    }

    void update();

private:
    struct ClientContext
    {
        TcpConnection::pointer client;
        NetCommand command;
    };

    void startAccept();
    void handleAccept(TcpConnection::pointer newConnection,
        const boost::system::error_code& error);

    void startReceive(TcpConnection::pointer client);

    void startGame();
    void sendUpdate(TcpConnection::pointer player);
    void processObjects();

    boost::asio::io_context& ioContext_;
    boost::asio::ip::tcp::acceptor acceptor_;

    std::vector<std::unique_ptr<ClientContext>> players;
    std::vector<Object2D> cars;
};
