#ifndef SOUNDFILE
#define SOUNDFILE
#include <asndlib.h>
#include <gctypes.h>
void playSound(const u8 *filename, u32 len);
void playPCM(const u8 *filename, u32 len, void *callback);
#endif