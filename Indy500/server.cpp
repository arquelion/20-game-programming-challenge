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
    Car defaultCar;
    defaultCar.lapStartTime = clock_.now();
    defaultCar.lapDuration = defaultCar.lapDuration.zero();

    cars_.clear();
    cars_.resize(players_.size(), defaultCar);
    for (auto& player : players_)
    {
        startReceive(player.get());
    }
}

void TcpServer::prepareGame()
{
    OBB block = { {0, 0}, {50, 40} };
    trackData.collideables.push_back(block);

    trackData.startingLine.center = { 30, 30 };
    trackData.dir = TrackData::Direction::COUNTERCLOCKWISE;

    auto numMarkers = 3;
    Radians increment = glm::two_pi<float>() / numMarkers;
    if (trackData.dir == TrackData::Direction::CLOCKWISE)
    {
        increment *= -1;
    }

    for (int i = 0; i < numMarkers; ++i)
    {
        auto center = trackData.startingLine.center;
        std::vector<glm::vec2> vertices = { center };
        auto angle = trackData.startingLine.angle + increment * i;
        auto markerDirHat = glm::vec2(cos(angle), -sin(angle));
        vertices.push_back(center + markerDirHat * 100.f);
        vertices.push_back(center + epsilon * glm::vec2(markerDirHat.y, -markerDirHat.x));
        trackData.lapMarkers.push_back(OBB(center, vertices));
    }

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
    auto now = clock_.now();

    for (auto& car : cars_)
    {
        auto heading = car.object.renderObj.heading;
        auto& velocity = car.object.renderObj.velocity;
        // Decay side velocity more than forward velocity.
        glm::vec2 headUnit{ cos(heading), sin(heading) };
        glm::vec2 sideUnit{ sin(heading), -cos(heading) };
        velocity = 0.99f * glm::dot(headUnit, velocity) * headUnit
            + 0.90f * glm::dot(sideUnit, velocity) * sideUnit;

        auto dir = velocity * (float)duration_cast<std::chrono::milliseconds>(updateInterval_).count() / 1000.f;
        auto intersect = checkForIntersect(car, dir);
        switch (intersect.object)
        {
        case CollisionObject::NONE:
            car.object.translate(dir);
            break;
        case CollisionObject::WALL:
            dir *= intersect.closest.t;
            car.object.translate(dir);
            velocity = { 0, 0 };
            break;
        case CollisionObject::CAR:
            break;
        default:
            throw std::exception("invalid collision");
        }

        car.lapDuration = std::chrono::duration_cast<std::chrono::milliseconds>(now - car.lapStartTime);
        auto marker = checkForProgress(car, dir); // returns std::optional
        if (marker)
        {
            car.updateProgress(*marker, (int)trackData.lapMarkers.size());
        }
    }
}
Intersect TcpServer::checkForIntersect(const Car& car, glm::vec2 dir) const
{
    auto carBox = car.object.boundingBox;
    Sweep closest;
    auto nextCollisionObject = CollisionObject::NONE;

    // Walls
    for (auto& wall : trackData.walls)
    {
        auto sweep = wall.sweepOBB(carBox, dir);
        if ((sweep.t != 1.f) && (sweep.t < closest.t))
        {
            closest = sweep;
            nextCollisionObject = CollisionObject::WALL;
        }
    }

    for (auto& object : trackData.collideables)
    {
        auto sweep = object.sweepOBB(carBox, dir);
        if ((sweep.t != 1.f) && (sweep.t < closest.t))
        {
            closest = sweep;
            nextCollisionObject = CollisionObject::WALL;
        }
    }

    // Cars

    return { closest, nextCollisionObject };
}

bool TcpServer::isIntersecting(const Car& car) const
{
    return checkForIntersect(car, { 0, 0 }).closest.t < 1.0f;
}

std::optional<int> TcpServer::checkForProgress(const Car& car, glm::vec2 dir) const
{
    auto carBox = car.object.boundingBox;
    Sweep closest;
    auto nextCollisionObject = CollisionObject::NONE;

    for (int i = 0; i < trackData.lapMarkers.size(); ++i)
    {
        auto sweep = trackData.lapMarkers[i].sweepOBB(carBox, dir);
        if ((sweep.t != 1.f) && (sweep.t < closest.t))
        {
            return std::make_optional(i);
        }
    }

    return std::nullopt;
}

void TcpServer::acceleratePlayer(int playerIndex, float snAccel)
{
    auto& car = cars_[playerIndex];
    auto& body = car.object.renderObj;
    body.velocity += glm::vec2(cos(body.heading), sin(body.heading)) * car.maxAccel * snAccel;
}

void TcpServer::rotatePlayer(int playerIndex, float snRotation)
{
    auto& car = cars_[playerIndex];
    assert(!isIntersecting(car));
    Radians delta = snRotation * car.maxRotation;
    car.object.rotate(delta);

    auto intersect = checkForIntersect(car, glm::vec2(0));
    if (intersect.closest.hit) {
        car.object.translate(intersect.closest.hit->delta);
    }
    car.object.renderObj.heading += delta;
}
