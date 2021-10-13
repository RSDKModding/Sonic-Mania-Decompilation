#include "SonicMania.h"

ObjectWoodrow *Woodrow;

void Woodrow_Update(void)
{
    RSDK_THIS(Woodrow);
    StateMachine_Run(entity->state);
}

void Woodrow_LateUpdate(void) {}

void Woodrow_StaticUpdate(void) {}

void Woodrow_Draw(void)
{
    RSDK_THIS(Woodrow);
    RSDK.DrawSprite(&entity->animator, NULL, false);
}

void Woodrow_Create(void *data)
{
    RSDK_THIS(Woodrow);
    entity->visible = 1;
    entity->drawFX |= FX_FLIP;
    entity->drawOrder     = Zone->drawOrderLow - 1;
    entity->active        = ACTIVE_BOUNDS;
    entity->updateRange.x = 0x800000;
    entity->updateRange.y = 0x800000;
    if (!RSDK_sceneInfo->inEditor) {
        if (entity->type == 1) {
            entity->startPos.x = entity->position.x;
            entity->startPos.y = entity->position.y;
            RSDK.SetSpriteAnimation(Woodrow->aniFrames, 2, &entity->animator, true, 0);
            entity->state = Woodrow_Unknown8;
        }
        else if (data) {
            RSDK.SetSpriteAnimation(Woodrow->aniFrames, 3, &entity->animator, true, 0);
            entity->state = Woodrow_Unknown7;
        }
        else {
            entity->drawOrder = Zone->drawOrderHigh;
            entity->bombCount      = 0;
            int32 pos           = RSDK_sceneInfo->entitySlot + 1;
            EntityWoodrow *bombSpawn                   = RSDK_GET_ENTITY(pos, Woodrow);
            while (bombSpawn->objectID == Woodrow->objectID) {
                if (bombSpawn->type != 1)
                    break;
                bombSpawn->parent = (Entity *)entity;
                ++entity->bombCount;
                ++pos;
                bombSpawn = RSDK_GET_ENTITY(pos, Woodrow);
            }
            entity->startPos.x = entity->position.x;
            entity->startPos.y = entity->position.y;
            entity->startDir   = entity->direction;
            entity->field_6A   = 60;
            entity->distUp     = entity->position.y - (entity->distUp << 16);
            entity->distDown   = entity->position.y + (entity->distDown << 16);
            entity->field_6B   = RSDK.Rand(15, 120);
            RSDK.SetSpriteAnimation(Woodrow->aniFrames, 0, &entity->animator, true, 0);
            entity->state = Woodrow_State_Setup;
        }
    }
}

void Woodrow_StageLoad(void)
{
    if (RSDK.CheckStageFolder("PSZ1"))
        Woodrow->aniFrames = RSDK.LoadSpriteAnimation("PSZ1/Woodrow.bin", SCOPE_STAGE);
    else if (RSDK.CheckStageFolder("PSZ2"))
        Woodrow->aniFrames = RSDK.LoadSpriteAnimation("PSZ2/Woodrow.bin", SCOPE_STAGE);

    Woodrow->hitbox1.left   = -32;
    Woodrow->hitbox1.top    = -24;
    Woodrow->hitbox1.right  = 12;
    Woodrow->hitbox1.bottom = 5;
    Woodrow->hitbox2.left   = -112;
    Woodrow->hitbox2.top    = -128;
    Woodrow->hitbox2.right  = 144;
    Woodrow->hitbox2.bottom = 128;
    Woodrow->hitbox3.left   = -48;
    Woodrow->hitbox3.top    = -128;
    Woodrow->hitbox3.right  = 80;
    Woodrow->hitbox3.bottom = 128;
    Woodrow->hitbox4.left   = -12;
    Woodrow->hitbox4.top    = -12;
    Woodrow->hitbox4.right  = 12;
    Woodrow->hitbox4.bottom = 12;

    Woodrow->sfxExplosion = RSDK.GetSFX("Stage/Explosion2.wav");
    Woodrow->sfxPeck      = RSDK.GetSFX("PSZ/Peck.wav");
    DEBUGMODE_ADD_OBJ(Woodrow);
}

void Woodrow_DebugSpawn(void)
{
    RSDK_THIS(Woodrow);
    CREATE_ENTITY(Woodrow, NULL, entity->position.x, entity->position.y);
}

void Woodrow_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(Woodrow->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, 0, false);
}

void Woodrow_CheckPlayerCollisions(void)
{
    RSDK_THIS(Woodrow);

    uint8 flags = 0;
    foreach_active(Player, player)
    {
        if (Player_CheckBadnikHit(player, entity, &Woodrow->hitbox1)) {
            Player_CheckBadnikBreak(entity, player, true);
        }
        else if (player->state != Ice_State_FrozenPlayer) {
            if (Player_CheckCollisionTouch(player, entity, &Woodrow->hitbox3)) {
                if (entity->field_6C == 1)
                    entity->field_6A = 30;
                flags |= 2;
                entity->field_6C |= 2;
            }
            else if (Player_CheckCollisionTouch(player, entity, &Woodrow->hitbox2)) {
                entity->field_6C |= 1;
                flags |= 1;
            }
        }
    }

    if (flags) {
        if (flags == 1 && entity->field_6A > 30)
            entity->field_6A = 30;
    }
    else {
        if (entity->animator.animationID == 1)
            RSDK.SetSpriteAnimation(Woodrow->aniFrames, 4, &entity->animator, false, 0);
        entity->field_6C = 0;
    }
}

void Woodrow_CheckOnScreen(void)
{
    RSDK_THIS(Woodrow);
    if (!RSDK.CheckOnScreen(entity, NULL) && !RSDK.CheckPosOnScreen(&entity->startPos, &entity->updateRange)) {
        entity->position.x = entity->startPos.x;
        entity->position.y = entity->startPos.y;
        entity->direction  = entity->startDir;
        entity->distUp     = (entity->position.y - entity->distUp) >> 16;
        entity->distDown   = (entity->distDown - entity->position.y) >> 16;
        Woodrow_Create(NULL);
    }
}

void Woodrow_State_Setup(void)
{
    RSDK_THIS(Woodrow);
    entity->active = ACTIVE_NORMAL;
    entity->state  = Woodrow_Unknown4;
    Woodrow_Unknown4();
}

void Woodrow_Unknown4(void)
{
    RSDK_THIS(Woodrow);
    RSDK.ProcessAnimation(&entity->animator);

    if (entity->field_6C <= 1) {
        if (entity->animator.animationID <= 3) {
            if (entity->animator.animationID == 1)
                RSDK.SetSpriteAnimation(Woodrow->aniFrames, 4, &entity->animator, false, 0);
            entity->field_6C = 0;
        }
        else if (entity->animator.frameID == entity->animator.frameCount - 1) {
            RSDK.SetSpriteAnimation(Woodrow->aniFrames, 0, &entity->animator, false, 0);
        }
        if ((entity->distUp == entity->distDown) != entity->startPos.y) {
            if (!--entity->field_6B) {
                entity->field_6B = RSDK.Rand(15, 120);
                entity->field_6A = RSDK.Rand(8, 30);

                if (entity->position.y > entity->distUp) {
                    if (entity->position.y < entity->distDown && RSDK.Rand(0, 2) == 1)
                        entity->state = Woodrow_Unknown6;
                    else
                        entity->state = Woodrow_Unknown5;
                }
                else {
                    entity->state = Woodrow_Unknown6;
                }

                if (entity->state == Woodrow_Unknown6)
                    RSDK.SetSpriteAnimation(Woodrow->aniFrames, 6, &entity->animator, true, 0);
                else
                    RSDK.SetSpriteAnimation(Woodrow->aniFrames, 5, &entity->animator, true, 0);
            }
        }
        Woodrow_CheckPlayerCollisions();
        Woodrow_CheckOnScreen();
    }
    else {
        if (!(Zone->timer & 7))
            RSDK.PlaySfx(Woodrow->sfxPeck, false, 255);
        RSDK.SetSpriteAnimation(Woodrow->aniFrames, 1, &entity->animator, false, 0);

        if (--entity->field_6A) {
            Woodrow_CheckPlayerCollisions();
            Woodrow_CheckOnScreen();
        }
        else if (!entity->bombCount) {
            entity->field_6A = -76;
            if (entity->field_68 == 0) {
                entity->field_68 = 1;
                Woodrow_CheckPlayerCollisions();
                Woodrow_CheckOnScreen();
            }
            else {
                entity->field_6C = 0;
                Woodrow_CheckPlayerCollisions();
                Woodrow_CheckOnScreen();
            }
        }
        if (entity->field_68 >= entity->bombCount) {
            entity->field_6A = 30;
            entity->field_6C = 0;
            Woodrow_CheckPlayerCollisions();
            Woodrow_CheckOnScreen();
        }
        else {
            int32 pos = RSDK_sceneInfo->entitySlot + 1;

            EntityWoodrow *bombSpawn = NULL;
            do {
                bombSpawn = RSDK_GET_ENTITY(pos + RSDK.Rand(0, entity->bombCount), Woodrow);
            } while (bombSpawn->field_68);

            EntityWoodrow *bomb = CREATE_ENTITY(Woodrow, intToVoid(1), bombSpawn->position.x, bombSpawn->position.y);
            bombSpawn->position.y -= 0x100000;
            bombSpawn->field_6A = 120;
            bombSpawn->field_68 = 32;
            bombSpawn->active   = ACTIVE_NORMAL;
            bomb->parent        = (Entity *)bombSpawn->parent;
            if (++entity->field_68 == entity->bombCount)
                entity->field_6A = -76;
            else
                entity->field_6A = 60;
            Woodrow_CheckPlayerCollisions();
            Woodrow_CheckOnScreen();
        }
    }
}

void Woodrow_Unknown5(void)
{
    RSDK_THIS(Woodrow);
    RSDK.ProcessAnimation(&entity->animator);
    if (entity->animator.frameID >= 3) {
        entity->position.y -= 0x20000;
        if (entity->position.y < entity->distUp)
            entity->position.y = entity->distUp;
    }
    if (entity->animator.frameID == entity->animator.frameCount - 1) {
        RSDK.SetSpriteAnimation(Woodrow->aniFrames, 4, &entity->animator, false, 0);
        entity->state = Woodrow_Unknown4;
    }
    Woodrow_CheckPlayerCollisions();
    Woodrow_CheckOnScreen();
}

void Woodrow_Unknown6(void)
{
    RSDK_THIS(Woodrow);
    RSDK.ProcessAnimation(&entity->animator);
    entity->velocity.y += 0x3800;
    entity->position.y += entity->velocity.y;

    if (!--entity->field_6A) {
        entity->velocity.y = 0;
        RSDK.SetSpriteAnimation(Woodrow->aniFrames, 4, &entity->animator, false, 0);
        entity->state = Woodrow_Unknown4;
    }
    else if (entity->position.y >= entity->distDown) {
        entity->position.y = entity->distDown;
        entity->velocity.y = 0;
        RSDK.SetSpriteAnimation(Woodrow->aniFrames, 4, &entity->animator, false, 0);
        entity->state = Woodrow_Unknown4;
    }
    Woodrow_CheckPlayerCollisions();
    Woodrow_CheckOnScreen();
}

void Woodrow_Unknown7(void)
{
    RSDK_THIS(Woodrow);
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    entity->velocity.y += 0x3800;
    if (RSDK.CheckOnScreen(entity, &entity->updateRange)) {
        if (RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x80000, true)) {
            RSDK.PlaySfx(Woodrow->sfxExplosion, false, 255);
            CREATE_ENTITY(Explosion, intToVoid(1), entity->position.x, entity->position.y)->drawOrder = Zone->drawOrderHigh;
            destroyEntity(entity);
        }
        RSDK.ProcessAnimation(&entity->animator);

        foreach_active(Player, player)
        {
            if (Player_CheckCollisionTouch(player, entity, &Woodrow->hitbox4)) {
#if RETRO_USE_PLUS
                if (!Player_CheckMightyUnspin(768, player, 2, &player->uncurlTimer))
#endif
                    Player_CheckHit(player, entity);
                RSDK.PlaySfx(Woodrow->sfxExplosion, false, 255);
                CREATE_ENTITY(Explosion, intToVoid(1), entity->position.x, entity->position.y)->drawOrder = Zone->drawOrderHigh;
                destroyEntity(entity);
                foreach_break;
            }
        }
    }
    else {
        destroyEntity(entity);
    }
}

void Woodrow_Unknown8(void)
{
    RSDK_THIS(Woodrow);
    if (entity->field_6A) {
        entity->visible = false;
        entity->field_6A--;
    }
    else {
        entity->visible = true;
        if (entity->field_68) {
            entity->position.y += 0x8000;
            entity->field_68--;
        }
        else {
            EntityWoodrow *parent = (EntityWoodrow *)entity->parent;
            if (parent) {
                entity->position.x = entity->startPos.x;
                entity->position.y = entity->startPos.y;
                if (parent->objectID != Woodrow->objectID) {
                    entity->active = ACTIVE_BOUNDS;
                }
                else {
                    if (parent->animator.animationID == 1) {
                        entity->position.x += RSDK.Rand(-1, 2) << 16;
                        entity->position.y += RSDK.Rand(-1, 2) << 16;
                    }
                    if (entity->active == ACTIVE_NORMAL) {
                        --parent->field_68;
                        entity->active = ACTIVE_BOUNDS;
                    }
                }
            }
        }
    }
}

void Woodrow_EditorDraw(void) {}

void Woodrow_EditorLoad(void) {}

void Woodrow_Serialize(void)
{
    RSDK_EDITABLE_VAR(Woodrow, VAR_UINT8, type);
    RSDK_EDITABLE_VAR(Woodrow, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(Woodrow, VAR_ENUM, distUp);
    RSDK_EDITABLE_VAR(Woodrow, VAR_ENUM, distDown);
}
