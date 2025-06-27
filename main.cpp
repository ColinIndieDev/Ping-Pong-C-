#include <cmath>
#include <iostream>

#include "raylib.h"

#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 800

class Player
{
    public:
    int width = 30;
    int height = 180;

    float offset = 8;
    float posX = 0 + offset;
    float posY = SCREEN_HEIGHT / 2;
    float speed = 1000;

    Color color = {255, 0, 0, 255};

    void Draw() const
    {
        Rectangle rectangle = Rectangle{posX, posY, (float)width, (float)height};
        DrawRectangleRounded(rectangle, 2, 1, color);
    }

    void Update()
    {
        if (IsKeyDown(KEY_W))
        {
            posY -= speed * GetFrameTime();
        }
        else if (IsKeyDown(KEY_S))
        {
            posY += speed * GetFrameTime();
        }

        if (posY < 0)
        {
            posY = 0;
        }
        else if (posY + height > SCREEN_HEIGHT)
        {
            posY = SCREEN_HEIGHT - height;
        }
    }

};

class Player2
{
public:
    int width = 30;
    int height = 180;

    float offset = 8;
    float posX = SCREEN_WIDTH - width - offset;
    float posY = SCREEN_HEIGHT / 2;
    float speed = 1000;

    Color color = {0, 0, 255, 255};

    void Draw() const
    {
        Rectangle rectangle = Rectangle{posX, posY, (float)width, (float)height};
        DrawRectangleRounded(rectangle, 2, 1, color);
    }

    void Update()
    {
        if (IsKeyDown(KEY_UP))
        {
            posY -= speed * GetFrameTime();
        }
        else if (IsKeyDown(KEY_DOWN))
        {
            posY += speed * GetFrameTime();
        }

        if (posY < 0)
        {
            posY = 0;
        }
        else if (posY + height > SCREEN_HEIGHT)
        {
            posY = SCREEN_HEIGHT - height;
        }
    }

};

class Ball
{
    public:
    float radius = 20;
    float speedX = 500, speedY = speedX;

    float posX = SCREEN_WIDTH / 2;
    float posY = SCREEN_HEIGHT / 2;

    bool isRestarting = true;

    void Draw() const
    {
        DrawCircle((int)posX, (int)posY, radius, WHITE);
    }

    void Update()
    {
        posX += speedX * GetFrameTime();
        posY += speedY * GetFrameTime();

        if (isRestarting)
        {
            float factor = 0.025f * GetFrameTime();
            float additionX = speedX < 0 ? speedX : -speedX;
            float additionY = speedY < 0 ? speedY : -speedY;

            speedX = speedX + speedX * factor;
            speedY = speedY + speedY * factor;
        }
    }

    void Reset()
    {
        isRestarting = false;
        speedX = speedY = 500;

        int randomDirX = GetRandomValue(0, 1) == 0 ? -1 : 1;
        int randomDirY = GetRandomValue(0, 1) == 0 ? -1 : 1;

        posX = SCREEN_WIDTH / 2;
        posY = SCREEN_HEIGHT / 2;
        speedX *= randomDirX, speedY *= randomDirY;

        isRestarting = true;
    }

    void checkCollisionWithEnd()
    {
        if (posX >= SCREEN_WIDTH - radius)
        {
            posX = SCREEN_WIDTH - radius;
            speedX *= -1;
        }
        else if (posX <= 0 - radius)
        {
            posX = radius;
            speedX *= -1;
        }

        if (posY >= SCREEN_HEIGHT - radius)
        {
            posY = SCREEN_HEIGHT - radius;
            speedY *= -1;
        }
        else if (posY <= 0 + radius)
        {
            posY = radius;
            speedY *= -1;
        }
    }
};

class Decoration
{
    public:
    static void Draw()
    {
        DrawLine((float)SCREEN_WIDTH / 2, 0, (float)SCREEN_WIDTH / 2, (float)SCREEN_HEIGHT, WHITE);
        DrawCircleLines(0, (float)SCREEN_HEIGHT / 2, 200, WHITE);
        DrawCircleLines((float)SCREEN_WIDTH, (float)SCREEN_HEIGHT / 2, 200, WHITE);
    }
};

int scorePlayer1 = 0;
int scorePlayer2 = 0;

int main()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Ping Pong");

    // Start
    Player player = Player();
    Player2 player2 = Player2();
    Ball ball = Ball();
    Decoration decoration = Decoration();

    while (!WindowShouldClose())
    {
        // Loop
        player.Update();
        player2.Update();
        ball.Update();
        player.speed = abs(ball.speedX);
        player2.speed = abs(ball.speedX);
        ball.checkCollisionWithEnd();
        if (IsKeyPressed(KEY_ESCAPE)) WindowShouldClose();

        if (ball.posX <= player.offset + (float)player.width / 2)
        {
            scorePlayer2++;
            ball.Reset();
        }
        else if (ball.posX >= SCREEN_WIDTH - (player2.offset + (float)player2.width / 2))
        {
            scorePlayer1++;
            ball.Reset();
        }

        // Collisions
        if (CheckCollisionCircleLine(Vector2{ball.posX, ball.posY}, 10, Vector2{player.posX + player.width, player.posY}, Vector2{player.posX + player.width, player.posY + player.height}))
        {
            ball.speedX *= -1;
        }

        if (CheckCollisionCircleLine(Vector2{ball.posX, ball.posY}, 10, Vector2{(float)SCREEN_WIDTH - player2.width, player2.posY}, Vector2{(float)SCREEN_WIDTH - player2.width, player2.posY + player2.height}))
        {
            ball.speedX *= -1;
        }

        // Drawing
        BeginDrawing();
        decoration.Draw();
        player.Draw();
        player2.Draw();
        ball.Draw();

        // Texts
        DrawText(TextFormat("FPS: %i", GetFPS()), SCREEN_WIDTH - 125, SCREEN_HEIGHT - 25, 20, WHITE);
        DrawText(TextFormat("%i : %i", scorePlayer1, scorePlayer2), SCREEN_WIDTH / 2 - 60, 40, 60, WHITE);
        DrawText("Player 1", 150, 20, 40, Color{255, 0, 0, 255});
        DrawText("Player 2", SCREEN_WIDTH - 400, 20, 40, Color{0, 0, 255, 255});
        ClearBackground(GREEN);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}