#include <MLV/MLV_all.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define LIGNE 7
#define POSITION 24
#define LARGEUR 1500
#define HAUTEUR 980
#define MARGE 140
#define TAILLE_CASE_X 55
#define TAILLE_CASE_Y 106
#define TAILLE_PLATEAU_X LARGEUR-1.2*MARGE-18
#define TAILLE_PLATEAU_Y HAUTEUR-2.3*MARGE+36

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

typedef struct {
    int x;
    int y;

}endroit_virus;




void clic(Coord *coord,  int *x, int *y){         // x, y = coordonnées du clic
    int i, j, tmp;
    if(*x>MARGE && *x < TAILLE_PLATEAU_X+2*MARGE && *y>MARGE && *y<TAILLE_PLATEAU_Y+2*MARGE){      //Clique dans la grille
        for(i=MARGE; i<TAILLE_PLATEAU_X+MARGE; i+=54){
            if(*x>i && *x<i+54){
                *x = i/54 -1;
                if(*x==25) *x = 24;
                coord->x = *x;
            }
        }
        tmp = 0;
        for(j=MARGE; j< TAILLE_PLATEAU_Y+MARGE; j+= 54){
            if(*y>j && *y<j+54){
                if(tmp%2==0)
                    coord->y = (j/54)/2;
                else
                    coord->y = 0;
            }
            ++tmp;
        }
        
    }
    else{
        coord->x = *x;
        coord->y = *y;
    }
}


void afficher_vie(Game game){
    MLV_draw_filled_rectangle(50, 100, 50, 10, MLV_COLOR_GREEN);
    MLV_draw_filled_rectangle(50, 100, 20, 10, MLV_COLOR_RED);
    //faut savoir la vie du virus
    //son affichage viendra apres quand on affiche les viruses.
}




void affiche_plateau_graphqiue(){
    //MLV_draw_filled_rectangle(MARGE, MARGE, TAILLE_PLATEAU_X, TAILLE_PLATEAU_Y, MLV_COLOR_GREEN);
    
    int i, j;
    for(i=1; i<LIGNE+1; i++){
        for(j=1; j<POSITION+1; j++){
            //MLV_draw_filled_rectangle(j*TAILLE_CASE_X+110-29, i*TAILLE_CASE_Y+75-29-10, 54, 54, MLV_COLOR_BLUE);

            MLV_draw_filled_circle(j*TAILLE_CASE_X+110, i*TAILLE_CASE_Y+75-10, 6, MLV_COLOR_RED);
        }
    }            
   //MLV_draw_filled_rectangle(TAILLE_CASE_X+110-29, TAILLE_CASE_Y+75-29+54-10, 54, 54, MLV_COLOR_PURPLE);
    MLV_actualise_window();
}


void affiche_bar_haut(){
    // il faut prendre game et afficher les tourelles disponible
    // pareil pour les viruses
    MLV_Image *ram;
    MLV_Image *firewall;
    MLV_Image *alu;
    


    ram = MLV_load_image("ram.png");
    firewall = MLV_load_image("firewall.png");
    alu = MLV_load_image("alu.png");
    
    MLV_draw_filled_rectangle(90, 8, 500, 70, MLV_COLOR_GREY);
    
    MLV_draw_text(450, 35, "Money : 300", MLV_COLOR_BLACK); //pour afficher l'argent on prend game.money
    
    

    MLV_draw_image(ram, 100, 15);
    MLV_draw_image(firewall, 200, 15);
    MLV_draw_image(alu, 300, 15);
    MLV_actualise_window();
}



void affiche_vage(endroit_virus *endroit){
    //on prend un virus on l'affiche et on change de coordonées vers la gauche jusqu'au la fin(à droite)

    MLV_Image *virus;
    virus = MLV_load_image("fpe.png");
    MLV_draw_image(virus, endroit->x, endroit->y);
    endroit->x -= TAILLE_CASE_X;
    if(endroit->x == 0){
        return;
    }
    MLV_wait_seconds( 1 );

    
    MLV_actualise_window();
    //MLV_actualise_window();
}

void poser_chips(Coord coord, endroit_virus *endroit,int x, int y){
    MLV_Image *ram;
    MLV_Image *firewall;
    MLV_Image *alu;
    int defence;
    for(;;){
        MLV_wait_mouse(&x, &y);
        clic(&coord, &x, &y);
        printf("%d, %d\n", coord.x, coord.y);
        if(101<coord.x && coord.x < 199 && 5<coord.y && coord.y<90){
            defence = 1;
            printf("%d\n", defence);
        }
        if((201<coord.x && coord.x < 299 && 5<coord.y && coord.y<90)){
            defence = 2;
            printf("%d\n", defence);       
        }
        if((301<coord.x && coord.x < 399 && 5<coord.y && coord.y<90)){
            defence = 3;
            printf("%d\n", defence);       
        }
        if(coord.y != 0){
            if(defence == 1){
                //fprintf(stderr, "-------------------");
                ram = MLV_load_image("ram.png");
                MLV_draw_image(ram, coord.x*TAILLE_CASE_X+MARGE/4+TAILLE_CASE_X, coord.y*TAILLE_CASE_Y+MARGE/4);
                MLV_actualise_window();
                }
            if(defence == 2){
                firewall = MLV_load_image("firewall.png");
                MLV_draw_image(firewall, coord.x*TAILLE_CASE_X+MARGE/4+TAILLE_CASE_X, coord.y*TAILLE_CASE_Y+MARGE/4);
                MLV_actualise_window();
            }
            if(defence == 3){
                alu = MLV_load_image("alu.png");
                MLV_draw_image(alu, coord.x*TAILLE_CASE_X+MARGE/4+TAILLE_CASE_X, coord.y*TAILLE_CASE_Y+MARGE/4);
                MLV_actualise_window();
            }
        }
        affiche_vage(endroit);
    }
    
}





int main(int argc, char const *argv[])
{
    //Game game;
    Coord coord;
    endroit_virus endroit;
    endroit.x = TAILLE_PLATEAU_X;
    endroit.y = TAILLE_PLATEAU_Y;
    int image_width, image_height, x, y;
    MLV_create_window("Virus vs chips", "Virus vs chips", LARGEUR+MARGE+10, HAUTEUR+MARGE+10);
    for(;;){
        MLV_draw_filled_rectangle(0, TAILLE_PLATEAU_Y, TAILLE_PLATEAU_X, TAILLE_PLATEAU_Y, MLV_COLOR_WHITE);
        MLV_Image *background;
        
        background = MLV_load_image("background.jpg");
        MLV_resize_image_with_proportions(background, LARGEUR, HAUTEUR);
        MLV_get_image_size( background, &image_width, &image_height );
        MLV_change_window_size( image_width, image_height );
        MLV_draw_image( background, 0, 0 );
        //poser_chips(coord, x, y);
        affiche_plateau_graphqiue();
        affiche_bar_haut();
        affiche_vage(&endroit);
        MLV_wait_seconds( 5 );
    }
    return 0;
}