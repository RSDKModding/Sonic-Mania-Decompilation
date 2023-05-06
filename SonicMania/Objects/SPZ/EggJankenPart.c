// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: EggJankenPart Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectEggJankenPart *EggJankenPart = NULL;

void EggJankenPart_Update(void)
{
    RSDK_THIS(EggJankenPart);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->velocity.y += 0x3800;

    self->rotation += self->angle;

    if (!RSDK.CheckOnScreen(self, NULL))
        destroyEntity(self);
}

void EggJankenPart_LateUpdate(void) {}

void EggJankenPart_StaticUpdate(void) {}

void EggJankenPart_Draw(void)
{
    RSDK_THIS(EggJankenPart);

    if (Zone->timer & 1)
        RSDK.DrawSprite(&self->animator, NULL, false);
}

void EggJankenPart_Create(void *data)
{
    RSDK_THIS(EggJankenPart);

    RSDK.SetSpriteAnimation(EggJankenPart->aniFrames, 6, &self->animator, true, 0);

    self->active        = ACTIVE_BOUNDS;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;
    self->visible       = true;
    self->drawGroup     = Zone->objectDrawGroup[0] + 1;
    self->drawFX |= FX_ROTATE | FX_FLIP;

    self->animator.frameID = VOID_TO_INT(data);
}

void EggJankenPart_StageLoad(void) { EggJankenPart->aniFrames = RSDK.LoadSpriteAnimation("SPZ2/EggJanken.bin", SCOPE_STAGE); }

#if GAME_INCLUDE_EDITOR
void EggJankenPart_EditorDraw(void)
{
    RSDK_THIS(EggJankenPart);
    RSDK.DrawSprite(&self->animator, NULL, false);
}

void EggJankenPart_EditorLoad(void) { EggJankenPart->aniFrames = RSDK.LoadSpriteAnimation("SPZ2/EggJanken.bin", SCOPE_STAGE); }
#endif

void EggJankenPart_Serialize(void) {}
