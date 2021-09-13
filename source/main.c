#include <MLlib.h>
#include <asndlib.h> //aSNEEDlib
#include <ogc/lwp.h>
#include <time.h>
#include <mp3player.h>
#include "sound.h"
#include "utils.h"
#include "files.h"
#define center _screenWidth/2

ML_Image spriteData, logoData, enterData, bgData, fpData, cursorData, storyData;
ML_Sprite girlfriend, logo, enter, bg, fp, cursor, story;
ML_Font font;

lwp_t soundthread;

char *text[] = {
	"can i put my balls in",
	"wii port yo",
	"omg matt port when??",
	"omg eteled using libmii when??"
};

enum states{
    TITLE,
    MENU,
    PLAY
} curstate = TITLE;

lwp_t waitthread;
bool fading;

void switchState(){
	switch(curstate){
		case TITLE:
			curstate = MENU;
			break;
		case MENU:
			curstate = PLAY;
			break;
	}
}

void wait(int seconds){
	ML_Wait(2000);
	fading = true;
	ML_Wait(1600);
	switchState();
	//callback();
}

bool hayawouldbeproud = false;
bool funny = false;
u8 selection = 0;

int main(int argc, char **argv) 
{
	timer_t elapsed;
	time(&elapsed);
	int start = 300;

	ML_Init();
    ML_InitMP3();
	ML_InitRand();

    ML_LoadFontFromBuffer(&font, fnf_ttf, fnf_ttf_size, 64);
	font.style = FONT_DEFAULT | FONT_JUSTIFY_CENTER;

	//ML_SplashScreen(); // You can delete this instruction, but please let it if you can :)
	ML_EnableTextureAntiAliasing();
	_aa_enabled = true;
	ML_LoadSpriteFromBuffer(&spriteData, &girlfriend, gf_png, 100, 100); // <- you must have a texture.png in ARGB (32 bits) at the root of your card so that you can test this example
	ML_LoadSpriteFromBuffer(&logoData, &logo, logo_png, 0, 0);
	ML_LoadSpriteFromBuffer(&enterData, &enter, enter_png, 300, 100);
	ML_LoadSpriteFromBuffer(&bgData, &bg, bg_png, 0, 0);
	ML_LoadSpriteFromBuffer(&fpData, &fp, freeplay_png, 100, 100);
	ML_LoadSpriteFromBuffer(&storyData, &story, storymode_png, 100, 150);
	ML_LoadSpriteFromBuffer(&cursorData, &cursor, cursor_png, 100, 100);
	ML_InitTile(&story, 250, 100);
	ML_InitTile(&fp, 250, 100);

	u8 opac = 255;
	playSound(freakyMenu_mp3, freakyMenu_mp3_size);
	//playSound(bopeebo_mp3, bopeebo_mp3_size);
	//playPCM(bopeebo_raw, bopeebo_raw_size, NULL);
	float timing = 0;
	char base[33];
	int funnynumbers[5];
	int fadey = -720;
	for(int i; i<5; i++){
		funnynumbers[i] = ML_Rand(0, 5); //rng is always the same that's why it doesn't work loL!
	}
	
	while(1)
	{
		if(curstate == TITLE){
			timing = ML_ElapsedTime()/1000;
			//ML_DrawText(&font, 25, 30, itoa(fadey, base, 10));
			if(start)start--;
			switch((int)timing){
				case 0:
					ML_DrawText(&font, 25, 100, "Ninjamuffin presents");
					break;
				case 2:
					ML_DrawText(&font, 50, 100, "A port to the wii");
					break;
				case 4:
					ML_DrawText(&font, 50, 100, text[funnynumbers[0]]);
					ML_DrawText(&font, 50, 150, "yo jaws?");

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
				ML_DrawSpriteFull(&girlfriend, 100, 0, 0, 0.7, 0.9, 255); // Draws the sprite on the screen

				ML_DrawSpriteFull(&logo, -150, -130, 0, 0.7, 0.7, 255);
				ML_DrawSpriteFull(&enter, 120, 100, 0, 1.9, 1.6, 255);
				ML_DrawText(&font, 50, 400, "Press A to Start");
				ML_DrawRect(0, 0, 720, 720, createRGBA(255, 255, 255, opac), true);
				//ML_DrawSpriteFull(&white, 120, 100, 0, 4, 4, opac);
			}

			if(hayawouldbeproud && opac != 1) opac -= 2;

			if(Wiimote[0].Newpress.A && !funny){
				//LWP_CreateThread(&soundthread, balls, NULL, NULL, 0, 0);

				if(timing>= 10){
					//switchState();
					LWP_CreateThread(&waitthread, wait, 2, NULL, 0, 0);
					playPCM(confirm_raw, confirm_raw_size, NULL);
					funny = true;
					opac=255;
					ML_DrawRect(0, fadey, 720, 720, createRGBA(0, 0, 0, opac), true);
				}
			};
		}
		if(curstate == MENU){
			ML_DrawSprite(&bg);
			ML_DrawTile(&story, 195, 50, selection==0?1:0);
			ML_DrawTile(&fp, 195, 155, selection==1?1:0);

			if(Wiimote[0].Newpress.Up && selection > 0){
				playPCM(scroll_raw, scroll_raw_size, NULL);
				selection--;
			}
			if(Wiimote[0].Newpress.Down && selection < 1) {
				playPCM(scroll_raw, scroll_raw_size, NULL);
				selection++;
			}
		}
		ML_DrawRect(0, fadey, 720, 720, createRGBA(0, 0, 0, 255), true);
		if(fading){
			if(fadey > 1000) fading = false;
			else fadey += 7;
		}
		ML_RotateSprite(&cursor, Wiimote[0].Orient.Roll, 0);
		ML_MoveSpriteWiimoteIR(&cursor, 0);
		ML_DrawSprite(&cursor);
		if(Wiimote[0].Newpress.Home) ML_Exit();
		ML_Refresh();
	}
	//free(logo);
	return 0;
}

