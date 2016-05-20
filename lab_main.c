#include "f_base.h"

TCHAR szClassName[] = _T("LabRato");
HWND main_window;

//Elementos do labirinto
labirinto lab;
character rato;
frame_count FPS_count;

//Elementos da interface
HBITMAP bitmaps[NUM_BITMAPS];
HWND buttons[NUM_BUTTONS];


int WINAPI WinMain (HINSTANCE hThisInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpszArgument,
                     int nCmdShow)
{
    HWND hwnd;
    MSG messages;
    WNDCLASSEX wincl;

    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;
    wincl.style = CS_DBLCLKS;
    wincl.cbSize = sizeof (WNDCLASSEX);
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = (HICON)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(LR_ICON), IMAGE_ICON, 16, 16, 0);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;//MAKEINTRESOURCE(IDR_MYMENU);
    wincl.cbClsExtra = 0;
    wincl.cbWndExtra = 0;
    wincl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;

    if (!RegisterClassEx (&wincl))
        return 0;

    hwnd = CreateWindowEx (
           0,                   /* Extended possibilites for variation */
           szClassName,         /* Classname */
           _T("Labirinto do Rato"),       /* Title Text */
           WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, /* default window */
           CW_USEDEFAULT,       /* Windows decides the position */
           CW_USEDEFAULT,       /* where the window ends up on the screen */
           WINDOW_WIDTH,                 /* The programs width */
           WINDOW_HEIGHT,                 /* and height in pixels */
           HWND_DESKTOP,        /* The window is a child-window to desktop */
           NULL,                /* No menu */
           hThisInstance,       /* Program Instance handler */
           NULL                 /* No Window Creation data */
           );

    ShowWindow (hwnd, nCmdShow);
    main_window = hwnd;

    //Loop principal
    while (GetMessage (&messages, NULL, 0, 0))
    {
        TranslateMessage(&messages);
        DispatchMessage(&messages);
    }

};

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{



    switch (message)
    {
        case WM_CREATE:
            {
                bitmaps[SPRITE_TILES] = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(LR_BMP_TILES));
                bitmaps[SPRITE_RATO] = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(LR_BMP_RATO));
                bitmaps[SPRITE_RATO_MASK] = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(LR_BMP_RATO));
                bitmaps[SPRITE_RATO_P] = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(LR_BMP_RATO_PARADO));
                bitmaps[SPRITE_RATO_P_MASK] = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(LR_BMP_RATO_PARADO));

                CreateBitmapMask(bitmaps[SPRITE_RATO_MASK],RGB(255,255,255));
                CreateBitmapMask(bitmaps[SPRITE_RATO_P_MASK],RGB(255,255,255));


                inicializar_labirinto(&lab,&rato,FALSE);

                criar_botoes(hwnd, buttons);

                inicializar_contador_de_frames(hwnd, &FPS_count);
                UpdateWindow(hwnd);
            };
            break;

        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);


            desenhar_labirinto(hwnd,hdc,&lab,&rato, &bitmaps , 0,0);

            EndPaint(hwnd, &ps);

        }
        break;
        case WM_COMMAND:
            switch(LOWORD(wParam))
            {
                case BN_CLICKED:
                    if(1){
                        verificar_botao_pressionado(hwnd,lParam,&lab,&rato,buttons);
                        InvalidateRect(hwnd,NULL,1);
                        UpdateWindow(hwnd);
                    };

                break;
            }
            break;
        case WM_LBUTTONDOWN:
            {

                //rato.active ^= 1;
            }
        break;

        case WM_TIMER:
            switch(wParam){

                case FPS_TIMER:
                    update(&lab,&rato);
                    atualizar_botoes_permitidos(buttons,&lab,&rato);
                    InvalidateRect(hwnd,NULL,1);
                    UpdateWindow(hwnd);
                    atualizar_frame(hwnd, &FPS_count);
                break;
            break;
            }
        break;
        case WM_ERASEBKGND:
            return (LRESULT)1;

        case WM_DESTROY:
            PostQuitMessage (0);
            break;
        default:
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
};
