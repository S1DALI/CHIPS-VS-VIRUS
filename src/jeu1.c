#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define LIGNE 7
#define POSITION 24

typedef struct chips{
    int type;
    int life;
    int line;
    int position;
    int price;
    struct chips* next;
}Chips;

typedef struct virus{
    int type;
    int life;
    int line;
    int position;
    int speed;
    int turn;
    struct virus* next;
    struct virus* next_line;
    struct virus* prev_line;
}Virus;

typedef struct{
    Virus* virus;
    Chips* chips;
    int turn;
    int money;
    int statut_partie;      //0 en cours, 1 victoire, -1 perdu
}Game;


typedef struct
{
    int x;
    int y;
}Coord;


typedef struct info_chips{
    int type;
    int life;
    int price;
    int damage;
    struct info_chips * next;
}Info_chips;


typedef struct info_virus{
    int type;
    int life;
    int speed;
    int price;
    int damage;
    struct info_virus * next;
}Info_virus;

/**
 * @brief Alloue l'espace pour un chips, et configure ses variables
 * 
 * @param type Type de chips
 * @param life Vie du chips
 * @param line Ligne du chips
 * @param position Position du chips
 * @param price Prix
 * @return Chips* 
 */
Chips* alloueChips(int type, int life, int line, int position, int price){
    Chips* tmp = (Chips*)malloc(sizeof(Chips));
    if(tmp)
    {
        tmp->type = type;
        tmp->life = life;
        tmp->line = line;
        tmp->position = position;
        tmp->price = price;
        tmp->next = NULL;
    } 
    // else pour erreur ?
    return tmp;
}


/**
 * @brief Alloue l'espace pour un virus, et configure ses variables
 * 
 * @param type Type de virus
 * @param life Vie du virus
 * @param line Ligne du virus
 * @param position Position du virus
 * @param speed Vitesse du virus
 * @param turn Tour d'apparition
 * @return Virus* 
 */
Virus* alloueVirus(int type, int life, int line, int position, int speed, int turn){
    Virus* tmp = (Virus*)malloc(sizeof(Virus));
    if(tmp)
    {
        tmp->type = type;
        tmp->life = life;
        tmp->line = line;
        tmp->position = position;
        tmp->speed = speed;
        tmp->turn = turn;
        tmp->next = NULL;
        tmp->next_line = NULL;
        tmp->prev_line = NULL;
    }
    //else pour erreur ?
    return tmp;
}

/**
 * @brief Aloue l'espace pour l'information d'un chips et configure ses variables
 * 
 * @param type Type du chips
 * @param life Vie du chips
 * @param price Prix du chips
 * @param damage Dégat du chips
 * @return Info_chips* 
 */
Info_chips * alloueInfo_chips(int type, int life, int price, int damage){
    Info_chips* tmp = (Info_chips*) malloc(sizeof(Info_chips));
    if(tmp)
    {
        tmp->next = NULL;
        tmp->type = type;
        tmp->price = price;
        tmp->damage = damage;
        tmp->life = life;
    }
    return tmp;
}


/**
 * @brief Alloue l'espace pour l'information d'un virus et configure ses variables
 * 
 * @param type Type du virus
 * @param life Vie du virus
 * @param speed Vitesse du virus
 * @param damage Dégat du virus
 * @return Info_virus* 
 */
Info_virus * alloueInfo_virus(int type, int life, int speed, int damage){
    Info_virus* tmp = (Info_virus*) malloc(sizeof(Info_virus));
    if(tmp)
    {
        tmp->next = NULL;
        tmp->type = type;
        tmp->life = life;
        tmp->damage = damage;
        tmp->speed = speed;
    }
    return tmp;
}

/**
 * @brief Peremt d'ajouter un nouvelle information chainé à la fin de la dans la structure
 * 
 * @param info 
 * @param type 
 * @param life 
 * @param speed 
 * @param damage 
 */
void add_info_virus(Info_virus **info, int type, int life, int speed, int damage){
    if(*info == NULL){
        Info_virus *tmp = alloueInfo_virus(type, life, speed, damage);
        *info = tmp;
    }
    else{
        Info_virus *tmp = alloueInfo_virus(type, life, speed, damage);
        while((*info)->next)
            info = &(*info)->next;
        (*info)->next = tmp;
    }
}


void add_info_chips(Info_chips **info, int type, int life, int price, int damage){
    if(*info == NULL)
    {
        Info_chips *tmp = alloueInfo_chips(type, life, price, damage);
        *info = tmp;
    }
    else
    {
        Info_chips *tmp = alloueInfo_chips(type, life, price, damage);
        while((*info)->next) info = &(*info)->next;
        (*info)->next = tmp;
    }
}


void cree_info_virus(Info_virus ** info){
    add_info_virus(info, 69, 3, 2, 2);  //E
    add_info_virus(info, 71, 4, 1, 2);  //G
    add_info_virus(info, 77, 2, 3, 1);  //M
}


void cree_info_chips(Info_chips **info){
    add_info_chips(info, 65, 3, 50, 3); // 65 = A
    add_info_chips(info, 70, 8, 100, 0); // 70 = F
    add_info_chips(info, 82, 3, 50, 1); // 82 = R
}

void affiche_tab_appercu(char tab[LIGNE][POSITION]){
    int i, j;
    for(i=0; i<LIGNE; i++)
    {
        printf("%d| ", i+1);
        for(j=0; j<POSITION; j++)
        {
            printf(" %c", tab[i][j]);
        }
        printf("\n");
    } 
}


void affiche_tab_game(char tab[LIGNE][POSITION][2]){
    int i, j;
    for(i=0; i<LIGNE; i++)
    {
        printf("%d| ", i+1);
        for(j=0; j<POSITION; j++)
        {
            printf(" %2c%c", tab[i][j][0], tab[i][j][1]);

        }
        printf("\n");
    } 
}


int demande_chips(Info_chips **info, Game *game){
    Info_chips ** start = info;
    int tourelle;
    int correct;
    do{
        printf("\nVous avez %d coins\n", (*game).money);
        info = start; 
        while(*info){
            printf("%c : prix( %d )\n", (*info)->type, (*info)->price);      //Afficher le prix de chacunes des tourelles
            info = &(*info)->next;
        }
        printf("q : Lancer la partie\n");
        printf("Votre choix ?\n");
        scanf(" %lc", &tourelle);
        //printf("%d\n", tourelle);
        if(tourelle!=70 && tourelle!=80 && tourelle!=65 && tourelle!=82 && tourelle!=88 && tourelle!=113)
        {
            correct=1;
            printf("Valeur incorecte !\n");
        }
        else if((*game).money < 50) //50 = prix minimum
        {            
            correct=0;
            tourelle = 113;
            printf("Vous n'avez plus assez d'argent !\n");
        }   
        else
        {
            info = start;
            while( (*info)->next && (*info)->type!=tourelle){
                info = &(*info)->next;
            }
            if(game->money>=(*info)->price)
            {
                game->money -= (*info)->price;
                correct = 0;
            }
        }
    }while(correct==1);
    return tourelle;
}


void add_chips(Chips ** lst, Info_chips** info, int type, Coord coord){
    while((*info)->type!=type && (*info)->next)
        info = &(*info)->next;
    if(*lst == NULL) //Liste vide
    {   
        Chips *tmp = alloueChips(type, (*info)->life, coord.y, coord.x, (*info)->price);
        *lst = tmp;
    }
    else
    {
        Chips *tmp = alloueChips(type, (*info)->life, coord.y, coord.x, (*info)->price);
        while((*lst)->next) lst = &(*lst)->next;
        (*lst)->next = tmp;
    }
}

void demande_coord(Chips ** lst, Info_chips** info, Coord *coord, int tourelle){
    do{
        printf("Dans quelles coordonees voulez vous la poser : ");
        scanf(" %d %d", &coord->x, &coord->y);
    }while(coord->x < 0 && coord->x>POSITION && coord->y<0 && coord->y>LIGNE);
    add_chips(lst, info, tourelle, *coord);
}

void insert_chips_plateau(Chips ** lst, char tab[LIGNE][POSITION][2]){
    if(*lst)
    {
        tab[(*lst)->line-1][(*lst)->position-1][0] = (*lst)->life+'0';
        tab[(*lst)->line-1][(*lst)->position-1][1] = (*lst)->type;
        
        while((*lst)->next)
        {
            lst = &(*lst)->next;
            tab[(*lst)->line-1][(*lst)->position-1][0] = (*lst)->life+'0';
            tab[(*lst)->line-1][(*lst)->position-1][1] = (*lst)->type;
        }
    }
}


void add_virus(Virus ** lst, Info_virus **info, int turn, int line, int type){
    int position = POSITION;
    Virus ** start = NULL;
    
    while((*info)->next && (*info)->type!=type)
         info = &(*info)->next; 


    if(*lst == NULL) //Liste vide
    {       
        Virus* tmp = alloueVirus(type, (*info)->life, line, position, (*info)->speed, turn);
        *lst = tmp;
    }
    else //Liste non vide
    {                   
        start = lst;
        Virus * tmp = alloueVirus(type, (*info)->life, line, position, (*info)->speed, turn);
        while((*lst)->next && (*lst)->line!=line) lst = &(*lst)->next;
        if((*lst)->line==line) // si on est sur la bonne ligne
        {
            while((*lst)->next_line)
                lst = &(*lst)->next_line;
            (*lst)->next_line = tmp;
            tmp->prev_line = *lst;
        }
        lst = start;
        while((*lst)->next)
            lst = &(*lst)->next;
        (*lst)->next = tmp;
    }
}

void delete_virus(Virus **virus, Virus ** prec, int line, int position){
    Virus * tmp;
    if(*virus == NULL)
    {
        exit(EXIT_FAILURE);
    }
    if(prec == NULL){
        if(!(*virus)->next){  // Dernier virus
            free((*virus));
            (*virus) = NULL;
            return;
        }
        if((*virus)->line==line && (*virus)->position==position){
            tmp = (*virus)->next;
            if((*virus)->next_line)
                (*virus)->next_line->prev_line = NULL; 
            free(*virus);
            (*virus) = NULL;
            *virus = tmp;
        }
    }else{
        if((*virus)->line==line && (*virus)->position==position){
            tmp = (*virus)->next;
            if((*virus)->next_line)
                (*virus)->next_line->prev_line = NULL;
            
            free((*virus));
            (*virus) = NULL;
            *virus = *prec;
            (*virus)->next = tmp;
        }
    }
}

void affiche_liste_chips(Chips** chips){
    Chips* tmp = NULL;
    while((*chips)){
        tmp = (*chips)->next;
        printf("Type: %d\n", (*chips)->type);
        printf("Life: %d\n", (*chips)->life);
        printf("Line: %d\n", (*chips)->line);
        printf("Position: %d\n", (*chips)->position);
        printf("Price: %d\n", (*chips)->price);
        chips= &tmp;
    }
}

void delete_chips(Chips **chips, Chips ** prec, int position, int line){
    Chips * tmp;
    if(*chips == NULL){
        exit(EXIT_FAILURE);
    }
    if(!(*chips)->next){
        free(*chips);
        *chips = NULL;
    }
    else{
        if(prec == NULL){
            if((*chips)->position==position && (*chips)->line==line){
                tmp = (*chips)->next;
                (*chips) = NULL;
                free(*chips);
                *chips = tmp;
            }
        }
        else{
            if((*chips)->position==position && (*chips)->line==line){
                tmp = (*chips)->next;
                (*chips) = NULL;
                free(*chips);
                *chips = *prec;
                (*chips)->next = tmp;
            }
        }
            
    }
}

void affiche_lst_virus(Virus * lst){
    //Fonction pour s'aider
    while(lst)
    {
        printf("\n");
        // printf("Type = %d\n", lst->type);
        // printf("Life = %d\n", lst->life);
        printf("Turn = %d\n", lst->turn);
        printf("Line = %d\n", lst->line);
        printf("Position = %d\n", lst->position);
        // printf("Speed = %d\n", lst->speed);
        printf("Next = %d\n", lst->next);
        // printf("Next line = %s\n", lst->next_line);
        // printf("Previous Line = %s\n\n", lst->prev_line);
        lst = lst->next;
    }
}


void lire_fichier(char nom_fichier[], Game* game, Virus ** lst_virus, Info_virus** info){
    FILE* fichier = NULL;
    fichier = fopen(nom_fichier, "r");
    char money_c[5] = "";
    int money;
    char ligne[7];
    int turn, line, type;

    if (fichier)
    {
        money = atoi(fgets(money_c, 5, fichier));
        game->money = money;

        while(fgets(ligne, 8, fichier))
        {
            ligne[strcspn(ligne, "\r\n")] = 0;   // Cette ligne est obligatoire pour pouvoir utiliser ce programme sur windows et linux car ils sont differents sur les sauts de lignes
            turn = atoi(&ligne[0]);
            line = atoi(&ligne[2]);
            type = ligne[4];
            //printf("%d, %d\n", turn, line);

            if(line>0)
                add_virus(lst_virus, info, turn, line, type);
        }
    }
    else printf("Impossible d'ouvrir le fichier");
    fclose(fichier);
}

void init_plateau(char tab[LIGNE][POSITION][2]){
    int i, j;
    for(i=0; i<LIGNE; i++)
    {
        for(j=0; j<POSITION; j++)
        {
            tab[i][j][0] = ' ';
            tab[i][j][1] = ' ';

        }
    }
}

void appercu_vague(Virus * lst, char tab[LIGNE][POSITION]){
    int i, j;
    for(i=0; i<LIGNE; i++)
    {
        for(j=0; j<POSITION; j++)
        {
            tab[i][j] = ' ';
        }
    }
    while(lst)
    {
        tab[lst->line-1][lst->turn-1] = lst->type;
        lst = lst->next;
    }    
}


void moove_virus(Virus **virus, int turn){
    while((*virus))
    {
        if((*virus)->turn <= turn)
        {
            (*virus)->position -= (*virus)->speed;    
        }
    virus = &(*virus)->next;
    }
}

void modif_speed_virus(Info_virus * info, Virus *tab[LIGNE]){
    int i;
    for(i=0; i<LIGNE; i++){
        if(tab[i] != NULL){
            while(info->next && tab[i]->type != info->type)
                info = info->next;
            tab[i]->speed = info->speed;
            tab[i] = NULL;
        }
    }
}


void contact_virus(Chips** chips, Virus ** virus, Game * game, Virus* modif_speed[LIGNE], Info_virus * info){
    Virus ** start_virus = virus;
    Virus ** cpy = start_virus;
    Chips ** prec_chips = NULL;
    Virus ** prec = NULL;
    Chips ** start_chips = chips;
    int i;
    while((*virus))
    {
        if(((*virus)->position==2 && (*virus)->speed>=2) || ((*virus)->position==3 && (*virus)->speed>=3))
            (*virus)->speed = 1;
        if((*virus)->position==1)
        {          //Virus contre la paroi gauche donc fin de partie
           //DEFAITE
           game->statut_partie = -1;
           return;
        }
        start_virus = cpy;
        chips = start_chips;

       // Contact virus contre virus
        while((*start_virus))
        {
            if((*virus)->line == (*start_virus)->line)
            {
                if((*virus)->position > (*start_virus)->position)
                {
                    if((*virus)->position == (*start_virus)->position+1)
                        (*virus)->speed = (*start_virus)->speed;
                    else if((*virus)->position < (*start_virus)->position)
                        if((*virus)->position+1 == (*start_virus)->position)
                            (*start_virus)->speed = (*virus)->speed;
                }
            }
            start_virus = &(*start_virus)->next;
        }

        //Contact virus contre chips

        if(*chips){
            chips = start_chips;
            while(*chips){
                if((*virus)->line == (*chips)->line && (*chips)->position+(*virus)->speed == (*virus)->position){
                    for(i=0; i<LIGNE; i++){
                        if(modif_speed[i] == NULL){
                            modif_speed[i] = *virus;
                            (*virus)->speed = 1;
                            break;
                        }
                    }
                }
               chips = &(*chips)->next;
            }

            chips = start_chips;
            if(!(*chips)->next){
                if((*virus)->line == (*chips)->line && (*virus)->position-1 == (*chips)->position){
                    while(info->next && (*virus)->type != info->type)
                        info = info->next;

                    (*chips)->life -= info->damage;
                    for(i=0; i<LIGNE; i++){
                        if(modif_speed[i] == NULL){
                            modif_speed[i] = *virus;
                            (*virus)->speed = 0;
                            break;
                        }
                    }
                    if((*chips)->life<=0){
                        (*virus)->life -= 1;
                        prec = NULL;
                        delete_chips(chips, prec_chips, (*chips)->position, (*chips)->line);
                    }
                }
            }
            else{
                chips = start_chips;
                while((*chips)){
                    if((*virus)->line == (*chips)->line && (*virus)->position-1 == (*chips)->position){
                        while(info->next && (*virus)->type != info->type)
                        info = info->next;

                        (*chips)->life -= info->damage;
                        for(i=0; i<LIGNE; i++){
                            if(modif_speed[i] == NULL){
                                modif_speed[i] = *virus;
                                (*virus)->speed = 0;
                                break;
                            }
                        }
                        if((*chips)->life<=0){
                            (*virus)->life -= 1;
                            delete_chips(chips, prec_chips, (*chips)->position, (*chips)->line);
                        }
                    }
                    prec_chips = chips;
                    chips = &(*chips)->next;
                }
            }
        }
        if((*virus)->life <= 0){
            delete_virus(virus, prec, (*virus)->line, (*virus)->position);
        }
        if(*virus){
            prec = virus;
            virus = &(*virus)->next;
        }
    }
}


void init_virus(Virus ** virus){
    while((*virus))
    {
        (*virus)->position = POSITION + (*virus)->speed;
        virus = &(*virus)->next;
    }
}


void dammage_chips(Chips ** chips, Virus ** virus, Info_chips ** info, Game game){
    Virus **  start_virus = virus;
    Info_chips ** start_info = info;
    while(*chips){
        info = start_info;
        while(*info && (*info)->type!=(*chips)->type)
            info = &(*info)->next;
        
        virus = start_virus;
        while((*virus) && (*virus)->line!=(*chips)->line)
            virus = &(*virus)->next;

        if(*virus){
            if((*virus)->line==(*chips)->line){
                if(!(*virus)->prev_line && (*virus)->position <=23){    
                    if(game.turn%2==0)
                        (*virus)->life -= (*info)->damage;
                }
            }
        }
        chips = &(*chips)->next;
    }

}


void actualise_tab(char tab[LIGNE][POSITION][2], Virus** virus, Chips ** chips, int turn){
    init_plateau(tab);
    
    insert_chips_plateau(chips, tab);

    if(*virus){
        if((*virus)->turn <= turn)
        {
            tab[(*virus)->line-1][(*virus)->position-1][0] = (*virus)->life+'0';
            tab[(*virus)->line-1][(*virus)->position-1][1] = (*virus)->type;

        }
        while((*virus) && (*virus)->turn <= turn)
        {
            tab[(*virus)->line-1][(*virus)->position-1][1] = (*virus)->life+'0';
            tab[(*virus)->line-1][(*virus)->position-1][0] = (*virus)->type;
            virus = &(*virus)->next;
        }
    }
}


//----------------------- fonctions pour le mode graphique -----------------------
//...



//-----------------------MAIN-----------------------

int main(int argc, char *argv[]){
    char nom_fichier[]  = "data/level1.txt";
    Info_chips * info_chips = NULL;
    Info_virus * info_virus = NULL;
    Virus * speed_modif[LIGNE] = {NULL};
    Coord coord;
    int tourelle;
    cree_info_virus(&info_virus);
    cree_info_chips(&info_chips);
    char tab_appercu[LIGNE][POSITION];
    char tab_game[LIGNE][POSITION][2];
    Game game;
    game.chips = NULL;
    game.virus = NULL;
    game.statut_partie = 0;         //0 en cours, 1 victoire, -1 défaite
    game.turn = 1;

    

    //----------------------MODE CONSOLE----------------------

    // Apperçu de la vague
    system("@cls||clear");      // Clear le terminal
    printf("Une nouvelle vague approche...\n");
    lire_fichier(nom_fichier, &game, &(game.virus), &info_virus);
    appercu_vague(game.virus, tab_appercu);
    affiche_tab_appercu(tab_appercu);
    affiche_lst_virus(game.virus);
    init_plateau(tab_game);

    //Positionnent des défenses
    do
    {
        tourelle = demande_chips(&info_chips, &game);
        if(tourelle!=113)
        {
        demande_coord(&game.chips, &info_chips, &coord, tourelle);
        insert_chips_plateau(&game.chips, tab_game);
        }
        
        system("@cls||clear");
        printf("Une nouvelle vague approche...\n");
        affiche_tab_appercu(tab_appercu);
        printf("\n\n");
        affiche_tab_game(tab_game);
    }while(tourelle!=113);

    init_virus(&game.virus);
    

    //Début de la partie

    while(game.statut_partie == 0)
    {
        system("@cls||clear");

        printf("Game turn %d\n\n", game.turn);
        moove_virus(&game.virus, game.turn);
        modif_speed_virus(info_virus, speed_modif);
        dammage_chips(&game.chips, &game.virus, &info_chips, game);
        contact_virus(&game.chips, &game.virus, &game, speed_modif, info_virus);

        actualise_tab(tab_game, &game.virus, &game.chips, game.turn);
        affiche_tab_game(tab_game);
        

        sleep(1);
        game.turn += 1;

        if(game.virus == NULL){
            game.statut_partie = 1;
        }
    }


    if(game.statut_partie == 1)
    {
        //VICTOIRE
        printf("Victoire ! vous avez tue tous les virus !");
    }
    else
    {
        //DEFAITE
        printf("Defaite ! Les virus ont reussi a rentrer dans vortre ordinateur");
    }

    


    //----------------------MODE GRAPHIQUE----------------------
    // ...
    return 0;
}


//A 1 1 A 1 2 A 1 3 A 1 4 A 1 5 A 1 6 A 1 7 q