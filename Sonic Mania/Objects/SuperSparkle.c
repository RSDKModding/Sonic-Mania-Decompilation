#include "../SonicMania.h"

ObjectSuperSparkle *SuperSparkle;

void SuperSparkle_Update()
{
    EntitySuperSparkle *entity = (EntitySuperSparkle *)RSDK_sceneInfo->entity;
    EntityPlayer *player       = (EntityPlayer *)entity->player;
    if (!player) {
        RSDK.ResetEntityPtr(entity, 0, 0);
        return;
    }

    if (player->groundedStore) {
        entity->activeFlag = abs(player->velocity.y) + abs(player->velocity.x) > 0x60000;
    }

    if (entity->activeFlag) {
        if (++entity->timer == 12) {
            entity->timer        = 0;
            EntityDebris *debris = (EntityDebris *)RSDK.CreateEntity(Debris->objectID, 0, player->position.x, player->position.y);
            debris->state          = Debris_State_LightningSpark;
            debris->timer          = 16;
            debris->inkEffect = INK_ADD;
            debris->alpha     = 256;
            debris->drawOrder = Zone->drawOrderHigh;
            debris->drawOrder = player->drawOrder;
            RSDK.SetSpriteAnimation(SuperSparkle->spriteIndex, 0, &debris->data, true, 0);
        }
    }
    else {
        entity->timer = 0;
    }

    if (player->characterID == ID_SONIC && !(Zone->timer & 7)) {
        EntityRing *ring = (EntityRing *)RSDK.CreateEntity(Ring->objectID, 0, player->position.x + RSDK.Rand(-0xC0000, 0xC0000),
                                                          player->position.y + RSDK.Rand(-0x120000, 0x120000));
        ring->state      = Ring_State_Sparkle;
        ring->stateDraw  = Ring_StateDraw_Sparkle;
        ring->active     = ACTIVE_NORMAL;
        ring->visible    = 0;
        ring->velocity.y = -0x10000;
        ring->drawOrder  = player->drawOrder;
        RSDK.SetSpriteAnimation(Ring->spriteIndex, Zone->timer % 3 + 2, &ring->animData, true, 0);
        int cnt = ring->animData.frameCount;
        if (ring->animData.animationID == 2) {
            ring->alpha = 224;
            cnt >>= 1;
        }
        ring->maxFrameCount           = cnt - 1;
        ring->animData.animationSpeed = RSDK.Rand(6, 8);
    }
    if (player->superState != 2 || !player->active)
        RSDK.ResetEntityPtr(entity, 0, 0);
}

void SuperSparkle_LateUpdate()
{

}

void SuperSparkle_StaticUpdate()
{

}

void SuperSparkle_Draw()
{

}

void SuperSparkle_Create(void *data)
{
    EntitySuperSparkle *entity = (EntitySuperSparkle *)RSDK_sceneInfo->entity;
    if (!RSDK_sceneInfo->inEditor) {
        entity->active = ACTIVE_NORMAL;
        entity->player = (EntityPlayer *)data;
    }
}

void SuperSparkle_StageLoad() { SuperSparkle->spriteIndex = RSDK.LoadAnimation("Global/SuperSparkle.bin", SCOPE_STAGE); }

void SuperSparkle_EditorDraw()
{

}

void SuperSparkle_EditorLoad()
{

}

void SuperSparkle_Serialize()
{

}

