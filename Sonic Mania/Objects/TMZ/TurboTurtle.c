#include "SonicMania.h"

ObjectTurboTurtle *TurboTurtle;

void TurboTurtle_Update(void)
{
    RSDK_THIS(TurboTurtle);

    RSDK.ProcessAnimation(&entity->animator);
    if (entity->animator.animationID == 1) {
        if (!entity->animator.frameID && entity->animator.animationTimer == 1)
            RSDK.PlaySfx(TurboTurtle->sfxWalk, false, 255);
        if (entity->animator.frameID == 5 && entity->animator.animationTimer == 1)
            RSDK.PlaySfx(TurboTurtle->sfxWalk2, false, 255);
    }

    entity->fanActiveTop   = false;
    entity->leftFanActive  = false;
    entity->rightFanActive = false;

    StateMachine_Run(entity->state);

    TurboTurtle_CheckPlayerCollisions();
    if (entity->state != TurboTurtle_State_Setup) {
        if (!RSDK.CheckOnScreen(entity, NULL) && !RSDK.CheckPosOnScreen(&entity->startPos, &entity->updateRange)) {
            entity->direction  = entity->startDir;
            entity->position.x = entity->startPos.x;
            entity->position.y = entity->startPos.y;
            TurboTurtle_Create(NULL);
        }
    }
}

void TurboTurtle_LateUpdate(void) {}

void TurboTurtle_StaticUpdate(void) {}

void TurboTurtle_Draw(void)
{
    RSDK_THIS(TurboTurtle);
    RSDK.DrawSprite(&entity->animator, NULL, false);
}

void TurboTurtle_Create(void *data)
{
    RSDK_THIS(TurboTurtle);

    entity->visible       = true;
    entity->drawOrder     = Zone->drawOrderLow;
    entity->startPos      = entity->position;
    entity->startDir      = entity->direction;
    entity->drawFX        = FX_FLIP;
    entity->active        = ACTIVE_BOUNDS;
    entity->updateRange.x = 0x800000;
    entity->updateRange.y = 0x800000;

    entity->updateRange.x =
        abs(0x70000 * (entity->stepCount * (2 * (entity->initialSide == 0) - 1) + entity->startPos.x - entity->startPos.x)) + 0x800000;
    if (!entity->leftFanDuration)
        entity->leftFanDuration = 60;
    if (!entity->rightFanDuration)
        entity->rightFanDuration = 60;
    if (!entity->leftFanLength)
        entity->leftFanLength = 128;
    if (!entity->rightFanLength)
        entity->rightFanLength = 128;
    if (!entity->leftFanStrength)
        entity->leftFanStrength = 18;
    if (!entity->rightFanStrength)
        entity->rightFanStrength = 18;

    entity->hitbox1.left   = -24;
    entity->hitbox1.top    = -19;
    entity->hitbox1.right  = 24;
    entity->hitbox1.bottom = -19;

    entity->hitbox2.left   = -19;
    entity->hitbox2.top    = -24;
    entity->hitbox2.right  = 19;
    entity->hitbox2.bottom = 24;

    entity->hitbox3.left   = -20;
    entity->hitbox3.top    = -24;
    entity->hitbox3.right  = -20;
    entity->hitbox3.bottom = 24;
    entity->state          = TurboTurtle_State_Setup;
}

void TurboTurtle_StageLoad(void)
{
    TurboTurtle->aniFrames     = RSDK.LoadSpriteAnimation("TMZ1/TurboTurtle.bin", SCOPE_STAGE);
    TurboTurtle->hitbox.left   = -13;
    TurboTurtle->hitbox.top    = -12;
    TurboTurtle->hitbox.right  = 13;
    TurboTurtle->hitbox.bottom = 16;

    DEBUGMODE_ADD_OBJ(TurboTurtle);

    TurboTurtle->sfxWalk  = RSDK.GetSFX("TMZ1/TurtleWalk.wav");
    TurboTurtle->sfxWalk2 = RSDK.GetSFX("TMZ1/TurtleWalk2.wav");

    Soundboard_LoadSFX("HCZ/SmallFan.wav", 37404, TurboTurtle_FanCheckCB, NULL);
}

void TurboTurtle_DebugSpawn(void)
{
    RSDK_THIS(DebugMode);
    EntityTurboTurtle *turboTurtle = CREATE_ENTITY(TurboTurtle, NULL, entity->position.x, entity->position.y);
    turboTurtle->direction         = entity->direction;
    turboTurtle->startDir          = entity->direction;
}

void TurboTurtle_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(TurboTurtle->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
}

void TurboTurtle_CheckPlayerCollisions(void)
{
    RSDK_THIS(TurboTurtle);

    foreach_active(Player, player)
    {
        if (Player_CheckBadnikTouch(player, entity, &TurboTurtle->hitbox))
            Player_CheckBadnikBreak(entity, player, true);
    }
}

void TurboTurtle_SetupState(void)
{
    RSDK_THIS(TurboTurtle);

    int dir = 0;
    if (entity->currentSide)
        dir = entity->rightFanDir;
    else
        dir = entity->leftFanDir;
    entity->timer = 0;

    switch (dir) {
        case 0:
            RSDK.SetSpriteAnimation(TurboTurtle->aniFrames, 2, &entity->animator, true, 0);
            entity->state = TurboTurtle_State1_Unknown1;
            break;
        case 1:
            RSDK.SetSpriteAnimation(TurboTurtle->aniFrames, 4, &entity->animator, true, 0);
            entity->direction = FLIP_NONE;
            entity->state     = TurboTurtle_State2_Unknown1;
            break;
        case 2:
            RSDK.SetSpriteAnimation(TurboTurtle->aniFrames, 4, &entity->animator, true, 0);
            entity->direction = FLIP_X;
            entity->state     = TurboTurtle_State3_Unknown1;
            break;
    }
}

void TurboTurtle_HandleFans(void)
{
    RSDK_THIS(TurboTurtle);

    int storeDir      = entity->direction;
    entity->direction = FLIP_NONE;

    int strength = 0;
    if (entity->currentSide)
        strength = entity->rightFanStrength;
    else
        strength = entity->leftFanStrength;

    int length = 0;
    if (entity->currentSide)
        length = entity->rightFanLength;
    else
        length = entity->leftFanLength;
    if (entity->fanActiveTop) {
        length += RSDK.Sin256(2 * Zone->timer) >> 5;
        entity->hitbox1.top = entity->hitbox1.bottom - length;

        foreach_active(Player, player)
        {
            if (Player_CheckCollisionTouch(player, entity, &entity->hitbox1)) {
                int anim = player->playerAnimator.animationID;
                if (anim != ANI_CLING && anim != ANI_SHAFTSWING) {
                    if (!player->onGround) {
                        player->velocity.y -= player->gravityStrength;
                        if (player->velocity.y > 0)
                            player->velocity.y -= (player->velocity.y >> 3);
                    }
                    int thisY  = entity->position.y;
                    int bottom = thisY + (entity->hitbox1.bottom << 16);
                    int top    = (entity->hitbox1.top << 16) + thisY;
                    if (player->position.y > top)
                        top = player->position.y;
                    player->position.y -= ((strength * (((length << 16) - bottom + top != 0 ? (length << 16) - bottom + top : 0) / length)) >> 1);
                }
            }
        }
        TurboTurtle_HandleFanDebris(0, strength, length << 16);
    }

    if (entity->leftFanActive) {
        entity->hitbox3.left = entity->hitbox3.right - length;

        foreach_active(Player, player)
        {
            if (Player_CheckCollisionTouch(player, entity, &entity->hitbox3)) {
                int anim = player->playerAnimator.animationID;
                if (anim != ANI_CLING && anim != ANI_SHAFTSWING) {
                    int left = (entity->hitbox3.left << 16) + entity->position.x;
                    if (player->position.x > left)
                        left = player->position.x;
                    int pos = (length << 16) - (entity->position.x + (entity->hitbox3.right << 16)) + left;
                    player->position.x -= (strength * ((pos & -(pos != 0)) / length)) >> 1;
                }
            }
        }
        TurboTurtle_HandleFanDebris(2, strength, length << 16);
    }

    if (entity->rightFanActive) {
        entity->hitbox2.right = length + entity->hitbox2.left;

        foreach_active(Player, player)
        {
            if (Player_CheckCollisionTouch(player, entity, &entity->hitbox3)) {
                int anim = player->playerAnimator.animationID;
                if (anim != ANI_CLING && anim != ANI_SHAFTSWING) {
                    int right = (entity->hitbox2.right << 16) + entity->position.x;
                    if (player->position.x < right)
                        right = player->position.x;
                    int pos = entity->position.x + (entity->hitbox2.left << 16);
                    player->position.x += (strength * (((length << 16) - right + (pos != 0) ? (length << 16) - right + pos : 0) / length)) >> 1;
                }
            }
        }
        TurboTurtle_HandleFanDebris(1, strength, length << 16);
    }

    entity->direction = storeDir;
}

void TurboTurtle_HandleFanDebris(uint8 type, int strength, int length)
{
    RSDK_THIS(TurboTurtle);

    if (!(Zone->timer % 3)) {
        for (int i = 0; i < RSDK.Rand(1, 2); ++i) {
            int timer = 0;
            int anim  = (RSDK.Rand(0, 10) > 7) + 6;
            int frame = RSDK.Rand(0, 3);

            int spawnX = 0, spawnY = 0;
            int velX = 0, velY = 0;
            switch (type) {
                case 0:
                    spawnY = entity->position.y + (entity->hitbox1.bottom << 16);
                    spawnX = (RSDK.Rand(-13, 13) << 16) + entity->position.x;
                    velX   = (spawnX - entity->position.x) >> 7;
                    RSDK.Rand(0, 4);
                    velY  = -0x8000 * strength;
                    timer = length / abs(-0x8000 * strength);
                    break;
                case 1:
                    spawnX = entity->position.x + (entity->hitbox2.left << 16);
                    spawnY = (RSDK.Rand(-13, 13) << 16) + entity->position.y;
                    RSDK.Rand(0, 4);
                    velX  = strength << 15;
                    velY  = (spawnY - entity->position.y) >> 7;
                    timer = length / abs(strength << 15);
                    break;
                case 2:
                    spawnX = entity->position.x - (entity->hitbox2.left << 16);
                    spawnY = (RSDK.Rand(-13, 13) << 16) + entity->position.y;
                    RSDK.Rand(0, 4);
                    velX  = -0x8000 * strength;
                    velY  = (spawnY - entity->position.y) >> 7;
                    timer = length / abs(-0x8000 * strength);
                    break;
                default: break;
            }

            timer += RSDK.Rand(-5, 5);
            EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_Move, spawnX, spawnY);
            RSDK.SetSpriteAnimation(TurboTurtle->aniFrames, anim, &debris->animator, true, frame);
            debris->velocity.x = velX;
            debris->velocity.y = velY;
            debris->drawOrder  = Zone->drawOrderLow - 1;
            debris->timer      = timer;
        }
    }
}

bool32 TurboTurtle_FanCheckCB(void)
{
    int count = 0;
    foreach_all(TurboTurtle, turtle)
    {
        if (turtle->state == TurboTurtle_State1_Unknown1 || turtle->state == TurboTurtle_State2_Unknown2
            || turtle->state == TurboTurtle_State3_Unknown2)
            ++count;
    }

    return count > 0;
}

void TurboTurtle_State_Setup(void)
{
    RSDK_THIS(TurboTurtle);

    entity->active = ACTIVE_NORMAL;
    RSDK.SetSpriteAnimation(TurboTurtle->aniFrames, 0, &entity->animator, true, 0);
    entity->direction   = entity->startDir;
    entity->position    = entity->startPos;
    entity->currentSide = entity->initialSide;
    TurboTurtle_SetupState();
}

void TurboTurtle_State_Unknown1(void)
{
    RSDK_THIS(TurboTurtle);
    int offsets[] = { 0, 0, 0, 0, 1, 1, 0, 0 };

    entity->position.x += (2 * (entity->currentSide == 1) - 1) * (offsets[entity->animator.frameID] << 16);
    if (!RSDK.ObjectTileGrip(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x100000, 2)) {
        entity->stepTimer = 0;
        TurboTurtle_SetupState();
    }
    else if (entity->animator.frameID == entity->animator.frameCount - 1) {
        if (entity->animator.animationTimer == entity->animator.frameDelay) {
            if (entity->stepTimer < entity->stepCount) {
                entity->stepTimer++;
            }
            else {
                entity->stepTimer = 0;
                TurboTurtle_SetupState();
            }
        }
    }
}

void TurboTurtle_State_Unknown2(void)
{
    RSDK_THIS(TurboTurtle);

    if (entity->animator.animationID == 5) {
        if (entity->animator.frameID == entity->animator.frameCount - 1) {
            uint8 dir           = entity->currentSide != 0;
            entity->currentSide = !entity->currentSide;
            entity->direction   = dir;
            entity->stepTimer   = 0;
            RSDK.SetSpriteAnimation(TurboTurtle->aniFrames, 1, &entity->animator, true, 0);
            entity->state = TurboTurtle_State_Unknown1;
        }
    }
}

void TurboTurtle_State1_Unknown1(void)
{
    RSDK_THIS(TurboTurtle);

    int duration = 0;
    if (entity->currentSide)
        duration = entity->rightFanDuration;
    else
        duration = entity->leftFanDuration;
    if (entity->stationary || entity->timer < duration) {
        entity->fanActiveTop = true;
        TurboTurtle_HandleFans();
        if (!entity->stationary)
            ++entity->timer;
    }
    else {
        uint8 dir           = entity->currentSide != 0;
        entity->currentSide = !entity->currentSide;
        entity->direction   = dir;
        entity->stepTimer   = 0;
        RSDK.SetSpriteAnimation(TurboTurtle->aniFrames, 1, &entity->animator, true, 0);
        entity->state = TurboTurtle_State_Unknown1;
    }
}

void TurboTurtle_State2_Unknown1(void)
{
    RSDK_THIS(TurboTurtle);

    if (entity->animator.animationID == 4) {
        if (entity->animator.frameID == entity->animator.frameCount - 1) {
            RSDK.SetSpriteAnimation(TurboTurtle->aniFrames, 3, &entity->animator, true, 0);
            entity->state = TurboTurtle_State2_Unknown2;
        }
    }
}

void TurboTurtle_State2_Unknown2(void)
{
    RSDK_THIS(TurboTurtle);

    int duration = 0;
    if (entity->currentSide)
        duration = entity->rightFanDuration;
    else
        duration = entity->leftFanDuration;
    if (entity->stationary || entity->timer < duration) {
        entity->rightFanActive = true;
        TurboTurtle_HandleFans();
        if (!entity->stationary)
            ++entity->timer;
    }
    else {
        RSDK.SetSpriteAnimation(TurboTurtle->aniFrames, 5, &entity->animator, true, 0);
        entity->state = TurboTurtle_State_Unknown2;
    }
}

void TurboTurtle_State3_Unknown1(void)
{
    RSDK_THIS(TurboTurtle);

    if (entity->animator.animationID == 4) {
        if (entity->animator.frameID == entity->animator.frameCount - 1) {
            RSDK.SetSpriteAnimation(TurboTurtle->aniFrames, 3, &entity->animator, true, 0);
            entity->state = TurboTurtle_State3_Unknown2;
        }
    }
}

void TurboTurtle_State3_Unknown2(void)
{
    RSDK_THIS(TurboTurtle);

    int duration = 0;
    if (entity->currentSide)
        duration = entity->rightFanDuration;
    else
        duration = entity->leftFanDuration;
    if (entity->stationary || entity->timer < duration) {
        entity->leftFanActive = true;
        TurboTurtle_HandleFans();
        if (!entity->stationary)
            ++entity->timer;
    }
    else {
        RSDK.SetSpriteAnimation(TurboTurtle->aniFrames, 5, &entity->animator, true, 0);
        entity->state = TurboTurtle_State_Unknown2;
    }
}

#if RETRO_INCLUDE_EDITOR
void TurboTurtle_EditorDraw(void)
{
    RSDK_THIS(TurboTurtle);

    entity->visible       = true;
    entity->drawOrder     = Zone->drawOrderLow;
    entity->startPos      = entity->position;
    entity->startDir      = entity->direction;

    entity->updateRange.x =
        abs(0x70000 * (entity->stepCount * (2 * (entity->initialSide == 0) - 1) + entity->startPos.x - entity->startPos.x)) + 0x800000;

    RSDK.SetSpriteAnimation(TurboTurtle->aniFrames, 0, &entity->animator, true, 0);
    entity->direction   = entity->startDir;

    TurboTurtle_Draw();
}

void TurboTurtle_EditorLoad(void) { TurboTurtle->aniFrames = RSDK.LoadSpriteAnimation("TMZ1/TurboTurtle.bin", SCOPE_STAGE); }
#endif

void TurboTurtle_Serialize(void)
{
    RSDK_EDITABLE_VAR(TurboTurtle, VAR_UINT16, stepCount);
    RSDK_EDITABLE_VAR(TurboTurtle, VAR_UINT8, initialSide);
    RSDK_EDITABLE_VAR(TurboTurtle, VAR_BOOL, stationary);
    RSDK_EDITABLE_VAR(TurboTurtle, VAR_UINT8, leftFanDir);
    RSDK_EDITABLE_VAR(TurboTurtle, VAR_UINT32, leftFanDuration);
    RSDK_EDITABLE_VAR(TurboTurtle, VAR_UINT32, leftFanStrength);
    RSDK_EDITABLE_VAR(TurboTurtle, VAR_UINT32, leftFanLength);
    RSDK_EDITABLE_VAR(TurboTurtle, VAR_UINT8, rightFanDir);
    RSDK_EDITABLE_VAR(TurboTurtle, VAR_UINT32, rightFanDuration);
    RSDK_EDITABLE_VAR(TurboTurtle, VAR_UINT32, rightFanStrength);
    RSDK_EDITABLE_VAR(TurboTurtle, VAR_UINT32, rightFanLength);
}
