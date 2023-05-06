// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: SideBarrel Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectSideBarrel *SideBarrel;

void SideBarrel_Update(void)
{
    RSDK_THIS(SideBarrel);

    self->barrelAnimator.speed = self->activePlayers <= 0 ? 0 : 128;

    RSDK.ProcessAnimation(&self->barrelAnimator);

    int32 playerID = 0;
    foreach_active(Player, player)
    {
        if (globals->gameMode >= MODE_TIMEATTACK && self->timeAttackFreeze) {
            self->hitboxBarrel.bottom = -8;
            self->hitboxBarrel.top    = -24;
            Player_CheckCollisionPlatform(player, self, &self->hitboxBarrel);
        }
        else {
            if (player->state == Player_State_KnuxGlideDrop) {
                self->playerTimer[playerID] = 0;
                self->playerPos[playerID]   = 128;
            }

            if (self->playerTimer[playerID]) {
                if (--self->playerTimer[playerID] <= 0)
                    self->activePlayers &= ~(1 << playerID);
            }
            else {
                if ((1 << playerID) & self->activePlayers)
                    self->playerPos[playerID] += 2;
                else
                    self->playerPos[playerID] = 128;

                self->hitboxBarrel.top    = (RSDK.Cos256(self->playerPos[playerID]) >> 3) - 4;
                self->hitboxBarrel.bottom = self->hitboxBarrel.top + 16;
                if (Player_CheckCollisionPlatform(player, self, &self->hitboxBarrel)) {
                    player->position.y += 0x40000;
                    if (player->state != Player_State_KnuxGlideDrop) {
                        if (self->playerPos[playerID] > 176) {
                            RSDK.SetSpriteAnimation(player->aniFrames, ANI_SPRING_CS, &player->animator, false, 1);
                            player->onGround            = false;
                            player->state               = Player_State_Air;
                            self->playerTimer[playerID] = 48;
                            RSDK.PlaySfx(SideBarrel->sfxDrop, false, 0xFF);
                        }

                        self->activePlayers |= 1 << playerID;
                    }
                }
                else {
                    self->playerPos[playerID] = 128;
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

    RSDK.DrawSprite(&self->barrelAnimator, NULL, false);
}

void SideBarrel_Create(void *data)
{
    RSDK_THIS(SideBarrel);

    self->visible   = true;
    self->drawGroup = Zone->objectDrawGroup[0];

    if (!SceneInfo->inEditor) {
        RSDK.SetSpriteAnimation(SideBarrel->aniFrames, 0, &self->barrelAnimator, true, 0);

        self->active        = ACTIVE_BOUNDS;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;

        self->hitboxBarrel.left   = -32;
        self->hitboxBarrel.top    = -32;
        self->hitboxBarrel.right  = 32;
        self->hitboxBarrel.bottom = 32;
    }
}

void SideBarrel_StageLoad(void)
{
    SideBarrel->aniFrames = RSDK.LoadSpriteAnimation("MSZ/SideBarrel.bin", SCOPE_STAGE);

    SideBarrel->sfxDrop = RSDK.GetSfx("Stage/Drop.wav");
}

#if GAME_INCLUDE_EDITOR
void SideBarrel_EditorDraw(void)
{
    RSDK_THIS(SideBarrel);
    RSDK.SetSpriteAnimation(SideBarrel->aniFrames, 0, &self->barrelAnimator, true, 0);

    SideBarrel_Draw();
}

void SideBarrel_EditorLoad(void) { SideBarrel->aniFrames = RSDK.LoadSpriteAnimation("MSZ/SideBarrel.bin", SCOPE_STAGE); }
#endif

void SideBarrel_Serialize(void) { RSDK_EDITABLE_VAR(SideBarrel, VAR_BOOL, timeAttackFreeze); }
