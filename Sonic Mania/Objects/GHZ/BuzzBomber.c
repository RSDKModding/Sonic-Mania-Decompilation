#include "SonicMania.h"

ObjectBuzzBomber *BuzzBomber;

void BuzzBomber_Update(void)
{
    RSDK_THIS(BuzzBomber);
    StateMachine_Run(entity->state);
}

void BuzzBomber_LateUpdate(void) {}

void BuzzBomber_StaticUpdate(void) {}

void BuzzBomber_Draw(void)
{
    RSDK_THIS(BuzzBomber);

    if (entity->inkEffect == INK_ADD) {
        RSDK.DrawSprite(&entity->animator1, NULL, false);
    }
    else {
        RSDK.DrawSprite(&entity->animator1, NULL, false);
        entity->inkEffect = INK_ALPHA;
        RSDK.DrawSprite(&entity->animator2, NULL, false);
        entity->inkEffect = INK_NONE;
        RSDK.DrawSprite(&entity->animator3, NULL, false);
    }
}

void BuzzBomber_Create(void *data)
{
    RSDK_THIS(BuzzBomber);
    entity->visible = true;
    entity->drawFX |= FX_FLIP;
    entity->drawOrder  = Zone->drawOrderLow;
    entity->startPos.x = entity->position.x;
    entity->startPos.y = entity->position.y;
    entity->startDir   = entity->direction;
    entity->timer      = 128;
    entity->field_64   = 0;
    entity->projectile = 0;
    if (entity->shotRange == 0)
        entity->shotRange = 96;

    entity->rangeHitbox.right  = entity->shotRange;
    entity->rangeHitbox.left   = -entity->shotRange;
    entity->rangeHitbox.top    = -256;
    entity->rangeHitbox.bottom = 256;

    if (data) {
        entity->inkEffect     = INK_ADD;
        entity->alpha         = 0xC0;
        entity->active        = ACTIVE_NORMAL;
        entity->updateRange.x = 0x200000;
        entity->updateRange.y = 0x200000;
        RSDK.SetSpriteAnimation(BuzzBomber->aniFrames, 4, &entity->animator1, true, 0);
        RSDK.SetSpriteAnimation(0xFFFF, 0, &entity->animator2, true, 0);
        RSDK.SetSpriteAnimation(0xFFFF, 0, &entity->animator3, true, 0);
        entity->state = BuzzBomber_Unknown6;
    }
    else {
        entity->active        = ACTIVE_BOUNDS;
        entity->updateRange.x = 0x800000;
        entity->updateRange.y = 0x800000;
        RSDK.SetSpriteAnimation(BuzzBomber->aniFrames, 0, &entity->animator1, true, 0);
        RSDK.SetSpriteAnimation(BuzzBomber->aniFrames, 2, &entity->animator2, true, 0);
        RSDK.SetSpriteAnimation(BuzzBomber->aniFrames, 3, &entity->animator3, true, 0);
        entity->state = BuzzBomber_Unknown2;
        entity->alpha = 0xC0;
    }
}

void BuzzBomber_StageLoad(void)
{
    if (RSDK.CheckStageFolder("GHZ"))
        BuzzBomber->aniFrames = RSDK.LoadSpriteAnimation("GHZ/BuzzBomber.bin", SCOPE_STAGE);
    else if (RSDK.CheckStageFolder("Blueprint"))
        BuzzBomber->aniFrames = RSDK.LoadSpriteAnimation("Blueprint/BuzzBomber.bin", SCOPE_STAGE);

    BuzzBomber->hitbox1.left   = -24;
    BuzzBomber->hitbox1.top    = -12;
    BuzzBomber->hitbox1.right  = 24;
    BuzzBomber->hitbox1.bottom = 12;
    BuzzBomber->hitbox2.left   = -6;
    BuzzBomber->hitbox2.top    = -6;
    BuzzBomber->hitbox2.right  = 6;
    BuzzBomber->hitbox2.bottom = 6;

    DEBUGMODE_ADD_OBJ(BuzzBomber);
}

void BuzzBomber_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(BuzzBomber->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
}

void BuzzBomber_DebugSpawn(void)
{
    RSDK_THIS(DebugMode);
    EntityBuzzBomber *buzzBomber = CREATE_ENTITY(BuzzBomber, NULL, entity->position.x, entity->position.y);

    buzzBomber->direction = entity->direction;
    buzzBomber->startDir  = entity->direction;
}

void BuzzBomber_CheckOnScreen(void)
{
    RSDK_THIS(BuzzBomber);
    if (!RSDK.CheckOnScreen(RSDK_sceneInfo->entity, NULL) && !RSDK.CheckPosOnScreen(&entity->startPos, &entity->updateRange)) {
        entity->position.x = entity->startPos.x;
        entity->position.y = entity->startPos.y;
        entity->direction  = entity->startDir;
        BuzzBomber_Create(NULL);
    }
}

void BuzzBomber_Unknown1(void)
{
    RSDK_THIS(BuzzBomber);

    foreach_active(Player, player)
    {
        if (Player_CheckBadnikTouch(player, entity, &BuzzBomber->hitbox1)) {
            if (Player_CheckBadnikBreak(entity, player, true)) {
                if (entity->projectile)
                    RSDK.ResetEntityPtr(entity->projectile, TYPE_BLANK, 0);
            }
        }
        else if (entity->state == BuzzBomber_Unknown3 && !entity->field_64) {
            if (Player_CheckCollisionTouch(player, entity, &entity->rangeHitbox)) {
                entity->field_64 = 1;
                entity->timer    = 90;
                RSDK.SetSpriteAnimation(0xFFFF, 0, &entity->animator3, true, 0);
                entity->state = BuzzBomber_Unknown5;
            }
        }
    }
}

void BuzzBomber_Unknown2(void)
{
    RSDK_THIS(BuzzBomber);

    entity->active = ACTIVE_NORMAL;
    if (!(entity->direction & FLIP_X))
        entity->velocity.x = -0x40000;
    else
        entity->velocity.x = 0x40000;
    entity->state = BuzzBomber_Unknown3;
    BuzzBomber_Unknown3();
}

void BuzzBomber_Unknown3(void)
{
    RSDK_THIS(BuzzBomber);

    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    if (!entity->timer) {
        entity->direction ^= FLIP_X;
        entity->timer      = 60;
        entity->velocity.x = -entity->velocity.x;
        entity->field_64   = 0;
        RSDK.SetSpriteAnimation(0xFFFF, 0, &entity->animator3, true, 0);
        entity->state = BuzzBomber_Unknown4;
    }

    RSDK.ProcessAnimation(&entity->animator1);
    RSDK.ProcessAnimation(&entity->animator2);
    RSDK.ProcessAnimation(&entity->animator3);
    BuzzBomber_Unknown1();
    BuzzBomber_CheckOnScreen();
}

void BuzzBomber_Unknown4(void)
{
    RSDK_THIS(BuzzBomber);

    if (!--entity->timer) {
        entity->timer = 128;
        RSDK.SetSpriteAnimation(BuzzBomber->aniFrames, 3, &entity->animator3, true, 0);
        entity->state = BuzzBomber_Unknown3;
    }
    RSDK.ProcessAnimation(&entity->animator1);
    RSDK.ProcessAnimation(&entity->animator2);
    BuzzBomber_Unknown1();
    BuzzBomber_CheckOnScreen();
}

void BuzzBomber_Unknown5(void)
{
    RSDK_THIS(BuzzBomber);

    RSDK.ProcessAnimation(&entity->animator1);
    RSDK.ProcessAnimation(&entity->animator2);
    BuzzBomber_Unknown1();
    BuzzBomber_CheckOnScreen();

    entity->timer--;
    if (entity->timer == 82) {
        RSDK.SetSpriteAnimation(BuzzBomber->aniFrames, 1, &entity->animator1, true, 0);
    }
    else if (entity->timer == 45) {
        EntityBuzzBomber *projectile = (EntityBuzzBomber *)RSDK.CreateEntity(BuzzBomber->objectID, (void *)1, entity->position.x, entity->position.y);
        if (entity->direction) {
            projectile->position.x += 0x180000;
            projectile->velocity.x = 0x20000;
        }
        else {
            projectile->position.x -= 0x180000;
            projectile->velocity.x = -0x20000;
        }
        projectile->position.y += 0x1C0000;
        projectile->velocity.y = 0x20000;
        projectile->groundVel  = 0;
        projectile->projectile = (Entity *)entity;
        projectile->direction  = entity->direction;
        projectile->active     = ACTIVE_NORMAL;
        entity->projectile     = (Entity *)projectile;
    }
    else if (!entity->timer) {
        RSDK.SetSpriteAnimation(BuzzBomber->aniFrames, 0, &entity->animator1, true, 0);
        entity->timer = 128;
        RSDK.SetSpriteAnimation(BuzzBomber->aniFrames, 3, &entity->animator3, true, 0);
        entity->state = BuzzBomber_Unknown3;
    }
}

void BuzzBomber_Unknown6(void)
{
    RSDK_THIS(BuzzBomber);
    RSDK.ProcessAnimation(&entity->animator1);

    if (entity->animator1.frameID == 6) {
        entity->state          = BuzzBomber_Unknown7;
        EntityBuzzBomber *shot = (EntityBuzzBomber *)entity->projectile;
        shot->projectile       = NULL;
    }
}

void BuzzBomber_Unknown7(void)
{
    RSDK_THIS(BuzzBomber);
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    if (RSDK.CheckOnScreen(entity, NULL)) {
        RSDK.ProcessAnimation(&entity->animator1);

        foreach_active(Player, player)
        {
            if (Player_CheckCollisionTouch(player, entity, &BuzzBomber->hitbox2))
                Player_CheckProjectileHit(player, entity);
        }
    }
    else {
        destroyEntity(entity);
    }
}

#if RETRO_INCLUDE_EDITOR
void BuzzBomber_EditorDraw(void) { BuzzBomber_Draw(); }

void BuzzBomber_EditorLoad(void)
{
    if (RSDK.CheckStageFolder("GHZ"))
        BuzzBomber->aniFrames = RSDK.LoadSpriteAnimation("GHZ/BuzzBomber.bin", SCOPE_STAGE);
    else if (RSDK.CheckStageFolder("Blueprint"))
        BuzzBomber->aniFrames = RSDK.LoadSpriteAnimation("Blueprint/BuzzBomber.bin", SCOPE_STAGE);
}
#endif

void BuzzBomber_Serialize(void)
{
    RSDK_EDITABLE_VAR(BuzzBomber, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(BuzzBomber, VAR_UINT8, shotRange);
}
