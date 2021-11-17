#include "SonicMania.h"

ObjectWisp *Wisp = NULL;

void Wisp_Update(void)
{
    RSDK_THIS(Wisp);
    StateMachine_Run(entity->state);
}

void Wisp_LateUpdate(void) {}

void Wisp_StaticUpdate(void) {}

void Wisp_Draw(void)
{
    RSDK_THIS(Wisp);
    RSDK.DrawSprite(&entity->bodyAnimator, NULL, false);

    entity->inkEffect = INK_ALPHA;
    RSDK.DrawSprite(&entity->wingAnimator, NULL, false);

    entity->inkEffect = INK_NONE;
}

void Wisp_Create(void *data)
{
    RSDK_THIS(Wisp);
    entity->visible   = true;
    entity->drawOrder = Zone->drawOrderLow;
    entity->drawFX |= FX_FLIP;
    entity->startPos      = entity->position;
    entity->active        = ACTIVE_BOUNDS;
    entity->updateRange.x = 0x800000;
    entity->updateRange.y = 0x800000;
    entity->direction     = FLIP_NONE;
    entity->targetPtr     = NULL;
    entity->alpha         = 0xC0;
    entity->timer         = 16;
    entity->buzzCount        = 4;
    RSDK.SetSpriteAnimation(Wisp->aniFrames, 0, &entity->bodyAnimator, true, 0);
    RSDK.SetSpriteAnimation(Wisp->aniFrames, 1, &entity->wingAnimator, true, 0);
    entity->state = Wisp_State_Setup;
}

void Wisp_StageLoad(void)
{
    Wisp->aniFrames     = RSDK.LoadSpriteAnimation("Blueprint/Wisp.bin", SCOPE_STAGE);
    Wisp->hitbox.left   = -8;
    Wisp->hitbox.top    = -8;
    Wisp->hitbox.right  = 8;
    Wisp->hitbox.bottom = 8;
    DEBUGMODE_ADD_OBJ(Wisp);
}

void Wisp_DebugSpawn(void)
{
    RSDK_THIS(DebugMode);

    CREATE_ENTITY(Wisp, NULL, entity->position.x, entity->position.y);
}

void Wisp_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(Wisp->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
}

void Wisp_HandlePlayerInteractions(void)
{
    RSDK_THIS(Wisp);
    foreach_active(Player, player)
    {
        EntityPlayer *target = entity->targetPtr;
        if (target) {
            if (abs(player->position.x - entity->position.x) < abs(target->position.x - entity->position.x))
                entity->targetPtr = player;
        }
        else {
            entity->targetPtr = player;
        }
        if (Player_CheckBadnikTouch(player, entity, &Wisp->hitbox))
            Player_CheckBadnikBreak(entity, player, true);
    }
}

void Wisp_CheckOnScreen(void)
{
    RSDK_THIS(Wisp);
    if (!RSDK.CheckOnScreen(entity, NULL) && !RSDK.CheckPosOnScreen(&entity->startPos, &entity->updateRange)) {
        entity->position.x = entity->startPos.x;
        entity->position.y = entity->startPos.y;
        Wisp_Create(NULL);
    }
}

void Wisp_State_Setup(void)
{
    RSDK_THIS(Wisp);
    entity->active     = ACTIVE_NORMAL;
    entity->velocity.x = 0;
    entity->velocity.y = 0;
    entity->state      = Wisp_WaitInPlace;
    Wisp_WaitInPlace();
}

void Wisp_WaitInPlace(void)
{
    RSDK_THIS(Wisp);

    if (!--entity->timer) {
        if (--entity->buzzCount) {
            entity->velocity.y = -0x10000;
            entity->timer      = 96;
            entity->state      = Wisp_FlyTowardTarget;
        }
        else {
            entity->velocity.x = -0x20000;
            entity->velocity.y = -0x20000;
            entity->state      = Wisp_BasicFly;
        }
    }
    RSDK.ProcessAnimation(&entity->wingAnimator);
    Wisp_HandlePlayerInteractions();
    Wisp_CheckOnScreen();
}

void Wisp_FlyTowardTarget(void)
{
    RSDK_THIS(Wisp);

    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;

    EntityPlayer *target = entity->targetPtr;
    if (target) {
        if (target->position.x >= entity->position.x) {
            entity->velocity.x += 0x1000;
            entity->direction = FLIP_X;
            if (entity->velocity.x > 0x20000)
                entity->velocity.x = 0x20000;
        }
        else {
            entity->velocity.x -= 0x1000;
            entity->direction = FLIP_NONE;
            if (entity->velocity.x < -0x20000)
                entity->velocity.x = -0x20000;
        }

        if (target->position.y >= entity->position.y) {
            entity->velocity.y += 0x1000;
            if (entity->velocity.y > 0x20000)
                entity->velocity.y = 0x20000;
        }
        else {
            entity->velocity.y -= 0x1000;
            if (entity->velocity.y < -0x20000)
                entity->velocity.y = -0x20000;
        }
    }

    entity->timer--;
    if (!entity->timer) {
        entity->timer      = RSDK.Rand(0, 32);
        entity->state      = Wisp_WaitInPlace;
        entity->velocity.x = 0;
        entity->velocity.y = 0;
        entity->direction  = FLIP_NONE;
    }
    RSDK.ProcessAnimation(&entity->wingAnimator);
    Wisp_HandlePlayerInteractions();
    Wisp_CheckOnScreen();
}

void Wisp_BasicFly(void)
{
    RSDK_THIS(Wisp);
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    RSDK.ProcessAnimation(&entity->wingAnimator);
    Wisp_HandlePlayerInteractions();
    Wisp_CheckOnScreen();
}

#if RETRO_INCLUDE_EDITOR
void Wisp_EditorDraw(void) {}

void Wisp_EditorLoad(void) {}
#endif

void Wisp_Serialize(void) { RSDK_EDITABLE_VAR(Wisp, VAR_UINT8, direction); }
