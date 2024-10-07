#include "precomp.h"

#include "botCar.h"

void BotCar::run()
{
    while (isRunning_)
    {
        while (ioContext_.poll() > 0);
        accelerate(snAccel);
        rotate(snRotation);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void BotCar::newGame()
{
    track_ = std::make_shared<Track>();
    cars_.push_back(std::make_shared<Car>());
    track_->loadLevel(0);

    client_ = TcpConnection::connect(ioContext_, "localhost");
    NetCommand level, ready;
    asyncLoadLevel(level);
    asyncLoadLevel(ready);
    ioContext_.run();
    asyncGetServerUpdate();
}

void BotCar::accelerate(float snAccel)
{
    NetCommand cmd;
    cmd.dataLength = 0;
    cmd.type = NetCommand::CommandType::ACCELERATE;
    cmd.float32 = snAccel;
    client_->write(cmd);
    ioContext_.poll();
}

void BotCar::rotate(float snRotation)
{
    NetCommand cmd;
    cmd.dataLength = 0;
    cmd.type = NetCommand::CommandType::ROTATE;
    cmd.float32 = snRotation;
    client_->write(cmd);
    ioContext_.poll();
}

void BotCar::asyncGetServerUpdate()
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
                *cars_[i] = updateData_.cars[i];
            }
        }
        asyncGetServerUpdate();
    });
}

void BotCar::asyncLoadLevel(NetCommand& cmd)
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
                break;
            default:
                throw std::exception("invalid command");
            }
        }
    });
}

void botCarThread()
{
    BotCar bot;
    bot.newGame();
    bot.run();
}
