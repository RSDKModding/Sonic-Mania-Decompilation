#include "RSDK/Core/RetroEngine.hpp"

FileInfo videoFile;

THEORAPLAY_Decoder *videoDecoder;
const THEORAPLAY_VideoFrame *videoFrameData;
THEORAPLAY_Io callbacks;

bool32 videoPlaying = false;
int32 vidFrameMS    = 0;
int32 vidBaseticks  = 0;
int32 videoWidth    = 0;
int32 videoHeight   = 0;
float videoAR       = 0;

long videoRead(THEORAPLAY_Io *io, void *buf, long buflen)
{
    FileInfo *file  = (FileInfo *)io->userdata;
    const size_t br = (size_t)ReadBytes(file, buf, (int32)buflen);
    if (br == 0)
        return -1;
    return (int)br;
}

void videoClose(THEORAPLAY_Io *io)
{
    FileInfo *info = (FileInfo *)io->userdata;
    CloseFile(info);
}

void LoadVideo(const char *filename, double a2, bool32 (*skipCallback)(void))
{
    if (sceneInfo.state == ENGINESTATE_VIDEOPLAYBACK)
        return;

    char buffer[0x80];
    sprintf(buffer, "Data/Video/%s", filename);

    InitFileInfo(&videoFile);
    if (LoadFile(&videoFile, buffer, FMODE_RB)) {

        callbacks.read     = videoRead;
        callbacks.close    = videoClose;
        callbacks.userdata = (void *)&videoFile;
#if RETRO_USING_SDL2
        videoDecoder = THEORAPLAY_startDecode(&callbacks, /*FPS*/ 60, THEORAPLAY_VIDFMT_IYUV);
#endif

        // TODO: does SDL1.2 support YUV?
#if RETRO_USING_SDL1
        videoDecoder = THEORAPLAY_startDecode(&callbacks, /*FPS*/ 60, THEORAPLAY_VIDFMT_RGBA);
#endif

        if (!videoDecoder) {
            PrintLog(PRINT_NORMAL, "Video Decoder Error!");
            CloseFile(&videoFile);
            return;
        }
        while (!videoFrameData) {
            if (!videoFrameData)
                videoFrameData = THEORAPLAY_getVideo(videoDecoder);
        }
        if (!videoFrameData) {
            PrintLog(PRINT_NORMAL, "Video Error!");
            CloseFile(&videoFile);
            return;
        }

        videoWidth  = videoFrameData->width;
        videoHeight = videoFrameData->height;
        videoAR = float(videoWidth) / float(videoHeight);

        SetupVideoBuffer(videoWidth, videoHeight);
        vidBaseticks = SDL_GetTicks();
        vidFrameMS   = (videoFrameData->fps == 0.0) ? 0 : ((Uint32)(1000.0 / videoFrameData->fps));
        videoPlaying = true;

        engine.displayTime = 0.0;
        /*engine.videoUnknown = 0.0;
        if (dword_66B80C == 1)
            engine.videoUnknown = a2;*/
        engine.prevShaderID   = engine.shaderID;
        engine.screenCount    = 0;
        engine.prevEngineMode = sceneInfo.state;
        // if (yuv_mode)
        //    gameSettings.shaderID = (yuv_mode != 2) + SHADER_YUV_422;
        // else
        //    gameSettings.shaderID = SHADER_YUV_420; // YUV-420
        engine.skipCallback = NULL;
        ProcessVideo();
        engine.skipCallback = skipCallback;
        RSDK::SKU::settingsChanged = false;
        sceneInfo.state     = ENGINESTATE_VIDEOPLAYBACK;
    }
}

int32 ProcessVideo() {
    if (engine.skipCallback) {
        if (engine.skipCallback()) {
            StopVideoPlayback();
            return 1; // video finished
        }
    }

    if (videoPlaying) {
        if (!THEORAPLAY_isDecoding(videoDecoder)) {
            StopVideoPlayback();
            return 1; // video finished
        }

        // Don't pause or it'll go wild
        if (videoPlaying) {
            const Uint32 now = (SDL_GetTicks() - vidBaseticks);

            if (!videoFrameData)
                videoFrameData = THEORAPLAY_getVideo(videoDecoder);

            // Play video frames when it's time.
            if (videoFrameData && (videoFrameData->playms <= now)) {
                //Removed the lagging handler code, so if its lagging the uh oh, but it should make everything else smoother

                int half_w     = videoFrameData->width / 2;
                const Uint8 *y = (const Uint8 *)videoFrameData->pixels;
                const Uint8 *u = y + (videoFrameData->width * videoFrameData->height);
                const Uint8 *v = u + (half_w * (videoFrameData->height / 2));

#if RETRO_USING_SDL2
                SDL_UpdateYUVTexture(engine.imageTexture, NULL, y, videoFrameData->width, u, half_w, v, half_w);
#endif
#if RETRO_USING_SDL1
                uint *videoFrameBuffer = (uint *)Engine.imageTexture->pixels;
                memcpy(videoFrameBuffer, videoFrameData->pixels, videoFrameData->xsize * videoFrameData->ysize * sizeof(uint));
#endif

                THEORAPLAY_freeVideo(videoFrameData);
                videoFrameData = NULL;
            }

            return 2; // its playing as expected
        }
    }

    return 0; // its not even initialised
}

void StopVideoPlayback()
{
    if (videoPlaying) {
        if (videoFrameData) {
            THEORAPLAY_freeVideo(videoFrameData);
            videoFrameData = NULL;
        }
        if (videoDecoder) {
            THEORAPLAY_stopDecode(videoDecoder);
            videoDecoder = NULL;
        }

        CloseVideoBuffer();
        videoPlaying = false;

        engine.shaderID    = engine.prevShaderID;
        sceneInfo.state    = engine.prevEngineMode;
        engine.screenCount = 1;
    }
}

void SetupVideoBuffer(int32 width, int32 height)
{
    SetImageTexture(width, height, NULL);

    if (!engine.imageTexture)
        PrintLog(PRINT_ERROR, "Failed to create video buffer!");
}

void CloseVideoBuffer()
{
    if (videoPlaying) {
#if RETRO_USING_SDL1
        SDL_FreeSurface(engine.imageTexture);
#endif
#if RETRO_USING_SDL2
        SDL_DestroyTexture(engine.imageTexture);
#endif
        engine.imageTexture = nullptr;
    }
}
