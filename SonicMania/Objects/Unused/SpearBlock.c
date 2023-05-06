// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: SpearBlock Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

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
    self->visible       = true;
    self->drawGroup     = Zone->objectDrawGroup[0];
    self->spearPos.x    = self->position.x;
    self->spearPos.y    = self->position.y;
    self->active        = ACTIVE_BOUNDS;
    self->updateRange.x = 0x400000;
    self->updateRange.y = 0x400000;
    RSDK.SetSpriteAnimation(SpearBlock->aniFrames, 1, &self->animator, true, 0);
    RSDK.SetSpriteAnimation(SpearBlock->aniFrames, 0, &SpearBlock->animator, true, 0);
    self->state = SpearBlock_State_SetupSpears;
}

void SpearBlock_StageLoad(void)
{
    SpearBlock->aniFrames = RSDK.LoadSpriteAnimation("Blueprint/SpearBlock.bin", SCOPE_STAGE);

    SpearBlock->blockHitbox.left   = -16;
    SpearBlock->blockHitbox.top    = -16;
    SpearBlock->blockHitbox.right  = 16;
    SpearBlock->blockHitbox.bottom = 16;

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

    DEBUGMODE_ADD_OBJ(SpearBlock);
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

    foreach_active(Player, player)
    {
        Player_CheckCollisionBox(player, self, &SpearBlock->blockHitbox);
        Vector2 storePos = self->position;
        self->position.x = self->spearPos.x;
        self->position.y = self->spearPos.y;

        if (Player_CheckCollisionTouch(player, self, &SpearBlock->spearHitboxes[self->animator.frameID])) {
            self->position = storePos;
            Player_HurtFlip(player);
        }
        self->position = storePos;
    }
}

void SpearBlock_State_SetupSpears(void)
{
    RSDK_THIS(SpearBlock);

    self->active = ACTIVE_NORMAL;
    self->state  = SpearBlock_State_SpearRetracted;
    if (!(Zone->timer & 0x7F)) {
        int32 frameTimer       = (Zone->timer >> 7) + self->spearDir;
        self->timer            = 4;
        self->state            = SpearBlock_State_ExtendSpears;
        self->animator.frameID = (frameTimer & 3);
    }
}

void SpearBlock_State_SpearRetracted(void)
{
    RSDK_THIS(SpearBlock);
    if (!(Zone->timer & 0x7F)) {
        int32 frameTimer       = (Zone->timer >> 7) + self->spearDir;
        self->timer            = 4;
        self->state            = SpearBlock_State_ExtendSpears;
        self->animator.frameID = frameTimer & 3;
        // Spear Sfx should be played here, it's prolly not here in the code since this is just a test object, nothing more.
    }
}

void SpearBlock_State_ExtendSpears(void)
{
    RSDK_THIS(SpearBlock);
    switch (self->animator.frameID) {
        case 0: self->spearPos.y -= 0x80000; break;
        case 1: self->spearPos.x += 0x80000; break;
        case 2: self->spearPos.y += 0x80000; break;
        case 3: self->spearPos.x -= 0x80000; break;
        default: break;
    }

    self->timer--;
    if (!self->timer)
        self->state = SpearBlock_State_SpearExtended;
}

void SpearBlock_State_SpearExtended(void)
{
    RSDK_THIS(SpearBlock);
    if (!(Zone->timer & 0x3F)) {
        self->timer = 4;
        self->state = SpearBlock_State_RetractSpears;
        // Spear Sfx should be played here, it's prolly not here in the code since this is just a test object, nothing more.
    }
}

void SpearBlock_State_RetractSpears(void)
{
    RSDK_THIS(SpearBlock);
    switch (self->animator.frameID) {
        case 0: self->spearPos.y += 0x80000; break;
        case 1: self->spearPos.x -= 0x80000; break;
        case 2: self->spearPos.y -= 0x80000; break;
        case 3: self->spearPos.x += 0x80000; break;
        default: break;
    }

    self->timer--;
    if (!self->timer)
        self->state = SpearBlock_State_SpearRetracted;
}

#if GAME_INCLUDE_EDITOR
void SpearBlock_EditorDraw(void)
{
    RSDK_THIS(SpearBlock);
    self->spearPos.x = self->position.x;
    self->spearPos.y = self->position.y;

    RSDK.SetSpriteAnimation(SpearBlock->aniFrames, 1, &self->animator, true, self->spearDir & 3);
    RSDK.SetSpriteAnimation(SpearBlock->aniFrames, 0, &SpearBlock->animator, true, 0);

    switch (self->animator.frameID) {
        case 0: self->spearPos.y -= 0x200000; break;
        case 1: self->spearPos.x += 0x200000; break;
        case 2: self->spearPos.y += 0x200000; break;
        case 3: self->spearPos.x -= 0x200000; break;
        default: break;
    }

    RSDK.DrawSprite(&self->animator, &self->spearPos, false);
    RSDK.DrawSprite(&SpearBlock->animator, NULL, false);
}

void SpearBlock_EditorLoad(void)
{
    SpearBlock->aniFrames = RSDK.LoadSpriteAnimation("Blueprint/SpearBlock.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(SpearBlock, spearDir);
    RSDK_ENUM_VAR("Up", FLIP_NONE);
    RSDK_ENUM_VAR("Left", FLIP_X);
    RSDK_ENUM_VAR("Down", FLIP_Y);
    RSDK_ENUM_VAR("Right", FLIP_XY);
}
#endif

void SpearBlock_Serialize(void) { RSDK_EDITABLE_VAR(SpearBlock, VAR_UINT8, spearDir); }
