#include "SonicMania.h"

ObjectUFO_Ring *UFO_Ring;

void UFO_Ring_Update(void)
{
    RSDK_THIS(UFO_Ring);
    StateMachine_Run(entity->state);
}

void UFO_Ring_LateUpdate(void)
{
    RSDK_THIS(UFO_Ring);
    int32 x = entity->position.x >> 8;
    int32 y = entity->height >> 8;
    int32 z = entity->position.y >> 8;

    Matrix *mat = &UFO_Camera->matWorld;

    entity->worldX = mat->values[0][3] + (y * mat->values[0][1] >> 8) + (z * mat->values[0][2] >> 8) + (x * mat->values[0][0] >> 8);
    entity->worldY = mat->values[1][3] + (y * mat->values[1][1] >> 8) + (z * mat->values[1][2] >> 8) + (x * mat->values[1][0] >> 8);
    entity->depth3D = mat->values[2][3] + (y * mat->values[2][1] >> 8) + (z * mat->values[2][2] >> 8) + (x * mat->values[2][0] >> 8);
}

void UFO_Ring_StaticUpdate(void) {}

void UFO_Ring_Draw(void)
{
    RSDK_THIS(UFO_Ring);

    if (entity->depth3D >= 0x100) {
        entity->direction = entity->animator.frameID > 8;
        Vector2 drawPos;
        drawPos.x = (RSDK_screens->centerX + (entity->worldX << 8) / entity->depth3D) << 16;
        drawPos.y = (RSDK_screens->centerY - (entity->worldY << 8) / entity->depth3D) << 16;

        entity->scale.x = 0x1000000 / entity->depth3D;
        entity->scale.y = 0x1000000 / entity->depth3D;
        if (entity->state == UFO_Ring_Unknown2)
            entity->animator.frameID = UFO_Setup->ringFrame;
        RSDK.DrawSprite(&entity->animator, &drawPos, true);
    }
}

void UFO_Ring_Create(void *data)
{
    RSDK_THIS(UFO_Ring);
    if (!RSDK_sceneInfo->inEditor) {
        entity->visible       = true;
        entity->drawFX        = FX_FLIP | FX_SCALE;
        entity->drawOrder     = 4;
        entity->active        = ACTIVE_RBOUNDS;
        entity->updateRange.x = 0x400;
        entity->updateRange.y = 0x400;
        if (!entity->height)
            entity->height = 12;
        entity->height <<= 16;
        entity->state = UFO_Ring_Unknown2;
        RSDK.SetSpriteAnimation(UFO_Ring->spriteIndex, 1, &entity->animator, true, 0);
    }
}

void UFO_Ring_StageLoad(void)
{
    UFO_Ring->spriteIndex = RSDK.LoadSpriteAnimation("Global/Ring.bin", SCOPE_STAGE);
    UFO_Ring->sfx_Ring    = RSDK.GetSFX("Global/Ring.wav");
}

void UFO_Ring_PlayRingSFX(void)
{
    if (UFO_Setup->rings < 999) {
        UFO_Setup->rings++;
    }

    if (UFO_Setup->ringPan) {
        int32 channel = RSDK.PlaySfx(UFO_Ring->sfx_Ring, 0, 255);
        RSDK.SetChannelAttributes(channel, 1.0, -1.0, 1.0);
        UFO_Setup->ringPan = 0;
    }
    else {
        int32 channel = RSDK.PlaySfx(UFO_Ring->sfx_Ring, 0, 255);
        RSDK.SetChannelAttributes(channel, 1.0, 1.0, 1.0);
        UFO_Setup->ringPan = 1;
    }
}

void UFO_Ring_LoseRings(EntityUFO_Player *player)
{
    UFO_Setup->rings -= 10;
    int32 i = 10;
    if (UFO_Setup->rings < 0) {
        i                = UFO_Setup->rings + 10;
        UFO_Setup->rings = 0;
    }

    for (; i >= 0; --i) {
        EntityUFO_Ring *ring      = (EntityUFO_Ring *)RSDK.CreateEntity(UFO_Ring->objectID, 0, player->position.x, player->position.y);
        ring->height              = player->height + 0x80000;
        ring->velocity.x          = player->velocity.x + RSDK.Rand(-0x40000, 0x40000);
        ring->velocity.y          = player->velocity.y + RSDK.Rand(-0x40000, 0x40000);
        ring->field_64            = RSDK.Rand(0x40000, 0x60000);
        ring->inkEffect           = 2;
        ring->alpha               = 0x200;
        ring->animator.animationSpeed = 0x100;
        ring->state               = UFO_Ring_Unknown4;
    }
}

void UFO_Ring_Unknown2(void)
{
    RSDK_THIS(UFO_Ring);
    foreach_active(UFO_Player, player)
    {
        int32 rx     = (entity->height - player->height - 0xA0000) >> 16;
        int32 ry     = (entity->position.y - player->position.y) >> 16;
        int32 radius = ry * ry + rx * rx;

        int32 pr = UFO_Player->maxSpeed >> 9;
        if (((entity->position.x - player->position.x) >> 16) * ((entity->position.x - player->position.x) >> 16) + radius < pr) {
            RSDK.SetSpriteAnimation(UFO_Ring->spriteIndex, 2, &entity->animator, true, 4);
            ++entity->drawOrder;
            entity->state = UFO_Ring_Unknown3;
            UFO_Ring_PlayRingSFX();
        }
    }
}

void UFO_Ring_Unknown3(void)
{
    RSDK_THIS(UFO_Ring);
    RSDK.ProcessAnimation(&entity->animator);

    if (entity->animator.frameID == entity->animator.frameCount - 1)
        RSDK.ResetEntityPtr(entity, TYPE_BLANK, 0);
}

void UFO_Ring_Unknown4(void)
{
    RSDK_THIS(UFO_Ring);
    RSDK.ProcessAnimation(&entity->animator);
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    entity->field_64 -= 0x3800;

    entity->height += entity->field_64;
    if (entity->height < 0xC0000 && entity->field_64 < 0) {
        entity->height   = 0xC0000;
        entity->field_64 = -(entity->field_64 >> 1);
    }

    if (entity->animator.animationSpeed > 0x40)
        entity->animator.animationSpeed--;

    entity->alpha -= 4;
    if (!entity->alpha)
        RSDK.ResetEntityPtr(entity, TYPE_BLANK, 0);
}

void UFO_Ring_EditorDraw(void) {}

void UFO_Ring_EditorLoad(void) {}

void UFO_Ring_Serialize(void) { RSDK_EDITABLE_VAR(UFO_Ring, VAR_ENUM, height); }
