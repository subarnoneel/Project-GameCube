#include <SDL.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL_timer.h>
#include <math.h>
#include <SDL_mixer.h>
#include <SDL_image.h>

#define SCREEN_WIDTH 950
#define SCREEN_HEIGHT 644
#define GRID_SIZE 64
#define NUM_ROWS 10
#define NUM_COLS 10
#define VISIBLE_WIDTH 642
#define DICE_SIZE 80 // Size of the dice image
#define SHUFFLE_DURATION 1000 // 8 seconds in milliseconds
#define DICE_GAP 45 // Gap between the dice
#define SCORE_WIDTH 80 // Size of scoreboard
#define SCORE_HEIGHT 100
#define RIGHT_MARGIN (SCREEN_WIDTH - VISIBLE_WIDTH) // Width of the right margin
#define LOC_DELAY_MS 1000

    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    Mix_Music* background_music=NULL;
    Mix_Music* game=NULL;
    Mix_Chunk* ladder=NULL;
    Mix_Chunk* click_effect=NULL;
    Mix_Chunk* snake=NULL;
    Mix_Chunk* dice=NULL;
    Mix_Chunk* board_change = NULL;
    Mix_Chunk* applause=NULL;
    Mix_Chunk* ability=NULL;
    SDL_Surface* surface = NULL;


    SDL_Texture* block1texture = NULL;
    SDL_Texture* block2texture = NULL;
    SDL_Texture* block3texture = NULL;
    SDL_Texture* block4texture = NULL;



bool initSDL();
void closeSDL();
int generateRandomNumber();
//void drawPiece(SDL_Renderer *renderer, int pieceX, int pieceY);
void movePiece(SDL_Event *event, int *pieceX, int *pieceY);
SDL_Texture* loadScoreboardTexture(SDL_Renderer *renderer, const char* path);
SDL_Texture* loadWinTexture(SDL_Renderer *renderer, const char* path);
void renderScoreboard(SDL_Renderer *renderer, SDL_Texture *scoreboardTexture, int scoreX, int scoreY);
void renderWin(SDL_Renderer *renderer, SDL_Texture *WinTexture, int scoreX, int scoreY);
void setLocation_0(int value, int* pieceX_0, int* pieceY_0, int* pieceX_1, int* pieceY_1);
void setLocation_1(int value, int* pieceX_0, int* pieceY_0, int* pieceX_1, int* pieceY_1);
void setLocation_2(int value, int* pieceX_0, int* pieceY_0, int* pieceX_1, int* pieceY_1);
void setLocation_3(int value, int* pieceX_0, int* pieceY_0, int* pieceX_1, int* pieceY_1);
void setLocation_4(int value, int* pieceX_0, int* pieceY_0, int* pieceX_1, int* pieceY_1);
void setLocation_5(int value, int* pieceX_0, int* pieceY_0, int* pieceX_1, int* pieceY_1);

void updateScoreboardTexture(SDL_Renderer *renderer, SDL_Texture **scoreboardTexture, int *score);
void drawImageOverGrid(SDL_Renderer *renderer, int board_number, int gridWidth, int gridHeight);
void renderMenu(SDL_Renderer *renderer, const char *imagePath);
void handleMenuEvents(SDL_Renderer *renderer);
void drawPiece(SDL_Renderer *renderer, int pieceX, int pieceY);
void drawPiece2(SDL_Renderer *renderer, int pieceX, int pieceY);
void drawPiece3(SDL_Renderer *renderer, int pieceX, int pieceY);
void drawPiece4(SDL_Renderer *renderer, int pieceX, int pieceY);

// starting location of the piece
int loc = 1, loc2 = 1, loc3 = 1, loc4=1, delay = 0;
bool initShuffle = false;
// 2D array to store the indexes for each box in the grid
int gridIndexes[NUM_ROWS][NUM_COLS];
// initial score
int score = 0;
// board number
int board_number = 1;
int p1 = 0;
int p2=0;
int p3=0;
int p4=0;

int player=2;

int ability_1=0,ability_2=0,ability_3=0,ability_4=0;

int pieceX_0, pieceY_0, pieceX_1, pieceY_1 ;

    pieceX_0 = 7;
    pieceY_0 = 560;
    pieceX_1 = 7;
    pieceY_1 = 560;

int pieceX_2, pieceY_2, pieceX_22, pieceY_22 ;

    pieceX_2 = 6;
    pieceY_2 = 600;
    pieceX_22 = 6;
    pieceY_22 = 600;

int pieceX_3, pieceY_3, pieceX_33, pieceY_33 ;

    pieceX_3 = -24;
    pieceY_3 = 560;
    pieceX_33 = -24;
    pieceY_33 = 560;

int pieceX_4, pieceY_4, pieceX_44, pieceY_44 ;

    pieceX_4 = -23;
    pieceY_4 = 600;
    pieceX_44 = -23;
    pieceY_44 = 600;



int main(int argc, char *args[]) {
    if (!initSDL()) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    // SDL window and renderer setup
    window = SDL_CreateWindow("GAME_CUBE", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        closeSDL();
        return 1;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        closeSDL();
        return 1;
    }


    Mix_Init(MIX_INIT_MP3);
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 1, 2048);



    game=Mix_LoadMUS("ingame.mp3");
    background_music = Mix_LoadMUS("menu_audio.mp3");
    ladder=Mix_LoadWAV("Ladder.wav");
    snake=Mix_LoadWAV("snake.wav");
    click_effect=Mix_LoadWAV("click_effect.wav");
    dice=Mix_LoadWAV("dice_roll.wav");
    board_change=Mix_LoadWAV("board_change.wav");
    applause=Mix_LoadWAV("applause.wav");
    ability=Mix_LoadWAV("ability.wav");

    Mix_AllocateChannels(3);


    // Show initial menu
    renderMenu(renderer, "menu_1.bmp");

    // Handle menu events
    Mix_PlayMusic(background_music,-1);

    handleMenuEvents(renderer);


    Mix_PlayMusic(game,-1);


    // Load winning screen

    SDL_Texture *WinTexture1 = loadWinTexture(renderer, "end_1.bmp");
    SDL_Texture *WinTexture2 = loadWinTexture(renderer, "end_2.bmp");
    SDL_Texture *WinTexture3 = loadWinTexture(renderer, "end_3.bmp");
    SDL_Texture *WinTexture4 = loadWinTexture(renderer, "end_4.bmp");

    if (WinTexture1 == NULL ) {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        closeSDL();
        return 1;
    }



    surface = IMG_Load("piece_1.png");
    block1texture = SDL_CreateTextureFromSurface(renderer, surface);
    surface = IMG_Load("piece_2.png");
    block2texture = SDL_CreateTextureFromSurface(renderer, surface);
    surface = IMG_Load("piece_3.png");
    block3texture = SDL_CreateTextureFromSurface(renderer, surface);
    surface = IMG_Load("piece_4.png");
    block4texture = SDL_CreateTextureFromSurface(renderer, surface);

    // Load scoreboard texture
    char filename[20];
    sprintf(filename, "digit_%d.bmp", score);
    SDL_Texture *scoreboardTexture = loadScoreboardTexture(renderer, filename);
    if (scoreboardTexture == NULL) {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        closeSDL();
        return 1;
    }

    int scoreX = 755; // Initial X-coordinate for the scoreboard
    int scoreY = 350;  // Initial Y-coordinate for the scoreboard

    // Load dice images
    SDL_Surface *diceSurfaces[6];
    SDL_Texture *diceTextures[6];
    for (int i = 0; i < 6; ++i) {
        char filename[20];
        sprintf(filename, "dice_%d.bmp", i + 1);
        diceSurfaces[i] = SDL_LoadBMP(filename);
        if (diceSurfaces[i] == NULL) {
            printf("Unable to load dice image %d! SDL_Error: %s\n", i + 1, SDL_GetError());
            for (int j = 0; j < i; ++j) {
                SDL_FreeSurface(diceSurfaces[j]);
                SDL_DestroyTexture(diceTextures[j]);
            }
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
            closeSDL();
            return 1;
        }
        diceTextures[i] = SDL_CreateTextureFromSurface(renderer, diceSurfaces[i]);
        if (diceTextures[i] == NULL) {
            printf("Unable to create texture from dice image %d! SDL_Error: %s\n", i + 1, SDL_GetError());
            for (int j = 0; j < i; ++j) {
                SDL_FreeSurface(diceSurfaces[j]);
                SDL_DestroyTexture(diceTextures[j]);
            }
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
            closeSDL();
            return 1;
        }
    }

    bool quit = false;
    bool shuffle1 = false; // Indicates if shuffling for dice 1 is active
    bool shuffle2 = false; // Indicates if shuffling for dice 2 is active
    SDL_Event e;
    Uint32 startTime1 = 0; // Starting time of the shuffle for dice 1
    Uint32 startTime2 = 0; // Starting time of the shuffle for dice 2
    int currentFrame1 = 3;
    int currentFrame2 = 0;

    Uint32 previousTime = SDL_GetTicks(); // Record the previous time
    bool spacePressed = false; // Track if space bar is pressed

    //int pieceX = 8, pieceY = 583;


    /*int pieceX2 = 8; // Initial X-coordinate of the piece
    int pieceY2 = 572; // Initial Y-coordinate of the piece

    int pieceX3 = 14; // Initial X-coordinate of the piece
    int pieceY3 = 588; // Initial Y-coordinate of the piece
*/


    while (!quit) {
        Uint32 currentTime = SDL_GetTicks(); // Get the current time
        Uint32 elapsedTime = currentTime - previousTime; // Calculate elapsed time

        if (elapsedTime < 200) { // Limit frame rate to approximately 20 frames per second (1000ms / 50ms = 20fps)
            SDL_Delay(200 - elapsedTime); // Delay if necessary to maintain frame rate
        }

        previousTime = currentTime; // Update the previous time

        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            } else if (e.type == SDL_KEYDOWN) {
                Mix_PlayChannel(-1,dice,0);
                if (e.key.keysym.sym == SDLK_q) {
                        p1=1;
                        p2=0;
                        p3=0;
                        p4=0;
                    // Toggle shuffling state for dice 1
                    shuffle1 = !shuffle1;
                    if (shuffle1) {
                        // Restart animation for dice 1
                        startTime1 = SDL_GetTicks();
                        spacePressed = true;
                    } else {
                        spacePressed = false;
                    }

                    // Toggle shuffling state for dice 2
                    shuffle2 = !shuffle2;
                    if (shuffle2) {
                        // Restart animation for dice 2
                        startTime2 = SDL_GetTicks();
                    }
                }


                if (e.key.keysym.sym == SDLK_c) {
                        p1=0;
                        p2=1;
                        p3=0;
                        p4=0;
                    // Toggle shuffling state for dice 1
                    shuffle1 = !shuffle1;
                    if (shuffle1) {
                        // Restart animation for dice 1
                        startTime1 = SDL_GetTicks();
                        spacePressed = true;
                    } else {
                        spacePressed = false;
                    }

                    // Toggle shuffling state for dice 2
                    shuffle2 = !shuffle2;
                    if (shuffle2) {
                        // Restart animation for dice 2
                        startTime2 = SDL_GetTicks();
                    }
                }


                if (e.key.keysym.sym == SDLK_n) {
                        p1=0;
                        p2=0;
                        p3=1;
                        p4=0;
                    // Toggle shuffling state for dice 1
                    shuffle1 = !shuffle1;
                    if (shuffle1) {
                        // Restart animation for dice 1
                        startTime1 = SDL_GetTicks();
                        spacePressed = true;
                    } else {
                        spacePressed = false;
                    }

                    // Toggle shuffling state for dice 2
                    shuffle2 = !shuffle2;
                    if (shuffle2) {
                        // Restart animation for dice 2
                        startTime2 = SDL_GetTicks();
                    }
                }
                if (e.key.keysym.sym == SDLK_p) {
                        p1=0;
                        p2=0;
                        p3=0;
                        p4=1;
                    // Toggle shuffling state for dice 1
                    shuffle1 = !shuffle1;
                    if (shuffle1) {
                        // Restart animation for dice 1
                        startTime1 = SDL_GetTicks();
                        spacePressed = true;
                    } else {
                        spacePressed = false;
                    }

                    // Toggle shuffling state for dice 2
                    shuffle2 = !shuffle2;
                    if (shuffle2) {
                        // Restart animation for dice 2
                        startTime2 = SDL_GetTicks();
                    }
                }

                //Ability_Keys
                if (e.key.keysym.sym == SDLK_d) {
                    Mix_PlayChannel(-1,ability,0);
                    ability_1 = 1;
                    printf("Ability 1 activated!\n");
                }
                if (e.key.keysym.sym == SDLK_r) {
                    Mix_PlayChannel(-1,ability,0);
                    ability_2 = 1;
                    printf("Ability 2 activated!\n");
                }
                if (e.key.keysym.sym == SDLK_g) {
                    Mix_PlayChannel(-1,ability,0);
                    ability_3 = 1;
                    printf("Ability 3 activated!\n");
                }
                if (e.key.keysym.sym == SDLK_y) {
                    Mix_PlayChannel(-1,ability,0);
                    ability_4 = 1;
                    printf("Ability 4 activated!\n");
                }
       }
}

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Background color: black
        SDL_RenderClear(renderer);



        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Grid color: red

        // Draw the grid within the visible width
        for (int i = 0; i <= NUM_ROWS; ++i) {
            SDL_RenderDrawLine(renderer, 0, i * GRID_SIZE, VISIBLE_WIDTH, i * GRID_SIZE);
        }
        for (int i = 0; i <= NUM_COLS; ++i) {
            SDL_RenderDrawLine(renderer, i * GRID_SIZE, 0, i * GRID_SIZE, SCREEN_HEIGHT);
        }

        // Draw the image over the grid
        drawImageOverGrid(renderer, board_number, VISIBLE_WIDTH, SCREEN_HEIGHT);

        // Update the piece's location
        while((pieceX_0 <= pieceX_1)){
            drawPiece(renderer, pieceX_0, pieceY_0);

            pieceX_0 += 64;
        }
        while((pieceX_2 <= pieceX_22)){
            drawPiece2(renderer, pieceX_2, pieceY_2);
            pieceX_2 += 64;

        }
        if(player>=3){
            while(pieceX_3 <= pieceX_33){
                drawPiece3(renderer, pieceX_3, pieceY_3);
                pieceX_3 += 64;
            }
        }
        if(player>=4){
        while(pieceX_4 <= pieceX_44){
            drawPiece4(renderer, pieceX_4, pieceY_4);
            pieceX_4 += 64;
        }
        }


              // Draw pieces
        drawPiece(renderer, pieceX_1, pieceY_1);
            //printf("%d %d\n", pieceX_0, pieceX_1);
        if(player>=2)
            drawPiece2(renderer, pieceX_22, pieceY_22);
        if(player>=3)
            drawPiece3(renderer, pieceX_33, pieceY_33);
        if(player>=4)
            drawPiece4(renderer, pieceX_44, pieceY_44);




        // Render dice images if shuffling is active
        if (shuffle1) {
            initShuffle = true;
            // Check if SHUFFLE_DURATION time has elapsed for dice 1
            Uint32 currentTime = SDL_GetTicks();
            if (currentTime - startTime1 >= SHUFFLE_DURATION) {
                shuffle1 = false; // Stop shuffling for dice 1 after duration
            } else {
                // Render dice 1 image
                int diceX1 = 710; // X-coordinate for Dice 1
                int diceY1 = 520; // Y-coordinate for Dice 1
                SDL_Rect diceRect1 = { diceX1, diceY1, DICE_SIZE, DICE_SIZE };
                SDL_RenderCopy(renderer, diceTextures[currentFrame1 % 6], NULL, &diceRect1);

                // Increment current frame for dice 1
                currentFrame1 += rand() % 3;
            }
        } else {
            // Render the dice 1 image frozen at the current frame
            int diceX1 = 710; // X-coordinate for Dice 1
            int diceY1 = 520; // Y-coordinate for Dice 1
            SDL_Rect diceRect1 = { diceX1, diceY1, DICE_SIZE, DICE_SIZE };
            SDL_RenderCopy(renderer, diceTextures[currentFrame1 % 6], NULL, &diceRect1);
        }

        if (shuffle2) {
            initShuffle = true;
            // Check if SHUFFLE_DURATION time has elapsed for dice 2
            Uint32 currentTime = SDL_GetTicks();
            if (currentTime - startTime2 >= SHUFFLE_DURATION) {
                shuffle2 = false; // Stop shuffling for dice 2 after duration
            } else {
                // Render dice 2 image
                int diceX2 = 800; // X-coordinate for Dice 2
                int diceY2 = 520; // Y-coordinate for Dice 2
                SDL_Rect diceRect2 = { diceX2, diceY2, DICE_SIZE, DICE_SIZE };
                SDL_RenderCopy(renderer, diceTextures[currentFrame2 % 6], NULL, &diceRect2);

                // Increment current frame for dice 2
                currentFrame2 += rand() % 3;
            }
        } else {
            // Render the dice 2 image frozen at the current frame
            int diceX2 = 800; // X-coordinate for Dice 2
            int diceY2 = 520; // Y-coordinate for Dice 2
            SDL_Rect diceRect2 = { diceX2, diceY2, DICE_SIZE, DICE_SIZE };
            SDL_RenderCopy(renderer, diceTextures[currentFrame2 % 6], NULL, &diceRect2);
        }

//Logic to check after the board changes

        if (initShuffle && !shuffle1 && !shuffle2 && p4) {
            initShuffle = false;
            if((currentFrame1 % 6) == (currentFrame2 % 6)){
                 board_number= currentFrame1 % 6;
                 Mix_PlayChannel(-1,board_change,0);
            }

            score = abs((currentFrame1 % 6) - (currentFrame2 % 6));

            updateScoreboardTexture(renderer, &scoreboardTexture, &score);

            if(board_number==0){
                setLocationp4_0(abs((currentFrame1 % 6) - (currentFrame2 % 6)), &pieceX_4, &pieceY_4,  &pieceX_44, &pieceY_44);
            }else if(board_number==1){
                setLocationp4_1(abs((currentFrame1 % 6) - (currentFrame2 % 6)), &pieceX_4, &pieceY_4,  &pieceX_44, &pieceY_44);
            }else if(board_number==2){
                setLocationp4_2(abs((currentFrame1 % 6) - (currentFrame2 % 6)), &pieceX_4, &pieceY_4,  &pieceX_44, &pieceY_44);
            }else if(board_number==3){
                setLocationp4_3(abs((currentFrame1 % 6) - (currentFrame2 % 6)), &pieceX_4, &pieceY_4,  &pieceX_44, &pieceY_44);
            }else if(board_number==4){
                setLocationp4_4(abs((currentFrame1 % 6) - (currentFrame2 % 6)), &pieceX_4, &pieceY_4,  &pieceX_44, &pieceY_44);
            }
            else if(board_number==5){
                setLocationp4_5(abs((currentFrame1 % 6) - (currentFrame2 % 6)), &pieceX_4, &pieceY_4,  &pieceX_44, &pieceY_44);
            }

        }




        if (initShuffle && !shuffle1 && !shuffle2 && p1) {
            initShuffle = false;
            if((currentFrame1 % 6) == (currentFrame2 % 6)){
                 board_number= currentFrame1 % 6;
                 Mix_PlayChannel(-1,board_change,0);
            }//key

            score = abs((currentFrame1 % 6) - (currentFrame2 % 6));

            updateScoreboardTexture(renderer, &scoreboardTexture, &score);

            if(board_number==0){
                setLocation_0(abs((currentFrame1 % 6) - (currentFrame2 % 6)), &pieceX_0, &pieceY_0, &pieceX_1, &pieceY_1);
            }else if(board_number==1){
                setLocation_1(abs((currentFrame1 % 6) - (currentFrame2 % 6)), &pieceX_0, &pieceY_0, &pieceX_1, &pieceY_1);
            }else if(board_number==2){
                setLocation_2(abs((currentFrame1 % 6) - (currentFrame2 % 6)), &pieceX_0, &pieceY_0, &pieceX_1, &pieceY_1);
            }else if(board_number==3){
                setLocation_3(abs((currentFrame1 % 6) - (currentFrame2 % 6)), &pieceX_0, &pieceY_0, &pieceX_1, &pieceY_1);
            }else if(board_number==4){
                setLocation_4(abs((currentFrame1 % 6) - (currentFrame2 % 6)), &pieceX_0, &pieceY_0, &pieceX_1, &pieceY_1);
            }
            else if(board_number==5){
                setLocation_5(abs((currentFrame1 % 6) - (currentFrame2 % 6)), &pieceX_0, &pieceY_0, &pieceX_1, &pieceY_1);
            }

        }



         if (initShuffle && !shuffle1 && !shuffle2 && p2) {
            initShuffle = false;
            if((currentFrame1 % 6) == (currentFrame2 % 6)){
                 board_number= currentFrame1 % 6;
                 Mix_PlayChannel(-1,board_change,0);
            }

            score = abs((currentFrame1 % 6) - (currentFrame2 % 6));

            updateScoreboardTexture(renderer, &scoreboardTexture, &score);
            if(board_number==0){
                setLocationp2_0(abs((currentFrame1 % 6) - (currentFrame2 % 6)), &pieceX_2, &pieceY_2, &pieceX_22, &pieceY_22);
            }else if(board_number==1){
                setLocationp2_1(abs((currentFrame1 % 6) - (currentFrame2 % 6)), &pieceX_2, &pieceY_2, &pieceX_22, &pieceY_22);
            }else if(board_number==2){
                setLocationp2_2(abs((currentFrame1 % 6) - (currentFrame2 % 6)), &pieceX_2, &pieceY_2, &pieceX_22, &pieceY_22);
            }else if(board_number==3){
                setLocationp2_3(abs((currentFrame1 % 6) - (currentFrame2 % 6)), &pieceX_2, &pieceY_2, &pieceX_22, &pieceY_22);
            }else if(board_number==4){
                setLocationp2_4(abs((currentFrame1 % 6) - (currentFrame2 % 6)), &pieceX_2, &pieceY_2, &pieceX_22, &pieceY_22);
            }
            else if(board_number==5){
                setLocationp2_5(abs((currentFrame1 % 6) - (currentFrame2 % 6)), &pieceX_2, &pieceY_2, &pieceX_22, &pieceY_22);
            }


        }


         if (initShuffle && !shuffle1 && !shuffle2 && p3) {
            initShuffle = false;

            if((currentFrame1 % 6) == (currentFrame2 % 6)){
                 board_number= currentFrame1 % 6;
                 Mix_PlayChannel(-1,board_change,0);
            }
            score = abs((currentFrame1 % 6) - (currentFrame2 % 6));

            updateScoreboardTexture(renderer, &scoreboardTexture, &score);
            if(board_number==0){
                setLocationp3_0(abs((currentFrame1 % 6) - (currentFrame2 % 6)), &pieceX_3, &pieceY_3, &pieceX_33, &pieceY_33);
            }else if(board_number==1){
                setLocationp3_1(abs((currentFrame1 % 6) - (currentFrame2 % 6)), &pieceX_3, &pieceY_3, &pieceX_33, &pieceY_33);
            }else if(board_number==2){
                setLocationp3_2(abs((currentFrame1 % 6) - (currentFrame2 % 6)), &pieceX_3, &pieceY_3, &pieceX_33, &pieceY_33);
            }else if(board_number==3){
                setLocationp3_3(abs((currentFrame1 % 6) - (currentFrame2 % 6)), &pieceX_3, &pieceY_3, &pieceX_33, &pieceY_33);
            }else if(board_number==4){
                setLocationp3_4(abs((currentFrame1 % 6) - (currentFrame2 % 6)), &pieceX_3, &pieceY_3, &pieceX_33, &pieceY_33);
            }
            else if(board_number==5){
                setLocationp3_5(abs((currentFrame1 % 6) - (currentFrame2 % 6)), &pieceX_3, &pieceY_3, &pieceX_33, &pieceY_33);
            }


        }







        // Render the scoreboard
        renderScoreboard(renderer, scoreboardTexture, scoreX, scoreY);


        if(loc==100){
                 SDL_Delay(1500);
                Mix_PlayChannel(-1,applause,0);
            renderWin(renderer, WinTexture1, scoreX, scoreY );
        }
        else if(loc2==100){
             SDL_Delay(1500);
            Mix_PlayChannel(-1,applause,0);
            renderWin(renderer, WinTexture2, scoreX, scoreY );
        }
        else if(loc3==100){
             SDL_Delay(1500);
            Mix_PlayChannel(-1,applause,0);
            renderWin(renderer, WinTexture3, scoreX, scoreY );
        }
        else if(loc4==100){
             SDL_Delay(1500);
            Mix_PlayChannel(-1,applause,0);
            renderWin(renderer, WinTexture4, scoreX, scoreY );
        }

        SDL_RenderPresent(renderer);


    }

    // Free dice surfaces and textures
    for (int i = 0; i < 6; ++i) {
        SDL_FreeSurface(diceSurfaces[i]);
        SDL_DestroyTexture(diceTextures[i]);
    }

    SDL_DestroyTexture(scoreboardTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    closeSDL();
    return 0;
}

bool initSDL() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return false;
    }
    return true;
}

void closeSDL() {
    Mix_FreeMusic(background_music);
    Mix_FreeChunk(ladder);
    Mix_FreeChunk(click_effect);
    Mix_FreeChunk(snake);
    Mix_FreeChunk(dice);
    Mix_FreeChunk(board_change);
    Mix_FreeChunk(game);
    Mix_FreeChunk(board_change);

    SDL_Quit();
}

int generateRandomNumber() {
    // Generate a random number between 1 and 6
    return rand() % 6 + 1;
}

void drawPiece(SDL_Renderer *renderer, int pieceX, int pieceY) {
    // Render the piece
    SDL_Rect pieceRect = { pieceX, pieceY, 90, 45 };
    SDL_RenderCopy(renderer, block1texture, NULL, &pieceRect);
}


void drawPiece2(SDL_Renderer *renderer, int pieceX, int pieceY) {
    // Render the piece
    SDL_Rect pieceRect = { pieceX, pieceY, 100, 50 };
    SDL_RenderCopy(renderer, block2texture, NULL, &pieceRect);
}

void drawPiece3(SDL_Renderer *renderer, int pieceX, int pieceY) {
    // Render the piece
    SDL_Rect pieceRect = { pieceX, pieceY, 100, 50 };
    SDL_RenderCopy(renderer, block3texture, NULL, &pieceRect);
}

void drawPiece4(SDL_Renderer *renderer, int pieceX, int pieceY) {
    // Render the piece
    SDL_Rect pieceRect = { pieceX, pieceY, 100, 50 };
    SDL_RenderCopy(renderer, block4texture, NULL, &pieceRect);
}

//For Player 1
void setLocation_0(int value, int* pieceX_0, int* pieceY_0, int* pieceX_1, int* pieceY_1) {
    printf("sl(%d)\n", value);
    if(ability_1==1){
        loc += (2*value);
        ability_1=0;
    }
    else loc+=value;
    if (loc < 1) loc = 1;
    if (loc > 100) loc -=value ;

    // Teleportation conditions

    //SNAKE_POSITION
    if (loc == 99){
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        loc = 35;
    }
    else if (loc == 82){
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        loc = 65;
    }
    else if (loc == 79){
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        loc = 29;
    }
    else if (loc == 54){
        //SDL_Delay(LOC_DELAY_MS);
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        loc = 25;
    }
    else if (loc == 43){
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        loc = 11;
    }
    else if (loc == 26){
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        loc = 7;
    }


    //LADDER_POSITION
    if (loc == 10){
        Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        loc = 50;
    }
    else if (loc == 24){
        Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        loc = 53;
    }
    else if (loc == 38){
            Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        //SDL_Delay(LOC_DELAY_MS);
        loc = 70;
    }
    else if (loc == 41){
        Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        loc = 63;
    }
    else if (loc == 52){
        Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        loc = 81;
    }
    else if (loc == 75){
        Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        loc = 93;
    }



    if (loc >= 100)
        printf("Finally, Won!!!");

    int x;
    x = loc % NUM_COLS;
    if (x == 0) x = NUM_COLS;
    int y = (loc + NUM_ROWS - 1) / NUM_COLS;


    *pieceX_0 = *pieceX_1;
    *pieceY_0 = *pieceY_1;
    *pieceX_1 = 7 + (x - 1) * GRID_SIZE;
    *pieceY_1 = 1 + (NUM_COLS - y) * GRID_SIZE;

    printf("(%d, %d) ", x, y);
}


void setLocation_1(int value, int* pieceX_0, int* pieceY_0, int* pieceX_1, int* pieceY_1) {
    printf("sl(%d)\n", value);
    if(ability_1==1){
        loc += (2*value);
        ability_1=0;
    }
    else loc+=value;
    if (loc < 1) loc = 1;
    if (loc > 100) loc -=value;

    // Teleportation conditions
    //SNAKE_POSITION
    if (loc == 96){
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        loc = 31;
    }
    else if (loc == 88){
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        loc = 59;
    }
    else if (loc == 73){
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        loc = 44;
    }
    else if (loc == 55){
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        //SDL_Delay(LOC_DELAY_MS);
        loc = 28;
    }
    else if (loc == 43){
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        loc = 15;
    }
    else if (loc == 40){
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        loc = 8;
    }


    //LADDER_POSITION
    if (loc == 2){
        Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        loc = 35;
    }
    else if (loc == 16){
        Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        loc = 48;
    }
    else if (loc == 32){
        Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        loc = 61;
    }
    else if (loc == 47){
        Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        //SDL_Delay(LOC_DELAY_MS);
        loc = 76;
    }
    else if (loc == 50){
        Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        loc = 90;
    }
    else if (loc == 72){
        Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        loc = 94;
    }


    if (loc >= 100)
        printf("Finally, Won!!!");

    int x;
    x = loc % NUM_COLS;
    if (x == 0) x = NUM_COLS;
    int y = (loc + NUM_ROWS - 1) / NUM_COLS;

    *pieceX_0 = *pieceX_1;
    *pieceY_0 = *pieceY_1;
    *pieceX_1 = 7 + (x - 1) * GRID_SIZE;
    *pieceY_1 = 1 + (NUM_COLS - y) * GRID_SIZE;

    printf("(%d, %d) ", x, y);
}


void setLocation_2(int value, int* pieceX_0, int* pieceY_0, int* pieceX_1, int* pieceY_1) {
    printf("sl(%d)\n", value);
   if(ability_1==1){
        loc += (2*value);
        ability_1=0;
    }
    else loc+=value;
    if (loc < 1) loc = 1;
    if (loc > 100) loc -= value;
// Teleportation conditions

    //SNAKE_POSITION
    if (loc == 96){
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        loc = 69;
    }
    else if (loc == 83){
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        loc = 15;
    }
    else if (loc == 76){
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        loc = 54;
    }
    else if (loc == 53){
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        //SDL_Delay(LOC_DELAY_MS);
        loc = 21;
    }
    else if (loc == 60){
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        loc = 28;
    }
    else if (loc == 36){
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        loc = 7;
    }


    //LADDER_POSITION
    if (loc == 9){
        Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        loc = 40;
    }
    else if (loc == 11){
        Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        loc = 51;
    }
    else if (loc == 37){
        Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        loc = 56;
    }
    else if (loc == 47){
        Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        //SDL_Delay(LOC_DELAY_MS);
        loc = 70;
    }
    else if (loc == 62){
        Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        loc = 95;
    }
    else if (loc == 67){
        Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        loc = 89;
    }



    if (loc >= 100)
        printf("Finally, Won!!!");

    int x;
    x = loc % NUM_COLS;
    if (x == 0) x = NUM_COLS;
    int y = (loc + NUM_ROWS - 1) / NUM_COLS;


    *pieceX_0 = *pieceX_1;
    *pieceY_0 = *pieceY_1;
    *pieceX_1 = 7 + (x - 1) * GRID_SIZE;
    *pieceY_1 = 1 + (NUM_COLS - y) * GRID_SIZE;

    printf("(%d, %d) ", x, y);
}

void setLocation_3(int value, int* pieceX_0, int* pieceY_0, int* pieceX_1, int* pieceY_1) {
    printf("sl(%d)\n", value);
    if(ability_1==1){
        loc += (2*value);
        ability_1=0;
    }
    else loc+=value;
    if (loc < 1) loc = 1;
    if (loc > 100) loc -= value;

    // Teleportation conditions

    //SNAKE_POSITION
    if (loc == 95){
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        loc = 27;
    }
    else if (loc == 81){
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        loc = 52;
    }
    else if (loc == 70){
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        loc = 20;
    }
    else if (loc == 64){
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        //SDL_Delay(LOC_DELAY_MS);
        loc = 34;
    }
    else if (loc == 42){
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        loc = 4;
    }
    else if (loc == 55){
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        loc = 38;
    }


    //LADDER_POSITION
    if (loc == 6){
        Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        loc = 44;
    }
    else if (loc == 19){
        Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        loc = 59;
    }
    else if (loc == 21){
        Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        loc = 52;
    }
    else if (loc == 16){
        Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        //SDL_Delay(LOC_DELAY_MS);
        loc = 50;
    }
    else if (loc == 43){
        Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        loc = 82;
    }
    else if (loc == 65){
        Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        loc = 89;
    }


    if (loc >= 100)
        printf("Finally, Won!!!");

    int x;
    x = loc % NUM_COLS;
    if (x == 0) x = NUM_COLS;
    int y = (loc + NUM_ROWS - 1) / NUM_COLS;



    *pieceX_0 = *pieceX_1;
    *pieceY_0 = *pieceY_1;
    *pieceX_1 = 9 + (x - 1) * GRID_SIZE;
    *pieceY_1 = 1 + (NUM_COLS - y) * GRID_SIZE;

    printf("(%d, %d) ", x, y);
}


void setLocation_4(int value, int* pieceX_0, int* pieceY_0, int* pieceX_1, int* pieceY_1) {
    printf("sl(%d)\n", value);
    if(ability_1==1){
        loc += (2*value);
        ability_1=0;
    }
    else loc+=value;
    if (loc < 1) loc = 1;
    if (loc > 100) loc -= value;

    // Teleportation conditions

    //SNAKE_POSITION
    if (loc == 98){
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        loc = 30;
    }
    else if (loc == 85){
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        loc = 35;
    }
    else if (loc == 80){
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        loc = 17;
    }
    else if (loc == 61){
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        //SDL_Delay(LOC_DELAY_MS);
        loc = 34;
    }
    else if (loc == 68){
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        loc = 45;
    }
    else if (loc == 44){
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        loc = 6;
    }


    //LADDER_POSITION
    if (loc == 10){
        Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        loc = 50;
    }
    else if (loc == 13){
        Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        loc = 51;
    }
    else if (loc == 26){
        Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        loc = 65;
    }
    else if (loc == 33){
        Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        //SDL_Delay(LOC_DELAY_MS);
        loc = 72;
    }
    else if (loc == 54){
        Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        loc = 92;
    }
    else if (loc == 66){
        Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        loc = 89;
    }


    if (loc >= 100)
        printf("Finally, Won!!!");

    int x;
    x = loc % NUM_COLS;
    if (x == 0) x = NUM_COLS;
    int y = (loc + NUM_ROWS - 1) / NUM_COLS;



    *pieceX_0 = *pieceX_1;
    *pieceY_0 = *pieceY_1;
    *pieceX_1 = 7 + (x - 1) * GRID_SIZE;
    *pieceY_1 = 1 + (NUM_COLS - y) * GRID_SIZE;
    printf("(%d, %d) ", x, y);
}


void setLocation_5(int value, int* pieceX_0, int* pieceY_0, int* pieceX_1, int* pieceY_1) {
    printf("sl(%d)\n", value);
    if(ability_1==1){
        loc += (2*value);
        ability_1=0;
    }
    else loc+=value;
    if (loc < 1) loc = 1;
    if (loc > 100) loc -= value;

    // Teleportation conditions

    //SNAKE_POSITION
    if (loc == 99){
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        loc = 57;
    }
    else if (loc == 84){
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        loc = 32;
    }
    else if (loc == 72){
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        loc = 4;
    }
    else if (loc == 60){
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        //SDL_Delay(LOC_DELAY_MS);
        loc = 24;
    }
    else if (loc == 47){
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        loc = 19;
    }
    else if (loc == 25){
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        loc = 2;
    }


    //LADDER_POSITION
    if (loc == 10){
        Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        loc = 49;
    }
    else if (loc == 21){
        Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        loc = 65;
    }
    else if (loc == 26){
        Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        loc = 55;
    }
    else if (loc == 41){
        Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        //SDL_Delay(LOC_DELAY_MS);
        loc = 81;
    }
    else if (loc == 58){
        Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        loc = 90;
    }
    else if (loc == 75){
        Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        loc = 98;
    }


    if (loc >= 100)
        printf("Finally, Won!!!");

    int x;
    x = loc % NUM_COLS;
    if (x == 0) x = NUM_COLS;
    int y = (loc + NUM_ROWS - 1) / NUM_COLS;



    *pieceX_0 = *pieceX_1;
    *pieceY_0 = *pieceY_1;
    *pieceX_1 = 7 + (x - 1) * GRID_SIZE;
    *pieceY_1 = 1 + (NUM_COLS - y) * GRID_SIZE;

    printf("(%d, %d) ", x, y);
}


//For Player 2
void setLocationp2_0(int value, int* pieceX_0, int* pieceY_0, int* pieceX_1, int* pieceY_1) {
    printf("sl(%d)\n", value);
    loc2 += value;
    if (loc2 < 1) loc2 = 1;
    if (loc2 > 100) loc2 -= value;

    // Teleportation conditions

    //SNAKE_POSITION
    if(ability_2==0){
        if (loc2 == 99){
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        loc2 = 35;
    }
    else if (loc2 == 82){
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        loc2 = 65;
    }
    else if (loc2 == 79){
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        loc2 = 29;
    }
    else if (loc2 == 54){
        //SDL_Delay(LOC_DELAY_MS);
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        loc2 = 25;
    }
    else if (loc2 == 43){
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        loc2 = 11;
    }
    else if (loc2 == 26){
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        loc2 = 7;
    }


    //LADDER_POSITION
    if (loc2 == 10){
        Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        loc2 = 50;
    }
    else if (loc2 == 24){
        Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        loc2 = 53;
    }
    else if (loc2 == 38){
            Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        //SDL_Delay(LOC_DELAY_MS);
        loc2 = 70;
    }
    else if (loc2 == 41){
        Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        loc2 = 63;
    }
    else if (loc2 == 52){
        Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        loc2 = 81;
    }
    else if (loc2 == 75){
        Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        loc2 = 93;
    }

    }
    else if(ability_2==1)
        ability_2=0;



    if (loc2 >= 100)
        printf("Finally, Won!!!");

    int x;
    x = loc2 % NUM_COLS;
    if (x == 0) x = NUM_COLS;
    int y = (loc2 + NUM_ROWS - 1) / NUM_COLS;


    *pieceX_0 = *pieceX_1;
    *pieceY_0 = *pieceY_1;
    *pieceX_1 = 1 + (x - 1) * GRID_SIZE;
    *pieceY_1 = -3 + (NUM_COLS - y) * GRID_SIZE;


    printf("(%d, %d) ", x, y);
}


void setLocationp2_1(int value, int* pieceX_0, int* pieceY_0, int* pieceX_1, int* pieceY_1) {
    printf("sl(%d)\n", value);
    loc2 += value;
    if (loc2 < 1) loc2 = 1;
    if (loc2 > 100) loc2 -= value;

    // Teleportation conditions

    // Teleportation conditions
    if(ability_2==0){
        //SNAKE_POSITION
    if (loc2 == 96){
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        loc2 = 31;
    }
    else if (loc2 == 88){
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        loc2 = 59;
    }
    else if (loc2 == 73){
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        loc2 = 44;
    }
    else if (loc2 == 55){
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        //SDL_Delay(LOC_DELAY_MS);
        loc2 = 28;
    }
    else if (loc2 == 43){
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        loc2 = 15;
    }
    else if (loc2 == 40){
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        loc2 = 8;
    }


    //LADDER_POSITION
    if (loc2 == 2){
        Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        loc2 = 35;
    }
    else if (loc2 == 16){
        Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        loc2 = 48;
    }
    else if (loc2 == 32){
        Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        loc2 = 61;
    }
    else if (loc2 == 47){
        Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        //SDL_Delay(LOC_DELAY_MS);
        loc2 = 76;
    }
    else if (loc2 == 50){
        Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        loc2 = 90;
    }
    else if (loc2 == 72){
        Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        loc2 = 94;
    }
    }
    else if(ability_2==1)
        ability_2 = 0;




    if (loc2 >= 100)
        printf("Finally, Won!!!");

    int x;
    x = loc2 % NUM_COLS;
    if (x == 0) x = NUM_COLS;
    int y = (loc2 + NUM_ROWS - 1) / NUM_COLS;



    *pieceX_0 = *pieceX_1;
    *pieceY_0 = *pieceY_1;
    *pieceX_1 = 1 + (x - 1) * GRID_SIZE;
    *pieceY_1 = -3 + (NUM_COLS - y) * GRID_SIZE;

    printf("(%d, %d) ", x, y);
}


void setLocationp2_2(int value, int* pieceX_0, int* pieceY_0, int* pieceX_1, int* pieceY_1) {
    printf("sl(%d)\n", value);
    loc2 += value;
    if (loc2 < 1) loc2 = 1;
    if (loc2 > 100) loc2 -= value;
// Teleportation conditions

if(ability_2==0){

    //SNAKE_POSITION
    if (loc2 == 96){
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        loc2 = 69;
    }
    else if (loc2 == 83){
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        loc2 = 15;
    }
    else if (loc2 == 76){
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        loc2 = 54;
    }
    else if (loc2 == 53){
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        //SDL_Delay(LOC_DELAY_MS);
        loc2 = 21;
    }
    else if (loc2 == 60){
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        loc2 = 28;
    }
    else if (loc2 == 36){
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        loc2 = 7;
    }


    //LADDER_POSITION
    if (loc2 == 9){
        Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        loc2 = 40;
    }
    else if (loc2 == 11){
        Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        loc2 = 51;
    }
    else if (loc2 == 37){
        Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        loc2 = 56;
    }
    else if (loc2 == 47){
        Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        //SDL_Delay(LOC_DELAY_MS);
        loc2 = 70;
    }
    else if (loc2 == 62){
        Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        loc2 = 95;
    }
    else if (loc2 == 67){
        Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        loc2 = 89;
    }
}
else if(ability_2==1)
    ability_2 = 0;





    if (loc2 >= 100)
        printf("Finally, Won!!!");

    int x;
    x = loc2 % NUM_COLS;
    if (x == 0) x = NUM_COLS;
    int y = (loc2 + NUM_ROWS - 1) / NUM_COLS;



    *pieceX_0 = *pieceX_1;
    *pieceY_0 = *pieceY_1;
    *pieceX_1 = 1 + (x - 1) * GRID_SIZE;
    *pieceY_1 = -3 + (NUM_COLS - y) * GRID_SIZE;

    printf("(%d, %d) ", x, y);
}

void setLocationp2_3(int value, int* pieceX_0, int* pieceY_0, int* pieceX_1, int* pieceY_1) {
    printf("sl(%d)\n", value);
    loc2 += value;
    if (loc2 < 1) loc2 = 1;
    if (loc2 > 100) loc2 -= value;

    // Teleportation conditions
    if(ability_2==0){

        //SNAKE_POSITION
    if (loc2 == 95){
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        loc2 = 27;
    }
    else if (loc2 == 81){
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        loc2 = 52;
    }
    else if (loc2 == 70){
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        loc2 = 20;
    }
    else if (loc2 == 64){
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        //SDL_Delay(LOC_DELAY_MS);
        loc2 = 34;
    }
    else if (loc2 == 42){
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        loc2 = 4;
    }
    else if (loc2 == 55){
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        loc2 = 38;
    }


    //LADDER_POSITION
    if (loc2 == 6){
        Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        loc2 = 44;
    }
    else if (loc2 == 19){
        Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        loc2 = 59;
    }
    else if (loc2 == 21){
        Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        loc2 = 52;
    }
    else if (loc2 == 16){
        Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        //SDL_Delay(LOC_DELAY_MS);
        loc2 = 50;
    }
    else if (loc2 == 43){
        Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        loc2 = 82;
    }
    else if (loc2 == 65){
        Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        loc2 = 89;
    }
    }
    else if(ability_2==1)
        ability_2 = 0;



    if (loc2 >= 100)
        printf("Finally, Won!!!");

    int x;
    x = loc2 % NUM_COLS;
    if (x == 0) x = NUM_COLS;
    int y = (loc2 + NUM_ROWS - 1) / NUM_COLS;



    *pieceX_0 = *pieceX_1;
    *pieceY_0 = *pieceY_1;
    *pieceX_1 = 1 + (x - 1) * GRID_SIZE;
    *pieceY_1 = -3 + (NUM_COLS - y) * GRID_SIZE;

    printf("(%d, %d) ", x, y);
}


void setLocationp2_4(int value, int* pieceX_0, int* pieceY_0, int* pieceX_1, int* pieceY_1) {
    printf("sl(%d)\n", value);
    loc2 += value;
    if (loc2 < 1) loc2 = 1;
    if (loc2 > 100) loc2 -= value;

    // Teleportation conditions
    if(ability_2==0){

        //SNAKE_POSITION
    if (loc2 == 98){
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        loc2 = 30;
    }
    else if (loc2 == 85){
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        loc2 = 35;
    }
    else if (loc2 == 80){
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        loc2 = 17;
    }
    else if (loc2 == 61){
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        //SDL_Delay(LOC_DELAY_MS);
        loc2 = 34;
    }
    else if (loc2 == 68){
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        loc2 = 45;
    }
    else if (loc2 == 44){
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        loc2 = 6;
    }


    //LADDER_POSITION
    if (loc2 == 10){
        Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        loc2 = 50;
    }
    else if (loc2 == 13){
        Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        loc2 = 51;
    }
    else if (loc2 == 26){
        Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        loc2 = 65;
    }
    else if (loc2 == 33){
        Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        //SDL_Delay(LOC_DELAY_MS);
        loc2 = 72;
    }
    else if (loc2 == 54){
        Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        loc2 = 92;
    }
    else if (loc2 == 66){
        Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        loc2 = 89;
    }
    }
    else if(ability_2==1)
        ability_2 = 0;

    if (loc2 >= 100)
        printf("Finally, Won!!!");

    int x;
    x = loc2 % NUM_COLS;
    if (x == 0) x = NUM_COLS;
    int y = (loc2 + NUM_ROWS - 1) / NUM_COLS;



    *pieceX_0 = *pieceX_1;
    *pieceY_0 = *pieceY_1;
    *pieceX_1 = 1 + (x - 1) * GRID_SIZE;
    *pieceY_1 = -3 + (NUM_COLS - y) * GRID_SIZE;

    printf("(%d, %d) ", x, y);
}


void setLocationp2_5(int value, int* pieceX_0, int* pieceY_0, int* pieceX_1, int* pieceY_1) {
    printf("sl(%d)\n", value);
    loc2 += value;
    if (loc2 < 1) loc2 = 1;
    if (loc2 > 100) loc2 -= value;

    // Teleportation conditions
    if(ability_2==0){
        //SNAKE_POSITION
    if (loc2 == 99){
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        loc2 = 57;
    }
    else if (loc2 == 84){
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        loc2 = 32;
    }
    else if (loc2 == 72){
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        loc2 = 4;
    }
    else if (loc2 == 60){
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        //SDL_Delay(LOC_DELAY_MS);
        loc2 = 24;
    }
    else if (loc2 == 47){
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        loc2 = 19;
    }
    else if (loc2 == 25){
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        loc2 = 2;
    }


    //LADDER_POSITION
    if (loc2 == 10){
        Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        loc2 = 49;
    }
    else if (loc2 == 21){
        Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        loc2 = 65;
    }
    else if (loc2 == 26){
        Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        loc2 = 55;
    }
    else if (loc2 == 41){
        Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        //SDL_Delay(LOC_DELAY_MS);
        loc2 = 81;
    }
    else if (loc2 == 58){
        Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        loc2 = 90;
    }
    else if (loc2 == 75){
        Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        loc2 = 98;
    }


    }
    else if(ability_2==1)
        ability_2 = 0;

    if (loc2 >= 100)
        printf("Finally, Won!!!");

    int x;
    x = loc2 % NUM_COLS;
    if (x == 0) x = NUM_COLS;
    int y = (loc2 + NUM_ROWS - 1) / NUM_COLS;



    *pieceX_0 = *pieceX_1;
    *pieceY_0 = *pieceY_1;
    *pieceX_1 = 1 + (x - 1) * GRID_SIZE;
    *pieceY_1 = -3 + (NUM_COLS - y) * GRID_SIZE;

    printf("(%d, %d) ", x, y);
}


//For Player 3
void setLocationp3_0(int value, int* pieceX_0, int* pieceY_0, int* pieceX_1, int* pieceY_1) {
    printf("sl(%d)\n", value);
    loc3 += value;

    if (ability_3 == 1) {
        loc3 += rand() % 12 + 1; // Add a random value between 1 and 15
        ability_3 = 0; // Reset the ability
        printf("%d mystery number added",rand() %12 +1);
}

    if (loc3 < 1) loc3 = 1;
    if (loc3 > 100) loc3 -= value;

    // Teleportation conditions

    //SNAKE_POSITION
    if (loc3 == 99){
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        loc3 = 35;
    }
    else if (loc3 == 82){
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        loc3 = 65;
    }
    else if (loc3 == 79){
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        loc3 = 29;
    }
    else if (loc3 == 54){
        //SDL_Delay(LOC_DELAY_MS);
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        loc3 = 25;
    }
    else if (loc3 == 43){
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        loc3 = 11;
    }
    else if (loc3 == 26){
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        loc3 = 7;
    }


    //LADDER_POSITION
    if (loc3 == 10){
        Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        loc3 = 50;
    }
    else if (loc3 == 24){
        Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        loc3 = 53;
    }
    else if (loc3 == 38){
            Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        //SDL_Delay(LOC_DELAY_MS);
        loc3 = 70;
    }
    else if (loc3 == 41){
        Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        loc3 = 63;
    }
    else if (loc3 == 52){
        Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        loc3 = 81;
    }
    else if (loc3 == 75){
        Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        loc3 = 93;
    }



    if (loc3 >= 100)
        printf("Finally, Won!!!");

    int x;
    x = loc3 % NUM_COLS;
    if (x == 0) x = NUM_COLS;
    int y = (loc3 + NUM_ROWS - 1) / NUM_COLS;

    *pieceX_0 = *pieceX_1;
    *pieceY_0 = *pieceY_1;
    *pieceX_1 = -12 + (x - 1) * GRID_SIZE;
    *pieceY_1 = 10 + (NUM_COLS - y) * GRID_SIZE;


    printf("(%d, %d) ", x, y);
}


void setLocationp3_1(int value, int* pieceX_0, int* pieceY_0, int* pieceX_1, int* pieceY_1) {
    printf("sl(%d)\n", value);
    loc3 += value;

    if (ability_3 == 1) {
        loc3 += rand() % 12 + 1; // Add a random value between 1 and 15
        ability_3 = 0; // Reset the ability
        printf("%d mystery number added",rand() %12 +1);
}

    if (loc3 < 1) loc3 = 1;
    if (loc3 > 100) loc3 -= value;

    // Teleportation conditions

    // Teleportation conditions

    //SNAKE_POSITION
    if (loc3 == 96){
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        loc3 = 31;
    }
    else if (loc3 == 88){
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        loc3 = 59;
    }
    else if (loc3 == 73){
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        loc3 = 44;
    }
    else if (loc3 == 55){
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        //SDL_Delay(LOC_DELAY_MS);
        loc3 = 28;
    }
    else if (loc3 == 43){
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        loc3 = 15;
    }
    else if (loc3 == 40){
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        loc3 = 8;
    }


    //LADDER_POSITION
    if (loc3 == 2){
        Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        loc3 = 35;
    }
    else if (loc3 == 16){
        Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        loc3 = 48;
    }
    else if (loc3 == 32){
        Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        loc3 = 61;
    }
    else if (loc3 == 47){
        Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        //SDL_Delay(LOC_DELAY_MS);
        loc3 = 76;
    }
    else if (loc3 == 50){
        Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        loc3 = 90;
    }
    else if (loc3 == 72){
        Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        loc3 = 94;
    }


    if (loc3 >= 100)
        printf("Finally, Won!!!");

    int x;
    x = loc3 % NUM_COLS;
    if (x == 0) x = NUM_COLS;
    int y = (loc3 + NUM_ROWS - 1) / NUM_COLS;



    *pieceX_0 = *pieceX_1;
    *pieceY_0 = *pieceY_1;
    *pieceX_1 = -12 + (x - 1) * GRID_SIZE;
    *pieceY_1 = 10 + (NUM_COLS - y) * GRID_SIZE;

    printf("(%d, %d) ", x, y);
}


void setLocationp3_2(int value, int* pieceX_0, int* pieceY_0, int* pieceX_1, int* pieceY_1) {
    printf("sl(%d)\n", value);
    loc3 += value;

    if (ability_3 == 1) {
        loc3 += rand() % 12 + 1; // Add a random value between 1 and 15
        ability_3 = 0; // Reset the ability
        printf("%d mystery number added",rand() %12 + 1);
        }

    if (loc3 < 1) loc3 = 1;
    if (loc3 > 100) loc3 -= value;
// Teleportation conditions

    //SNAKE_POSITION
    if (loc3 == 96){
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        loc3 = 69;
    }
    else if (loc3 == 83){
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        loc3 = 15;
    }
    else if (loc3 == 76){
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        loc3 = 54;
    }
    else if (loc3 == 53){
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        //SDL_Delay(LOC_DELAY_MS);
        loc3 = 21;
    }
    else if (loc3 == 60){
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        loc3 = 28;
    }
    else if (loc3 == 36){
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        loc3 = 7;
    }


    //LADDER_POSITION
    if (loc3 == 9){
        Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        loc3 = 40;
    }
    else if (loc3 == 11){
        Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        loc3 = 51;
    }
    else if (loc3 == 37){
        Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        loc3 = 56;
    }
    else if (loc3 == 47){
        Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        //SDL_Delay(LOC_DELAY_MS);
        loc3 = 70;
    }
    else if (loc3 == 62){
        Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        loc3 = 95;
    }
    else if (loc3 == 67){
        Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        loc3 = 89;
    }



    if (loc3 >= 100)
        printf("Finally, Won!!!");

    int x;
    x = loc3 % NUM_COLS;
    if (x == 0) x = NUM_COLS;
    int y = (loc3 + NUM_ROWS - 1) / NUM_COLS;



    *pieceX_0 = *pieceX_1;
    *pieceY_0 = *pieceY_1;
    *pieceX_1 = -12 + (x - 1) * GRID_SIZE;
    *pieceY_1 = 10 + (NUM_COLS - y) * GRID_SIZE;
    printf("(%d, %d) ", x, y);
}

void setLocationp3_3(int value, int* pieceX_0, int* pieceY_0, int* pieceX_1, int* pieceY_1) {
    printf("sl(%d)\n", value);
    loc3 += value;

    if (ability_3 == 1) {
        loc3 += rand() % 12 + 1; // Add a random value between 1 and 15
        ability_3 = 0; // Reset the ability
        printf("%d mystery number added",rand() %12 +1);
}

    if (loc3 < 1) loc3 = 1;
    if (loc3 > 100) loc3 -= value;

    // Teleportation conditions

    //SNAKE_POSITION
    if (loc3 == 95){
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        loc3 = 27;
    }
    else if (loc3 == 81){
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        loc3 = 52;
    }
    else if (loc3 == 70){
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        loc3 = 20;
    }
    else if (loc3 == 64){
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        //SDL_Delay(LOC_DELAY_MS);
        loc3 = 34;
    }
    else if (loc3 == 42){
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        loc3 = 4;
    }
    else if (loc3 == 55){
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        loc3 = 38;
    }


    //LADDER_POSITION
    if (loc3 == 6){
        Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        loc3 = 44;
    }
    else if (loc3 == 19){
        Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        loc3 = 59;
    }
    else if (loc3 == 21){
        Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        loc3 = 52;
    }
    else if (loc3 == 16){
        Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        //SDL_Delay(LOC_DELAY_MS);
        loc3 = 50;
    }
    else if (loc3 == 43){
        Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        loc3 = 82;
    }
    else if (loc3 == 65){
        Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        loc3 = 89;
    }


    if (loc3 >= 100)
        printf("Finally, Won!!!");

    int x;
    x = loc3 % NUM_COLS;
    if (x == 0) x = NUM_COLS;
    int y = (loc3 + NUM_ROWS - 1) / NUM_COLS;



    *pieceX_0 = *pieceX_1;
    *pieceY_0 = *pieceY_1;
    *pieceX_1 = -12 + (x - 1) * GRID_SIZE;
    *pieceY_1 = 10 + (NUM_COLS - y) * GRID_SIZE;

    printf("(%d, %d) ", x, y);
}


void setLocationp3_4(int value, int* pieceX_0, int* pieceY_0, int* pieceX_1, int* pieceY_1) {
    printf("sl(%d)\n", value);
    loc3 += value;

    if (ability_3 == 1) {
        loc3 += rand() % 12 + 1; // Add a random value between 1 and 15
        ability_3 = 0; // Reset the ability
        printf("%d mystery number added",rand() %12 +1);
}

    if (loc3 < 1) loc3 = 1;
    if (loc3 > 100) loc3 -= value;

    // Teleportation conditions

    //SNAKE_POSITION
    if (loc3 == 98){
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        loc3 = 30;
    }
    else if (loc3 == 85){
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        loc3 = 35;
    }
    else if (loc3 == 80){
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        loc3 = 17;
    }
    else if (loc3 == 61){
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        //SDL_Delay(LOC_DELAY_MS);
        loc3 = 34;
    }
    else if (loc3 == 68){
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        loc3 = 45;
    }
    else if (loc3 == 44){
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        loc3 = 6;
    }


    //LADDER_POSITION
    if (loc3 == 10){
        Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        loc3 = 50;
    }
    else if (loc3 == 13){
        Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        loc3 = 51;
    }
    else if (loc3 == 26){
        Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        loc3 = 65;
    }
    else if (loc3 == 33){
        Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        //SDL_Delay(LOC_DELAY_MS);
        loc3 = 72;
    }
    else if (loc3 == 54){
        Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        loc3 = 92;
    }
    else if (loc3 == 66){
        Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        loc3 = 89;
    }


    if (loc3 >= 100)
        printf("Finally, Won!!!");

    int x;
    x = loc3 % NUM_COLS;
    if (x == 0) x = NUM_COLS;
    int y = (loc3 + NUM_ROWS - 1) / NUM_COLS;



    *pieceX_0 = *pieceX_1;
    *pieceY_0 = *pieceY_1;
    *pieceX_1 = -12 + (x - 1) * GRID_SIZE;
    *pieceY_1 = 10 + (NUM_COLS - y) * GRID_SIZE;

    printf("(%d, %d) ", x, y);
}


void setLocationp3_5(int value, int* pieceX_0, int* pieceY_0, int* pieceX_1, int* pieceY_1) {
    printf("sl(%d)\n", value);
    loc3 += value;

    if (ability_3 == 1) {
        loc3 += rand() % 12 + 1; // Add a random value between 1 and 15
        ability_3 = 0; // Reset the ability
        printf("%d mystery number added",rand() %12 + 1);
}

    if (loc3 < 1) loc3 = 1;
    if (loc3 > 100) loc3 -= value;

    // Teleportation conditions

    //SNAKE_POSITION
    if (loc3 == 99){
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        loc3 = 57;
    }
    else if (loc3 == 84){
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        loc3 = 32;
    }
    else if (loc3 == 72){
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        loc3 = 4;
    }
    else if (loc3 == 60){
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        //SDL_Delay(LOC_DELAY_MS);
        loc3 = 24;
    }
    else if (loc3 == 47){
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        loc3 = 19;
    }
    else if (loc3 == 25){
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        loc3 = 2;
    }


    //LADDER_POSITION
    if (loc3 == 10){
        Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        loc3 = 49;
    }
    else if (loc3 == 21){
        Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        loc3 = 65;
    }
    else if (loc3 == 26){
        Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        loc3 = 55;
    }
    else if (loc3 == 41){
        Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        //SDL_Delay(LOC_DELAY_MS);
        loc3 = 81;
    }
    else if (loc3 == 58){
        Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        loc3 = 90;
    }
    else if (loc3 == 75){
        Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        loc3 = 98;
    }


    if (loc3 >= 100)
        printf("Finally, Won!!!");

    int x;
    x = loc3 % NUM_COLS;
    if (x == 0) x = NUM_COLS;
    int y = (loc3 + NUM_ROWS - 1) / NUM_COLS;



    *pieceX_0 = *pieceX_1;
    *pieceY_0 = *pieceY_1;
    *pieceX_1 = -12 + (x - 1) * GRID_SIZE;
    *pieceY_1 = 10 + (NUM_COLS - y) * GRID_SIZE;

    printf("(%d, %d) ", x, y);
}



//For Player 4
void setLocationp4_0(int value, int* pieceX_0, int* pieceY_0, int* pieceX_1, int* pieceY_1) {
    printf("sl(%d)\n", value);
    loc4 += value;

    if(ability_4==1){
        loc4+=2; loc-=2; loc2-=2; loc3-=2;
        ability_4=0;
        }

    if (loc4 < 1) loc4 = 1;
    if (loc4 > 100) loc4 -= value;

    // Teleportation conditions

    //SNAKE_POSITION
    if (loc4 == 99){
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        loc4 = 35;
    }
    else if (loc4 == 82){
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        loc4 = 65;
    }
    else if (loc4 == 79){
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        loc4 = 29;
    }
    else if (loc4 == 54){
        //SDL_Delay(LOC_DELAY_MS);
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        loc4 = 25;
    }
    else if (loc4 == 43){
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        loc4 = 11;
    }
    else if (loc4 == 26){
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        loc4 = 7;
    }


    //LADDER_POSITION
    if (loc4 == 10){
        Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        loc4 = 50;
    }
    else if (loc4 == 24){
        Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        loc4 = 53;
    }
    else if (loc4 == 38){
            Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        //SDL_Delay(LOC_DELAY_MS);
        loc4 = 70;
    }
    else if (loc4 == 41){
        Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        loc4 = 63;
    }
    else if (loc4 == 52){
        Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        loc4 = 81;
    }
    else if (loc4 == 75){
        Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        loc4 = 93;
    }



    if (loc4 >= 100)
        printf("Finally, Won!!!");

    int x;
    x = loc4 % NUM_COLS;
    if (x == 0) x = NUM_COLS;
    int y = (loc4 + NUM_ROWS - 1) / NUM_COLS;



    *pieceX_0 = *pieceX_1;
    *pieceY_0 = *pieceY_1;
    *pieceX_1 = -11 + (x - 1) * GRID_SIZE;
    *pieceY_1 = -20 + (NUM_COLS - y) * GRID_SIZE;

    printf("(%d, %d) ", x, y);
}


void setLocationp4_1(int value, int* pieceX_0, int* pieceY_0, int* pieceX_1, int* pieceY_1) {
    printf("sl(%d)\n", value);
    loc4 += value;

    if(ability_4==1){
        loc4+=2; loc-=2; loc2-=2; loc3-=2;
        ability_4=0;
}

    if (loc4 < 1) loc4 = 1;
    if (loc4 > 100) loc4 -= value;

    // Teleportation conditions

    // Teleportation conditions

    //SNAKE_POSITION
    if (loc4 == 96){
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        loc4 = 31;
    }
    else if (loc4 == 88){
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        loc4 = 59;
    }
    else if (loc4 == 73){
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        loc4 = 44;
    }
    else if (loc4 == 55){
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        //SDL_Delay(LOC_DELAY_MS);
        loc4 = 28;
    }
    else if (loc4 == 43){
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        loc4 = 15;
    }
    else if (loc4 == 40){
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        loc4 = 8;
    }


    //LADDER_POSITION
    if (loc4 == 2){
        Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        loc4 = 35;
    }
    else if (loc4 == 16){
        Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        loc4 = 48;
    }
    else if (loc4 == 32){
        Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        loc4 = 61;
    }
    else if (loc4 == 47){
        Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        //SDL_Delay(LOC_DELAY_MS);
        loc4 = 76;
    }
    else if (loc4 == 50){
        Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        loc4 = 90;
    }
    else if (loc4 == 72){
        Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        loc4 = 94;
    }


    if (loc4 >= 100)
        printf("Finally, Won!!!");

    int x;
    x = loc4 % NUM_COLS;
    if (x == 0) x = NUM_COLS;
    int y = (loc4 + NUM_ROWS - 1) / NUM_COLS;

    printf("Before update: pieceX_0 = %d, pieceY_0 = %d, pieceX_1 = %d, pieceY_1 = %d\n", *pieceX_0, *pieceY_0, *pieceX_1, *pieceY_1);

    *pieceX_0 = *pieceX_1;
    *pieceY_0 = *pieceY_1;
    *pieceX_1 = -11 + (x - 1) * GRID_SIZE;
    *pieceY_1 = -20 + (NUM_COLS - y) * GRID_SIZE;
    printf("After update: pieceX_0 = %d, pieceY_0 = %d, pieceX_1 = %d, pieceY_1 = %d\n", *pieceX_0, *pieceY_0, *pieceX_1, *pieceY_1);
    printf("(%d, %d) ", x, y);
}


void setLocationp4_2(int value, int* pieceX_0, int* pieceY_0, int* pieceX_1, int* pieceY_1) {
    printf("sl(%d)\n", value);
    loc4 += value;

    if(ability_4==1){
        loc4+=2; loc-=2; loc2-=2; loc3-=2;
        ability_4=0;
}

    if (loc4 < 1) loc4 = 1;
    if (loc4 > 100) loc4 -= value;
// Teleportation conditions

    //SNAKE_POSITION
    if (loc4 == 96){
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        loc4 = 69;
    }
    else if (loc4 == 83){
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        loc4 = 15;
    }
    else if (loc4 == 76){
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        loc4 = 54;
    }
    else if (loc4 == 53){
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        //SDL_Delay(LOC_DELAY_MS);
        loc4 = 21;
    }
    else if (loc4 == 60){
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        loc4 = 28;
    }
    else if (loc4 == 36){
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        loc4 = 7;
    }


    //LADDER_POSITION
    if (loc4 == 9){
        Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        loc4 = 40;
    }
    else if (loc4 == 11){
        Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        loc4 = 51;
    }
    else if (loc4 == 37){
        Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        loc4 = 56;
    }
    else if (loc4 == 47){
        Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        //SDL_Delay(LOC_DELAY_MS);
        loc4 = 70;
    }
    else if (loc4 == 62){
        Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        loc4 = 95;
    }
    else if (loc4 == 67){
        Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        loc4 = 89;
    }



    if (loc4 >= 100)
        printf("Finally, Won!!!");

    int x;
    x = loc4 % NUM_COLS;
    if (x == 0) x = NUM_COLS;
    int y = (loc4 + NUM_ROWS - 1) / NUM_COLS;


printf("Before update: pieceX_0 = %d, pieceY_0 = %d, pieceX_1 = %d, pieceY_1 = %d\n", *pieceX_0, *pieceY_0, *pieceX_1, *pieceY_1);
    *pieceX_0 = *pieceX_1;
    *pieceY_0 = *pieceY_1;
    *pieceX_1 = -11 + (x - 1) * GRID_SIZE;
    *pieceY_1 = -20 + (NUM_COLS - y) * GRID_SIZE;
 printf("After update: pieceX_0 = %d, pieceY_0 = %d, pieceX_1 = %d, pieceY_1 = %d\n", *pieceX_0, *pieceY_0, *pieceX_1, *pieceY_1);
    printf("(%d, %d) ", x, y);
}

void setLocationp4_3(int value, int* pieceX_0, int* pieceY_0, int* pieceX_1, int* pieceY_1) {
    printf("sl(%d)\n", value);
    loc4 += value;

    if(ability_4==1){
        loc4+=2; loc-=2; loc2-=2; loc3-=2;
        ability_4=0;
}

    if (loc4 < 1) loc4 = 1;
    if (loc4 > 100) loc4 -= value;

    // Teleportation conditions

    //SNAKE_POSITION
    if (loc4 == 95){
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        loc4 = 27;
    }
    else if (loc4 == 81){
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        loc4 = 52;
    }
    else if (loc4 == 70){
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        loc4 = 20;
    }
    else if (loc4 == 64){
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        //SDL_Delay(LOC_DELAY_MS);
        loc4 = 34;
    }
    else if (loc4 == 42){
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        loc4 = 4;
    }
    else if (loc4 == 55){
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        loc4 = 38;
    }


    //LADDER_POSITION
    if (loc4 == 6){
        Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        loc4 = 44;
    }
    else if (loc4 == 19){
        Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        loc4 = 59;
    }
    else if (loc4 == 21){
        Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        loc4 = 52;
    }
    else if (loc4 == 16){
        Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        //SDL_Delay(LOC_DELAY_MS);
        loc4 = 50;
    }
    else if (loc4 == 43){
        Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        loc4 = 82;
    }
    else if (loc4 == 65){
        Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        loc4 = 89;
    }


    if (loc4 >= 100)
        printf("Finally, Won!!!");

    int x;
    x = loc4 % NUM_COLS;
    if (x == 0) x = NUM_COLS;
    int y = (loc4 + NUM_ROWS - 1) / NUM_COLS;

printf("Before update: pieceX_0 = %d, pieceY_0 = %d, pieceX_1 = %d, pieceY_1 = %d\n", *pieceX_0, *pieceY_0, *pieceX_1, *pieceY_1);

    *pieceX_0 = *pieceX_1;
    *pieceY_0 = *pieceY_1;
    *pieceX_1 = -11 + (x - 1) * GRID_SIZE;
    *pieceY_1 = -20 + (NUM_COLS - y) * GRID_SIZE;
 printf("After update: pieceX_0 = %d, pieceY_0 = %d, pieceX_1 = %d, pieceY_1 = %d\n", *pieceX_0, *pieceY_0, *pieceX_1, *pieceY_1);
    printf("(%d, %d) ", x, y);
}


void setLocationp4_4(int value, int* pieceX_0, int* pieceY_0, int* pieceX_1, int* pieceY_1) {
    printf("sl(%d)\n", value);
    loc4 += value;


    if(ability_4==1){
        loc4+=2; loc-=2; loc2-=2; loc3-=2;
        ability_4=0;
}


    if (loc4 < 1) loc4 = 1;
    if (loc4 > 100) loc4 -= value;

    // Teleportation conditions

    //SNAKE_POSITION
    if (loc4 == 98){
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        loc4 = 30;
    }
    else if (loc4 == 85){
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        loc4 = 35;
    }
    else if (loc4 == 80){
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        loc4 = 17;
    }
    else if (loc4 == 61){
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        //SDL_Delay(LOC_DELAY_MS);
        loc4 = 34;
    }
    else if (loc4 == 68){
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        loc4 = 45;
    }
    else if (loc4 == 44){
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        loc4 = 6;
    }


    //LADDER_POSITION
    if (loc4 == 10){
        Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        loc4 = 50;
    }
    else if (loc4 == 13){
        Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        loc4 = 51;
    }
    else if (loc4 == 26){
        Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        loc4 = 65;
    }
    else if (loc4 == 33){
        Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        //SDL_Delay(LOC_DELAY_MS);
        loc4 = 72;
    }
    else if (loc4 == 54){
        Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        loc4 = 92;
    }
    else if (loc4 == 66){
        Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        loc4 = 89;
    }


    if (loc4 >= 100)
        printf("Finally, Won!!!");

    int x;
    x = loc4 % NUM_COLS;
    if (x == 0) x = NUM_COLS;
    int y = (loc4 + NUM_ROWS - 1) / NUM_COLS;


printf("Before update: pieceX_0 = %d, pieceY_0 = %d, pieceX_1 = %d, pieceY_1 = %d\n", *pieceX_0, *pieceY_0, *pieceX_1, *pieceY_1);
    *pieceX_0 = *pieceX_1;
    *pieceY_0 = *pieceY_1;
    *pieceX_1 = -11 + (x - 1) * GRID_SIZE;
    *pieceY_1 = -20 + (NUM_COLS - y) * GRID_SIZE;
 printf("After update: pieceX_0 = %d, pieceY_0 = %d, pieceX_1 = %d, pieceY_1 = %d\n", *pieceX_0, *pieceY_0, *pieceX_1, *pieceY_1);
    printf("(%d, %d) ", x, y);
}


void setLocationp4_5(int value, int* pieceX_0, int* pieceY_0, int* pieceX_1, int* pieceY_1) {
    printf("sl(%d)\n", value);
    loc4 += value;


    if(ability_4==1){
        loc4+=2; loc-=2; loc2-=2; loc3-=2;
        ability_4=0;
}


    if (loc4 < 1) loc4 = 1;
    if (loc4 > 100) loc4 -= value;

    // Teleportation conditions

    //SNAKE_POSITION
    if (loc4 == 99){
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        loc4 = 57;
    }
    else if (loc4 == 84){
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        loc4 = 32;
    }
    else if (loc4 == 72){
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        loc4 = 4;
    }
    else if (loc4 == 60){
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        //SDL_Delay(LOC_DELAY_MS);
        loc4 = 24;
    }
    else if (loc4 == 47){
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        loc4 = 19;
    }
    else if (loc4 == 25){
        Mix_PlayChannel(-1,snake,0);
        SDL_Delay(10);
        loc4 = 2;
    }


    //LADDER_POSITION
    if (loc4 == 10){
        Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        loc4 = 49;
    }
    else if (loc4 == 21){
        Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        loc4 = 65;
    }
    else if (loc4 == 26){
        Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        loc4 = 55;
    }
    else if (loc4 == 41){
        Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        //SDL_Delay(LOC_DELAY_MS);
        loc4 = 81;
    }
    else if (loc4 == 58){
        Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        loc4 = 90;
    }
    else if (loc4 == 75){
        Mix_PlayChannel(-1,ladder,0);
        SDL_Delay(10);
        loc4 = 98;
    }


    if (loc4 >= 100)
        printf("Finally, Won!!!");

    int x;
    x = loc4 % NUM_COLS;
    if (x == 0) x = NUM_COLS;
    int y = (loc4 + NUM_ROWS - 1) / NUM_COLS;


printf("Before update: pieceX_0 = %d, pieceY_0 = %d, pieceX_1 = %d, pieceY_1 = %d\n", *pieceX_0, *pieceY_0, *pieceX_1, *pieceY_1);
    *pieceX_0 = *pieceX_1;
    *pieceY_0 = *pieceY_1;
    *pieceX_1 = -11 + (x - 1) * GRID_SIZE;
    *pieceY_1 = -20 + (NUM_COLS - y) * GRID_SIZE;
 printf("After update: pieceX_0 = %d, pieceY_0 = %d, pieceX_1 = %d, pieceY_1 = %d\n", *pieceX_0, *pieceY_0, *pieceX_1, *pieceY_1);
    printf("(%d, %d) ", x, y);
}


void colorBox(SDL_Renderer *renderer, int row, int col, int colorR, int colorG, int colorB) {
    SDL_SetRenderDrawColor(renderer, colorR, colorG, colorB, 255);
    SDL_Rect boxRect = { col * GRID_SIZE, row * GRID_SIZE, GRID_SIZE, GRID_SIZE };
    SDL_RenderFillRect(renderer, &boxRect);
}

SDL_Texture* loadScoreboardTexture(SDL_Renderer *renderer, const char* path) {
    SDL_Surface *surface = SDL_LoadBMP(path);
    if (surface == NULL) {
        printf("Unable to load image %s! SDL_Error: %s\n", path, SDL_GetError());
        return NULL;
    }
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    if (texture == NULL) {
        printf("Unable to create texture from %s! SDL_Error: %s\n", path, SDL_GetError());
    }
    return texture;
}

void renderScoreboard(SDL_Renderer *renderer, SDL_Texture *scoreboardTexture, int scoreX, int scoreY) {
    SDL_Rect scoreboardRect = { scoreX, scoreY, SCORE_WIDTH, SCORE_HEIGHT };
    SDL_RenderCopy(renderer, scoreboardTexture, NULL, &scoreboardRect);
}
void updateScoreboardTexture(SDL_Renderer *renderer, SDL_Texture **scoreboardTexture, int *score) {
    // Generate the filename based on the current score
    char filename[20];
    sprintf(filename, "digit_%d.bmp", *score);

    // Load the new scoreboard texture
    SDL_Texture *newScoreboardTexture = loadScoreboardTexture(renderer, filename);
    if (newScoreboardTexture == NULL) {
        printf("Unable to update scoreboard texture!\n");
        return;
    }

    // Free the previous texture
    SDL_DestroyTexture(*scoreboardTexture);

    // Update the pointer to the new texture
    *scoreboardTexture = newScoreboardTexture;
}

//winning screen

SDL_Texture* loadWinTexture(SDL_Renderer *renderer, const char* path) {
    SDL_Surface *surface = SDL_LoadBMP(path);
    if (surface == NULL) {
        printf("Unable to load image %s! SDL_Error: %s\n", path, SDL_GetError());
        return NULL;
    }
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    if (texture == NULL) {
        printf("Unable to create texture from %s! SDL_Error: %s\n", path, SDL_GetError());
    }
    return texture;
}


void renderWin(SDL_Renderer *renderer, SDL_Texture *WinTexture, int scoreX, int scoreY) {
    SDL_Rect winRect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
    SDL_RenderCopy(renderer, WinTexture, NULL, &winRect);
}


void drawImageOverGrid(SDL_Renderer *renderer, int board_number, int gridWidth, int gridHeight) {
    // Generate the image path based on the board number
    char imagePath[20];
    sprintf(imagePath, "board_%d.bmp", board_number);

    // Load the image
    SDL_Surface *imageSurface = SDL_LoadBMP(imagePath);
    if (imageSurface == NULL) {
        printf("Unable to load image %s! SDL_Error: %s\n", imagePath, SDL_GetError());
        return;
    }

    // Create texture from the surface
    SDL_Texture *imageTexture = SDL_CreateTextureFromSurface(renderer, imageSurface);
    if (imageTexture == NULL) {
        printf("Unable to create texture from image surface! SDL_Error: %s\n", SDL_GetError());
        SDL_FreeSurface(imageSurface);
        return;
    }

    // Define the destination rectangle to fit the grid dimensions
    SDL_Rect destRect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

    // Render the image over the grid
    SDL_RenderCopy(renderer, imageTexture, NULL, &destRect);

    // Free surfaces and texture
    SDL_DestroyTexture(imageTexture);
    SDL_FreeSurface(imageSurface);
}



void renderMenu(SDL_Renderer *renderer, const char *imagePath) {
    // Load the menu image
    SDL_Surface *menuSurface = SDL_LoadBMP(imagePath);
    if (menuSurface == NULL) {
        printf("Unable to load menu image! SDL_Error: %s\n", SDL_GetError());
        return;
    }

    // Create texture from the surface
    SDL_Texture *menuTexture = SDL_CreateTextureFromSurface(renderer, menuSurface);
    if (menuTexture == NULL) {
        printf("Unable to create texture from menu surface! SDL_Error: %s\n", SDL_GetError());
        SDL_FreeSurface(menuSurface);
        return;
    }

    // Define the destination rectangle to fit the entire screen
    SDL_Rect destRect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

    // Render the menu image
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, menuTexture, NULL, &destRect);
    SDL_RenderPresent(renderer);

    // Free surfaces and texture
    SDL_DestroyTexture(menuTexture);
    SDL_FreeSurface(menuSurface);
}

void handleMenuEvents(SDL_Renderer *renderer) {
    bool startGame = false;
    int currentMenu = 0; // Variable to track the current menu
    SDL_Event e;

    const char *imagePaths[6] = {
        "menu_1.bmp",
        "menu_2.bmp",
        "menu_3.bmp",
        "menu_4.bmp",
        "menu_5.bmp",
        "menu_6.bmp"
    };

    while (!startGame) {
        // Render the current menu
        renderMenu(renderer, imagePaths[currentMenu]);

        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                exit(0); // Exit the program if the window is closed
            } else if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
                Mix_PlayChannel(-1,click_effect,0);
                int mouseX = e.button.x;
                int mouseY = e.button.y;

                // Check if the mouse click is within the first area
                if (currentMenu == 0 && mouseX >= 0 && mouseX <= SCREEN_WIDTH &&
                    mouseY >= 0 && mouseY <= SCREEN_HEIGHT) {
                    currentMenu = 1; // Switch to menu_2.bmp

                }
                else if (currentMenu == 2 && mouseX >= 429 && mouseX <= 429 + 300 &&
                         mouseY >= 64 && mouseY <= 64 + 100) {
                             player=2;
                         currentMenu = 5;
                    // 2player is clicked
                }
                else if (currentMenu == 5 && mouseX >= 0 && mouseX <= SCREEN_WIDTH &&
                    mouseY >= 0 && mouseY <= SCREEN_HEIGHT) {
                    startGame = true;
                }
                else if (currentMenu == 2 && mouseX >= 429 && mouseX <= 429 + 300 &&
                         mouseY >= 205 && mouseY <= 205 + 100) {
                             player=3;
                    currentMenu = 5; //  3player area is clicked
                }
                else if (currentMenu == 2 && mouseX >= 429 && mouseX <= 429 + 300 &&
                         mouseY >= 340 && mouseY <= 340 + 100) {
                             player=4;
                    currentMenu = 5; // if 4player area is clicked
                }
                else if (currentMenu == 2 && mouseX >= 429 && mouseX <= 429 + 300 &&
                         mouseY >= 476 && mouseY <= 476 + 100) {
                    currentMenu = 1 ;// back
                }
                // Check if the mouse click is within the third area
                else if (currentMenu == 1 && mouseX >= 429 && mouseX <= 429 + 300 &&
                         mouseY >= 205 && mouseY <= 205 + 100) {
                    currentMenu = 3; // Switch to menu_4.bmp
                }
                // Check if the mouse click is within the fourth area
                else if (currentMenu == 1 && mouseX >= 429 && mouseX <= 429 + 300 &&
                         mouseY >= 340 && mouseY <= 340 + 100) {
                    currentMenu = 4; // Switch to menu_5.bmp
                }
                // Check if the mouse click is within the fifth area
                else if (currentMenu == 1 && mouseX >= 429 && mouseX <= 429 + 300 &&
                         mouseY >= 476 && mouseY <= 476 + 100) {
                    closeSDL(); // Quit the game if the fifth area is clicked
                }
                else if (currentMenu == 3 && mouseX >= 65 && mouseX <= 65 + 125 &&
                         mouseY >= 556 && mouseY <= 556 + 48 ){
                    currentMenu = 1; //back
                }
                else if (currentMenu == 4 && mouseX >= 65 && mouseX <= 65 + 125 &&
                         mouseY >= 556 && mouseY <= 556 + 48 ){
                    currentMenu = 1; //back
                }
                else if (currentMenu == 1 && mouseX >= 429 && mouseX <= 429 + 300 &&
                         mouseY >= 64 && mouseY <= 64 + 100){
                    currentMenu = 2; //switch to menu_3.bmp
                }


            }else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) {
                Mix_PlayChannel(-1,click_effect,0);
                // Check if the escape key is pressed during the game
                if (currentMenu > 1) {
                    currentMenu = 1; // Return to menu_1.bmp
                }
            }
        }
    }
}





