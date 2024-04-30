#include <MLV/MLV_all.h>
#include <stdio.h>
#include <stdlib.h>


int main(){
    int width = 640, height = 480;
    MLV_create_window( "explotion", "explotion", width, height );
    const char* file = "explotion.png";
    MLV_Image *explotion = MLV_load_image( file );
    if( ! explotion ){
        fprintf(stderr, "imposible d'ouvrir le fichier");
        exit(1);
    }
    MLV_Animation *animation;
    int nb_frames = 6;
	int nb_layers = 1;
	int nb_channels = 1;
	int w = 125, h = 125;
    MLV_Image *images[4];

    images[0] = MLV_copy_partial_image( explotion, 0, 0, w, h );
	images[1] = MLV_copy_partial_image( explotion, 1*(w+1), 0, w, h );
	images[2] = MLV_copy_partial_image( explotion, 2*(w+1), 0, w, h );
	images[3] = MLV_copy_partial_image( explotion, 3*(w+1), 0, w, h );

    animation	= MLV_create_animation(
		nb_frames, nb_layers, nb_channels
	);
    int layer = 0;
	MLV_add_frame_in_animation( 
		images+0, NULL, 4, animation
	);
	MLV_add_frame_in_animation(
		images+1, NULL, 4, animation
	);
	MLV_add_frame_in_animation(
		images+2, NULL, 4, animation
	);
	MLV_add_frame_in_animation(
		images+3, NULL, 4, animation
	);

    MLV_Animation_player* animation_player = MLV_create_animation_player( animation );

    MLV_play_animation_player( animation_player );
    MLV_change_frame_rate( 24 );

    int avance = 0;
	MLV_Keyboard_button touche;
	while( 
		MLV_get_event (
			&touche, NULL, NULL,
			NULL, NULL,
			NULL, NULL, NULL,
			NULL
		) == MLV_NONE ||
		touche != MLV_KEYBOARD_ESCAPE
 	){
		MLV_update_animation_player( animation_player );

		MLV_clear_window( MLV_COLOR_BROWN );
		MLV_draw_filled_rectangle(
			width/2,0, width/2, height, MLV_COLOR_YELLOW
		);
		
		
		MLV_play_animation_player( animation_player );
		int position_y = 20;
		MLV_draw_image_from_animation_player( animation_player, layer, avance, position_y );
		MLV_actualise_window();
		MLV_play_sound_from_animation_player( animation_player, 0 );
		MLV_delay_according_to_frame_rate();
	}

	MLV_free_animation_player( animation_player );

	MLV_free_animation( animation );

	MLV_free_image( images[0] );
	MLV_free_image( images[1] );
	MLV_free_image( images[2] );
	MLV_free_image( images[3] );

	MLV_free_image( explotion );

	MLV_free_window();
	MLV_free_audio();

	return 0;

}