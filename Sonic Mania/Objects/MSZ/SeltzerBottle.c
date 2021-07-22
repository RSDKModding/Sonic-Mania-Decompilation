#include "SonicMania.h"

ObjectSeltzerBottle *SeltzerBottle;

void SeltzerBottle_Update(void)
{
    RSDK_THIS(SeltzerBottle);
    RSDK.ProcessAnimation(&entity->animator3);

    foreach_active(Player, player)
    {
        Player_CheckCollisionBox(player, entity, &SeltzerBottle->hitbox1);
        if (Player_CheckCollisionPlatform(player, entity, &SeltzerBottle->hitbox2) && !entity->animator2.frameID) {
            entity->animator2.frameID = 1;
            entity->active       = ACTIVE_NORMAL;
            entity->state             = SeltzerBottle_Unknown1;
            RSDK.SetSpriteAnimation(SeltzerBottle->aniFrames, 6, &entity->animator4, false, 0);
            player->groundVel = clampVal(player->groundVel, -0xC0000, 0xC0000);
            for (int p = 0; p < Player->playerCount; ++p) {
                RSDK_GET_ENTITY(p, Player)->collisionLayers |= SeltzerBottle->layerBitFlags;
            }
            RSDK.PlaySFX(SeltzerBottle->sfxSpray, 0, 255);
        }
    }
    StateMachine_Run(entity->state);
}

void SeltzerBottle_LateUpdate(void) {}

void SeltzerBottle_StaticUpdate(void) {}

void SeltzerBottle_Draw(void)
{
    RSDK_THIS(SeltzerBottle);
    Vector2 drawPos;

    RSDK.DrawRect(entity->position.x - 0x2E0000, entity->position.y - entity->field_64 + 0x2C0000, 0x5C0000, entity->field_64, 0x00F0F0, 64, INK_SUB,
                  false);

    drawPos = entity->position;
    drawPos.y += 0x2C0000 - entity->field_64;
    entity->inkEffect = INK_ADD;
    RSDK.DrawSprite(&entity->animator3, &drawPos, false);

    entity->animator1.frameID = 0;
    entity->inkEffect         = INK_SUB;
    RSDK.DrawSprite(&entity->animator1, NULL, false);

    entity->animator1.frameID = 1;
    entity->inkEffect         = INK_ADD;
    RSDK.DrawSprite(&entity->animator1, NULL, false);

    entity->animator1.frameID = 2;
    entity->inkEffect         = INK_NONE;
    RSDK.DrawSprite(&entity->animator1, NULL, false);

    entity->animator1.frameID = 3;
    RSDK.DrawSprite(&entity->animator1, NULL, false);

    entity->animator1.frameID = 4;
    RSDK.DrawSprite(&entity->animator1, NULL, false);

    entity->animator1.frameID = 5;
    entity->drawFX            = FX_FLIP;
    RSDK.DrawSprite(&entity->animator4, NULL, false);

    RSDK.DrawSprite(&entity->animator1, NULL, false);

    entity->drawFX = FX_NONE;
    RSDK.DrawSprite(&entity->animator2, NULL, false);
}

void SeltzerBottle_Create(void *data)
{
    RSDK_THIS(SeltzerBottle);
    if (!RSDK_sceneInfo->inEditor) {
        RSDK.SetSpriteAnimation(SeltzerBottle->aniFrames, 0, &entity->animator1, true, 0);
        RSDK.SetSpriteAnimation(SeltzerBottle->aniFrames, 1, &entity->animator2, true, 0);
        RSDK.SetSpriteAnimation(SeltzerBottle->aniFrames, 2, &entity->animator3, true, 0);
        entity->active        = ACTIVE_BOUNDS;
        entity->updateRange.x = 0x800000;
        entity->updateRange.y = 0x800000;
        entity->visible       = true;
        entity->drawOrder     = Zone->drawOrderHigh;
        entity->alpha         = 255;
        entity->field_60      = entity->sprayTime;
        entity->field_64      = 0x400000;
    }
}

void SeltzerBottle_StageLoad(void)
{
    SeltzerBottle->aniFrames      = RSDK.LoadSpriteAnimation("MSZ/Seltzer.bin", SCOPE_STAGE);
    SeltzerBottle->hitbox1.left   = -48;
    SeltzerBottle->hitbox1.top    = -80;
    SeltzerBottle->hitbox1.right  = 48;
    SeltzerBottle->hitbox1.bottom = 64;
    SeltzerBottle->hitbox2.left   = -16;
    SeltzerBottle->hitbox2.top    = -90;
    SeltzerBottle->hitbox2.right  = 16;
    SeltzerBottle->hitbox2.bottom = SeltzerBottle->hitbox2.top + 16;
    SeltzerBottle->layerID        = RSDK.GetSceneLayerID("Seltzer Path");
    if (SeltzerBottle->layerID)
        SeltzerBottle->layerBitFlags = 1 << SeltzerBottle->layerID;
    SeltzerBottle->sfxSpray = RSDK.GetSFX("MSZ/Spray.wav");
}

void SeltzerBottle_Unknown1(void)
{
    RSDK_THIS(SeltzerBottle);
    int timer = ((entity->field_60 << 14) / entity->sprayTime) << 8;
    if (timer < 0x40000)
        timer = 0x40000;
    entity->field_64 = timer;

    EntitySeltzerWater *spray = CREATE_ENTITY(SeltzerWater, NULL, entity->position.x, entity->position.y - 0x4C0000);
    if (entity->direction == FLIP_X)
        spray->position.x += 0x300000;
    else
        spray->position.x -= 0x300000;
    spray->field_64  = RSDK.Rand(0xA00, 0xC00);
    spray->field_5C  = RSDK.Rand(0, 0x100);
    spray->drawOrder = Zone->playerDrawLow;
    spray->nodeSlot  = RSDK_sceneInfo->entitySlot + 1;

    int storeX = entity->position.x;
    int storeY = entity->position.y;

    foreach_active(Player, player)
    {
        if (player->onGround) {
            entity->position.x = player->position.x;
            entity->position.y = player->position.y;
            entity->position.x += RSDK.Sin256(player->angle) << 13;
            entity->position.y += RSDK.Cos256(player->angle) << 13;
            if (RSDK.ObjectTileCollision(entity, SeltzerBottle->layerBitFlags, player->collisionMode, 0, 0, 0, false)) {
                if (entity->direction == FLIP_X) {
                    if (player->groundVel < 0x50000)
                        player->groundVel = 0x50000;
                }
                else if (player->groundVel > -0x50000) {
                    player->groundVel = -0x50000;
                }
            }
        }
    }

    entity->position.x = storeX;
    entity->position.y = storeY;
    RSDK.ProcessAnimation(&entity->animator4);

    if (--entity->field_60 <= 0) {
        for (int p = 0; p < Player->playerCount; ++p) {
            RSDK_GET_ENTITY(p, Player)->collisionLayers &= ~SeltzerBottle->layerBitFlags;
        }
        RSDK.SetSpriteAnimation(0xFFFF, 0, &entity->animator4, false, 0);
        entity->state = SeltzerBottle_Unknown2;
        foreach_active(SeltzerWater, water)
        {
            water->gravityStrength = RSDK.Rand(0x3800, 0x4000);
            water->state           = SeltzerWater_Unknown2;
        }
    }
}

void SeltzerBottle_Unknown2(void)
{
    RSDK_THIS(SeltzerBottle);

    if (!RSDK.CheckOnScreen(entity, NULL)) {
        entity->state             = StateMachine_None;
        entity->field_60          = entity->sprayTime;
        entity->field_64          = 0x400000;
        entity->animator2.frameID = 0;
        entity->active            = ACTIVE_BOUNDS;
    }
}

void SeltzerBottle_EditorDraw(void) {}

void SeltzerBottle_EditorLoad(void) {}

void SeltzerBottle_Serialize(void)
{
    RSDK_EDITABLE_VAR(SeltzerBottle, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(SeltzerBottle, VAR_ENUM, sprayTime);
}
