#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <conio.h>

typedef struct _node {

    char musica[50];
    struct _node *prev;
    struct _node *next;

} Node;


typedef struct _linked_list {

    Node *begin;
    Node *end;
    int cont;

} LinkedList;



Node *Node_create(char nome[50]) {
    Node *node = (Node*) calloc(1, sizeof(Node));
    strcpy(node->musica, nome);
    node->prev = NULL;
    node->next = NULL;
    return node;
}



LinkedList *LinkedList_create() {

    LinkedList *L = (LinkedList *) calloc(1, sizeof(LinkedList));
    L->begin = NULL;
    L->end = NULL;
    L->cont = 0;
    return L;
}

void LinkedList_add_first(LinkedList *L, char nome[50]) {

    Node *p = Node_create(nome);

    if (L->begin == NULL){   //se a lista esta vazia
        L->begin = p;
        L->end = p;
        p->next = p;
        p->prev = p;
    } else {
        p->next = L->begin;
        p->prev = L->end;
        L->begin->prev = p;
        L->begin = p;
        L->end->next = p;
    }

    L->cont++;
}

void LinkedList_add_last(LinkedList *L, char nome[50]){
    Node *p = Node_create(nome);

    if(L->begin != NULL){ // lista nao vazia
        L->end->next = p;
        p->prev = L->end;
        p->next = L->begin;
        L->end = p;
        L->begin->prev = p;
    }else{
        L->begin = p;
        L->end = p;
        p->next = p;
        p->prev = p;
    }

    L->cont++;

}
void LinkedList_remove(LinkedList *L, char nome[50]){

    Node *ante = NULL;
    Node *pos = NULL;

    if(L->begin != NULL){ //lista  nao vazia
        if(strcmp(L->begin->musica, nome) == 0){ // primeiro node
            if(L->cont == 1){ // unico node
                pos = L->begin;
                free(pos);
                L->begin = NULL;
                L->end = NULL;
            }else{ //remover primeiro elemento
                pos = L->begin;
                L->begin = pos->next;
                L->begin->prev = pos->prev;
                free(pos);
            }
            L->cont--;
        }else{ //remover elemento no meio/final
            pos = L->begin->next;
            ante = L->begin;
            int contador = 0;
            while(contador < L->cont && strcmp(pos->musica, nome) != 0){
                ante = pos;
                pos = pos->next;
                contador++;
            }
            if(contador < L->cont){
                if(contador != L->cont){ // nao eh o ultimo elemento
                    ante->next = pos->next;
                    pos->next->prev = ante;
                    free(pos);
                }else{ // eh o ultimo
                    ante->next = L->begin;
                    free(pos);
                    L->end = ante;
                }
                L->cont--;
            }
        }
    }
}

//funcoes uteis

int LinkedList_numero_elem(LinkedList *L){
    return (L->cont);
}

void LinkedList_copia(LinkedList *L, LinkedList *M){
    Node *p = L->begin;
    int contador = 0;

    while(contador < L->cont){
        LinkedList_add_last(M, p->musica);
        p = p->next;
        contador++;
    }
}

void LinkedList_copia_invertida(LinkedList *L, LinkedList *M){
    Node *p = L->begin;
    int contador = 0;

    while(contador < L->cont){
        LinkedList_add_first(M, p->musica);
        p = p->next;
        contador++;
    }
}

void LinkedList_print(LinkedList *L) {
    Node *p = L->begin;
    int contador = 0;
    printf("Inicio -> ");

    while (contador < L->cont) {
        printf("%s -> ", p->musica);
        p = p->next;
        contador++;
    }
    printf(" Fim da Fila");
}

//funcoes do radio
void LinkedList_shuffle_radio(LinkedList *R){
    LinkedList *L = LinkedList_create();
    LinkedList *Result = LinkedList_create();
    LinkedList_copia_invertida(R, L);

    Node *pR = R->begin;
    Node *pL = L->begin;
    Node *pResult = NULL;

    int contador = R->cont;
    int metadeCont = contador / 2;

    for(int i = 0; i < metadeCont; i++){
        LinkedList_add_last(Result, pR->musica);
        LinkedList_add_last(Result, pL->musica);
        pR = pR->next;
        pL = pL->next;
    }

    //adiciona o elemento do meio se for impar
    if(contador % 2 != 0){
        LinkedList_add_last(Result, pR->musica);
    }

    R->begin = Result->begin;
    R->end = Result->end;
    R->cont = Result->cont;

    free(L);
    free(Result);
}

Node* musica_selecionada_por_numero(LinkedList *R, Node *p, int num){
    if(num > R->cont){
        printf("Nao existe musica nessa posicao\nPermanecendo na musica atual\n");
        return p;
    }else{
        int contador = 1;
        p = R->begin;
        while(contador != num){
            p = p->next;
            contador++;
        }

        return p;
    }
}

int posicao_musica(LinkedList *R, char nome[50]){
    Node *p = R->begin;
    int pos = 1;

    while(strcmp(p->musica, nome) != 0){
        pos++;
        p = p->next;
    }

    return pos;
}

Node* encontrar_musica(LinkedList *R, char nome[50]){
    Node *p = R->begin;

    while(strcmp(p->musica, nome) != 0){
        p = p->next;
    }

    return p;
}


int main (){

    LinkedList *Radio = LinkedList_create();

    LinkedList_add_first(Radio, "19-2000");
    LinkedList_add_first(Radio, "Clint Eastwood");
    LinkedList_add_first(Radio, "Feel Good Inc.");
    LinkedList_add_first(Radio, "Rhinestone Eyes");
    LinkedList_add_first(Radio, "El manana");

    int opcao = -1;
    int posicao;
    int verificaLoop = 0;
    int cont = 0;
    Node *musicaAtual = Radio->begin;
    char nomeAtual[50];
    time_t ultimoTempo = time(NULL);

    printf("****RADIO FM**** \n\n");
    printf("Musica Atual: %d - %s\n\n",posicao_musica(Radio, musicaAtual->musica), musicaAtual->musica);

    if(musicaAtual == NULL){
        printf("Radio sem musicas");
        opcao = 0;
    }

    while(opcao != 0){

        time_t tempoAtual = time(NULL);

        if(difftime(tempoAtual, ultimoTempo) >= 10){
            if(verificaLoop == 0){
                musicaAtual = musicaAtual->next;
                printf("\nTroca automatica -> %d - %s\n\n",posicao_musica(Radio, musicaAtual->musica), musicaAtual->musica);
                cont = 0;
            }else{
                printf("\nMusica em looping: %d - %s\n\n",posicao_musica(Radio, musicaAtual->musica), musicaAtual->musica);
                cont = 0;
            }


            ultimoTempo = tempoAtual;
        }

        if(cont == 0){
            printf("Opcao: ");
            cont++;
        }

        if(kbhit()){
            scanf("%d", &opcao);


            switch(opcao){
                case 1:
                    printf("Musica Atual: %d - %s",posicao_musica(Radio, musicaAtual->musica), musicaAtual->musica);
                    break;
                case 2:
                    if(verificaLoop == 0){
                        musicaAtual = musicaAtual->next;
                    }
                    printf("Musica Atual: %d - %s",posicao_musica(Radio, musicaAtual->musica), musicaAtual->musica);
                    break;
                case 3:
                    if(verificaLoop == 0){
                        musicaAtual = musicaAtual->prev;
                    }
                    printf("Musica Atual: %d - %s",posicao_musica(Radio, musicaAtual->musica), musicaAtual->musica);
                    break;
                case 4:
                    Radio->begin = musicaAtual;
                    Radio->end = musicaAtual->prev;
                    printf("Novo inicio da fila\n");
                    LinkedList_print(Radio);
                    break;
                case 5:
                    if(verificaLoop == 0){
                        printf("Musica em looping: %d - %s",posicao_musica(Radio, musicaAtual->musica), musicaAtual->musica);
                        verificaLoop = 1;
                    }else{
                        printf("Saindo do looping...");
                        verificaLoop = 0;
                    }
                    break;
                case 6:
                    strcpy(nomeAtual, musicaAtual->musica);
                    LinkedList_shuffle_radio(Radio);
                    musicaAtual = encontrar_musica(Radio, nomeAtual);
                    printf("Fila embaralhada \nNova ordem: ");
                    LinkedList_print(Radio);
                    verificaLoop = 0;
                    break;
                case 7:
                    printf("Posicao da musica: ");
                    scanf("%d", &posicao);
                    musicaAtual = musica_selecionada_por_numero(Radio, musicaAtual, posicao);
                    printf("Musica selecionada: %d - %s",posicao_musica(Radio, musicaAtual->musica), musicaAtual->musica);
                    verificaLoop = 0;
                    break;
                case 8:
                    musicaAtual = Radio->begin;
                    printf("Indo para a musica inicial: %d - %s",posicao_musica(Radio, musicaAtual->musica), musicaAtual->musica);
                    verificaLoop = 0;
                    break;
                case 9:
                    musicaAtual = Radio->end;
                    printf("Indo para a musica final: %d - %s",posicao_musica(Radio, musicaAtual->musica), musicaAtual->musica);
                    verificaLoop = 0;
                    break;
                case 10:
                    LinkedList_print(Radio);
                    break;
                default:
                    break;
            }

            ultimoTempo = tempoAtual;
            cont = 0;
            printf("\n\n");
        }
    }

    printf("****Desligando o Radio****");

    return (0);
}
