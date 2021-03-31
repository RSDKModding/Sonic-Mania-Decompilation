#include "../SonicMania.h"

ObjectUFO_Ring *UFO_Ring;

void UFO_Ring_Update()
{
    RSDK_THIS(UFO_Ring);
    if (entity->state)
        entity->state();
}

void UFO_Ring_LateUpdate()
{
    RSDK_THIS(UFO_Ring);
    int x = entity->position.x >> 8;
    int y = entity->height >> 8;
    int z = entity->position.y >> 8;

    Matrix *mat = &UFO_Camera->matWorld;

    entity->worldX = mat->values[0][3] + (y * mat->values[0][1] >> 8) + (z * mat->values[0][2] >> 8) + (x * mat->values[0][0] >> 8);
    entity->worldY = mat->values[1][3] + (y * mat->values[1][1] >> 8) + (z * mat->values[1][2] >> 8) + (x * mat->values[1][0] >> 8);
    entity->depth  = mat->values[2][3] + (y * mat->values[2][1] >> 8) + (z * mat->values[2][2] >> 8) + (x * mat->values[2][0] >> 8);
}

void UFO_Ring_StaticUpdate() {}

void UFO_Ring_Draw()
{
    RSDK_THIS(UFO_Ring);

    if (entity->depth >= 256) {
        entity->direction = entity->data.frameID > 8;
        Vector2 drawPos;
        drawPos.x = (RSDK_screens->centerX + (entity->worldX << 8) / entity->depth) << 16;
        drawPos.y = (RSDK_screens->centerY - (entity->worldY << 8) / entity->depth) << 16;

        entity->scale.x = 0x1000000 / entity->depth;
        entity->scale.y = 0x1000000 / entity->depth;
        if (entity->state == UFO_Ring_Unknown2)
            entity->data.frameID = UFO_Setup->ringFrame;
        RSDK.DrawSprite(&entity->data, &drawPos, true);
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
        RSDK.SetSpriteAnimation(UFO_Ring->spriteIndex, 1, &entity->data, true, 0);
    }
}

void UFO_Ring_StageLoad()
{
    UFO_Ring->spriteIndex = RSDK.LoadSpriteAnimation("Global/Ring.bin", SCOPE_STAGE);
    UFO_Ring->sfx_Ring    = RSDK.GetSFX("Global/Ring.wav");
}

void UFO_Ring_PlayRingSFX()
{
    if (UFO_Setup->rings < 999) {
        UFO_Setup->rings++;
    }

    if (UFO_Setup->ringPan) {
        int channel = RSDK.PlaySFX(UFO_Ring->sfx_Ring, 0, 255);
        RSDK.SetChannelAttributes(channel, 1.0, -1.0, 1.0);
        UFO_Setup->ringPan = 0;
    }
    else {
        int channel = RSDK.PlaySFX(UFO_Ring->sfx_Ring, 0, 255);
        RSDK.SetChannelAttributes(channel, 1.0, 1.0, 1.0);
        UFO_Setup->ringPan = 1;
    }
}

void UFO_Ring_LoseRings(EntityUFO_Player *player)
{
    UFO_Setup->rings -= 10;
    int i = 10;
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
        ring->data.animationSpeed = 0x100;
        ring->state               = UFO_Ring_Unknown4;
    }
}

void UFO_Ring_Unknown2()
{
    RSDK_THIS(UFO_Ring);
    foreach_active(UFO_Player, player)
    {
        int rx     = (entity->height - player->height - 0xA0000) >> 16;
        int ry     = (entity->position.y - player->position.y) >> 16;
        int radius = ry * ry + rx * rx;

        int pr = UFO_Player->maxSpeed >> 9;
        if (((entity->position.x - player->position.x) >> 16) * ((entity->position.x - player->position.x) >> 16) + radius < pr) {
            RSDK.SetSpriteAnimation(UFO_Ring->spriteIndex, 2, &entity->data, true, 4);
            ++entity->drawOrder;
            entity->state = UFO_Ring_Unknown3;
            UFO_Ring_PlayRingSFX();
        }
    }
}

void UFO_Ring_Unknown3()
{
    RSDK_THIS(UFO_Ring);
    RSDK.ProcessAnimation(&entity->data);

    if (entity->data.frameID == entity->data.frameCount - 1)
        RSDK.ResetEntityPtr(entity, TYPE_BLANK, 0);
}

void UFO_Ring_Unknown4()
{
    RSDK_THIS(UFO_Ring);
    RSDK.ProcessAnimation(&entity->data);
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    entity->field_64 -= 0x3800;

    entity->height += entity->field_64;
    if (entity->height < 0xC0000 && entity->field_64 < 0) {
        entity->height   = 0xC0000;
        entity->field_64 = -(entity->field_64 >> 1);
    }

    if (entity->data.animationSpeed > 0x40)
        entity->data.animationSpeed--;

    entity->alpha -= 4;
    if (!entity->alpha)
        RSDK.ResetEntityPtr(entity, TYPE_BLANK, 0);
}

void UFO_Ring_EditorDraw() {}

void UFO_Ring_EditorLoad() {}

void UFO_Ring_Serialize() { RSDK_EDITABLE_VAR(UFO_Ring, VAR_ENUM, height); }
