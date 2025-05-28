#include <iostream>
#include <raylib.h>

using namespace std;

Color Purple = Color{111, 34, 179, 255};
Color Dark_Purple = Color{74, 22, 119, 255};
Color Light_Purple = Color{147, 66, 219, 155};

int player_score = 0;
int cpu_score = 0;

//create ball class which holds all of the ball's properties
class Ball{
public:
float x,y; 
int speed_x, speed_y;
int radius;

//create method which will draw the ball
void Draw() {
    DrawCircle(x,y, radius, WHITE);
}
//create method which will update the position of the ball
void Update() {
    x += speed_x;
    y += speed_y;


    // CHECK FOR COLLISIONS WITH EDGES
    // which checks if ball position is greater than or equal to screen_variable(hitting bottom) or less than or equal to 0 (top edge)
    // if so, multiply speed_variable by -1 in order to change the ball's direction
    if(y + radius >= GetScreenHeight() || y-radius <= 0)
    {
        speed_y *= -1;
    }
    //checks if ball hits right edge
    if(x + radius >= GetScreenWidth()) //cpu scores points
    {
        cpu_score++;
        resetBall();
    }
    //checks if ball hits left edge
    if(x - radius <= 0) //player scores
    {
        player_score++;
        resetBall();
    }
}
//method which will reset ball and give send it in a random direction
void resetBall()
{
    x = GetScreenWidth()/2;
    y = GetScreenHeight()/2;
    //sends ball in random direction by multiplying 
    int speeds[2] = {1,-1};
    speed_x *= speeds[GetRandomValue(0,1)];
    speed_y *= speeds[GetRandomValue(0,1)];
}
};

class Paddle {
// protected allows memmbers to not be accessed outside the class but can by inherited classes
// differs from private which does not allow any access from outside the class
protected:
//create a protected method which will prevent paddles from crossing bottom or top edges
void LimitMovement()
{
    if(y<= 0)
    {
        // if so, updates paddles y-position to 0 so it cannot go past top edge
        y = 0;
    }
    // checks if paddle has hit bottom edge
    if(y+height >= GetScreenHeight())
    {
        // if so, updates paddle's y-position = screen height - paddle's height so no parts of the paddle cross bottom edge
        y = GetScreenHeight()-height;
    }
}
public:
float x,y;
float width, height;
int speed;


void Draw(){

    //Left Paddle
    DrawRectangleRounded(Rectangle{x,y,width,height}, 0.8, 0, WHITE);
}

void Update(){
    //moves paddle up if W is pressed
    if(IsKeyDown(KEY_W))
    {
        // does this by decreasing paddle's y-position by value stored in speed position
        y = y-speed;
    }
    // moves paddle down if D is pressed
    if(IsKeyDown(KEY_S))
    {
        // does this by increasing paddle's y-position by value stored in speed position
        y = y + speed;
    }
    //call on LimitMovement method which checks if paddle has crossed boundaries
    LimitMovement();
    
}
};

//Create CPU's paddle class
// will inherit Paddle's public attributes
class cpuPaddle: public Paddle{

    public:

    void Update(int ball_y)
    {
        if (y + height/2 > ball_y)
        {
            y = y-speed;
        }

        if(y + height/2 <= ball_y)
        {
            y = y + speed;
        }
        //call on LimitMovement method which checks if paddle has crossed boundaries
        LimitMovement();
    }
};

//Initialize game ball
Ball ball;
Paddle player;
cpuPaddle cpu;
int main () {


    //declare constant variables which will be used to define window's width and height
    const int screen_width = 1280;
    const int screen_height = 800;

    // call on InitWindow which takes in width, height, and title
    InitWindow(screen_width, screen_height, "Pong!");
    //function which initializes game's framerate (if not initialized computer will run game at highest framerate possible)
    SetTargetFPS(60);
    // set ball's properties
    ball.radius = 20;
    ball.x = screen_width/2;
    ball.y = screen_height/2;
    ball.speed_x = 7;
    ball.speed_y = 7;

    //set player's paddle properties
    player.width = 20;
    player.height = 120;
    player.x = screen_width - player.width - 10;
    player.y = screen_height/2 - player.height/2;
    player.speed = 6;

    //set player's paddle properties
    cpu.width = 20;
    cpu.height = 120;
    cpu.x = 10;
    cpu.y = screen_height/2 - cpu.height/2;
    cpu.speed = 6;

    //GAME LOOP
    //run while loop while WindowShouldClose function returns false
    //WindowShouldClose will only return true if ESC key is pressed or if close icon on window is pressed
    while(WindowShouldClose() == false){
            //creates canvas to draw entities
            BeginDrawing();

            //Updating Ball's position
            ball.Update();
            //Updating Player Paddle
            player.Update();
            //update cpu's paddle
            cpu.Update(ball.y);

            //Check for ball collision with paddles
            if(CheckCollisionCircleRec(Vector2{ball.x,ball.y}, ball.radius, Rectangle{player.x, player.y, player.width, player.height}))
            {
                ball.speed_x *= -1;
            }
            if(CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{cpu.x,cpu.y,cpu.width,cpu.height}))
            {
                ball.speed_x *= -1;
            }

            //Drawing
            //Clean background of game before drawing objects
            ClearBackground(Dark_Purple);
            DrawRectangle(screen_width/2, 0,screen_width/2, screen_height, Purple);
            DrawCircle(screen_width/2, screen_height/2, 150, Light_Purple);
            // Draw line which separates both players
            DrawLine(screen_width/2, 0, screen_width/2, screen_height, WHITE);
            //Ball
            ball.Draw();
            //Left Paddle
            cpu.Draw();
            //Right Paddle
            player.Draw();

            //draw scoreboard
            DrawText(TextFormat("%i", cpu_score), screen_width/4-20, 20, 80, WHITE);
            DrawText(TextFormat("%i", player_score), 3 * screen_width/4, 20, 80, WHITE);

            EndDrawing();
    }

    CloseWindow();

    return 0;
}