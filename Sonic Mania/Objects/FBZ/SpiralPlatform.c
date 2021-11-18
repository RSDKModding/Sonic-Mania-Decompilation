#include "SonicMania.h"

ObjectSpiralPlatform *SpiralPlatform;

void SpiralPlatform_Update(void)
{
    RSDK_THIS(SpiralPlatform);

    self->collision = PLATFORM_C_2;
    Platform_Update();

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionBox(player, self, &SpiralPlatform->hitbox) == 4) {
            if (player->onGround && !player->collisionMode)
                player->hurtFlag = 1;
        }
    }
}

void SpiralPlatform_LateUpdate(void) {}

void SpiralPlatform_StaticUpdate(void) {}

void SpiralPlatform_Draw(void)
{
    RSDK_THIS(SpiralPlatform);
    RSDK.DrawTile(self->tiles, self->tileSize.x >> 20, self->tileSize.y >> 20, &self->drawPos, 0, false);
}

void SpiralPlatform_Create(void *data)
{
    RSDK_THIS(SpiralPlatform);
    self->targetPos.x = 0x400000;
    self->targetPos.y = 0x280000;
    self->tileSize.x  = 0x600000;
    self->tileSize.y  = 0x300000;
    TilePlatform_Create(NULL);

    if (!SceneInfo->inEditor) {
        self->offRange.y <<= 10;
        self->drawPos.y += self->offRange.x;
        self->hitbox.left += 17;
        self->hitbox.right -= 17;
        self->speed <<= 16;
        self->offRange.x <<= 10;
        self->fallY         = self->offRange.x;
        self->active        = ACTIVE_XBOUNDS;
        self->updateRange.y = 0x800000 + self->offRange.y;
        self->state         = SpiralPlatform_Unknown1;
    }
}

void SpiralPlatform_StageLoad(void)
{
    SpiralPlatform->hitbox.left   = -48;
    SpiralPlatform->hitbox.top    = -7;
    SpiralPlatform->hitbox.right  = 48;
    SpiralPlatform->hitbox.bottom = 7;
}

void SpiralPlatform_Unknown1(void)
{
    RSDK_THIS(SpiralPlatform);

    self->fallY += self->speed;
    self->drawPos.y += self->speed;
    self->velocity.y = self->speed;
    if ((self->speed <= 0 && self->fallY <= self->offRange.y) || (self->speed > 0 && self->fallY >= self->offRange.y)) {
        self->fallY     = 0;
        self->drawPos.y = self->centerPos.y;
    }
}

#if RETRO_INCLUDE_EDITOR
void SpiralPlatform_EditorDraw(void) {}

void SpiralPlatform_EditorLoad(void) {}
#endif

void SpiralPlatform_Serialize(void)
{
    RSDK_EDITABLE_VAR(SpiralPlatform, VAR_VECTOR2, offRange);
    RSDK_EDITABLE_VAR(SpiralPlatform, VAR_ENUM, speed);
    RSDK_EDITABLE_VAR(SpiralPlatform, VAR_ENUM, childCount);
}
