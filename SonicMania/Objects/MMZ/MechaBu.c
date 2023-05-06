// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: MechaBu Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectMechaBu *MechaBu;

void MechaBu_Update(void)
{
    RSDK_THIS(MechaBu);

    StateMachine_Run(self->state);

    Vector2 offset = MechaBu_GetSawOffset();
    self->sawPos.x = self->position.x + offset.x;
    self->sawPos.y = self->position.y + offset.y;
}

void MechaBu_LateUpdate(void) {}

void MechaBu_StaticUpdate(void) {}

void MechaBu_Draw(void)
{
    RSDK_THIS(MechaBu);

    RSDK.DrawSprite(&self->badnikAnimator, NULL, false);
    RSDK.DrawSprite(&self->sawAnimator, &self->sawPos, false);
    RSDK.DrawSprite(&self->hornAnimator, NULL, false);
}

void MechaBu_Create(void *data)
{
    RSDK_THIS(MechaBu);

    self->visible = true;
    if (self->planeFilter > 0 && (((uint8)self->planeFilter - 1) & 2))
        self->drawGroup = Zone->objectDrawGroup[1];
    else
        self->drawGroup = Zone->objectDrawGroup[0];

    self->drawFX |= FX_FLIP;
    self->startPos      = self->position;
    self->startDir      = self->direction;
    self->active        = ACTIVE_BOUNDS;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;

    RSDK.SetSpriteAnimation(MechaBu->aniFrames, 1, &self->badnikAnimator, true, 0);
    RSDK.SetSpriteAnimation(MechaBu->aniFrames, 3, &self->hornAnimator, true, 0);
    RSDK.SetSpriteAnimation(MechaBu->aniFrames, 7, &self->sawAnimator, true, 0);

    self->state = MechaBu_State_Init;
}

void MechaBu_StageLoad(void)
{
    if (RSDK.CheckSceneFolder("MMZ"))
        MechaBu->aniFrames = RSDK.LoadSpriteAnimation("MMZ/MechaBu.bin", SCOPE_STAGE);

    MechaBu->hitboxSaw.left   = -14;
    MechaBu->hitboxSaw.top    = -14;
    MechaBu->hitboxSaw.right  = 14;
    MechaBu->hitboxSaw.bottom = 14;

    MechaBu->hitboxUnused.left   = -8;
    MechaBu->hitboxUnused.top    = -12;
    MechaBu->hitboxUnused.right  = 8;
    MechaBu->hitboxUnused.bottom = 12;

    DEBUGMODE_ADD_OBJ(MechaBu);

    MechaBu->sfxSawUp   = RSDK.GetSfx("MMZ/SawUp.wav");
    MechaBu->sfxSawDown = RSDK.GetSfx("MMZ/SawDown.wav");
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
            Vector2 posStore = self->position;
            self->position   = self->sawPos;
            if (Player_CheckCollisionTouch(player, self, &MechaBu->hitboxSaw)) {
                self->position = posStore;
#if MANIA_USE_PLUS
                if (!Player_CheckMightyUnspin(player, 0x200, 2, &player->uncurlTimer))
#endif
                    Player_Hurt(player, self);
            }
            else {
                self->position = posStore;
                if (Player_CheckBadnikTouch(player, self, &MechaBu->hitboxSaw) && Player_CheckBadnikBreak(player, self, false)) {
                    EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, self->sawPos.x, self->sawPos.y);
                    RSDK.SetSpriteAnimation(MechaBu->aniFrames, 7, &debris->animator, false, 0);
                    debris->velocity.x      = RSDK.Rand(-2, 3) << 16;
                    debris->velocity.y      = RSDK.Rand(-4, -1) << 16;
                    debris->gravityStrength = 0x3800;
                    debris->drawGroup       = self->drawGroup;
                    debris->updateRange.x   = 0x200000;
                    debris->updateRange.y   = 0x200000;
                    destroyEntity(self);
                }
            }
        }
    }
}

void MechaBu_CheckOffScreen(void)
{
    RSDK_THIS(MechaBu);

    if (!RSDK.CheckOnScreen(self, NULL) && !RSDK.CheckPosOnScreen(&self->startPos, &self->updateRange)) {
        self->position  = self->startPos;
        self->direction = self->startDir;
        MechaBu_Create(NULL);
    }
}

Vector2 MechaBu_GetSawOffset(void)
{
    RSDK_THIS(MechaBu);

    Vector2 offset;
    switch (self->hornAnimator.animationID) {
        case 4:
            offset.x = MechaBu->sawOffsets[8];
            offset.y = MechaBu->sawOffsets[9];
            break;

        case 5:
            offset.x = MechaBu->sawOffsets[2 * (4 - self->hornAnimator.frameID) + 0];
            offset.y = MechaBu->sawOffsets[2 * (4 - self->hornAnimator.frameID) + 1];
            break;

        case 6:
            offset.x = MechaBu->sawOffsets[(2 * self->hornAnimator.frameID) + 0];
            offset.y = MechaBu->sawOffsets[(2 * self->hornAnimator.frameID) + 1];
            break;

        default:
            offset.x = MechaBu->sawOffsets[0];
            offset.y = MechaBu->sawOffsets[1];
            break;
    }

    return offset;
}

void MechaBu_State_Init(void)
{
    RSDK_THIS(MechaBu);

    self->active     = ACTIVE_NORMAL;
    self->sawTimer   = 0;
    self->velocity.x = -0x10000;
    self->velocity.y = 0;

    self->state = MechaBu_State_Moving;
    MechaBu_State_Moving();
}

void MechaBu_State_Moving(void)
{
    RSDK_THIS(MechaBu);

    RSDK.ProcessAnimation(&self->badnikAnimator);
    RSDK.ProcessAnimation(&self->hornAnimator);
    RSDK.ProcessAnimation(&self->sawAnimator);

    self->sawTimer++;
    if (self->sawTimer == 60) {
        if (self->onScreen == 1)
            RSDK.PlaySfx(MechaBu->sfxSawUp, false, 255);

        RSDK.SetSpriteAnimation(MechaBu->aniFrames, 6, &self->hornAnimator, true, 0);
    }
    else if (self->sawTimer == 120) {
        if (self->onScreen == 1)
            RSDK.PlaySfx(MechaBu->sfxSawDown, false, 255);

        self->sawTimer = 0;
        RSDK.SetSpriteAnimation(MechaBu->aniFrames, 5, &self->hornAnimator, true, 0);
    }

    self->position.x += self->velocity.x;

    if (RSDK.ObjectTileGrip(self, Zone->collisionLayers, CMODE_FLOOR, 0, 0, 0xF0000, 8)) {
        bool32 collidedWall  = false;
        bool32 collidedFloor = false;
        if (self->velocity.x <= 0) {
            collidedWall  = RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_RWALL, 0, -0x120000, 0, true);
            collidedFloor = RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_FLOOR, 0, -0x20000, 0x100000, false);
        }
        else {
            collidedWall  = RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_LWALL, 0, 0x120000, 0, true);
            collidedFloor = RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_FLOOR, 0, 0x20000, 0x100000, false);
        }

        if (!collidedFloor || collidedWall) {
            self->timer = 0;
            self->state = MechaBu_State_Stopped;
        }
    }
    else {
        RSDK.SetSpriteAnimation(MechaBu->aniFrames, 1, &self->badnikAnimator, true, 0);
        self->timer = 0;

        bool32 collided = false;
        if (self->velocity.x >= 0)
            collided = RSDK.ObjectTileGrip(self, Zone->collisionLayers, CMODE_FLOOR, 0, -0x10000, 0xF0000, 8);
        else
            collided = RSDK.ObjectTileGrip(self, Zone->collisionLayers, CMODE_FLOOR, 0, 0x10000, 0xF0000, 8);

        self->state = collided ? MechaBu_State_Stopped : MechaBu_State_Falling;
    }

    MechaBu_CheckPlayerCollisions();
    MechaBu_CheckOffScreen();
}

void MechaBu_State_Stopped(void)
{
    RSDK_THIS(MechaBu);

    RSDK.ProcessAnimation(&self->badnikAnimator);
    RSDK.ProcessAnimation(&self->hornAnimator);
    RSDK.ProcessAnimation(&self->sawAnimator);

    if (++self->timer == 48) {
        self->velocity.x = -self->velocity.x;

        self->state = MechaBu_State_Moving;
        MechaBu_State_Moving();
    }

    MechaBu_CheckPlayerCollisions();
    MechaBu_CheckOffScreen();
}

void MechaBu_State_Falling(void)
{
    RSDK_THIS(MechaBu);

    RSDK.ProcessAnimation(&self->badnikAnimator);
    RSDK.ProcessAnimation(&self->hornAnimator);
    RSDK.ProcessAnimation(&self->sawAnimator);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->velocity.y += 0x3800;

    if (RSDK.ObjectTileGrip(self, Zone->collisionLayers, CMODE_FLOOR, 0, 0, 0xF0000, 8)) {
        self->velocity.y = 0;
        RSDK.SetSpriteAnimation(MechaBu->aniFrames, 0, &self->badnikAnimator, true, 0);

        self->state = MechaBu_State_Moving;
        MechaBu_State_Moving();
    }
    else {
        bool32 collided = false;
        if (self->velocity.x <= 0)
            collided = RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_RWALL, 0, -0x120000, 0, true);
        else
            collided = RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_LWALL, 0, 0x120000, 0, true);

        if (collided)
            self->velocity.x = 0;

        MechaBu_CheckPlayerCollisions();
        MechaBu_CheckOffScreen();
    }
}

#if GAME_INCLUDE_EDITOR
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
    RSDK_ENUM_VAR("None", PLANEFILTER_NONE);
    RSDK_ENUM_VAR("AL", PLANEFILTER_AL);
    RSDK_ENUM_VAR("BL", PLANEFILTER_BL);
    RSDK_ENUM_VAR("AH", PLANEFILTER_AH);
    RSDK_ENUM_VAR("BH", PLANEFILTER_BH);
}
#endif

void MechaBu_Serialize(void) { RSDK_EDITABLE_VAR(MechaBu, VAR_ENUM, planeFilter); }
