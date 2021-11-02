#include "SonicMania.h"

ObjectGrabber *Grabber;

void Grabber_Update(void)
{
    RSDK_THIS(Grabber);
    StateMachine_Run(entity->state);
}

void Grabber_LateUpdate(void) {}

void Grabber_StaticUpdate(void) {}

void Grabber_Draw(void)
{
    RSDK_THIS(Grabber);
    if (RSDK_sceneInfo->currentDrawGroup == entity->drawOrder) {
        RSDK.DrawLine(entity->position.x, entity->startPos.y - 0x100000, entity->position.x, entity->position.y, 0x202020, 0, INK_NONE, false);
        RSDK.DrawLine(entity->position.x - 0x10000, entity->startPos.y - 0x100000, entity->position.x - 0x10000, entity->position.y, 0xE0E0E0, 0,
                      INK_NONE, false);

        Vector2 drawPos;
        int32 dir           = entity->direction;
        drawPos.x         = entity->position.x;
        drawPos.y         = entity->startPos.y;
        entity->direction = 0;
        RSDK.DrawSprite(&entity->animator3, &drawPos, false);
        entity->direction = dir;
        RSDK.DrawSprite(&entity->animator1, NULL, false);
        if (!RSDK_sceneInfo->currentScreenID)
            RSDK.AddDrawListRef(Zone->drawOrderHigh, RSDK_sceneInfo->entitySlot);
    }
    else {
        RSDK.DrawSprite(&entity->animator2, NULL, false);
    }
}

void Grabber_Create(void *data)
{
    RSDK_THIS(Grabber);
    entity->visible   = true;
    entity->drawOrder = Zone->drawOrderLow;
    entity->startPos  = entity->position;
    entity->startDir  = entity->direction;
    entity->active    = ACTIVE_BOUNDS;
    if (!entity->direction)
        entity->velocity.x = -0x4000;
    else
        entity->velocity.x = 0x4000;
    entity->updateRange.x = 0x800000;
    entity->updateRange.y = 0x800000;
    entity->drawFX        = FX_FLIP;
    RSDK.SetSpriteAnimation(Grabber->aniFrames, 0, &entity->animator1, true, 0);
    RSDK.SetSpriteAnimation(Grabber->aniFrames, 1, &entity->animator2, true, 0);
    RSDK.SetSpriteAnimation(Grabber->aniFrames, 4, &entity->animator3, true, 0);
    entity->state = Grabber_State_Setup;
}

void Grabber_StageLoad(void)
{
    Grabber->aniFrames          = RSDK.LoadSpriteAnimation("CPZ/Grabber.bin", SCOPE_STAGE);
    Grabber->hitbox1.left       = -8;
    Grabber->hitbox1.top        = -8;
    Grabber->hitbox1.right      = 8;
    Grabber->hitbox1.bottom     = 8;
    Grabber->hitboxRange.left   = -64;
    Grabber->hitboxRange.top    = 0;
    Grabber->hitboxRange.right  = 64;
    Grabber->hitboxRange.bottom = 128;
    Grabber->hitbox2.left       = -8;
    Grabber->hitbox2.top        = 0;
    Grabber->hitbox2.right      = 8;
    Grabber->hitbox2.bottom     = 16;
    DEBUGMODE_ADD_OBJ(Grabber);
    Grabber->sfxGrab    = RSDK.GetSFX("Global/Grab.wav");
    Grabber->sfxDestroy = RSDK.GetSFX("Global/Destroy.wav");
}

void Grabber_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(Grabber->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
}

void Grabber_DebugSpawn(void)
{
    RSDK_THIS(Grabber);
    EntityGrabber *grabber = CREATE_ENTITY(Grabber, NULL, entity->position.x, entity->position.y);
    grabber->direction     = entity->direction;
    grabber->startDir      = entity->direction;

    if (!entity->direction)
        grabber->velocity.x = -0x4000;
    else
        grabber->velocity.x = 0x4000;
}

void Grabber_CheckPlayerCollisions(void)
{
    RSDK_THIS(Grabber);
    foreach_active(Player, player)
    {
        if (player != (EntityPlayer *)entity->grabbedPlayer) {
            if (entity->state == Grabber_State_Unknown3) {
                if (Player_CheckCollisionTouch(player, entity, &Grabber->hitbox2)) {
                    RSDK.PlaySfx(Grabber->sfxGrab, 0, 255);
                    entity->state           = Grabber_State_Unknown7;
                    entity->isPermanent     = true;
                    entity->grabbedPlayer   = (Entity*)player;
                    player->velocity.x      = 0;
                    player->velocity.y      = 0;
                    player->groundVel       = 0;
                    player->state           = Player_State_None;
                    player->nextAirState    = StateMachine_None;
                    player->nextGroundState = StateMachine_None;
                    player->onGround        = false;
                    player->direction       = entity->direction;
                    RSDK.SetSpriteAnimation(player->spriteIndex, ANI_FAN, &player->playerAnimator, true, 0);
                    player->playerAnimator.animationSpeed = 0;
                    player->direction                     = entity->direction ^ 1;
                }
            }
            if (player != (EntityPlayer*)entity->grabbedPlayer && !entity->timer && Player_CheckBadnikTouch(player, entity, &Grabber->hitbox1)
                && Player_CheckBadnikBreak(entity, player, false)) {
                EntityPlayer *player = (EntityPlayer *)entity->grabbedPlayer;
                if (player)
                    player->state = Player_State_Air;
                destroyEntity(entity);
            }
        }
    }
}

void Grabber_CheckOnScreen(void)
{
    RSDK_THIS(Grabber);
    if (!RSDK.CheckOnScreen(RSDK_sceneInfo->entity, NULL) && !RSDK.CheckPosOnScreen(&entity->startPos, &entity->updateRange)) {
        entity->position      = entity->startPos;
        entity->field_A8      = 0;
        entity->struggleFlags = 0;
        entity->struggleFlags = 0;
        entity->direction     = entity->startDir;
        entity->grabbedPlayer = NULL;
        entity->field_BC      = 0;
        entity->explodeTimer  = 0;
        Grabber_Create(NULL);
    }
}

void Grabber_HandleExplode(void)
{
    RSDK_THIS(Grabber);
    if (!--entity->field_BC) {
        entity->animator1.frameID ^= 1;
        entity->field_BC = entity->explodeTimer;
        entity->explodeTimer--;
        if (!entity->explodeTimer) {
            EntityPlayer *player = (EntityPlayer *)entity->grabbedPlayer;
            if (player && player->state == Player_State_None) {
                Player_CheckHit(player, entity);
                if (player->state != Player_State_Hit && Player_CheckValidState(player))
                    player->state = Player_State_Air;
                entity->grabbedPlayer = NULL;
            }
            CREATE_ENTITY(Explosion, intToVoid(EXPLOSION_ENEMY), entity->position.x, entity->position.y)->drawOrder = Zone->drawOrderHigh;
            RSDK.PlaySfx(Explosion->sfxDestroy, false, 255);
            destroyEntity(entity);
        }
    }
}

void Grabber_State_Setup(void)
{
    RSDK_THIS(Grabber);
    entity->active = ACTIVE_NORMAL;
    entity->state  = Grabber_State_Unknown5;
    Grabber_State_Unknown5();
}

void Grabber_State_Unknown5(void)
{
    RSDK_THIS(Grabber);
    RSDK.ProcessAnimation(&entity->animator3);
    if (++entity->field_BC >= 256) {
        entity->field_BC = 0;
        RSDK.SetSpriteAnimation(Grabber->aniFrames, 3, &entity->animator1, true, 0);
        RSDK.SetSpriteAnimation(0xFFFF, 0, &entity->animator2, true, 0);
        entity->state = Grabber_State_Unknown6;
    }
    entity->position.x += entity->velocity.x;

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, entity, &Grabber->hitboxRange)) {
            entity->explodeTimer = 16;
            entity->state        = Grabber_State_Unknown2;
        }
    }

    Grabber_CheckPlayerCollisions();
    Grabber_CheckOnScreen();
}

void Grabber_State_Unknown2(void)
{
    RSDK_THIS(Grabber);
    if (--entity->explodeTimer < 0) {
        entity->explodeTimer = 32;
        entity->state        = Grabber_State_Unknown3;
    }
    Grabber_CheckPlayerCollisions();
    Grabber_CheckOnScreen();
}

void Grabber_State_Unknown6(void)
{
    RSDK_THIS(Grabber);

    RSDK.ProcessAnimation(&entity->animator1);
    if (entity->animator1.frameID == entity->animator1.frameCount - 1) {
        RSDK.SetSpriteAnimation(Grabber->aniFrames, 0, &entity->animator1, true, 0);
        RSDK.SetSpriteAnimation(Grabber->aniFrames, 1, &entity->animator2, true, 0);
        if (entity->direction == FLIP_X)
            entity->position.x -= 0x10000;
        else
            entity->position.x += 0x10000;

        entity->state      = Grabber_State_Unknown5;
        entity->velocity.x = -entity->velocity.x;
        entity->direction ^= 1;
        Grabber_State_Unknown5();
    }
    else {
        Grabber_CheckPlayerCollisions();
        Grabber_CheckOnScreen();
    }
}

void Grabber_State_Unknown3(void)
{
    RSDK_THIS(Grabber);
    if (--entity->explodeTimer < 0)
        entity->state = Grabber_State_Unknown4;
    entity->position.y += 0x20000;
    Grabber_CheckPlayerCollisions();
    Grabber_CheckOnScreen();
}

void Grabber_State_Unknown4(void)
{
    RSDK_THIS(Grabber);
    if (++entity->explodeTimer >= 32) {
        entity->explodeTimer = 0;
        entity->state        = Grabber_State_Unknown5;
    }
    entity->position.y -= 0x20000;
    Grabber_CheckPlayerCollisions();
    Grabber_CheckOnScreen();
}

void Grabber_State_Unknown7(void)
{
    RSDK_THIS(Grabber);

    RSDK.ProcessAnimation(&entity->animator1);
    RSDK.ProcessAnimation(&entity->animator2);
    if (++entity->explodeTimer < 32) {
        entity->position.y -= 0x20000;
        EntityPlayer *player = (EntityPlayer *)entity->grabbedPlayer;
        if (player) {
            player->playerAnimator.animationSpeed = 0;
            player->position.x                    = entity->position.x;
            player->position.y                    = entity->position.y;
            player->position.y += 0x100000;
            player->velocity.x = 0;
            player->direction  = entity->direction ^ 1;
            player->velocity.y = 0;
        }
        Grabber_CheckPlayerCollisions();
    }
    else if (entity->animator1.frameID != entity->animator1.frameCount - 1) {
        Grabber_CheckPlayerCollisions();
    }
    else {
        entity->field_BC     = 1;
        entity->explodeTimer = 16;
        RSDK.SetSpriteAnimation(Grabber->aniFrames, 2, &entity->animator1, true, 0);
        entity->state = Grabber_State_Struggle;
        Grabber_CheckPlayerCollisions();
    }

    Grabber_CheckOnScreen();
}

void Grabber_State_Struggle(void)
{
    RSDK_THIS(Grabber);
    EntityPlayer *player = (EntityPlayer *)entity->grabbedPlayer;
    if (player) {
        player->playerAnimator.animationSpeed = 0;
        if (entity->struggleFlags) {
            player->velocity.x = 0;
            player->velocity.y = 0;
            if (entity->field_A8) {
                uint8 flags = 0;
                entity->field_A8--;
                if (player->left)
                    flags = 1;
                if (player->right)
                    flags |= 2;
                if (flags) {
                    if (flags != 3 && flags != entity->struggleFlags) {
                        entity->struggleFlags = flags;
                        if (++entity->struggleTimer >= 4) {
                            player->state         = Player_State_Air;
                            entity->grabbedPlayer = 0;
                            entity->timer         = 16;
                            entity->state         = Grabber_State_Unknown9;
                        }
                    }
                }
            }
            else {
                entity->struggleTimer = 0;
                entity->struggleFlags = 0;
            }
        }
        else {
            if (player->left) {
                entity->struggleFlags = 1;
                entity->field_A8      = 64;
            }
            else if (player->right) {
                entity->struggleFlags = 2;
                entity->field_A8      = 64;
            }
        }
        player->position.x = entity->position.x;
        player->position.y = entity->position.y;
        player->position.y += 0x100000;
        player->direction = entity->direction ^ 1;
    }

    Grabber_CheckPlayerCollisions();
    Grabber_HandleExplode();
    Grabber_CheckOnScreen();
}

void Grabber_State_Unknown9(void)
{
    RSDK_THIS(Grabber);
    if (entity->timer)
        entity->timer--;
    Grabber_CheckPlayerCollisions();
    Grabber_HandleExplode();
    Grabber_CheckOnScreen();
}

#if RETRO_INCLUDE_EDITOR
void Grabber_EditorDraw(void)
{
    RSDK_THIS(Grabber);

    RSDK.DrawLine(entity->position.x, entity->startPos.y - 0x100000, entity->position.x, entity->position.y, 0x202020, 0, INK_NONE, false);
    RSDK.DrawLine(entity->position.x - 0x10000, entity->startPos.y - 0x100000, entity->position.x - 0x10000, entity->position.y, 0xE0E0E0, 0,
                  INK_NONE, false);

    Vector2 drawPos;
    int32 dir         = entity->direction;
    drawPos.x         = entity->position.x;
    drawPos.y         = entity->startPos.y;
    entity->direction = FLIP_NONE;
    RSDK.DrawSprite(&entity->animator3, &drawPos, false);
    entity->direction = dir;
    RSDK.DrawSprite(&entity->animator1, NULL, false);
    RSDK.DrawSprite(&entity->animator2, NULL, false);
}

void Grabber_EditorLoad(void) { Grabber->aniFrames = RSDK.LoadSpriteAnimation("CPZ/Grabber.bin", SCOPE_STAGE); }
#endif

void Grabber_Serialize(void) { RSDK_EDITABLE_VAR(Grabber, VAR_UINT8, direction); }
