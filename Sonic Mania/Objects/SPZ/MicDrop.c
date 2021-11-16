#include "SonicMania.h"

ObjectMicDrop *MicDrop;

void MicDrop_Update(void)
{
    RSDK_THIS(MicDrop);
    StateMachine_Run(entity->state);
}

void MicDrop_LateUpdate(void) {}

void MicDrop_StaticUpdate(void) {}

void MicDrop_Draw(void)
{
    RSDK_THIS(MicDrop);

    SpriteFrame *frame      = RSDK.GetFrame(MicDrop->aniFrames, 1, entity->animator2.frameID);
    SpriteFrame *otherFrame = RSDK.GetFrame(MicDrop->aniFrames, 1, entity->animator2.frameID + 4);

    if (entity->animator2.frameID == 3) {
        RSDK.DrawLine(entity->startPos.x, entity->startPos.y, entity->position.x, entity->position.y, 0x081828, 0, INK_NONE, false);
        RSDK.DrawLine(entity->startPos.x - 0x10000, entity->startPos.y, entity->position.x - 0x10000, entity->position.y, 0x204078, 0, INK_NONE,
                      false);
    }
    else {
        int distance = (entity->position.y - entity->startPos.y) >> 16;
        if (distance > 58) {
            frame->height = otherFrame->height;
            frame->pivotY = otherFrame->pivotY;
            frame->sprY   = otherFrame->sprY;
            RSDK.DrawLine(entity->startPos.x, entity->startPos.y, entity->position.x, entity->position.y - (58 << 16), 0x081828, 0, INK_NONE, false);
            RSDK.DrawLine(entity->startPos.x - 0x10000, entity->startPos.y, entity->position.x - 0x10000, entity->position.y - (58 << 16), 0x204078,
                          0, INK_NONE, false);
        }
        else {
            frame->height = distance + 2;
            frame->pivotY = 1 - distance;
            frame->sprY   = otherFrame->sprY - distance + 58;
        }
    }

    RSDK.DrawSprite(&entity->animator2, NULL, false);
    RSDK.DrawSprite(&entity->animator1, NULL, false);
}

void MicDrop_Create(void *data)
{
    RSDK_THIS(MicDrop);

    entity->visible       = true;
    entity->drawOrder     = Zone->drawOrderLow;
    entity->startPos.x    = entity->position.x;
    entity->startPos.y    = entity->position.y;
    entity->active        = ACTIVE_BOUNDS;
    entity->updateRange.x = 0x800000;
    entity->updateRange.y = 0x800000;
    entity->drawFX        = FX_ROTATE | FX_FLIP;
    RSDK.SetSpriteAnimation(MicDrop->aniFrames, 0, &entity->animator1, true, 0);
    RSDK.SetSpriteAnimation(MicDrop->aniFrames, 1, &entity->animator2, true, 0);
    entity->state = MicDrop_State_Setup;
}

void MicDrop_StageLoad(void)
{
    if (RSDK.CheckStageFolder("SPZ1"))
        MicDrop->aniFrames = RSDK.LoadSpriteAnimation("SPZ1/MicDrop.bin", SCOPE_STAGE);
    else
        MicDrop->aniFrames = RSDK.LoadSpriteAnimation("SPZ2/MicDrop.bin", SCOPE_STAGE);
    MicDrop->hitbox1.left   = -12;
    MicDrop->hitbox1.top    = -15;
    MicDrop->hitbox1.right  = 12;
    MicDrop->hitbox1.bottom = 15;
    MicDrop->hitbox2.left   = -128;
    MicDrop->hitbox2.top    = 0;
    MicDrop->hitbox2.right  = 128;
    MicDrop->hitbox2.bottom = 256;
    DEBUGMODE_ADD_OBJ(MicDrop);
    MicDrop->sfxElectrify = RSDK.GetSFX("Stage/Electrify.wav");
}

void MicDrop_DebugSpawn(void)
{
    RSDK_THIS(DebugMode);

    EntityMicDrop *micDrop = CREATE_ENTITY(MicDrop, NULL, entity->position.x, entity->position.y);
    micDrop->distance      = 128;
}

void MicDrop_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(MicDrop->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
}

void MicDrop_CheckPlayerCollisions(void)
{
    RSDK_THIS(MicDrop);

    int storeX = entity->position.x;
    int storeY = entity->position.y;

    entity->position.x = (((entity->radius + 25) * RSDK.Cos512(entity->angle + 128)) << 7) + entity->startPos.x;
    entity->position.y = (((entity->radius + 25) * RSDK.Sin512(entity->angle + 128)) << 7) + entity->startPos.y;

    foreach_active(Player, player)
    {
        if (Player_CheckBadnikTouch(player, entity, &MicDrop->hitbox1)) {
            if (entity->animator1.animationID != 3 || player->shield == SHIELD_LIGHTNING || player->invincibleTimer || player->blinkTimer) {
                Player_CheckBadnikBreak(entity, player, true);
            }
            else {
                Player_CheckHit(player, entity);
            }
        }
    }
    entity->position.x = storeX;
    entity->position.y = storeY;
}

void MicDrop_CheckOnScreen(void)
{

    RSDK_THIS(MicDrop);

    if (!RSDK.CheckOnScreen(entity, NULL) && !RSDK.CheckPosOnScreen(&entity->startPos, &entity->updateRange)) {
        entity->position.x = entity->startPos.x;
        entity->position.y = entity->startPos.y;
        MicDrop_Create(NULL);
    }
}

void MicDrop_State_Setup(void)
{
    RSDK_THIS(MicDrop);

    entity->active     = ACTIVE_NORMAL;
    entity->velocity.y = 0;
    entity->direction  = FLIP_NONE;
    entity->timer      = 0;
    entity->field_A8   = 0;
    entity->angle      = 0;
    entity->rotation   = 0;
    entity->state      = MicDrop_State_Unknown1;
    MicDrop_State_Unknown1();
}

void MicDrop_State_Unknown1(void)
{
    RSDK_THIS(MicDrop);

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, entity, &MicDrop->hitbox2))
            entity->state = MicDrop_State_Unknown2;
    }
    MicDrop_CheckPlayerCollisions();
}

void MicDrop_State_Unknown2(void)
{
    RSDK_THIS(MicDrop);

    entity->position.y += entity->velocity.y;
    entity->velocity.y += 0x3800;
    entity->radius = (entity->position.y - entity->startPos.y) >> 16;

    if (entity->radius >= 0x20000)
        entity->animator1.frameID = 3;

    if (entity->radius >= entity->distance) {
        entity->radius            = entity->distance;
        entity->position.y        = entity->startPos.y + (entity->distance << 16);
        entity->animator1.frameID = 4;
        entity->state             = MicDrop_State_Unknown3;
    }
    MicDrop_CheckPlayerCollisions();
}

void MicDrop_State_Unknown3(void)
{
    RSDK_THIS(MicDrop);

    switch (++entity->timer) {
        case 2:
            entity->animator2.frameID = 1;
            entity->animator1.frameID = 5;
            break;
        case 4:
            entity->animator2.frameID = 2;
            entity->animator1.frameID = 1;
            break;
        case 6:
            entity->animator2.frameID = 3;
            entity->animator1.frameID = 0;
            entity->timer             = 0;
            entity->state             = MicDrop_State_Unknown4;
            break;
    }
    MicDrop_CheckPlayerCollisions();
    MicDrop_CheckOnScreen();
}

void MicDrop_State_Unknown4(void)
{
    RSDK_THIS(MicDrop);

    if (++entity->timer >= 30) {
        entity->field_9C = 64;
        entity->field_A0 = 4;
        entity->state    = MicDrop_State_Unknown5;
    }
    MicDrop_CheckPlayerCollisions();
    MicDrop_CheckOnScreen();
}

void MicDrop_State_Unknown5(void)
{
    RSDK_THIS(MicDrop);

    entity->field_A8 += entity->field_9C;
    if (entity->field_A8 < 0) {
        entity->field_A0 = 1;
        entity->field_A8 -= entity->field_9C;
        entity->field_9C = entity->field_9C >> 1;
        entity->state    = MicDrop_State_Unknown6;
        MicDrop_State_Unknown6();
    }
    else {
        entity->rotation   = entity->field_A8 >> 4;
        entity->angle      = entity->rotation >> 2;
        entity->field_9C   = entity->field_9C - entity->field_A0;
        entity->position.x = (entity->radius << 7) * RSDK.Cos512(entity->angle + 128) + entity->startPos.x;
        entity->position.y = (entity->radius << 7) * RSDK.Sin512(entity->angle + 128) + entity->startPos.y;
        MicDrop_CheckPlayerCollisions();
        MicDrop_CheckOnScreen();
    }
}

void MicDrop_State_Unknown6(void)
{
    RSDK_THIS(MicDrop);

    entity->field_A8 += entity->field_9C;
    entity->angle    = entity->field_A8 >> 4;
    entity->rotation = entity->angle;
    if (entity->angle >= 0)
        entity->field_9C -= entity->field_A0;
    else
        entity->field_9C += entity->field_A0;
    entity->position.x = (entity->radius << 7) * RSDK.Cos512(entity->angle + 128) + entity->startPos.x;
    entity->position.y = (entity->radius << 7) * RSDK.Sin512(entity->angle + 128) + entity->startPos.y;

    switch (++entity->timer) {
        case 60: RSDK.SetSpriteAnimation(MicDrop->aniFrames, 2, &entity->animator1, true, 0); break;
        case 90:
            RSDK.PlaySfx(MicDrop->sfxElectrify, false, 255);
            RSDK.SetSpriteAnimation(MicDrop->aniFrames, 3, &entity->animator1, true, 0);
            break;
        case 120:
            entity->timer = 0;
            RSDK.SetSpriteAnimation(MicDrop->aniFrames, 0, &entity->animator1, true, 0);
            break;
    }

    if (entity->timer >= 60)
        RSDK.ProcessAnimation(&entity->animator1);
    MicDrop_CheckPlayerCollisions();
    MicDrop_CheckOnScreen();
}

#if RETRO_INCLUDE_EDITOR
void MicDrop_EditorDraw(void)
{
    RSDK_THIS(MicDrop);

    entity->startPos.y = entity->position.y;
    entity->position.y = entity->startPos.y + (entity->distance << 16);

    MicDrop_Draw();

    entity->position.y = entity->startPos.y;
}

void MicDrop_EditorLoad(void)
{
    if (RSDK.CheckStageFolder("SPZ1"))
        MicDrop->aniFrames = RSDK.LoadSpriteAnimation("SPZ1/MicDrop.bin", SCOPE_STAGE);
    else
        MicDrop->aniFrames = RSDK.LoadSpriteAnimation("SPZ2/MicDrop.bin", SCOPE_STAGE);
}
#endif

void MicDrop_Serialize(void) { RSDK_EDITABLE_VAR(MicDrop, VAR_UINT16, distance); }
