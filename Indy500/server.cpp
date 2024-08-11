#include "precomp.h"

#include "car.h"

#include "server.h"

using boost::asio::ip::tcp;

TcpConnection::pointer TcpConnection::create(boost::asio::io_context& ioContext)
{
    return std::make_shared<TcpConnection>(ioContext);
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

void TcpConnection::write(const NetCommand& cmd)
{
    boost::asio::write(socket_, boost::asio::buffer(&cmd, sizeof(cmd)));
}

void TcpConnection::handleWrite(const boost::system::error_code&, size_t)
{
}

void TcpServer::update()
{
    processObjects();
    for (auto& player : players)
    {
        sendUpdate(player.get());
    }
}

void TcpServer::startAccept()
{
    TcpConnection::pointer newConnection =
        TcpConnection::create(ioContext_);

    boost::system::error_code error;
    try
    {
        acceptor_.accept(newConnection->socket());
    }
    catch (boost::system::error_code e)
    {
        error = e;
    }
    handleAccept(newConnection, error);
}

void TcpServer::handleAccept(TcpConnection::pointer newConnection, const boost::system::error_code& error)
{
    if (!error)
    {
        players.emplace_back(std::make_unique<ClientContext>(newConnection, NetCommand{}, (int)players.size()));
        newConnection->start();
    }

    if (players.size() < 2)
    {
        //startAccept();
    }

    startGame();
}

void TcpServer::startReceive(ClientContext* clientContext)
{
    auto command = clientContext->command;
    boost::asio::async_read(clientContext->client->socket(), boost::asio::buffer(&command, sizeof(command)),
        [this, &command](boost::system::error_code ec, std::size_t length)
        {
            if (ec)
            {

            }
            else
            {
                switch (command.type)
                {
                case NetCommand::CommandType::MOVE:
                    movePlayer(0, glm::vec2{ command.vector2[0], command.vector2[1]});
                    break;
                default:
                    throw std::exception("invalid command");
                }
            }
        });
}

void TcpServer::startGame()
{
    cars.clear();
    cars.resize(players.size());
    for (auto& player : players)
    {
        startReceive(player.get());
    }

    auto lastUpdate = clock.now();
    updateInterval = std::chrono::seconds(1);
    while (true)
    {
        std::this_thread::sleep_until(lastUpdate + updateInterval);
        lastUpdate += updateInterval;
        ioContext_.poll();
        update();
    }
}

void TcpServer::sendUpdate(ClientContext* player)
{
}

void TcpServer::processObjects()
{
}

void TcpServer::movePlayer(int playerIndex, glm::vec2 dir)
{
    auto& car = cars[playerIndex];
    car.velocity += dir;
}

// server updates: process commands since last update, advance server state based on time elapsed, send updated state to clients
// need separate io context for server to poll async functions
// client updates: read updates from server, 
