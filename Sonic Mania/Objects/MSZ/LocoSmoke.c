// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: LocoSmoke Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

ObjectLocoSmoke *LocoSmoke;

void LocoSmoke_Update(void)
{
    RSDK_THIS(LocoSmoke);
    RSDK.ProcessAnimation(&self->animator);
    if (++self->timer == 26) {
        for (int32 i = 0; i < 8; ++i) {
            EntityDebris *debris = CREATE_ENTITY(Debris, NULL, self->position.x + RSDK.Rand(-0x280000, 0x180000),
                                                 self->position.y + RSDK.Rand(-0x480000, -0x180000));
            debris->state        = Debris_State_Fall;
            debris->gravity      = 512;
            debris->velocity.x   = RSDK.Rand(-0x20000, -0x8000);
            debris->velocity.y   = RSDK.Rand(-0x20000, -0x8000);
            debris->drawOrder    = Zone->drawOrderLow - 1;
            debris->timer        = 42;
            RSDK.SetSpriteAnimation(LocoSmoke->aniFrames, 9, &debris->animator, true, 0);
        }
    }
    if (self->timer > 32) {
        self->alpha -= 16;
        if (self->alpha <= 0)
            destroyEntity(self);
    }
}

void LocoSmoke_LateUpdate(void) {}

void LocoSmoke_StaticUpdate(void) {}

void LocoSmoke_Draw(void)
{
    RSDK_THIS(LocoSmoke);
    RSDK.DrawSprite(&self->animator, NULL, false);
}

void LocoSmoke_Create(void *data)
{
    RSDK_THIS(LocoSmoke);
    self->active    = ACTIVE_NORMAL;
    self->visible   = 1;
    self->drawOrder = Zone->drawOrderLow;
    self->inkEffect = INK_ALPHA;
    self->alpha     = 0x100;
    RSDK.SetSpriteAnimation(LocoSmoke->aniFrames, 8, &self->animator, true, 0);
}

void LocoSmoke_StageLoad(void) { LocoSmoke->aniFrames = RSDK.LoadSpriteAnimation("MSZ/Train.bin", SCOPE_STAGE); }

#if RETRO_INCLUDE_EDITOR
void LocoSmoke_EditorDraw(void) {}

void LocoSmoke_EditorLoad(void) {}
#endif

void LocoSmoke_Serialize(void) {}
