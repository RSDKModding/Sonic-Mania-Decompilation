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
    if (entity->playedHuff) {
        Vector2 drawPos;
        drawPos.x = entity->position.x;
        drawPos.y = entity->position.y;
        if (entity->state == Rhinobot_State_HandleGroundMovement) {
            if (!entity->moveDir)
                drawPos.x -= 0xD0000;
            else
                drawPos.x += 0xD0000;
        }
        RSDK.DrawSprite(&entity->animatorDust, &drawPos, false);
    }
    RSDK.DrawSprite(&entity->animatorBot, NULL, false);
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
    entity->state = Rhinobot_State_Setup;
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
    if (entity->moveDir) {
        if (!RSDK.ObjectTileGrip(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0x40000, 0xF0000, 8)) {
            if (RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_LWALL, 0, 0x100000, 0, false))
                return false;
            if (RSDK.ObjectTileGrip(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0xF0000, 8))
                return false;
            entity->state = Rhinobot_State_Fall;
        }
    }
    else if (!RSDK.ObjectTileGrip(entity, Zone->fgLayers, CMODE_FLOOR, 0, -0x40000, 0xF0000, 8)) {
        if (RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_RWALL, 0, -0x100000, 0, false))
            return false;
        if (RSDK.ObjectTileGrip(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0xF0000, 8))
            return false;
        entity->state = Rhinobot_State_Fall;
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
    if (!RSDK.CheckOnScreen(entity, NULL) && !RSDK.CheckPosOnScreen(&entity->startPos, &entity->updateRange)) {
        entity->position  = entity->startPos;
        entity->direction = entity->startDir;
        Rhinobot_Create(NULL);
    }
}

void Rhinobot_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(Rhinobot->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
}

void Rhinobot_DebugSpawn(void)
{
    RSDK_THIS(DebugMode);

    CREATE_ENTITY(Rhinobot, NULL, entity->position.x, entity->position.y);
}

void Rhinobot_StateDelay_StartMove(void)
{
    RSDK_THIS(Rhinobot);
    entity->playedHuff = false;
    if (!entity->moveDir)
        entity->velocity.x = -0x40000;
    else
        entity->velocity.x = 0x40000;
    entity->state      = Rhinobot_State_Skidding;
    entity->stateDelay = Rhinobot_StateDelay_Skidding;
}

void Rhinobot_StateDelay_Skidding(void)
{
    RSDK_THIS(Rhinobot);
    RSDK.SetSpriteAnimation(Rhinobot->aniFrames, 1, &entity->animatorBot, true, 0);
    entity->skidDir ^= FLIP_X;
    entity->acceleration = -entity->acceleration;
    entity->stateDelay   = Rhinobot_StateDelay_SkidFinish;
    entity->topSpeed     = -entity->topSpeed;
}

void Rhinobot_StateDelay_SkidFinish(void)
{
    RSDK_THIS(Rhinobot);
    entity->moveDir ^= FLIP_X;
    entity->direction ^= FLIP_X;
    entity->playedHuff = false;
    RSDK.SetSpriteAnimation(Rhinobot->aniFrames, 0, &entity->animatorBot, true, 0);
    entity->stateDelay = Rhinobot_StateDelay_Skidding;
}

void Rhinobot_State_Setup(void)
{
    RSDK_THIS(Rhinobot);

    entity->active     = ACTIVE_NORMAL;
    entity->stateDelay = Rhinobot_StateDelay_Skidding;
    entity->moveDir    = entity->startDir;
    entity->skidDir    = entity->startDir;
    if (entity->direction == FLIP_NONE) {
        entity->acceleration = -0x1000;
        entity->topSpeed     = -0x30000;
    }
    else {
        entity->acceleration = 0x1000;
        entity->topSpeed     = 0x30000;
    }
    entity->state = Rhinobot_State_HandleGroundMovement;
    Rhinobot_State_HandleGroundMovement();
}

void Rhinobot_State_HandleGroundMovement(void)
{
    RSDK_THIS(Rhinobot);
    EntityPlayer *player = (EntityPlayer *)Player_GetNearestPlayer();

    bool32 flag = false;
    if (abs(player->position.y - entity->position.y) <= 0x200000) {
        if (abs(player->position.x - entity->position.x) <= 0x600000) {
            if (entity->skidDir == entity->moveDir && (player->position.x > entity->position.x) == entity->skidDir) {
                entity->state      = Rhinobot_State_Idle;
                entity->velocity.x = 0;
                entity->timer      = 32;
                entity->stateDelay = Rhinobot_StateDelay_StartMove;
                entity->playedHuff = true;
                if (entity->activeScreens)
                    RSDK.PlaySfx(Rhinobot->sfxHuff, false, 255);
                flag = true;
            }
        }
    }

    if (!flag) {
        if (Rhinobot_CheckTileCollisions()) {
            if (entity->state == Rhinobot_State_HandleGroundMovement) {
                entity->velocity.x += entity->acceleration;
                entity->position.x += entity->velocity.x;

                if (!entity->velocity.x || (entity->skidDir && entity->velocity.x >= entity->topSpeed)
                    || (!entity->skidDir && entity->velocity.x <= entity->topSpeed)) {
                    entity->stateDelay();
                }
            }
        }
        else {
            entity->velocity.x = 0;
            entity->skidDir    = entity->moveDir ^ 1;
            entity->moveDir ^= FLIP_X;
            entity->direction ^= FLIP_X;
            entity->playedHuff = false;
            RSDK.SetSpriteAnimation(Rhinobot->aniFrames, 0, &entity->animatorBot, true, 0);
            entity->stateDelay = Rhinobot_StateDelay_Skidding;
        }

        if (((entity->moveDir && !entity->skidDir && entity->velocity.x < 0x28000)
             || (!entity->moveDir && entity->skidDir && entity->velocity.x > -0x28000))
            && !entity->playedHuff) {
            entity->playedHuff = true;
            if (entity->activeScreens)
                RSDK.PlaySfx(Rhinobot->sfxHuff, false, 255);
        }
    }

    RSDK.ProcessAnimation(&entity->animatorBot);
    RSDK.ProcessAnimation(&entity->animatorDust);
    Rhinobot_CheckHit();
    Rhinobot_CheckOnScreen();
}

void Rhinobot_State_Skidding(void)
{
    RSDK_THIS(Rhinobot);
    entity->position.x += entity->velocity.x;
    if (!Rhinobot_CheckTileCollisions()) {
        entity->velocity.x = 0;
        entity->skidDir    = entity->moveDir ^ FLIP_X;
        entity->moveDir ^= FLIP_X;
        entity->direction ^= FLIP_X;
        entity->playedHuff = false;
        RSDK.SetSpriteAnimation(Rhinobot->aniFrames, 0, &entity->animatorBot, true, 0);
        entity->stateDelay = Rhinobot_StateDelay_Skidding;
        entity->state      = Rhinobot_State_HandleGroundMovement;
    }
    RSDK.ProcessAnimation(&entity->animatorBot);
    RSDK.ProcessAnimation(&entity->animatorDust);
    Rhinobot_CheckHit();
    Rhinobot_CheckOnScreen();
}

void Rhinobot_State_Idle(void)
{
    RSDK_THIS(Rhinobot);
    RSDK.ProcessAnimation(&entity->animatorDust);
    if (--entity->timer <= 0)
        entity->stateDelay();
    Rhinobot_CheckHit();
    Rhinobot_CheckOnScreen();
}

void Rhinobot_State_Fall(void)
{
    RSDK_THIS(Rhinobot);
    RSDK.ProcessAnimation(&entity->animatorDust);
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    entity->velocity.y += 0x3800;
    if (RSDK.ObjectTileGrip(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x100000, 8)) {
        entity->velocity.y = 0;
        RSDK.SetSpriteAnimation(Rhinobot->aniFrames, 0, &entity->animatorBot, true, 0);
        entity->playedHuff = true;
        if (entity->activeScreens)
            RSDK.PlaySfx(Rhinobot->sfxHuff, false, 255);
        entity->state = Rhinobot_State_HandleGroundMovement;
        Rhinobot_State_HandleGroundMovement();
    }
    else {
        Rhinobot_CheckHit();
        Rhinobot_CheckOnScreen();
    }
}

#if RETRO_INCLUDE_EDITOR
void Rhinobot_EditorDraw(void) { Rhinobot_Draw(); }

void Rhinobot_EditorLoad(void) { Rhinobot->aniFrames = RSDK.LoadSpriteAnimation("AIZ/Rhinobot.bin", SCOPE_STAGE); }
#endif

void Rhinobot_Serialize(void) {}
#endif
