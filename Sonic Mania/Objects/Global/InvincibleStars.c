#include "SonicMania.h"

ObjectInvincibleStars *InvincibleStars;

void InvincibleStars_Update(void)
{
    RSDK_THIS(InvincibleStars);
    EntityPlayer *player = (EntityPlayer *)self->parent;
    if (player) {
        self->starAngle[2] = (self->starAngle[2] + 1) % 12;
        self->starAngle[3] = (self->starAngle[3] + 1) % 10;

        for (int32 i = 8 - 1; i > 0; --i) {
            self->starPos[i].x = self->starPos[i - 1].x;
            self->starPos[i].y = self->starPos[i - 1].y;
        }
        self->starPos[0].x = player->position.x;
        self->starPos[0].y = player->position.y;

        Hitbox *playerHitbox = Player_GetHitbox(player);
        if (player->direction & FLIP_X)
            self->starPos[0].x += (playerHitbox->left << 15) - (playerHitbox->right << 15) - (playerHitbox->left << 16);
        else
            self->starPos[0].x += ((playerHitbox->right + 2 * playerHitbox->left) << 15) - (playerHitbox->left << 15);
        if (player->direction & 2 && player->invertGravity != 1)
            self->starPos[0].y += (playerHitbox->top << 15) - (playerHitbox->bottom << 15) - (playerHitbox->top << 16);
        else
            self->starPos[0].y += ((playerHitbox->bottom + 2 * playerHitbox->top) << 15) - (playerHitbox->top << 15);
        if (player->direction) {
            self->starAngle[0] -= 144;
            self->starAngle[1] -= 16;
        }
        else {
            self->starAngle[0] += 144;
            self->starAngle[1] += 16;
        }
        self->starAngle[0] &= 0x1FF;
        self->starAngle[1] &= 0x1FF;

        if (player->invincibleTimer < 32)
            self->alpha = 8 * player->invincibleTimer;
        self->drawOrder  = player->drawOrder;
        self->visible    = player->visible || (player->state == Ice_State_FrozenPlayer);
        self->starOffset = 11;
    }
    else {
        destroyEntity(self);
    }
}

void InvincibleStars_LateUpdate(void) {}

void InvincibleStars_StaticUpdate(void) {}

void InvincibleStars_Draw(void)
{
    RSDK_THIS(InvincibleStars);
    EntityPlayer *player = (EntityPlayer *)self->parent;
    if (player) {
        if (player->isChibi) {
            self->drawFX |= FX_SCALE;
            self->scale.x = 0x100;
            self->scale.y = 0x100;
        }
        else {
            if (player->drawFX & FX_SCALE)
                self->drawFX |= FX_SCALE;
            else
                self->drawFX &= ~FX_SCALE;
            self->scale.x = player->scale.x;
            self->scale.y = player->scale.y;
        }
    }

    Vector2 drawPos;
    drawPos.x                    = (RSDK.Cos512(self->starAngle[1] + 0x74) << self->starOffset) + self->starPos[7].x;
    drawPos.y                    = (RSDK.Sin512(self->starAngle[1] + 0x74) << self->starOffset) + self->starPos[7].y;
    self->starsData[3].frameID = self->starAngle[2];
    RSDK.DrawSprite(&self->starsData[3], &drawPos, 0);

    drawPos.x = (RSDK.Cos512(self->starAngle[1] + 0x74) << self->starOffset) + self->starPos[7].x;
    drawPos.y = (RSDK.Sin512(self->starAngle[1] + 0x74) << self->starOffset) + self->starPos[7].y;
    self->starsData[3].frameID += 6;
    RSDK.DrawSprite(&self->starsData[3], &drawPos, 0);

    drawPos.x                    = (RSDK.Cos512(self->starAngle[1]) << self->starOffset) + self->starPos[5].x;
    drawPos.y                    = (RSDK.Sin512(self->starAngle[1]) << self->starOffset) + self->starPos[5].y;
    self->starsData[2].frameID = self->starAngle[2];
    RSDK.DrawSprite(&self->starsData[2], &drawPos, 0);

    drawPos.x = (RSDK.Cos512(self->starAngle[1] + 0x100) << self->starOffset) + self->starPos[5].x;
    drawPos.y = (RSDK.Sin512(self->starAngle[1] + 0x100) << self->starOffset) + self->starPos[5].y;
    self->starsData[2].frameID += 6;
    RSDK.DrawSprite(&self->starsData[2], &drawPos, 0);

    drawPos.x                    = (RSDK.Cos512(self->starAngle[1] + 0xA8) << self->starOffset) + self->starPos[3].x;
    drawPos.y                    = (RSDK.Sin512(self->starAngle[1] + 0xA8) << self->starOffset) + self->starPos[3].y;
    self->starsData[1].frameID = self->starAngle[3];
    RSDK.DrawSprite(&self->starsData[1], &drawPos, 0);

    drawPos.x = (RSDK.Cos512(self->starAngle[1] + 0x1A8) << self->starOffset) + self->starPos[3].x;
    drawPos.y = (RSDK.Sin512(self->starAngle[1] + 0x1A8) << self->starOffset) + self->starPos[3].y;
    self->starsData[1].frameID += 5;
    RSDK.DrawSprite(&self->starsData[1], &drawPos, 0);

    drawPos.x                    = (RSDK.Cos512(self->starAngle[0]) << self->starOffset) + self->starPos[0].x;
    drawPos.y                    = (RSDK.Sin512(self->starAngle[0]) << self->starOffset) + self->starPos[0].y;
    self->starsData[0].frameID = self->starAngle[2];
    RSDK.DrawSprite(self->starsData, &drawPos, 0);

    drawPos.x = (RSDK.Cos512(self->starAngle[0] + 0x100) << self->starOffset) + self->starPos[0].x;
    drawPos.y = (RSDK.Sin512(self->starAngle[0] + 0x100) << self->starOffset) + self->starPos[0].y;
    RSDK.DrawSprite(self->starsData, &drawPos, 0);
}

void InvincibleStars_Create(void *data)
{
    RSDK_THIS(InvincibleStars);
    if (!SceneInfo->inEditor) {
        Entity *player  = (Entity *)data;
        self->active  = ACTIVE_NORMAL;
        self->visible = true;
        self->parent  = player;
        for (int32 i = 0; i < 8; ++i) {
            self->starPos[i].x = player->position.x;
            self->starPos[i].y = player->position.y;
        }
        self->drawFX       = FX_FLIP;
        self->inkEffect    = INK_ADD;
        self->starAngle[0] = 180;
        self->starAngle[1] = 0;
        self->alpha        = 255;
        RSDK.SetSpriteAnimation(InvincibleStars->aniFrames, 0, self->starsData, true, 0);
        RSDK.SetSpriteAnimation(InvincibleStars->aniFrames, 1, &self->starsData[1], true, 0);
        RSDK.SetSpriteAnimation(InvincibleStars->aniFrames, 2, &self->starsData[2], true, 0);
        RSDK.SetSpriteAnimation(InvincibleStars->aniFrames, 3, &self->starsData[3], true, 0);
    }
}

void InvincibleStars_StageLoad(void) { InvincibleStars->aniFrames = RSDK.LoadSpriteAnimation("Global/Invincible.bin", SCOPE_STAGE); }

#if RETRO_INCLUDE_EDITOR
void InvincibleStars_EditorDraw(void) {}

void InvincibleStars_EditorLoad(void) {}
#endif

void InvincibleStars_Serialize(void) {}
