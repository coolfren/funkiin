#include "sound.h"
#include "confirm_raw.h"
void playSound(const u8 *filename, u32 len){
    ML_PlayMP3FromBuffer(filename, len);
}
bool loop = false;
void playPCM(const u8 *filename, u32 len, void *callback){
		int vol = 255;
		int voice = ASND_GetFirstUnusedVoice();
		if(voice >= 0)
			ASND_SetVoice(voice, VOICE_STEREO_16BIT, 44100, 0,
				(u8 *)filename, len, vol, vol, NULL);
}