// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: UIVideo Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectUIVideo *UIVideo;

void UIVideo_Update(void)
{
    RSDK_THIS(UIVideo);

    StateMachine_Run(self->state);
}

void UIVideo_LateUpdate(void) {}

void UIVideo_StaticUpdate(void) {}

void UIVideo_Draw(void) {}

void UIVideo_Create(void *data)
{
    RSDK_THIS(UIVideo);

    if (!SceneInfo->inEditor) {
        self->active    = ACTIVE_NORMAL;
        self->visible   = true;
        self->drawGroup = 0;

        UIVideo->playing = false;
        self->state      = UIVideo_State_PlayVideo1;
    }
}

void UIVideo_StageLoad(void) {}

bool32 UIVideo_SkipCB(void)
{
    if (ControllerInfo->keyStart.press || ControllerInfo->keyA.press || ControllerInfo->keyB.press) {
        Music_FadeOut(0.0125);

        UIVideo->playing = true;
        return true;
    }

    return false;
}

void UIVideo_State_PlayVideo1(void)
{
    RSDK_THIS(UIVideo);

    if (!self->timer)
        Music_Stop();

    if (++self->timer >= 16) {
        char videoFile1[64];
        char audioFile[64];
        RSDK.GetCString(audioFile, &self->audioFile);
        RSDK.GetCString(videoFile1, &self->videoFile1);

        int32 len = self->videoFile1.length;
#if MANIA_USE_PLUS
        if (videoFile1[len - 3] == 'p' && videoFile1[len - 2] == 'n' && videoFile1[len - 1] == 'g')
#else
        if (videoFile1[len - 3] == 't' && videoFile1[len - 2] == 'g' && videoFile1[len - 1] == 'a')
#endif
            RSDK.LoadImage(videoFile1, 32.0, 1.0, UIVideo_SkipCB);
        else
            RSDK.LoadVideo(videoFile1, 0.0, UIVideo_SkipCB);

        self->state = UIVideo_State_PlayVideo2;
        RSDK.PlayStream(audioFile, Music->channelID, 0, false, false);

        self->timer = 0;
    }
}
void UIVideo_State_PlayVideo2(void)
{
    RSDK_THIS(UIVideo);

    if (self->videoFile2.length && !UIVideo->playing) {
        char videoFile2[64];
        RSDK.GetCString(videoFile2, &self->videoFile2);
        RSDK.LoadVideo(videoFile2, 14.0, NULL);
    }

    self->state = UIVideo_State_FinishPlayback;
}
void UIVideo_State_FinishPlayback(void)
{
    RSDK_THIS(UIVideo);

    if (++self->timer == 120) {
        if (self->gotoStage) {
            char categoryName[64];
            char sceneName[64];
            RSDK.GetCString(categoryName, &self->stageListCategory);
            RSDK.GetCString(sceneName, &self->stageListName);

            RSDK.SetScene(categoryName, sceneName);

            if (!RSDK.CheckValidScene())
                RSDK.SetScene("Presentation", "Title Screen");

            RSDK.LoadScene();
        }

        destroyEntity(self);
    }
}

#if GAME_INCLUDE_EDITOR
void UIVideo_EditorDraw(void) {}

void UIVideo_EditorLoad(void) {}
#endif

void UIVideo_Serialize(void)
{
    RSDK_EDITABLE_VAR(UIVideo, VAR_STRING, videoFile1);
    RSDK_EDITABLE_VAR(UIVideo, VAR_STRING, videoFile2);
    RSDK_EDITABLE_VAR(UIVideo, VAR_STRING, audioFile);
    RSDK_EDITABLE_VAR(UIVideo, VAR_BOOL, gotoStage);
    RSDK_EDITABLE_VAR(UIVideo, VAR_STRING, stageListCategory);
    RSDK_EDITABLE_VAR(UIVideo, VAR_STRING, stageListName);
}
