#ifndef VIDEO_H
#define VIDEO_H

void LoadVideo(const char *filename, double a2, bool32 (*skipCallback)(void));
int32 ProcessVideo();
void StopVideoPlayback();
void SetupVideoBuffer(int32 width, int32 height);
void CloseVideoBuffer();

#endif // VIDEO_H
