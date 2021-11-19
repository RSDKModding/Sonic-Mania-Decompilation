#include "SonicMania.h"

ObjectJunctionWheel *JunctionWheel;

void JunctionWheel_Update(void)
{
    RSDK_THIS(JunctionWheel);
    if (RSDK_GET_ENTITY((SceneInfo->entitySlot - 1), Button)->currentlyActive)
        self->field_88 ^= 1;
    if (self->field_88)
        self->rotation += 4;
    else
        self->rotation -= 4;
    self->rotation &= 0x1FF;

    int32 rot     = ((self->rotation + 48) >> 6) & 7;

    foreach_active(Player, player) {
        if (player->state == Player_State_None) {
            if (Player_CheckCollisionTouch(player, self, &JunctionWheel->hitbox1)) {
                player->position.x += player->groundVel * RSDK.Cos512(self->rotation) + self->position.x;
                player->position.y += player->groundVel * RSDK.Sin512(self->rotation) + self->position.y;

                if (player->groundVel < -0x1000)
                    player->groundVel += 0x200;

                if (player->groundVel == -0x1000) {
                    if (self->rotation == 384) {
                        player->state          = Player_State_Air;
                        player->tileCollisions = true;
                        player->groundVel      = 0;
                        player->velocity.x     = 0;
                        player->velocity.y     = 0x80000;
                        player->onGround       = false;
                        RSDK.PlaySfx(Player->sfxRelease, false, 255);
                    }

                    if (self->field_88 == 1 && self->rotation == 284) {
                        player->state          = Player_State_Air;
                        player->tileCollisions = true;
                        player->groundVel      = 0x80000;
                        player->velocity.x     = 0x80000;
                        player->velocity.y     = 0x40000;
                        player->onGround       = false;
                        RSDK.PlaySfx(Player->sfxRelease, false, 255);
                    }
                }
            }
        }
        else if (rot == 1) {
            Player_CheckCollisionBox(player, self, &JunctionWheel->hitbox3);

            if (Player_CheckCollisionTouch(player, self, &JunctionWheel->hitbox5)) {
                player->state = Player_State_None;
                RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->animator, false, 0);
                player->onGround  = false;
                player->groundVel       = -0x1C00;

                player->tileCollisions  = false;
                player->nextAirState    = 0;
                player->nextGroundState = 0;
                player->velocity.x      = 0;
                player->velocity.y      = 0;
                RSDK.PlaySfx(Player->sfxRoll, false, 255);
            }
            if (player->velocity.y < 0 || (player->onGround && player->collisionMode))
                Player_CheckCollisionBox(player, self, &JunctionWheel->hitbox4);
        }
        else {
            if (rot != 5) {
                if (player->position.y < self->position.y)
                    Player_CheckCollisionBox(player, self, &JunctionWheel->hitbox2);
                Player_CheckCollisionBox(player, self, &JunctionWheel->hitbox3);
            }
            else if (player->velocity.x <= 0) {
                Player_CheckCollisionBox(player, self, &JunctionWheel->hitbox2);
                if (Player_CheckCollisionTouch(player, self, &JunctionWheel->hitbox6)) {
                    player->state = Player_State_None;
                    RSDK.SetSpriteAnimation(player->aniFrames, 10, &player->animator, false, 0);
                    player->onGround  = false;
                    player->groundVel = -0x1800;
                }
                player->tileCollisions  = false;
                player->nextAirState    = 0;
                player->nextGroundState = 0;
                player->velocity.x      = 0;
                player->velocity.y      = 0;
                RSDK.PlaySfx(Player->sfxRoll, false, 255);
            }
            if (player->velocity.y < 0 || (player->onGround && player->collisionMode))
                Player_CheckCollisionBox(player, self, &JunctionWheel->hitbox4);
        }
    }
}

void JunctionWheel_LateUpdate(void)
{

}

void JunctionWheel_StaticUpdate(void)
{

}

void JunctionWheel_Draw(void)
{
    RSDK_THIS(JunctionWheel);
    RSDK.DrawSprite(&self->animator1, NULL, false);
    RSDK.DrawSprite(&self->animator2, NULL, false);
}

void JunctionWheel_Create(void* data)
{
    RSDK_THIS(JunctionWheel);
    self->visible       = true;
    self->drawOrder     = Zone->drawOrderLow;
    self->active        = ACTIVE_BOUNDS;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;
    self->drawFX        = FX_ROTATE;
    RSDK.SetSpriteAnimation(JunctionWheel->aniFrames, 0, &self->animator1, true, 0);
    RSDK.SetSpriteAnimation(JunctionWheel->aniFrames, 1, &self->animator2, true, 0);
}

void JunctionWheel_StageLoad(void)
{
    if (RSDK.CheckStageFolder("SSZ1"))
        JunctionWheel->aniFrames = RSDK.LoadSpriteAnimation("SSZ1/JunctionWheel.bin", SCOPE_STAGE);
    JunctionWheel->hitbox1.left   = -192;
    JunctionWheel->hitbox1.top    = -192;
    JunctionWheel->hitbox1.right  = 192;
    JunctionWheel->hitbox1.bottom = 192;

    JunctionWheel->hitbox2.left   = -40;
    JunctionWheel->hitbox2.top    = -56;
    JunctionWheel->hitbox2.right  = -16;
    JunctionWheel->hitbox2.bottom = 32;

    JunctionWheel->hitbox3.left   = 16;
    JunctionWheel->hitbox3.top    = -56;
    JunctionWheel->hitbox3.right  = 40;
    JunctionWheel->hitbox3.bottom = 32;

    JunctionWheel->hitbox4.left   = -40;
    JunctionWheel->hitbox4.top    = 32;
    JunctionWheel->hitbox4.right  = 40;
    JunctionWheel->hitbox4.bottom = 56;

    JunctionWheel->hitbox5.left   = -40;
    JunctionWheel->hitbox5.top    = -40;
    JunctionWheel->hitbox5.right  = -16;
    JunctionWheel->hitbox5.bottom = -8;

    JunctionWheel->hitbox6.left   = 16;
    JunctionWheel->hitbox6.top    = 8;
    JunctionWheel->hitbox6.right  = 38;
    JunctionWheel->hitbox6.bottom = 40;
}

#if RETRO_INCLUDE_EDITOR
void JunctionWheel_EditorDraw(void) { JunctionWheel_Draw(); }

void JunctionWheel_EditorLoad(void) { JunctionWheel->aniFrames = RSDK.LoadSpriteAnimation("SSZ1/JunctionWheel.bin", SCOPE_STAGE); }
#endif

void JunctionWheel_Serialize(void)
{

}

