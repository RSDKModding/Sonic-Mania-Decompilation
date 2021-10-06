#include "SonicMania.h"

ObjectERZShinobi *ERZShinobi;

void ERZShinobi_Update(void)
{
    RSDK_THIS(ERZShinobi);
    if (entity->invincibilityTimer > 0)
        entity->invincibilityTimer--;
    StateMachine_Run(entity->state);
    entity->rotStore = (entity->rotStore + 8) & 0x1FF;
}

void ERZShinobi_LateUpdate(void) {}

void ERZShinobi_StaticUpdate(void) {}

void ERZShinobi_Draw(void)
{
    RSDK_THIS(ERZShinobi);
    if ((entity->invincibilityTimer & 1))
        RSDK.CopyPalette(2, 128, 0, 128, 128);
    entity->rotation = entity->rotStore;

    for (int i = 0; i < 8; ++i) {
        Vector2 drawPos;

        drawPos.x                 = entity->field_6C * RSDK.Sin512(entity->rotation) + entity->position.x;
        drawPos.y                 = entity->position.y - entity->field_6C * RSDK.Cos512(entity->rotation);
        entity->animator3.frameID = ((entity->rotation + 16) >> 5) & 0xF;
        RSDK.DrawSprite(&entity->animator3, &drawPos, false);

        drawPos.x = ((5 * entity->field_6C * RSDK.Sin512(entity->rotation)) >> 3) + entity->position.x;
        drawPos.y = entity->position.y - ((5 * entity->field_6C * RSDK.Cos512(entity->rotation)) >> 3);
        RSDK.DrawSprite(&entity->animator2, &drawPos, false);

        entity->rotation += 64;
    }

    RSDK.DrawSprite(&entity->animator1, NULL, false);
    if ((entity->invincibilityTimer & 1))
        RSDK.CopyPalette(1, 128, 0, 128, 128);
}

void ERZShinobi_Create(void *data)
{
    RSDK_THIS(ERZShinobi);
    if (!RSDK_sceneInfo->inEditor) {
        entity->visible         = true;
        entity->drawFX          = FX_ROTATE | FX_FLIP;
        entity->drawOrder       = Zone->drawOrderLow;
        entity->active          = ACTIVE_NORMAL;
        entity->updateRange.x   = 0x800000;
        entity->updateRange.y   = 0x800000;
        entity->collisionLayers = Zone->fgLayers;
        entity->tileCollisions  = true;
        entity->field_6C        = 0x1600;
        entity->state           = ERZShinobi_Unknown3;
        RSDK.SetSpriteAnimation(ERZShinobi->aniFrames, 0, &entity->animator1, true, 0);
        RSDK.SetSpriteAnimation(ERZShinobi->aniFrames, 0, &entity->animator2, true, 1);
        RSDK.SetSpriteAnimation(ERZShinobi->aniFrames, 1, &entity->animator3, true, 0);
    }
}

void ERZShinobi_StageLoad(void)
{
    ERZShinobi->aniFrames = RSDK.LoadSpriteAnimation("Phantom/PhantomShinobi.bin", SCOPE_STAGE);

    ERZShinobi->hitbox.left   = -24;
    ERZShinobi->hitbox.top    = -24;
    ERZShinobi->hitbox.right  = 24;
    ERZShinobi->hitbox.bottom = 24;
}

void ERZShinobi_CheckHit(void)
{
    RSDK_THIS(ERZShinobi);

    foreach_active(Player, playerLoop)
    {
        if (!entity->invincibilityTimer && Player_CheckBadnikHit(playerLoop, entity, &ERZShinobi->hitbox)
            && Player_CheckBossHit(playerLoop, entity)) {
            entity->invincibilityTimer = 48;
            // RSDK.PlaySfx(ERZKing->sfxHit, false, 255);
            foreach_return;
        }
    }

    int storeX = entity->position.x;
    int storeY = entity->position.y;
    int angle  = entity->rotStore;
    foreach_active(Player, player)
    {
        entity->position.x = storeX + entity->field_6C * RSDK.Sin512(angle);
        entity->position.y = storeY - entity->field_6C * RSDK.Cos512(angle);
        if (RSDK.CheckObjectCollisionTouchCircle(player, 0xC0000, entity, 0x80000)) {
            Player_CheckHit(player, entity);
        }
        angle += 64;
    }

    entity->position.x = storeX;
    entity->position.y = storeY;
}

void ERZShinobi_HandleTileCollisions(void)
{
    RSDK_THIS(ERZShinobi);
    if (entity->onGround) {
        entity->field_6C += (4096 - entity->field_6C) >> 3;
        if (!entity->field_7C) {
            if (entity->field_78 > 0) {
                entity->velocity.y = -0x30000;
                entity->field_78--;
                entity->onGround = false;
            }
        }
    }
    else {
        entity->velocity.y += 0x3800;
        if (entity->velocity.y > 0xC0000)
            entity->velocity.y = 0xC0000;
        entity->field_6C += (0x1600 - entity->field_6C) >> 3;
    }

    int val                 = entity->field_6C / 88;
    entity->outerBox.right  = val;
    entity->outerBox.bottom = val;
    entity->outerBox.left   = -val;
    entity->outerBox.top    = -val;
    entity->innerBox.left   = 2 - val;
    entity->innerBox.right  = val - 2;
    entity->innerBox.top    = -val;
    entity->innerBox.bottom = val;

    entity->field_7C = entity->onGround;
    RSDK.ProcessTileCollisions(entity, &entity->outerBox, &entity->innerBox);
}

void ERZShinobi_Unknown3(void)
{
    RSDK_THIS(ERZShinobi);
    ERZShinobi_HandleTileCollisions();
    if (++entity->timer == 60) {
        entity->timer = 0;
        if (entity->onGround) {
            entity->field_78   = 2;
            entity->velocity.y = -0x80000;
            entity->onGround   = false;
        }
    }
    ERZShinobi_CheckHit();
}

void ERZShinobi_EditorDraw(void) {}

void ERZShinobi_EditorLoad(void) {}

void ERZShinobi_Serialize(void) {}
