#ifndef SOUNDFILE
#define SOUNDFILE
#include <asndlib.h>
#include <gctypes.h>
void playSound(const u8 *filename, u32 len);
/**
 * you play pcm sound (converted with goldwave or sox) using this
 * @param filename the buffer you wanna use
 * @param len the buffer length
 * @param callback the callback, set to NULL if not required
 */
void playPCM(const u8 *filename, u32 len, void *callback);
#endif