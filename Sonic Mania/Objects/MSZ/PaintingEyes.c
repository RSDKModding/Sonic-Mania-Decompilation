#include "SonicMania.h"

ObjectPaintingEyes *PaintingEyes;

void PaintingEyes_Update(void)
{
    RSDK_THIS(PaintingEyes);
    foreach_active(Player, player)
    {
        if ((player->direction && player->position.x >= entity->position.x - 0x80000)
            || (!player->direction && player->position.x <= entity->position.x + 0x80000)) {
            if (player->onGround) {
                entity->flag = false;
            }
            else if (entity->timer < 30) {
                entity->timer = 0;
            }
        }
        else {
            entity->flag = true;
        }
    }

    if (entity->flag) {
        if (entity->lookPos < 0x80000)
            entity->lookPos += 0x20000;
        entity->timer = 0;
    }
    else {
        if (entity->timer >= 30) {
            if (entity->lookPos > 0)
                entity->lookPos -= 0x20000;
        }
        else {
            entity->timer++;
        }
    }
}

void PaintingEyes_LateUpdate(void) {}

void PaintingEyes_StaticUpdate(void) {}

void PaintingEyes_Draw(void)
{
    RSDK_THIS(PaintingEyes);
    Vector2 drawPos;

    drawPos.x = clampVal(((RSDK_screens->position.x + RSDK_screens->centerX) << 10) - (entity->position.x >> 6), -0x10000, 0x10000)
                + entity->position.x;
    drawPos.y =
        clampVal(((RSDK_screens->position.y + RSDK_screens->centerY) << 10) - (entity->position.y >> 6), -0x10000, 0x10000) + entity->position.y;
    RSDK.DrawSprite(&entity->animator2, &drawPos, false);

    drawPos.x =
        clampVal(((RSDK_screens->position.x + RSDK_screens->centerX) << 11) - (entity->position.x >> 5), -0x10000, 0x20000) + entity->position.x;
    drawPos.y =
        clampVal(((RSDK_screens->position.y + RSDK_screens->centerY) << 11) - (entity->position.y >> 5), -0x10000, 0x20000) + entity->position.y;
    RSDK.DrawSprite(&entity->animator3, &drawPos, false);

    drawPos.x = entity->position.x;
    drawPos.y = entity->position.y + entity->lookPos;
    RSDK.DrawSprite(&entity->animator1, &drawPos, false);
}

void PaintingEyes_Create(void *data)
{
    RSDK_THIS(PaintingEyes);
    if (!RSDK_sceneInfo->inEditor) {
        RSDK.SetSpriteAnimation(PaintingEyes->aniFrames, 0, &entity->animator1, true, 0);
        RSDK.SetSpriteAnimation(PaintingEyes->aniFrames, 0, &entity->animator2, true, 1);
        RSDK.SetSpriteAnimation(PaintingEyes->aniFrames, 0, &entity->animator3, true, 2);
        entity->active        = ACTIVE_BOUNDS;
        entity->updateRange.x = 0x100000;
        entity->updateRange.y = 0x100000;
        entity->visible       = true;
        entity->drawOrder     = Zone->drawOrderLow - 1;
    }
}

void PaintingEyes_StageLoad(void) { PaintingEyes->aniFrames = RSDK.LoadSpriteAnimation("MSZ/PaintingEyes.bin", SCOPE_STAGE); }

void PaintingEyes_EditorDraw(void)
{
    RSDK_THIS(PaintingEyes);
    RSDK.SetSpriteAnimation(PaintingEyes->aniFrames, 0, &entity->animator1, false, 0);
    RSDK.SetSpriteAnimation(PaintingEyes->aniFrames, 0, &entity->animator2, false, 1);
    RSDK.SetSpriteAnimation(PaintingEyes->aniFrames, 0, &entity->animator3, false, 2);

    RSDK.DrawSprite(&entity->animator2, NULL, false);
    RSDK.DrawSprite(&entity->animator3, NULL, false);
    RSDK.DrawSprite(&entity->animator1, NULL, false);
}

void PaintingEyes_EditorLoad(void) { PaintingEyes->aniFrames = RSDK.LoadSpriteAnimation("MSZ/PaintingEyes.bin", SCOPE_STAGE); }

void PaintingEyes_Serialize(void) {}
