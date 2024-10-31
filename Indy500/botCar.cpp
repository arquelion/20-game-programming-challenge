#include "precomp.h"

#include "botCar.h"

void BotCar::run()
{
    while (isRunning_)
    {
        while (ioContext_.poll() > 0);
        if (glm::length(track_->data.beacons[nextBeacon].center - cars_[carIndex].object.boundingBox.center) < 10.f)
        {
            nextBeacon = (nextBeacon + 1) % track_->data.beacons.size();
        }
        Radians angle = getAngleBetweenVectors(
            getUnitVector(cars_[carIndex].object.renderObj.heading),
            track_->data.beacons[nextBeacon].center - cars_[carIndex].object.boundingBox.center
        );
        snRotation = 0.7 * std::clamp((float)(angle / (M_PI / 4)), -1.f, 1.f);
        accelerate(snAccel);
        rotate(snRotation);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void BotCar::newGame()
{
    track_ = std::make_shared<Track>();
    track_->loadLevel(0);

    client_ = TcpConnection::connect(ioContext_, "localhost");
    NetCommand level, ready, index;
    asyncLoadLevel(level);
    asyncLoadLevel(ready);
    asyncLoadLevel(index);
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
                cars_[i] = updateData_.cars[i];
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
                cars_.clear();
                for (uint32_t i = 0; i < cmd.number; ++i)
                {
                    Car car;
                    cars_.push_back(car);
                }
                break;
            case NetCommand::CommandType::PLAYER_INDEX:
                carIndex = cmd.number;
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
