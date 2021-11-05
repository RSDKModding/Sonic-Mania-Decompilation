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

    for (int32 i = 0; i < 8; ++i) {
        Vector2 drawPos;

        drawPos.x                 = entity->spearOffset * RSDK.Sin512(entity->rotation) + entity->position.x;
        drawPos.y                 = entity->position.y - entity->spearOffset * RSDK.Cos512(entity->rotation);
        entity->animator3.frameID = ((entity->rotation + 16) >> 5) & 0xF;
        RSDK.DrawSprite(&entity->animator3, &drawPos, false);

        drawPos.x = ((5 * entity->spearOffset * RSDK.Sin512(entity->rotation)) >> 3) + entity->position.x;
        drawPos.y = entity->position.y - ((5 * entity->spearOffset * RSDK.Cos512(entity->rotation)) >> 3);
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
        entity->spearOffset     = 0x1600;
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

void ERZShinobi_CheckPlayerCollisions(void)
{
    RSDK_THIS(ERZShinobi);

    foreach_active(Player, playerLoop)
    {
        if (!entity->invincibilityTimer && Player_CheckBadnikTouch(playerLoop, entity, &ERZShinobi->hitbox)
            && Player_CheckBossHit(playerLoop, entity)) {
            entity->invincibilityTimer = 48;
            RSDK.PlaySfx(ERZKing->sfxHit, false, 255);
            foreach_return;
        }
    }

    int32 storeX = entity->position.x;
    int32 storeY = entity->position.y;
    int32 angle  = entity->rotStore;
    foreach_active(Player, player)
    {
        entity->position.x = storeX + entity->spearOffset * RSDK.Sin512(angle);
        entity->position.y = storeY - entity->spearOffset * RSDK.Cos512(angle);
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
        entity->spearOffset += (4096 - entity->spearOffset) >> 3;
        if (!entity->prevOnGround) {
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
        entity->spearOffset += (0x1600 - entity->spearOffset) >> 3;
    }

    int32 val               = entity->spearOffset / 88;
    entity->outerBox.right  = val;
    entity->outerBox.bottom = val;
    entity->outerBox.left   = -val;
    entity->outerBox.top    = -val;
    entity->innerBox.left   = 2 - val;
    entity->innerBox.right  = val - 2;
    entity->innerBox.top    = -val;
    entity->innerBox.bottom = val;

    entity->prevOnGround = entity->onGround;
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
    ERZShinobi_CheckPlayerCollisions();
}

#if RETRO_INCLUDE_EDITOR
void ERZShinobi_EditorDraw(void)
{
    RSDK_THIS(ERZShinobi);

    entity->spearOffset = 0x1600;
    RSDK.SetSpriteAnimation(ERZShinobi->aniFrames, 0, &entity->animator1, false, 0);
    RSDK.SetSpriteAnimation(ERZShinobi->aniFrames, 0, &entity->animator2, false, 1);
    RSDK.SetSpriteAnimation(ERZShinobi->aniFrames, 1, &entity->animator3, false, 0);

    ERZShinobi_Draw();
}

void ERZShinobi_EditorLoad(void) { ERZShinobi->aniFrames = RSDK.LoadSpriteAnimation("Phantom/PhantomShinobi.bin", SCOPE_STAGE); }
#endif

void ERZShinobi_Serialize(void) {}
