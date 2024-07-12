#include <fstream>

#include "arena.h"
#include "view.h"

using namespace ci;

Arena::Arena(GameState& game)
    : game(game)
{
    // Walls
    Wall wall;
    wall.topLeft = { 0, 7 };
    wall.botRight = { 100, 9 };
    wall.boundingBox = AABB({ 50, 8 }, { 50, 1 });
    walls.push_back(wall);

    wall.topLeft = { 0, 7 };
    wall.botRight = { 2, 75 };
    wall.boundingBox = AABB({ 1, 41 }, { 1, 34 });
    walls.push_back(wall);

    wall.topLeft = { 98, 7 };
    wall.botRight = { 100, 75 };
    wall.boundingBox = AABB({ 99, 41 }, { 1, 34 });
    walls.push_back(wall);

    exit.boundingBox = AABB({ 50, 78 }, { 50, 1 });

    // Bricks
    bricks.reserve(MaxBricksPerLevel);
}

void Arena::removeBrick(const AABB* boundingBox)
{
    auto brick = AABBToBrick[*boundingBox];
    brick->hit();
    if (brick->hitsRemaining <= 0)
    {
        auto color = brick->getColor();
        if (color != BrickColor::GOLD)
        {
            --bricksToClear;
            game.scoreboard->incrementScore(brick->getColor(), game.currentLevel);
        }
    }
}

bool Arena::isLevelClear()
{
    return bricksToClear == 0 || game.paddle->wentThroughBonusExit;
}

void Arena::loadLevel(int level)
{
    bricks.clear();
    AABBToBrick.clear();

    char name[20];
    sprintf_s(name, "level%02d.txt", level);
    std::ifstream levelFile{ name, std::ifstream::in };

    std::string line;
    int numRows;
    levelFile >> numRows;
    std::getline(levelFile, line);

    for (int row = 0; row < numRows && row < 20; ++row)
    {
        std::getline(levelFile, line);
        for (int col = 0; col < line.size() && col < 16; ++col)
        {
            BrickColor color;
            bool isBrick = true;
            switch (line[col])
            {
            case 'W': color = BrickColor::WHITE; break;
            case 'O': color = BrickColor::ORANGE; break;
            case 'C': color = BrickColor::CYAN; break;
            case 'G': color = BrickColor::GREEN; break;
            case 'R': color = BrickColor::RED; break;
            case 'B': color = BrickColor::BLUE; break;
            case 'M': color = BrickColor::MAGENTA; break;
            case 'Y': color = BrickColor::YELLOW; break;
            case 'S': color = BrickColor::SILVER; break;
            case 'A': color = BrickColor::GOLD; break;
            default:
                isBrick = false;
                break;
            }
            if (isBrick)
            {
                if (color != BrickColor::GOLD) ++bricksToClear;

                Brick brick(game, color, glm::vec2(2 + 3 + 6 * col, 15 + 1 + 2 * row), level);
                brick.boundingBox = AABB(brick.center, brick.radius);
                bricks.push_back(brick);
                AABBToBrick[brick.boundingBox] = &bricks.back();
            }
        }
    }

    if (bricks.size() > MaxBricksPerLevel)
    {
        throw std::out_of_range("too many bricks");
    }
}

void Arena::draw()
{
    gl::color(color);
    for (auto& wall : walls)
    {
        gl::drawSolidRect(view.getRect(wall.topLeft, wall.botRight));
    }
    
    if (game.isActive)
    {
        for (auto& brick : bricks)
        {
            if (brick.isActive)
            {
                brick.draw();
            }
        }

        if (isBonusExitOpen)
        {
            gl::color(Color::black());
            gl::drawSolidRect(view.getRect(bonusExit.getUpperLeft(), bonusExit.getLowerRight()));
        }
    }
}
