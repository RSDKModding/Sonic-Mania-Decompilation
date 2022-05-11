#ifndef VIDEO_H
#define VIDEO_H

void LoadVideo(const char *filename, double a2, bool32 (*skipCallback)(void));
int32 ProcessVideo();

#endif // VIDEO_H
