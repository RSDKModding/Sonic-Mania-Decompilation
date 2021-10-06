#include "SonicMania.h"

ObjectCrane *Crane;

void Crane_Update(void)
{
    RSDK_THIS(Crane);
    StateMachine_Run(entity->state);
    RSDK.ProcessAnimation(&entity->animator1);
    RSDK.ProcessAnimation(&entity->animator2);
}

void Crane_LateUpdate(void) {}

void Crane_StaticUpdate(void)
{
    foreach_active(Crane, crane) { RSDK.AddDrawListRef(Zone->playerDrawLow + 1, RSDK.GetEntityID(crane)); }
}

void Crane_Draw(void)
{
    RSDK_THIS(Crane);
    if (RSDK_sceneInfo->currentDrawGroup == entity->drawOrder) {
        SpriteFrame *frame = RSDK.GetFrame(Crane->aniFrames, 4, 0);
        int height         = (entity->position.y - entity->startPos.y) >> 16;
        frame->sprY        = 230 - height;
        frame->height      = height;
        frame->pivotY      = -16 - height;
        RSDK.DrawSprite(&entity->animator3, NULL, false);
        RSDK.DrawSprite(&entity->animator2, NULL, false);
    }
    else {
        RSDK.DrawSprite(&entity->animator1, NULL, false);
    }
}

void Crane_Create(void *data)
{
    RSDK_THIS(Crane);
    entity->visible       = true;
    entity->drawOrder     = Zone->drawOrderLow;
    entity->startPos      = entity->position;
    entity->startDir      = entity->direction;
    entity->active        = ACTIVE_BOUNDS;
    entity->updateRange.x = 0x800000;
    entity->updateRange.y = 0x800000;
    entity->drawFX        = FX_FLIP;
    RSDK.SetSpriteAnimation(Crane->aniFrames, 2, &entity->animator1, true, 3);
    RSDK.SetSpriteAnimation(Crane->aniFrames, 3, &entity->animator2, true, 3);
    RSDK.SetSpriteAnimation(Crane->aniFrames, 4, &entity->animator3, true, 0);
    entity->state = Crane_State_Unknown1;
}

void Crane_StageLoad(void)
{
    Crane->aniFrames      = RSDK.LoadSpriteAnimation("FBZ/Crane.bin", SCOPE_STAGE);
    Crane->hitbox1.left   = -8;
    Crane->hitbox1.top    = -8;
    Crane->hitbox1.right  = 8;
    Crane->hitbox1.bottom = 8;
    Crane->hitbox3.left   = -8;
    Crane->hitbox3.top    = 0;
    Crane->hitbox3.right  = 8;
    Crane->hitbox3.bottom = 128;
    Crane->hitbox2.left   = -8;
    Crane->hitbox2.top    = 0;
    Crane->hitbox2.right  = 8;
    Crane->hitbox2.bottom = 16;
    Crane->active         = ACTIVE_ALWAYS;
    Crane->sfxGrab        = RSDK.GetSFX("Global/Grab.wav");
    DEBUGMODE_ADD_OBJ(Crane);
}

void Crane_DebugSpawn(void)
{
    RSDK_THIS(Crane);
    EntityCrane *crane = CREATE_ENTITY(Crane, NULL, entity->position.x, entity->position.y);
    crane->direction   = entity->direction;
    crane->startDir    = entity->direction;
}

void Crane_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(Crane->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, 0, false);
}

void Crane_HandlePlayerInteractions(void)
{
    RSDK_THIS(Crane);

    foreach_active(Player, player)
    {
        if (!entity->grabbedPlayer) {
            if ((entity->playerType || !player->sidekick) && (entity->playerType != 1 || player->sidekick)) {
                if (Player_CheckCollisionTouch(player, entity, &Crane->hitbox2)) {
                    RSDK.PlaySfx(Crane->sfxGrab, false, 255);
                    entity->state       = Crane_State_Unknown5;
                    entity->isPermanent = true;
                    RSDK.SetSpriteAnimation(Crane->aniFrames, 0, &entity->animator1, true, 0);
                    RSDK.SetSpriteAnimation(Crane->aniFrames, 1, &entity->animator2, true, 0);
                    entity->grabbedPlayer   = (Entity *)player;
                    player->velocity.x      = 0;
                    player->velocity.y      = 0;
                    player->groundVel       = 0;
                    player->state           = Player_State_None;
                    player->nextGroundState = 0;
                    player->nextAirState    = 0;
                    player->onGround        = false;
                    player->direction       = entity->direction;
                    RSDK.SetSpriteAnimation(player->spriteIndex, ANI_FAN, &player->playerAnimator, true, 0);
                    player->playerAnimator.animationSpeed = 0;
                }
            }
        }
    }
}

void Crane_CheckOnScreen(void)
{
    RSDK_THIS(Crane);
    if (!RSDK.CheckOnScreen(RSDK_sceneInfo->entity, &entity->updateRange)) {
        int x              = entity->position.x;
        int y              = entity->position.y;
        entity->position.x = entity->startPos.x;
        entity->position.y = entity->startPos.y;
        if (RSDK.CheckOnScreen(entity, &entity->updateRange)) {
            entity->position.x = x;
            entity->position.y = y;
        }
        else {
            entity->grabbedPlayer = NULL;
            entity->timer2        = 0;
            entity->timer         = 0;
            entity->direction     = entity->startDir;
            entity->visible       = false;
            entity->state         = Crane_State_CheckOnScreen;
        }
    }
}

void Crane_State_CheckOnScreen(void)
{
    RSDK_THIS(Crane);
    if (!RSDK.CheckOnScreen(RSDK_sceneInfo->entity, &RSDK_sceneInfo->entity->updateRange)) {
        entity->state = Crane_State_Unknown1;
        Crane_Create(NULL);
    }
}

void Crane_State_Unknown1(void)
{
    RSDK_THIS(Crane);

    entity->active = ACTIVE_NORMAL;
    if (!entity->startType) {
        entity->state = Crane_State_Unknown2;
        Crane_State_Unknown2();
    }
    else {
        entity->position.y += 0x400000;
        entity->timer = -1;
        entity->state = Crane_State_Unknown4;
        Crane_HandlePlayerInteractions();
        Crane_CheckOnScreen();
    }
}

void Crane_State_Unknown2(void)
{
    RSDK_THIS(Crane);
    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, entity, &Crane->hitbox3)) {
            entity->timer = 32;
            entity->state = Crane_State_Unknown3;
        }
    }
    Crane_CheckOnScreen();
}

void Crane_State_Unknown3(void)
{
    RSDK_THIS(Crane);
    Crane_HandlePlayerInteractions();
    Crane_CheckOnScreen();
    --entity->timer;
    if (entity->state == Crane_State_Unknown3) {
        if (entity->timer >= 0)
            entity->position.y += 0x20000;
        else
            entity->state = Crane_State_Unknown4;
    }
}

void Crane_State_Unknown4(void)
{
    Crane_HandlePlayerInteractions();
    Crane_CheckOnScreen();
}

void Crane_State_Unknown5(void)
{
    RSDK_THIS(Crane);
    if (++entity->timer < 32) {
        entity->position.y -= 0x20000;
        EntityPlayer *player = (EntityPlayer *)entity->grabbedPlayer;
        if (player) {
            if (Player_CheckValidState(player)) {
                player->playerAnimator.animationSpeed = 0;
                player->position.x                    = entity->position.x;
                player->position.y                    = entity->position.y;
                if (entity->direction) {
                    player->position.x += 0x20000;
                    player->position.y += 0xE0000;
                }
                else {
                    player->position.x -= 0x20000;
                    player->position.y += 0xE0000;
                }
            }
            else {
                entity->grabbedPlayer = NULL;
            }
        }
    }
    else {
        entity->timer2     = 1;
        entity->timer      = 16;
        entity->velocity.x = 0;
        entity->state      = Crane_State_Unknown6;
    }
    Crane_CheckOnScreen();
}

void Crane_State_Unknown6(void)
{
    RSDK_THIS(Crane);

    entity->velocity.x += 0x1000;
    if (entity->direction == FLIP_NONE) {
        entity->position.x += entity->velocity.x;
        if (entity->position.x >= entity->startPos.x + (entity->distance << 15))
            entity->state = Crane_State_Unknown7;
    }
    else {
        entity->position.x -= entity->velocity.x;
        if (entity->position.x <= entity->startPos.x - (entity->distance << 15))
            entity->state = Crane_State_Unknown7;
    }

    EntityPlayer *player = (EntityPlayer *)entity->grabbedPlayer;
    if (player) {
        if (Player_CheckValidState(player)) {
            player->playerAnimator.animationSpeed = 0;
            player->position.x                    = entity->position.x;
            player->position.y                    = entity->position.y;
            if (entity->direction) {
                player->position.x += 0x20000;
                player->position.y += 0xE0000;
            }
            else {
                player->position.x -= 0x20000;
                player->position.y += 0xE0000;
            }
        }
        else {
            entity->grabbedPlayer = NULL;
        }
    }
    Crane_CheckOnScreen();
}

void Crane_State_Unknown7(void)
{
    RSDK_THIS(Crane);

    EntityPlayer *player = (EntityPlayer *)entity->grabbedPlayer;
    entity->velocity.x -= 0x1000;
    if (entity->direction == FLIP_NONE)
        entity->position.x += entity->velocity.x;
    else
        entity->position.x -= entity->velocity.x;
    if (entity->velocity.x <= 0) {
        if (player) {
            if (Player_CheckValidState(player))
                player->state = Player_State_Air;
            else
                entity->grabbedPlayer = 0;
        }
        RSDK.SetSpriteAnimation(Crane->aniFrames, 2, &entity->animator1, true, 0);
        RSDK.SetSpriteAnimation(Crane->aniFrames, 3, &entity->animator2, true, 0);
        entity->timer2      = 30;
        entity->state       = Crane_State_Unknown8;
        entity->isPermanent = false;
    }

    if (player) {
        if (Player_CheckValidState(player)) {
            player->playerAnimator.animationSpeed = 0;
            player->position.x                    = entity->position.x;
            player->position.y                    = entity->position.y;
            if (entity->direction) {
                player->position.x += 0x20000;
                player->position.y += 0xE0000;
            }
            else {
                player->position.x -= 0x20000;
                player->position.y += 0xE0000;
            }
        }
        else {
            entity->grabbedPlayer = NULL;
        }
    }
    Crane_CheckOnScreen();
}

void Crane_State_Unknown8(void)
{
    RSDK_THIS(Crane);
    if (--entity->timer2 <= 0)
        entity->state = Crane_State_Unknown9;
    Crane_HandlePlayerInteractions();
    Crane_CheckOnScreen();
}

void Crane_State_Unknown9(void)
{
    RSDK_THIS(Crane);

    entity->velocity.x -= 0x1000;
    if (entity->direction) {
        entity->position.x -= entity->velocity.x;
        if (entity->position.x >= entity->startPos.x - (entity->distance << 15))
            entity->state = Crane_State_Unknown10;
    }
    else {
        entity->position.x += entity->velocity.x;
        if (entity->position.x <= entity->startPos.x + (entity->distance << 15))
            entity->state = Crane_State_Unknown10;
    }
    Crane_CheckOnScreen();
}

void Crane_State_Unknown10(void)
{
    RSDK_THIS(Crane);
    entity->velocity.x += 0x1000;

    if (entity->direction)
        entity->position.x -= entity->velocity.x;
    else
        entity->position.x += entity->velocity.x;
    if (entity->velocity.x >= 0) {
        if (entity->startType) {
            entity->timer         = 32;
            entity->state         = Crane_State_Unknown3;
            entity->grabbedPlayer = NULL;
        }
        else {
            entity->state         = Crane_State_Unknown2;
            entity->grabbedPlayer = NULL;
        }
    }
    Crane_CheckOnScreen();
}

void Crane_EditorDraw(void) {}

void Crane_EditorLoad(void) {}

void Crane_Serialize(void)
{
    RSDK_EDITABLE_VAR(Crane, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(Crane, VAR_UINT16, distance);
    RSDK_EDITABLE_VAR(Crane, VAR_UINT8, startType);
    RSDK_EDITABLE_VAR(Crane, VAR_UINT8, playerType);
}
