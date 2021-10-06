#include "SonicMania.h"

ObjectSplats *Splats;

void Splats_Update(void)
{
    RSDK_THIS(Splats);
    StateMachine_Run(entity->state);
}

void Splats_LateUpdate(void) {}

void Splats_StaticUpdate(void) {}

void Splats_Draw(void)
{
    RSDK_THIS(Splats);
    RSDK.DrawSprite(&entity->animator2, NULL, false);
    RSDK.DrawSprite(&entity->animator1, NULL, false);
}

void Splats_Create(void *data)
{
    RSDK_THIS(Splats);

    entity->startPos.x    = entity->position.x;
    entity->startPos.y    = entity->position.y;
    entity->startDir      = entity->direction;
    entity->visible       = true;
    entity->active        = ACTIVE_BOUNDS;
    entity->updateRange.x = 0x800000;
    entity->updateRange.y = 0x800000;
    if (entity->bounceCount < 1)
        entity->bounceCount = 1;

    if (Splats->state == Splats_Unknown3) {
        entity->drawFX |= FX_FLIP;
        entity->drawOrder = Zone->drawOrderLow;
        RSDK.SetSpriteAnimation(Splats->aniFrames, 0, &entity->animator1, true, 0);
        entity->state = Splats_Unknown2;
    }
    else {
        switch (voidToInt(data)) {
            case 0:
                entity->drawOrder = Zone->drawOrderHigh;
                entity->delay     = 0;
                RSDK.SetSpriteAnimation(Splats->aniFrames, 1, &entity->animator1, true, 0);
                entity->state = Splats_Unknown2;
                break;
            case 1:
                entity->drawFX |= FX_FLIP;
                entity->drawOrder = Zone->drawOrderLow;
                entity->active    = ACTIVE_NORMAL;
                RSDK.SetSpriteAnimation(Splats->aniFrames, 0, &entity->animator1, true, 0);
                entity->animator1.loopIndex  = 1;
                entity->state                = Splats_Unknown6;
                entity->animator1.frameCount = 2;
                break;
            case 2:
                entity->drawFX |= FX_FLIP;
                entity->inkEffect |= INK_ALPHA;
                entity->alpha     = 256;
                entity->drawOrder = Zone->drawOrderHigh;
                entity->active    = ACTIVE_NORMAL;
                RSDK.SetSpriteAnimation(Splats->aniFrames, 3, &entity->animator1, true, 0);
                entity->state = Splats_Unknown10;
                break;
        }
    }
}

void Splats_StageLoad(void)
{
    if (RSDK.CheckStageFolder("GHZ")) {
        Splats->aniFrames = RSDK.LoadSpriteAnimation("GHZ/Splats.bin", SCOPE_STAGE);
        Splats->state     = Splats_Unknown3;
    }
    else if (RSDK.CheckStageFolder("PSZ1")) {
        Splats->aniFrames = RSDK.LoadSpriteAnimation("PSZ1/Splats.bin", SCOPE_STAGE);
        Splats->state     = Splats_Unknown5;
    }
    Splats->hitbox1.left   = -10;
    Splats->hitbox1.top    = -20;
    Splats->hitbox1.right  = 6;
    Splats->hitbox1.bottom = 20;
    Splats->hitbox2.left   = -18;
    Splats->hitbox2.top    = -26;
    Splats->hitbox2.right  = 18;
    Splats->hitbox2.bottom = 17;
    Splats->hitbox3.left   = -18;
    Splats->hitbox3.top    = -146;
    Splats->hitbox3.right  = 18;
    Splats->hitbox3.bottom = -18;

    DEBUGMODE_ADD_OBJ(Splats);
    Splats->sfxSplatsSpawn = RSDK.GetSFX("PSZ/SplatsSpawn.wav");
    Splats->sfxSplatsLand  = RSDK.GetSFX("PSZ/SplatsLand.wav");
}

void Splats_DebugDraw(void)
{
    if (Splats->state == Splats_Unknown3)
        RSDK.SetSpriteAnimation(Splats->aniFrames, 0, &DebugMode->animator, true, 0);
    else
        RSDK.SetSpriteAnimation(Splats->aniFrames, 1, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, 0, 0);
}

void Splats_DebugSpawn(void) { RSDK.CreateEntity(Splats->objectID, 0, RSDK_sceneInfo->entity->position.x, RSDK_sceneInfo->entity->position.y); }

void Splats_CheckPlayerCollisions(void)
{
    RSDK_THIS(Splats);

    foreach_active(Player, player)
    {
        if (Player_CheckBadnikHit(player, entity, &Splats->hitbox1)) {
            if (Splats->state == Splats_Unknown3) {
                Player_CheckBadnikBreak(entity, player, true);
            }
            else {
                if (Player_CheckBadnikBreak(entity, player, false)) {
                    EntitySplats *parent = (EntitySplats *)entity->parent;
                    if (parent) {
                        if (parent->objectID == Splats->objectID)
                            --parent->activeCount;
                    }
                    RSDK.ResetEntityPtr(entity, TYPE_BLANK, NULL);
                    foreach_break;
                }
            }
        }
    }
}

void Splats_CheckOnScreen(void)
{
    RSDK_THIS(Splats);
    if (!RSDK.CheckOnScreen(RSDK_sceneInfo->entity, 0) && !RSDK.CheckPosOnScreen(&entity->startPos, &entity->updateRange)) {
        entity->position.x = entity->startPos.x;
        entity->position.y = entity->startPos.y;
        entity->direction  = entity->startDir;
        entity->field_68   = 0;
        Splats_Create(NULL);
    }
}

void Splats_Unknown2(void)
{
    RSDK_THIS(Splats);
    entity->active     = ACTIVE_NORMAL;
    entity->velocity.x = -0x10000;
    entity->state      = Splats->state;
    StateMachine_Run(entity->state);
}

void Splats_Unknown3(void)
{
    RSDK_THIS(Splats);
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    entity->velocity.y += 0x3800;

    if (entity->velocity.y > 0 && RSDK.ObjectTileCollision(entity, Zone->fgLayers, 0, 0, 0, 0x100000, true)) {
        if (entity->bounceCount) {
            if (++entity->activeCount >= entity->bounceCount) {
                entity->activeCount = 0;
                entity->direction ^= FLIP_X;
                entity->velocity.x = -entity->velocity.x;
            }
        }
        entity->position.y -= 0x80000;
        entity->velocity.y = -0x40000;
    }
    entity->animator1.frameID = entity->velocity.y < 0;
    Splats_CheckPlayerCollisions();
    Splats_CheckOnScreen();
}

void Splats_Unknown4(void)
{
    RSDK_THIS(Splats);
    entity->field_68 = 0;
    entity->delay    = 0;
    entity->active   = ACTIVE_NORMAL;
    entity->state    = Splats_Unknown5;
    Splats_Unknown5();
}

void Splats_Unknown5(void)
{
    RSDK_THIS(Splats);
    if (!entity->field_68) {
        if (!entity->activeScreens)
            return;
        entity->field_68 = true;
    }
    RSDK.ProcessAnimation(&entity->animator2);

    bool32 flag = false;
    foreach_active(Player, player)
    {
        Player_CheckCollisionBox(player, entity, &Splats->hitbox2);
        flag |= Player_CheckCollisionTouch(player, entity, &Splats->hitbox3);
        flag |= (player->playerAnimator.animationID == ANI_SPINDASH);
    }

    if (--entity->delay <= 0) {
        if (flag || (entity->activeCount >= entity->numActive)) {
            entity->delay++;
        }
        else {
            RSDK.SetSpriteAnimation(Splats->aniFrames, 2, &entity->animator2, true, 0);
            entity->delay = entity->minDelay;
            RSDK.PlaySfx(Splats->sfxSplatsSpawn, 0, 255);
            EntitySplats *child = (EntitySplats *)RSDK.CreateEntity(Splats->objectID, intToVoid(1), entity->position.x, entity->position.y - 0x60000);
            child->parent       = (Entity *)entity;
            child->bounceCount  = entity->bounceCount;
            child->direction    = entity->direction;
            child->velocity.y   = -0x60000;
            if (entity->direction == FLIP_NONE)
                child->velocity.x = -0x10000;
            else
                child->velocity.x = 0x10000;
            ++entity->activeCount;
        }
    }

    if (!RSDK.CheckOnScreen(entity, NULL)) {
        entity->state  = Splats_Unknown4;
        entity->active = ACTIVE_NORMAL;
    }
}

void Splats_Unknown6(void)
{
    RSDK_THIS(Splats);
    RSDK.ProcessAnimation(&entity->animator1);
    entity->position.y += entity->velocity.y;
    entity->velocity.y += 0x3800;

    if (entity->velocity.y > -0x40000)
        entity->position.x += entity->velocity.x;

    if (entity->velocity.y > 0) {
        entity->animator1.loopIndex  = 4;
        entity->animator1.frameCount = 5;
        if (RSDK.ObjectTileCollision(entity, Zone->fgLayers, 0, 0, 0, 0x120000, true)) {
            RSDK.PlaySfx(Splats->sfxSplatsLand, 0, 255);
            EntitySplats *splats = (EntitySplats *)RSDK.CreateEntity(Splats->objectID, intToVoid(2), entity->position.x, entity->position.y);
            splats->direction    = entity->direction;
            entity->delay        = 4;
            entity->state        = Splats_Unknown8;
        }
    }
    Splats_CheckPlayerCollisions();
}

void Splats_Unknown7(void)
{
    RSDK_THIS(Splats);

    RSDK.ProcessAnimation(&entity->animator1);
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    entity->velocity.y += 0x3800;
    if (entity->velocity.y > 0) {
        entity->animator1.loopIndex  = 4;
        entity->animator1.frameCount = 5;
        if (RSDK.ObjectTileCollision(entity, Zone->fgLayers, 0, 0, 0, 0x120000, true)) {
            RSDK.PlaySfx(Splats->sfxSplatsLand, 0, 0xFF);
            if (entity->bounceCount) {
                if (++entity->activeCount < entity->bounceCount) {
                    entity->delay = 4;
                    entity->state = Splats_Unknown8;
                }
                else {
                    RSDK.SetSpriteAnimation(Splats->aniFrames, 4, &entity->animator1, true, 0);
                    entity->state       = Splats_Unknown9;
                    entity->activeCount = 0;
                }
            }

            EntitySplats *splats = (EntitySplats *)RSDK.CreateEntity(Splats->objectID, intToVoid(2), entity->position.x, entity->position.y);
            splats->direction    = entity->direction;
        }
    }

    if (RSDK.CheckOnScreen(entity, NULL)) {
        Splats_CheckPlayerCollisions();
    }
    else {
        EntitySplats *parent = (EntitySplats *)entity->parent;
        if (parent) {
            if (parent->objectID == Splats->objectID)
                --parent->activeCount;
        }
        RSDK.ResetEntityPtr(entity, TYPE_BLANK, NULL);
    }
}

void Splats_Unknown8(void)
{
    RSDK_THIS(Splats);
    if (--entity->delay <= 0) {
        RSDK.SetSpriteAnimation(Splats->aniFrames, 0, &entity->animator1, true, 0);
        entity->animator1.loopIndex  = 1;
        entity->animator1.frameCount = 2;
        entity->position.y -= 0x80000;
        entity->velocity.y = -0x40000;
        entity->state      = Splats_Unknown7;
    }

    if (RSDK.CheckOnScreen(entity, NULL)) {
        Splats_CheckPlayerCollisions();
    }
    else {
        EntitySplats *parent = (EntitySplats *)entity->parent;
        if (parent) {
            if (parent->objectID == Splats->objectID)
                --parent->activeCount;
        }
        RSDK.ResetEntityPtr(entity, TYPE_BLANK, NULL);
    }
}

void Splats_Unknown9(void)
{
    RSDK_THIS(Splats);

    RSDK.ProcessAnimation(&entity->animator1);
    if (entity->animator1.frameID == entity->animator1.frameCount - 1) {
        RSDK.SetSpriteAnimation(Splats->aniFrames, 0, &entity->animator1, true, 0);
        entity->position.y -= 0x80000;
        entity->animator1.frameCount = 2;
        entity->animator1.loopIndex  = 1;
        entity->direction ^= FLIP_X;
        entity->velocity.y = -0x40000;
        entity->velocity.x = -entity->velocity.x;
        entity->state      = Splats_Unknown7;
    }

    if (RSDK.CheckOnScreen(entity, NULL)) {
        Splats_CheckPlayerCollisions();
    }
    else {
        EntitySplats *parent = (EntitySplats *)entity->parent;
        if (parent) {
            if (parent->objectID == Splats->objectID)
                --parent->activeCount;
        }
        RSDK.ResetEntityPtr(entity, TYPE_BLANK, NULL);
    }
}

void Splats_Unknown10(void)
{
    RSDK_THIS(Splats);
    RSDK.ProcessAnimation(&entity->animator1);

    if (entity->delay >= 30) {
        entity->alpha -= 2;
        if (!entity->alpha)
            RSDK.ResetEntityPtr(entity, TYPE_BLANK, NULL);
    }
    else {
        entity->delay++;
    }
}

void Splats_EditorDraw(void) {}

void Splats_EditorLoad(void) {}

void Splats_Serialize(void)
{
    RSDK_EDITABLE_VAR(Splats, VAR_UINT8, bounceCount);
    RSDK_EDITABLE_VAR(Splats, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(Splats, VAR_UINT8, numActive);
    RSDK_EDITABLE_VAR(Splats, VAR_UINT16, minDelay);
}
