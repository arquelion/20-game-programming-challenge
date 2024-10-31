#pragma once
#include "game.h"

#include "car.h"

#include "track.h"
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
    
    void accelerate(float snAccel);
    void rotate(float snRotation);

    void update(float deltaSec);
    void draw() const;
    void quit();

    // Game elements
    std::shared_ptr<Track> track_;
    std::shared_ptr<Scoreboard> scoreboard_;
    std::vector<Car> cars_;

    bool isActive_ = true;

    Clock clock_;

private:
    enum class LevelState
    {
        LOAD,
        READY,
        PLAY,
    } levelState;

    int32_t playerIndex = 0;
    int32_t currentLevel = 1;

    Clock::time_point transitionTime_;

    boost::asio::io_context ioContext_;
    std::shared_ptr<TcpServer> server_;
    TcpConnection::pointer client_;

    GameUpdateData updateData_;
    
    void handleGamePrep(int numCars);
    void asyncGetServerUpdate();
    void asyncLoadLevel(NetCommand& cmd);
};
