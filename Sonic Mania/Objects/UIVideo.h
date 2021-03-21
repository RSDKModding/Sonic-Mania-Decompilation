#ifndef OBJ_UIVIDEO_H
#define OBJ_UIVIDEO_H

#include "../SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    bool32 playing;
    int field_8;
} ObjectUIVideo;

// Entity Class
typedef struct {
    RSDK_ENTITY
    void (*state)();
    TextInfo videoFile1;
    TextInfo videoFile2;
    TextInfo audioFile;
    bool32 gotoStage;
    TextInfo stageListCategory;
    TextInfo stageListName;
    int timer;
    char field_8C;
    char field_8D;
    char field_8E;
    char field_8F;
    char field_90;
    char field_91;
    char field_92;
    char field_93;
    char field_94;
    char field_95;
    char field_96;
    char field_97;
    char field_98;
    char field_99;
    char field_9A;
    char field_9B;
    char field_9C;
    char field_9D;
    char field_9E;
    char field_9F;
    char field_A0;
    char field_A1;
    char field_A2;
    char field_A3;
} EntityUIVideo;

// Object Struct
extern ObjectUIVideo *UIVideo;

// Standard Entity Events
void UIVideo_Update();
void UIVideo_LateUpdate();
void UIVideo_StaticUpdate();
void UIVideo_Draw();
void UIVideo_Create(void* data);
void UIVideo_StageLoad();
void UIVideo_EditorDraw();
void UIVideo_EditorLoad();
void UIVideo_Serialize();

// Extra Entity Functions
bool32 UIVideo_SkipCallback();

void UIVideo_State_PlayVid1();
void UIVideo_State_PlayVid2();
void UIVideo_State_FinishPlayback();

#endif //!OBJ_UIVIDEO_H
