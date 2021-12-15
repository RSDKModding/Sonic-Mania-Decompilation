// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: Staircase Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled By Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

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

    self->visible               = true;
    self->drawOrder             = Zone->drawOrderLow;
    self->active                = ACTIVE_BOUNDS;
    self->updateRange.x         = 0x1000000;
    self->updateRange.y         = 0x1000000;
    Staircase->blockHitbox.top    = -16;
    Staircase->blockHitbox.left   = -16;
    Staircase->blockHitbox.bottom = 16;
    Staircase->blockHitbox.right  = 16;

    int32 add = self->direction ? -0x200000 : 0x200000;

    for (int i = 0; i < Starcase_StairCount; ++i) {
        self->blocks[i].x = self->position.x + i * add;
        self->blocks[i].y = self->position.y;
    }

    RSDK.SetSpriteAnimation(Staircase->animID, 0, &self->animator, true, 0);

    self->state     = Staircase_State_Idle;
    self->stateDraw = Staircase_Draw_Blocks;
}

void Staircase_StageLoad(void)
{
    if (RSDK.CheckStageFolder("CPZ")) {
        Staircase->animID = RSDK.LoadSpriteAnimation("CPZ/Staircase.bin", SCOPE_STAGE);
        Staircase->sfxID  = RSDK.GetSfx("CPZ/CPZ2HitBlocksStop.wav");
        Soundboard_LoadSFX("CPZ/CPZ2HitBlocks.wav", true, Staircase_CheckCB, NULL);
    }
}

bool32 Staircase_CheckCB(void)
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
    Vector2 oldpos = self->position;
    foreach_active(Player, player)
    {
        for (int32 i = 0; i < Starcase_StairCount; i++) {
            self->position = self->blocks[i];
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
    self->position = oldpos;
}

void Staircase_State_Wait(void)
{
    RSDK_THIS(Staircase);
    Vector2 oldpos = self->position;
    foreach_active(Player, player)
    {
        for (int32 i = 0; i < Starcase_StairCount; i++) {
            self->position = self->blocks[i];
            Player_CheckCollisionBox(player, self, &Staircase->blockHitbox);
        }
    }
    self->position = oldpos;

    if (--self->timer < 0) {
        self->timer = 128;
        if (!RSDK.IsSfxPlaying(Staircase->sfxID))
            RSDK.PlaySfx(Staircase->sfxID, false, 255);
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
    Vector2 oldpos = self->position;
    foreach_active(Player, player)
    {
        for (int32 i = 0; i < Starcase_StairCount; i++) {
            self->position = self->blocks[i];
            if (Player_CheckCollisionBox(player, self, &Staircase->blockHitbox) == 1)
                player->position.y += add * (i + 1);
        }
    }
    self->position = oldpos;

    for (int i = 0; i < Starcase_StairCount; ++i) self->blocks[i].y += (i + 1) * add;

    if (--self->timer <= 0)
        self->state = Staircase_State_MovedBlocks;
    if (!RSDK.CheckOnScreen(self, &self->updateRange))
        Staircase_Create(NULL);
}

void Staircase_State_MovedBlocks(void)
{
    RSDK_THIS(Staircase);
    Vector2 oldpos = self->position;
    foreach_active(Player, player)
    {
        for (int32 i = 0; i < Starcase_StairCount; i++) {
            self->position = self->blocks[i];
            if (Player_CheckCollisionBox(player, self, &Staircase->blockHitbox) == C_BOTTOM && player->onGround)
                player->deathType = PLAYER_DEATH_DIE_USESFX;
        }
    }
    self->position = oldpos;
    if (!RSDK.CheckOnScreen(self, &self->updateRange))
        Staircase_Create(NULL);
}

void Staircase_Draw_Blocks(void)
{
    RSDK_THIS(Staircase);
    for (int32 i = 0; i < Starcase_StairCount; i++) {
        RSDK.DrawSprite(&self->animator, &self->blocks[i], false);
    }
}
void Staircase_Draw_Shake(void) {
    RSDK_THIS(Staircase);
    for (int32 i = 0; i < Starcase_StairCount; i++) {
        Vector2 drawPos = self->blocks[i];
        drawPos.y += ((self->timer - i * 4) << 14) & 0x10000;
        RSDK.DrawSprite(&self->animator, &drawPos, false);
    }
}

#if RETRO_INCLUDE_EDITOR
void Staircase_EditorDraw(void)
{
    RSDK_THIS(Staircase);

    int32 add = (self->direction) ? -0x200000 : 0x200000;

    for (int i = 0; i < Starcase_StairCount; ++i) {
        self->blocks[i].x = self->position.x + i * add;
        self->blocks[i].y = self->position.y;
    }

    Staircase_Draw_Blocks();
}

void Staircase_EditorLoad(void) { Staircase->animID = RSDK.LoadSpriteAnimation("CPZ/Staircase.bin", SCOPE_STAGE); }
#endif

void Staircase_Serialize(void)
{
    RSDK_EDITABLE_VAR(Staircase, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(Staircase, VAR_UINT8, type);
    // fun fact: why is this never used
    RSDK_EDITABLE_VAR(Staircase, VAR_BOOL, bumpable);
}