#include "f_base.h"

void desenhar_labirinto(HWND hwnd, HDC hdc, labirinto *lab, int pos_x, int pos_y){

    HWND mat_base = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(LR_BMP_TILES));
    if(mat_base == NULL)
        MessageBox(hwnd, "Could not load SM_BMP_MATRIZ!", "Error", MB_OK | MB_ICONEXCLAMATION);

    HDC hdcMem = CreateCompatibleDC(hdc);
    HBITMAP hbmOld = (HBITMAP)SelectObject(hdcMem, mat_base);
    int i,j;

    //Desenhar labirinto
    for(i=0;i<MAX_SIZE_LAB_Y;i++){
        for(j=0;j<MAX_SIZE_LAB_X;j++){
            BitBlt(hdc, pos_x+j*SIZE_CELL_X, pos_y+i*SIZE_CELL_Y, SIZE_CELL_X, SIZE_CELL_Y, hdcMem, TILE_BITMAP_POSITION_X(lab->mat[i][j]), 0, SRCCOPY);
            }
        }
    }
    BitBlt(hdc, pos_x, pos_y, M_FIELD_WIDTH, M_FIELD_HEIGHT, hdcMem, 0, 0, SRCCOPY);

    SelectObject(hdcMem, hbmOld);
    DeleteDC(hdcMem);




};

int inicializar labirinto(labirinto *lab){
    int i,j;

    //Desenhar labirinto
    for(i=0;i<MAX_SIZE_LAB_Y;i++){
        for(j=0;j<MAX_SIZE_LAB_X;j++){
            //zerar lab
        };
    };


}
