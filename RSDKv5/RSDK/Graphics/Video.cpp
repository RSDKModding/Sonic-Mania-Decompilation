#include "RSDK/Core/RetroEngine.hpp"

using namespace RSDK;

FileInfo VideoManager::file;

ogg_sync_state VideoManager::oy;
ogg_page VideoManager::og;
ogg_packet VideoManager::op;
ogg_stream_state VideoManager::vo;
ogg_stream_state VideoManager::to;
th_info VideoManager::ti;
th_comment VideoManager::tc;
th_dec_ctx *VideoManager::td    = NULL;
th_setup_info *VideoManager::ts = NULL;

th_pixel_fmt VideoManager::pixelFormat;
ogg_int64_t VideoManager::granulePos = 0;
bool32 VideoManager::shouldInit      = false;

bool32 RSDK::LoadVideo(const char *filename, double startDelay, bool32 (*skipCallback)())
{
    if (sceneInfo.state == ENGINESTATE_VIDEOPLAYBACK)
        return false;

    char fullFilePath[0x80];
    sprintf_s(fullFilePath, (int32)sizeof(fullFilePath), "Data/Video/%s", filename);

    InitFileInfo(&VideoManager::file);
    if (LoadFile(&VideoManager::file, fullFilePath, FMODE_RB)) {

        // Init
        ogg_sync_init(&VideoManager::oy);

        th_comment_init(&VideoManager::tc);
        th_info_init(&VideoManager::ti);

        int32 theora_p = 0;
        char *buffer   = NULL;

        // Parse header stuff
        bool32 finishedHeader = false;
        while (!finishedHeader) {
            buffer    = ogg_sync_buffer(&VideoManager::oy, 0x1000);
            int32 ret = (int32)ReadBytes(&VideoManager::file, buffer, 0x1000);
            ogg_sync_wrote(&VideoManager::oy, 0x1000);

            if (ret == 0)
                break;

            while (ogg_sync_pageout(&VideoManager::oy, &VideoManager::og) > 0) {
                ogg_stream_state test;

                /* is this a mandated initial header? If not, stop parsing */
                if (!ogg_page_bos(&VideoManager::og)) {
                    /* don't leak the page; get it into the appropriate stream */
                    ogg_stream_pagein(&VideoManager::to, &VideoManager::og);
                    finishedHeader = 1;
                    break;
                }

                ogg_stream_init(&test, ogg_page_serialno(&VideoManager::og));
                ogg_stream_pagein(&test, &VideoManager::og);
                ogg_stream_packetout(&test, &VideoManager::op);

                // identify codec
                if (!theora_p && th_decode_headerin(&VideoManager::ti, &VideoManager::tc, &VideoManager::ts, &VideoManager::op) >= 0) {
                    // theora
                    memcpy(&VideoManager::to, &test, sizeof(test));
                    theora_p = 1;
                }
                else {
                    // we dont care (possibly vorbis)
                    ogg_stream_clear(&test);
                }
            }
        }

        if (theora_p) {
            VideoManager::ts = NULL;
            theora_p         = 1;
            while (theora_p && theora_p < 3) {
                int32 ret;

                while (theora_p && (theora_p < 3) && (ret = ogg_stream_packetout(&VideoManager::to, &VideoManager::op))) {
                    if (ret < 0) {
#if !RETRO_USE_ORIGINAL_CODE
                        PrintLog(PRINT_NORMAL, "ERROR: failed to parse theora stream headers. corrupted stream?");
#endif

                        theora_p = 0;
                        break;
                    }

                    if (!th_decode_headerin(&VideoManager::ti, &VideoManager::tc, &VideoManager::ts, &VideoManager::op)) {
#if !RETRO_USE_ORIGINAL_CODE
                        PrintLog(PRINT_NORMAL, "ERROR: failed to parse theora stream headers. corrupted stream?");
#endif

                        theora_p = 0;
                        break;
                    }

                    theora_p++;
                }

                if (!theora_p)
                    break;

                /* The header pages/packets will arrive before anything else we
                   care about, or the stream is not obeying spec */

                if (ogg_sync_pageout(&VideoManager::oy, &VideoManager::og) > 0) {
                    ogg_stream_pagein(&VideoManager::to, &VideoManager::og);
                }
                else {
                    buffer    = ogg_sync_buffer(&VideoManager::oy, 0x1000);
                    int32 ret = (int32)ReadBytes(&VideoManager::file, buffer, 0x1000);
                    ogg_sync_wrote(&VideoManager::oy, 0x1000);
                    if (ret == 0) {
#if !RETRO_USE_ORIGINAL_CODE
                        PrintLog(PRINT_NORMAL, "ERROR: Reached end of file while searching for codec headers.");
#endif

                        theora_p = 0;
                    }
                }
            }

            if (!theora_p) {
                th_info_clear(&VideoManager::ti);
                th_comment_clear(&VideoManager::tc);
                th_setup_free(VideoManager::ts);
            }
            else {
                VideoManager::td          = th_decode_alloc(&VideoManager::ti, VideoManager::ts);
                VideoManager::pixelFormat = VideoManager::ti.pixel_fmt;

                int32 ppLevelMax = 0;
                th_decode_ctl(VideoManager::td, TH_DECCTL_GET_PPLEVEL_MAX, &ppLevelMax, sizeof(int32));
                int32 ppLevel = 0;
                th_decode_ctl(VideoManager::td, TH_DECCTL_SET_PPLEVEL, &ppLevel, sizeof(int32));

                th_setup_free(VideoManager::ts);

                engine.storedShaderID     = videoSettings.shaderID;
                videoSettings.screenCount = 0;
                engine.storedState        = sceneInfo.state;
                engine.displayTime        = 0.0;
                VideoManager::shouldInit  = true;
                VideoManager::granulePos  = 0;

                engine.displayTime     = 0.0;
                engine.videoStartDelay = 0.0;
                if (AudioDevice::audioState == 1)
                    engine.videoStartDelay = startDelay;

                switch (VideoManager::pixelFormat) {
                    default: break;
                    case TH_PF_420: videoSettings.shaderID = SHADER_YUV_420; break;
                    case TH_PF_422: videoSettings.shaderID = SHADER_YUV_422; break;
                    case TH_PF_444: videoSettings.shaderID = SHADER_YUV_444; break;
                }

                engine.skipCallback = NULL;
                ProcessVideo();
                engine.skipCallback = skipCallback;

                changedVideoSettings = false;
                sceneInfo.state      = ENGINESTATE_VIDEOPLAYBACK;

                return true;
            }
        }

        CloseFile(&VideoManager::file);
    }

    return false;
}

void RSDK::ProcessVideo()
{
    bool32 finished = false;
    double curTime  = 0;
    if (!VideoManager::shouldInit) {
        double streamPos = GetVideoStreamPos();

        if (streamPos <= -1.0)
            engine.displayTime += (1.0 / 60.0); // deltaTime frame-step
        else
            engine.displayTime = streamPos;

        curTime = th_granule_time(VideoManager::td, VideoManager::granulePos);

        if (engine.skipCallback && engine.skipCallback()) {
            finished = true;
        }
    }

    if (!finished && (VideoManager::shouldInit || engine.displayTime >= engine.videoStartDelay + curTime)) {
        while (ogg_stream_packetout(&VideoManager::to, &VideoManager::op) <= 0) {
            char *buffer = ogg_sync_buffer(&VideoManager::oy, 0x1000);
            if (!(VideoManager::shouldInit | ReadBytes(&VideoManager::file, buffer, 0x1000))) {
                finished = true;
                break;
            }

            ogg_sync_wrote(&VideoManager::oy, 0x1000);

            while (ogg_sync_pageout(&VideoManager::oy, &VideoManager::og) > 0) ogg_stream_pagein(&VideoManager::to, &VideoManager::og);
        }

        if (!finished && !th_decode_packetin(VideoManager::td, &VideoManager::op, &VideoManager::granulePos)) {
            th_ycbcr_buffer yuv;
            th_decode_ycbcr_out(VideoManager::td, yuv);

            int32 dataPos = (VideoManager::ti.pic_x & 0xFFFFFFFE) + (VideoManager::ti.pic_y & 0xFFFFFFFE) * yuv[0].stride;
            switch (VideoManager::pixelFormat) {
                default: break;

                case TH_PF_444:
                    RenderDevice::SetupVideoTexture_YUV444(yuv[0].width, yuv[0].height, &yuv[0].data[dataPos], &yuv[1].data[dataPos],
                                                           &yuv[2].data[dataPos], yuv[0].stride, yuv[1].stride, yuv[2].stride);
                    break;

                case TH_PF_422:
                    RenderDevice::SetupVideoTexture_YUV422(yuv[0].width, yuv[0].height, &yuv[0].data[dataPos],
                                                           &yuv[1].data[yuv[1].stride * VideoManager::ti.pic_y + (VideoManager::ti.pic_x >> 1)],
                                                           &yuv[2].data[yuv[1].stride * VideoManager::ti.pic_y + (VideoManager::ti.pic_x >> 1)],
                                                           yuv[0].stride, yuv[1].stride, yuv[2].stride);
                    break;

                case TH_PF_420:
                    RenderDevice::SetupVideoTexture_YUV420(
                        yuv[0].width, yuv[0].height, &yuv[0].data[dataPos],
                        &yuv[1].data[yuv[1].stride * (VideoManager::ti.pic_y >> 1) + (VideoManager::ti.pic_x >> 1)],
                        &yuv[2].data[yuv[1].stride * (VideoManager::ti.pic_y >> 1) + (VideoManager::ti.pic_x >> 1)], yuv[0].stride, yuv[1].stride,
                        yuv[2].stride);
                    break;
            }
        }

        VideoManager::shouldInit = false;
    }

    if (finished) {
        CloseFile(&VideoManager::file);

        // Flush everything out
        while (ogg_sync_pageout(&VideoManager::oy, &VideoManager::og) > 0) ogg_stream_pagein(&VideoManager::to, &VideoManager::og);

        ogg_stream_clear(&VideoManager::to);
        th_decode_free(VideoManager::td);
        th_comment_clear(&VideoManager::tc);
        th_info_clear(&VideoManager::ti);
        ogg_sync_clear(&VideoManager::oy);

        videoSettings.shaderID    = engine.storedShaderID;
        videoSettings.screenCount = 1;
        sceneInfo.state           = engine.storedState;
    }
}
