#include "SonicMania.h"

#if RETRO_USE_PLUS
ObjectRhinobot *Rhinobot;

void Rhinobot_Update(void)
{
    RSDK_THIS(Rhinobot);
    StateMachine_Run(entity->state);
}

void Rhinobot_LateUpdate(void) {}

void Rhinobot_StaticUpdate(void) {}

void Rhinobot_Draw(void)
{
    RSDK_THIS(Rhinobot);
    if (entity->flag) {
        Vector2 drawPos;
        drawPos.x = entity->position.x;
        drawPos.y = entity->position.y;
        if (entity->state == Rhinobot_State2) {
            if (!entity->field_6C)
                drawPos.x -= 0xD0000;
            else
                drawPos.x += 0xD0000;
        }
        RSDK.DrawSprite(&entity->animatorDust, &drawPos, false);
    }
    RSDK.DrawSprite(&entity->animatorBot, 0, false);
}

void Rhinobot_Create(void *data)
{
    RSDK_THIS(Rhinobot);
    entity->visible = true;
    entity->drawFX |= FX_FLIP;
    entity->drawOrder     = Zone->drawOrderLow;
    entity->startPos      = entity->position;
    entity->startDir      = entity->direction;
    entity->active        = ACTIVE_BOUNDS;
    entity->updateRange.x = 0x800000;
    entity->updateRange.y = 0x800000;
    RSDK.SetSpriteAnimation(Rhinobot->aniFrames, 0, &entity->animatorBot, true, 0);
    RSDK.SetSpriteAnimation(Rhinobot->aniFrames, 2, &entity->animatorDust, true, 0);
    entity->state = Rhinobot_State1;
}

void Rhinobot_StageLoad(void)
{
    if (RSDK.CheckStageFolder("AIZ"))
        Rhinobot->aniFrames = RSDK.LoadSpriteAnimation("AIZ/Rhinobot.bin", SCOPE_STAGE);
    Rhinobot->hitbox.left   = -14;
    Rhinobot->hitbox.top    = -9;
    Rhinobot->hitbox.right  = 23;
    Rhinobot->hitbox.bottom = 15;
    Rhinobot->sfxHuff       = RSDK.GetSFX("Stage/Huff.wav");
    DEBUGMODE_ADD_OBJ(Rhinobot);
}

bool32 Rhinobot_CheckTileCollisions(void)
{
    RSDK_THIS(Rhinobot);
    if (entity->field_6C) {
        if (!RSDK.ObjectTileGrip(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0x40000, 0xF0000, 8)) {
            if (RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_LWALL, 0, 0x100000, 0, false))
                return false;
            if (RSDK.ObjectTileGrip(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0xF0000, 8))
                return false;
            entity->state = Rhinobot_HandleUpdate4;
        }
    }
    else if (!RSDK.ObjectTileGrip(entity, Zone->fgLayers, CMODE_FLOOR, 0, -0x40000, 0xF0000, 8)) {
        if (RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_RWALL, 0, -0x100000, 0, false))
            return false;
        if (RSDK.ObjectTileGrip(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0xF0000, 8))
            return false;
        entity->state = Rhinobot_HandleUpdate4;
    }
    return true;
}

void Rhinobot_CheckHit(void)
{
    RSDK_THIS(Rhinobot);
    foreach_active(Player, player)
    {
        if (Player_CheckBadnikTouch(player, entity, &Rhinobot->hitbox))
            Player_CheckBadnikBreak(entity, player, true);
    }
}

void Rhinobot_CheckOnScreen(void)
{
    RSDK_THIS(Rhinobot);
    if (!RSDK.CheckOnScreen(RSDK_sceneInfo->entity, 0) && !RSDK.CheckPosOnScreen(&entity->startPos, &entity->updateRange)) {
        entity->position  = entity->startPos;
        entity->direction = entity->startDir;
        Rhinobot_Create(NULL);
    }
}

void Rhinobot_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(Rhinobot->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, 0, false);
}

void Rhinobot_DebugSpawn(void)
{
    RSDK.CreateEntity(Rhinobot->objectID, NULL, RSDK_sceneInfo->entity->position.x, RSDK_sceneInfo->entity->position.y);
}

void Rhinobot_State4(void)
{
    RSDK_THIS(Rhinobot);
    entity->flag = false;
    if (!entity->field_6C)
        entity->velocity.x = -0x40000;
    else
        entity->velocity.x = 0x40000;
    entity->state      = Rhinobot_HandleUpdate2;
    entity->timedState = Rhinobot_StateSkidding;
}

void Rhinobot_StateSkidding(void)
{
    RSDK_THIS(Rhinobot);
    RSDK.SetSpriteAnimation(Rhinobot->aniFrames, 1, &entity->animatorBot, true, 0);
    entity->field_68 ^= 1;
    entity->field_74   = -entity->field_74;
    entity->timedState = Rhinobot_StateFinishSkid;
    entity->field_78   = -entity->field_78;
}

void Rhinobot_StateFinishSkid(void)
{
    RSDK_THIS(Rhinobot);
    entity->field_6C ^= 1;
    entity->direction ^= 1;
    entity->flag = false;
    RSDK.SetSpriteAnimation(Rhinobot->aniFrames, 0, &entity->animatorBot, true, 0);
    entity->timedState = Rhinobot_StateSkidding;
}

void Rhinobot_State1(void)
{
    RSDK_THIS(Rhinobot);

    entity->active     = ACTIVE_NORMAL;
    entity->timedState = Rhinobot_StateSkidding;
    entity->field_6C   = entity->startDir;
    entity->field_68   = entity->startDir;
    if (entity->direction == FLIP_NONE) {
        entity->field_74 = -0x1000;
        entity->field_78 = -0x30000;
    }
    else {
        entity->field_74 = 0x1000;
        entity->field_78 = 0x30000;
    }
    entity->state = Rhinobot_State2;
    Rhinobot_State2();
}

void Rhinobot_State2(void)
{
    RSDK_THIS(Rhinobot);
    EntityPlayer *player = (EntityPlayer *)Player_GetNearestPlayer();

    bool32 flag = false;
    if (abs(player->position.y - entity->position.y) <= 0x200000) {
        if (abs(player->position.x - entity->position.x) <= 0x600000) {
            if (entity->field_68 == entity->field_6C && (player->position.x > entity->position.x) == entity->field_68) {
                entity->state      = Rhinobot_HandleUpdate3;
                entity->velocity.x = 0;
                entity->timer      = 32;
                entity->timedState = Rhinobot_State4;
                entity->flag       = true;
                if (entity->activeScreens)
                    RSDK.PlaySfx(Rhinobot->sfxHuff, 0, 255);
                flag = true;
            }
        }
    }
    
    if (!flag) {
        if (Rhinobot_CheckTileCollisions()) {
            if (entity->state == Rhinobot_State2) {
                entity->velocity.x = entity->velocity.x + entity->field_74;
                entity->position.x += entity->velocity.x;

                if (!entity->velocity.x || (entity->field_68 && entity->velocity.x >= entity->field_78)
                    || (!entity->field_68 && entity->velocity.x <= entity->field_78)) {
                    entity->timedState();
                }
            }
        }
        else {
            entity->velocity.x = 0;
            entity->field_68   = entity->field_6C ^ 1;
            entity->field_6C ^= 1;
            entity->direction ^= 1;
            entity->flag = false;
            RSDK.SetSpriteAnimation(Rhinobot->aniFrames, 0, &entity->animatorBot, true, 0);
            entity->timedState = Rhinobot_StateSkidding;
        }

        if (((entity->field_6C && !entity->field_68 && entity->velocity.x < 0x28000)
            || (!entity->field_6C && entity->field_68 && entity->velocity.x > -0x28000)) && !entity->flag) {
            entity->flag = true;
            if (entity->activeScreens)
                RSDK.PlaySfx(Rhinobot->sfxHuff, 0, 255);
        }
    }

    RSDK.ProcessAnimation(&entity->animatorBot);
    RSDK.ProcessAnimation(&entity->animatorDust);
    Rhinobot_CheckHit();
    Rhinobot_CheckOnScreen();
}

void Rhinobot_HandleUpdate2(void)
{
    RSDK_THIS(Rhinobot);
    entity->position.x += entity->velocity.x;
    if (!Rhinobot_CheckTileCollisions()) {
        entity->velocity.x = 0;
        entity->field_68   = entity->field_6C ^ 1;
        entity->field_6C ^= 1;
        entity->direction ^= 1;
        entity->flag = false;
        RSDK.SetSpriteAnimation(Rhinobot->aniFrames, 0, &entity->animatorBot, true, 0);
        entity->timedState = Rhinobot_StateSkidding;
        entity->state      = Rhinobot_State2;
    }
    RSDK.ProcessAnimation(&entity->animatorBot);
    RSDK.ProcessAnimation(&entity->animatorDust);
    Rhinobot_CheckHit();
    Rhinobot_CheckOnScreen();
}

void Rhinobot_HandleUpdate3(void)
{
    RSDK_THIS(Rhinobot);
    RSDK.ProcessAnimation(&entity->animatorDust);
    if (--entity->timer <= 0)
        entity->timedState();
    Rhinobot_CheckHit();
    Rhinobot_CheckOnScreen();
}

void Rhinobot_HandleUpdate4(void)
{
    RSDK_THIS(Rhinobot);
    RSDK.ProcessAnimation(&entity->animatorDust);
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    entity->velocity.y += 0x3800;
    if (RSDK.ObjectTileGrip(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x100000, 8)) {
        entity->velocity.y = 0;
        RSDK.SetSpriteAnimation(Rhinobot->aniFrames, 0, &entity->animatorBot, true, 0);
        entity->flag = true;
        if (entity->activeScreens)
            RSDK.PlaySfx(Rhinobot->sfxHuff, 0, 255);
        entity->state = Rhinobot_State2;
        Rhinobot_State2();
    }
    else {
        Rhinobot_CheckHit();
        Rhinobot_CheckOnScreen();
    }
}

void Rhinobot_EditorDraw(void) {}

void Rhinobot_EditorLoad(void) {}

void Rhinobot_Serialize(void) {}
#endif
