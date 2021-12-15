// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: SideBarrel Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

ObjectSideBarrel *SideBarrel;

void SideBarrel_Update(void)
{
    RSDK_THIS(SideBarrel);
    if (self->activePlayers <= 0)
        self->animator.speed = 0;
    else
        self->animator.speed = 128;
    RSDK.ProcessAnimation(&self->animator);

    int playerID = 0;
    foreach_active(Player, player)
    {
        if (globals->gameMode >= MODE_TIMEATTACK && self->timeAttackFreeze) {
            self->hitbox.bottom = -8;
            self->hitbox.top    = -24;
            Player_CheckCollisionPlatform(player, self, &self->hitbox);
        }
        else {
            if (player->state == Player_State_KnuxGlideDrop) {
                self->field_74[playerID] = 0;
                self->field_64[playerID] = 128;
            }

            if (self->field_74[playerID]) {
                self->field_74[playerID]--;
                if (self->field_74[playerID] <= 0) {
                    self->activePlayers &= ~(1 << playerID);
                }
            }
            else {
                if ((1 << playerID) & self->activePlayers)
                    self->field_64[playerID] += 2;
                else
                    self->field_64[playerID] = 128;

                self->hitbox.top    = (RSDK.Cos256(self->field_64[playerID]) >> 3) - 4;
                self->hitbox.bottom = self->hitbox.top + 16;
                if (Player_CheckCollisionPlatform(player, self, &self->hitbox)) {
                    player->position.y += 0x40000;
                    if (player->state != Player_State_KnuxGlideDrop) {
                        if (self->field_64[playerID] > 176) {
                            RSDK.SetSpriteAnimation(player->aniFrames, ANI_SPRINGCS, &player->animator, false, 1);
                            player->onGround           = false;
                            player->state              = Player_State_Air;
                            self->field_74[playerID] = 48;
                            RSDK.PlaySfx(SideBarrel->sfxDrop, false, 255);
                        }
                        self->activePlayers |= (1 << playerID);
                    }
                }
                else {
                    self->field_64[playerID] = 128;
                }
            }
        }
        playerID++;
    }
}

void SideBarrel_LateUpdate(void) {}

void SideBarrel_StaticUpdate(void) {}

void SideBarrel_Draw(void)
{
    RSDK_THIS(SideBarrel);
    RSDK.DrawSprite(&self->animator, NULL, false);
}

void SideBarrel_Create(void *data)
{
    RSDK_THIS(SideBarrel);
    self->visible   = true;
    self->drawOrder = Zone->drawOrderLow;
    if (!SceneInfo->inEditor) {
        RSDK.SetSpriteAnimation(SideBarrel->aniFrames, 0, &self->animator, true, 0);
        self->active        = ACTIVE_BOUNDS;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;
        self->hitbox.left   = -32;
        self->hitbox.top    = -32;
        self->hitbox.right  = 32;
        self->hitbox.bottom = 32;
    }
}

void SideBarrel_StageLoad(void)
{
    SideBarrel->aniFrames = RSDK.LoadSpriteAnimation("MSZ/SideBarrel.bin", SCOPE_STAGE);
    SideBarrel->sfxDrop   = RSDK.GetSfx("Stage/Drop.wav");
}

#if RETRO_INCLUDE_EDITOR
void SideBarrel_EditorDraw(void) { SideBarrel_Draw(); }

void SideBarrel_EditorLoad(void) { SideBarrel->aniFrames = RSDK.LoadSpriteAnimation("MSZ/SideBarrel.bin", SCOPE_STAGE); }
#endif

void SideBarrel_Serialize(void) { RSDK_EDITABLE_VAR(SideBarrel, VAR_BOOL, timeAttackFreeze); }
