#pragma once
#include "game.h"

#include "car.h"

#include "arena.h"
#include "clock.h"
#include "scoreboard.h"
#include "textBox.h"

#include "server.h"

class GameState
{
public:
    GameState() : server_(std::make_shared<TcpServer>(ioContext_)) {}
    void init();

    void newGame();
    bool isGameOver() const;
    
    void update(float deltaSec);
    void draw() const;

    // Game elements
    std::shared_ptr<Arena> arena_;
    std::shared_ptr<Scoreboard> scoreboard_;
    std::shared_ptr<Car> car_;

    bool isActive_ = true;

    Clock clock_;

private:
    enum class LevelState
    {
        LOAD,
        READY,
        PLAY,
    } levelState;

    //int currentLevel = 1;

    Clock::time_point transitionTime_;

    boost::asio::io_context ioContext_;
    std::shared_ptr<TcpServer> server_;
    TcpConnection::pointer client_;
};
