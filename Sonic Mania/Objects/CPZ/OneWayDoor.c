#include "SonicMania.h"

ObjectOneWayDoor *OneWayDoor;

void OneWayDoor_Update(void)
{
    RSDK_THIS(OneWayDoor);
    StateMachine_Run(entity->state);
    RSDK.ProcessAnimation(&entity->animator);
}

void OneWayDoor_LateUpdate(void) {}

void OneWayDoor_StaticUpdate(void) {}

void OneWayDoor_Draw(void)
{
    RSDK_THIS(OneWayDoor);
    StateMachine_Run(entity->drawState);
}

void OneWayDoor_Create(void *data)
{
    RSDK_THIS(OneWayDoor);
    entity->active        = ACTIVE_BOUNDS;
    entity->visible       = true;
    entity->updateRange.x = 0x1000000;
    entity->updateRange.y = 0x1000000;
    entity->state         = OneWayDoor_State_MoveDown;
    entity->drawOrder     = Zone->drawOrderHigh - 2;
    if (RSDK.CheckStageFolder("MMZ")) {
        RSDK.SetSpriteAnimation(OneWayDoor->animID, entity->direction ? 4 : 2, &entity->animator, true, 0);
        entity->drawState = OneWayDoor_MMZDraw;
        entity->groundVel = 0x60000;
    }
    else if (RSDK.CheckStageFolder("CPZ")) {
        RSDK.SetSpriteAnimation(OneWayDoor->animID, 0, &entity->animator, true, 0);
        entity->drawState = OneWayDoor_CPZDraw;
        entity->groundVel = 0x80000;
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
    entity->state = OneWayDoor_State_MoveDown;
    bool32 isMMZ1 = true;
    if (!RSDK.CheckStageFolder("MMZ") || (Zone->actID != 1))
        isMMZ1 = false;
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    bool32 playerIsBehind = false;
    if (entity->direction == FLIP_X)
        playerIsBehind = player1->position.x >= entity->position.x;
    else if (!entity->direction)
        playerIsBehind = player1->position.x <= entity->position.x;

    foreach_active(Player, currentPlayer)
    {
        int32 yChange = entity->yChange;
        if (entity->drawState != OneWayDoor_MMZDraw)
            yChange *= 2;

        entity->position.y -= yChange;
        Player_CheckCollisionBox(currentPlayer, entity, &OneWayDoor->hitbox1);
        if (entity->drawState == OneWayDoor_MMZDraw)
            entity->position.y += 2 * entity->yChange;

        Player_CheckCollisionBox(currentPlayer, entity, &OneWayDoor->hitbox2);
        if (entity->drawState == OneWayDoor_MMZDraw)
            entity->position.y -= entity->yChange;
        else
            entity->position.y += 2 * entity->yChange;

        if (currentPlayer->velocity.x < 0x60000)
            OneWayDoor->touchBox.left = -64;
        else
            OneWayDoor->touchBox.left = -88;

        if (Player_CheckCollisionTouch(currentPlayer, entity, &OneWayDoor->touchBox)) {
            if (isMMZ1 && currentPlayer->sidekick && !playerIsBehind) {
#if RETRO_USE_PLUS
                Player->cantSwap = true;
                NoSwap->counter++;
#endif
            }
            else {
                entity->state = OneWayDoor_State_MoveUp;
            }
        }
    }
}

void OneWayDoor_State_MoveDown(void)
{
    RSDK_THIS(OneWayDoor);
    if (entity->yChange > 0)
        entity->yChange -= entity->groundVel;
    OneWayDoor_Interact();
}

void OneWayDoor_State_MoveUp(void)
{
    RSDK_THIS(OneWayDoor);
    if (entity->yChange < 0x200000)
        entity->yChange += entity->groundVel;
    OneWayDoor_Interact();
}

void OneWayDoor_MMZDraw(void)
{
    RSDK_THIS(OneWayDoor);
    entity->position.y -= entity->yChange;
    RSDK.DrawSprite(&entity->animator, 0, false);
    entity->position.y += 2 * entity->yChange + 0x200000;
    RSDK.DrawSprite(&entity->animator, 0, false);
    entity->position.y -= 0x200000 + entity->yChange;
}

void OneWayDoor_CPZDraw(void)
{
    RSDK_THIS(OneWayDoor);
    entity->animator.frameID = 0;
    entity->position.y -= 2 * entity->yChange;
    RSDK.DrawSprite(&entity->animator, NULL, false);
    entity->animator.frameID = 1;
    RSDK.DrawSprite(&entity->animator, NULL, false);
    entity->position.y += 2 * entity->yChange;
}

#if RETRO_INCLUDE_EDITOR
void OneWayDoor_EditorDraw(void)
{
    RSDK_THIS(OneWayDoor);

    if (RSDK.CheckStageFolder("MMZ")) {
        RSDK.SetSpriteAnimation(OneWayDoor->animID, entity->direction ? 4 : 2, &entity->animator, true, 0);
        OneWayDoor_MMZDraw();
    }
    else if (RSDK.CheckStageFolder("CPZ")) {
        RSDK.SetSpriteAnimation(OneWayDoor->animID, 0, &entity->animator, true, 0);
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