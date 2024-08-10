#include "precomp.h"

#include "gameState.h"

void GameState::init()
{

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

}
