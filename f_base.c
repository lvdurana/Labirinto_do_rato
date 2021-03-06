#include "f_base.h"

void push(stack **pilha, int dado){

    stack *tmp;
    tmp = criar_dado_pilha();
    tmp->dado = dado;
    tmp->prox = *pilha;

    *pilha = tmp;
}

int pop(stack **pilha){
    stack *aux = *pilha;
    if (*pilha != NULL)
    {
        *pilha = (*pilha)->prox;
        free(aux);
        return 1;
    }
    return 0;
}

int criar_dado_pilha(){
    return (stack*)malloc(sizeof(stack));
}

void liberar_lista(stack **list){

    stack *aux = *list;
    stack *aux2;

    while(aux != NULL){
        aux2 = aux;
        aux = aux->prox;
        free(aux2);
    }


}

int saiu(character *rato)
{
    int rato_x = GET_X_FROM_STACK(rato->pilha->dado);
    int rato_y = GET_Y_FROM_STACK(rato->pilha->dado);

    if (rato_x == 0 || rato_x == MAX_SIZE_LAB_X-1 || rato_y == 0 || rato_y == MAX_SIZE_LAB_Y-1)
        return 1;
    else
        return 0;
}


void gerar(labirinto *lab){
    int tp0,c,d;
    borda(lab);
    srand(time(NULL));
    for(c=1;c<MAX_SIZE_LAB_X-1;c++)
    for(d=1;d<MAX_SIZE_LAB_Y-1;d++)
    {
        lab->mat[c][d] = TILE_FLOOR;
        tp0 = rand() % 10;
        if (tp0 == 0)
            lab->mat[c][d] = TILE_WALL;
    }

}

void gerar2(labirinto *lab){
    int molde[11][4]={{2,2,2,2},{1,2,1,2},{2,1,2,1},{2,1,1,2},{1,1,2,2},{1,2,2,1},{2,2,1,1},{2,1,2,2},{2,2,1,1},{2,1,2,2},{1,2,2,2}};
    int c,d,e,tp0;

    //Zerar matrix
    for(c=0;c<MAX_SIZE_LAB_X;c++)
        for(d=0;d<MAX_SIZE_LAB_X;d++)
            lab->mat[c][d] = 0;

    srand(time(NULL));
    for(c=0;c<MAX_SIZE_LAB_X-1;c+=2)
    for(d=0;d<MAX_SIZE_LAB_Y-1;d+=2)
    {
        lab->mat[c][d] = TILE_WALL;
        if (c+2 < MAX_SIZE_LAB_X && d+2 < MAX_SIZE_LAB_Y)
        {

            tp0 = rand() % 11;

        /*    if(mat[c+2][d+1] == 0)
            {
                mat[c+2][d+1] = molde[tp0][0];
                printf("%d",mat[c+2][d+1]);
            }*/

            if(lab->mat[c+1][d] == 0)
            {
                lab->mat[c+1][d] = molde[tp0][1];
            }
            if(lab->mat[c][d+1] == 0)
            {
                lab->mat[c][d+1] = molde[tp0][2];
            }
            if(lab->mat[c+1][d+2] == 0)
            {
                lab->mat[c+1][d+2] = molde[tp0][3];
            }

        }
    }
//limpa os restos
    for(c=1;c<MAX_SIZE_LAB_X-1;c++)
    for(d=1;d<MAX_SIZE_LAB_Y-1;d++)
    {
        if (lab->mat[c][d] == 2)
            lab->mat[c][d] = 0;


    }
//limpa os quadradinhos solitários e os buracos
while (e)
{
    e = 0;
    for(c=1;c<MAX_SIZE_LAB_X-1;c++)
    for(d=1;d<MAX_SIZE_LAB_Y-1;d++)
    {
        if (lab->mat[c][d] == 0)
        {
            if (lab->mat[c-1][d] == 1 && lab->mat[c+1][d] == 1 && lab->mat[c][d-1] == 1 && lab->mat[c][d+1] == 1)
            {
                tp0 = rand() % 16;
                if (tp0 == 0)
                {
                    lab->mat[c+1][d] = 0;
                    e = 1;
                }

                if (tp0 == 1)
                {
                    lab->mat[c][d-1] = 0;
                    e = 1;
                }

                if (tp0 == 2)
                {
                    lab->mat[c-1][d] = 0;
                    e = 1;
                }

                if (tp0 == 3)
                {
                    lab->mat[c][d+1] = 0;
                    e = 1;
                }

            }

        }

        if (lab->mat[c][d] == 1)
        {
            if (lab->mat[c-1][d] == 0 && lab->mat[c+1][d] == 0 && lab->mat[c][d-1] == 0 && lab->mat[c][d+1] == 0)
            {
                tp0 = rand() % 16;
                if (tp0 == 0)
                {
                    lab->mat[c+1][d] = 1;
                    e = 1;
                }

                if (tp0 == 1)
                {
                    lab->mat[c][d-1] = 1;
                    e = 1;
                }

                if (tp0 == 2)
                {
                    lab->mat[c-1][d] = 1;
                    e = 1;
                }

                if (tp0 == 3)
                {
                    lab->mat[c][d+1] = 1;
                    e = 1;
                }

            }

        }
    }
}

    borda(lab);
}


int atualizar_pilha(labirinto *lab, character *rato)
{
    int dado = rato->pilha->dado;
    int rato_x = GET_X_FROM_STACK(dado);
    int rato_y = GET_Y_FROM_STACK(dado);
    int dir;

    if (lab->mat[rato_y+1][rato_x] == MAP_FLOOR)
    {
        push( &(rato->pilha) , dado+1);
        dir = DIRECTION_DOWN;
    }
    else
    {
        if (lab->mat[rato_y][rato_x+1] == MAP_FLOOR)
        {
            push(&(rato->pilha),dado+100);
            dir = DIRECTION_RIGHT;
        }
        else
        {
            if (lab->mat[rato_y-1][rato_x] == MAP_FLOOR)
            {
                push(&(rato->pilha),dado-1);
                dir = DIRECTION_UP;
            }
            else
            {
                if (lab->mat[rato_y][rato_x-1] == MAP_FLOOR)
                    {
                        push(&(rato->pilha),dado-100);
                        dir = DIRECTION_LEFT;
                    }
                    else
                    {
                        pop(&(rato->pilha));

                        if(rato->pilha == NULL){
                            printf("POP NULL");
                            //FIM DO LAB
                            return -1;
                        };


                        int diff_pos = rato->pilha->dado - dado;

                        if(diff_pos == 100)
                            dir = DIRECTION_RIGHT;
                        else
                        if(diff_pos == 1)
                            dir = DIRECTION_DOWN;
                        else
                        if(diff_pos == -1)
                            dir = DIRECTION_UP;
                        else
                            dir = DIRECTION_LEFT;

                        lab->mat[rato_y][rato_x] = MAP_DEADEND;
                        if(verificar_beco(lab,rato_x,rato_y))
                            PlaySound("rat3.wav",NULL, SND_FILENAME|SND_ASYNC|SND_NOSTOP);
                        return dir;
                    }
            }
        }
    }



    lab->mat[rato_y][rato_x] = MAP_VISITED;
    return dir;
}

int verificar_beco(labirinto *lab, int pos_x, int pos_y){
    int wall_counter = 0;
    if(lab->mat[pos_y][pos_x+1] == MAP_WALL)
        wall_counter++;
    if(lab->mat[pos_y][pos_x-1] == MAP_WALL)
        wall_counter++;
    if(lab->mat[pos_y+1][pos_x] == MAP_WALL)
        wall_counter++;
    if(lab->mat[pos_y-1][pos_x] == MAP_WALL)
        wall_counter++;

    if(wall_counter >= 3)
        return 1;
    return 0;

}


void borda(labirinto *lab){
    int c,d;
    for(c=0;c<MAX_SIZE_LAB_X;c++)
    {
        lab->mat[c][0] = TILE_WALL;
        lab->mat[c][MAX_SIZE_LAB_Y-1] = TILE_WALL;
    }

    for(d=0;d<MAX_SIZE_LAB_Y;d++)
    {
        lab->mat[0][d] = TILE_WALL;
        lab->mat[MAX_SIZE_LAB_X-1][d] = TILE_WALL;
    }
    //faz um buraco na borda
    srand(time(NULL));
    c = rand() % 2;
    if (c == 0){
        c = 1+(rand() % (MAX_SIZE_LAB_X-3));
        d = rand() % 2;
        d = d*(MAX_SIZE_LAB_Y-1);
        lab->mat[c][d] = TILE_FLOOR;
    }
    else
    {
        c = 1+(rand() % (MAX_SIZE_LAB_Y-3));
        d = rand() % 2;
        d = d*(MAX_SIZE_LAB_X-1);
        lab->mat[d][c] = TILE_FLOOR;
    }
}

