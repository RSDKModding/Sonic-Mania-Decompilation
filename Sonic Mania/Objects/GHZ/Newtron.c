#include "SonicMania.h"

ObjectNewtron *Newtron;

void Newtron_Update(void)
{
    RSDK_THIS(Newtron);
    StateMachine_Run(entity->state);
}

void Newtron_LateUpdate(void) {}

void Newtron_StaticUpdate(void) {}

void Newtron_Draw(void)
{
    RSDK_THIS(Newtron);
    RSDK.DrawSprite(&entity->animator1, NULL, false);
    if (entity->state == Newtron_Unknown8)
        RSDK.DrawSprite(&entity->animator2, NULL, false);
}

void Newtron_Create(void *data)
{
    RSDK_THIS(Newtron);
    if (!RSDK_sceneInfo->inEditor) {
        entity->drawFX |= FX_FLIP;
        entity->startPos.x = entity->position.x;
        entity->startPos.y = entity->position.y;
        entity->visible    = true;
        if (data == (void *)1) {
            entity->type          = 1;
            entity->active        = ACTIVE_BOUNDS;
            entity->updateRange.x = 0x800000;
            entity->updateRange.y = 0x800000;
        }
        else if (data) {
            entity->inkEffect = INK_ADD;
            entity->alpha     = 0xC0;
            RSDK.SetSpriteAnimation(Newtron->aniFrames, 6, &entity->animator1, true, 0);
            entity->state         = Newtron_State_Projectile;
            entity->active        = ACTIVE_NORMAL;
            entity->updateRange.x = 0x200000;
            entity->updateRange.y = 0x200000;
            entity->drawOrder     = Zone->drawOrderHigh;
            return;
        }
        entity->inkEffect     = INK_ALPHA;
        entity->alpha         = 0x00;
        entity->active        = ACTIVE_BOUNDS;
        entity->updateRange.x = 0x800000;
        entity->updateRange.y = 0x800000;
        if (entity->type == 1) {
            RSDK.SetSpriteAnimation(Newtron->aniFrames, 2, &entity->animator1, true, 0);
            entity->drawOrder = Zone->drawOrderLow;
        }
        else {
            RSDK.SetSpriteAnimation(Newtron->aniFrames, 0, &entity->animator1, true, 0);
            entity->drawOrder = Zone->drawOrderHigh;
        }
        RSDK.SetSpriteAnimation(Newtron->aniFrames, 5, &entity->animator2, true, 0);
        entity->state = Newtron_Unknown4;
    }
}

void Newtron_StageLoad(void)
{
    if (RSDK.CheckStageFolder("GHZ"))
        Newtron->aniFrames = RSDK.LoadSpriteAnimation("GHZ/Newtron.bin", SCOPE_STAGE);
    Newtron->hitbox1.left   = -12;
    Newtron->hitbox1.top    = -14;
    Newtron->hitbox1.right  = 12;
    Newtron->hitbox1.bottom = 14;
    Newtron->hitbox2.left   = -16;
    Newtron->hitbox2.top    = -8;
    Newtron->hitbox2.right  = 16;
    Newtron->hitbox2.bottom = 8;
    Newtron->hitbox3.left   = -6;
    Newtron->hitbox3.top    = -6;
    Newtron->hitbox3.right  = 6;
    Newtron->hitbox3.bottom = 6;
    Newtron->hitbox4.left   = -128;
    Newtron->hitbox4.top    = -64;
    Newtron->hitbox4.right  = 128;
    Newtron->hitbox4.bottom = 64;

    DEBUGMODE_ADD_OBJ(Newtron);
}

void Newtron_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(Newtron->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
}

void Newtron_DebugSpawn(void)
{
    RSDK_THIS(DebugMode);
    CREATE_ENTITY(Newtron, NULL, entity->position.x, entity->position.y);
}

void Newtron_CheckHit(void)
{
    RSDK_THIS(Newtron);
    foreach_active(Player, player)
    {
        if (Player_CheckBadnikTouch(player, entity, &Newtron->hitbox1))
            Player_CheckBadnikBreak(entity, player, true);
    }
}

void Newtron_CheckOnScreen(void)
{
    RSDK_THIS(Newtron);
    if (!RSDK.CheckOnScreen(RSDK_sceneInfo->entity, NULL) && !RSDK.CheckPosOnScreen(&entity->startPos, &entity->updateRange)) {
        entity->position.x = entity->startPos.x;
        entity->position.y = entity->startPos.y;
        entity->velocity.x = 0;
        entity->velocity.y = 0;
        entity->timer      = 0;
        Newtron_Create(NULL);
    }
}

void Newtron_Unknown3(void)
{
    RSDK_THIS(Newtron);

    EntityPlayer *playerPtr = NULL;
    foreach_active(Player, player)
    {
        if (playerPtr) {
            if (abs(player->position.x - entity->position.x) < abs(playerPtr->position.x - entity->position.x))
                playerPtr = player;
        }
        else {
            playerPtr = player;
        }
    }

    if (playerPtr) {
        entity->direction = playerPtr->position.x < entity->position.x;
    }
}

void Newtron_Unknown4(void)
{
    RSDK_THIS(Newtron);
    entity->active = ACTIVE_NORMAL;
    entity->state  = Newtron_Unknown5;
    Newtron_Unknown5();
}

void Newtron_Unknown5(void)
{
    RSDK_THIS(Newtron);
    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, entity, &Newtron->hitbox4)) {
            entity->state = Newtron_Unknown6;
        }
    }
    Newtron_CheckOnScreen();
}

void Newtron_Unknown6(void)
{
    RSDK_THIS(Newtron);
    if (entity->alpha >= 0xF8) {
        entity->alpha = 0xFF;
        if (entity->type == 1) {
            entity->state = Newtron_Unknown7;
            RSDK.SetSpriteAnimation(Newtron->aniFrames, 3, &entity->animator1, true, 0);
            RSDK.ProcessAnimation(&entity->animator1);
            Newtron_CheckOnScreen();
            return;
        }
        entity->state = Newtron_Unknown9;
    }
    else {
        entity->alpha += 4;
        if (entity->type == 1) {
            Newtron_Unknown3();
            RSDK.ProcessAnimation(&entity->animator1);
            Newtron_CheckOnScreen();
            return;
        }
    }
    RSDK.ProcessAnimation(&entity->animator1);
    Newtron_CheckOnScreen();
}

void Newtron_Unknown7(void)
{
    RSDK_THIS(Newtron);
    Newtron_Unknown3();
    if (entity->animator1.frameID >= 2) {
        entity->position.y += entity->velocity.y;
        entity->velocity.y += 0x3800;
        if (RSDK.ObjectTileGrip(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x80000, 8)) {
            entity->velocity.y = 0;
            if (entity->direction == FLIP_NONE)
                entity->velocity.x = 0x20000;
            else
                entity->velocity.x = -0x20000;
            RSDK.SetSpriteAnimation(Newtron->aniFrames, 4, &entity->animator1, true, 0);
            entity->state = Newtron_Unknown8;
        }
    }
    RSDK.ProcessAnimation(&entity->animator1);
    Newtron_CheckHit();
    Newtron_CheckOnScreen();
}

void Newtron_Unknown8(void)
{
    RSDK_THIS(Newtron);
    entity->position.x += entity->velocity.x;
    RSDK.ObjectTileGrip(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x80000, 8);
    RSDK.ProcessAnimation(&entity->animator1);
    RSDK.ProcessAnimation(&entity->animator2);
    Newtron_CheckHit();
    Newtron_CheckOnScreen();
}

void Newtron_Unknown9(void)
{
    RSDK_THIS(Newtron);
    switch (++entity->timer) {
        case 30:
            RSDK.SetSpriteAnimation(Newtron->aniFrames, 1, &entity->animator1, true, 0);
            if (entity->direction)
                RSDK.CreateEntity(Newtron->objectID, (void *)2, entity->position.x - 0x140000, entity->position.y - 0x80000)->velocity.x = -0x20000;
            else
                RSDK.CreateEntity(Newtron->objectID, (void *)2, entity->position.x + 0x140000, entity->position.y - 0x80000)->velocity.x = 0x20000;
            break;
        case 45: RSDK.SetSpriteAnimation(Newtron->aniFrames, 0, &entity->animator1, true, 0); break;
        case 90: entity->state = Newtron_Unknown10; break;
    }
    RSDK.ProcessAnimation(&entity->animator1);
    Newtron_CheckHit();
    Newtron_CheckOnScreen();
}

void Newtron_Unknown10(void)
{
    RSDK_THIS(Newtron);
    RSDK.ProcessAnimation(&entity->animator1);
    Newtron_CheckOnScreen();
    if (entity->alpha <= 0)
        RSDK.ResetEntityPtr(entity, TYPE_BLANK, NULL);
    entity->alpha -= 4;
}

void Newtron_State_Projectile(void)
{
    RSDK_THIS(Newtron);
    entity->position.x += entity->velocity.x;
    if (!RSDK.CheckOnScreen(entity, NULL)) {
        RSDK.ResetEntityPtr(entity, TYPE_BLANK, NULL);
    }
    else {
        RSDK.ProcessAnimation(&entity->animator1);
        foreach_active(Player, player)
        {
            if (Player_CheckCollisionTouch(player, entity, &Newtron->hitbox3)) {
                Player_CheckProjectileHit(player, entity);
            }
        }
    }
}

void Newtron_EditorDraw(void)
{
    RSDK_THIS(Newtron);
    if (entity->type == 1)
        RSDK.SetSpriteAnimation(Newtron->aniFrames, 2, &entity->animator1, true, 0);
    else 
        RSDK.SetSpriteAnimation(Newtron->aniFrames, 0, &entity->animator1, true, 0);
    RSDK.SetSpriteAnimation(Newtron->aniFrames, 5, &entity->animator2, true, 0);
    
    Newtron_Draw();
}

void Newtron_EditorLoad(void) { Newtron->aniFrames = RSDK.LoadSpriteAnimation("GHZ/Newtron.bin", SCOPE_STAGE); }

void Newtron_Serialize(void)
{
    RSDK_EDITABLE_VAR(Newtron, VAR_UINT8, type);
    RSDK_EDITABLE_VAR(Newtron, VAR_UINT8, direction);
}
