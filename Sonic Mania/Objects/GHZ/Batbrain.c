#include "SonicMania.h"

ObjectBatbrain *Batbrain;

void Batbrain_Update(void)
{
    RSDK_THIS(Batbrain);
    StateMachine_Run(entity->state);
}

void Batbrain_LateUpdate(void) {}

void Batbrain_StaticUpdate(void) {}

void Batbrain_Draw(void)
{
    RSDK_THIS(Batbrain);
    RSDK.DrawSprite(&entity->animator, NULL, false);
}

void Batbrain_Create(void *data)
{
    RSDK_THIS(Batbrain);
    entity->visible = true;
    entity->drawFX |= FX_FLIP;
    entity->drawOrder     = Zone->drawOrderLow;
    entity->startPos.x    = entity->position.x;
    entity->startPos.y    = entity->position.y;
    entity->active        = ACTIVE_BOUNDS;
    entity->updateRange.x = 0x800000;
    entity->updateRange.y = 0x800000;
    RSDK.SetSpriteAnimation(Batbrain->aniFrames, 0, &entity->animator, true, 0);
    entity->state = Batbrain_Unknown2;
}

void Batbrain_StageLoad(void)
{
    if (RSDK.CheckStageFolder("GHZ"))
        Batbrain->aniFrames = RSDK.LoadSpriteAnimation("GHZ/Batbrain.bin", SCOPE_STAGE);
    Batbrain->hitbox.left   = -12;
    Batbrain->hitbox.top    = -18;
    Batbrain->hitbox.right  = 12;
    Batbrain->hitbox.bottom = 18;
    Batbrain->sfxFlap       = RSDK.GetSFX("Stage/Flap.wav");

    DEBUGMODE_ADD_OBJ(Batbrain);
}

void Batbrain_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(Batbrain->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, 0, 0);
}

void Batbrain_DebugSpawn(void) { RSDK.CreateEntity(Batbrain->objectID, 0, RSDK_sceneInfo->entity->position.x, RSDK_sceneInfo->entity->position.y); }

void Batbrain_CheckHit(void)
{
    RSDK_THIS(Batbrain);
    foreach_active(Player, player)
    {
        if (Player_CheckBadnikHit(player, entity, &Batbrain->hitbox))
            Player_CheckBadnikBreak(entity, player, true);
    }
}

void Batbrain_CheckOnScreen(void)
{
    RSDK_THIS(Batbrain);
    if (!RSDK.CheckOnScreen(entity, NULL) && !RSDK.CheckPosOnScreen(&entity->startPos, &entity->updateRange)) {
        entity->position.x = entity->startPos.x;
        entity->position.y = entity->startPos.y;
        Batbrain_Create(NULL);
    }
}

void Batbrain_Unknown2(void)
{
    RSDK_THIS(Batbrain);
    entity->active     = ACTIVE_NORMAL;
    entity->velocity.x = 0;
    entity->velocity.y = 0;
    entity->state      = Batbrain_Unknown3;
    Batbrain_Unknown3();
}

void Batbrain_Unknown3(void)
{
    RSDK_THIS(Batbrain);
    int targetPos           = 0x7FFFFFFF;
    EntityPlayer *player1   = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    EntityPlayer *playerPtr = NULL;

    foreach_active(Player, player)
    {
        int distance = abs(player->position.x - entity->position.x);
        if (distance >= targetPos) {
            player1 = player;
        }
        else {
            player1   = player;
            targetPos = distance;
        }
        if (distance < 0x800000) {
            if (player->position.y >= entity->position.y && (!playerPtr || player->position.y < playerPtr->position.y))
                playerPtr = player;
        }
    }

    if (playerPtr) {
        player       = playerPtr;
        int distance = playerPtr->position.y - entity->position.y;
        if (distance >= 0 && distance <= 0x800000 && !RSDK.Rand(0, 8)) {
            entity->state   = Batbrain_Unknown4;
            entity->targetY = player->position.y;
            entity->target  = (Entity*)player;
            RSDK.SetSpriteAnimation(Batbrain->aniFrames, 1, &entity->animator, true, 0);
        }
    }
    else {
        player = player1;
    }
    entity->direction = player->position.x >= entity->position.x;
    Batbrain_CheckHit();
    Batbrain_CheckOnScreen();
}

void Batbrain_Unknown4(void)
{
    RSDK_THIS(Batbrain);
    entity->position.y += entity->velocity.y;
    entity->velocity.y += 0x1800;
    entity->direction = entity->target->position.x >= entity->position.x;

    if (entity->targetY - entity->position.y < 0x100000) {
        entity->velocity.y = 0;
        if (entity->direction == FLIP_NONE)
            entity->velocity.x = -0x10000;
        else
            entity->velocity.x = 0x10000;
        RSDK.SetSpriteAnimation(Batbrain->aniFrames, 2, &entity->animator, true, 0);
        entity->state = Batbrain_Unknown5;
    }
    RSDK.ProcessAnimation(&entity->animator);
    Batbrain_CheckHit();
    Batbrain_CheckOnScreen();
}

void Batbrain_Unknown5(void)
{
    RSDK_THIS(Batbrain);
    Entity *target = entity->target;
    entity->position.x += entity->velocity.x;

    if (abs(target->position.x - entity->position.x) >= 0x800000 && !RSDK.Rand(0, 8))
        entity->state = Batbrain_Unknown6;

    if (!(Zone->timer & 0xF))
        RSDK.PlaySFX(Batbrain->sfxFlap, 0, 255);

    RSDK.ProcessAnimation(&entity->animator);
    Batbrain_CheckHit();
    Batbrain_CheckOnScreen();
}

void Batbrain_Unknown6(void)
{
    RSDK_THIS(Batbrain);
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    entity->velocity.y -= 0x1800;
    if (RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_RWALL, 0, 0, -0xC0000, true)) {
        entity->velocity.x = 0;
        entity->velocity.y = 0;
        entity->state      = Batbrain_Unknown3;
        RSDK.SetSpriteAnimation(Batbrain->aniFrames, 0, &entity->animator, true, 0);
    }

    if (!(Zone->timer & 0xF))
        RSDK.PlaySFX(Batbrain->sfxFlap, 0, 255);

    RSDK.ProcessAnimation(&entity->animator);
    Batbrain_CheckHit();
    Batbrain_CheckOnScreen();
}

void Batbrain_EditorDraw(void) {}

void Batbrain_EditorLoad(void) {}

void Batbrain_Serialize(void) {}
