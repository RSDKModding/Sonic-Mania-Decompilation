// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: HangGlider Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

#if MANIA_USE_PLUS
ObjectHangGlider *HangGlider;

void HangGlider_Update(void)
{
    RSDK_THIS(HangGlider);

    StateMachine_Run(self->state);
}

void HangGlider_LateUpdate(void) {}

void HangGlider_StaticUpdate(void) {}

void HangGlider_Draw(void)
{
    RSDK_THIS(HangGlider);

    if (SceneInfo->currentDrawGroup == self->drawGroup) {
        RSDK.DrawSprite(&self->sailAnimator, NULL, false);
        RSDK.DrawSprite(&self->handleBackAnimator, NULL, false);

        if (self->attachedPlayer)
            RSDK.DrawSprite(&self->playerAnimator, NULL, false);

        RSDK.AddDrawListRef(Zone->objectDrawGroup[1], SceneInfo->entitySlot);
    }
    else {
        RSDK.DrawSprite(&self->handleFrontAnimator, NULL, false);
    }
}

void HangGlider_Create(void *data)
{
    RSDK_THIS(HangGlider);

    if (!SceneInfo->inEditor) {
        self->visible         = true;
        self->drawGroup       = Zone->objectDrawGroup[0];
        self->active          = ACTIVE_BOUNDS;
        self->updateRange.x   = 0x400000;
        self->updateRange.y   = 0x400000;
        self->gravityStrength = 0x1000;
        self->state           = HangGlider_State_CheckGrab;

        RSDK.SetSpriteAnimation(HangGlider->aniFrames, 0, &self->sailAnimator, true, 0);
        RSDK.SetSpriteAnimation(HangGlider->aniFrames, 1, &self->handleBackAnimator, true, 0);
        RSDK.SetSpriteAnimation(HangGlider->aniFrames, 1, &self->handleFrontAnimator, true, 1);
    }
}

void HangGlider_StageLoad(void)
{
    if (RSDK.CheckSceneFolder("FBZ"))
        HangGlider->aniFrames = RSDK.LoadSpriteAnimation("FBZ/HangGlider.bin", SCOPE_STAGE);
    else
        HangGlider->aniFrames = RSDK.LoadSpriteAnimation("PSZ1/HangGlider.bin", SCOPE_STAGE);

    // A distance check was used instead
    HangGlider->hitboxUnused.left   = 12;
    HangGlider->hitboxUnused.top    = 8;
    HangGlider->hitboxUnused.right  = 24;
    HangGlider->hitboxUnused.bottom = 128;
}

void HangGlider_State_CheckGrab(void)
{
    RSDK_THIS(HangGlider);

    foreach_active(Player, player)
    {
        if (abs(player->position.x - self->position.x) < 0x40000 && abs(player->position.y - self->position.y) < 0x140000) {
            self->attachedPlayer = player;
            player->active       = ACTIVE_NEVER;
            player->visible      = false;
            self->velocity.x     = (0xC0 * player->velocity.x) >> 8;

            RSDK.SetSpriteAnimation(player->aniFrames, ANI_SPRING_DIAGONAL, &self->playerAnimator, true, 0);
            self->playerAnimator.rotationStyle = ROTSTYLE_FULL;
            self->rotation                     = 0x80;
            self->drawFX                       = FX_ROTATE;
            RSDK.PlaySfx(Player->sfxGrab, false, 255);
            self->state = HangGlider_State_Glide;
        }
    }
}
void HangGlider_State_Glide(void)
{
    RSDK_THIS(HangGlider);

    self->velocity.y += self->gravityStrength;
    if (self->velocity.y > 0x10000)
        self->velocity.y = 0x10000;

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;

    EntityPlayer *player = self->attachedPlayer;
    if (player) {
        player->position.x = self->position.x;
        player->position.y = self->position.y;
    }

    RSDK.ProcessAnimation(&self->sailAnimator);
    RSDK.ProcessAnimation(&self->playerAnimator);
}

#if GAME_INCLUDE_EDITOR
void HangGlider_EditorDraw(void)
{
    RSDK_THIS(HangGlider);

    RSDK.SetSpriteAnimation(HangGlider->aniFrames, 0, &self->sailAnimator, true, 0);
    RSDK.SetSpriteAnimation(HangGlider->aniFrames, 1, &self->handleBackAnimator, true, 0);
    RSDK.SetSpriteAnimation(HangGlider->aniFrames, 1, &self->handleFrontAnimator, true, 1);

    RSDK.DrawSprite(&self->sailAnimator, NULL, false);
    RSDK.DrawSprite(&self->handleBackAnimator, NULL, false);
    RSDK.DrawSprite(&self->handleFrontAnimator, NULL, false);
}

void HangGlider_EditorLoad(void)
{
    if (RSDK.CheckSceneFolder("FBZ"))
        HangGlider->aniFrames = RSDK.LoadSpriteAnimation("FBZ/HangGlider.bin", SCOPE_STAGE);
    else
        HangGlider->aniFrames = RSDK.LoadSpriteAnimation("PSZ1/HangGlider.bin", SCOPE_STAGE);
}
#endif

void HangGlider_Serialize(void) {}
#endif
