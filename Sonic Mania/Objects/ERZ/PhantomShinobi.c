#include "SonicMania.h"

ObjectPhantomShinobi *PhantomShinobi;

void PhantomShinobi_Update(void)
{
    RSDK_THIS(PhantomShinobi);
    if (entity->invincibilityTimer > 0)
        entity->invincibilityTimer--;
    StateMachine_Run(entity->state);

    entity->rotStore += entity->field_78;
    entity->rotStore &= 0xFFFF;
}

void PhantomShinobi_LateUpdate(void) {}

void PhantomShinobi_StaticUpdate(void) {}

void PhantomShinobi_Draw(void)
{
    RSDK_THIS(PhantomShinobi);

    RSDK.SetActivePalette(4, 0, RSDK_screens[RSDK_sceneInfo->currentScreenID].height);

    if (entity->invincibilityTimer & 1)
        RSDK.CopyPalette(6, 128, 4, 128, 128);
    entity->rotation = entity->rotStore >> 7;

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

    if (entity->invincibilityTimer & 1)
        RSDK.CopyPalette(5, 128, 4, 128, 128);

    RSDK.SetActivePalette(0, 0, RSDK_screens[RSDK_sceneInfo->currentScreenID].height);
}

void PhantomShinobi_Create(void *data)
{
    RSDK_THIS(PhantomShinobi);

    if (!RSDK_sceneInfo->inEditor) {
        entity->visible         = true;
        entity->drawFX          = FX_ROTATE | FX_FLIP;
        entity->drawOrder       = Zone->drawOrderLow;
        entity->active          = ACTIVE_NEVER;
        entity->updateRange.x   = 0x800000;
        entity->updateRange.y   = 0x800000;
        entity->collisionLayers = Zone->fgLayers;
        entity->startPos.x      = entity->position.x;
        entity->startPos.y      = entity->position.y;
        entity->tileCollisions  = true;
        entity->spearOffset     = 0x1600;
        PhantomShinobi_ResetStates();
        RSDK.SetSpriteAnimation(PhantomShinobi->aniFrames, 0, &entity->animator1, true, 0);
        RSDK.SetSpriteAnimation(PhantomShinobi->aniFrames, 0, &entity->animator2, true, 1);
        RSDK.SetSpriteAnimation(PhantomShinobi->aniFrames, 1, &entity->animator3, true, 0);
    }
}

void PhantomShinobi_StageLoad(void)
{
    PhantomShinobi->aniFrames = RSDK.LoadSpriteAnimation("Phantom/PhantomShinobi.bin", SCOPE_STAGE);

    PhantomShinobi->hitbox.left   = -24;
    PhantomShinobi->hitbox.top    = -24;
    PhantomShinobi->hitbox.right  = 24;
    PhantomShinobi->hitbox.bottom = 24;

    PhantomShinobi->sfxBlade = RSDK.GetSFX("TMZ3/ShinobiBlade.wav");
    PhantomShinobi->sfxHit   = RSDK.GetSFX("TMZ3/ShinobiHit.wav");

    Soundboard_LoadSFX("TMZ3/ShinobiBlade.wav", true, PhantomShinobi_BladeCheckCB, NULL);
}

void PhantomShinobi_CheckPlayerCollisions(void)
{
    RSDK_THIS(PhantomShinobi);

    foreach_active(Player, playerLoop)
    {
        if (!entity->invincibilityTimer && Player_CheckBadnikTouch(playerLoop, entity, &PhantomShinobi->hitbox)
            && Player_CheckBossHit(playerLoop, entity)) {
            entity->invincibilityTimer = 48;
            RSDK.PlaySfx(PhantomEgg->sfxHit, false, 255);
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

void PhantomShinobi_HandleTileCollisions(void)
{
    RSDK_THIS(PhantomShinobi);
    if (entity->onGround) {
        entity->spearOffset += (4096 - entity->spearOffset) >> 3;
        if (!entity->prevOnGround) {
            if (entity->field_7C > 0) {
                entity->velocity.y = -0x30000;
                entity->field_7C--;
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

void PhantomShinobi_ResetStates(void)
{
    RSDK_THIS(PhantomShinobi);

    entity->position.x = entity->startPos.x;
    entity->position.y = entity->startPos.y;
    entity->position.y -= 0x400000;
    entity->spearOffset = 0;
    entity->field_80    = 0;
    entity->state       = PhantomShinobi_State_Unknown1;
    entity->direction   = RSDK.Rand(0, 2);
}

void PhantomShinobi_State_Unknown1(void)
{
    RSDK_THIS(PhantomShinobi);

    if (entity->position.y < entity->startPos.y) {
        entity->position.y += 0x8000;
    }

    if (++entity->timer >= 128) {
        entity->spearOffset += 0x100;
        if (entity->spearOffset >= 0x1600) {
            entity->timer       = 0;
            entity->spearOffset = 0x1600;
            entity->state       = PhantomShinobi_State_Unknown2;
        }
    }
}

void PhantomShinobi_State_Unknown2(void)
{
    RSDK_THIS(PhantomShinobi);

    if (++entity->timer == 16) {
        entity->timer = 0;
        entity->state = PhantomShinobi_State_Unknown3;
    }
}

void PhantomShinobi_State_Unknown3(void)
{
    RSDK_THIS(PhantomShinobi);

    if (entity->direction) {
        if (entity->field_78 > -0x600) {
            entity->field_78 -= 12;
        }
        else {
            entity->velocity.x -= 0x20000;
            entity->field_78   = -0x600;
            entity->state      = PhantomShinobi_State_Unknown4;
            entity->onGround   = false;
            entity->field_7C   = 1;
            entity->velocity.y = -0x30000;
        }
    }
    else {
        if (entity->field_78 >= 0x600) {
            entity->field_78   = 0x600;
            entity->velocity.x = 0x20000;
            entity->state      = PhantomShinobi_State_Unknown4;
            entity->onGround   = false;
            entity->field_7C   = 1;
            entity->velocity.y = -0x30000;
        }
        else {
            entity->field_78 += 12;
        }
    }
    PhantomShinobi_CheckPlayerCollisions();
}

void PhantomShinobi_State_Unknown4(void)
{
    RSDK_THIS(PhantomShinobi);

    PhantomShinobi_HandleTileCollisions();
    if (entity->onGround && !entity->prevOnGround) {
        if (entity->direction == FLIP_NONE)
            entity->groundVel = 0x80000;
        else
            entity->groundVel = -0x80000;
    }
    if (!entity->groundVel) {
        entity->onGround   = false;
        entity->velocity.y = -0x40000;
        if (entity->direction == FLIP_NONE)
            entity->velocity.x = -0x38000;
        else
            entity->velocity.x = 0x38000;
        entity->field_7C = 1;
        entity->direction ^= FLIP_X;
        entity->state = PhantomShinobi_State_Unknown5;
    }
    PhantomShinobi_CheckPlayerCollisions();
}

void PhantomShinobi_State_Unknown5(void)
{
    RSDK_THIS(PhantomShinobi);

    if (entity->velocity.y < 0x20000) {
        entity->velocity.y += 0x2800;
        entity->position.x += entity->velocity.x;
        entity->position.y += entity->velocity.y;
    }

    if (abs(entity->field_78) <= 256) {
        if (!(entity->rotStore & 0x1C00)) {
            entity->field_78 = 0;
            entity->rotStore = 0;
            entity->timer    = 0;
            entity->state    = PhantomShinobi_State_Unknown6;
        }
    }
    else {
        entity->field_78 -= (entity->field_78 >> 5);
    }
    PhantomShinobi_CheckPlayerCollisions();
}

void PhantomShinobi_State_Unknown6(void)
{
    RSDK_THIS(PhantomShinobi);

    entity->spearOffset -= 128;
    if (++entity->timer == 24) {
        entity->timer      = 0;
        entity->velocity.x = 0x400;
        RSDK.PlaySfx(PhantomShinobi->sfxHit, false, 255);
        entity->state = PhantomShinobi_State_Unknown7;
    }
    PhantomShinobi_CheckPlayerCollisions();
}

void PhantomShinobi_State_Unknown7(void)
{
    RSDK_THIS(PhantomShinobi);

    entity->velocity.x -= 16;
    if (entity->velocity.x < -0xC00)
        entity->velocity.x = -0xC00;

    entity->spearOffset += entity->velocity.x;
    if (entity->velocity.x < 0 && entity->spearOffset < 0x1600) {
        entity->spearOffset = 0x1600;
        if (++entity->field_80 == 2) {
            PhantomEgg_SetupScanlineCB();
            entity->timer = 0;
            entity->state = PhantomShinobi_State_Unknown8;
        }
        else {
            entity->state = PhantomShinobi_State_Unknown2;
        }
    }
    PhantomShinobi_CheckPlayerCollisions();
}

void PhantomShinobi_State_Unknown8(void)
{
    RSDK_THIS(PhantomShinobi);

    if (entity->spearOffset > 0) {
        entity->spearOffset -= (entity->spearOffset >> 4);
    }

    if (++entity->timer == 60) {
        PhantomShinobi_ResetStates();
        entity->active = ACTIVE_NEVER;
    }
}

bool32 PhantomShinobi_BladeCheckCB(void)
{
    foreach_active(PhantomShinobi, shinobi)
    {
        if (shinobi->state == PhantomShinobi_State_Unknown4)
            foreach_return true;
    }
    return false;
}

#if RETRO_INCLUDE_EDITOR
void PhantomShinobi_EditorDraw(void)
{
    RSDK_THIS(PhantomShinobi);

    entity->spearOffset = 0x1600;
    RSDK.SetSpriteAnimation(PhantomShinobi->aniFrames, 0, &entity->animator1, false, 0);
    RSDK.SetSpriteAnimation(PhantomShinobi->aniFrames, 0, &entity->animator2, false, 1);
    RSDK.SetSpriteAnimation(PhantomShinobi->aniFrames, 1, &entity->animator3, false, 0);

    PhantomShinobi_Draw();
}

void PhantomShinobi_EditorLoad(void) { PhantomShinobi->aniFrames = RSDK.LoadSpriteAnimation("Phantom/PhantomShinobi.bin", SCOPE_STAGE); }
#endif

void PhantomShinobi_Serialize(void) {}
