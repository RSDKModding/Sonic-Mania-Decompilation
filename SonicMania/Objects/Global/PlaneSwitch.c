// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: PlaneSwitch Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectPlaneSwitch *PlaneSwitch;

void PlaneSwitch_Update(void)
{
    RSDK_THIS(PlaneSwitch);

    foreach_active(Player, player)
    {
        PlaneSwitch_CheckCollisions(self, player, self->flags, self->size, true, Zone->playerDrawGroup[0], Zone->playerDrawGroup[1]);
    }

    self->visible = DebugMode->debugActive;
}

void PlaneSwitch_LateUpdate(void) {}

void PlaneSwitch_StaticUpdate(void) {}

void PlaneSwitch_Draw(void) { PlaneSwitch_DrawSprites(); }

void PlaneSwitch_Create(void *data)
{
    RSDK_THIS(PlaneSwitch);

    RSDK.SetSpriteAnimation(PlaneSwitch->aniFrames, 0, &self->animator, true, 0);

    if (!SceneInfo->inEditor) {
        self->active = ACTIVE_BOUNDS;

        self->updateRange.x = TO_FIXED(32) + abs(self->size * RSDK.Sin256(self->angle) << 11);
        self->updateRange.y = TO_FIXED(32) + abs(self->size * RSDK.Cos256(self->angle) << 11);
        self->visible       = false;
        self->drawGroup     = Zone->objectDrawGroup[0];
        self->negAngle      = -self->angle & 0xFF;
    }
}

void PlaneSwitch_StageLoad(void) { PlaneSwitch->aniFrames = RSDK.LoadSpriteAnimation("Global/PlaneSwitch.bin", SCOPE_STAGE); }

void PlaneSwitch_DrawSprites(void)
{
    RSDK_THIS(PlaneSwitch);

    Vector2 drawPos;

    drawPos.x = self->position.x - TO_FIXED(8);
    drawPos.y = self->position.y - (self->size << 19);
    Zone_RotateOnPivot(&drawPos, &self->position, self->angle);

    self->animator.frameID = self->flags & 3;
    for (int32 i = 0; i < self->size; ++i) {
        RSDK.DrawSprite(&self->animator, &drawPos, false);
        drawPos.x += RSDK.Sin256(self->angle) << 12;
        drawPos.y += RSDK.Cos256(self->angle) << 12;
    }

    drawPos.x = self->position.x + TO_FIXED(8);
    drawPos.y = self->position.y - (self->size << 19);
    Zone_RotateOnPivot(&drawPos, &self->position, self->angle);

    self->animator.frameID = (self->flags >> 2) & 3;
    for (int32 i = 0; i < self->size; ++i) {
        RSDK.DrawSprite(&self->animator, &drawPos, false);
        drawPos.x += RSDK.Sin256(self->angle) << 12;
        drawPos.y += RSDK.Cos256(self->angle) << 12;
    }
}

// Custom function, but it allows me to majorly shrink any planeSwitch code
// it's based on all the repeated planeswitch code, just made more modular
// (and also it means you can add planeswitches to basically anything with ease :P)
void PlaneSwitch_CheckCollisions(EntityPlaneSwitch *self, void *o, int32 flags, int32 size, bool32 switchdrawGroup, uint8 low, uint8 high)
{
    Entity *other = (Entity *)o;

    Vector2 pivotPos = other->position;
    Vector2 pivotVel = other->velocity;

    Zone_RotateOnPivot(&pivotPos, &self->position, self->negAngle);
    Zone_RotateOnPivot(&pivotVel, &self->velocity, self->negAngle);

    if (!self->onPath || other->onGround) {
        if (abs(pivotPos.x - self->position.x) < TO_FIXED(24) && abs(pivotPos.y - self->position.y) < size << 19) {
            if (pivotPos.x + pivotVel.x >= self->position.x) {
                other->collisionPlane = (flags >> 3) & 1; // collision plane bit
                if (switchdrawGroup) {
                    if (!(flags & 4)) // priority bit
                        other->drawGroup = low;
                    else
                        other->drawGroup = high;
                }
            }
            else {
                other->collisionPlane = (flags >> 1) & 1; // collision plane bit
                if (switchdrawGroup) {
                    if (!(flags & 1)) // priority bit
                        other->drawGroup = low;
                    else
                        other->drawGroup = high;
                }
            }
        }
    }
}

#if GAME_INCLUDE_EDITOR
void PlaneSwitch_EditorDraw(void)
{
    RSDK_THIS(PlaneSwitch);

    self->angle &= 0xFF;
    self->flags &= 0xF;
    if (!self->size)
        self->size = 1;

    self->updateRange.x = TO_FIXED(32) + abs(self->size * RSDK.Sin256(self->angle) << 11);
    self->updateRange.y = TO_FIXED(32) + abs(self->size * RSDK.Cos256(self->angle) << 11);
    self->visible       = false;
    self->drawGroup     = Zone->objectDrawGroup[0];
    self->negAngle      = -self->angle & 0xFF;

    PlaneSwitch_DrawSprites();
}

void PlaneSwitch_EditorLoad(void)
{
    PlaneSwitch_StageLoad();

    // I dont want clang format to do its magic here since I want a specific formatting so
    // clang-format off
    RSDK_ACTIVE_VAR(PlaneSwitch, flags);
    RSDK_ENUM_VAR("Plane R: A, Draw Group R: Low,  Plane L: A, Draw Group L: Low ", PLANESWITCH_PLANEA_RIGHT | PLANESWITCH_LOWLAYER_RIGHT  | PLANESWITCH_PLANEA_LEFT | PLANESWITCH_LOWLAYER_LEFT);
    RSDK_ENUM_VAR("Plane R: A, Draw Group R: Low,  Plane L: A, Draw Group L: High", PLANESWITCH_PLANEA_RIGHT | PLANESWITCH_LOWLAYER_RIGHT  | PLANESWITCH_PLANEA_LEFT | PLANESWITCH_HIGHLAYER_LEFT);
    RSDK_ENUM_VAR("Plane R: A, Draw Group R: Low,  Plane L: B, Draw Group L: Low ", PLANESWITCH_PLANEA_RIGHT | PLANESWITCH_LOWLAYER_RIGHT  | PLANESWITCH_PLANEB_LEFT | PLANESWITCH_LOWLAYER_LEFT);
    RSDK_ENUM_VAR("Plane R: A, Draw Group R: Low,  Plane L: B, Draw Group L: High", PLANESWITCH_PLANEA_RIGHT | PLANESWITCH_LOWLAYER_RIGHT  | PLANESWITCH_PLANEB_LEFT | PLANESWITCH_HIGHLAYER_LEFT);
    RSDK_ENUM_VAR("Plane R: A, Draw Group R: High, Plane L: A, Draw Group L: Low ", PLANESWITCH_PLANEA_RIGHT | PLANESWITCH_HIGHLAYER_RIGHT | PLANESWITCH_PLANEA_LEFT | PLANESWITCH_LOWLAYER_LEFT);
    RSDK_ENUM_VAR("Plane R: A, Draw Group R: High, Plane L: A, Draw Group L: High", PLANESWITCH_PLANEA_RIGHT | PLANESWITCH_HIGHLAYER_RIGHT | PLANESWITCH_PLANEA_LEFT | PLANESWITCH_HIGHLAYER_LEFT);
    RSDK_ENUM_VAR("Plane R: A, Draw Group R: High, Plane L: B, Draw Group L: Low ", PLANESWITCH_PLANEA_RIGHT | PLANESWITCH_HIGHLAYER_RIGHT | PLANESWITCH_PLANEB_LEFT | PLANESWITCH_LOWLAYER_LEFT);
    RSDK_ENUM_VAR("Plane R: A, Draw Group R: High, Plane L: B, Draw Group L: High", PLANESWITCH_PLANEA_RIGHT | PLANESWITCH_HIGHLAYER_RIGHT | PLANESWITCH_PLANEB_LEFT | PLANESWITCH_HIGHLAYER_LEFT);
    RSDK_ENUM_VAR("Plane R: B, Draw Group R: Low,  Plane L: A, Draw Group L: Low ", PLANESWITCH_PLANEB_RIGHT | PLANESWITCH_LOWLAYER_RIGHT  | PLANESWITCH_PLANEA_LEFT | PLANESWITCH_LOWLAYER_LEFT);
    RSDK_ENUM_VAR("Plane R: B, Draw Group R: Low,  Plane L: A, Draw Group L: High", PLANESWITCH_PLANEB_RIGHT | PLANESWITCH_LOWLAYER_RIGHT  | PLANESWITCH_PLANEA_LEFT | PLANESWITCH_HIGHLAYER_LEFT);
    RSDK_ENUM_VAR("Plane R: B, Draw Group R: Low,  Plane L: B, Draw Group L: Low ", PLANESWITCH_PLANEB_RIGHT | PLANESWITCH_LOWLAYER_RIGHT  | PLANESWITCH_PLANEB_LEFT | PLANESWITCH_LOWLAYER_LEFT);
    RSDK_ENUM_VAR("Plane R: B, Draw Group R: Low,  Plane L: B, Draw Group L: High", PLANESWITCH_PLANEB_RIGHT | PLANESWITCH_LOWLAYER_RIGHT  | PLANESWITCH_PLANEB_LEFT | PLANESWITCH_HIGHLAYER_LEFT);
    RSDK_ENUM_VAR("Plane R: B, Draw Group R: High, Plane L: A, Draw Group L: Low ", PLANESWITCH_PLANEB_RIGHT | PLANESWITCH_HIGHLAYER_RIGHT | PLANESWITCH_PLANEA_LEFT | PLANESWITCH_LOWLAYER_LEFT);
    RSDK_ENUM_VAR("Plane R: B, Draw Group R: High, Plane L: A, Draw Group L: High", PLANESWITCH_PLANEB_RIGHT | PLANESWITCH_HIGHLAYER_RIGHT | PLANESWITCH_PLANEA_LEFT | PLANESWITCH_HIGHLAYER_LEFT);
    RSDK_ENUM_VAR("Plane R: B, Draw Group R: High, Plane L: B, Draw Group L: Low ", PLANESWITCH_PLANEB_RIGHT | PLANESWITCH_HIGHLAYER_RIGHT | PLANESWITCH_PLANEB_LEFT | PLANESWITCH_LOWLAYER_LEFT);
    RSDK_ENUM_VAR("Plane R: B, Draw Group R: High, Plane L: B, Draw Group L: High", PLANESWITCH_PLANEB_RIGHT | PLANESWITCH_HIGHLAYER_RIGHT | PLANESWITCH_PLANEB_LEFT | PLANESWITCH_HIGHLAYER_LEFT);
    // clang-format on
}
#endif

void PlaneSwitch_Serialize(void)
{
    RSDK_EDITABLE_VAR(PlaneSwitch, VAR_ENUM, flags);
    RSDK_EDITABLE_VAR(PlaneSwitch, VAR_ENUM, size);
    RSDK_EDITABLE_VAR(PlaneSwitch, VAR_INT32, angle);
    RSDK_EDITABLE_VAR(PlaneSwitch, VAR_BOOL, onPath);
}
