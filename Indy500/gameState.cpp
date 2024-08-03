#include "precomp.h"

#include "gameState.h"

void GameState::init()
{

}

void GameState::newGame()
{
    client_ = TcpConnection::connect(ioContext_, "localhost");
    ioContext_.run();
    auto msg = client_->read();
    return;
}

bool GameState::isGameOver() const
{
    return false;
}

void GameState::update(float deltaSec)
{

}

void GameState::draw() const
{

}
