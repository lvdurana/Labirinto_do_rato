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

#define WINDOW_WIDTH (MAX_SIZE_LAB_X * SIZE_CELL_X) + 7
#define WINDOW_HEIGHT (MAX_SIZE_LAB_Y * SIZE_CELL_Y) + 28

//Especificação dos tiles no bitmap

#define TILE_FLOOR 0
#define TILE_WALL 1
#define TILE_DEADEND 2
#define TILE_EXIT 3
#define TILE_BITMAP_POSITION_X(x) (x*SIZE_CELL_X)

#define MAX_FPS 30
#define FPS_TIMER 0x0001




typedef struct lab {
    int mat[MAX_SIZE_LAB_Y][MAX_SIZE_LAB_X];


} labirinto;

typedef struct frame{
    int counter;
    int

} frame_count;

void desenhar_labirinto(HWND hwnd, HDC hdc, labirinto *lab, int pos_x, int pos_y);
int inicializar_labirinto(labirinto *lab);
