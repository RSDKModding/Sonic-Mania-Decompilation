#include "SonicMania.h"

ObjectCanista *Canista;

void Canista_Update(void)
{
    RSDK_THIS(Canista);
    StateMachine_Run(entity->state);
}

void Canista_LateUpdate(void) {}

void Canista_StaticUpdate(void) {}

void Canista_Draw(void)
{
    RSDK_THIS(Canista);
    RSDK.DrawSprite(&entity->animator1, NULL, false);
    RSDK.DrawSprite(&entity->animator2, NULL, false);
    RSDK.DrawSprite(&entity->animator3, NULL, false);
}

void Canista_Create(void *data)
{
    RSDK_THIS(Canista);
    entity->visible   = true;
    entity->drawOrder = Zone->drawOrderLow + 1;
    if (data) {
        entity->drawFX |= FX_FLIP;
        entity->inkEffect     = INK_ALPHA;
        entity->alpha         = 256;
        entity->timer3        = 90;
        entity->updateRange.x = 0x400000;
        entity->updateRange.y = 0x400000;
        entity->active        = ACTIVE_NORMAL;
        entity->drawOrder     = Zone->drawOrderLow;
        RSDK.SetSpriteAnimation(Canista->aniFrames, 3, &entity->animator1, true, 0);
        entity->state = Canista_State2_Unknown1;
    }
    else {
        entity->drawFX |= FX_ROTATE | FX_FLIP;
        entity->shootOffset <<= 16;
        entity->shootSize <<= 16;
        entity->startPos.x     = entity->position.x;
        entity->startPos.y     = entity->position.y;
        entity->startDir       = entity->direction;
        entity->hitbox.left    = -entity->detectSize.x >> 17;
        entity->hitbox.top     = -entity->detectSize.x >> 17;
        entity->active         = ACTIVE_BOUNDS;
        entity->updateRange.x  = 0x800000;
        entity->updateRange.y  = 0x800000;
        entity->detectedPlayer = 0;
        entity->hitbox.right   = entity->detectSize.x >> 17;
        entity->hitbox.bottom  = entity->detectSize.x >> 17;
        RSDK.SetSpriteAnimation(Canista->aniFrames, 0, &entity->animator1, true, 0);
        RSDK.SetSpriteAnimation(Canista->aniFrames, 1, &entity->animator2, true, 0);
        RSDK.SetSpriteAnimation(Canista->aniFrames, 2, &entity->animator3, true, 0);
        entity->state = Canista_State_Setup;
    }
}

void Canista_StageLoad(void)
{
    if (RSDK.CheckStageFolder("SPZ1"))
        Canista->aniFrames = RSDK.LoadSpriteAnimation("SPZ1/Canista.bin", SCOPE_STAGE);
    else if (RSDK.CheckStageFolder("SPZ2"))
        Canista->aniFrames = RSDK.LoadSpriteAnimation("SPZ2/Canista.bin", SCOPE_STAGE);

    Canista->hitbox1.left   = -14;
    Canista->hitbox1.top    = -14;
    Canista->hitbox1.right  = 14;
    Canista->hitbox1.bottom = 14;
    Canista->hitbox2.left   = -128;
    Canista->hitbox2.top    = -128;
    Canista->hitbox2.right  = 128;
    Canista->hitbox2.bottom = 128;
    Canista->hitbox3.left   = -132;
    Canista->hitbox3.top    = 48;
    Canista->hitbox3.right  = 132;
    Canista->hitbox3.bottom = 49;
    Canista->hitbox4.left   = -26;
    Canista->hitbox4.top    = -5;
    Canista->hitbox4.right  = -16;
    Canista->hitbox4.bottom = 5;

    DEBUGMODE_ADD_OBJ(Canista);
    Canista->sfxPon = RSDK.GetSFX("Stage/Pon.wav");
}

void Canista_DebugSpawn(void)
{
    RSDK_THIS(DebugMode);

    EntityCanista *canista = CREATE_ENTITY(Canista, NULL, entity->position.x, entity->position.y);
    canista->direction     = entity->direction;
    canista->startDir      = entity->direction;
}

void Canista_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(Canista->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
}

void Canista_CheckPlayerCollisions(void)
{
    RSDK_THIS(Canista);

    int storeX = entity->position.x;
    int storeY = entity->position.y;

    foreach_active(Player, player)
    {
        if (!entity->timer2) {
            if (entity->triggerMode) {
                entity->position.x = entity->startPos.x;
                entity->position.y = entity->startPos.y;
                entity->position.x += entity->detectOffset.x;
                entity->position.y += entity->detectOffset.y;
                if (Player_CheckCollisionTouch(player, entity, &entity->hitbox)) {
                    EntityPlayer *playerPtr = (EntityPlayer *)entity->detectedPlayer;
                    if (playerPtr) {
                        if (abs(entity->position.x - player->position.x) < abs(entity->position.x - playerPtr->position.x))
                            entity->detectedPlayer = (Entity *)player;
                    }
                    else {
                        entity->detectedPlayer = (Entity *)player;
                    }
                }
            }
            else {
                if (Player_CheckCollisionTouch(player, entity, &Canista->hitbox3)) {
                    EntityPlayer *playerPtr = (EntityPlayer *)entity->detectedPlayer;
                    if (playerPtr) {
                        if (abs(entity->position.x - player->position.x) < abs(entity->position.x - playerPtr->position.x))
                            entity->detectedPlayer = (Entity *)player;
                    }
                    else {
                        entity->detectedPlayer = (Entity *)player;
                    }
                }
            }
        }

        entity->position.x = storeX;
        entity->position.y = storeY;
        if (Player_CheckBadnikTouch(player, entity, &Canista->hitbox1))
            Player_CheckBadnikBreak(entity, player, true);
    }
}

void Canista_CheckOnScreen(void)
{
    RSDK_THIS(Canista);

    if (!RSDK.CheckOnScreen(entity, NULL) && !RSDK.CheckPosOnScreen(&entity->startPos, &entity->updateRange)) {
        entity->position.x  = entity->startPos.x;
        entity->position.y  = entity->startPos.y;
        entity->direction   = entity->startDir;
        entity->shootOffset = (entity->shootOffset >> 16);
        Canista_Create(NULL);
    }
}

void Canista_State_Setup(void)
{
    RSDK_THIS(Canista);

    entity->active = ACTIVE_NORMAL;
    int offset     = -0x110000;
    if (!(entity->direction & FLIP_X))
        offset = 0x100000;
    RSDK.ObjectTileGrip(entity, Zone->fgLayers, (2 * ((entity->direction & FLIP_X) != 0) + 1), 0, offset, -0x180000, 8);
    entity->field_8D      = 0;
    entity->timer         = 0;
    entity->timer3        = 0;
    entity->updateRange.y = 0x2000000;
    entity->state         = Canista_State_Unknown1;
    Canista_State_Unknown1();
}

void Canista_State_Unknown1(void)
{
    RSDK_THIS(Canista);

    if (entity->timer2)
        entity->timer2--;

    EntityPlayer *player = (EntityPlayer *)entity->detectedPlayer;
    if (player) {
        int storeX = entity->position.x;
        int storeY = entity->position.y;
        if (entity->triggerMode) {
            entity->position.x = entity->startPos.x;
            entity->position.y = entity->startPos.y;
            entity->position.x += entity->detectOffset.x;
            entity->position.y += entity->detectOffset.y;

            if (!Player_CheckCollisionTouch(player, entity, &entity->hitbox)) {
                entity->detectedPlayer = NULL;
            }
        }
        else {
            if (!Player_CheckCollisionTouch(player, entity, &Canista->hitbox2)) {
                entity->detectedPlayer = NULL;
            }
        }

        if (entity->detectedPlayer) {
            if (entity->triggerMode) {
                int playerY = player->position.y & 0xFFFF0000;
                int shootY  = (storeY + entity->shootOffset - entity->shootSize) & 0xFFFF0000;

                if (playerY < shootY || playerY > shootY) {
                    entity->animator1.animationSpeed = 128;
                    entity->velocity.y = ((entity->shootOffset + storeY) & 0xFFFF0000) < (player->position.y & 0xFFFF0000) ? 0x10000 : -0x10000;
                }
                else {
                    entity->state          = Canista_State_Unknown2;
                    entity->timer3         = 60;
                    entity->detectedPlayer = 0;
                    entity->timer2         = 180;
                }
            }
            else {
                bool32 collided = Player_CheckCollisionTouch(player, entity, &Canista->hitbox3);
                if (!collided) {
                    entity->animator1.animationSpeed = 128;
                    if (player->position.y - 0x400000 > storeY)
                        entity->velocity.y = 0x10000;
                    else
                        entity->velocity.y = -0x1000;
                }
                else {
                    entity->state          = Canista_State_Unknown2;
                    entity->timer3         = 60;
                    entity->detectedPlayer = 0;
                    entity->timer2         = 180;
                }
            }
        }
        entity->position.x = storeX;
        entity->position.y = storeY;
    }
    else {
        entity->animator1.animationSpeed = 85;
        if (entity->field_8D)
            entity->velocity.y = -0x8000;
        else
            entity->velocity.y = 0x8000;
    }

    entity->position.y += entity->velocity.y;
    int rotate = entity->velocity.y >> 12;
    if (entity->direction)
        entity->rotation += rotate;
    else
        entity->rotation -= rotate;

    int offset = -0x110000;
    if (!(entity->direction & FLIP_X))
        offset = 0x100000;
    if (!RSDK.ObjectTileGrip(entity, Zone->fgLayers, (2 * ((entity->direction & 1) != 0) + 1), 0, offset,
                             ((entity->velocity.y >> 31) & 0xFFD40000) + 0x140000, 0)) {
        entity->state  = Canista_State_Unknown2;
        entity->timer3 = 30;
        if (entity->detectedPlayer) {
            entity->detectedPlayer = 0;
            entity->timer2         = 180;
        }
    }
    RSDK.ProcessAnimation(&entity->animator1);

    if (entity->timer) {
        entity->timer--;
        if (entity->timer == 1) {
            entity->animator3.frameID = 3;
        }
    }
    else {
        entity->animator3.animationTimer = (entity->animator3.animationTimer + (abs(entity->velocity.y) >> 15)) % 7;
        entity->animator3.frameID        = (entity->animator3.animationTimer / 3) & 1;
    }
    Canista_CheckPlayerCollisions();
    Canista_CheckOnScreen();
}

void Canista_State_Unknown2(void)
{
    RSDK_THIS(Canista);
    if (!--entity->timer3) {
        if (abs(entity->velocity.y) == 0x8000)
            entity->field_8D ^= 1;
        entity->state = Canista_State_Unknown1;
        Canista_State_Unknown1();
    }
    else {
        if (entity->timer3 == 30) {
            int offsetX = -0x180000;
            if (!(entity->direction & 1))
                offsetX = 0x180000;

            EntityCanista *shot = CREATE_ENTITY(Canista, intToVoid(true), offsetX + entity->position.x, entity->position.y);
            shot->direction     = entity->direction;
            if (shot->direction == FLIP_X)
                shot->velocity.x = 0x40000;
            else
                shot->velocity.x = -0x40000;

            shot            = CREATE_ENTITY(Canista, intToVoid(true), offsetX + entity->position.x, entity->position.y);
            shot->direction = entity->direction;
            if (shot->direction == FLIP_X)
                shot->velocity.x = 0x4C000;
            else
                shot->velocity.x = -0x4C000;

            shot            = CREATE_ENTITY(Canista, intToVoid(true), offsetX + entity->position.x, entity->position.y);
            shot->direction = entity->direction;
            if (shot->direction == FLIP_X)
                shot->velocity.x = 0x58000;
            else
                shot->velocity.x = -0x58000;

            shot            = CREATE_ENTITY(Canista, intToVoid(true), offsetX + entity->position.x, entity->position.y);
            shot->direction = entity->direction;
            if (shot->direction == FLIP_X)
                shot->velocity.x = 0x64000;
            else
                shot->velocity.x = -0x64000;

            entity->animator3.frameID = 2;
            entity->timer             = 4;
            RSDK.PlaySfx(Canista->sfxPon, false, 255);
        }
        Canista_CheckPlayerCollisions();
        Canista_CheckOnScreen();
    }

    if (entity->timer) {
        entity->timer--;
        if (entity->timer == 1) {
            entity->animator3.frameID = 3;
        }
    }
    else {
        entity->animator3.frameID = (entity->animator3.animationTimer / 3) & 1;
    }
    Canista_CheckOnScreen();
}

void Canista_CheckPlayerCollisions2(void)
{
    RSDK_THIS(Canista);
    if (entity->alpha >= 0x80) {

        foreach_active(Player, player)
        {
            if (Player_CheckCollisionTouch(player, entity, &Canista->hitbox4)) {
                int anim = player->playerAnimator.animationID;
#if RETRO_USE_PLUS
                if (player->characterID == ID_MIGHTY && (anim == ANI_CROUCH || anim == ANI_JUMP || anim == ANI_SPINDASH || anim == ANI_DROPDASH)) {
                    int angle           = RSDK.ATan2(player->position.x - entity->position.x, player->position.y - entity->position.y);
                    entity->velocity.x  = -0x400 * RSDK.Cos256(angle);
                    entity->velocity.y  = -0x400 * RSDK.Sin256(angle);
                    entity->timer3      = 0;
                    entity->interaction = false;
                }
                else
#endif
                    Player_CheckHit(player, entity);
            }
        }
    }

    RSDK.ProcessAnimation(&entity->animator1);
    if (!RSDK.CheckOnScreen(entity, &entity->updateRange))
        destroyEntity(entity);
}

void Canista_State2_Unknown1(void)
{
    RSDK_THIS(Canista);
    entity->position.x += entity->velocity.x;
    if (entity->direction == FLIP_X) {
        entity->velocity.x -= 0x2000;
        if (entity->velocity.x - 0x2000 < 0) {
            entity->startPos.x = entity->position.x;
            entity->startPos.y = entity->position.y;
            entity->velocity.x = 0;
            entity->rotation   = 6;
            entity->angle      = 2;
            RSDK.SetSpriteAnimation(Canista->aniFrames, 4, &entity->animator1, true, 0);
            entity->state = Canista_State2_Unknown2;
        }
    }
    else {
        entity->velocity.x += 0x2000;
        if (entity->velocity.x > 0) {
            entity->startPos.x = entity->position.x;
            entity->startPos.y = entity->position.y;
            entity->velocity.x = 0;
            entity->rotation   = 6;
            entity->angle      = 2;
            RSDK.SetSpriteAnimation(Canista->aniFrames, 4, &entity->animator1, true, 0);
            entity->state = Canista_State2_Unknown2;
        }
    }
    Canista_CheckPlayerCollisions2();
}

void Canista_State2_Unknown2(void)
{
    RSDK_THIS(Canista);
    entity->angle += entity->rotation;
    if (!(Zone->timer & 0x1F) && RSDK.Rand(0, 2) == 1)
        entity->rotation = -entity->rotation;

    entity->startPos.x += entity->velocity.x;
    entity->startPos.y += entity->velocity.y;
    entity->velocity.y += 0x800;
    if (entity->velocity.y > 0x10000) {
        entity->velocity.y -= 0x8000;
        if (entity->velocity.y < 0x10000)
            entity->velocity.y = 0x10000;
    }

    if (entity->velocity.x) {
        if (entity->velocity.x <= 0) {
            entity->velocity.x += 0x2000;
            if (entity->velocity.x > 0)
                entity->velocity.x = 0;
        }
        else {
            entity->velocity.x -= 0x2000;
            if (entity->velocity.x < 0)
                entity->velocity.x = 0;
        }
    }

    entity->position.x = (RSDK.Cos256(entity->angle) << 9) + entity->startPos.x;
    entity->position.y = (RSDK.Sin256(entity->angle) << 9) + entity->startPos.y;

    if (entity->timer3) {
        entity->timer3--;
    }
    else {
        entity->alpha -= 4;
        if (entity->alpha <= 0)
            destroyEntity(entity);
    }
    Canista_CheckPlayerCollisions2();
}

#if RETRO_INCLUDE_EDITOR
void Canista_EditorDraw(void) { Canista_Draw(); }

void Canista_EditorLoad(void)
{
    if (RSDK.CheckStageFolder("SPZ1"))
        Canista->aniFrames = RSDK.LoadSpriteAnimation("SPZ1/Canista.bin", SCOPE_STAGE);
    else if (RSDK.CheckStageFolder("SPZ2"))
        Canista->aniFrames = RSDK.LoadSpriteAnimation("SPZ2/Canista.bin", SCOPE_STAGE);
}
#endif

void Canista_Serialize(void)
{
    RSDK_EDITABLE_VAR(Canista, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(Canista, VAR_UINT8, triggerMode);
    RSDK_EDITABLE_VAR(Canista, VAR_VECTOR2, detectOffset);
    RSDK_EDITABLE_VAR(Canista, VAR_VECTOR2, detectSize);
    RSDK_EDITABLE_VAR(Canista, VAR_ENUM, shootOffset);
    RSDK_EDITABLE_VAR(Canista, VAR_ENUM, shootSize);
}
