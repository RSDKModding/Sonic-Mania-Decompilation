#include "SonicMania.h"

ObjectTeeterTotter *TeeterTotter;

void TeeterTotter_Update(void)
{
    RSDK_THIS(TeeterTotter);
    StateMachine_Run(self->state);
    RSDK.ProcessAnimation(&self->animator);
}

void TeeterTotter_LateUpdate(void) {}

void TeeterTotter_StaticUpdate(void) {}

void TeeterTotter_Draw(void)
{
    RSDK_THIS(TeeterTotter);

    int32 x       = self->position.x;
    int32 y       = self->position.y;
    int32 offsetX = 0x100000 - (self->length << 21) + x;

    for (int32 i = 0; i < 2 * self->length; ++i) {
        if (!((1 << i) & self->inactiveSegments)) {
            Vector2 drawPos;
            drawPos.x = (offsetX + (i << 21)) & 0xFFFF0000;
            drawPos.y = (self->field_90[i] + y) & 0xFFFF0000;
            RSDK.SetSpriteAnimation(TeeterTotter->aniFrames, 0, &self->animator, true, (i + self->color) % 2);
            RSDK.DrawSprite(&self->animator, &drawPos, false);
        }
    }
}

void TeeterTotter_Create(void *data)
{
    RSDK_THIS(TeeterTotter);

    if (self->length >= 0x10 || self->length) {
        if (self->length > 0x10)
            self->length = 0x10;
    }
    else {
        self->length = 1;
    }

    self->active        = ACTIVE_BOUNDS;
    self->drawOrder     = Zone->drawOrderLow;
    self->origin        = self->position;
    self->updateRange.x = (self->length + 2) << 22;
    self->visible       = true;
    self->drawFX        = FX_FLIP;
    self->updateRange.y = (self->length + 4) << 21;

    self->hitbox.left   = -16;
    self->hitbox.top    = -16;
    self->hitbox.right  = 16;
    self->hitbox.bottom = 16;
    self->state         = TeeterTotter_State_Setup;
}

void TeeterTotter_StageLoad(void)
{
    if (RSDK.CheckStageFolder("TMZ1") || RSDK.CheckStageFolder("TMZ2"))
        TeeterTotter->aniFrames = RSDK.LoadSpriteAnimation("TMZ1/TeeterTotter.bin", SCOPE_STAGE);
}

int32 TeeterTotter_CheckPlayerCollisions(void)
{
    RSDK_THIS(TeeterTotter);
    int32 id = -1;

    int32 ids[] = { -1, -1, -1, -1 };

    int32 storeX = self->position.x;
    int32 storeY = self->position.y;

    int32 x = 0x100000 - (self->length << 21) + self->position.x;

    for (int32 i = 0; i < 2 * self->length; ++i) {
        if (!((1 << id) & self->inactiveSegments)) {
            self->position.x = (x + (i << 21)) & 0xFFFF0000;
            self->position.y = (self->field_90[i] + storeY) & 0xFFFF0000;
            foreach_active(Player, player)
            {
                int32 playerID = RSDK.GetEntityID(player);
                if (self->playerIDs[playerID] == i) {
                    player->position.y += self->field_110[i];
                    player->position.y += 0x10000;
                }
                if (Player_CheckCollisionBox(player, self, &self->hitbox) == C_TOP) {
                    if (!player->sidekick)
                        id = i;
                    ids[playerID] = i;
                }
            }
        }
    }

    self->position.x = storeX;
    self->position.y = storeY;

    for (int32 i = 0; i < PLAYER_MAX; ++i) self->playerIDs[i] = ids[i];

    return id;
}

void TeeterTotter_ProcessSegmentGravity(void)
{
    RSDK_THIS(TeeterTotter);

    int32 x       = self->position.x;
    int32 y       = self->position.y;
    int32 offsetX = (0x100000 - (self->length << 21)) + x;

    for (int32 i = 0; i < 2 * self->length; ++i) {
        if (!((1 << i) & self->inactiveSegments)) {
            self->field_110[i] += 0x3800;
            self->field_90[i] += self->field_110[i];

            Vector2 pos;
            pos.x = (offsetX + (i << 21)) & 0xFFFF0000;
            pos.y = (self->field_90[i] + y) & 0xFFFF0000;
            if (!RSDK.CheckPosOnScreen(&pos, &self->updateRange))
                self->inactiveSegments |= 1 << i;
        }
    }
}

void TeeterTotter_Unknown3(void)
{
    RSDK_THIS(TeeterTotter);

    uint8 len = self->length;
    for (int32 i = 0; i < 2 * self->length; ++i) {
        int8 val = i - len + 1;
        if (i - len < 0)
            val = i - len;
        self->field_90[i] = (self->field_84 >> 1) * (int8)(2 * val + 2 * ((int8)(2 * val) <= 0) - 1);
    }
}

void TeeterTotter_State_Setup(void)
{
    RSDK_THIS(TeeterTotter);

    self->field_8C = 0;
    self->field_84 = 0;
    self->field_88 = 0;

    for (int32 i = 0; i < 2 * self->length; ++i) {
        self->field_90[i]  = 0;
        self->field_110[i] = 0;
    }
    self->inactiveSegments = 0;

    self->visible = true;
    self->active  = ACTIVE_NORMAL;
    self->state   = TeeterTotter_State_Unknown1;
}

void TeeterTotter_State_Unknown1(void)
{
    RSDK_THIS(TeeterTotter);

    int32 prevVal[32];
    for (int32 i = 0; i < 2 * self->length; ++i) {
        prevVal[i] = self->field_90[i];
    }

    TeeterTotter_Unknown3();

    for (int32 i = 0; i < 2 * self->length; ++i) {
        self->field_110[i] = self->field_90[i] - prevVal[i];
    }

    int32 val = TeeterTotter_CheckPlayerCollisions();
    if (val > -1) {
        int32 id = val - self->length;
        if (id >= 0)
            ++id;
        self->field_88 += 32 * id;
    }
    self->field_84 += self->field_88;

    if (abs(self->field_84) > 0x200000)
        self->state = TeeterTotter_State_Unknown2;
}

void TeeterTotter_State_Unknown2(void)
{
    RSDK_THIS(TeeterTotter);
    TeeterTotter_ProcessSegmentGravity();

    bool32 flag = true;
    for (int32 i = 0; i < 2 * self->length; ++i) {
        flag &= ((1 << i) & self->inactiveSegments) != 0;
    }

    if (flag || !(2 * self->length)) {
        if (!RSDK.CheckOnScreen(self, NULL) && !RSDK.CheckPosOnScreen(&self->origin, &self->updateRange)) {
            self->position.x = self->origin.x;
            self->position.y = self->origin.y;
            self->visible    = false;
            self->active     = ACTIVE_BOUNDS;
            self->state      = TeeterTotter_State_Setup;
        }
    }
}

#if RETRO_INCLUDE_EDITOR
void TeeterTotter_EditorDraw(void)
{
    RSDK_THIS(TeeterTotter);

    if (self->length >= 0x10 || self->length) {
        if (self->length > 0x10)
            self->length = 0x10;
    }
    else {
        self->length = 1;
    }

    self->origin        = self->position;
    self->updateRange.x = (self->length + 2) << 22;
    self->updateRange.y = (self->length + 4) << 21;

    TeeterTotter_Draw();
}

void TeeterTotter_EditorLoad(void) { TeeterTotter->aniFrames = RSDK.LoadSpriteAnimation("TMZ1/TeeterTotter.bin", SCOPE_STAGE); }
#endif

void TeeterTotter_Serialize(void)
{
    RSDK_EDITABLE_VAR(TeeterTotter, VAR_UINT8, color);
    RSDK_EDITABLE_VAR(TeeterTotter, VAR_UINT32, length);
}
