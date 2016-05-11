#include "f_base.h"

int inicializar_contador_de_frames(HWND hwnd, frame_count *frame){
    frame->counter = 0;
    frame->last_frame = clock();
    frame->clocks_per_frame = CLOCKS_PER_SEC / MAX_FPS;
    SetTimer(hwnd,FPS_TIMER ,frame->clocks_per_frame,(TIMERPROC)NULL);

}

int atualizar_frame(HWND hwnd, frame_count *frame){
    clock_t cur_time, time_diff;
    cur_time = clock();
    time_diff = frame->clocks_per_frame - (cur_time - frame->last_frame);
    time_diff = time_diff < 0 ? 0 : time_diff;
    frame->last_frame = cur_time;

    //printf("%d\n", time_diff);
    SetTimer(hwnd,FPS_TIMER ,time_diff ,(TIMERPROC)NULL);


}

int inicializar_rato(character *rato){
    rato->pos.x = 16;
    rato->pos.y = 16;
    rato->pos_map.x = 0;
    rato->pos_map.y = 0;
    rato->movement = 0;
    rato->speed = SPEED_LOW;
    rato->frame = 0;
    rato->frame_duration = FRAME_DURATION(SPEED_LOW);
    rato->pilha = criar_dado_pilha();
    rato->pilha->dado = 101;
    rato->pilha->prox = NULL;
    rato->active = FALSE;

}

int inicializar_labirinto(labirinto *lab, character *rato){
    gerar(lab);

}

int atualizar_movimento_rato(character *rato){

    int move;
    //Executar movimentação
    if(rato->movement){
        move = (rato->speed > rato->movement) ? rato->movement : rato->speed;
        switch(rato->direction){
        case DIRECTION_UP:
            rato->pos.y -= move;
            break;
        case DIRECTION_DOWN:
            rato->pos.y += move;
            break;
        case DIRECTION_RIGHT:
            rato->pos.x += move;
            break;
        case DIRECTION_LEFT:
            rato->pos.x -= move;
            break;
        }

        rato->movement -= move;

    }

    //Atualizar frame
    rato->frame_duration--;
    if(!rato->frame_duration){
        rato->frame |= 1;
        rato->frame_duration = FRAME_DURATION(rato->speed);
    }

}

int verificar_movimentacao(character *rato){
    if(rato->active && !rato->movement )
        return 1;
    return 0;

}

int atualizar_IA_rato(labirinto *lab, character *rato){
    int dir = atualizar_pilha(lab,rato);
    rato->direction = dir;
    //printf("%d\n",rato->direction);
    switch(dir){
        case DIRECTION_UP:
        case DIRECTION_DOWN:
            rato->movement = SIZE_CELL_Y;
            break;
        case DIRECTION_RIGHT:
        case DIRECTION_LEFT:
            rato->movement = SIZE_CELL_X;
            break;

    }

}

int update(labirinto *lab, character *rato){
    atualizar_movimento_rato(rato);
    if(verificar_movimentacao(rato)){;

        atualizar_IA_rato(lab,rato);
    }

}


void desenhar_labirinto(HWND hwnd, HDC hdc, labirinto *lab, character *rato, int pos_x, int pos_y){

    HWND mat_base = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(LR_BMP_TILES));

    HDC hdcMem = CreateCompatibleDC(hdc);
    HBITMAP hbmOld = SelectObject(hdcMem, mat_base);
    int i,j;

    //Desenhar labirinto
    for(i=0;i<MAX_SIZE_LAB_Y;i++){
        for(j=0;j<MAX_SIZE_LAB_X;j++){
            BitBlt(hdc, pos_x+j*SIZE_CELL_X, pos_y+i*SIZE_CELL_Y, SIZE_CELL_X, SIZE_CELL_Y, hdcMem, TILE_BITMAP_POSITION_X(lab->mat[i][j]), 0, SRCCOPY);
            };
        };
    BitBlt(hdc, pos_x+rato->pos.x, pos_y+rato->pos.y, SIZE_CELL_X, SIZE_CELL_Y, hdcMem, 48, 0, SRCCOPY);


    SelectObject(hdcMem, hbmOld);

    DeleteDC(hdcMem);




};

int desenhar_rato(HWND hwnd, HDC hdc,character *rato,int pos_x, int pos_y){

    HWND mat_base = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(LR_BMP_RATO));
    if(mat_base == NULL)
        MessageBox(hwnd, "Could not load SM_BMP_MATRIZ!", "Error", MB_OK | MB_ICONEXCLAMATION);

    HDC hdcMem = CreateCompatibleDC(hdc);
    HBITMAP hbmOld = (HBITMAP)SelectObject(hdcMem, mat_base);

    //Desenhar labirinto

    BitBlt(hdc, pos_x+rato->pos.x, pos_y+rato->pos.y, SIZE_CELL_X, SIZE_CELL_Y, hdcMem, 0, 0, SRCPAINT);

    SelectObject(hdcMem, hbmOld);
    DeleteDC(hdcMem);


}


