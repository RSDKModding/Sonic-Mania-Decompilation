// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: BladePole Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectBladePole *BladePole;

void BladePole_Update(void)
{
    RSDK_THIS(BladePole);

    RSDK.ProcessAnimation(&self->bladeTopAnimator);
    RSDK.ProcessAnimation(&self->bladeBottomAnimator);

    StateMachine_Run(self->state);
}

void BladePole_LateUpdate(void) {}

void BladePole_StaticUpdate(void) {}

void BladePole_Draw(void) { BladePole_DrawSprites(); }

void BladePole_Create(void *data)
{
    RSDK_THIS(BladePole);

    self->active        = ACTIVE_BOUNDS;
    self->visible       = true;
    self->drawGroup     = Zone->objectDrawGroup[0];
    self->drawFX        = FX_FLIP;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;

    RSDK.SetSpriteAnimation(BladePole->aniFrames, 0, &self->poleAnimator, true, 0);
    RSDK.SetSpriteAnimation(BladePole->aniFrames, 1, &self->bladeTopAnimator, true, 0);
    RSDK.SetSpriteAnimation(BladePole->aniFrames, 1, &self->bladeBottomAnimator, true, 0);

    // Why is this initialized here?? wouldn't StageLoad be better?
    BladePole->hitboxBottom.left   = -24;
    BladePole->hitboxBottom.top    = 20;
    BladePole->hitboxBottom.right  = 24;
    BladePole->hitboxBottom.bottom = BladePole->hitboxBottom.top + 24;

    BladePole->hitboxTop.left   = -24;
    BladePole->hitboxTop.top    = -32;
    BladePole->hitboxTop.right  = 24;
    BladePole->hitboxTop.bottom = BladePole->hitboxTop.top + 24;

    self->state = BladePole_State_TopBladeActive;
}

void BladePole_StageLoad(void) { BladePole->aniFrames = RSDK.LoadSpriteAnimation("MMZ/BladePole.bin", SCOPE_STAGE); }

void BladePole_DrawSprites(void)
{
    RSDK_THIS(BladePole);

    self->direction = FLIP_NONE;
    Vector2 drawPos = self->position;
    RSDK.DrawSprite(&self->poleAnimator, &drawPos, false);

    drawPos.y -= 0xC0000;
    RSDK.DrawSprite(&self->poleAnimator, &drawPos, false);

    drawPos.y -= 0xC0000;
    RSDK.DrawSprite(&self->poleAnimator, &drawPos, false);

    drawPos.x = self->position.x;
    drawPos.y = self->position.y - 0x180000;
    RSDK.DrawSprite(&self->bladeTopAnimator, &drawPos, false);

    if (SceneInfo->inEditor) {
        drawPos.y += 0x180000;
    }
    else {
        self->direction = FLIP_Y;
        drawPos.y += 0x3C0000;
    }

    RSDK.DrawSprite(&self->bladeBottomAnimator, &drawPos, false);
    self->direction = FLIP_NONE;
}

bool32 BladePole_SetAnimation(Animator *animator)
{
    RSDK_THIS(BladePole);

    switch (animator->animationID) {
        case 1: RSDK.SetSpriteAnimation(BladePole->aniFrames, 2, animator, true, 0); break;

        case 2:
            if (animator->frameID == animator->frameCount - 1)
                RSDK.SetSpriteAnimation(BladePole->aniFrames, 3, animator, true, 0);
            break;

        case 3:
            if (self->timer >= 74 && !animator->frameID)
                RSDK.SetSpriteAnimation(BladePole->aniFrames, 4, animator, true, 0);
            break;

        case 4:
            if (animator->frameID == animator->frameCount - 1) {
                RSDK.SetSpriteAnimation(BladePole->aniFrames, 1, animator, true, 0);
                return true;
            }
            break;

        default: break;
    }

    return false;
}

void BladePole_CheckPlayerCollisions(Hitbox *hitbox)
{
    RSDK_THIS(BladePole);

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, hitbox)) {
            int32 storeX = self->position.x;
            int32 storeY = self->position.y;

            self->position.y = ((BladePole->hitboxBottom.bottom - BladePole->hitboxBottom.top) << 15) + storeY;
#if MANIA_USE_PLUS
            if (!Player_CheckMightyUnspin(player, 0x400, 2, &player->uncurlTimer))
#endif
                Player_Hurt(player, self);

            self->position.x = storeX;
            self->position.y = storeY;
        }
    }
}

void BladePole_State_TopBladeActive(void)
{
    RSDK_THIS(BladePole);

    if (BladePole_SetAnimation(&self->bladeTopAnimator)) {
        self->state = BladePole_State_BottomBladeActive;
        self->timer = 0;
    }

    if (self->bladeTopAnimator.animationID == 3)
        BladePole_CheckPlayerCollisions(&BladePole->hitboxTop);

    ++self->timer;
}

void BladePole_State_BottomBladeActive(void)
{
    RSDK_THIS(BladePole);

    if (BladePole_SetAnimation(&self->bladeBottomAnimator)) {
        self->state = BladePole_State_TopBladeActive;
        self->timer = 0;
    }

    if (self->bladeBottomAnimator.animationID == 3)
        BladePole_CheckPlayerCollisions(&BladePole->hitboxBottom);

    ++self->timer;
}

#if GAME_INCLUDE_EDITOR
void BladePole_EditorDraw(void) { BladePole_DrawSprites(); }

void BladePole_EditorLoad(void) { BladePole->aniFrames = RSDK.LoadSpriteAnimation("MMZ/BladePole.bin", SCOPE_STAGE); }
#endif

void BladePole_Serialize(void) {}
