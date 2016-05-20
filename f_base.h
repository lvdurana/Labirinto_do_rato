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
#define WINDOW_HEIGHT ((MAX_SIZE_LAB_Y * SIZE_CELL_Y) + WINDOW_PADDING_Y) + 36

//Sprites
#define SPRITE_TILES 0
#define SPRITE_RATO 1
#define SPRITE_RATO_MASK 2
#define SPRITE_RATO_P 3
#define SPRITE_RATO_P_MASK 4
#define SPRITE_LEGENDA 5
#define NUM_BITMAPS 6

//Botões
#define NUM_BUTTONS 7

#define PAUSE_BUTTON 0
#define SPEED_LOW_BUTTON 1
#define SPEED_MID_BUTTON 2
#define SPEED_HIGH_BUTTON 3
#define SPEED_MAX_BUTTON 4
#define OPTIONS_BUTTON 5
#define RESET_BUTTON 6


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

#define INITIAL_POSITION_X 1
#define INITIAL_POSITION_Y 1
#define INITIAL_POSITION (INITIAL_POSITION_X*100 + INITIAL_POSITION_Y)

#define STATUS_ACTIVE 0
#define STATUS_EXIT 1
#define STATUS_NOEXIT 2

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
    int status;


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
    BOOL active; // Se o rato deve se mover
    BOOL animation; //Se o rato deve executar a animação. Se estiver parado, o sprite de parado é utilizado

} character;


void push(stack **pilha, int dado);
int pop(stack **pilha);
int criar_dado_pilha();
void liberar_lista(stack **list);
int saiu(character *rato);
void gerar(labirinto *lab);
void gerar2(labirinto *lab);
int atualizar_pilha(labirinto *lab, character *rato);
int verificar_beco(labirinto *lab, int pos_x, int pos_y);
void borda(labirinto *lab);

int inicializar_contador_de_frames(HWND hwnd, frame_count *frame);
int atualizar_frame(HWND hwnd, frame_count *frame);
int inicializar_rato(character *rato);
int inicializar_labirinto(labirinto *lab, character *rato, int mode);
int atualizar_movimento_rato(character *rato);
int verificar_movimentacao(character *rato);
int atualizar_IA_rato(labirinto *lab, character *rato);
int update(labirinto *lab, character *rato);
void desenhar_labirinto(HWND hwnd, HDC hdc, labirinto *lab, character *rato, HBITMAP *bitmaps, int pos_x, int pos_y);
HBITMAP CreateBitmapMask(HBITMAP hbmColour, COLORREF crTransparent);
void desenhar_texto(HDC hdc, char *text, int size, int pos_x, int pos_y, int limit_x, int modifier);
void criar_botoes(HWND hwnd, HWND *buttons);
int verificar_botao_pressionado(HWND hwnd, HWND pressed, labirinto *lab, character *rato, HWND *buttons);

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

