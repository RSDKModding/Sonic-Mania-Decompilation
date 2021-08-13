#include "SonicMania.h"

ObjectUIVideo *UIVideo;

void UIVideo_Update(void)
{
    RSDK_THIS(UIVideo);
    StateMachine_Run(entity->state);
}

void UIVideo_LateUpdate(void) {}

void UIVideo_StaticUpdate(void) {}

void UIVideo_Draw(void) {}

void UIVideo_Create(void *data)
{
    RSDK_THIS(UIVideo);
    if (!RSDK_sceneInfo->inEditor) {
        entity->active    = ACTIVE_NORMAL;
        entity->visible   = true;
        entity->drawOrder = 0;
        UIVideo->playing  = false;
        entity->state     = UIVideo_State_PlayVid1;
    }
}

void UIVideo_StageLoad(void) {}

bool32 UIVideo_SkipCallback(void)
{
    if (RSDK_controller->keyStart.press && RSDK_controller->keyA.press && RSDK_controller->keyB.press) {
        Music_FadeOut(0.0125);
        UIVideo->playing = true;
        return true;
    }
    return false;
}

void UIVideo_State_PlayVid1(void)
{
    RSDK_THIS(UIVideo);
    if (!entity->timer)
        RSDK.StopChannel(Music->channelID);

    if (++entity->timer >= 16) {
        char videoFile1[64];
        char audioFile[64];
        RSDK.GetCString(audioFile, &entity->audioFile);
        RSDK.GetCString(videoFile1, &entity->videoFile1);
        int len = entity->videoFile1.textLength;
#if RETRO_USE_PLUS
        if (videoFile1[len - 3] == 'p' && videoFile1[len - 2] == 'n' && videoFile1[len - 1] == 'g')
#else
        if (videoFile1[len - 3] == 't' && videoFile1[len - 2] == 'g' && videoFile1[len - 1] == 'a')
#endif
            RSDK.LoadImage(videoFile1, 1.0, 32.0, UIVideo_SkipCallback);
        else
            RSDK.LoadVideo(videoFile1, 0.0, UIVideo_SkipCallback);
        entity->state = UIVideo_State_PlayVid2;
        RSDK.PlayStream(audioFile, Music->channelID, 0, 0, 0);
        entity->timer = 0;
    }
}
void UIVideo_State_PlayVid2(void)
{
    RSDK_THIS(UIVideo);
    if (entity->videoFile2.textLength) {
        if (!UIVideo->playing) {
            char videoFile2[64];
            RSDK.GetCString(videoFile2, &entity->videoFile2);
            RSDK.LoadVideo(videoFile2, 14.0, 0);
        }
    }
    entity->state = UIVideo_State_FinishPlayback;
}
void UIVideo_State_FinishPlayback(void)
{
    RSDK_THIS(UIVideo);
    if (++entity->timer == 120) {
        if (entity->gotoStage) {
            char catName[64];
            char sceneName[64];
            RSDK.GetCString(catName, &entity->stageListCategory);
            RSDK.GetCString(sceneName, &entity->stageListName);
            RSDK.SetScene(catName, sceneName);
            if (!RSDK.CheckValidScene())
                RSDK.SetScene("Presentation", "Title Screen");
            RSDK.LoadScene();
        }
        destroyEntity(entity);
    }
}

void UIVideo_EditorDraw(void) {}

void UIVideo_EditorLoad(void) {}

void UIVideo_Serialize(void)
{
    RSDK_EDITABLE_VAR(UIVideo, VAR_STRING, videoFile1);
    RSDK_EDITABLE_VAR(UIVideo, VAR_STRING, videoFile2);
    RSDK_EDITABLE_VAR(UIVideo, VAR_STRING, audioFile);
    RSDK_EDITABLE_VAR(UIVideo, VAR_BOOL, gotoStage);
    RSDK_EDITABLE_VAR(UIVideo, VAR_STRING, stageListCategory);
    RSDK_EDITABLE_VAR(UIVideo, VAR_STRING, stageListName);
}
