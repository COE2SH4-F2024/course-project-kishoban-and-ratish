#include <iostream>
#include <windows.h>
#include "MacUILib.h"
#include "objPos.h"
#include "Player.h"
#include <conio.h>
#include <stdlib.h>
#include <ctime>

using namespace std;


#define MAX_SCORE 20
#define INIT_SIZE 5

// Global Objects
Player *playerPtr = nullptr;
GameMechs *gameMech = nullptr;
Food *foodPtr = nullptr;

// Global Variables
int DELAY_CONST = 100000;

int x;
int y;
wchar_t symb;
char input;
string buffer;

// Iterator Variables
int i;
int j;

// Constant Variables
int HEIGHT;
int WIDTH;
int OBJ_SIZE;
string Missing_Row = "";
const string BORDER_SYMBOL = u8"█░█";
const string SPACE_CHAR = "   ";
string HEAD_SYMBOL;
string BODY_SYMBOL;

// Function Prototypes
void Initialize(void);
void GetInput(void);
void RunLogic(void);
void DrawScreen(void);
void LoopDelay(void);
void CleanUp(void);

int main(void)
{

    SetConsoleOutputCP(CP_UTF8);

    // Hide Cursor
    printf("\e[?25l");

    Initialize();

    while (gameMech->getExitFlagStatus() == false)
    {
        GetInput();
        RunLogic();
        DrawScreen();
        LoopDelay();
    }

    CleanUp();
    return 0;
}

void Initialize(void)
{
    MacUILib_init();
    MacUILib_clearScreen();

    srand(time(NULL));

    // Allocating Heap Memory
    gameMech = new GameMechs(10, 20); // Game Mechanics Object
    playerPtr = new Player(gameMech, 7, 6, INIT_SIZE);   // Player Object

    // Initialising Global Variables
    HEIGHT = gameMech->getBoardSizeX();                     // Get Board Height
    WIDTH = gameMech->getBoardSizeY();                     // Get Board Width
    HEAD_SYMBOL = "🐍";             // Get Head Symbol
    BODY_SYMBOL = " 🟫";      // Set Body Symbol
    OBJ_SIZE = playerPtr->getPlayerPos()->getSize(); // Get Player Size

    buffer += u8"===============================\n\t SNAKE GAME \n===============================\n\n";

    gameMech->generateFood(playerPtr->getPlayerPos()); // Generate Food
    // Generate Missing Row
    for (i = 0; i < WIDTH; i++)    
        (i == 0 || i == WIDTH - 1) ? Missing_Row += BORDER_SYMBOL : Missing_Row += SPACE_CHAR;

}

void GetInput(void)
{
    if (_kbhit())
        gameMech->setInput(_getch()); // Get Input
    else
        gameMech->clearInput(); // Clear input if no input is given
}

void RunLogic(void)
{
    // Get Input
    input = gameMech->getInput();

    // Check Exit Condition
    if (input == 32)
    {
        gameMech->setExitTrue();
        return ;
    }

    // Update Player Direction
    playerPtr->updatePlayerDir();      // Update player direction
    playerPtr->movePlayer();    // Move player based on direction
    OBJ_SIZE = playerPtr->getPlayerPos()->getSize(); // Update Player Size



    // Check Self Collision
    if (playerPtr->checkSelfCollision())
    {
        gameMech->setExitTrue(); 
        return ;
    }

    // Check Win/Lose Status
    if (gameMech->getScore() >= MAX_SCORE)
    {
        gameMech->setLoseFlag();
        gameMech->setExitTrue();
        return ;
    }

    gameMech->checkFoodConsumption(playerPtr);
}

void DrawScreen(void)
{
    // Check Exit status
    if (gameMech->getExitFlagStatus())
        return ;

    int food_x = foodPtr->getFoodPos(0).getX();
    int food_y = foodPtr->getFoodPos(0).getY();
    MacUILib_clearScreen();

    buffer.clear(); // Clear buffer at the start

    buffer += u8"===============================\n\t SNAKE GAME \n===============================\n\n";
    objPosArrayList* foodBucket = foodPtr->getFoodBucket();
    int k;
    for (i = 0; i < HEIGHT; i++)
    {
        for (j = 0; j < WIDTH; j++)
        {
            if (i == 0 || i == HEIGHT - 1 || j == 0 || j == WIDTH - 1)
            {
                buffer += BORDER_SYMBOL;
            }
            // else if (i == food_x && j == food_y)
            // {
            //     buffer += u8" 🥚";
            // }
            else
            {
                bool foodDrawn = false;
                for (k = 0; k < foodBucket->getSize(); k++)
                {
                    objPos food = foodBucket->getElement(k);
                    if (i == food.getX() && j == food.getY())
                    {
                        buffer += (food.getSymbol() == '@')? u8" 🥚" : u8" 🍒";
                        foodDrawn = true;
                        break; 
                    }
                }
                if (!foodDrawn)
                {
                    for (k = 0; k < OBJ_SIZE; k++)
                    {
                        if (i == playerPtr->getPlayerPos()->getElement(k).getX() && j == playerPtr->getPlayerPos()->getElement(k).getY())
                        {
                            if (k == 0)
                            {
                                buffer += " ";
                                // buffer += ;
                                buffer += HEAD_SYMBOL;
                            }
                            else
                            {
                                buffer += BODY_SYMBOL;
                            }
                            break;
                        }
                    }
                }
                if (k == OBJ_SIZE)
                {
                    buffer += SPACE_CHAR;
                }
            }
        }
        (i < HEIGHT - 1) ? buffer += "\n" + Missing_Row + "\n" : buffer += "\n\n";
    }

    printf("%s", buffer.c_str());
}

void LoopDelay(void)
{
    MacUILib_Delay(DELAY_CONST); // 0.1s delay
}

void CleanUp(void)
{
    MacUILib_clearScreen();


    // Display Win/Lose Message
    if (gameMech->getLoseFlagStatus()) 
        printf("                                                                       .-')      ('-.          \n                                                                      ( OO ).  _(  OO)             \n    ,--.   ,--..-'),-----.  ,--. ,--.          ,--.      .-'),-----. (_)---\\_)(,------.       \n     \\  `.'  /( OO'  .-.  ' |  | |  |          |  |.-') ( OO'  .-.  '/    _ |  |  .---'      \n   .-')     / /   |  | |  | |  | | .-')        |  | OO )/   |  | |  |\\  :` `.  |  |             \n  (OO  \\   /  \\_) |  |\\|  | |  |_|( OO )       |  |`-' |\\_) |  |\\|  | '..`''.)(|  '--.        \n   |   /  /\\_   \\ |  | |  | |  | | `-' /      (|  '---.'  \\ |  | |  |.-._)   \\ |  .--'      \n   `-./  /.__)   `'  '-'  '('  '-'(_.-'        |      |    `'  '-'  '\\       / |  `---.        \n     `--'          `-----'   `-----'           `------'      `-----'  `-----'  `------'        \n\n");
    else
        printf("\n     ____     __   ,-----.      ___    _         .--.      .--..-./`) ,---.   .--.          _ _  .--.     \n     \\   \\   /  /.'  .-,  '.  .'   |  | |        |  |_     |  |\\ .-.')|    \\  |  |         ( ` ) `-. \\    \n      \\  _. /  '/ ,-.|  \\ _ \\ |   .'  | |        | _( )_   |  |/ `-' \\|  ,  \\ |  |        (_ o _)   \\_\\   \n       _( )_ .';  \\  '_ /  | :.'  '_  | |        |(_ o _)  |  | `-'`\"`|  |\\_ \\|  |         (_,_)   _( )_  \n   ___(_ o _)' |  _`,/ \\ _/  |'   ( \\.-.|        | (_,_) \\ |  | .---. |  _( )_\\  |                (_ o _) \n  |   |(_,_)'  : (  '\\_/ \\   ;' (`. _` /|        |  |/    \\|  | |   | | (_ o _)  |           _     (_,_) \n  |   `-'  /    \\ `\"/  \\  ) / | (_ (_) _)        |  '  /\\  `  | |   | |  (_,_)\\  |         _( )_    / /   \n   \\      /      '. \\_/``\".'   \\ /  . \\ /        |    /  \\    | |   | |  |    |  |        (_ o _).-' /    \n    `-..-'         '-----'      ``-'`-''         `---'    `---` '---' '--'    '--'         (_,_) `--'     \n                                                                                                          \n");
    
    // De-allocate Heap Memory
    delete playerPtr;
    delete gameMech;
    delete foodPtr;

    MacUILib_uninit();
}
