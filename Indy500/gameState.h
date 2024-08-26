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
    GameState() : server_(std::make_shared<TcpServer>()) {}
    void init();

    void newGame();
    bool isGameOver() const;
    
    void accelerate(int player, float snAccel);
    void rotate(int player, float snRotation);

    void update(float deltaSec);
    void draw() const;

    // Game elements
    std::shared_ptr<Arena> arena_;
    std::shared_ptr<Scoreboard> scoreboard_;
    std::vector<std::shared_ptr<Car>> cars_;

    bool isActive_ = true;

    Clock clock_;

private:
    enum class LevelState
    {
        LOAD,
        READY,
        PLAY,
    } levelState;

    int32_t currentLevel = 1;

    Clock::time_point transitionTime_;

    boost::asio::io_context ioContext_;
    std::shared_ptr<TcpServer> server_;
    TcpConnection::pointer client_;

    GameUpdateData updateData_;

    void startReceive();
    void prepareGame(NetCommand& cmd);
};
