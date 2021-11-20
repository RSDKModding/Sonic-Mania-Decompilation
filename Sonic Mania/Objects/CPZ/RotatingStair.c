#include "SonicMania.h"

ObjectRotatingStair *RotatingStair;

void RotatingStair_Update(void) { Platform_Update(); }

void RotatingStair_LateUpdate(void) {}

void RotatingStair_StaticUpdate(void) {}

void RotatingStair_Draw(void)
{
    RSDK_THIS(RotatingStair);
    RSDK.DrawSprite(&self->animator, &self->drawPos, false);
}

void RotatingStair_Create(void *data)
{
    RSDK_THIS(RotatingStair);
    self->frameID   = 2;
    self->collision = PLATFORM_C_SOLID_ALL;
    self->speed     = 3;
    if (self->direction)
        self->amplitude.x = -self->amplitude.x;
    if (self->mode & 1)
        self->amplitude.x = -self->amplitude.x;

    int32 typeStore = self->mode;
    self->mode    = PLATFORM_MOVING;
    Platform_Create(NULL);
    self->mode = typeStore;

    if (self->mode >= 4) {
        self->duration = 120 * self->speed;
        self->state    = RotatingStair_Unknown2;
        self->interval = self->duration + 512;
    }
    else {
        self->state = RotatingStair_Unknown1;
    }
}

void RotatingStair_StageLoad(void) { RotatingStair->flag = 0; }

void RotatingStair_Unknown1(void)
{
    RSDK_THIS(RotatingStair);
    int32 timer = Zone->timer + self->oscOff;
    int32 drawX = -self->drawPos.x;
    int32 drawY = -self->drawPos.y;

    int32 dir = 0;
    if (self->mode & 1)
        dir = self->mode - (((3 * timer) >> 9) & 3) - 2;
    else
        dir = ((3 * timer) >> 9) + self->mode;
    switch (dir & 3) {
        case 0:
            self->drawPos.x = self->amplitude.x * RSDK.Cos1024(timer * self->speed) + self->centerPos.x;
            self->drawPos.y = self->centerPos.y + (self->amplitude.y << 10);
            break;
        case 1:
            self->drawPos.x = self->centerPos.x - (self->amplitude.x << 10);
            self->drawPos.y = self->centerPos.y - self->amplitude.y * RSDK.Cos1024(timer * self->speed);
            break;
        case 2:
            self->drawPos.x = self->centerPos.x - self->amplitude.x * RSDK.Cos1024(timer * self->speed);
            self->drawPos.y = self->centerPos.y - (self->amplitude.y << 10);
            break;
        case 3:
            self->drawPos.x = self->centerPos.x + (self->amplitude.x << 10);
            self->drawPos.y = self->amplitude.y * RSDK.Cos1024(timer * self->speed) + self->centerPos.y;
            break;
    }
    self->velocity.x = drawX + self->drawPos.x;
    self->velocity.y = drawY + self->drawPos.y;
}

void RotatingStair_Unknown2(void)
{
    RSDK_THIS(RotatingStair);
    int32 drawX = -self->drawPos.x;
    int32 drawY = -self->drawPos.y;

    int32 timer = 0;
    if (self->speed * (Zone->timer + self->oscOff) % self->interval >= self->duration)
        timer = self->speed * (Zone->timer + self->oscOff) % self->interval - self->duration;

    int32 dir = 0;
    if (self->mode & 1)
        dir = self->mode - ((self->speed * (Zone->timer + self->oscOff) / self->interval) & 3) - 2;
    else
        dir = self->speed * (Zone->timer + self->oscOff) / self->interval + self->mode;
    switch (dir) {
        case 0:
            self->drawPos.x = self->amplitude.x * RSDK.Cos1024(timer) + self->centerPos.x;
            self->drawPos.y = self->centerPos.y + (self->amplitude.y << 10);
            break;
        case 1:
            self->drawPos.x = self->centerPos.x - (self->amplitude.x << 10);
            self->drawPos.y = self->centerPos.y - self->amplitude.y * RSDK.Cos1024(timer + 512);
            break;
        case 2:
            self->drawPos.x = self->centerPos.x - self->amplitude.x * RSDK.Cos1024(timer);
            self->drawPos.y = self->centerPos.y - (self->amplitude.y << 10);
            break;
        case 3:
            self->drawPos.x = self->centerPos.x + (self->amplitude.x << 10);
            self->drawPos.y = self->amplitude.y * RSDK.Cos1024(timer + 512) + self->centerPos.y;
            break;
    }
    self->velocity.x = drawX + self->drawPos.x;
    self->velocity.y = drawY + self->drawPos.y;
}

void RotatingStair_EditorDraw(void)
{
    RSDK_THIS(RotatingStair);

    if (self->direction)
        self->amplitude.x = -self->amplitude.x;
    if (self->mode & 1)
        self->amplitude.x = -self->amplitude.x;

    int32 typeStore = self->mode;
    self->mode    = PLATFORM_MOVING;
    Platform_Create(NULL);
    self->mode = typeStore;

    if (self->mode >= 4) {
        self->duration = 120 * self->speed;
        self->interval = self->duration + 512;
        RotatingStair_Unknown2();
    }
    else {
        RotatingStair_Unknown1();
    }

    RotatingStair_Draw();
}

void RotatingStair_EditorLoad(void) {}

void RotatingStair_Serialize(void)
{
    RSDK_EDITABLE_VAR(RotatingStair, VAR_ENUM, mode);
    RSDK_EDITABLE_VAR(RotatingStair, VAR_VECTOR2, amplitude);
    RSDK_EDITABLE_VAR(RotatingStair, VAR_ENUM, childCount);
    RSDK_EDITABLE_VAR(RotatingStair, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(RotatingStair, VAR_UINT16, oscOff);
}
