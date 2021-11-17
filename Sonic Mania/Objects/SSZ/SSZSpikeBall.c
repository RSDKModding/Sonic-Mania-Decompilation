#include "SonicMania.h"

ObjectSSZSpikeBall *SSZSpikeBall;

void SSZSpikeBall_Update(void)
{
    RSDK_THIS(SSZSpikeBall);
    StateMachine_Run(entity->state);

    foreach_active(Player, player)
    {
        int type = entity->type;
        if (type < 4)
            Player_CheckCollisionBox(player, entity, &SSZSpikeBall->hitboxBase[type]);

        if (Player_CheckCollisionTouch(player, &entity->spikeBallPos, &SSZSpikeBall->hitboxBall)) {
#if RETRO_USE_PLUS
            if (!Player_CheckMightyUnspin(0x400, player, 2, &player->uncurlTimer))
#endif
                Player_CheckHit(player, entity);
        }
    }
}

void SSZSpikeBall_LateUpdate(void) {}

void SSZSpikeBall_StaticUpdate(void) {}

void SSZSpikeBall_Draw(void)
{
    RSDK_THIS(SSZSpikeBall);
    RSDK.DrawSprite(&entity->animator2, &entity->spikeBallPos, false);
    RSDK.DrawSprite(&entity->animator1, NULL, false);
}

void SSZSpikeBall_Create(void *data)
{
    RSDK_THIS(SSZSpikeBall);
    if (!entity->speed) {
        entity->speed = 12;
        entity->dist  = 64;
    }
    if (!entity->interval)
        entity->interval = 120;

    if (!SceneInfo->inEditor) {
        RSDK.SetSpriteAnimation(SSZSpikeBall->aniFrames, 0, &entity->animator1, true, 0);
        RSDK.SetSpriteAnimation(SSZSpikeBall->aniFrames, 1, &entity->animator2, true, 0);

        entity->active         = ACTIVE_BOUNDS;
        entity->updateRange.x  = 0x600000;
        entity->updateRange.y  = 0x600000;
        entity->visible        = true;
        entity->drawOrder      = Zone->drawOrderLow;
        entity->speed <<= 15;
        entity->dist           = (entity->dist << 16) / entity->speed;
        entity->spikeBallPos.x = entity->position.x;
        entity->spikeBallPos.y = entity->position.y;

        switch (entity->type) {
            case 0:
            case 2:
                entity->spikeBallPos.x = -0x200000;
                entity->spikeBallPos.y = -0x200000;
                entity->direction      = FLIP_NONE;
                break;
            case 1:
            case 3:
                entity->spikeBallPos.x = -0x200000;
                entity->spikeBallPos.y = -0x200000;
                entity->direction      = FLIP_X;
                break;
            case 4: entity->velocity.y = -entity->speed; break;
            case 5: entity->velocity.y = entity->speed; break;
            case 6: entity->velocity.x = -entity->speed; break;
            case 7: entity->velocity.x = entity->speed; break;
            default: break;
        }

        entity->state             = SSZSpikeBall_State_Setup;
        entity->animator1.frameID = minVal(entity->type, 4);
    }
}

void SSZSpikeBall_StageLoad(void)
{
    if (RSDK.CheckStageFolder("SSZ1"))
        SSZSpikeBall->aniFrames = RSDK.LoadSpriteAnimation("SSZ1/SpikeBall.bin", SCOPE_STAGE);
    else
        SSZSpikeBall->aniFrames = RSDK.LoadSpriteAnimation("SSZ2/SpikeBall.bin", SCOPE_STAGE);

    SSZSpikeBall->hitboxBall.left  = -12;
    SSZSpikeBall->hitboxBall.top  = -12;
    SSZSpikeBall->hitboxBall.right = 12;
    SSZSpikeBall->hitboxBall.bottom = 12;

    SSZSpikeBall->hitboxBase[0].left  = -16;
    SSZSpikeBall->hitboxBase[0].top  = -12;
    SSZSpikeBall->hitboxBase[0].right = 16;
    SSZSpikeBall->hitboxBase[0].bottom = 0;

    SSZSpikeBall->hitboxBase[1].left   = -16;
    SSZSpikeBall->hitboxBase[1].top    = 0;
    SSZSpikeBall->hitboxBase[1].right  = 16;
    SSZSpikeBall->hitboxBase[1].bottom = 12;

    SSZSpikeBall->hitboxBase[2].left   = -12;
    SSZSpikeBall->hitboxBase[2].top    = -16;
    SSZSpikeBall->hitboxBase[2].right  = 0;
    SSZSpikeBall->hitboxBase[2].bottom = 16;

    SSZSpikeBall->hitboxBase[3].left   = -12;
    SSZSpikeBall->hitboxBase[3].top    = -16;
    SSZSpikeBall->hitboxBase[3].right  = 0;
    SSZSpikeBall->hitboxBase[3].bottom = 16;

    SSZSpikeBall->sfxPon = RSDK.GetSFX("Stage/Pon.wav");
}

void SSZSpikeBall_State_Setup(void)
{
    RSDK_THIS(SSZSpikeBall);
    if (!((Zone->timer + entity->intervalOffset) % entity->interval)) {
        entity->spikeBallPos.x = entity->position.x;
        entity->spikeBallPos.y = entity->position.y;
        entity->active         = ACTIVE_NORMAL;
        entity->timer          = 0;
        switch (entity->type) {
            case 0:
                entity->state = SSZSpikeBall_Unknown3;
                entity->spikeBallPos.y += 0x40000;
                entity->velocity.y = -entity->speed;
                break;
            case 1:
                entity->spikeBallPos.y -= 0x40000;
                entity->state      = SSZSpikeBall_Unknown3;
                entity->velocity.y = entity->speed;
                break;
            case 2:
                entity->state = SSZSpikeBall_Unknown2;
                entity->spikeBallPos.x += 0x30000;
                entity->velocity.x = -entity->speed;
                break;
            case 3:
                entity->spikeBallPos.x -= 0x30000;
                entity->state      = SSZSpikeBall_Unknown2;
                entity->velocity.x = entity->speed;
                break;
            case 4:
            case 5:
            case 6:
            case 7:
                entity->timer = entity->dist;
                entity->state = SSZSpikeBall_Unknown6;
                break;
            default: break;
        }
    }
}

void SSZSpikeBall_Unknown2(void)
{
    RSDK_THIS(SSZSpikeBall);

    if (++entity->timer >= 0x20) {
        if (entity->timer == 48) {
            RSDK.PlaySfx(SSZSpikeBall->sfxPon, false, 255);
            entity->state = SSZSpikeBall_Unknown4;
        }
    }
    else if (entity->direction)
        entity->spikeBallPos.x += 0x8000;
    else
        entity->spikeBallPos.x -= 0x8000;
}

void SSZSpikeBall_Unknown3(void)
{
    RSDK_THIS(SSZSpikeBall);

    if (++entity->timer >= 0x20) {
        if (entity->timer == 48) {
            RSDK.PlaySfx(SSZSpikeBall->sfxPon, false, 255);
            entity->state = SSZSpikeBall_Unknown5;
        }
    }
    else if (entity->direction)
        entity->spikeBallPos.y += 0x8000;
    else
        entity->spikeBallPos.y -= 0x8000;
}

void SSZSpikeBall_Unknown4(void)
{
    RSDK_THIS(SSZSpikeBall);

    entity->spikeBallPos.x += entity->velocity.x;
    if (entity->direction) {
        entity->velocity.x -= 0x3800;
        if (entity->spikeBallPos.x < entity->position.x) {
            entity->state          = SSZSpikeBall_State_Setup;
            entity->active         = ACTIVE_BOUNDS;
            entity->spikeBallPos.x = -0x200000;
        }
    }
    else {
        entity->velocity.x += 0x3800;
        if (entity->spikeBallPos.x > entity->position.x) {
            entity->state          = SSZSpikeBall_State_Setup;
            entity->active         = ACTIVE_BOUNDS;
            entity->spikeBallPos.x = -0x200000;
        }
    }
}

void SSZSpikeBall_Unknown5(void)
{
    RSDK_THIS(SSZSpikeBall);

    entity->spikeBallPos.y += entity->velocity.y;
    if (entity->direction) {
        entity->velocity.y -= 0x3800;
        if (entity->spikeBallPos.y < entity->position.y) {
            entity->state          = SSZSpikeBall_State_Setup;
            entity->active         = ACTIVE_BOUNDS;
            entity->spikeBallPos.y = -0x200000;
        }
    }
    else {
        entity->velocity.y += 0x3800;
        if (entity->spikeBallPos.y > entity->position.y) {
            entity->state          = SSZSpikeBall_State_Setup;
            entity->active         = ACTIVE_BOUNDS;
            entity->spikeBallPos.y = -0x200000;
        }
    }
}

void SSZSpikeBall_Unknown6(void)
{
    RSDK_THIS(SSZSpikeBall);

    entity->spikeBallPos.x += entity->velocity.x;
    entity->spikeBallPos.y += entity->velocity.y;
    if (!--entity->timer) {
        entity->timer = (entity->interval >> 1) - entity->dist;
        entity->state = SSZSpikeBall_Unknown7;
    }
}

void SSZSpikeBall_Unknown7(void)
{
    RSDK_THIS(SSZSpikeBall);

    if (!--entity->timer) {
        entity->timer = entity->dist;
        entity->state = SSZSpikeBall_Unknown8;
    }
}

void SSZSpikeBall_Unknown8(void)
{
    RSDK_THIS(SSZSpikeBall);

    entity->spikeBallPos.x -= entity->velocity.x;
    entity->spikeBallPos.y -= entity->velocity.y;
    if (!--entity->timer) {
        entity->active = ACTIVE_BOUNDS;
        entity->state  = SSZSpikeBall_State_Setup;
    }
}

#if RETRO_INCLUDE_EDITOR
void SSZSpikeBall_EditorDraw(void)
{
    RSDK_THIS(SSZSpikeBall);
    RSDK.SetSpriteAnimation(SSZSpikeBall->aniFrames, 0, &entity->animator1, true, 0);
    RSDK.SetSpriteAnimation(SSZSpikeBall->aniFrames, 1, &entity->animator2, true, 0);

    entity->spikeBallPos.x = entity->position.x;
    entity->spikeBallPos.y = entity->position.y;

    switch (entity->type) {
        case 0:
        case 2:
            entity->spikeBallPos.x = -0x200000;
            entity->spikeBallPos.y = -0x200000;
            entity->direction      = FLIP_NONE;
            break;
        case 1:
        case 3:
            entity->spikeBallPos.x = -0x200000;
            entity->spikeBallPos.y = -0x200000;
            entity->direction      = FLIP_X;
            break;
        default: break;
    }

    entity->animator1.frameID = minVal(entity->type, 4);

    SSZSpikeBall_Draw();
}

void SSZSpikeBall_EditorLoad(void)
{
    if (RSDK.CheckStageFolder("SSZ1"))
        SSZSpikeBall->aniFrames = RSDK.LoadSpriteAnimation("SSZ1/SpikeBall.bin", SCOPE_STAGE);
    else
        SSZSpikeBall->aniFrames = RSDK.LoadSpriteAnimation("SSZ2/SpikeBall.bin", SCOPE_STAGE);
}
#endif

void SSZSpikeBall_Serialize(void)
{
    RSDK_EDITABLE_VAR(SSZSpikeBall, VAR_UINT8, type);
    RSDK_EDITABLE_VAR(SSZSpikeBall, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(SSZSpikeBall, VAR_UINT16, interval);
    RSDK_EDITABLE_VAR(SSZSpikeBall, VAR_UINT16, intervalOffset);
    RSDK_EDITABLE_VAR(SSZSpikeBall, VAR_ENUM, speed);
    RSDK_EDITABLE_VAR(SSZSpikeBall, VAR_ENUM, dist);
}
