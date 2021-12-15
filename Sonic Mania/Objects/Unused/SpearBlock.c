// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: SpearBlock Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled By Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

ObjectSpearBlock *SpearBlock = NULL;

void SpearBlock_Update(void)
{
    RSDK_THIS(SpearBlock);
    StateMachine_Run(self->state);
    SpearBlock_CheckPlayerCollisions();

    if (!RSDK.CheckOnScreen(self, &self->updateRange)) {
        self->state = SpearBlock_State_SetupSpears;
        SpearBlock_Create(NULL);
    }
}

void SpearBlock_LateUpdate(void) {}

void SpearBlock_StaticUpdate(void) {}

void SpearBlock_Draw(void)
{
    RSDK_THIS(SpearBlock);
    RSDK.DrawSprite(&self->animator, &self->spearPos, false);
    RSDK.DrawSprite(&SpearBlock->animator, NULL, false);
}

void SpearBlock_Create(void *data)
{
    RSDK_THIS(SpearBlock);
    self->visible          = true;
    self->drawOrder        = Zone->drawOrderLow;
    self->spearPos.x       = self->position.x;
    self->spearPos.y       = self->position.y;
    self->active           = ACTIVE_BOUNDS;
    self->updateRange.x    = 0x400000;
    self->updateRange.y    = 0x400000;
    RSDK.SetSpriteAnimation(SpearBlock->aniFrames, 1, &self->animator, true, 0);
    RSDK.SetSpriteAnimation(SpearBlock->aniFrames, 0, &SpearBlock->animator, true, 0);
    self->state = SpearBlock_State_SetupSpears;
}

void SpearBlock_StageLoad(void)
{
    SpearBlock->aniFrames = RSDK.LoadSpriteAnimation("Blueprint/SpearBlock.bin", SCOPE_STAGE);

    SpearBlock->blockHitbox.left        = -16;
    SpearBlock->blockHitbox.top         = -16;
    SpearBlock->blockHitbox.right       = 16;
    SpearBlock->blockHitbox.bottom      = 16;
    SpearBlock->spearHitboxes[0].left   = -4;
    SpearBlock->spearHitboxes[0].top    = -16;
    SpearBlock->spearHitboxes[0].right  = 4;
    SpearBlock->spearHitboxes[0].bottom = 16;
    SpearBlock->spearHitboxes[1].left   = -16;
    SpearBlock->spearHitboxes[1].top    = -4;
    SpearBlock->spearHitboxes[1].right  = 16;
    SpearBlock->spearHitboxes[1].bottom = 4;
    SpearBlock->spearHitboxes[2].left   = -4;
    SpearBlock->spearHitboxes[2].top    = -16;
    SpearBlock->spearHitboxes[2].right  = 4;
    SpearBlock->spearHitboxes[2].bottom = 16;
    SpearBlock->spearHitboxes[3].left   = -16;
    SpearBlock->spearHitboxes[3].top    = -4;
    SpearBlock->spearHitboxes[3].right  = 16;
    SpearBlock->spearHitboxes[3].bottom = 4;

    DebugMode_AddObject(SpearBlock->objectID, SpearBlock_DebugDraw, SpearBlock_DebugSpawn);
}

void SpearBlock_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(SpearBlock->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
}
void SpearBlock_DebugSpawn(void)
{
    RSDK_THIS(DebugMode);

    CREATE_ENTITY(SpearBlock, NULL, self->position.x, self->position.y);
}

void SpearBlock_CheckPlayerCollisions(void)
{
    RSDK_THIS(SpearBlock);
    foreach_active(Player, player) {
        Player_CheckCollisionBox(player, self, &SpearBlock->blockHitbox);
        Vector2 storePos;
        storePos.x         = self->position.x;
        storePos.y         = self->position.y;
        self->position.x = self->spearPos.x;
        self->position.y = self->spearPos.y;

        if (Player_CheckCollisionTouch(player, self, &SpearBlock->spearHitboxes[self->animator.frameID])) {
            if (player->state != Player_State_Hit && player->state != Player_State_Die && player->state != Player_State_Drown
                && !player->invincibleTimer && player->blinkTimer <= 0) {
                if (player->position.x > self->position.x)
                    player->velocity.x = 0x20000;
                else
                    player->velocity.x = -0x20000;
                Player_Hit(player);
            }
        }
        self->position.x = storePos.x;
        self->position.y = storePos.y;
    }
}

void SpearBlock_State_SetupSpears(void)
{
    RSDK_THIS(SpearBlock);
    self->active           = ACTIVE_NORMAL;
    self->state            = SpearBlock_State_CheckSpearExtend;
    if (!(Zone->timer & 0x7F)) {
        int32 frameTimer       = (Zone->timer >> 7) + self->spearDir;
        self->timer        = 4;
        self->state        = SpearBlock_State_ExtendSpears;
        self->animator.frameID = (frameTimer & 3);
    }
}

void SpearBlock_State_CheckSpearExtend(void)
{
    RSDK_THIS(SpearBlock);
    if (!(Zone->timer & 0x7F)) {
        int32 frameTimer       = (Zone->timer >> 7) + self->spearDir;
        self->timer        = 4;
        self->state        = SpearBlock_State_ExtendSpears;
        self->animator.frameID = (frameTimer & 3);
    }
}

void SpearBlock_State_ExtendSpears(void)
{
    RSDK_THIS(SpearBlock);
    switch (self->animator.frameID) {
        case FLIP_NONE: self->spearPos.y -= 0x80000; break;
        case FLIP_X: self->spearPos.x += 0x80000; break;
        case FLIP_Y: self->spearPos.y += 0x80000; break;
        case FLIP_XY: self->spearPos.x -= 0x80000; break;
        default: break;
    }

    self->timer--;
    if (!self->timer)
        self->state = SpearBlock_State_CheckSpearRetract;
}

void SpearBlock_State_CheckSpearRetract(void)
{
    RSDK_THIS(SpearBlock);
    if (!(Zone->timer & 0x3F)) {
        self->timer = 4;
        self->state = SpearBlock_State_RetractSpears;
    }
}

void SpearBlock_State_RetractSpears(void)
{
    RSDK_THIS(SpearBlock);
    switch (self->animator.frameID) {
        case FLIP_NONE: self->spearPos.y += 0x80000; break;
        case FLIP_X: self->spearPos.x -= 0x80000; break;
        case FLIP_Y: self->spearPos.y -= 0x80000; break;
        case FLIP_XY: self->spearPos.x += 0x80000; break;
        default: break;
    }

    self->timer--;
    if (!self->timer)
        self->state = SpearBlock_State_CheckSpearExtend;
}

#if RETRO_INCLUDE_EDITOR
void SpearBlock_EditorDraw(void) {}

void SpearBlock_EditorLoad(void) {}
#endif

void SpearBlock_Serialize(void) { RSDK_EDITABLE_VAR(SpearBlock, VAR_UINT8, spearDir); }
