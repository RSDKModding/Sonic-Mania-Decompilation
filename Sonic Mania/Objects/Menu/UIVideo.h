#ifndef OBJ_UIVIDEO_H
#define OBJ_UIVIDEO_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    bool32 playing;
    int32 field_8;
} ObjectUIVideo;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    TextInfo videoFile1;
    TextInfo videoFile2;
    TextInfo audioFile;
    bool32 gotoStage;
    TextInfo stageListCategory;
    TextInfo stageListName;
    int32 timer;
    int32 field_8C;
    int32 field_90;
    int32 field_94;
    int32 field_98;
    int32 field_9C;
    int32 field_A0;
} EntityUIVideo;

// Object Struct
extern ObjectUIVideo *UIVideo;

// Standard Entity Events
void UIVideo_Update(void);
void UIVideo_LateUpdate(void);
void UIVideo_StaticUpdate(void);
void UIVideo_Draw(void);
void UIVideo_Create(void* data);
void UIVideo_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void UIVideo_EditorDraw(void);
void UIVideo_EditorLoad(void);
#endif
void UIVideo_Serialize(void);

// Extra Entity Functions
bool32 UIVideo_SkipCallback(void);

void UIVideo_State_PlayVid1(void);
void UIVideo_State_PlayVid2(void);
void UIVideo_State_FinishPlayback(void);

#endif //!OBJ_UIVIDEO_H
