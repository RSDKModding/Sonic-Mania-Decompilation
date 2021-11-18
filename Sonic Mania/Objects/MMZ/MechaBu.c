#include "SonicMania.h"

ObjectMechaBu *MechaBu;

void MechaBu_Update(void)
{
    RSDK_THIS(MechaBu);
    StateMachine_Run(self->state);

    Vector2 offset   = MechaBu_GetSawOffset();
    self->sawPos.x = offset.x;
    self->sawPos.y = offset.y;
    self->sawPos.x += self->position.x;
    self->sawPos.y += self->position.y;
}

void MechaBu_LateUpdate(void) {}

void MechaBu_StaticUpdate(void) {}

void MechaBu_Draw(void)
{
    RSDK_THIS(MechaBu);
    RSDK.DrawSprite(&self->animator1, NULL, false);
    RSDK.DrawSprite(&self->animator3, &self->sawPos, false);
    RSDK.DrawSprite(&self->animator2, NULL, false);
}

void MechaBu_Create(void *data)
{
    RSDK_THIS(MechaBu);

    self->visible = true;
    if (self->planeFilter > 0 && (((uint8)self->planeFilter - 1) & 2))
        self->drawOrder = Zone->drawOrderHigh;
    else
        self->drawOrder = Zone->drawOrderLow;
    self->drawFX |= FX_FLIP;
    self->startPos      = self->position;
    self->startDir      = self->direction;
    self->active        = ACTIVE_BOUNDS;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;
    RSDK.SetSpriteAnimation(MechaBu->aniFrames, 1, &self->animator1, true, 0);
    RSDK.SetSpriteAnimation(MechaBu->aniFrames, 3, &self->animator2, true, 0);
    RSDK.SetSpriteAnimation(MechaBu->aniFrames, 7, &self->animator3, true, 0);
    self->state = MechaBu_State_Setup;
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
    RSDK_THIS(DebugMode);
    CREATE_ENTITY(MechaBu, NULL, self->position.x, self->position.y);
}

void MechaBu_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(MechaBu->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
}

void MechaBu_CheckPlayerCollisions(void)
{
    RSDK_THIS(MechaBu);

    foreach_active(Player, player)
    {
        if (self->planeFilter <= 0 || player->collisionPlane == (((uint8)self->planeFilter - 1) & 1)) {
            if (Player_CheckCollisionTouch(player, &self->sawPos, &MechaBu->hitboxSaw)) {
#if RETRO_USE_PLUS
                if (!Player_CheckMightyUnspin(0x200, player, 2, &player->uncurlTimer))
#endif
                    Player_CheckHit(player, self);
            }
            else if (Player_CheckBadnikTouch(player, self, &MechaBu->hitboxSaw) && Player_CheckBadnikBreak(self, player, false)) {
                EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, self->sawPos.x, self->sawPos.y);
                RSDK.SetSpriteAnimation(MechaBu->aniFrames, 7, &debris->animator, false, 0);
                debris->velocity.x    = RSDK.Rand(-2, 3) << 16;
                debris->velocity.y    = RSDK.Rand(-4, -1) << 16;
                debris->gravity       = 0x3800;
                debris->drawOrder     = self->drawOrder;
                debris->updateRange.x = 0x200000;
                debris->updateRange.y = 0x200000;
                destroyEntity(self);
            }
        }
    }
}

void MechaBu_CheckOnScreen(void)
{
    RSDK_THIS(MechaBu);
    if (!RSDK.CheckOnScreen(self, NULL) && !RSDK.CheckPosOnScreen(&self->startPos, &self->updateRange)) {
        self->position.x = self->startPos.x;
        self->position.y = self->startPos.y;
        self->direction  = self->startDir;
        MechaBu_Create(NULL);
    }
}

Vector2 MechaBu_GetSawOffset(void)
{
    RSDK_THIS(MechaBu);

    Vector2 result;
    switch (self->animator2.animationID) {
        case 4:
            result.x = MechaBu->sawOffsets[8];
            result.y = MechaBu->sawOffsets[9];
            break;
        case 5:
            result.x = MechaBu->sawOffsets[2 * (4 - self->animator2.frameID) + 0];
            result.y = MechaBu->sawOffsets[2 * (4 - self->animator2.frameID) + 1];
            break;
        case 6:
            result.x = MechaBu->sawOffsets[(2 * self->animator2.frameID) + 0];
            result.y = MechaBu->sawOffsets[(2 * self->animator2.frameID) + 1];
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
    self->active     = ACTIVE_NORMAL;
    self->velocity.x = -0x10000;
    self->timer2     = 0;
    self->velocity.y = 0;
    self->state      = MechaBu_Unknown6;
    MechaBu_Unknown6();
}

void MechaBu_Unknown6(void)
{
    RSDK_THIS(MechaBu);

    RSDK.ProcessAnimation(&self->animator1);
    RSDK.ProcessAnimation(&self->animator2);
    RSDK.ProcessAnimation(&self->animator3);

    self->timer2++;
    if (self->timer2 == 60) {
        if (self->activeScreens == 1)
            RSDK.PlaySfx(MechaBu->sfxSawUp, false, 255);
        RSDK.SetSpriteAnimation(MechaBu->aniFrames, 6, &self->animator2, true, 0);
    }
    else if (self->timer2 == 120) {
        if (self->activeScreens == 1)
            RSDK.PlaySfx(MechaBu->sfxSawDown, false, 255);
        self->timer2 = 0;
        RSDK.SetSpriteAnimation(MechaBu->aniFrames, 5, &self->animator2, true, 0);
    }

    self->position.x += self->velocity.x;
    if (RSDK.ObjectTileGrip(self, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0xF0000, 8)) {
        bool32 collidedWall  = false;
        bool32 collidedFloor = false;
        if (self->velocity.x <= 0) {
            collidedWall  = RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_RWALL, 0, -0x120000, 0, true);
            collidedFloor = RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_FLOOR, 0, -0x20000, 0x100000, false);
        }
        else {
            collidedWall  = RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_LWALL, 0, 0x120000, 0, true);
            collidedFloor = RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_FLOOR, 0, 0x20000, 0x100000, false);
        }

        if (!collidedFloor || collidedWall) {
            self->timer = 0;
            self->state = MechaBu_Unknown7;
        }
    }
    else {
        RSDK.SetSpriteAnimation(MechaBu->aniFrames, 1, &self->animator1, true, 0);
        self->timer = 0;

        bool32 collided = false;
        if (self->velocity.x >= 0)
            collided = RSDK.ObjectTileGrip(self, Zone->fgLayers, CMODE_FLOOR, 0, -0x10000, 0xF0000, 8);
        else
            collided = RSDK.ObjectTileGrip(self, Zone->fgLayers, CMODE_FLOOR, 0, 0x10000, 0xF0000, 8);

        if (collided)
            self->state = MechaBu_Unknown7;
        else
            self->state = MechaBu_Unknown8;
    }
    MechaBu_CheckPlayerCollisions();
    MechaBu_CheckOnScreen();
}

void MechaBu_Unknown7(void)
{
    RSDK_THIS(MechaBu);

    RSDK.ProcessAnimation(&self->animator1);
    RSDK.ProcessAnimation(&self->animator2);
    RSDK.ProcessAnimation(&self->animator3);
    if (++self->timer == 48) {
        self->velocity.x = -self->velocity.x;
        self->state      = MechaBu_Unknown6;
        MechaBu_Unknown6();
    }
    MechaBu_CheckPlayerCollisions();
    MechaBu_CheckOnScreen();
}

void MechaBu_Unknown8(void)
{
    RSDK_THIS(MechaBu);

    RSDK.ProcessAnimation(&self->animator1);
    RSDK.ProcessAnimation(&self->animator2);
    RSDK.ProcessAnimation(&self->animator3);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->velocity.y += 0x3800;

    if (RSDK.ObjectTileGrip(self, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0xF0000, 8)) {
        self->velocity.y = 0;
        RSDK.SetSpriteAnimation(MechaBu->aniFrames, 0, &self->animator1, true, 0);
        self->state = MechaBu_Unknown6;
        MechaBu_Unknown6();
    }
    else {
        bool32 collided = false;
        if (self->velocity.x <= 0)
            collided = RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_RWALL, 0, -0x120000, 0, true);
        else
            collided = RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_LWALL, 0, 0x120000, 0, true);
        if (collided)
            self->velocity.x = 0;
        MechaBu_CheckPlayerCollisions();
        MechaBu_CheckOnScreen();
    }
}

void MechaBu_EditorDraw(void)
{
    RSDK_THIS(MechaBu);
    self->sawPos.x = self->position.x + -0x150000;
    self->sawPos.y = self->position.y + -0xF0000;

    MechaBu_Draw();
}

void MechaBu_EditorLoad(void)
{
    MechaBu->aniFrames = RSDK.LoadSpriteAnimation("MMZ/MechaBu.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(MechaBu, planeFilter);
    RSDK_ENUM_VAR("No Filter", PLANEFILTER_NONE);
    RSDK_ENUM_VAR("Plane A", PLANEFILTER_A);
    RSDK_ENUM_VAR("Plane B", PLANEFILTER_B);
}

void MechaBu_Serialize(void) { RSDK_EDITABLE_VAR(MechaBu, VAR_ENUM, planeFilter); }
