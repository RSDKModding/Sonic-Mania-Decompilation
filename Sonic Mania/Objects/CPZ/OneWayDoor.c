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
    StateMachine_Run(self->drawState);
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
        self->drawState = OneWayDoor_MMZDraw;
        self->groundVel = 0x60000;
    }
    else if (RSDK.CheckStageFolder("CPZ")) {
        self->drawState = OneWayDoor_CPZDraw;
        self->groundVel = 0x80000;
    }
}

void OneWayDoor_StageLoad(void)
{
    if (RSDK.CheckStageFolder("MMZ"))
        OneWayDoor->animID = RSDK.LoadSpriteAnimation("MMZ/OneWayDoor.bin", SCOPE_STAGE);
    else if (RSDK.CheckStageFolder("CPZ"))
        OneWayDoor->animID = RSDK.LoadSpriteAnimation("CPZ/OneWayDoor.bin", SCOPE_STAGE);
    OneWayDoor->hitbox1.left   = -8;
    OneWayDoor->hitbox1.top    = -32;
    OneWayDoor->hitbox1.right  = 8;
    OneWayDoor->hitbox1.bottom = 0;

    OneWayDoor->hitbox2.left   = -8;
    OneWayDoor->hitbox2.top    = 0;
    OneWayDoor->hitbox2.right  = 8;
    OneWayDoor->hitbox2.bottom = 32;

    OneWayDoor->touchBox.left   = -64;
    OneWayDoor->touchBox.top    = -32;
    OneWayDoor->touchBox.right  = 8;
    OneWayDoor->touchBox.bottom = 32;
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
        int32 yChange = self->yChange;
        if (self->drawState != OneWayDoor_MMZDraw)
            yChange *= 2;

        self->position.y -= yChange;
        Player_CheckCollisionBox(currentPlayer, self, &OneWayDoor->hitbox1);
        if (self->drawState == OneWayDoor_MMZDraw)
            self->position.y += 2 * self->yChange;

        Player_CheckCollisionBox(currentPlayer, self, &OneWayDoor->hitbox2);
        if (self->drawState == OneWayDoor_MMZDraw)
            self->position.y -= self->yChange;
        else
            self->position.y += 2 * self->yChange;

        if (currentPlayer->velocity.x < 0x60000)
            OneWayDoor->touchBox.left = -64;
        else
            OneWayDoor->touchBox.left = -88;

        if (Player_CheckCollisionTouch(currentPlayer, self, &OneWayDoor->touchBox)) {
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

void OneWayDoor_MMZDraw(void)
{
    RSDK_THIS(OneWayDoor);

    self->position.y -= self->yChange;
    RSDK.DrawSprite(&self->animator, NULL, false);

    self->position.y += 2 * self->yChange + 0x200000;
    RSDK.DrawSprite(&self->animator, NULL, false);

    self->position.y += -0x200000 - self->yChange;
}

void OneWayDoor_CPZDraw(void)
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
        OneWayDoor_MMZDraw();
    }
    else if (RSDK.CheckStageFolder("CPZ")) {
        RSDK.SetSpriteAnimation(OneWayDoor->animID, 0, &self->animator, true, 0);
        OneWayDoor_CPZDraw();
    }
}

void OneWayDoor_EditorLoad(void)
{
    if (RSDK.CheckStageFolder("MMZ"))
        OneWayDoor->animID = RSDK.LoadSpriteAnimation("MMZ/OneWayDoor.bin", SCOPE_STAGE);
    else if (RSDK.CheckStageFolder("CPZ"))
        OneWayDoor->animID = RSDK.LoadSpriteAnimation("CPZ/OneWayDoor.bin", SCOPE_STAGE);
}
#endif

void OneWayDoor_Serialize(void) { RSDK_EDITABLE_VAR(OneWayDoor, VAR_UINT8, direction); }