#include "../SonicMania.h"

ObjectUIVideo *UIVideo;

void UIVideo_Update()
{
    RSDK_THIS(UIVideo);
    if (entity->state)
        entity->state();
}

void UIVideo_LateUpdate() {}

void UIVideo_StaticUpdate() {}

void UIVideo_Draw() {}

void UIVideo_Create(void *data)
{
    RSDK_THIS(UIVideo);
    if (!RSDK_sceneInfo->inEditor) {
        entity->active    = ACTIVE_NORMAL;
        entity->visible   = true;
        entity->drawOrder = 0;
        UIVideo->playing  = 0;
        entity->state     = UIVideo_State_PlayVid1;
    }
}

void UIVideo_StageLoad() {}

bool32 UIVideo_SkipCallback()
{
    if (RSDK_controller->keyStart.press && RSDK_controller->keyA.press && RSDK_controller->keyB.press) {
        if (Music->activeTrack != TRACK_DROWNING) {
            EntityMusic *music = (EntityMusic *)RSDK.GetEntityByID(SLOT_MUSIC);
            RSDK.ResetEntityPtr(music, Music->objectID, 0);
            music->state    = Music_Unknown14;
            music->volume   = 1.0;
            music->fadeSpeed = 0.0125;
        }
        UIVideo->playing = true;
        return true;
    }
    return false;
}

void UIVideo_State_PlayVid1()
{
    RSDK_THIS(UIVideo);
    if (!entity->timer)
        RSDK.SoundUnknown1(Music->slotID);

    if (++entity->timer >= 16) {
        char videoFile1[64];
        char audioFile[64];
        RSDK.GetCString(audioFile, &entity->audioFile);
        RSDK.GetCString(videoFile1, &entity->videoFile1);
        int len = entity->videoFile1.textLength;
        if (videoFile1[len - 3] == 'p' && videoFile1[len - 2] == 'n' && videoFile1[len - 1] == 'g')
            RSDK.LoadImage(videoFile1, 1.0, 32.0, UIVideo_SkipCallback);
        else
            RSDK.LoadVideo(videoFile1, 0.0, UIVideo_SkipCallback);
        entity->state = UIVideo_State_PlayVid2;
        RSDK.PlayMusic(audioFile, Music->slotID, 0, 0, 0);
        entity->timer = 0;
    }
}
void UIVideo_State_PlayVid2()
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
void UIVideo_State_FinishPlayback()
{
    RSDK_THIS(UIVideo);
    if (++entity->timer == 120) {
        if (entity->gotoStage) {
            char catName[64];
            char sceneName[64];
            RSDK.GetCString(catName, &entity->stageListCategory);
            RSDK.GetCString(sceneName, &entity->stageListName);
            RSDK.LoadScene(catName, sceneName);
            if (!RSDK.CheckValidScene())
                RSDK.LoadScene("Presentation", "Title Screen");
            RSDK.InitSceneLoad();
        }
        RSDK.ResetEntityPtr(entity, 0, 0);
    }
}

void UIVideo_EditorDraw() {}

void UIVideo_EditorLoad() {}

void UIVideo_Serialize()
{
    RSDK_EDITABLE_VAR(UIVideo, VAR_STRING, videoFile1);
    RSDK_EDITABLE_VAR(UIVideo, VAR_STRING, videoFile2);
    RSDK_EDITABLE_VAR(UIVideo, VAR_STRING, audioFile);
    RSDK_EDITABLE_VAR(UIVideo, VAR_BOOL, gotoStage);
    RSDK_EDITABLE_VAR(UIVideo, VAR_STRING, stageListCategory);
    RSDK_EDITABLE_VAR(UIVideo, VAR_STRING, stageListName);
}
