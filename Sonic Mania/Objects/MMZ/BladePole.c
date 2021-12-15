// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: BladePole Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

ObjectBladePole *BladePole;

void BladePole_Update(void)
{
    RSDK_THIS(BladePole);
    RSDK.ProcessAnimation(&self->animator2);
    RSDK.ProcessAnimation(&self->animator3);
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
    self->drawOrder     = Zone->drawOrderLow;
    self->drawFX        = FX_FLIP;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;
    RSDK.SetSpriteAnimation(BladePole->aniFrames, 0, &self->animator1, true, 0);
    RSDK.SetSpriteAnimation(BladePole->aniFrames, 1, &self->animator2, true, 0);
    RSDK.SetSpriteAnimation(BladePole->aniFrames, 1, &self->animator3, true, 0);
    BladePole->hitbox2.top    = 20;
    BladePole->hitbox2.bottom = BladePole->hitbox2.top + 24;
    BladePole->hitbox2.left   = -24;
    BladePole->hitbox2.right  = 24;
    BladePole->hitbox1.top    = -32;
    BladePole->hitbox1.bottom = BladePole->hitbox1.top + 24;
    BladePole->hitbox1.left   = -24;
    BladePole->hitbox1.right  = 24;
    self->state             = BladePole_Unknown4;
}

void BladePole_StageLoad(void) { BladePole->aniFrames = RSDK.LoadSpriteAnimation("MMZ/BladePole.bin", SCOPE_STAGE); }

void BladePole_DrawSprites(void)
{
    RSDK_THIS(BladePole);
    Vector2 drawPos;

    self->direction = FLIP_NONE;
    drawPos.x         = self->position.x;
    drawPos.y         = self->position.y;
    RSDK.DrawSprite(&self->animator1, &drawPos, false);
    drawPos.y -= 0xC0000;
    RSDK.DrawSprite(&self->animator1, &drawPos, false);
    drawPos.y -= 0xC0000;
    RSDK.DrawSprite(&self->animator1, &drawPos, false);
    drawPos.x = self->position.x;
    drawPos.y = self->position.y - 0x180000;
    RSDK.DrawSprite(&self->animator2, &drawPos, false);
    if (SceneInfo->inEditor) {
        drawPos.y += 0x180000;
    }
    else {
        self->direction = FLIP_Y;
        drawPos.y += 0x3C0000;
    }

    RSDK.DrawSprite(&self->animator3, &drawPos, false);
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
            int32 storeX         = self->position.x;
            int32 storeY         = self->position.y;
            self->position.y = ((BladePole->hitbox2.bottom - BladePole->hitbox2.top) << 15) + storeY;
#if RETRO_USE_PLUS
            if (!Player_CheckMightyUnspin(0x400, player, 2, &player->uncurlTimer))
#endif
                Player_CheckHit(player, self);
            self->position.x = storeX;
            self->position.y = storeY;
        }
    }
}

void BladePole_Unknown4(void)
{
    RSDK_THIS(BladePole);

    if (BladePole_SetAnimation(&self->animator2)) {
        self->state = BladePole_Unknown5;
        self->timer = 0;
    }
    if (self->animator2.animationID == 3)
        BladePole_CheckPlayerCollisions(&BladePole->hitbox1);
    ++self->timer;
}

void BladePole_Unknown5(void)
{
    RSDK_THIS(BladePole);

    if (BladePole_SetAnimation(&self->animator3)) {
        self->state = BladePole_Unknown4;
        self->timer = 0;
    }
    if (self->animator3.animationID == 3)
        BladePole_CheckPlayerCollisions(&BladePole->hitbox2);
    ++self->timer;
}

#if RETRO_INCLUDE_EDITOR
void BladePole_EditorDraw(void) { BladePole_DrawSprites(); }

void BladePole_EditorLoad(void) { BladePole->aniFrames = RSDK.LoadSpriteAnimation("MMZ/BladePole.bin", SCOPE_STAGE); }
#endif

void BladePole_Serialize(void) {}
