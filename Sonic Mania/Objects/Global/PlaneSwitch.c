// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: PlaneSwitch Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled By Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

ObjectPlaneSwitch *PlaneSwitch;

void PlaneSwitch_Update(void)
{
    RSDK_THIS(PlaneSwitch);
    foreach_active(Player, player)
    {
        PlaneSwitch_CheckCollisions(self, player, self->flags, self->size, true, Zone->playerDrawLow, Zone->playerDrawHigh);
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

        self->updateRange.x = abs(self->size * RSDK.Sin256(self->angle) << 11) + 0x200000;
        self->updateRange.y = abs(self->size * RSDK.Cos256(self->angle) << 11) + 0x200000;
        self->visible       = false;
        self->drawOrder     = Zone->drawOrderLow;
        self->negAngle      = (uint8) - (self->angle & 0xFF);
    }
}

void PlaneSwitch_StageLoad(void) { PlaneSwitch->aniFrames = RSDK.LoadSpriteAnimation("Global/PlaneSwitch.bin", SCOPE_STAGE); }

void PlaneSwitch_DrawSprites(void)
{
    RSDK_THIS(PlaneSwitch);
    Vector2 drawPos;

    drawPos.x = self->position.x;
    drawPos.y = self->position.y;
    drawPos.x -= 0x80000;
    drawPos.y -= self->size << 19;
    Zone_RotateOnPivot(&drawPos, &self->position, self->angle);

    self->animator.frameID = self->flags & 3;
    for (int32 i = 0; i < self->size; ++i) {
        RSDK.DrawSprite(&self->animator, &drawPos, false);
        drawPos.x += RSDK.Sin256(self->angle) << 12;
        drawPos.y += RSDK.Cos256(self->angle) << 12;
    }

    drawPos.x = self->position.x + 0x80000;
    drawPos.y = self->position.y - (self->size << 19);

    int32 dx  = (drawPos.x + 0x80000 - self->position.x) >> 8;
    int32 dy  = (drawPos.y - self->position.y) >> 8;
    drawPos.x = (dy * RSDK.Sin256(self->angle)) + dx * RSDK.Cos256(self->angle) + self->position.x;
    drawPos.y = (dy * RSDK.Cos256(self->angle)) - dx * RSDK.Sin256(self->angle) + self->position.y;

    self->animator.frameID = (self->flags >> 2) & 3;
    for (int32 i = 0; i < self->size; ++i) {
        RSDK.DrawSprite(&self->animator, &drawPos, false);
        drawPos.x += RSDK.Sin256(self->angle) << 12;
        drawPos.y += RSDK.Cos256(self->angle) << 12;
    }
}

void PlaneSwitch_CheckCollisions(EntityPlaneSwitch *self, void *o, int32 flags, int32 size, bool32 switchDrawOrder, uint8 low, uint8 high)
{
    Entity *other = (Entity *)o;

    int32 x     = (other->position.x - self->position.x) >> 8;
    int32 y     = (other->position.y - self->position.y) >> 8;
    int32 scanX = (y * RSDK.Sin256(self->negAngle)) + (x * RSDK.Cos256(self->negAngle)) + self->position.x;
    int32 scanY = (y * RSDK.Cos256(self->negAngle)) - (x * RSDK.Sin256(self->negAngle)) + self->position.y;
    int32 pos   = ((other->velocity.y >> 8) * RSDK.Sin256(self->negAngle)) + (other->velocity.x >> 8) * RSDK.Cos256(self->negAngle);
    RSDK.Cos256(self->negAngle);
    RSDK.Sin256(self->negAngle);
    if (!(self->onPath && !other->onGround)) {
        int32 xDif = abs(scanX - self->position.x);
        int32 yDif = abs(scanY - self->position.y);

        if (xDif < 0x180000 && yDif < size << 19) {
            if (scanX + pos >= self->position.x) {
                other->collisionPlane = (flags >> 3) & 1;
                if (switchDrawOrder) {
                    if (!(flags & 4))
                        other->drawOrder = low;
                    else
                        other->drawOrder = high;
                }
            }
            else {
                other->collisionPlane = (flags >> 1) & 1;
                if (switchDrawOrder) {
                    if (!(flags & 1))
                        other->drawOrder = low;
                    else
                        other->drawOrder = high;
                }
            }
        }
    }
}

#if RETRO_INCLUDE_EDITOR
void PlaneSwitch_EditorDraw(void)
{
    RSDK_THIS(PlaneSwitch);
    self->updateRange.x = abs(self->size * RSDK.Sin256(self->angle) << 11) + 0x200000;
    self->updateRange.y = abs(self->size * RSDK.Cos256(self->angle) << 11) + 0x200000;
    self->visible       = false;
    self->drawOrder     = Zone->drawOrderLow;
    self->negAngle      = (uint8) - (uint8)self->angle;

    PlaneSwitch_DrawSprites();
}

void PlaneSwitch_EditorLoad(void)
{
    PlaneSwitch_StageLoad();

    // I dont want clang format to do its magic here since I want a specific formatting so
    // clang-format off
    RSDK_ACTIVE_VAR(PlaneSwitch, flags);
    RSDK_ENUM_VAR("Plane R: A, Layer R: Low,  Plane L: A, Layer L: Low ", PLANESWITCH_PLANEA_RIGHT | PLANESWITCH_LOWLAYER_RIGHT  | PLANESWITCH_PLANEA_LEFT | PLANESWITCH_LOWLAYER_LEFT);
    RSDK_ENUM_VAR("Plane R: A, Layer R: Low,  Plane L: A, Layer L: High", PLANESWITCH_PLANEA_RIGHT | PLANESWITCH_LOWLAYER_RIGHT  | PLANESWITCH_PLANEA_LEFT | PLANESWITCH_HIGHLAYER_LEFT);
    RSDK_ENUM_VAR("Plane R: A, Layer R: Low,  Plane L: B, Layer L: Low ", PLANESWITCH_PLANEA_RIGHT | PLANESWITCH_LOWLAYER_RIGHT  | PLANESWITCH_PLANEB_LEFT | PLANESWITCH_LOWLAYER_LEFT);
    RSDK_ENUM_VAR("Plane R: A, Layer R: Low,  Plane L: B, Layer L: High", PLANESWITCH_PLANEA_RIGHT | PLANESWITCH_LOWLAYER_RIGHT  | PLANESWITCH_PLANEB_LEFT | PLANESWITCH_HIGHLAYER_LEFT);
    RSDK_ENUM_VAR("Plane R: A, Layer R: High, Plane L: A, Layer L: Low ", PLANESWITCH_PLANEA_RIGHT | PLANESWITCH_HIGHLAYER_RIGHT | PLANESWITCH_PLANEA_LEFT | PLANESWITCH_LOWLAYER_LEFT);
    RSDK_ENUM_VAR("Plane R: A, Layer R: High, Plane L: A, Layer L: High", PLANESWITCH_PLANEA_RIGHT | PLANESWITCH_HIGHLAYER_RIGHT | PLANESWITCH_PLANEA_LEFT | PLANESWITCH_HIGHLAYER_LEFT);
    RSDK_ENUM_VAR("Plane R: A, Layer R: High, Plane L: B, Layer L: Low ", PLANESWITCH_PLANEA_RIGHT | PLANESWITCH_HIGHLAYER_RIGHT | PLANESWITCH_PLANEB_LEFT | PLANESWITCH_LOWLAYER_LEFT);
    RSDK_ENUM_VAR("Plane R: A, Layer R: High, Plane L: B, Layer L: High", PLANESWITCH_PLANEA_RIGHT | PLANESWITCH_HIGHLAYER_RIGHT | PLANESWITCH_PLANEB_LEFT | PLANESWITCH_HIGHLAYER_LEFT);
    RSDK_ENUM_VAR("Plane R: B, Layer R: Low,  Plane L: A, Layer L: Low ", PLANESWITCH_PLANEB_RIGHT | PLANESWITCH_LOWLAYER_RIGHT  | PLANESWITCH_PLANEA_LEFT | PLANESWITCH_LOWLAYER_LEFT);
    RSDK_ENUM_VAR("Plane R: B, Layer R: Low,  Plane L: A, Layer L: High", PLANESWITCH_PLANEB_RIGHT | PLANESWITCH_LOWLAYER_RIGHT  | PLANESWITCH_PLANEA_LEFT | PLANESWITCH_HIGHLAYER_LEFT);
    RSDK_ENUM_VAR("Plane R: B, Layer R: Low,  Plane L: B, Layer L: Low ", PLANESWITCH_PLANEB_RIGHT | PLANESWITCH_LOWLAYER_RIGHT  | PLANESWITCH_PLANEB_LEFT | PLANESWITCH_LOWLAYER_LEFT);
    RSDK_ENUM_VAR("Plane R: B, Layer R: Low,  Plane L: B, Layer L: High", PLANESWITCH_PLANEB_RIGHT | PLANESWITCH_LOWLAYER_RIGHT  | PLANESWITCH_PLANEB_LEFT | PLANESWITCH_HIGHLAYER_LEFT);
    RSDK_ENUM_VAR("Plane R: B, Layer R: High, Plane L: A, Layer L: Low ", PLANESWITCH_PLANEB_RIGHT | PLANESWITCH_HIGHLAYER_RIGHT | PLANESWITCH_PLANEA_LEFT | PLANESWITCH_LOWLAYER_LEFT);
    RSDK_ENUM_VAR("Plane R: B, Layer R: High, Plane L: A, Layer L: High", PLANESWITCH_PLANEB_RIGHT | PLANESWITCH_HIGHLAYER_RIGHT | PLANESWITCH_PLANEA_LEFT | PLANESWITCH_HIGHLAYER_LEFT);
    RSDK_ENUM_VAR("Plane R: B, Layer R: High, Plane L: B, Layer L: Low ", PLANESWITCH_PLANEB_RIGHT | PLANESWITCH_HIGHLAYER_RIGHT | PLANESWITCH_PLANEB_LEFT | PLANESWITCH_LOWLAYER_LEFT);
    RSDK_ENUM_VAR("Plane R: B, Layer R: High, Plane L: B, Layer L: High", PLANESWITCH_PLANEB_RIGHT | PLANESWITCH_HIGHLAYER_RIGHT | PLANESWITCH_PLANEB_LEFT | PLANESWITCH_HIGHLAYER_LEFT);
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
