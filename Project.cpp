#include <iostream>
#include "MacUILib.h"
#include "objPos.h"
#include "Player.h"
#include <conio.h>
#include <stdlib.h>
#include <ctime>

using namespace std;

#define DELAY_CONST 10000
#define MAX_SCORE 2

// Global Objects
Player *playerPtr = nullptr;
GameMechs *gameMech = nullptr;

// Global Variables
int x;
int y;
int symb;
char input;

// Iterator Variables
int i;
int j;

int HEIGHT;
int WIDTH;
int OBJ_SIZE;

void Initialize(void);
void GetInput(void);
void RunLogic(void);
void DrawScreen(void);
void LoopDelay(void);
void CleanUp(void);

int main(void)
{

    Initialize();

    while (gameMech->getExitFlagStatus() == false)
    {
        GetInput();
        RunLogic();
        DrawScreen();
        LoopDelay();
    }

    CleanUp();
}

void Initialize(void)
{
    MacUILib_init();
    MacUILib_clearScreen();

    srand(time(NULL));

    // Allocating Heap Memory
    gameMech = new GameMechs(); // Game Mechanics Object
    playerPtr = new Player(gameMech);   // Player Object

    // Initialising Global Variables
    HEIGHT = gameMech->getBoardSizeX(); // Get Board Height
    WIDTH = gameMech->getBoardSizeY();   // Get Board Width
    symb = playerPtr->getPlayerPos()->getHeadElement().getSymbol();  // Get Player Symbol

    OBJ_SIZE = playerPtr->getPlayerPos()->getSize(); // Get Player Size

}

void GetInput(void)
{
    if (MacUILib_hasChar())
        gameMech->setInput(_getch()); // Get Input
        //Check for debug key 'r' that regenerates food
    else
        gameMech->clearInput(); // Clear input if no input is given
}

void RunLogic(void)
{
    input = gameMech->getInput();

    if (input == 32)
        gameMech->setExitTrue();

    playerPtr->updatePlayerDir();      // Update player direction
    playerPtr->movePlayer();    // Move player based on direction

    OBJ_SIZE = playerPtr->getPlayerPos()->getSize(); // Update Player Size

    if (gameMech->getScore() == MAX_SCORE)
    {
        gameMech->setLoseFlag();
        gameMech->setExitTrue();
    }

}

void DrawScreen(void)
{
    int food_x = gameMech->getFoodPos().getX();
    int food_y = gameMech->getFoodPos().getY();
    MacUILib_clearScreen();

    // Comment out if too much lag in printing
    printf(" __      ____                      __                  ____                                     __ \n/\\ \\    /\\  _`\\                   /\\ \\                /\\  _`\\                                  /\\ \\ \n\\ \\ \\   \\ \\,\\L\\_\\    ___      __  \\ \\ \\/'\\      __    \\ \\ \\L\\_\\     __      ___ ___      __    \\ \\ \\ \n \\ \\ \\   \\/_\\__ \\  /' _ `\\  /'__`\\ \\ \\ , <    /'__`\\   \\ \\ \\L_L   /'__`\\  /' __` __`\\  /'__`\\   \\ \\ \\ \n  \\ \\ \\    /\\ \\L\\ \\/\\ \\/\\ \\/\\ \\L\\.\\_\\ \\ \\\\`\\ /\\  __/    \\ \\ \\/, \\/\\ \\L\\.\\_/\\ \\/\\ \\/\\ \\/\\  __/    \\ \\ \\ \n   \\ \\ \\   \\ `\\____\\ \\_\\ \\_\\ \\__/.\\_\\\\ \\_\\ \\_\\ \\____\\    \\ \\____/\\ \\__/.\\_\\ \\_\\ \\_\\ \\_\\ \\____\\    \\ \\ \\ \n    \\ \\ \\   \\/_____/\\/_/\\/_/\\/__/\\/_/ \\/_/\\/_/\\/____/     \\/___/  \\/__/\\/_/\\/_/\\/_/\\/_/\\/____/     \\ \\ \\ \n     \\ \\_\\                                                                                          \\ \\_\\ \n      \\/_/                                                                                           \\/_/  \n");

    for (i = 0; i < HEIGHT; i++)
    {
        printf("\t\t\t\t\t");
        for (j = 0; j < WIDTH; j++)
        {
            if (i == 0 || i == HEIGHT - 1 || j == 0 || j == WIDTH - 1)
            {
                printf("+");
            }
            else if (i == food_x && j == food_y)
            {
                printf("@");
            }
            else
            {
                int k;
                for (k = 0; k < OBJ_SIZE; k++)
                {
                    if (i == playerPtr->getPlayerPos()->getElement(k).getX() && j == playerPtr->getPlayerPos()->getElement(k).getY())
                    {
                        printf("%c", 'X');
                        break;
                    }
                }
                if (k == OBJ_SIZE)
                {
                    printf(" ");
                }
            }
        }
        printf("\n");
    }

    printf("Input Character: %c\n", input);
    printf("Score: %d\n", gameMech->getScore());
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
        printf("You Lose!\n");
    else
        printf("You Win!\n");
    
    // De-allocate Heap Memory
    delete playerPtr;
    delete gameMech;

    MacUILib_uninit();
}
