#include "precomp.h"

#include "server.h"

using boost::asio::ip::tcp;

inline TcpConnection::pointer TcpConnection::create(boost::asio::io_context& ioContext)
{
    return pointer(new TcpConnection(ioContext));
}

inline void TcpConnection::start()
{
    boost::asio::async_write(socket_, boost::asio::buffer(message_),
        std::bind(&TcpConnection::handleWrite, shared_from_this(),
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));
}

inline void TcpConnection::handleWrite(const boost::system::error_code&, size_t)
{
}

void TcpServer::update()
{
    processObjects();
    for (auto& player : players)
    {
        sendUpdate(player);
    }
}

inline void TcpServer::startAccept()
{
    TcpConnection::pointer newConnection =
        TcpConnection::create(ioContext_);

    acceptor_.async_accept(newConnection->socket(),
        std::bind(&TcpServer::handleAccept, this, newConnection,
            boost::asio::placeholders::error));
}

inline void TcpServer::handleAccept(TcpConnection::pointer newConnection, const boost::system::error_code& error)
{
    if (!error)
    {
        players.push_back(newConnection);
    }

    if (players.size() < 2)
    {
        startAccept();
    }
    else
    {
        startGame();
    }
}

void TcpServer::startGame()
{

}

void TcpServer::sendUpdate(TcpConnection::pointer player)
{
}

void TcpServer::processObjects()
{
}
