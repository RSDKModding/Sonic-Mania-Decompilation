#include "SonicMania.h"

ObjectTurboSpiker *TurboSpiker;

// back in 3mixed, i was assigned this object
// i was never able to get it properly working
// here it is. :smirk2: -rmg

void TurboSpiker_Update(void)
{
    RSDK_THIS(TurboSpiker);
    StateMachine_Run(entity->state);
}

void TurboSpiker_LateUpdate(void) {}

void TurboSpiker_StaticUpdate(void) {}

void TurboSpiker_Draw(void)
{
    RSDK_THIS(TurboSpiker);
    RSDK.DrawSprite(&entity->animator, 0, false);
    RSDK.DrawSprite(&entity->spikeAnimator, 0, false);
}

void TurboSpiker_Create(void *data)
{
    RSDK_THIS(TurboSpiker);
    entity->launchPlayed = false;
    entity->visible      = true;
    entity->drawFX |= FX_FLIP;
    entity->drawOrder     = Zone->drawOrderLow + 1;
    entity->spawnPos      = entity->position;
    entity->active        = ACTIVE_BOUNDS;
    entity->updateRange.x = 0x800000;
    entity->updateRange.y = 0x800000;
    entity->spawnDir      = entity->direction;
    if (data)
        entity->active = ACTIVE_NORMAL;
    else {
        entity->unused_64 = 0x10;
        RSDK.SetSpriteAnimation(TurboSpiker->animID, 1, &entity->animator, true, 0);
        entity->state = TurboSpiker_Hermit_Create;
    }
}

void TurboSpiker_StageLoad(void)
{
    if (RSDK.CheckStageFolder("HCZ") == 1)
        TurboSpiker->animID = RSDK.LoadSpriteAnimation("HCZ/TurboSpiker.bin", SCOPE_STAGE);
    TurboSpiker->hermitHitbox.left   = -14;
    TurboSpiker->hermitHitbox.top    = -14;
    TurboSpiker->hermitHitbox.right  = 14;
    TurboSpiker->hermitHitbox.bottom = 14;
    TurboSpiker->spikeHitbox.left    = -5;
    TurboSpiker->spikeHitbox.top     = -31;
    TurboSpiker->spikeHitbox.right   = 9;
    TurboSpiker->spikeHitbox.bottom  = 2;
    TurboSpiker->checkbox.left       = -0x80;
    TurboSpiker->checkbox.top        = -0x200;
    TurboSpiker->checkbox.right      = 0x80;
    TurboSpiker->checkbox.bottom     = 0x200;
    TurboSpiker->launchSFX           = RSDK.GetSFX("Stage/Launch.wav");
    TurboSpiker->splashSFX           = RSDK.GetSFX("Stage/Splash.wav");
    DEBUGMODE_ADD_OBJ(TurboSpiker);
}

void TurboSpiker_DebugDraw()
{
    RSDK.SetSpriteAnimation(TurboSpiker->animID, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, 0, false);
}

void TurboSpiker_DebugSpawn() { CREATE_ENTITY(TurboSpiker, NULL, RSDK_sceneInfo->entity->position.x, RSDK_sceneInfo->entity->position.y); }

void TurboSpiker_EditorDraw(void) { TurboSpiker_DebugDraw(); }

void TurboSpiker_EditorLoad(void) { TurboSpiker_StageLoad(); }

void TurboSpiker_Serialize(void)
{
    RSDK_EDITABLE_VAR(TurboSpiker, VAR_ENUM, type);
    RSDK_EDITABLE_VAR(TurboSpiker, VAR_UINT8, direction);
}

void TurboSpiker_Hermit_Collide(void)
{
    RSDK_THIS(TurboSpiker);
    foreach_active(Player, player)
    {
        if (Player_CheckBadnikHit(player, entity, &TurboSpiker->hermitHitbox) && Player_CheckBadnikBreak(entity, player, false)) {
            if (entity->spike)
                destroyEntity(entity->spike);
            destroyEntity(entity);
        }
    }
}

void TurboSpiker_Hermit_CheckOnScreen(void)
{
    RSDK_THIS(TurboSpiker);
    if (!RSDK.CheckOnScreen(entity, 0) && RSDK.CheckPosOnScreen(&entity->spawnPos, &entity->updateRange)) {
        entity->position.x = entity->spawnPos.x;
        entity->position.y = entity->spawnPos.y;
        if (entity->spike)
            destroyEntity(entity->spike);
        entity->direction = entity->spawnDir;
        TurboSpiker_Create(NULL);
    }
}

void TurboSpiker_Hermit_NextState(void)
{
    RSDK_THIS(TurboSpiker);
    entity->position.x += entity->velocity.x;
    if (RSDK.ObjectTileGrip(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x80000, 8u) == false) {
        RSDK.SetSpriteAnimation(TurboSpiker->animID, 1, &entity->animator, true, 0);
        entity->timer = 0;
        if (RSDK.ObjectTileGrip(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0x20000 * (entity->direction ? -1 : 1), 0x80000, 8u))
            entity->state = TurboSpiker_Hermit_Turn;
        else
            entity->state = TurboSpiker_Hermit_Move;
    }
}

void TurboSpiker_Hermit_Create(void)
{
    RSDK_THIS(TurboSpiker);
    entity->active = ACTIVE_NORMAL;
    if (entity->type == 1) {
        RSDK.SetSpriteAnimation(TurboSpiker->animID, 1, &entity->spikeAnimator, true, 0);
        RSDK.SetSpriteAnimation(TurboSpiker->animID, 3, &entity->animator, true, 0);
        entity->drawOrder = Zone->fgLayerLow + 1;
        if (Player_Unknown3())
            entity->direction = Player_Unknown3()->position.x >= entity->position.x;
    }
    EntityTurboSpiker *spike = CREATE_ENTITY(TurboSpiker, intToVoid(1), entity->position.x, entity->position.y);
    spike->isPermanent       = true;
    spike->direction         = entity->direction;
    spike->drawOrder         = entity->drawOrder - 1;
    RSDK.SetSpriteAnimation(TurboSpiker->animID, 3, &spike->animator, true, 0);
    spike->state  = (void (*)(void))TurboSpiker_Spike_Collide;
    entity->spike = (Entity*)spike;
    if (!entity->type) {
        entity->timer      = 64;
        entity->state      = TurboSpiker_Hermit_Idle;
        entity->velocity.x = 0x10000 * (entity->direction ? 1 : -1);
        TurboSpiker_Hermit_Idle();
        entity->velocity.y = 0;
    }
    else {
        entity->state = TurboSpiker_Hermit_IdleWater;
        TurboSpiker_Hermit_IdleWater();
        entity->velocity.y = 0;
    }
}
void TurboSpiker_Hermit_Idle(void)
{
    RSDK_THIS(TurboSpiker);
    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, entity, &TurboSpiker->checkbox)) {
            entity->timer = 1;
            entity->state = TurboSpiker_Hermit_Fire;
            TurboSpiker_Hermit_Collide();
            TurboSpiker_Hermit_CheckOnScreen();
            foreach_return;
        }
    }

    TurboSpiker_Hermit_NextState();
    Entity *spike = entity->spike;
    if (spike) {
        spike->position.x = entity->position.x;
        spike->position.y = entity->position.y;
    }
    if (entity->state == TurboSpiker_Hermit_Idle && !--entity->timer) {
        RSDK.SetSpriteAnimation(TurboSpiker->animID, 1, &entity->animator, true, 0);
        entity->timer = 0;
        entity->state = TurboSpiker_Hermit_Turn;
    }
    RSDK.ProcessAnimation(&entity->animator);
    TurboSpiker_Hermit_Collide();
    TurboSpiker_Hermit_CheckOnScreen();
}

void TurboSpiker_Hermit_IdleWater(void)
{
    RSDK_THIS(TurboSpiker);
    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, entity, &TurboSpiker->checkbox)) {
            CREATE_ENTITY(Water, intToVoid(6), entity->position.x, entity->position.y + 0x80000)->childPtr = intToVoid(1);
            RSDK.PlaySFX(TurboSpiker->splashSFX, 0, 255);
            RSDK.SetSpriteAnimation(0xFFFF, 0, &entity->spikeAnimator, true, 0);
            RSDK.SetSpriteAnimation(TurboSpiker->animID, 1, &entity->animator, true, 0);
            entity->drawOrder = Zone->drawOrderLow + 1;
            if (entity->spike)
                entity->spike->drawOrder = Zone->drawOrderLow;
            EntityTurboSpiker *ember = CREATE_ENTITY(TurboSpiker, intToVoid(1), entity->position.x, entity->position.y);
            ember->direction         = entity->direction;
            ember->drawOrder         = entity->drawOrder + 1;
            RSDK.SetSpriteAnimation(TurboSpiker->animID, 6, &ember->animator, true, 0);
            ember->state  = TurboSpiker_Ember_Animate;
            entity->timer = 60;
            entity->state = TurboSpiker_Hermit_Fire;
            TurboSpiker_Hermit_Fire();
            foreach_return;
        }
    }
    RSDK.ProcessAnimation(&entity->spikeAnimator);
    TurboSpiker_Hermit_Collide();
    TurboSpiker_Hermit_CheckOnScreen();
}

void TurboSpiker_Hermit_Handle(void)
{
    RSDK_THIS(TurboSpiker);
    TurboSpiker_Hermit_NextState();
    if (entity->spike) {
        entity->spike->position.x = entity->position.x;
        entity->spike->position.y = entity->position.y;
    }
    RSDK.ProcessAnimation(&entity->animator);
    TurboSpiker_Hermit_Collide();
    TurboSpiker_Hermit_CheckOnScreen();
}

void TurboSpiker_Hermit_Turn(void)
{
    RSDK_THIS(TurboSpiker);
    RSDK.ProcessAnimation(&entity->animator);
    if (++entity->timer == 15) {
        RSDK.SetSpriteAnimation(TurboSpiker->animID, 2, &entity->animator, true, 0);
        entity->state = TurboSpiker_Hermit_AfterTurn;
    }
    TurboSpiker_Hermit_Collide();
    TurboSpiker_Hermit_CheckOnScreen();
}

void TurboSpiker_Hermit_AfterTurn(void)
{
    RSDK_THIS(TurboSpiker);
    RSDK.ProcessAnimation(&entity->animator);
    if (entity->animator.frameID == entity->animator.frameCount - 1) {
        RSDK.SetSpriteAnimation(TurboSpiker->animID, 0, &entity->animator, true, 0);
        entity->direction ^= 1u;
        entity->velocity.x = -entity->velocity.x;
        if (entity->spike) {
            entity->spike->direction = entity->direction;
            entity->timer            = 128;
            entity->state            = TurboSpiker_Hermit_Idle;
            TurboSpiker_Hermit_Idle();
        }
        else {
            entity->state = TurboSpiker_Hermit_Handle;
            TurboSpiker_Hermit_Handle();
        }
    }
    else {
        TurboSpiker_Hermit_Collide();
        TurboSpiker_Hermit_CheckOnScreen();
    }
}

void TurboSpiker_Hermit_Move(void)
{
    RSDK_THIS(TurboSpiker);
    RSDK.ProcessAnimation(&entity->animator);
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    entity->velocity.y += 0x3800;
    if (entity->spike) {
        entity->spike->position.x = entity->position.x;
        entity->spike->position.y = entity->position.y;
    }
    if (RSDK.ObjectTileGrip(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x80000, 8u) == true) {
        entity->velocity.y = 0;
        RSDK.SetSpriteAnimation(TurboSpiker->animID, 0, &entity->animator, true, 0);
        entity->state = TurboSpiker_Hermit_Handle;
        TurboSpiker_Hermit_Handle();
    }
    else {
        TurboSpiker_Hermit_Collide();
        TurboSpiker_Hermit_CheckOnScreen();
    }
}

void TurboSpiker_Hermit_Fire(void)
{
    RSDK_THIS(TurboSpiker);
    if (!--entity->timer) {
        if (Player_Unknown3())
            entity->direction = Player_Unknown3()->position.x < entity->position.x;
        EntityTurboSpiker *spike = (EntityTurboSpiker *)entity->spike;
        if (spike) {
            RSDK.PlaySFX(TurboSpiker->launchSFX, 0, 255);
            RSDK.SetSpriteAnimation(TurboSpiker->animID, 4, &spike->spikeAnimator, true, 0);
            spike->direction  = entity->direction;
            spike->velocity.x = 0x14000 * (entity->direction ? 1 : -1);
            spike->velocity.y = -0x30000;
            spike->state      = (void (*)(void))TurboSpiker_Spike_Fly;
            entity->spike     = NULL;
        }
        entity->velocity.x = 0x20000 * (entity->direction ? 1 : -1);
        RSDK.SetSpriteAnimation(TurboSpiker->animID, 0, &entity->animator, true, 0);
        entity->state                   = TurboSpiker_Hermit_Handle;
        entity->animator.animationSpeed = 2;
        TurboSpiker_Hermit_NextState();
        RSDK.ProcessAnimation(&entity->animator);
        TurboSpiker_Hermit_Collide();
        TurboSpiker_Hermit_CheckOnScreen();
    }
    RSDK.ProcessAnimation(&entity->animator);
    TurboSpiker_Hermit_Collide();
    TurboSpiker_Hermit_CheckOnScreen();
}

void TurboSpiker_Spike_Collide(void)
{
    RSDK_THIS(TurboSpiker);
    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, entity, &TurboSpiker->spikeHitbox)
            && !Player_CheckMightyUnspin(1024, player, 2, &player->uncurlTimer)) {
            Player_CheckHit(player, entity);
        }
    }
}

void TurboSpiker_Spike_Fly(void)
{
    RSDK_THIS(TurboSpiker);
    if (entity->launchPlayed == false) {
        RSDK.PlaySFX(TurboSpiker->launchSFX, 0, 255);
        entity->launchPlayed = true;
    }
    if (!entity->activeScreens) {
        RSDK.StopSFX(TurboSpiker->launchSFX);
        entity->launchPlayed = false;
    }
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    TurboSpiker_Spike_Collide();
}

void TurboSpiker_Ember_Animate(void)
{
    RSDK_THIS(TurboSpiker);
    RSDK.ProcessAnimation(&entity->animator);
    if (entity->animator.frameCount - 1 == entity->animator.frameID)
        destroyEntity(entity);
}
