#include <MLlib.h>
#include <asndlib.h> //aSNEEDlib
#include <ogc/lwp.h>
#include <time.h>
#include <mp3player.h>
#include "note.h"
#include "sound.h"
#include "utils.h"
#include "files.h"
#define center _screenWidth/2
#define BF_OFFSET_NOTES 330
#define arrsize(arr) sizeof(&arr)
#define HIT_INCREMENT 6
ML_Image enemystrumsdata[8], playerstrumsdata[8], gfData, bfData, dadData, logoData, enterData, bgData, fpData, cursorData, storyData, stageData, noterData, barData, bfiData, dadiData;
ML_Sprite enemystrums[8], playerstrums[8], girlfriend, boyfriend, dad, logo, enter, bg, fp, cursor, story, stage, note, bar, bficon, dadicon;
ML_Font font, hf;

Note notes[1024];
cam camera;
con conductor = {bpm: 100, crochet: ((30 / 100) * 1000)};

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

lwp_t waitthread, camerathread;

bool fading;

void switchState(){
	switch(curstate){
		case TITLE:
			curstate = MENU;
			break;
		case MENU:
			ML_StopMP3();
			playPCM(bopeebo_raw, bopeebo_raw_size, NULL);
			ML_Wait(5);
			playSound(bopeebo_mp3, bopeebo_mp3_size);
			curstate = PLAY;
			break;
	}
}

void generateNotes2(){//unsigned long
	u32 chart[1024][4] = {{0,2,0,false},{600,3,450,false},{1050,3,600,false},{2400,2,0,true},{3000,3,450,true},{3450,3,600,true},{4800,1,300,false},{5400,0,300,false},{6000,3,300,false},{7200,1,300,true},{7800,0,300,true},{8400,3,300,true},{9600,1,150,false},{10200,3,0,false},{10500,0,0,false},{10800,1,450,false},{12000,1,150,true},{12600,3,0,true},{12900,0,0,true},{13200,1,450,true},{14400,3,0,false},{14700,1,0,false},{15300,0,0,false},{15600,2,300,false},{16800,3,0,true},{17100,1,0,true},{17700,0,0,true},{18000,2,300,true},{19200,0,0,false},{19500,3,0,false},{19800,1,750,false},{21600,0,0,true},{21900,3,0,true},{22200,1,750,true},{24000,1,0,false},{24300,3,0,false},{24600,0,750,false},{26700,3,0,true},{27000,0,750,true},{26400,1,0,true},{28800,2,0,false},{29100,3,0,false},{29400,0,1050,false},{31200,2,0,true},{31500,3,0,true},{31800,0,1050,true},{33600,0,0,false},{33900,3,0,false},{34500,2,0,false},{34800,1,450,false},{36000,0,0,true},{36300,3,0,true},{36900,2,0,true},{37200,1,450,true},{38400,2,450,false},{39000,3,300,false},{39600,0,450,false},{40800,2,450,true},{41400,3,300,true},{42000,0,450,true},{43200,1,0,false},{43800,2,0,false},{44400,1,0,false},{44550,1,0,false},{44700,1,0,false},{45000,2,0,false},{45600,1,0,true},{46200,2,0,true},{46800,1,0,true},{46950,1,0,true},{47100,1,0,true},{47400,2,0,true},{48000,2,450,false},{48600,3,300,false},{49200,0,450,false},{50400,2,450,true},{51000,3,300,true},{51600,0,450,true},{52800,3,1650,false},{55200,3,1650,true},{57600,2,0,false},{57900,3,0,false},{58200,0,1050,false},{60000,2,0,true},{60300,3,0,true},{60600,0,1050,true},{62100,6,0,true},{62400,0,0,false},{62700,3,0,false},{63300,2,0,false},{63600,1,450,false},{64500,6,0,false},{64800,0,0,true},{65100,3,0,true},{65700,2,0,true},{66000,1,450,true},{67200,2,0,false},{67500,3,0,false},{67800,0,0,false},{68100,2,0,false},{68400,1,450,false},{69600,2,0,true},{69900,3,0,true},{70200,0,0,true},{70500,2,0,true},{70800,1,450,true},{71700,6,0,true},{72000,0,0,false},{72300,3,0,false},{72900,2,0,false},{73200,1,450,false},{74100,6,0,false},{74400,0,0,true},{74700,3,0,true},{75300,2,0,true},{75600,1,450,true}};
	for(int i=0; i<sizeof(chart)/sizeof(chart[0]); i++){
		ML_CloneSprite(&note, &notes[i].note);
		switch(chart[i][1]){
			case 0:
				notes[i].type = PURPLE;
				break;
			case 1:
				notes[i].type = BLUE;
				break;
			case 2:
				notes[i].type = GREEN;
				break;
			case 3:
				notes[i].type = RED;
				break;
			default:
				notes[i].type = PURPLE;
		}
		notes[i].musthit = chart[i][3];
		notes[i].strumtime = chart[i][0];
	}
}

void generateNotes(bool player){
	if(player){
		for(int i=0; i<4; i++){
			ML_CloneSprite(&note, &enemystrums[i]);
			switch(i){
				case 0:
					ML_RotateSprite(&enemystrums[i], -90, false);
					break;
				case 1:
					ML_RotateSprite(&enemystrums[i], 180, false);
					break;
				case 2:
					ML_RotateSprite(&enemystrums[i], 0, false);
					break;
				case 3:
					ML_RotateSprite(&enemystrums[i], 90, false);
					break;
			}
		}
	}
	else{
		for(int i=0; i<4; i++){
			ML_CloneSprite(&note, &playerstrums[i]);
			switch(i){
				case 0:
					ML_RotateSprite(&playerstrums[i], -90, false);
					break;
				case 1:
					ML_RotateSprite(&playerstrums[i], 180, false);
					break;
				case 2:
					ML_RotateSprite(&playerstrums[i], 0, false);
					break;
				case 3:
					ML_RotateSprite(&playerstrums[i], 90, false);
					break;
			}
		}
	}
}

void wait(int seconds){
	ML_Wait(2000);
	fading = true;
	ML_Wait(1600);
	switchState();
	//callback();
}

void docamerashit(){
	while(1){
		for(int i=0; i<50; i++){
			ML_Wait(5);
			camera.x++;
		}
		ML_Wait(2000);
		for(int i=0; i<60; i++){
			ML_Wait(5);
			camera.x--;
		}
		ML_Wait(2000);
	}
	//callback();
}

float mustime;
bool hayawouldbeproud = false;
bool funny = false;
u8 selection = 0;
u8 enemyHealth = 0;
u16 bfHealth = 100;
unsigned long rollinginthedeep = 0;
int main(int argc, char **argv) 
{
	timer_t elapsed;
	time(&elapsed);
	int start = 300;

	ML_Init();
    ML_InitMP3();
	ML_InitRand();

    ML_LoadFontFromBuffer(&font, fnf_ttf, fnf_ttf_size, 64);
	ML_LoadFontFromBuffer(&hf, vcr_ttf, vcr_ttf_size, 16);
	font.style = FONT_DEFAULT | FONT_JUSTIFY_CENTER;

	//ML_SplashScreen(); // You can delete this instruction, but please let it if you can :)
	ML_EnableTextureAntiAliasing();
	_aa_enabled = true;
	ML_LoadSpriteFromBuffer(&noterData, &note, notes_png, 100, 100);
	ML_SetSpriteScale(&note, 0.7, 0.8);
	ML_InitTile(&note, 100, 100);
	generateNotes(false);
	generateNotes(true);
	generateNotes2();
	ML_LoadSpriteFromBuffer(&stageData, &stage, stage_png, 100, 100);
	ML_LoadSpriteFromBuffer(&gfData, &girlfriend, gf_png, 100, 100); // <- you must have a texture.png in ARGB (32 bits) at the root of your card so that you can test this example
	ML_LoadSpriteFromBuffer(&dadData, &dad, dad_png, 100, 100);
	ML_LoadSpriteFromBuffer(&bfData, &boyfriend, bf_png, 100, 100); // <- you must have a texture.png in ARGB (32 bits) at the root of your card so that you can test this example	
	ML_LoadSpriteFromBuffer(&bfiData, &bficon, iconbf_png, 100, 100);
	ML_LoadSpriteFromBuffer(&dadiData, &dadicon, icondad_png, 100, 100);
	ML_InitTile(&dadicon, 50, 100);
	ML_InitTile(&bficon, 50, 100);
	ML_SetSpriteScale(&bficon, -1.5, 1);
	ML_SetSpriteScale(&dadicon, 1.5, 1);
	ML_LoadSpriteFromBuffer(&logoData, &logo, logo_png, 0, 0);
	ML_LoadSpriteFromBuffer(&enterData, &enter, enter_png, 300, 100);
	ML_LoadSpriteFromBuffer(&bgData, &bg, bg_png, 0, 0);
	ML_LoadSpriteFromBuffer(&barData, &bar, healthBar_png, 0, 0);
	ML_LoadSpriteFromBuffer(&fpData, &fp, freeplay_png, 100, 100);
	ML_LoadSpriteFromBuffer(&storyData, &story, storymode_png, 100, 150);
	ML_LoadSpriteFromBuffer(&cursorData, &cursor, cursor_png, 100, 100);
	ML_InitTile(&story, 250, 100);
	ML_InitTile(&fp, 250, 100);

	u8 opac = 255;
	playSound(freakyMenu_mp3, freakyMenu_mp3_size);
	if(curstate == PLAY){
		ML_StopMP3();
		playSound(bopeebo_mp3, bopeebo_mp3_size);
		ML_Wait(5);
		playPCM(bopeebo_raw, bopeebo_raw_size, NULL);
	}
	float timing = 0;
	char base[33];
	int funnynumbers[5];
	int fadey = -720;
	for(int i; i<5; i++){
		funnynumbers[i] = ML_Rand(0, 5); //rng is always the same that's why it doesn't work loL!
	}
	
	LWP_CreateThread(&camerathread, docamerashit, NULL, NULL, 0, 0);
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
			if(Wiimote[0].Newpress.A) {
				playPCM(confirm_raw, confirm_raw_size, NULL);
				LWP_CreateThread(&waitthread, wait, 2, NULL, 0, 0);
			}
		}
		if(curstate == PLAY){
			ML_DrawSpriteFull(&stage, 0 + camera.x, -50 + camera.y, 0, 1.3, 1.3, 255);
			ML_DrawSpriteFull(&girlfriend, 0  + camera.x, -30 + camera.y, 0, 0.7, 0.8, 255);
			ML_DrawSpriteFull(&dad, -25 + camera.x, 55 + camera.y, 0, 1, 1.1, 255);
			ML_DrawSpriteFull(&boyfriend, 250 + camera.x, 120 + camera.y, 0, 0.7, 0.8, 255);
			mustime++;
			if(Wiimote[0].Newpress.Up){
				for(int i=0; i<arrsize(notes); i++){
					if(ML_IsCollisionEx(&playerstrums[2], &notes[i].note)) {
						ML_SetSpriteAlpha(&notes[i].note, 0);
						bfHealth += HIT_INCREMENT;
						}
				}
			}
			if(Wiimote[0].Newpress.Down) {
				for(int i=0; i<arrsize(notes); i++){
					if(ML_IsCollisionEx(&playerstrums[1], &notes[i].note)) {
						ML_SetSpriteAlpha(&notes[i].note, 0);
						bfHealth += HIT_INCREMENT;
						}
				}
			}
			if(Wiimote[0].Newpress.Left){
				for(int i=0; i<arrsize(notes); i++){
					if(ML_IsCollisionEx(&playerstrums[0], &notes[i].note)) {
						ML_SetSpriteAlpha(&notes[i].note, 0);
						bfHealth += HIT_INCREMENT;
					}
				}
			}
			if(Wiimote[0].Newpress.Right) {
				for(int i=0; i<arrsize(notes); i++){
					if(ML_IsCollisionEx(&playerstrums[3], &notes[i].note)) {
						ML_SetSpriteAlpha(&notes[i].note, 0);
						bfHealth += HIT_INCREMENT;
					}
				}
			}
			for(int i=0; i<4; i++){
				ML_DrawTile(&enemystrums[i], (i * 67), 0, 0);
				ML_DrawTile(&playerstrums[i], BF_OFFSET_NOTES + (i * 67), 0, 0);
			}
			for(int i=0; i<arrsize(notes); i++){
				u8 notedecide;
				switch(notes[i].type){case PURPLE: notedecide = 0; break; case BLUE: notedecide = 1 * 67; break; case GREEN: notedecide = 2 * 67; break; case RED: notedecide = 3 * 67; break;}
				ML_DrawTile(&notes[i].note, (notes[i].musthit?BF_OFFSET_NOTES:0) + notedecide, ((notes[i].strumtime / 3) - (mustime * 5)) - 500, notes[i].type);//notes[i].type (-5 - (playerstrums[i].height / 2)) - ((mustime - notes[i].strumtime) * 1)
			}
			rollinginthedeep += 4;
			ML_DrawSpriteFull(&bar, 20, 400, 0, 0.8, 0.9, 255);
			ML_DrawRect(130, 405, 380, 10, 0x00FF00FF, true);
			ML_DrawRect(130, 405, 290 - (bfHealth), 10, 0xFF0000FF, true); // + (bfHealth*5)  - bfHealth380
			ML_DrawSpriteXY(&bficon, 380 - (bfHealth - 100), 360);
			ML_DrawSpriteXY(&dadicon, 260 - (bfHealth - 100), 360);
			ML_DrawRect(0, fadey, 720, 720, createRGBA(0, 0, 0, 255), true);
			ML_DrawText(&hf, 430, 420, "Score: ");
			if(bfHealth<=-100) bfHealth=-100;
			if(bfHealth>=290) bfHealth=290;
		}

		if(fading){
			if(fadey > 1000) fading = false;
			else fadey += 7;
		}
		ML_DrawRect(0, fadey, 720, 720, createRGBA(0, 0, 0, 255), true);
		ML_RotateSprite(&cursor, Wiimote[0].Orient.Roll, 0);
		ML_MoveSpriteWiimoteIR(&cursor, 0);
		ML_DrawSprite(&cursor);
		if(Wiimote[0].Newpress.Home) ML_Exit();
		ML_Refresh();
	}
	//free(logo);
	return 0;
}

