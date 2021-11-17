#include "SonicMania.h"

ObjectShutterbug *Shutterbug;

//cleanup note: look in Unused/Wisp

void Shutterbug_Update(void)
{
    RSDK_THIS(Shutterbug);
    StateMachine_Run(entity->state);
}

void Shutterbug_LateUpdate(void) {}

void Shutterbug_StaticUpdate(void) {}

void Shutterbug_Draw(void)
{
    RSDK_THIS(Shutterbug);
    RSDK.DrawSprite(&entity->animator, NULL, false);
    if (entity->flickerTimer & 2) {
        entity->inkEffect = INK_ADD;
        entity->alpha     = 0x100;
        RSDK.DrawSprite(&entity->overlayAnim, NULL, false);
        entity->inkEffect = INK_NONE;
    }
}

void Shutterbug_Create(void *data)
{
    RSDK_THIS(Shutterbug);
    entity->visible   = true;
    entity->drawOrder = Zone->drawOrderHigh;
    if (!RSDK_sceneInfo->inEditor) {
        entity->active        = ACTIVE_BOUNDS;
        entity->updateRange.x = 0x800000;
        entity->updateRange.y = 0x800000;
        if (!entity->range.x) {
            entity->range.x = 0x7FFF0000;
            entity->range.y = 0x7FFF0000;
        }
        entity->drawFX |= FX_FLIP;
        entity->focusBox.left   = (-entity->range.x) >> 16;
        entity->focusBox.top    = (-entity->range.y) >> 16;
        entity->focusBox.bottom = entity->range.y >> 16;
        entity->focusBox.right  = entity->range.x >> 16;
        entity->direction       = FLIP_X;
        entity->moveDir         = FLIP_X;
        if (!entity->snaps)
            entity->numSnaps = 0xFF;
        else
            entity->numSnaps = entity->snaps;
        entity->focus     = NULL;
        entity->alpha     = 0xC0;
        entity->snapTimer = 16;

        RSDK.SetSpriteAnimation(Shutterbug->aniFrames, 0, &entity->animator, true, 0);
        RSDK.SetSpriteAnimation(Shutterbug->aniFrames, 1, &entity->overlayAnim, true, 0);
        entity->state = Shutterbug_CreateState;
    }
}

void Shutterbug_StageLoad(void)
{
    if (RSDK.CheckStageFolder("SPZ1"))
        Shutterbug->aniFrames = RSDK.LoadSpriteAnimation("SPZ1/Shutterbug.bin", SCOPE_STAGE);
    else
        Shutterbug->aniFrames = RSDK.LoadSpriteAnimation("SPZ2/Shutterbug.bin", SCOPE_STAGE);
    Shutterbug->hitbox.left     = -15;
    Shutterbug->hitbox.top      = -12;
    Shutterbug->hitbox.right    = 15;
    Shutterbug->hitbox.bottom   = 12;
    Shutterbug->checkbox.left   = -160;
    Shutterbug->checkbox.top    = -160;
    Shutterbug->checkbox.right  = 160;
    Shutterbug->checkbox.bottom = 160;
    Shutterbug->pictureCount    = 0;
    DEBUGMODE_ADD_OBJ(Shutterbug);
    Shutterbug->snapSfx = RSDK.GetSFX("SPZ/ShBugSnap.wav");
}

void Shutterbug_DebugSpawn(void)
{
    RSDK_THIS(DebugMode);

    CREATE_ENTITY(Shutterbug, NULL, entity->position.x, entity->position.y);
}

void Shutterbug_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(Shutterbug->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
}

void Shutterbug_CheckOnScreen(void)
{
    RSDK_THIS(Shutterbug);

    if (!RSDK.CheckOnScreen(entity, NULL) && !RSDK.CheckPosOnScreen(&entity->spawnPos, &entity->updateRange)) {
        entity->position.x = entity->spawnPos.x;
        entity->position.y = entity->spawnPos.y;
        Shutterbug_Create(NULL);
    }
}

void Shutterbug_CreateState(void)
{
    RSDK_THIS(Shutterbug);

    entity->active     = ACTIVE_NORMAL;
    entity->velocity.x = 0;
    entity->velocity.y = 0;
    entity->offset.x   = 0;
    entity->focus      = 0;
    entity->turnTimer  = 0;
    entity->state      = Shutterbug_FlyAround;
    Shutterbug_FlyAround();
}

void Shutterbug_FlyAround(void)
{
    RSDK_THIS(Shutterbug);
    if (entity->snapTimer == 20 && entity->focus) {
        entity->flickerTimer = 8;
        RSDK.PlaySfx(Shutterbug->snapSfx, false, 255);
        Shutterbug_IncrementPicCount();
    }
    if (!entity->snapTimer--) {
        if (entity->focus)
            --entity->numSnaps;
        if (entity->numSnaps < 0) {
            if (entity->focus && entity->position.x >= entity->focus->position.x)
                entity->velocity.x = 0x20000;
            else
                entity->velocity.x = -0x20000;
            entity->velocity.y = -0x20000;
            entity->state      = Shutterbug_BasicMove;
        }
        else {
            entity->offset.x = 0;
            entity->offset.y = 0;
            if (entity->focus)
                entity->velocity.y = -0x10000;
            entity->snapTimer = 96;
            entity->state     = Shutterbug_ShakeFly;
        }
    }
    Shutterbug_HandleBodyAnim();
    Shutterbug_CheckFocus();
    Shutterbug_CheckOnScreen();
}

void Shutterbug_ShakeFly()
{
    RSDK_THIS(Shutterbug);
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    if (entity->focus) {
        entity->offset.x = entity->focus->position.x;
        entity->offset.y = entity->focus->position.y;
    }
    else if (!entity->offset.x) {
        entity->offset.x = 0;
        entity->offset.y = 0;

        while (abs(entity->offset.x) < 4) entity->offset.x = RSDK.Rand(-16, 17);
        while (abs(entity->offset.y) < 4) entity->offset.y = RSDK.Rand(-16, 17);
        entity->offset.x = entity->position.x + (entity->offset.x << 17);
        entity->offset.y = entity->position.y + (entity->offset.y << 17);
    }
    // TODO: rmg-code this?
    if (entity->offset.x >= entity->position.x) {
        entity->velocity.x += 0x1000;
        if (entity->velocity.x > 0x20000)
            entity->velocity.x = 0x20000;
        entity->moveDir = FLIP_NONE;
    }
    else {
        entity->velocity.x -= 0x1000;
        if (entity->velocity.x < -0x20000)
            entity->velocity.x = -0x20000;
        entity->moveDir = FLIP_X;
    }
    if (entity->offset.y >= entity->position.y) {
        entity->velocity.y += 0x1000;
        if (entity->velocity.y > 0x20000)
            entity->velocity.y = 0x20000;
    }
    else {
        entity->velocity.y -= 0x1000;
        if (entity->velocity.y < -0x20000)
            entity->velocity.y = -0x20000;
    }
    if (!entity->snapTimer--) {
        entity->state      = Shutterbug_FlyAround;
        entity->velocity.x = 0;
        entity->velocity.y = 0;
        entity->snapTimer  = RSDK.Rand(0, 32) + 20;
    }
    if (!entity->passThrough) {
        if (abs(entity->velocity.x) <= abs(entity->velocity.y)) {
            if (!Shutterbug_BounceY())
                Shutterbug_BounceX();
        }
        else if (!Shutterbug_BounceX()) {
            Shutterbug_BounceY();
        }
    }
    Shutterbug_HandleBodyAnim();
    Shutterbug_CheckFocus();
    Shutterbug_CheckOnScreen();
}

void Shutterbug_BasicMove(void)
{
    RSDK_THIS(Shutterbug);
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    Shutterbug_HandleBodyAnim();
    Shutterbug_CheckFocus();
    Shutterbug_CheckOnScreen();
}

int Shutterbug_BounceX(void)
{
    RSDK_THIS(Shutterbug);

    // hi rmg here you may be wondering why the hell CMODE_FLOOR is used
    // oh lemme tell ya buddy i have no fuckin idea

    int x   = (RSDK_sceneInfo->entity->velocity.x <= 0) ? -0x90000 : 0x90000;
    int res = RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_FLOOR, 0, x, 0x80000, false)
              | RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_FLOOR, 0, x, -0x80000, false);
    if (res == 1)
        entity->velocity.x = -entity->velocity.x;
    return res;
}
int Shutterbug_BounceY(void)
{
    RSDK_THIS(Shutterbug);

    // makes sense here though!
    int y   = (RSDK_sceneInfo->entity->velocity.y <= 0) ? -0x90000 : 0x90000;
    int res = RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0x80000, y, false)
              | RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_FLOOR, 0, -0x80000, y, false);
    if (res == 1)
        entity->velocity.y = -entity->velocity.y;
    return res;
}

void Shutterbug_CheckFocus(void)
{
    RSDK_THIS(Shutterbug);
    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, entity, &entity->focusBox)) {
            if (!entity->focus)
                entity->focus = player;
            else {
                // pick closest
                if (abs(entity->position.x - player->position.x) < abs(entity->position.x - entity->focus->position.x))
                    entity->focus = player;
            }
            if (Player_CheckBadnikTouch(player, entity, &Shutterbug->hitbox))
                Player_CheckBadnikBreak(entity, player, true);
        }
    }
}

void Shutterbug_IncrementPicCount(void)
{
    RSDK_THIS(Shutterbug);
    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, entity, &Shutterbug->checkbox) && ++Shutterbug->pictureCount == 10)
            API_UnlockAchievement("ACH_SPZ");
    }
}

void Shutterbug_HandleBodyAnim(void)
{

    RSDK_THIS(Shutterbug);

    ++entity->animator.animationTimer;
    entity->animator.animationTimer &= 3;
    int animTimer = entity->animator.animationTimer;
    if (!entity->flickerTimer || entity->turnTimer) {
        if (entity->direction == entity->moveDir) {
            if (entity->turnTimer)
                entity->turnTimer--;
        }
        else if (++entity->turnTimer >= 9)
            entity->direction = entity->moveDir;
        entity->animator.frameID = (animTimer >> 1) + (entity->turnTimer & ~1);
    }
    else {
        entity->animator.frameID = (animTimer >> 1) + (((28 - --entity->flickerTimer) >> 1) & ~1);
    }
}

#if RETRO_INCLUDE_EDITOR
void Shutterbug_EditorDraw(void)
{
    RSDK_THIS(Shutterbug);
    RSDK.SetSpriteAnimation(Shutterbug->aniFrames, 0, &entity->animator, true, 0);
    RSDK.SetSpriteAnimation(Shutterbug->aniFrames, 1, &entity->overlayAnim, true, 0);
    entity->drawFX = FX_FLIP;

    Shutterbug_Draw();
}

void Shutterbug_EditorLoad(void)
{
    if (RSDK.CheckStageFolder("SPZ1"))
        Shutterbug->aniFrames = RSDK.LoadSpriteAnimation("SPZ1/Shutterbug.bin", SCOPE_STAGE);
    else
        Shutterbug->aniFrames = RSDK.LoadSpriteAnimation("SPZ2/Shutterbug.bin", SCOPE_STAGE);
}
#endif

void Shutterbug_Serialize(void)
{
    RSDK_EDITABLE_VAR(Shutterbug, VAR_UINT8, snaps);
    RSDK_EDITABLE_VAR(Shutterbug, VAR_BOOL, passThrough);
    RSDK_EDITABLE_VAR(Shutterbug, VAR_VECTOR2, range);
}