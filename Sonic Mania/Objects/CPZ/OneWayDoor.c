// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: OneWayDoor Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled By Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

ObjectOneWayDoor *OneWayDoor;

void OneWayDoor_Update(void)
{
    RSDK_THIS(OneWayDoor);
    StateMachine_Run(self->state);
    RSDK.ProcessAnimation(&self->animator);
}

void OneWayDoor_LateUpdate(void) {}

void OneWayDoor_StaticUpdate(void) {}

void OneWayDoor_Draw(void)
{
    RSDK_THIS(OneWayDoor);
    StateMachine_Run(self->stateDraw);
}

void OneWayDoor_Create(void *data)
{
    RSDK_THIS(OneWayDoor);
    self->active        = ACTIVE_BOUNDS;
    self->visible       = true;
    self->updateRange.x = 0x1000000;
    self->updateRange.y = 0x1000000;
    self->state         = OneWayDoor_State_MoveDown;
    self->drawOrder     = Zone->drawOrderHigh - 2;
    RSDK.SetSpriteAnimation(OneWayDoor->animID, 0, &self->animator, true, 0);
    if (RSDK.CheckStageFolder("MMZ")) {
#if RETRO_USE_PLUS
        RSDK.SetSpriteAnimation(OneWayDoor->animID, self->direction ? 4 : 2, &self->animator, true, 0);
#endif
        self->stateDraw = OneWayDoor_Draw_MMZ;
        self->groundVel = 0x60000;
    }
    else if (RSDK.CheckStageFolder("CPZ")) {
        self->stateDraw = OneWayDoor_Draw_CPZ;
        self->groundVel = 0x80000;
    }
}

void OneWayDoor_StageLoad(void)
{
    if (RSDK.CheckStageFolder("MMZ"))
        OneWayDoor->animID = RSDK.LoadSpriteAnimation("MMZ/OneWayDoor.bin", SCOPE_STAGE);
    else if (RSDK.CheckStageFolder("CPZ"))
        OneWayDoor->animID = RSDK.LoadSpriteAnimation("CPZ/OneWayDoor.bin", SCOPE_STAGE);
    OneWayDoor->hitboxTop.left   = -8;
    OneWayDoor->hitboxTop.top    = -32;
    OneWayDoor->hitboxTop.right  = 8;
    OneWayDoor->hitboxTop.bottom = 0;

    OneWayDoor->hitboxBottom.left   = -8;
    OneWayDoor->hitboxBottom.top    = 0;
    OneWayDoor->hitboxBottom.right  = 8;
    OneWayDoor->hitboxBottom.bottom = 32;

    OneWayDoor->hitboxRange.left   = -64;
    OneWayDoor->hitboxRange.top    = -32;
    OneWayDoor->hitboxRange.right  = 8;
    OneWayDoor->hitboxRange.bottom = 32;
}

void OneWayDoor_Interact(void)
{
    RSDK_THIS(OneWayDoor);
    self->state = OneWayDoor_State_MoveDown;
    bool32 isMMZ1 = false;
    if (RSDK.CheckStageFolder("MMZ") && Zone->actID == 1)
        isMMZ1 = true;
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    bool32 playerIsBehind = false;
    if (self->direction == FLIP_X)
        playerIsBehind = player1->position.x >= self->position.x;
    else if (!self->direction)
        playerIsBehind = player1->position.x <= self->position.x;

    foreach_active(Player, currentPlayer)
    {
        if (self->stateDraw != OneWayDoor_Draw_MMZ)
            self->position.y -= 2 * self->yChange;
        else
            self->position.y -= self->yChange;

        Player_CheckCollisionBox(currentPlayer, self, &OneWayDoor->hitboxTop);
        if (self->stateDraw == OneWayDoor_Draw_MMZ)
            self->position.y += 2 * self->yChange;

        Player_CheckCollisionBox(currentPlayer, self, &OneWayDoor->hitboxBottom);
        if (self->stateDraw == OneWayDoor_Draw_MMZ)
            self->position.y -= self->yChange;
        else
            self->position.y += 2 * self->yChange;

        if (currentPlayer->velocity.x < 0x60000)
            OneWayDoor->hitboxRange.left = -64;
        else
            OneWayDoor->hitboxRange.left = -88;

        if (Player_CheckCollisionTouch(currentPlayer, self, &OneWayDoor->hitboxRange)) {
#if RETRO_USE_PLUS
            if (isMMZ1 && currentPlayer->sidekick && !playerIsBehind) {
                Player->cantSwap = true;
                NoSwap->counter++;
            }
            else
#endif
                self->state = OneWayDoor_State_MoveUp;
            
        }
    }
}

void OneWayDoor_State_MoveDown(void)
{
    RSDK_THIS(OneWayDoor);
    if (self->yChange > 0)
        self->yChange -= self->groundVel;
    OneWayDoor_Interact();
}

void OneWayDoor_State_MoveUp(void)
{
    RSDK_THIS(OneWayDoor);
    if (self->yChange < 0x200000)
        self->yChange += self->groundVel;
    OneWayDoor_Interact();
}

void OneWayDoor_Draw_MMZ(void)
{
    RSDK_THIS(OneWayDoor);

    self->position.y -= self->yChange;
    RSDK.DrawSprite(&self->animator, NULL, false);

    self->position.y += 2 * self->yChange + 0x200000;
    RSDK.DrawSprite(&self->animator, NULL, false);

    self->position.y += -0x200000 - self->yChange;
}

void OneWayDoor_Draw_CPZ(void)
{
    RSDK_THIS(OneWayDoor);

    self->animator.frameID = 0;
    self->position.y -= 2 * self->yChange;
    RSDK.DrawSprite(&self->animator, NULL, false);

    self->animator.frameID = 1;
    RSDK.DrawSprite(&self->animator, NULL, false);

    self->position.y += 2 * self->yChange;
}

#if RETRO_INCLUDE_EDITOR
void OneWayDoor_EditorDraw(void)
{
    RSDK_THIS(OneWayDoor);

    if (RSDK.CheckStageFolder("MMZ")) {
        RSDK.SetSpriteAnimation(OneWayDoor->animID, self->direction ? 4 : 2, &self->animator, true, 0);
        OneWayDoor_Draw_MMZ();
    }
    else if (RSDK.CheckStageFolder("CPZ")) {
        RSDK.SetSpriteAnimation(OneWayDoor->animID, 0, &self->animator, true, 0);
        OneWayDoor_Draw_CPZ();
    }
}

void OneWayDoor_EditorLoad(void)
{
    if (RSDK.CheckStageFolder("MMZ"))
        OneWayDoor->animID = RSDK.LoadSpriteAnimation("MMZ/OneWayDoor.bin", SCOPE_STAGE);
    else if (RSDK.CheckStageFolder("CPZ"))
        OneWayDoor->animID = RSDK.LoadSpriteAnimation("CPZ/OneWayDoor.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(OneWayDoor, direction);
    RSDK_ENUM_VAR("No Flip", FLIP_NONE);
    RSDK_ENUM_VAR("Flip X", FLIP_X);
}
#endif

void OneWayDoor_Serialize(void) { RSDK_EDITABLE_VAR(OneWayDoor, VAR_UINT8, direction); }