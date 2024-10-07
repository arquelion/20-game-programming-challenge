#pragma once
#include "server.h"

class BotCar
{
public:
    void run();
    void newGame();

    void accelerate(float snAccel);
    void rotate(float snRotation);

    void asyncGetServerUpdate();
    void asyncLoadLevel(NetCommand& cmd);

    // Game elements
    std::shared_ptr<Track> track_;
    std::vector<std::shared_ptr<Car>> cars_;

    boost::asio::io_context ioContext_;
    TcpConnection::pointer client_;

    GameUpdateData updateData_;

    bool isRunning_ = true;
    std::chrono::milliseconds updateInterval = 100ms;
    int32_t currentLevel = 1;

    float snAccel = 1.f;
    float snRotation = 0.f;
};

void botCarThread();
