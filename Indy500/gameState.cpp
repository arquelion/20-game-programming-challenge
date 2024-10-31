#include "precomp.h"

#include "gameState.h"

void GameState::init()
{
    track_ = std::make_shared<Track>();
    scoreboard_ = std::make_shared<Scoreboard>();
}

void GameState::newGame()
{
    client_ = TcpConnection::connect(ioContext_, "localhost");
    NetCommand level, ready, index;
    asyncLoadLevel(level);
    asyncLoadLevel(ready);
    asyncLoadLevel(index);
    ioContext_.run();
    asyncGetServerUpdate();
}

bool GameState::isGameOver() const
{
    return false;
}

void GameState::accelerate(float snAccel)
{
    NetCommand cmd;
    cmd.dataLength = 0;
    cmd.type = NetCommand::CommandType::ACCELERATE;
    cmd.float32 = snAccel;
    client_->write(cmd);
    ioContext_.run();
}

void GameState::rotate(float snRotation)
{
    NetCommand cmd;
    cmd.dataLength = 0;
    cmd.type = NetCommand::CommandType::ROTATE;
    cmd.float32 = snRotation;
    client_->write(cmd);
    ioContext_.run();
}

void GameState::update(float deltaSec)
{
    ioContext_.poll();
    scoreboard_->update(cars_[playerIndex].lapDuration, cars_[playerIndex].highestLap);
}

void GameState::draw() const
{
    track_->draw();
    scoreboard_->draw();
    for (auto& car : cars_)
    {
        car.draw();
    }
}

void GameState::quit()
{
    server_->kill();
}

void GameState::handleGamePrep(int numCars)
{
    cars_.clear();
    for (int i = 0; i < numCars; ++i)
    {
        Car car;
        car.loadTexture();
        cars_.push_back(car);
    }
}

void GameState::asyncGetServerUpdate()
{
    boost::asio::async_read(client_->socket(), boost::asio::buffer(&updateData_, sizeof(updateData_)),
        [this](boost::system::error_code ec, std::size_t length)
    {
        if (ec)
        {

        }
        else
        {
            for (int i = 0; i < updateData_.cars.size() && i < cars_.size(); ++i)
            {
                cars_[i] = updateData_.cars[i];
            }
        }
        asyncGetServerUpdate();
    });
}

void GameState::asyncLoadLevel(NetCommand& cmd)
{
    boost::asio::async_read(client_->socket(), boost::asio::buffer(&cmd, sizeof(cmd)),
        [this, &cmd](boost::system::error_code ec, std::size_t length)
    {
        if (ec)
        {

        }
        else
        {
            switch (cmd.type)
            {
            case NetCommand::CommandType::LEVEL_LAYOUT:
                currentLevel = cmd.number;
                track_->loadLevel(currentLevel);
                break;
            case NetCommand::CommandType::GAME_PREP:
                handleGamePrep(cmd.number);
                break;
            case NetCommand::CommandType::PLAYER_INDEX:
                playerIndex = cmd.number;
                break;
            default:
                throw std::exception("invalid command");
            }
        }
    });
}
