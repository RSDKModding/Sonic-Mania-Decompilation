#include "SonicMania.h"

ObjectMechaBu *MechaBu;

void MechaBu_Update(void)
{
    RSDK_THIS(MechaBu);
    StateMachine_Run(entity->state);

    Vector2 offset   = MechaBu_GetSawOffset();
    entity->sawPos.x = offset.x;
    entity->sawPos.y = offset.y;
    entity->sawPos.x += entity->position.x;
    entity->sawPos.y += entity->position.y;
}

void MechaBu_LateUpdate(void) {}

void MechaBu_StaticUpdate(void) {}

void MechaBu_Draw(void)
{
    RSDK_THIS(MechaBu);
    RSDK.DrawSprite(&entity->animator1, NULL, false);
    RSDK.DrawSprite(&entity->animator3, &entity->sawPos, false);
    RSDK.DrawSprite(&entity->animator2, NULL, false);
}

void MechaBu_Create(void *data)
{
    RSDK_THIS(MechaBu);

    entity->visible = true;
    if (entity->planeFilter > 0 && (((uint8)entity->planeFilter - 1) & 2))
        entity->drawOrder = Zone->drawOrderHigh;
    else
        entity->drawOrder = Zone->drawOrderLow;
    entity->drawFX |= FX_FLIP;
    entity->startPos      = entity->position;
    entity->startDir      = entity->direction;
    entity->active        = ACTIVE_BOUNDS;
    entity->updateRange.x = 0x800000;
    entity->updateRange.y = 0x800000;
    RSDK.SetSpriteAnimation(MechaBu->aniFrames, 1, &entity->animator1, true, 0);
    RSDK.SetSpriteAnimation(MechaBu->aniFrames, 3, &entity->animator2, true, 0);
    RSDK.SetSpriteAnimation(MechaBu->aniFrames, 7, &entity->animator3, true, 0);
    entity->state = MechaBu_State_Setup;
}

void MechaBu_StageLoad(void)
{
    if (RSDK.CheckStageFolder("MMZ"))
        MechaBu->aniFrames = RSDK.LoadSpriteAnimation("MMZ/MechaBu.bin", SCOPE_STAGE);

    MechaBu->hitboxSaw.left   = -14;
    MechaBu->hitboxSaw.top    = -14;
    MechaBu->hitboxSaw.right  = 14;
    MechaBu->hitboxSaw.bottom = 14;

    MechaBu->hitbox2.left   = -8;
    MechaBu->hitbox2.top    = -12;
    MechaBu->hitbox2.right  = 8;
    MechaBu->hitbox2.bottom = 12;

    DEBUGMODE_ADD_OBJ(MechaBu);

    MechaBu->sfxSawUp   = RSDK.GetSFX("MMZ/SawUp.wav");
    MechaBu->sfxSawDown = RSDK.GetSFX("MMZ/SawDown.wav");
}

void MechaBu_DebugSpawn(void)
{
    RSDK_THIS(MechaBu);

    CREATE_ENTITY(MechaBu, NULL, entity->position.x, entity->position.y);
}

void MechaBu_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(MechaBu->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, 0, false);
}

void MechaBu_CheckPlayerCollisions(void)
{
    RSDK_THIS(MechaBu);

    foreach_active(Player, player)
    {
        if (entity->planeFilter <= 0 || player->collisionPlane == (((uint8)entity->planeFilter - 1) & 1)) {
            if (Player_CheckCollisionTouch(player, &entity->sawPos, &MechaBu->hitboxSaw)
#if RETRO_USE_PLUS
                && !Player_CheckMightyUnspin(512, player, 2, &player->uncurlTimer)
#endif
            ) {
                Player_CheckHit(player, entity);
            }
            else if (Player_CheckBadnikTouch(player, entity, &MechaBu->hitboxSaw) && Player_CheckBadnikBreak(entity, player, false)) {
                EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, entity->sawPos.x, entity->sawPos.y);
                RSDK.SetSpriteAnimation(MechaBu->aniFrames, 7, &debris->animator, false, 0);
                debris->velocity.x    = RSDK.Rand(-2, 3) << 16;
                debris->velocity.y    = RSDK.Rand(-4, -1) << 16;
                debris->gravity       = 0x3800;
                debris->drawOrder     = entity->drawOrder;
                debris->updateRange.x = 0x200000;
                debris->updateRange.y = 0x200000;
                destroyEntity(entity);
            }
        }
    }
}

void MechaBu_CheckOnScreen(void)
{
    RSDK_THIS(MechaBu);
    if (!RSDK.CheckOnScreen(entity, NULL) && !RSDK.CheckPosOnScreen(&entity->startPos, &entity->updateRange)) {
        entity->position.x = entity->startPos.x;
        entity->position.y = entity->startPos.y;
        entity->direction  = entity->startDir;
        MechaBu_Create(NULL);
    }
}

Vector2 MechaBu_GetSawOffset(void)
{
    RSDK_THIS(MechaBu);

    Vector2 result;
    switch (entity->animator2.animationID) {
        case 4:
            result.x = MechaBu->sawOffsets[8];
            result.y = MechaBu->sawOffsets[9];
            break;
        case 5:
            result.x = MechaBu->sawOffsets[2 * (4 - entity->animator2.frameID) + 0];
            result.y = MechaBu->sawOffsets[2 * (4 - entity->animator2.frameID) + 1];
            break;
        case 6:
            result.x = MechaBu->sawOffsets[(2 * entity->animator2.frameID) + 0];
            result.y = MechaBu->sawOffsets[(2 * entity->animator2.frameID) + 1];
            break;
        default:
            result.x = MechaBu->sawOffsets[0];
            result.y = MechaBu->sawOffsets[1];
            break;
    }
    return result;
}

void MechaBu_State_Setup(void)
{
    RSDK_THIS(MechaBu);
    entity->active     = ACTIVE_NORMAL;
    entity->velocity.x = -0x10000;
    entity->timer2     = 0;
    entity->velocity.y = 0;
    entity->state      = MechaBu_Unknown6;
    MechaBu_Unknown6();
}

void MechaBu_Unknown6(void)
{
    RSDK_THIS(MechaBu);

    RSDK.ProcessAnimation(&entity->animator1);
    RSDK.ProcessAnimation(&entity->animator2);
    RSDK.ProcessAnimation(&entity->animator3);

    entity->timer2++;
    if (entity->timer2 == 60) {
        if (entity->activeScreens == 1)
            RSDK.PlaySfx(MechaBu->sfxSawUp, false, 255);
        RSDK.SetSpriteAnimation(MechaBu->aniFrames, 6, &entity->animator2, true, 0);
    }
    else if (entity->timer2 == 120) {
        if (entity->activeScreens == 1)
            RSDK.PlaySfx(MechaBu->sfxSawDown, false, 255);
        entity->timer2 = 0;
        RSDK.SetSpriteAnimation(MechaBu->aniFrames, 5, &entity->animator2, true, 0);
    }

    entity->position.x += entity->velocity.x;
    if (RSDK.ObjectTileGrip(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0xF0000, 8)) {
        bool32 collidedWall  = false;
        bool32 collidedFloor = false;
        if (entity->velocity.x <= 0) {
            collidedWall  = RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_RWALL, 0, -0x120000, 0, true);
            collidedFloor = RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_FLOOR, 0, -0x20000, 0x100000, false);
        }
        else {
            collidedWall  = RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_LWALL, 0, 0x120000, 0, true);
            collidedFloor = RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0x20000, 0x100000, false);
        }

        if (!collidedFloor || collidedWall) {
            entity->timer = 0;
            entity->state = MechaBu_Unknown7;
        }
    }
    else {
        RSDK.SetSpriteAnimation(MechaBu->aniFrames, 1, &entity->animator1, true, 0);
        entity->timer = 0;

        bool32 collided = false;
        if (entity->velocity.x >= 0)
            collided = RSDK.ObjectTileGrip(entity, Zone->fgLayers, CMODE_FLOOR, 0, -0x10000, 0xF0000, 8);
        else
            collided = RSDK.ObjectTileGrip(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0x10000, 0xF0000, 8);

        if (collided)
            entity->state = MechaBu_Unknown7;
        else
            entity->state = MechaBu_Unknown8;
    }
    MechaBu_CheckPlayerCollisions();
    MechaBu_CheckOnScreen();
}

void MechaBu_Unknown7(void)
{
    RSDK_THIS(MechaBu);

    RSDK.ProcessAnimation(&entity->animator1);
    RSDK.ProcessAnimation(&entity->animator2);
    RSDK.ProcessAnimation(&entity->animator3);
    if (++entity->timer == 48) {
        entity->velocity.x = -entity->velocity.x;
        entity->state      = MechaBu_Unknown6;
        MechaBu_Unknown6();
    }
    MechaBu_CheckPlayerCollisions();
    MechaBu_CheckOnScreen();
}

void MechaBu_Unknown8(void)
{
    RSDK_THIS(MechaBu);

    RSDK.ProcessAnimation(&entity->animator1);
    RSDK.ProcessAnimation(&entity->animator2);
    RSDK.ProcessAnimation(&entity->animator3);

    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    entity->velocity.y += 0x3800;

    if (RSDK.ObjectTileGrip(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0xF0000, 8)) {
        entity->velocity.y = 0;
        RSDK.SetSpriteAnimation(MechaBu->aniFrames, 0, &entity->animator1, true, 0);
        entity->state = MechaBu_Unknown6;
        MechaBu_Unknown6();
    }
    else {
        bool32 collided = false;
        if (entity->velocity.x <= 0)
            collided = RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_RWALL, 0, -0x120000, 0, true);
        else
            collided = RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_LWALL, 0, 0x120000, 0, true);
        if (collided)
            entity->velocity.x = 0;
        MechaBu_CheckPlayerCollisions();
        MechaBu_CheckOnScreen();
    }
}

void MechaBu_EditorDraw(void) {}

void MechaBu_EditorLoad(void)
{
    RSDK_ACTIVE_VAR(MechaBu, planeFilter);
    RSDK_ENUM_VAR(PLANEFILTER_NONE);
    RSDK_ENUM_VAR(PLANEFILTER_A);
    RSDK_ENUM_VAR(PLANEFILTER_B);
}

void MechaBu_Serialize(void) { RSDK_EDITABLE_VAR(MechaBu, VAR_ENUM, planeFilter); }
