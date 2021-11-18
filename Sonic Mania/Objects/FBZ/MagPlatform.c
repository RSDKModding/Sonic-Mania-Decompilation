#include "SonicMania.h"

ObjectMagPlatform *MagPlatform;

void MagPlatform_Update(void) { Platform_Update(); }

void MagPlatform_LateUpdate(void) {}

void MagPlatform_StaticUpdate(void) {}

void MagPlatform_Draw(void)
{
    RSDK_THIS(MagPlatform);
    RSDK.DrawSprite(&self->animator, &self->drawPos, false);

    self->animator.frameID                            = 1;
    RSDK.GetFrame(Platform->aniFrames, 3, 1)->height = (self->centerPos.y - self->drawPos.y) >> 16;
    RSDK.DrawSprite(&self->animator, &self->drawPos, false);

    self->animator.frameID = 2;
    RSDK.DrawSprite(&self->animator, &self->centerPos, false);

    self->animator.frameID = 0;
}

void MagPlatform_Create(void *data)
{
    RSDK_THIS(MagPlatform);
    Platform_Create(NULL);
    RSDK.SetSpriteAnimation(Platform->aniFrames, 3, &self->animator, true, 0);
    if (!SceneInfo->inEditor) {
        self->length <<= 16;
        self->stateCollide = MagPlatform_Unknown1;
        self->state        = MagPlatform_Unknown2;
    }
}

void MagPlatform_StageLoad(void) { MagPlatform->sfxChain = RSDK.GetSFX("Stage/Chain.wav"); }

void MagPlatform_Unknown1(void)
{
    RSDK_THIS(MagPlatform);
    Hitbox *hitbox       = RSDK.GetHitbox(&self->animator, 1);
    self->stoodPlayers = 0;
    self->pushPlayersL = 0;
    self->pushPlayersR = 0;

    int32 playerID = 0;
    foreach_active(Player, player)
    {
        bool32 groundStore = player->onGround;
        switch (Player_CheckCollisionBox(player, self, hitbox)) {
            case C_TOP:
                if (!groundStore) {
                    self->stood = true;
                    if (self->state == Platform_State_Collapse && !self->timer)
                        self->timer = 30;
                    self->stoodPlayers |= 1 << playerID;
                    player->position.x += self->collisionOffset.x;
                    player->position.y += self->collisionOffset.y;
                    player->position.y &= 0xFFFF0000;
                }
                break;
            case C_LEFT:
                if (player->onGround && player->right)
                    self->pushPlayersL |= 1 << playerID;
                break;
            case C_RIGHT:
                if (player->onGround && player->left)
                    self->pushPlayersR |= 1 << playerID;
                break;
            case C_BOTTOM:
                if (!player->onGround) {
#if RETRO_USE_PLUS
                    if (!Player_CheckMightyUnspin(0x400, player, true, &player->uncurlTimer)) 
#endif
                        Player_CheckHit(player, self);
                }
                else if (player->collisionMode) {
                    if (self->velocity.y > 0) {
                        Player_CheckHit(player, self);
                    }
                    else {
#if RETRO_USE_PLUS
                        if (!Player_CheckMightyUnspin(0x300, player, true, &player->uncurlTimer)) 
#endif
                            Player_CheckHit(player, self);
                    }
                }
                else {
                    player->hurtFlag = true;
                }
                break;
            default: break;
        }
        playerID++;
    }
}

void MagPlatform_Unknown2(void) {}

void MagPlatform_Unknown3(void)
{
    RSDK_THIS(MagPlatform);

    self->drawPos.y += self->velocity.y;
    int32 posY           = self->position.y;
    self->position.y = self->drawPos.y;
    self->velocity.y -= 0x3800;
    if (RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_ROOF, 0, 0, -0x40000, true))
        self->velocity.y = 0;

    if (self->drawPos.y <= self->centerPos.y - self->length) {
        self->drawPos.y = self->centerPos.y - self->length;
        if (self->velocity.y < -0x20000)
            RSDK.PlaySfx(MagPlatform->sfxChain, false, 255);
        self->velocity.y = 0;
    }
    self->position.y = posY;
    self->state      = MagPlatform_Unknown4;
}

void MagPlatform_Unknown4(void)
{
    RSDK_THIS(MagPlatform);

    self->drawPos.y += self->velocity.y;
    self->velocity.y += 0x3800;
    if (self->velocity.y <= 0 && RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_ROOF, 0, 0, -0x40000, true))
        self->velocity.y = 0;

    if (self->drawPos.y >= self->centerPos.y) {
        self->velocity.y = 0;
        self->drawPos.y  = self->centerPos.y;
        self->active     = ACTIVE_BOUNDS;
        self->state      = MagPlatform_Unknown2;
    }
}

#if RETRO_INCLUDE_EDITOR
void MagPlatform_EditorDraw(void) {}

void MagPlatform_EditorLoad(void) {}
#endif

void MagPlatform_Serialize(void) { RSDK_EDITABLE_VAR(MagPlatform, VAR_ENUM, length); }
