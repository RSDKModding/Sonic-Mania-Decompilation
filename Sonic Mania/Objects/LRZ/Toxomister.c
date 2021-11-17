#include "SonicMania.h"

ObjectToxomister *Toxomister;

void Toxomister_Update(void)
{
    RSDK_THIS(Toxomister);
    StateMachine_Run(entity->state);
}

void Toxomister_LateUpdate(void) {}

void Toxomister_StaticUpdate(void) {}

void Toxomister_Draw(void)
{
    RSDK_THIS(Toxomister);

    if (entity->state == Toxomister_State_Unknown1) {
        entity->animator.frameID = 1;
        RSDK.DrawSprite(&entity->animator, NULL, false);

        entity->animator.frameID = 0;
    }
    RSDK.DrawSprite(&entity->animator, NULL, false);
}

void Toxomister_Create(void *data)
{
    RSDK_THIS(Toxomister);

    entity->drawFX |= FX_FLIP;
    if (!RSDK_sceneInfo->inEditor) {
        entity->startDir      = entity->direction;
        entity->startPos.x    = entity->position.x;
        entity->startPos.y    = entity->position.y;
        entity->visible       = true;
        entity->active        = ACTIVE_BOUNDS;
        entity->updateRange.x = 0x1000000;
        entity->updateRange.y = 0x1000000;
        if (data) {
            entity->active    = ACTIVE_NORMAL;
            entity->drawOrder = Zone->playerDrawLow + 1;
            entity->inkEffect = INK_ADD;
            entity->alpha     = 0xE0;
            RSDK.SetSpriteAnimation(Toxomister->aniFrames, 2, &entity->animator, true, 0);
            entity->state = Toxomister_State1_Unknown1;
        }
        else {
            entity->drawOrder = Zone->drawOrderLow;
            entity->direction *= FLIP_Y;
            RSDK.SetSpriteAnimation(Toxomister->aniFrames, 0, &entity->animator, true, 0);
            entity->state = Toxomister_State_Setup;
        }
    }
}

void Toxomister_StageLoad(void)
{
    if (RSDK.CheckStageFolder("LRZ1"))
        Toxomister->aniFrames = RSDK.LoadSpriteAnimation("LRZ1/Toxomister.bin", SCOPE_STAGE);
    else if (RSDK.CheckStageFolder("LRZ2") || RSDK.CheckStageFolder("LRZ3"))
        Toxomister->aniFrames = RSDK.LoadSpriteAnimation("LRZ2/Toxomister.bin", SCOPE_STAGE);

    Toxomister->hitbox1.left   = -5;
    Toxomister->hitbox1.top    = -24;
    Toxomister->hitbox1.right  = 5;
    Toxomister->hitbox1.bottom = 16;

    Toxomister->hitbox2.left   = -8;
    Toxomister->hitbox2.top    = -8;
    Toxomister->hitbox2.right  = 8;
    Toxomister->hitbox2.bottom = 8;
    DEBUGMODE_ADD_OBJ(Toxomister);
}

void Toxomister_DebugSpawn(void)
{
    RSDK_THIS(DebugMode);

    CREATE_ENTITY(Toxomister, NULL, entity->position.x, entity->position.y);
}

void Toxomister_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(Toxomister->aniFrames, 0, &DebugMode->animator, true, 0);
    DebugMode->animator.frameID = 1;
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);

    DebugMode->animator.frameID = 0;
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
}

void Toxomister_CheckPlayerCollisions(void)
{
    RSDK_THIS(Toxomister);

    foreach_active(Player, player)
    {
        if (Player_CheckBadnikTouch(player, entity, &Toxomister->hitbox1)) {
            if (Player_CheckBadnikBreak(entity, player, true)) {
                if (entity->link)
                    destroyEntity(entity->link);
            }
        }
    }
}

void Toxomister_CheckOnScreen(void)
{
    RSDK_THIS(Toxomister);

    if (!RSDK.CheckOnScreen(entity, NULL) && !RSDK.CheckPosOnScreen(&entity->startPos, &entity->updateRange)) {
        if (entity->link)
            destroyEntity(entity->link);
        entity->link       = NULL;
        entity->position.x = entity->startPos.x;
        entity->position.y = entity->startPos.y;
        entity->direction  = entity->startDir;
        Toxomister_Create(NULL);
    }
}

void Toxomister_CheckMistOnScreen(void)
{
    RSDK_THIS(Toxomister);
    if (!RSDK.CheckOnScreen(RSDK_sceneInfo->entity, NULL)) {
        ((EntityToxomister *)entity->link)->link = NULL;
        destroyEntity(entity);
    }
}

void Toxomister_CheckPlayerMistCollisions(void)
{
    RSDK_THIS(Toxomister);

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, entity, &Toxomister->hitbox2)) {
            entity->grabbedPlayer = player;
            entity->drawOrder     = player->drawOrder + 1;
            entity->state         = Toxomister_State1_Unknown4;
            foreach_break;
        }
    }
}

void Toxomister_State_Setup(void)
{
    RSDK_THIS(Toxomister);

    entity->active = ACTIVE_NORMAL;
    entity->state  = Toxomister_State_Unknown1;
    Toxomister_State_Unknown1();
}

void Toxomister_State_Unknown1(void)
{
    RSDK_THIS(Toxomister);

    EntityPlayer *player = Player_GetNearestPlayer();

    if (player->position.x >= entity->position.x)
        entity->direction |= FLIP_X;
    else
        entity->direction &= ~FLIP_X;
    if (!entity->link && entity->activeScreens) {
        EntityToxomister *mist = CREATE_ENTITY(Toxomister, intToVoid(1), entity->position.x, entity->position.y);
        if (entity->direction & FLIP_X)
            mist->position.x += 0x120000;
        else
            mist->position.x -= 0x120000;
        if (entity->direction & FLIP_Y)
            mist->position.y += 0x100000;
        else
            mist->position.y -= 0x100000;
        mist->timer       = 32;
        mist->isPermanent = true;
        mist->link        = (Entity *)entity;
        entity->link      = (Entity *)mist;
    }
    Toxomister_CheckPlayerCollisions();
    Toxomister_CheckOnScreen();
}

void Toxomister_State1_Unknown1(void)
{
    RSDK_THIS(Toxomister);

    RSDK.ProcessAnimation(&entity->animator);
    if (--entity->timer > 0)
        entity->state = Toxomister_State1_Unknown2;
    Toxomister_CheckPlayerMistCollisions();
    Toxomister_CheckMistOnScreen();
}

void Toxomister_State1_Unknown2(void)
{
    RSDK_THIS(Toxomister);

    RSDK.ProcessAnimation(&entity->animator);

    entity->position.y += entity->velocity.y;
    if (entity->velocity.y < 0x20000)
        entity->velocity.y += 0x80;
    if (RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x180000, false))
        entity->state = Toxomister_State1_Unknown3;
    Toxomister_CheckMistOnScreen();
}

void Toxomister_State1_Unknown3(void)
{
    RSDK_THIS(Toxomister);

    RSDK.ProcessAnimation(&entity->animator);
    Toxomister_CheckPlayerMistCollisions();
    Toxomister_CheckMistOnScreen();
}

void Toxomister_State1_Unknown4(void)
{
    RSDK_THIS(Toxomister);

    RSDK.ProcessAnimation(&entity->animator);

    EntityPlayer *player = entity->grabbedPlayer;
    if (!player) {
        ((EntityToxomister *)entity->link)->link = NULL;
        RSDK.SetSpriteAnimation(Toxomister->aniFrames, 3, &entity->animator, true, 0);
        entity->state = Toxomister_State1_Unknown5;
    }
    else {
        if (++entity->timer >= 60) {
            entity->timer = 0;

            if (!player->rings || player->sidekick) {
                Player_Hit(player);
                if (player->position.x > entity->position.x)
                    player->velocity.x = 0x20000;
                else
                    player->velocity.x = -0x20000;
                entity->grabbedPlayer                    = NULL;
                ((EntityToxomister *)entity->link)->link = NULL;
                RSDK.SetSpriteAnimation(Toxomister->aniFrames, 3, &entity->animator, true, 0);
                entity->state = Toxomister_State1_Unknown5;
            }
            else {
                player->rings--;
                if (Ring->pan) {
                    int channel = RSDK.PlaySfx(Ring->sfxRing, false, 255);
                    RSDK.SetChannelAttributes(channel, 1.0, -1.0, 1.0);
                    Ring->pan = 0;
                }
                else {
                    int channel = RSDK.PlaySfx(Ring->sfxRing, false, 255);
                    RSDK.SetChannelAttributes(channel, 1.0, 1.0, 1.0);
                    Ring->pan = 1;
                }
            }
        }

        if (entity->state == Toxomister_State1_Unknown4) {
            entity->position.x = player->position.x;
            entity->position.y = player->position.y;
            if (player->onGround) {
                player->groundVel = clampVal(player->groundVel, -0x20000, 0x20000);
            }
            else {
                player->velocity.x = clampVal(player->velocity.x, -0x20000, 0x20000);

                if (player->velocity.y >= 0) {
                    if (player->velocity.y > 0x20000)
                        player->velocity.y = 0x20000;
                }
                else {
                    player->velocity.y -= (player->velocity.y >> 3);
                }
            }

            if (entity->prevShakeFlags) {
                if (!entity->shakeTimer) {
                    entity->shakeCount     = 0;
                    entity->prevShakeFlags = 0;
                }
                else {
                    entity->shakeTimer--;
                    uint8 shakeFlags = 0;
                    if (player->left)
                        shakeFlags = 1;
                    if (player->right)
                        shakeFlags |= 2;
                    if (shakeFlags) {
                        if (shakeFlags != 3 && shakeFlags != entity->prevShakeFlags) {
                            entity->prevShakeFlags = shakeFlags;
                            if (++entity->shakeCount >= 6) {
                                entity->grabbedPlayer                    = NULL;
                                ((EntityToxomister *)entity->link)->link = NULL;
                                RSDK.SetSpriteAnimation(Toxomister->aniFrames, 3, &entity->animator, true, 0);
                                entity->state = Toxomister_State1_Unknown5;
                            }
                        }
                    }
                }
            }
            else if (player->left) {
                entity->prevShakeFlags = 1;
                entity->shakeTimer     = 64;
            }
            else if (player->right) {
                entity->prevShakeFlags = 2;
                entity->shakeTimer     = 64;
            }
        }
    }
}

void Toxomister_State1_Unknown5(void)
{
    RSDK_THIS(Toxomister);

    RSDK.ProcessAnimation(&entity->animator);
    if (entity->animator.frameID >= entity->animator.frameCount - 1)
        destroyEntity(entity);
}

#if RETRO_INCLUDE_EDITOR
void Toxomister_EditorDraw(void)
{
    RSDK_THIS(Toxomister);
    int dir = entity->direction;
    entity->direction *= FLIP_Y;

    RSDK.SetSpriteAnimation(Toxomister->aniFrames, 1, &entity->animator, true, 0);
    RSDK.DrawSprite(&entity->animator, NULL, false);

    RSDK.SetSpriteAnimation(Toxomister->aniFrames, 0, &entity->animator, true, 0);
    RSDK.DrawSprite(&entity->animator, NULL, false);

    entity->direction = dir;
}

void Toxomister_EditorLoad(void)
{
    if (RSDK.CheckStageFolder("LRZ1"))
        Toxomister->aniFrames = RSDK.LoadSpriteAnimation("LRZ1/Toxomister.bin", SCOPE_STAGE);
    else if (RSDK.CheckStageFolder("LRZ2") || RSDK.CheckStageFolder("LRZ3"))
        Toxomister->aniFrames = RSDK.LoadSpriteAnimation("LRZ2/Toxomister.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(Toxomister, direction);
    RSDK_ENUM_VAR("No Flip", FLIP_NONE);
    RSDK_ENUM_VAR("Flip Y", FLIP_Y);
}
#endif

void Toxomister_Serialize(void) { RSDK_EDITABLE_VAR(Toxomister, VAR_UINT8, direction); }
