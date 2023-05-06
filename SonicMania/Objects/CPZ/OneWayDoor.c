// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: OneWayDoor Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

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
    self->drawGroup     = Zone->objectDrawGroup[1] - 2;

    RSDK.SetSpriteAnimation(OneWayDoor->aniFrames, 0, &self->animator, true, 0);

    if (RSDK.CheckSceneFolder("MMZ")) {
#if MANIA_USE_PLUS
        RSDK.SetSpriteAnimation(OneWayDoor->aniFrames, self->direction ? 4 : 2, &self->animator, true, 0);
#endif
        self->stateDraw = OneWayDoor_Draw_MMZ;
        self->groundVel = 0x60000;
    }
    else if (RSDK.CheckSceneFolder("CPZ")) {
        self->stateDraw = OneWayDoor_Draw_CPZ;
        self->groundVel = 0x80000;
    }
}

void OneWayDoor_StageLoad(void)
{
    if (RSDK.CheckSceneFolder("MMZ"))
        OneWayDoor->aniFrames = RSDK.LoadSpriteAnimation("MMZ/OneWayDoor.bin", SCOPE_STAGE);
    else if (RSDK.CheckSceneFolder("CPZ"))
        OneWayDoor->aniFrames = RSDK.LoadSpriteAnimation("CPZ/OneWayDoor.bin", SCOPE_STAGE);

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

void OneWayDoor_HandlePlayerInteractions(void)
{
    RSDK_THIS(OneWayDoor);

    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    self->state = OneWayDoor_State_MoveDown;

    bool32 isMMZ1 = false;
    if (RSDK.CheckSceneFolder("MMZ") && Zone->actID == 1)
        isMMZ1 = true;

    bool32 isBehind = false;
    if (self->direction == FLIP_X)
        isBehind = player1->position.x >= self->position.x;
    else if (!self->direction)
        isBehind = player1->position.x <= self->position.x;

    foreach_active(Player, currentPlayer)
    {
        if (self->stateDraw != OneWayDoor_Draw_MMZ)
            self->position.y -= 2 * self->offsetY;
        else
            self->position.y -= self->offsetY;

        Player_CheckCollisionBox(currentPlayer, self, &OneWayDoor->hitboxTop);
        if (self->stateDraw == OneWayDoor_Draw_MMZ)
            self->position.y += 2 * self->offsetY;

        Player_CheckCollisionBox(currentPlayer, self, &OneWayDoor->hitboxBottom);
        if (self->stateDraw == OneWayDoor_Draw_MMZ)
            self->position.y -= self->offsetY;
        else
            self->position.y += 2 * self->offsetY;

        if (currentPlayer->velocity.x < 0x60000)
            OneWayDoor->hitboxRange.left = -64;
        else
            OneWayDoor->hitboxRange.left = -88;

        if (Player_CheckCollisionTouch(currentPlayer, self, &OneWayDoor->hitboxRange)) {
#if MANIA_USE_PLUS
            if (isMMZ1 && currentPlayer->sidekick && !isBehind) {
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

    if (self->offsetY > 0)
        self->offsetY -= self->groundVel;

    OneWayDoor_HandlePlayerInteractions();
}

void OneWayDoor_State_MoveUp(void)
{
    RSDK_THIS(OneWayDoor);

    if (self->offsetY < 0x200000)
        self->offsetY += self->groundVel;

    OneWayDoor_HandlePlayerInteractions();
}

void OneWayDoor_Draw_MMZ(void)
{
    RSDK_THIS(OneWayDoor);

    self->position.y -= self->offsetY;
    RSDK.DrawSprite(&self->animator, NULL, false);

    self->position.y += 2 * self->offsetY + 0x200000;
    RSDK.DrawSprite(&self->animator, NULL, false);

    self->position.y += -0x200000 - self->offsetY;
}

void OneWayDoor_Draw_CPZ(void)
{
    RSDK_THIS(OneWayDoor);

    self->animator.frameID = 0;
    self->position.y -= 2 * self->offsetY;
    RSDK.DrawSprite(&self->animator, NULL, false);

    self->animator.frameID = 1;
    RSDK.DrawSprite(&self->animator, NULL, false);

    self->position.y += 2 * self->offsetY;
}

#if GAME_INCLUDE_EDITOR
void OneWayDoor_EditorDraw(void)
{
    RSDK_THIS(OneWayDoor);

    if (RSDK.CheckSceneFolder("MMZ")) {
        RSDK.SetSpriteAnimation(OneWayDoor->aniFrames, self->direction ? 4 : 2, &self->animator, true, 0);
        OneWayDoor_Draw_MMZ();
    }
    else if (RSDK.CheckSceneFolder("CPZ")) {
        RSDK.SetSpriteAnimation(OneWayDoor->aniFrames, 0, &self->animator, true, 0);
        OneWayDoor_Draw_CPZ();
    }
}

void OneWayDoor_EditorLoad(void)
{
    if (RSDK.CheckSceneFolder("MMZ"))
        OneWayDoor->aniFrames = RSDK.LoadSpriteAnimation("MMZ/OneWayDoor.bin", SCOPE_STAGE);
    else if (RSDK.CheckSceneFolder("CPZ"))
        OneWayDoor->aniFrames = RSDK.LoadSpriteAnimation("CPZ/OneWayDoor.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(OneWayDoor, direction);
    RSDK_ENUM_VAR("No Flip", FLIP_NONE);
    RSDK_ENUM_VAR("Flipped", FLIP_X);
}
#endif

void OneWayDoor_Serialize(void) { RSDK_EDITABLE_VAR(OneWayDoor, VAR_UINT8, direction); }