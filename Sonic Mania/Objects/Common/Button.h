#ifndef OBJ_BUTTON_H
#define OBJ_BUTTON_H

#include "SonicMania.h"

typedef enum { BUTTON_FLOOR, BUTTON_ROOF, BUTTON_RWALL, BUTTON_LWALL } ButtonTypes;

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 aniFrames;
    bool32 hasEggman;
    bool32 hasPhantomRider;
    Hitbox hitboxV;
    Hitbox hitboxH;
    int32 activatePos;
    int32 buttonOffset;
    int32 hitboxOffset;
    int32 unused1; // set in stageload, but never used. no way to tell what it is
    uint16 sfxButton;
    uint16 unused2; // i removed this earlier, but it cannot be padding because of how mania passes the size of the object and entity, so it stays
} ObjectButton;

// Entity Class
typedef struct {
    RSDK_ENTITY
    int32 type;
    bool32 walkOnto;
    uint8 tag;
    bool32 down;            // 64
    bool32 toggled;         // 68
    bool32 currentlyActive; // 6B
    bool32 activated;       // 70
    bool32 wasActivated;    // 74
    int32 pressPos;         // 78
    Hitbox hitbox;
    Animator buttonAnimator;
    Animator baseAnimator;
} EntityButton;

// Object Struct
extern ObjectButton *Button;

// Standard Entity Events
void Button_Update(void);
void Button_LateUpdate(void);
void Button_StaticUpdate(void);
void Button_Draw(void);
void Button_Create(void *data);
void Button_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void Button_EditorDraw(void);
void Button_EditorLoad(void);
#endif
void Button_Serialize(void);

// Extra Entity Functions
void Button_CheckEggmanCollisions(void);
void Button_CheckPRiderCollisions(void);

void Button_TypeFloor(void);
void Button_TypeRoof(void);
void Button_TypeRWall(void);
void Button_TypeLWall(void);

#endif //! OBJ_BUTTON_H
