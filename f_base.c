#include "f_base.h"

#define TAM_X 30
#define TAM_Y 30



//0 = nada
//1 = parede
//2 = visitado
//3 = beco
//5 = carinha feliz :3

int mat[TAM_X][TAM_Y];

typedef struct STACK
{
    int dado;
    struct STACK *prox;
} stack;

rato_x = 2;
rato_y = 2;

stack *pilha;


void inicia(){
    pilha = NULL;
}

void push(int x,int y){
    stack *tmp;
    tmp = novo();
    tmp->dado = x*100+y;
    tmp->prox = pilha;
    pilha = tmp;
}

void pop(){
    int dado;
    if (pilha != NULL)
    {
        dado = pilha->dado;
        pilha = pilha->prox;
        rato_x = dado / 100;
        rato_y = dado % 100;
    }

}

int novo(){
    return malloc(sizeof(stack));
}

int saiu()
{
    if (rato_x == 0 || rato_x == TAM_X-1 || rato_y == 0 || rato_y == TAM_Y-1)
        return 1;
    else
        return 0;
}


void imprimir(){
    int c,d;

    for(d=0;d<TAM_Y;d++)
    {
        printf("\n");
        for(c=0;c<TAM_X;c++)
        {
            if (mat[c][d] == 0)//nada
                printf(" ");
            if (mat[c][d] == 1)//parede
                printf("%c",219);
            if (mat[c][d] == 2)//visitado
                printf("\177");
            if (mat[c][d] == 3)//beco
                printf("%c",1);
            if (mat[c][d] == 5)//rato
                printf("\207");
        }
    }


}


void gerar(labirinto *lab){
    int tp0,c,d;
    borda(lab);
    srand(time(NULL));
    for(c=1;c<TAM_X-1;c++)
    for(d=1;d<TAM_Y-1;d++)
    {
        lab->mat[c][d] = TILE_FLOOR;
        tp0 = rand() % 10;
        if (tp0 == 0)
            lab->mat[c][d] = TILE_WALL;
    }

}

void atualizar(){
    int c,d;

    for(d=0;d<TAM_Y;d++)
        for(c=0;c<TAM_X;c++)
        {
            if (mat[c][d] == 5)
            {
                mat[c][d] = 3;
            }
        }


    mat[rato_x][rato_y] = 5;
}

void mover()
{
    if (mat[rato_x+1][rato_y] == 0)
    {
        push(rato_x,rato_y);
        rato_x++;
    }
    else
    {
        if (mat[rato_x][rato_y+1] == 0)
        {
            push(rato_x,rato_y);
            rato_y++;
        }
        else
        {
            if (mat[rato_x-1][rato_y] == 0)
            {
                push(rato_x,rato_y);
                rato_x--;
            }
            else
            {
                if (mat[rato_x][rato_y-1] == 0)
                    {
                        push(rato_x,rato_y);
                        rato_y--;
                    }
                    else
                    {
                        pop();
                    }
            }
        }
    }
}


void borda(labirinto *lab){
    int c,d;
    for(c=0;c<TAM_X;c++)
    {
        lab->mat[c][0] = TILE_WALL;
        lab->mat[c][TAM_Y-1] = TILE_WALL;
    }

    for(d=0;d<TAM_Y;d++)
    {
        lab->mat[0][d] = TILE_WALL;
        lab->mat[TAM_X-1][d] = TILE_WALL;
    }
    //faz um buraco na borda
    srand(time(NULL));
    c = rand() % 2;
    if (c == 0){
        c = 1+(rand() % (TAM_X-3));
        d = rand() % 2;
        d = d*(TAM_Y-1);
        lab->mat[c][d] = TILE_FLOOR;
    }
    else
    {
        c = 1+(rand() % (TAM_Y-3));
        d = rand() % 2;
        d = d*(TAM_X-1);
        lab->mat[d][c] = TILE_FLOOR;
    }
}


/*void main(){
    gerar();
    rato_x = 2;
    rato_y = 2;
    //pilha
    inicia();
    while(1)
    {
        mover();
        atualizar();
        imprimir();
        //getch();
        if (saiu())
            break;
        system("cls");

    }
    printf("*yay*");

}*/
