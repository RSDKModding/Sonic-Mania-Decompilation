#ifndef OBJ_CPZ2OUTRO_H
#define OBJ_CPZ2OUTRO_H

#include "../SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    int field_4;
    Entity *prisonPtr;
} ObjectCPZ2Outro;

// Entity Class
typedef struct {
    RSDK_ENTITY
    Vector2 size;
    bool32 activated;
    int field_64;
    int field_68;
    int field_6C;
    int field_70;
    int field_74;
    int field_78;
    Hitbox hitbox;
} EntityCPZ2Outro;

// Object Struct
extern ObjectCPZ2Outro *CPZ2Outro;

// Standard Entity Events
void CPZ2Outro_Update(void);
void CPZ2Outro_LateUpdate(void);
void CPZ2Outro_StaticUpdate(void);
void CPZ2Outro_Draw(void);
void CPZ2Outro_Create(void* data);
void CPZ2Outro_StageLoad(void);
void CPZ2Outro_EditorDraw(void);
void CPZ2Outro_EditorLoad(void);
void CPZ2Outro_Serialize(void);

// Extra Entity Functions
void CPZ2Outro_HandleCutsceneSeq(void);
bool32 CPZ2Outro_CutsceneSeq(void *h);

#endif //!OBJ_CPZ2OUTRO_H
