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
    rato->pos.x = INITIAL_POSITION_X*SIZE_CELL_X;
    rato->pos.y = INITIAL_POSITION_Y*SIZE_CELL_Y;
    rato->pos_map.x = 0;
    rato->pos_map.y = 0;
    rato->movement = 0;
    rato->direction = DIRECTION_DOWN;
    rato->speed = SPEED_LOW;
    rato->frame = 0;
    rato->frame_duration = FRAME_DURATION(SPEED_LOW);
    rato->pilha = criar_dado_pilha();
    rato->pilha->dado = INITIAL_POSITION;
    rato->pilha->prox = NULL;
    rato->active = FALSE;
    rato->animation = TRUE;

}

int inicializar_labirinto(labirinto *lab, character *rato, int mode){

    inicializar_rato(rato);
    if(mode)
        gerar(lab);
    else
        gerar2(lab);

}

int atualizar_movimento_rato(character *rato){

    int move;
    //Executar movimentação
    if(rato->movement && rato->active){
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
    if(rato->active || rato->animation){
        rato->frame_duration--;
        if(!(rato->frame_duration)){
            rato->frame ^= 1;
            rato->frame_duration = FRAME_DURATION(rato->speed);
        }
    };

}

int verificar_movimentacao(character *rato){
    if(rato->active && !rato->movement )
        return 1;
    return 0;

}

int atualizar_IA_rato(labirinto *lab, character *rato){

    //Verificar se o rato saiu do labirinto
    if(saiu(rato)){
        rato->active = FALSE;
        printf("\nSAIU");
        return 1;
    };

    //Decidir próximo movimentp
    int dir = atualizar_pilha(lab,rato);

    //Verificar se o rato voltou para o começo do labirinto
    if(dir < 0){
        rato->active = FALSE;
        printf("\nSEM SAIDA");
        return 1;
    };

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
    return 0;

}

int update(labirinto *lab, character *rato){
        atualizar_movimento_rato(rato);
        if(verificar_movimentacao(rato)){


            atualizar_IA_rato(lab,rato);
        }
        return 0;
    ;

}


void desenhar_labirinto(HWND hwnd, HDC hdc, labirinto *lab, character *rato, HBITMAP *bitmaps, int pos_x, int pos_y){

    HBITMAP hMemBitmap = CreateCompatibleBitmap(hdc, LAB_WIDTH, LAB_HEIGHT);
    HDC tiles= CreateCompatibleDC(hdc);
    SelectObject(tiles,bitmaps[SPRITE_TILES]);
    HDC hdcMem = CreateCompatibleDC(hdc);
    SelectObject(hdcMem, hMemBitmap);

    int i,j;

    for(i=0;i<MAX_SIZE_LAB_Y;i++){
        for(j=0;j<MAX_SIZE_LAB_X;j++){
            BitBlt(hdcMem, pos_x+j*SIZE_CELL_X, pos_y+i*SIZE_CELL_Y, SIZE_CELL_X, SIZE_CELL_Y, tiles, TILE_BITMAP_POSITION_X(lab->mat[i][j]), 0, SRCCOPY);
        };
    };

    if(rato->animation){
        SelectObject(tiles,bitmaps[SPRITE_RATO_P]);
        BitBlt(hdcMem, pos_x+rato->pos.x, pos_y+rato->pos.y, SIZE_CELL_X, SIZE_CELL_Y, tiles, (rato->frame * 16), 0, SRCAND);

        SelectObject(tiles,bitmaps[SPRITE_RATO_P_MASK]);
        BitBlt(hdcMem, pos_x+rato->pos.x, pos_y+rato->pos.y, SIZE_CELL_X, SIZE_CELL_Y, tiles, (rato->frame * 16), 0, SRCPAINT);
    }
    else{
        SelectObject(tiles,bitmaps[SPRITE_RATO]);
        BitBlt(hdcMem, pos_x+rato->pos.x, pos_y+rato->pos.y, SIZE_CELL_X, SIZE_CELL_Y, tiles, (rato->frame * 16), (rato->direction*16), SRCAND);

        SelectObject(tiles,bitmaps[SPRITE_RATO_MASK]);
        BitBlt(hdcMem, pos_x+rato->pos.x, pos_y+rato->pos.y, SIZE_CELL_X, SIZE_CELL_Y, tiles, (rato->frame * 16), (rato->direction*16), SRCPAINT);

    };

    BitBlt(hdc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, hdcMem, 0, 0, SRCCOPY);



    //SelectObject(hdcMem, hbmOld);

    DeleteObject(hMemBitmap);

    DeleteDC(tiles);
    DeleteDC(hdcMem);

};


HBITMAP CreateBitmapMask(HBITMAP hbmColour, COLORREF crTransparent)
{
    HDC hdcMem, hdcMem2;
    HBITMAP hbmMask;
    BITMAP bm;

    GetObject(hbmColour, sizeof(BITMAP), &bm);
    hbmMask = CreateBitmap(bm.bmWidth, bm.bmHeight, 1, 1, NULL);

    hdcMem = CreateCompatibleDC(0);
    hdcMem2 = CreateCompatibleDC(0);

    SelectObject(hdcMem, hbmColour);
    SelectObject(hdcMem2, hbmMask);

    SetBkColor(hdcMem, crTransparent);

    BitBlt(hdcMem2, 0, 0, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCCOPY);

    BitBlt(hdcMem, 0, 0, bm.bmWidth, bm.bmHeight, hdcMem2, 0, 0, SRCINVERT);

    DeleteDC(hdcMem);
    DeleteDC(hdcMem2);

    return hbmMask;
}

void criar_botoes(HWND hwnd, HWND *buttons){
    //Estabelecer posições origem
    int x_pos = SPEED_BUTTONS_X;
    int y_pos = SPEED_BUTTONS_Y;


    buttons[PAUSE_BUTTON] = CreateWindow("BUTTON", "||",WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        x_pos, y_pos, SPEED_BUTTONS_WIDTH, SPEED_BUTTONS_HEIGHT,
        hwnd, NULL, GetModuleHandle(NULL), NULL);
    x_pos += SPEED_BUTTONS_WIDTH + SPEED_BUTTONS_PADDING;

    buttons[SPEED_LOW_BUTTON] = CreateWindow("BUTTON", "1",WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        x_pos, y_pos, SPEED_BUTTONS_WIDTH, SPEED_BUTTONS_HEIGHT,
        hwnd, NULL, GetModuleHandle(NULL), NULL);
    x_pos += SPEED_BUTTONS_WIDTH + SPEED_BUTTONS_PADDING;

    buttons[SPEED_MID_BUTTON] = CreateWindow("BUTTON", "2",WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        x_pos, y_pos, SPEED_BUTTONS_WIDTH, SPEED_BUTTONS_HEIGHT,
        hwnd, NULL, GetModuleHandle(NULL), NULL);
    x_pos += SPEED_BUTTONS_WIDTH + SPEED_BUTTONS_PADDING;

    buttons[SPEED_HIGH_BUTTON] = CreateWindow("BUTTON", "3",WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        x_pos, y_pos, SPEED_BUTTONS_WIDTH, SPEED_BUTTONS_HEIGHT,
        hwnd, NULL, GetModuleHandle(NULL), NULL);

    buttons[OPTIONS_BUTTON] = CreateWindow("BUTTON", "Opções",WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        OPTIONS_BUTTON_X, CONTROL_BUTTONS_Y, CONTROL_BUTTONS_WIDTH, CONTROL_BUTTONS_HEIGHT,
        hwnd, NULL, GetModuleHandle(NULL), NULL);

    buttons[RESET_BUTTON] = CreateWindow("BUTTON", "Resetar",WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        RESET_BUTTON_X, CONTROL_BUTTONS_Y, CONTROL_BUTTONS_WIDTH, CONTROL_BUTTONS_HEIGHT,
        hwnd, NULL, GetModuleHandle(NULL), NULL);


};

int verificar_botao_pressionado(HWND hwnd, HWND pressed, labirinto *lab, character *rato, HWND *buttons){

    if(pressed == buttons[PAUSE_BUTTON]){
        rato->active = FALSE;
    }
    if(pressed == buttons[SPEED_LOW_BUTTON]){
        rato->active = TRUE;
        rato->animation = FALSE;
        rato->speed = SPEED_LOW;
    }
    if(pressed == buttons[SPEED_MID_BUTTON]){
        rato->active = TRUE;
        rato->speed = SPEED_MID;
        rato->animation = FALSE;
    }
    if(pressed == buttons[SPEED_HIGH_BUTTON]){
        rato->active = TRUE;
        rato->speed = SPEED_HIGH;
        rato->animation = FALSE;
    }
    if(pressed == buttons[RESET_BUTTON]){
        inicializar_labirinto(lab,rato,TRUE);
    }



}

