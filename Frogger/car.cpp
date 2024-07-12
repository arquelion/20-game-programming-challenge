#include "precomp.h"

#include "car.h"

#include "gameState.h"
#include "view.h"
#include "utils.h"

using namespace ci;

Car::Car(CarType type, float speed)
    : type(type)
{
    // TODO: load textures instead
    glm::vec2 radius;
    switch (type)
    {
    case CarType::TRUCK:
        color = Color::white();
        radius = { 8.f, 4.f };
        break;
    case CarType::RACER_1:
        color = Color(1.0f, 0.3f, 0.3f);
        radius = { 4.f, 4.f };
        break;
    case CarType::CAR:
        color = Color(0.3f, 0.3f, 1.0f);
        radius = { 4.f, 4.f };
        break;
    case CarType::TRACTOR:
        color = Color(0.3f, 1.0f, 0.3f);
        radius = { 4.f, 4.f };
        break;
    case CarType::RACER_2:
        color = Color(0.5f, 0.5f, 0.1f);
        radius = { 4.f, 4.f };
        break;
    }

    initialCenter = {
        (signum(speed) == 1)
            ? game.arena->playArea.x1 - radius.x
            : game.arena->playArea.x2 + radius.x,
        95.f + static_cast<int>(type) * 10.f
    };
    sprite = BoundedRect::create(initialCenter, radius);
    velocity = glm::vec2(speed, 0);
}
