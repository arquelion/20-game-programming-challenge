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

void TcpConnection::write(const NetCommand& cmd, const void* data)
{
    write(cmd);
    boost::asio::write(socket_, boost::asio::buffer(data, cmd.dataLength));
}

void TcpConnection::handleWrite(const boost::system::error_code&, size_t)
{
}

void TcpServer::update()
{
    processObjects();
    for (auto& player : players_)
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
        players_.emplace_back(std::make_unique<ClientContext>(newConnection, NetCommand{}, (int)players_.size()));
        newConnection->start();
    }

    if (players_.size() < 2)
    {
        //startAccept();
    }

    prepareGame();
    startListeningToPlayers();
    runGame();
}

void TcpServer::startReceive(ClientContext* clientContext)
{
    auto& command = clientContext->command;
    boost::asio::async_read(clientContext->client->socket(), boost::asio::buffer(&command, sizeof(command)),
        [this, clientContext, &command](boost::system::error_code ec, std::size_t length)
        {
            if (ec)
            {

            }
            else
            {
                switch (command.type)
                {
                case NetCommand::CommandType::ACCELERATE:
                    acceleratePlayer(0, command.float32);
                    break;
                case NetCommand::CommandType::ROTATE:
                    rotatePlayer(0, command.float32);
                    break;
                case NetCommand::CommandType::LEVEL_LAYOUT:

                    break;
                case NetCommand::CommandType::GAME_PREP:
                    break;
                default:
                    throw std::exception("invalid command");
                }
            }
            startReceive(clientContext);
        });
}

void TcpServer::startListeningToPlayers()
{
    cars_.clear();
    cars_.resize(players_.size());
    for (auto& player : players_)
    {
        startReceive(player.get());
    }
}

void TcpServer::prepareGame()
{
    AABB block;
    block.center = { 10, 10 };
    block.radius = { 4, 4 };
    arena_.collideables.push_back(block);

    NetCommand level, ready;
    level.type = NetCommand::CommandType::LEVEL_LAYOUT;
    level.number = 0;
    ready.type = NetCommand::CommandType::GAME_PREP;
    ready.number = 1;
    for (auto& player : players_)
    {
        player->client->write(level);
        player->client->write(ready);
    }
}

void TcpServer::runGame()
{
    auto lastUpdate = clock_.now();
    updateInterval_ = std::chrono::milliseconds(16);
    while (true)
    {
        std::this_thread::sleep_until(lastUpdate + updateInterval_);
        lastUpdate += updateInterval_;
        ioContext_.poll();
        update();
    }
}

void TcpServer::sendUpdate(ClientContext* player)
{
    GameUpdateData data;
    for (int i = 0; i < cars_.size() && i < data.cars.size(); ++i)
    {
        data.cars[i] = cars_[i];
    }
    boost::asio::write(player->client->socket(), boost::asio::buffer(&data, sizeof(GameUpdateData)));
}

void TcpServer::processObjects()
{
    for (auto& car : cars_)
    {
        // Decay side velocity more than forward velocity.
        glm::vec2 headUnit{ cos(car.heading), sin(car.heading) };
        glm::vec2 sideUnit{ sin(car.heading), -cos(car.heading) };
        car.velocity = 0.99f * glm::dot(headUnit, car.velocity) * headUnit
            + 0.90f * glm::dot(sideUnit, car.velocity) * sideUnit;
        auto dir = car.velocity * (float)duration_cast<std::chrono::milliseconds>(updateInterval_).count() / 1000.f;
        auto intersect = checkForIntersect(car, dir);
        switch (intersect.object)
        {
        case CollisionObject::NONE:
            car.sprite.setCenter(car.sprite.getCenter() + dir);
            break;
        case CollisionObject::WALL:
            car.sprite.setCenter(intersect.closest.pos);
            car.velocity = { 0, 0 };
            break;
        case CollisionObject::CAR:
            break;
        default:
            throw std::exception("invalid collision");
        }
    }
}
Intersect TcpServer::checkForIntersect(CarData& car, glm::vec2 dir) const
{
    auto carBox = car.sprite.getBoundingBox();
    Sweep closest;
    auto nextCollisionObject = CollisionObject::NONE;

    // Walls
    for (auto& wall : arena_.walls)
    {
        auto sweep = wall.sweepAABB(carBox, dir);
        if (sweep.t != 1.f && (!closest.hit || sweep.hit->t < closest.hit->t))
        {
            closest = sweep;
            nextCollisionObject = CollisionObject::WALL;
        }
    }

    for (auto& object : arena_.collideables)
    {
        auto sweep = object.sweepAABB(carBox, dir);
        if (sweep.t != 1.f && (!closest.hit || sweep.hit->t < closest.hit->t))
        {
            closest = sweep;
            nextCollisionObject = CollisionObject::WALL;
        }
    }

    // Cars

    return { closest, nextCollisionObject };
}

void TcpServer::acceleratePlayer(int playerIndex, float snAccel)
{
    auto& car = cars_[playerIndex];
    car.velocity += glm::vec2(cos(car.heading), sin(car.heading)) * car.maxAccel * snAccel;
}

void TcpServer::rotatePlayer(int playerIndex, float snRotation)
{
    auto& car = cars_[playerIndex];
    car.heading += snRotation * car.maxRotation;
}

// Non axis-aligned bounding boxes
// More terrain
// Scoreboard
// Lap Detection
