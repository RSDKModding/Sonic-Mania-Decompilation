#include "SonicMania.h"

ObjectAquis *Aquis;

void Aquis_Update(void)
{
    RSDK_THIS(Aquis);
    StateMachine_Run(entity->state);
}

void Aquis_LateUpdate(void) {}

void Aquis_StaticUpdate(void) {}

void Aquis_Draw(void)
{
    RSDK_THIS(Aquis);

    if (entity->animator1.animationID == 4) {
        RSDK.DrawSprite(&entity->animator1, NULL, false);
    }
    else {
        RSDK.DrawSprite(&entity->animator1, NULL, false);
        RSDK.DrawSprite(&entity->animator2, NULL, false);
    }
}

void Aquis_Create(void *data)
{
    RSDK_THIS(Aquis);
    entity->visible   = true;
    entity->drawOrder = Zone->drawOrderLow;
    entity->drawFX |= FX_FLIP;
    entity->startPos = entity->position;
    entity->startDir = entity->direction;
    entity->timer    = 128;
    entity->field_60 = 0;

    if (data) {
        entity->active        = ACTIVE_NORMAL;
        entity->updateRange.x = 0x200000;
        entity->updateRange.y = 0x200000;
        RSDK.SetSpriteAnimation(Aquis->aniFrames, 2, &entity->animator1, true, 0);
        entity->state = Aquis_State_Shot;
    }
    else {
        entity->active        = ACTIVE_BOUNDS;
        entity->updateRange.x = 0x800000;
        entity->updateRange.y = 0x800000;
        entity->timer2        = 3;
        RSDK.SetSpriteAnimation(Aquis->aniFrames, 0, &entity->animator1, true, 0);
        RSDK.SetSpriteAnimation(Aquis->aniFrames, 3, &entity->animator2, true, 0);
        entity->state = Aquis_State_Setup;
    }
}

void Aquis_StageLoad(void)
{
    if (RSDK.CheckStageFolder("OOZ1") || RSDK.CheckStageFolder("OOZ2"))
        Aquis->aniFrames = RSDK.LoadSpriteAnimation("OOZ/Aquis.bin", SCOPE_STAGE);
    Aquis->hitbox1.left   = -10;
    Aquis->hitbox1.top    = -16;
    Aquis->hitbox1.right  = 10;
    Aquis->hitbox1.bottom = 16;
    Aquis->hitbox2.left   = -96;
    Aquis->hitbox2.top    = -64;
    Aquis->hitbox2.right  = 64;
    Aquis->hitbox2.bottom = 128;
    Aquis->hitbox3.left   = -3;
    Aquis->hitbox3.top    = -3;
    Aquis->hitbox3.right  = 3;
    Aquis->hitbox3.bottom = 3;
    Aquis->sfxShot        = RSDK.GetSFX("Stage/Shot.wav");
    DEBUGMODE_ADD_OBJ(Aquis);
}

void Aquis_DebugSpawn(void)
{
    RSDK_THIS(Aquis);

    EntityAquis *aquis = CREATE_ENTITY(Aquis, NULL, entity->position.x, entity->position.y);
    aquis->direction   = entity->direction;
    aquis->startDir    = entity->direction;
}

void Aquis_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(Aquis->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, 0, false);
}

void Aquis_CheckPlayerCollisions(void)
{
    RSDK_THIS(Aquis);
    foreach_active(Player, player)
    {
        if (Player_CheckBadnikTouch(player, entity, &Aquis->hitbox1))
            Player_CheckBadnikBreak(entity, player, true);
    }
}

void Aquis_CheckOnScreen(void)
{
    RSDK_THIS(Aquis);
    if (!RSDK.CheckOnScreen(entity, NULL) && !RSDK.CheckPosOnScreen(&entity->startPos, &entity->updateRange)) {
        entity->position.x = entity->startPos.x;
        entity->position.y = entity->startPos.y;
        entity->direction  = entity->startDir;
        Aquis_Create(NULL);
    }
}

void Aquis_State_Setup(void)
{
    RSDK_THIS(Aquis);
    entity->active = ACTIVE_NORMAL;
    entity->timer  = 32;
    entity->state  = Aquis_Unknown5;
    Aquis_Unknown5();
}

void Aquis_Unknown5(void)
{
    RSDK_THIS(Aquis);
    entity->timer--;
    if (!entity->timer) {
        int32 timer = entity->timer2;
        entity->timer2--;
        if (timer) {
            entity->velocity.y = -0x10000;
            entity->timer      = 128;
            entity->field_60   = 0;
            entity->state      = Aquis_Unknown6;
        }
        else {
            entity->velocity.y = 0;
            if (entity->direction == FLIP_X) {
                entity->velocity.x = 0;
                entity->state      = Aquis_Unknown8;
                RSDK.SetSpriteAnimation(Aquis->aniFrames, 4, &entity->animator1, true, 0);
            }
            else {
                entity->velocity.x = -0x20000;
                entity->state      = Aquis_Unknown9;
            }
        }
    }
    RSDK.ProcessAnimation(&entity->animator1);
    RSDK.ProcessAnimation(&entity->animator2);
    Aquis_CheckPlayerCollisions();
    Aquis_CheckOnScreen();
}

void Aquis_Unknown6(void)
{
    RSDK_THIS(Aquis);

    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    EntityPlayer *playerPtr = Player_GetNearestPlayer();
    if (playerPtr) {
        bool32 flag = false;
        if (playerPtr->position.x >= entity->position.x) {
            entity->velocity.x += 4096;
            if (entity->velocity.x > 0x10000)
                entity->velocity.x = 0x10000;
            flag = entity->direction == FLIP_NONE;
        }
        else {
            entity->velocity.x -= 4096;
            if (entity->velocity.x < -0x10000)
                entity->velocity.x = -0x10000;
            flag = entity->direction == FLIP_X;
        }

        if (flag) {
            entity->state = Aquis_Unknown8;
            RSDK.SetSpriteAnimation(Aquis->aniFrames, 4, &entity->animator1, true, 0);
        }
        if (playerPtr->position.y >= entity->position.y) {
            entity->velocity.y += 0x1000;
            if (entity->velocity.y > 0x10000)
                entity->velocity.y = 0x10000;
        }
        else {
            entity->velocity.y -= 0x1000;
            if (entity->velocity.y < -0x10000)
                entity->velocity.y = -0x10000;
        }
    }

    if (entity->state != Aquis_Unknown8) {
        int32 timer = entity->timer2;
        entity->timer--;
        if (!timer) {
            entity->timer      = 32;
            entity->state      = Aquis_Unknown5;
            entity->velocity.x = 0;
            entity->velocity.y = 0;
        }

        if (!entity->field_60) {
            foreach_active(Player, player)
            {
                if (player->velocity.y < 0 && !player->onGround)
                    continue;

                if (Player_CheckCollisionTouch(player, entity, &Aquis->hitbox2)) {
                    entity->field_60 = 1;
                    entity->timer    = 64;
                    RSDK.SetSpriteAnimation(Aquis->aniFrames, 1, &entity->animator1, true, 0);
                    entity->state = Aquis_Unknown7;
                    if (player->position.x >= entity->position.x) {
                        if (player->velocity.x <= (entity->position.x - player->position.x) >> 5) {
                            RSDK.SetSpriteAnimation(Aquis->aniFrames, 4, &entity->animator1, true, 0);
                            entity->state = Aquis_Unknown8;
                        }
                    }
                    else if (player->velocity.x >= (entity->position.x - player->position.x) >> 5) {
                        RSDK.SetSpriteAnimation(Aquis->aniFrames, 4, &entity->animator1, true, 0);
                        entity->state = Aquis_Unknown8;
                    }
                }
            }
        }
    }
    RSDK.ProcessAnimation(&entity->animator1);
    RSDK.ProcessAnimation(&entity->animator2);
    Aquis_CheckPlayerCollisions();
    Aquis_CheckOnScreen();
}

void Aquis_Unknown7(void)
{
    RSDK_THIS(Aquis);
    entity->timer--;
    if (entity->timer) {
        if (entity->timer == 33) {
            EntityAquis *shot = CREATE_ENTITY(Aquis, intToVoid(true), entity->position.x, entity->position.y);
            if (entity->direction) {
                shot->position.x += 0x100000;
                shot->velocity.x = 0x30000;
            }
            else {
                shot->position.x -= 0x100000;
                shot->velocity.x = -0x30000;
            }
            shot->position.y -= 0xA0000;
            shot->velocity.y = 0x20000;
            shot->direction  = entity->direction;
            shot->active     = ACTIVE_NORMAL;
            RSDK.PlaySfx(Aquis->sfxShot, false, 255);
        }
    }
    else {
        entity->timer      = 32;
        entity->timer2     = 0;
        entity->state      = Aquis_Unknown5;
        entity->velocity.x = 0;
        entity->velocity.y = 0;
    }
    RSDK.ProcessAnimation(&entity->animator1);
    RSDK.ProcessAnimation(&entity->animator2);
    Aquis_CheckPlayerCollisions();
    Aquis_CheckOnScreen();
}

void Aquis_Unknown8(void)
{
    RSDK_THIS(Aquis);

    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    RSDK.ProcessAnimation(&entity->animator1);
    if (entity->animator1.frameID == entity->animator1.frameCount - 1) {
        RSDK.SetSpriteAnimation(Aquis->aniFrames, 0, &entity->animator1, true, 0);
        entity->direction ^= 1;
        if (entity->timer2 < 0) {
            entity->velocity.x = -0x20000;
            entity->state      = Aquis_Unknown9;
            Aquis_Unknown9();
        }
        else if (entity->field_60) {
            entity->timer = 64;
            RSDK.SetSpriteAnimation(Aquis->aniFrames, 1, &entity->animator1, true, 0);
            entity->state = Aquis_Unknown7;
            Aquis_Unknown7();
        }
        else {
            entity->state = Aquis_Unknown6;
            Aquis_Unknown6();
        }
    }
    else {
        if (entity->timer > 1)
            entity->timer--;
        Aquis_CheckPlayerCollisions();
        Aquis_CheckOnScreen();
    }
}

void Aquis_Unknown9(void)
{
    RSDK_THIS(Aquis);
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    RSDK.ProcessAnimation(&entity->animator2);
    Aquis_CheckPlayerCollisions();
    Aquis_CheckOnScreen();
}

void Aquis_State_Shot(void)
{
    RSDK_THIS(Aquis);
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    if (RSDK.CheckOnScreen(entity, 0)) {
        RSDK.ProcessAnimation(&entity->animator1);
        foreach_active(Player, player)
        {
            if (Player_CheckCollisionTouch(player, entity, &Aquis->hitbox3))
                Player_CheckProjectileHit(player, entity);
        }
    }
    else {
        destroyEntity(entity);
    }
}

void Aquis_EditorDraw(void) {}

void Aquis_EditorLoad(void) {}

void Aquis_Serialize(void) {}
