#include "precomp.h"

#include "server.h"

using boost::asio::ip::tcp;

TcpConnection::pointer TcpConnection::create(boost::asio::io_context& ioContext)
{
    return pointer(new TcpConnection(ioContext));
}

TcpConnection::pointer TcpConnection::connect(boost::asio::io_context& ioContext, std::string hostname)
{
    auto connection = create(ioContext);
    tcp::resolver resolver(ioContext);
    char service[6];
    sprintf_s(service, "%u", TcpServer::PORT_NUM);
    auto endpoints = resolver.resolve(hostname, service);
    boost::asio::connect(connection->socket_, endpoints);
    return connection;
}

void TcpConnection::start()
{
    message_ = "test";

    boost::asio::write(socket_, boost::asio::buffer(message_));
}

std::string TcpConnection::read()
{
    boost::asio::streambuf b;
    boost::asio::streambuf::mutable_buffers_type buf = b.prepare(256);
    size_t n = socket_.receive(buf);
    b.commit(n);
    std::istream is(&b);
    std::string msg;
    is >> msg;
    return msg;
}

void TcpConnection::handleWrite(const boost::system::error_code&, size_t)
{
}

void TcpServer::update()
{
    processObjects();
    for (auto& player : players)
    {
        sendUpdate(player->client);
    }
}

void TcpServer::startAccept()
{
    TcpConnection::pointer newConnection =
        TcpConnection::create(ioContext_);

    acceptor_.async_accept(newConnection->socket(),
        std::bind(&TcpServer::handleAccept, this, newConnection,
            boost::asio::placeholders::error));
}

void TcpServer::handleAccept(TcpConnection::pointer newConnection, const boost::system::error_code& error)
{
    if (!error)
    {
        players.emplace_back(newConnection);
        newConnection->start();
    }

    if (players.size() < 2)
    {
        //startAccept();
    }
    else
    {
        startGame();
    }
}

void TcpServer::startReceive(TcpConnection::pointer client)
{
    NetCommand command;
    auto self(shared_from_this());
    boost::asio::async_read(client->socket(), boost::asio::buffer(&command, sizeof(NetCommand)),
        [this, self, command](boost::system::error_code ec, std::size_t length)
        {
            if (ec || length < offsetof(NetCommand, number) || command.remainingLength)
            {

            }
        });
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
