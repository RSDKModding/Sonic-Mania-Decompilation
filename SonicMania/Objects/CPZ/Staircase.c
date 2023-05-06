// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: Staircase Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectStaircase *Staircase;

void Staircase_Update(void)
{
    RSDK_THIS(Staircase);

    StateMachine_Run(self->state);
}

void Staircase_LateUpdate(void) {}

void Staircase_StaticUpdate(void) {}

void Staircase_Draw(void)
{
    RSDK_THIS(Staircase);

    StateMachine_Run(self->stateDraw);
}

void Staircase_Create(void *data)
{
    RSDK_THIS(Staircase);

    self->visible       = true;
    self->drawGroup     = Zone->objectDrawGroup[0];
    self->active        = ACTIVE_BOUNDS;
    self->updateRange.x = 0x1000000;
    self->updateRange.y = 0x1000000;

    // Why is a static value being initialized in Create?
    Staircase->blockHitbox.top    = -16;
    Staircase->blockHitbox.left   = -16;
    Staircase->blockHitbox.bottom = 16;
    Staircase->blockHitbox.right  = 16;

    int32 add = self->direction ? -0x200000 : 0x200000;

    for (int32 i = 0; i < STAIRCASE_STAIR_COUNT; ++i) {
        self->blockPos[i].x = self->position.x + i * add;
        self->blockPos[i].y = self->position.y;
    }

    RSDK.SetSpriteAnimation(Staircase->aniFrames, 0, &self->animator, true, 0);

    self->state     = Staircase_State_Idle;
    self->stateDraw = Staircase_Draw_Blocks;
}

void Staircase_StageLoad(void)
{
    if (RSDK.CheckSceneFolder("CPZ")) {
        Staircase->aniFrames = RSDK.LoadSpriteAnimation("CPZ/Staircase.bin", SCOPE_STAGE);

        // What a name tbh
        Staircase->sfxBlockStop = RSDK.GetSfx("CPZ/CPZ2HitBlocksStop.wav");
        Soundboard_LoadSfx("CPZ/CPZ2HitBlocks.wav", true, Staircase_SfxCheck_HitBlocks, StateMachine_None);
    }
}

bool32 Staircase_SfxCheck_HitBlocks(void)
{
    foreach_active(Staircase, entity)
    {
        if (entity->stateDraw == Staircase_Draw_Shake)
            return true;
    }

    return false;
}

void Staircase_State_Idle(void)
{
    RSDK_THIS(Staircase);

    Vector2 posStore = self->position;
    foreach_active(Player, player)
    {
        for (int32 i = 0; i < STAIRCASE_STAIR_COUNT; i++) {
            self->position = self->blockPos[i];
            int32 side     = Player_CheckCollisionBox(player, self, &Staircase->blockHitbox);
            if (side == C_BOTTOM) {
                // bumpable would prolly be used here :D
                self->active    = ACTIVE_NORMAL;
                self->timer     = 59;
                self->state     = Staircase_State_Wait;
                self->stateDraw = Staircase_Draw_Shake;
                if (player->onGround)
                    player->deathType = PLAYER_DEATH_DIE_USESFX;
            }
            else if (side == C_TOP) {
                self->active = ACTIVE_NORMAL;
                self->timer  = 32;
                self->state  = Staircase_State_Wait;
            }
        }
    }

    self->position = posStore;
}

void Staircase_State_Wait(void)
{
    RSDK_THIS(Staircase);

    Vector2 posStore = self->position;
    foreach_active(Player, player)
    {
        for (int32 i = 0; i < STAIRCASE_STAIR_COUNT; i++) {
            self->position = self->blockPos[i];
            Player_CheckCollisionBox(player, self, &Staircase->blockHitbox);
        }
    }
    self->position = posStore;

    if (--self->timer < 0) {
        self->timer = 128;
        if (!RSDK.IsSfxPlaying(Staircase->sfxBlockStop))
            RSDK.PlaySfx(Staircase->sfxBlockStop, false, 255);
        self->state     = Staircase_State_MoveBlocks;
        self->stateDraw = Staircase_Draw_Blocks;
    }

    if (!RSDK.CheckOnScreen(self, &self->updateRange))
        Staircase_Create(NULL);
}

void Staircase_State_MoveBlocks(void)
{
    RSDK_THIS(Staircase);

    int32 add        = self->type ? -0x4000 : 0x4000;
    Vector2 posStore = self->position;
    foreach_active(Player, player)
    {
        for (int32 i = 0; i < STAIRCASE_STAIR_COUNT; i++) {
            self->position = self->blockPos[i];
            if (Player_CheckCollisionBox(player, self, &Staircase->blockHitbox) == C_TOP)
                player->position.y += add * (i + 1);
        }
    }
    self->position = posStore;

    for (int32 i = 0; i < STAIRCASE_STAIR_COUNT; ++i) self->blockPos[i].y += (i + 1) * add;

    if (--self->timer <= 0)
        self->state = Staircase_State_MovedBlocks;

    if (!RSDK.CheckOnScreen(self, &self->updateRange))
        Staircase_Create(NULL);
}

void Staircase_State_MovedBlocks(void)
{
    RSDK_THIS(Staircase);

    Vector2 posStore = self->position;
    foreach_active(Player, player)
    {
        for (int32 i = 0; i < STAIRCASE_STAIR_COUNT; i++) {
            self->position = self->blockPos[i];
            if (Player_CheckCollisionBox(player, self, &Staircase->blockHitbox) == C_BOTTOM && player->onGround)
                player->deathType = PLAYER_DEATH_DIE_USESFX;
        }
    }
    self->position = posStore;

    if (!RSDK.CheckOnScreen(self, &self->updateRange))
        Staircase_Create(NULL);
}

void Staircase_Draw_Blocks(void)
{
    RSDK_THIS(Staircase);

    for (int32 i = 0; i < STAIRCASE_STAIR_COUNT; i++) {
        RSDK.DrawSprite(&self->animator, &self->blockPos[i], false);
    }
}

void Staircase_Draw_Shake(void)
{
    RSDK_THIS(Staircase);

    for (int32 i = 0; i < STAIRCASE_STAIR_COUNT; i++) {
        Vector2 drawPos = self->blockPos[i];
        drawPos.y += ((self->timer - i * STAIRCASE_STAIR_COUNT) << 14) & 0x10000;
        RSDK.DrawSprite(&self->animator, &drawPos, false);
    }
}

#if GAME_INCLUDE_EDITOR
void Staircase_EditorDraw(void)
{
    RSDK_THIS(Staircase);

    int32 addX = self->direction ? -0x200000 : 0x200000;

    for (int32 i = 0; i < STAIRCASE_STAIR_COUNT; ++i) {
        self->blockPos[i].x = self->position.x + i * addX;
        self->blockPos[i].y = self->position.y;
    }

    Staircase_Draw_Blocks();

    if (showGizmos()) {
        RSDK_DRAWING_OVERLAY(true);

        self->inkEffect = INK_BLEND;

        int32 addY = self->type ? -0x200000 : 0x200000;
        for (int32 i = 0; i < STAIRCASE_STAIR_COUNT; ++i) self->blockPos[i].y += (i + 1) * addY;

        Staircase_Draw_Blocks();

        self->inkEffect = INK_NONE;

        RSDK_DRAWING_OVERLAY(false);
    }
}

void Staircase_EditorLoad(void)
{
    Staircase->aniFrames = RSDK.LoadSpriteAnimation("CPZ/Staircase.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(Staircase, direction);
    RSDK_ENUM_VAR("Right", FLIP_NONE);
    RSDK_ENUM_VAR("Left", FLIP_X);

    RSDK_ACTIVE_VAR(Staircase, type);
    RSDK_ENUM_VAR("Down", STAIRCASE_DOWN);
    RSDK_ENUM_VAR("Up", STAIRCASE_UP);
}
#endif

void Staircase_Serialize(void)
{
    RSDK_EDITABLE_VAR(Staircase, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(Staircase, VAR_UINT8, type);
    // fun fact: why is this never used
    RSDK_EDITABLE_VAR(Staircase, VAR_BOOL, bumpable);
}