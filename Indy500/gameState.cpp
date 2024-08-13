#include "precomp.h"

#include "gameState.h"

void GameState::init()
{
    //arena_ = std::make_shared<Arena>();
    //scoreboard_ = std::make_shared<Scoreboard>();
    cars_.push_back(std::make_shared<Car>());
}

void GameState::newGame()
{
    client_ = TcpConnection::connect(ioContext_, "localhost");
    ioContext_.poll();
    startReceive();
}

bool GameState::isGameOver() const
{
    return false;
}

void GameState::accelerate(int player, float snAccel)
{
    NetCommand cmd;
    cmd.remainingLength = 0;
    cmd.type = NetCommand::CommandType::ACCELERATE;
    cmd.float32 = snAccel;
    client_->write(cmd);
    ioContext_.run();
}

void GameState::rotate(int player, float snRotation)
{
    NetCommand cmd;
    cmd.remainingLength = 0;
    cmd.type = NetCommand::CommandType::ROTATE;
    cmd.float32 = snRotation;
    client_->write(cmd);
    ioContext_.run();
}

void GameState::update(float deltaSec)
{
    ioContext_.poll();
}

void GameState::draw() const
{
    //arena_->draw();
    //scoreboard_->draw();
    for (auto& car : cars_)
    {
        car->draw();
    }
}

void GameState::startReceive()
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
                cars_[i]->data = updateData_.cars[i];
            }
        }
        startReceive();
    });
}
