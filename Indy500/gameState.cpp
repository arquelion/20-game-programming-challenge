#include "precomp.h"

#include "gameState.h"

void GameState::init()
{
    //arena_ = std::make_shared<Arena>();
    //scoreboard_ = std::make_shared<Scoreboard>();
    car_ = std::make_shared<Car>();
}

void GameState::newGame()
{
    client_ = TcpConnection::connect(ioContext_, "localhost");
    ioContext_.poll();
    auto msg = client_->read();

    NetCommand cmd;
    cmd.remainingLength = 0;
    cmd.type = NetCommand::CommandType::MOVE;
    cmd.vector2 = { 0, 1 };
    client_->write(cmd);
    ioContext_.run();
    return;
}

bool GameState::isGameOver() const
{
    return false;
}

void GameState::update(float deltaSec)
{
    ioContext_.poll();
}

void GameState::draw() const
{
    //arena_->draw();
    //scoreboard_->draw();
    car_->draw();
}
