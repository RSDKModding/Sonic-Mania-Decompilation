#ifndef VIDEO_H
#define VIDEO_H

void LoadVideo(const char *filename, double a2, bool32 (*skipCallback)(void));
int ProcessVideo();
void StopVideoPlayback();
void SetupVideoBuffer(int width, int height);
void CloseVideoBuffer();

#endif // VIDEO_H
