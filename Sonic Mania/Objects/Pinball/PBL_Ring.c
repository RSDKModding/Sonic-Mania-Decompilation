#include "SonicMania.h"

#if RETRO_USE_PLUS
ObjectPBL_Ring *PBL_Ring;

void PBL_Ring_Update(void)
{
    RSDK_THIS(PBL_Ring);
    StateMachine_Run(entity->state);
}

void PBL_Ring_LateUpdate(void)
{
    RSDK_THIS(PBL_Ring);
    int32 x = entity->position.x >> 8;
    int32 y = entity->height >> 8;
    int32 z = entity->position.y >> 8;

    Matrix *mat = &PBL_Camera->matrix1;

    entity->worldX = mat->values[0][3] + (y * mat->values[0][1] >> 8) + (z * mat->values[0][2] >> 8) + (x * mat->values[0][0] >> 8);
    entity->worldY = mat->values[1][3] + (y * mat->values[1][1] >> 8) + (z * mat->values[1][2] >> 8) + (x * mat->values[1][0] >> 8);
    entity->depth3D = mat->values[2][3] + (y * mat->values[2][1] >> 8) + (z * mat->values[2][2] >> 8) + (x * mat->values[2][0] >> 8);
}

void PBL_Ring_StaticUpdate(void) {}

void PBL_Ring_Draw(void)
{
    RSDK_THIS(PBL_Ring);

    if (entity->depth3D >= 0x100) {
        Vector2 drawPos;
        entity->direction = entity->animator.frameID > 8;
        drawPos.x         = (ScreenInfo->centerX + (entity->worldX << 8) / entity->depth3D) << 16;
        drawPos.y         = (ScreenInfo->centerY - (entity->worldY << 8) / entity->depth3D) << 16;
        entity->scale.x   = 0x1000000 / entity->depth3D;
        entity->scale.y   = 0x1000000 / entity->depth3D;
        if (entity->state == PBL_Ring_State_Ring)
            entity->animator.frameID = PBL_Setup->timer2;
        RSDK.DrawSprite(&entity->animator, &drawPos, true);
    }
}

void PBL_Ring_Create(void *data)
{
    RSDK_THIS(PBL_Ring);
    if (!SceneInfo->inEditor) {
        entity->visible       = true;
        entity->drawFX        = FX_SCALE | FX_FLIP;
        entity->drawOrder     = 4;
        entity->active        = ACTIVE_BOUNDS;
        entity->updateRange.x = 0x400000;
        entity->updateRange.y = 0x400000;
        if (entity->height == 0)
            entity->height = 12;
        entity->height <<= 16;
        entity->state = PBL_Ring_State_Ring;
        RSDK.SetSpriteAnimation(PBL_Ring->aniFrames, 1, &entity->animator, true, 0);
    }
}

void PBL_Ring_StageLoad(void)
{
    PBL_Ring->aniFrames = RSDK.LoadSpriteAnimation("Global/Ring.bin", SCOPE_STAGE);
    PBL_Ring->sfxRing   = RSDK.GetSFX("Global/Ring.wav");
}

void PBL_Ring_GiveRing(void)
{
    if (PBL_Setup->rings < 999)
        PBL_Setup->rings++;
    PBL_Setup_GiveScore(10);

    if (PBL_Setup->ringPan) {
        int32 channel = RSDK.PlaySfx(PBL_Ring->sfxRing, 0, 255);
        RSDK.SetChannelAttributes(channel, 1.0, -1.0, 1.0);
        PBL_Setup->ringPan = 0;
    }
    else {
        int32 channel = RSDK.PlaySfx(PBL_Ring->sfxRing, 0, 255);
        RSDK.SetChannelAttributes(channel, 1.0, 1.0, 1.0);
        PBL_Setup->ringPan = 1;
    }
}

void PBL_Ring_State_Ring(void)
{
    RSDK_THIS(PBL_Ring);

    foreach_active(PBL_Player, player)
    {
        int32 ry = (entity->height - player->height - 0xA0000) >> 16;
        int32 rz = (entity->position.y - player->position.y) >> 16;
        int32 rx = (entity->position.x - player->position.x) >> 16;
        if (rx * rx + rz * rz + ry * ry < 0x100) {
            RSDK.SetSpriteAnimation(PBL_Ring->aniFrames, 2, &entity->animator, true, 4);
            ++entity->drawOrder;
            entity->state = PBL_Ring_State_RingSparkle;
            PBL_Ring_GiveRing();
        }
    }
}

void PBL_Ring_State_RingSparkle(void)
{
    RSDK_THIS(PBL_Ring);
    RSDK.ProcessAnimation(&entity->animator);
    if (entity->animator.frameID == 15) {
        entity->state  = StateMachine_None;
        entity->active = ACTIVE_NEVER;
    }
}

void PBL_Ring_EditorDraw(void)
{
    RSDK_THIS(PBL_Ring);
    RSDK.SetSpriteAnimation(PBL_Ring->aniFrames, 1, &entity->animator, true, 0);
    RSDK.DrawSprite(&entity->animator, NULL, true);
}

void PBL_Ring_EditorLoad(void) { PBL_Ring->aniFrames = RSDK.LoadSpriteAnimation("Global/Ring.bin", SCOPE_STAGE); }

void PBL_Ring_Serialize(void) { RSDK_EDITABLE_VAR(PBL_Ring, VAR_ENUM, height); }
#endif
