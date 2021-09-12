#include <MLlib.h>
#include <asndlib.h> //aSNEEDlib
#include <ogc/lwp.h>
#include <mp3player.h>
#include "sound.h"
#include "utils.h"
#include "files.h"
#define center _screenWidth/2

ML_Image spriteData, logoData, enterData, whiteData;
ML_Sprite girlfriend, logo, enter, white;
ML_Font font;

lwp_t soundthread;

char *text[] = {
	"can i put my balls in yo jaws?",
	"wii port yo",
	"omg matt port when??",
	"omg eteled using libmii when??"
};

bool hayawouldbeproud = false;
bool funny = false;

void title();
void menu();
void playstate();

int main(int argc, char **argv) 
{
	timer_t elapsed;
	time(&elapsed);

	ML_Init();
    ML_InitMP3();
	ML_InitRand();

    ML_LoadFontFromBuffer(&font, fnf_ttf, fnf_ttf_size, 64);
	font.style = FONT_DEFAULT | FONT_JUSTIFY_CENTER;

	//ML_SplashScreen(); // You can delete this instruction, but please let it if you can :)

	ML_LoadSpriteFromBuffer(&spriteData, &girlfriend, gf_png, 100, 100); // <- you must have a texture.png in ARGB (32 bits) at the root of your card so that you can test this example
	ML_LoadSpriteFromBuffer(&logoData, &logo, logo_png, 300, 100);
	ML_LoadSpriteFromBuffer(&enterData, &enter, enter_png, 300, 100);

	u8 opac = 255;
	playSound(freakyMenu_mp3, freakyMenu_mp3_size);
	float timing = 0;
	//char base[33];
	int funnynumbers[5];
	for(int i; i<5; i++){
		funnynumbers[i] = ML_Rand(0, 5);
	}
	
	while(1)
	{

		timing = ML_ElapsedTime()/1000;
		//ML_DrawText(&font, 25, 30, itoa(ML_ElapsedTime()/1000, base, 10));
		switch((int)timing){
			case 0:
				ML_DrawText(&font, 25, 100, "Ninjamuffin presents");
				break;
			case 2:
				ML_DrawText(&font, 50, 100, "A port to the wii");
				break;
			case 3:
				ML_DrawText(&font, 50, 100, text[funnynumbers[0]]);
				break;
			case 5:
				ML_DrawText(&font, 50, 100, text[funnynumbers[1]]);
				break;
			case 7:
				ML_DrawText(&font, 200, 100, "Friday");
				break;
			case 8:
				ML_DrawText(&font, 200, 100, "Friday");
				ML_DrawText(&font, 200, 175, "Night");
				break;
			case 9:
				ML_DrawText(&font, 200, 100, "Friday");
				ML_DrawText(&font, 200, 175, "Night");
				ML_DrawText(&font, 200, 250, "Funkin");
				break;
		}
		if(timing>= 10){
			hayawouldbeproud = true;
			ML_DrawSpriteFull(&girlfriend, 400, 250, 0, 1.7, 1.8, 255); // Draws the sprite on the screen

			ML_DrawSpriteFull(&logo, 120, 100, 0, 1.9, 1.6, 255);
			ML_DrawSpriteFull(&enter, 120, 100, 0, 1.9, 1.6, 255);
			ML_DrawText(&font, 50, 400, "Press A to Start");
			ML_DrawRect(0, 0, 720, 720, createRGBA(255, 255, 255, opac), true);
			//ML_DrawSpriteFull(&white, 120, 100, 0, 4, 4, opac);
		}

		if(hayawouldbeproud && opac != 1) opac -= 2;

		if(Wiimote[0].Newpress.A){
			//LWP_CreateThread(&soundthread, balls, NULL, NULL, 0, 0);

			if(timing>= 10){
				playPCM(confirm_raw, confirm_raw_size, NULL);
				funny = true;
				opac=255;
			}
		};
		if(Wiimote[0].Newpress.Home) ML_Exit();
		ML_Refresh();
	}
	//free(logo);
	return 0;
}

