#pragma once
class TcpConnection;
class TcpServer;

#include "car.h"
#include "track.h"

#include "clock.h"
#include "collision.h"

struct NetCommand
{
    enum class CommandType : uint32_t
    {
        ACCELERATE,
        ROTATE,

        LEVEL_LAYOUT,
        GAME_PREP, // number is player count
        PLAYER_INDEX,
    } type;

    uint32_t dataLength;

    union
    {
        uint32_t number{ 0 };
        float float32;
        std::array<float, 2> vector2;
    };
};

struct GameUpdateData
{
    std::array<NetCar, 2> cars;
};

class TcpConnection
    : public std::enable_shared_from_this<TcpConnection>
{
public:
    typedef std::shared_ptr<TcpConnection> pointer;

    TcpConnection(boost::asio::io_context& ioContext)
        : socket_(ioContext)
    {
    }
    ~TcpConnection() {}

    static pointer create(boost::asio::io_context& ioContext);
    static pointer connect(boost::asio::io_context& ioContext, std::string hostname);

    boost::asio::ip::tcp::socket& socket()
    {
        return socket_;
    }

    void start();
    std::string read();
    void write(const NetCommand& cmd);
    void write(const NetCommand& cmd, const void* data);

private:

    void handleWrite(const boost::system::error_code& /*error*/,
        size_t /*bytes_transferred*/);

    boost::asio::ip::tcp::socket socket_;
    std::string message_;
};

class TcpServer : std::enable_shared_from_this<TcpServer>
{
public:
    static const boost::asio::ip::port_type PORT_NUM = 50013;

    TcpServer()
        : acceptor_(ioContext_, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), PORT_NUM))
    {
        thread_ = std::thread(&TcpServer::startAccept, this);
        thread_.detach();
    }

    void kill() { thread_.~thread(); }

    void update();

private:
    struct ClientContext
    {
        TcpConnection::pointer client;
        NetCommand command;
        int playerIndex;
    };

    void startAccept();
    void handleAccept(TcpConnection::pointer newConnection,
        const boost::system::error_code& error);

    void startReceive(ClientContext* clientContext);
    void startListeningToPlayers();
    void startBots();

    void prepareGame();
    void runGame();
    void sendUpdate(ClientContext* player);
    void processObjects();

    Intersect checkForIntersect(const Car& car, glm::vec2 dir) const;
    bool isIntersecting(const Car& car) const;
    std::optional<int> checkForProgress(const Car& car) const;

    void acceleratePlayer(int playerIndex, float snAccel);
    void rotatePlayer(int playerIndex, float snRotation);

    boost::asio::io_context ioContext_;
    boost::asio::ip::tcp::acceptor acceptor_;
    std::thread thread_;

    std::chrono::steady_clock clock_;
    std::chrono::steady_clock::duration updateInterval_;

    std::vector<std::unique_ptr<ClientContext>> players_;
    std::vector<Car> cars_;
    std::vector<std::thread> bots_;
    TrackData trackData;
};
