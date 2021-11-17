#include "SonicMania.h"

ObjectLaunchSpring *LaunchSpring;

void LaunchSpring_Update(void)
{
    RSDK_THIS(LaunchSpring);
    StateMachine_Run(entity->state);
}

void LaunchSpring_LateUpdate(void) {}

void LaunchSpring_StaticUpdate(void) {}

void LaunchSpring_Draw(void)
{
    RSDK_THIS(LaunchSpring);

    RSDK.GetFrame(LaunchSpring->aniFrames, 2, 0)->pivotY = -8 - entity->field_B1;
    RSDK.DrawSprite(&entity->animator3, NULL, false);

    entity->animator1.frameID = 0;
    RSDK.DrawSprite(&entity->animator2, NULL, false);

    SpriteFrame *frame = RSDK.GetFrame(LaunchSpring->aniFrames, 0, 0);
    RSDK.DrawSprite(&entity->animator1, NULL, false);

    frame->pivotY     = entity->timer - 55;
    entity->direction = FLIP_X;
    RSDK.DrawSprite(&entity->animator1, NULL, false);

    frame->pivotY     = -55;
    entity->direction = FLIP_NONE;
}

void LaunchSpring_Create(void *data)
{
    RSDK_THIS(LaunchSpring);
    if (!entity->rotSpeed)
        entity->rotSpeed = 4;

    entity->drawFX   = FX_ROTATE | FX_FLIP;
    entity->rotation = entity->angle << 6;
    if (!SceneInfo->inEditor) {
        entity->active        = ACTIVE_BOUNDS;
        entity->visible       = RSDK.CheckStageFolder("SSZ1") || RSDK.CheckStageFolder("SSZ2");
        entity->updateRange.x = 0x800000;
        entity->updateRange.y = 0x800000;
        RSDK.SetSpriteAnimation(LaunchSpring->aniFrames, 0, &entity->animator1, true, 0);
        RSDK.SetSpriteAnimation(LaunchSpring->aniFrames, 1, &entity->animator2, true, 0);
        RSDK.SetSpriteAnimation(LaunchSpring->aniFrames, 2, &entity->animator3, true, 0);
        entity->drawOrder = Zone->drawOrderHigh;

        switch (entity->type) {
            case 0:
                entity->timer = 47;
                entity->state = LaunchSpring_Unknown4;
                break;
            case 1:
                entity->state             = LaunchSpring_Unknown9;
                entity->animator3.frameID = 1;
                break;
            case 2:
                entity->field_B4          = entity->rotation - 64;
                entity->field_B6          = entity->rotation + 64;
                entity->state             = LaunchSpring_Unknown13;
                entity->animator3.frameID = 1;
                break;
            case 3:
                entity->field_B4          = entity->rotation - 128;
                entity->field_B6          = entity->rotation + 128;
                entity->state             = LaunchSpring_Unknown13;
                entity->animator3.frameID = 1;
                break;
            case 4:
                entity->field_B4          = entity->rotation - 192;
                entity->field_B6          = entity->rotation + 192;
                entity->state             = LaunchSpring_Unknown13;
                entity->animator3.frameID = 1;
                break;
            case 5:
                entity->field_B4          = entity->rotation - 256;
                entity->field_B6          = entity->rotation + 256;
                entity->state             = LaunchSpring_Unknown13;
                entity->animator3.frameID = 1;
                break;
            default: break;
        }

        LaunchSpring->sfxGrab = RSDK.GetSFX("Global/Grab.wav");
        if (RSDK.CheckStageFolder("SSZ1") || RSDK.CheckStageFolder("SSZ2")) {
            LaunchSpring->flag            = false;
            LaunchSpring->sfxClack        = RSDK.GetSFX("Stage/Clack.wav");
            LaunchSpring->sfxSpeedBooster = RSDK.GetSFX("Stage/SpeedBooster.wav");
        }
        else if (RSDK.CheckStageFolder("TMZ1") || RSDK.CheckStageFolder("TMZ2")) {
            LaunchSpring->flag            = true;
            LaunchSpring->sfxSpeedBooster = RSDK.GetSFX("Stage/CannonFire.wav");
        }
    }
}

void LaunchSpring_StageLoad(void)
{
    if (RSDK.CheckStageFolder("SSZ1"))
        LaunchSpring->aniFrames = RSDK.LoadSpriteAnimation("SSZ1/LaunchSpring.bin", SCOPE_STAGE);
    else if (RSDK.CheckStageFolder("SSZ2"))
        LaunchSpring->aniFrames = RSDK.LoadSpriteAnimation("SSZ2/LaunchSpring.bin", SCOPE_STAGE);

    LaunchSpring->hitbox.top    = -24;
    LaunchSpring->hitbox.left   = -24;
    LaunchSpring->hitbox.right  = 24;
    LaunchSpring->hitbox.bottom = 24;
}

bool32 LaunchSpring_Unknown1(void *p)
{
    if (!Firework)
        return false;

    EntityPlayer *player = (EntityPlayer *)p;
    int32 playerID = RSDK.GetEntityID(player);
    bool32 flag  = false;
    foreach_active(Firework, firework)
    {
        if ((1 << playerID) & firework->activePlayers) {
            flag = true;
            foreach_break;
        }
    }
    return flag;
}

void LaunchSpring_CheckPlayerCollisions(void *nextState)
{
    RSDK_THIS(LaunchSpring);

    foreach_active(Player, player)
    {
        if (!player->sidekick && !LaunchSpring_Unknown1(player)) {
            if (Player_CheckCollisionTouch(player, entity, &LaunchSpring->hitbox)) {
                RSDK.PlaySfx(LaunchSpring->sfxGrab, false, 255);
                RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->playerAnimator, false, 0);
                player->position.x = entity->position.x;
                player->position.y = entity->position.y;
                player->position.x += RSDK.Cos512(entity->rotation - 128) << 12;
                player->position.y += RSDK.Sin512(entity->rotation - 128) << 12;
                player->velocity.x        = 0;
                player->velocity.y        = 0;
                player->groundVel         = 0;
                player->onGround          = false;
                player->state             = Player_State_None;
                player->nextGroundState   = StateMachine_None;
                player->nextAirState      = StateMachine_None;
                entity->animator3.frameID = 1;
                entity->playerPtr         = (Entity *)player;
                entity->state             = nextState;
                foreach_break;
            }
        }
    }
}

void LaunchSpring_Unknown3(void)
{
    RSDK_THIS(LaunchSpring);
    EntityPlayer *player = (EntityPlayer *)entity->playerPtr;

    if (player->state == Player_State_None) {
        RSDK.PlaySfx(LaunchSpring->sfxSpeedBooster, false, 255);
        if (entity->strict) {
            entity->rotation   = (entity->rotation + 64) & 0x1C0;
            player->position.x = entity->position.x;
            player->position.y = entity->position.y;
            player->position.x += RSDK.Cos512(entity->rotation - 128) << 12;
            player->position.y += RSDK.Sin512(entity->rotation - 128) << 12;
        }
        player->jumpAbility      = 0;
        player->jumpAbilityTimer = 0;
        player->velocity.x       = RSDK.Cos512(entity->rotation - 128) << 11;
        player->velocity.y       = RSDK.Sin512(entity->rotation - 128) << 11;
        player->state            = Player_State_Air;
    }
}

void LaunchSpring_Unknown4(void)
{
    RSDK_THIS(LaunchSpring);

    entity->rotation = (entity->rotation - entity->rotSpeed) & 0x1FF;
    LaunchSpring_CheckPlayerCollisions(LaunchSpring_Unknown5);
}

void LaunchSpring_Unknown5(void)
{
    RSDK_THIS(LaunchSpring);
    EntityPlayer *player = (EntityPlayer *)entity->playerPtr;

    player->position.x = entity->position.x;
    player->position.y = entity->position.y;
    player->position.x += RSDK.Cos512(entity->rotation - 128) << 12;
    player->position.y += RSDK.Sin512(entity->rotation - 128) << 12;
    if (!--entity->timer) {
        RSDK.PlaySfx(LaunchSpring->sfxClack, false, 255);
        entity->state = LaunchSpring_Unknown6;
    }
}

void LaunchSpring_Unknown6(void)
{
    RSDK_THIS(LaunchSpring);
    EntityPlayer *player = (EntityPlayer *)entity->playerPtr;

    if (player->state == Player_State_None) {
        entity->rotation   = (entity->rotation - entity->rotSpeed) & 0x1FF;
        player->position.x = entity->position.x;
        player->position.y = entity->position.y;
        player->position.x += RSDK.Cos512(entity->rotation - 128) << 12;
        player->position.y += RSDK.Sin512(entity->rotation - 128) << 12;
        if (player->jumpPress) {
            LaunchSpring_Unknown3();
            entity->state = LaunchSpring_Unknown7;
        }
    }
    else {
        entity->playerPtr = NULL;
        entity->state     = LaunchSpring_Unknown7;
    }
}

void LaunchSpring_Unknown7(void)
{
    RSDK_THIS(LaunchSpring);

    RSDK.ProcessAnimation(&entity->animator3);
    if (entity->animator3.frameID == entity->animator3.frameCount - 1) {
        RSDK.SetSpriteAnimation(LaunchSpring->aniFrames, 2, &entity->animator3, true, 0);
        entity->state = LaunchSpring_Unknown8;
    }
}

void LaunchSpring_Unknown8(void)
{
    RSDK_THIS(LaunchSpring);

    entity->timer += 2;
    if (entity->timer >= 47) {
        entity->timer = 47;
        entity->state = LaunchSpring_Unknown4;
    }
}

void LaunchSpring_Unknown9(void)
{
    RSDK_THIS(LaunchSpring);

    if (entity->manual)
        LaunchSpring_CheckPlayerCollisions(LaunchSpring_Unknown11);
    else
        LaunchSpring_CheckPlayerCollisions(LaunchSpring_Unknown10);
}

void LaunchSpring_Unknown10(void)
{
    RSDK_THIS(LaunchSpring);
    EntityPlayer *player = (EntityPlayer *)entity->playerPtr;

    if (player->state == Player_State_None) {
        player->position.x = entity->position.x;
        player->position.y = entity->position.y;
        player->position.x += RSDK.Cos512(entity->rotation - 128) << 12;
        player->position.y += RSDK.Sin512(entity->rotation - 128) << 12;
        if (++entity->timer2 == 60) {
            entity->timer2 = 0;
            LaunchSpring_Unknown3();
            entity->state = LaunchSpring_Unknown12;
        }
    }
    else {
        entity->playerPtr = NULL;
        entity->state     = LaunchSpring_Unknown12;
    }
}

void LaunchSpring_Unknown11(void)
{
    RSDK_THIS(LaunchSpring);
    EntityPlayer *player = (EntityPlayer *)entity->playerPtr;

    if (player->state == Player_State_None) {
        player->position.x = entity->position.x;
        player->position.y = entity->position.y;
        player->position.x += RSDK.Cos512(entity->rotation - 128) << 12;
        player->position.y += RSDK.Sin512(entity->rotation - 128) << 12;
        if (player->jumpPress) {
            LaunchSpring_Unknown3();
            entity->timer2 = 0;
            entity->state  = LaunchSpring_Unknown12;
        }
    }
    else {
        entity->playerPtr = NULL;
        entity->state     = LaunchSpring_Unknown12;
    }
}

void LaunchSpring_Unknown12(void)
{
    RSDK_THIS(LaunchSpring);

    if (LaunchSpring->flag) {
        if (entity->timer2 >= 30) {
            entity->state = LaunchSpring_Unknown9;
        }
        else {
            entity->timer2++;
        }
    }
    else {
        RSDK.ProcessAnimation(&entity->animator3);
        if (entity->animator3.frameID == entity->animator3.frameCount - 1) {
            RSDK.SetSpriteAnimation(LaunchSpring->aniFrames, 2, &entity->animator3, true, 0);
            entity->animator3.frameID = 1;
            entity->state             = LaunchSpring_Unknown9;
        }
    }
}

void LaunchSpring_Unknown13(void)
{
    RSDK_THIS(LaunchSpring);

    if (entity->rotDir) {
        entity->rotation -= entity->rotSpeed;
        if (entity->rotation <= entity->field_B4) {
            entity->rotDir   = 0;
            entity->rotation = entity->field_B4;
        }
    }
    else {
        entity->rotation += entity->rotSpeed;
        if (entity->rotation >= entity->field_B6) {
            entity->rotDir   = 1;
            entity->rotation = entity->field_B6;
        }
    }

    if (entity->manual)
        LaunchSpring_CheckPlayerCollisions(LaunchSpring_Unknown15);
    else
        LaunchSpring_CheckPlayerCollisions(LaunchSpring_Unknown14);
}

void LaunchSpring_Unknown14(void)
{
    RSDK_THIS(LaunchSpring);
    EntityPlayer *player = (EntityPlayer *)entity->playerPtr;

    if (player->state == Player_State_None) {
        if (entity->strict)
            entity->rotation = (entity->rotation + 32) & 0xC0;
        player->position.x = entity->position.x;
        player->position.y = entity->position.y;
        player->position.x += RSDK.Cos512(entity->rotation - 128) << 12;
        player->position.y += RSDK.Sin512(entity->rotation - 128) << 12;
        if (++entity->timer2 == 60) {
            entity->timer2 = 0;
            LaunchSpring_Unknown3();
            entity->state = LaunchSpring_Unknown16;
        }
    }
    else {
        entity->playerPtr = NULL;
        entity->state     = LaunchSpring_Unknown16;
    }
}

void LaunchSpring_Unknown15(void)
{
    RSDK_THIS(LaunchSpring);
    EntityPlayer *player = (EntityPlayer *)entity->playerPtr;

    if (player->state == Player_State_None) {
        if (entity->rotDir) {
            entity->rotation -= entity->rotSpeed;
            if (entity->rotation <= entity->field_B4) {
                entity->rotation = entity->field_B4;
                entity->rotDir   = 0;
            }
        }
        else {
            entity->rotation += entity->rotSpeed;
            if (entity->rotation >= entity->field_B6) {
                entity->rotation = entity->field_B6;
                entity->rotDir   = 1;
            }
        }
        player->position.x = entity->position.x;
        player->position.y = entity->position.y;
        player->position.x += RSDK.Cos512(entity->rotation - 128) << 12;
        player->position.y += RSDK.Sin512(entity->rotation - 128) << 12;
        if (player->jumpPress) {
            LaunchSpring_Unknown3();
            entity->state = LaunchSpring_Unknown16;
        }
    }
    else {
        entity->playerPtr = NULL;
        entity->state     = LaunchSpring_Unknown16;
    }
}

void LaunchSpring_Unknown16(void)
{
    RSDK_THIS(LaunchSpring);
    RSDK.ProcessAnimation(&entity->animator3);

    if (entity->animator3.frameID == entity->animator3.frameCount - 1) {
        RSDK.SetSpriteAnimation(LaunchSpring->aniFrames, 2, &entity->animator3, true, 0);
        entity->animator3.frameID = 1;
        entity->state             = LaunchSpring_Unknown13;
    }
}

#if RETRO_INCLUDE_EDITOR
void LaunchSpring_EditorDraw(void)
{
    RSDK_THIS(LaunchSpring);
    if (RSDK.CheckStageFolder("SSZ1") || RSDK.CheckStageFolder("SSZ2")) {
        RSDK.SetSpriteAnimation(LaunchSpring->aniFrames, 0, &entity->animator1, true, 0);
        RSDK.SetSpriteAnimation(LaunchSpring->aniFrames, 1, &entity->animator2, true, 0);
        RSDK.SetSpriteAnimation(LaunchSpring->aniFrames, 2, &entity->animator3, true, 0);

        LaunchSpring_Draw();
    }
}

void LaunchSpring_EditorLoad(void)
{
    if (RSDK.CheckStageFolder("SSZ1"))
        LaunchSpring->aniFrames = RSDK.LoadSpriteAnimation("SSZ1/LaunchSpring.bin", SCOPE_STAGE);
    else if (RSDK.CheckStageFolder("SSZ2"))
        LaunchSpring->aniFrames = RSDK.LoadSpriteAnimation("SSZ2/LaunchSpring.bin", SCOPE_STAGE);
}
#endif

void LaunchSpring_Serialize(void)
{
    RSDK_EDITABLE_VAR(LaunchSpring, VAR_UINT8, type);
    RSDK_EDITABLE_VAR(LaunchSpring, VAR_UINT8, rotDir);
    RSDK_EDITABLE_VAR(LaunchSpring, VAR_UINT8, rotSpeed);
    RSDK_EDITABLE_VAR(LaunchSpring, VAR_INT32, angle);
    RSDK_EDITABLE_VAR(LaunchSpring, VAR_BOOL, manual);
    RSDK_EDITABLE_VAR(LaunchSpring, VAR_BOOL, strict);
}
