#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif

#include <tchar.h>
#include <windows.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#include "resource.h"

#define MAX_SIZE_LAB_X 30
#define MAX_SIZE_LAB_Y 30
#define SIZE_CELL_X 16
#define SIZE_CELL_Y 16


#define LAB_WIDTH (MAX_SIZE_LAB_X * SIZE_CELL_X)
#define LAB_HEIGHT (MAX_SIZE_LAB_Y * SIZE_CELL_Y)
#define WINDOW_PADDING_X 7
#define WINDOW_PADDING_Y 28

#define WINDOW_WIDTH ((MAX_SIZE_LAB_X * SIZE_CELL_X) + WINDOW_PADDING_X)
#define WINDOW_HEIGHT ((MAX_SIZE_LAB_Y * SIZE_CELL_Y) + WINDOW_PADDING_Y) + 64


//Botões
#define NUM_BUTTONS 6

#define PAUSE_BUTTON 0
#define SPEED_LOW_BUTTON 1
#define SPEED_MID_BUTTON 2
#define SPEED_HIGH_BUTTON 3
#define OPTIONS_BUTTON 4
#define RESET_BUTTON 5


#define SPEED_BUTTONS_WIDTH 48
#define SPEED_BUTTONS_HEIGHT 32
#define SPEED_BUTTONS_X WINDOW_PADDING_X
#define SPEED_BUTTONS_Y (LAB_HEIGHT + 4)
#define SPEED_BUTTONS_PADDING 10

#define CONTROL_BUTTONS_WIDTH 64
#define CONTROL_BUTTONS_HEIGHT 32
#define CONTROL_BUTTONS_Y (LAB_HEIGHT + 4)
#define CONTROL_BUTTONS_PADDING 10

#define RESET_BUTTON_X (LAB_WIDTH - CONTROL_BUTTONS_WIDTH)
#define OPTIONS_BUTTON_X (RESET_BUTTON_X - CONTROL_BUTTONS_PADDING - CONTROL_BUTTONS_WIDTH)






//Especificação dos tiles no bitmap
#define TILE_FLOOR 0
#define TILE_WALL 1
#define TILE_DEADEND 2
#define TILE_EXIT 3
#define TILE_BITMAP_POSITION_X(x) (x*SIZE_CELL_X)

//Especificação dos tipos de terreno
#define MAP_FLOOR 0
#define MAP_WALL 1
#define MAP_VISITED 2
#define MAP_DEADEND 3

//Especificação das direções dos sprites
#define DIRECTION_UP 0
#define DIRECTION_RIGHT 1
#define DIRECTION_LEFT 2
#define DIRECTION_DOWN 3
#define REVERSE_DIRECTION(x) ((~x) & 3)

#define SPEED_LOW 1
#define SPEED_MID 2
#define SPEED_HIGH 4
#define SPEED_MAX 16

#define GET_X_FROM_STACK(x) (x/100)
#define GET_Y_FROM_STACK(x) (x%100)

#define FRAME_DURATION(x) (8/x)

#define MAX_FPS 30
#define FPS_TIMER 0x0001



typedef struct STACK
{
    int dado;
    struct STACK *prox;
} stack;

typedef struct lab {
    int mat[MAX_SIZE_LAB_Y][MAX_SIZE_LAB_X];


} labirinto;

typedef struct frame{
    int counter;
    clock_t last_frame;
    clock_t clocks_per_frame;

} frame_count;

typedef struct cha{
    POINT pos; //em pixel
    POINT pos_map; //em coordenadas do mapa
    int movement; //pizels de distância do destino;
    int direction; //direção a qual o sprite está virado
    int speed; //pixels percorridos por frame;
    int frame; // frame da animação do sprite
    int frame_duration; //numero de frames restantes até a alteração do frame
    stack *pilha;
    BOOL active;

} character;


void push(stack **pilha, int dado);
int pop(stack **pilha);
int criar_dado_pilha();
int atualizar_pilha(labirinto *lab, character *rato);
void borda(labirinto *lab);


