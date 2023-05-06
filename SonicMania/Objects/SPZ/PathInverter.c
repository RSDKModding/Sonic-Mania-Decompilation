// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: PathInverter Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectPathInverter *PathInverter;

void PathInverter_Update(void)
{
    RSDK_THIS(PathInverter);

    StateMachine_Run(self->state);
}

void PathInverter_LateUpdate(void) {}

void PathInverter_StaticUpdate(void) {}

void PathInverter_Draw(void)
{
    RSDK_THIS(PathInverter);

    RSDK.DrawSprite(&self->animator, NULL, false);
}

void PathInverter_Create(void *data)
{
    RSDK_THIS(PathInverter);

    RSDK.SetSpriteAnimation(PathInverter->aniFrames, self->type, &self->animator, true, 0);

    if (!SceneInfo->inEditor) {
        self->active        = ACTIVE_BOUNDS;
        Hitbox *hitbox      = RSDK.GetHitbox(&self->animator, 0);
        self->size.x        = hitbox->right;
        self->size.y        = hitbox->bottom;
        self->size2x.x      = 2 * self->size.x;
        self->size2x.y      = 2 * self->size.y;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;
        self->visible       = true;
        self->drawGroup     = Zone->objectDrawGroup[0];

        switch (self->type) {
            default: break;
            case PATHINVERTER_H: self->state = PathInverter_State_Horizontal; break;
            case PATHINVERTER_V: self->state = PathInverter_State_Vertical; break;
        }
    }
}

void PathInverter_StageLoad(void)
{
    if (RSDK.CheckSceneFolder("SPZ2"))
        PathInverter->aniFrames = RSDK.LoadSpriteAnimation("SPZ2/PathInverter.bin", SCOPE_STAGE);
}

void PathInverter_HandlePathSwitch(EntityPlayer *player)
{
    RSDK_THIS(PathInverter);

    if (!self->playerPtrs[player->playerID]) {
        Hitbox *playerHitbox = Player_GetHitbox(player);

        if (player->position.y > self->position.y == player->position.x > self->position.x)
            self->playerFlipFlags[player->playerID] = (self->size.y + playerHitbox->bottom) << 7;
        else
            self->playerFlipFlags[player->playerID] = -((self->size.y << 7) + (playerHitbox->bottom << 7));

        if (player->collisionMode == CMODE_ROOF)
            player->direction ^= FLIP_X;

        self->playerPtrs[player->playerID]     = player;
        player->tileCollisions                 = TILECOLLISION_NONE;
        player->velocity.x                     = player->groundVel * RSDK.Cos256(player->angle) >> 8;
        player->velocity.y                     = player->groundVel * RSDK.Sin256(player->angle) >> 8;
        self->groundVelStore[player->playerID] = player->groundVel;

        int32 topSpeed     = player->state == Player_State_Roll ? 0xC0000 : 0x80000;
        player->velocity.x = CLAMP(player->velocity.x, -topSpeed, topSpeed);

        player->state           = Player_State_Static;
        player->nextAirState    = StateMachine_None;
        player->nextGroundState = StateMachine_None;
    }

    int32 pos        = self->size.x + ((self->position.x - player->position.x) >> 16);
    int32 frameAngle = 4 * (3 * pos) / self->size2x.x;
    int32 angle      = (pos << 8) / self->size2x.x;
    if (player->animator.animationID != ANI_JUMP || !player->groundedStore) {
        int32 frame = 12 - frameAngle;
        if (player->collisionMode != CMODE_ROOF * (self->playerFlipFlags[player->playerID] >= 0))
            frame = frameAngle;

        if (frame >= 0)
            frame %= 24;
        else
            frame += 24;

        if (player->collisionMode)
            player->direction &= ~FLIP_Y;
        else
            player->direction |= FLIP_Y;

        RSDK.SetSpriteAnimation(player->aniFrames, ANI_SPRING_CS, &player->animator, true, frame);
    }

    player->onGround   = true;
    player->position.y = self->playerFlipFlags[player->playerID] * RSDK.Cos512(angle) + self->position.y;
}
void PathInverter_State_Horizontal(void)
{
    RSDK_THIS(PathInverter);

    foreach_active(Player, player)
    {
        if (player->collisionMode == CMODE_FLOOR || player->collisionMode == CMODE_ROOF) {
            if (abs(self->position.x - player->position.x) >> 16 >= self->size.x
                || abs(self->position.y - player->position.y) >> 16 >= self->size.y + 32) {
                int32 playerID = player->playerID;

                if (self->playerPtrs[playerID]) {
                    self->playerPtrs[playerID] = NULL;

                    player->groundVel  = self->groundVelStore[player->playerID];
                    player->groundVel  = -player->groundVel;
                    player->velocity.x = -player->velocity.x;
                    if (player->collisionMode == CMODE_ROOF)
                        player->direction ^= FLIP_X;

                    player->collisionMode = (player->collisionMode - 2) & 3;
                    player->angle         = (player->angle + 0x80) & 0xFF;
                    player->controlLock   = 30;

                    if (player->animator.animationID == ANI_JUMP) {
                        player->state = Player_State_Roll;
                    }
                    else {
                        player->direction &= FLIP_X;
                        player->direction ^= FLIP_X;
                        player->state = Player_State_Ground;
                        RSDK.SetSpriteAnimation(player->aniFrames, ANI_JOG, &player->animator, false, 0);
                        player->rotation = player->angle << 1;
                    }

                    player->tileCollisions = TILECOLLISION_DOWN;
                }
            }
            else if (player->state == Player_State_Static) {
                PathInverter_HandlePathSwitch(player);
            }
            else {
                if (!player->onGround) {
                    if (self->playerPtrs[player->playerID])
                        PathInverter_HandlePathSwitch(player);
                }
                else {
                    if (abs(player->groundVel) < 0x40000) {
                        if (self->playerPtrs[player->playerID])
                            PathInverter_HandlePathSwitch(player);
                    }
                    else {
                        bool32 noInteractions = false;
                        if (player->velocity.x <= 0)
                            noInteractions = player->position.x <= self->position.x + 0x80000;
                        else
                            noInteractions = player->position.x >= self->position.x - 0x80000;

                        if (noInteractions)
                            self->playerPtrs[player->playerID] = NULL;
                        else
                            PathInverter_HandlePathSwitch(player);
                    }
                }
            }
        }
    }
}
void PathInverter_State_Vertical(void)
{
    // Nothing !
}

#if GAME_INCLUDE_EDITOR
void PathInverter_EditorDraw(void)
{
    RSDK_THIS(PathInverter);
    RSDK.SetSpriteAnimation(PathInverter->aniFrames, self->type, &self->animator, true, 0);

    PathInverter_Draw();
}

void PathInverter_EditorLoad(void)
{
    PathInverter->aniFrames = RSDK.LoadSpriteAnimation("SPZ2/PathInverter.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(PathInverter, type);
    RSDK_ENUM_VAR("Horizontal", PATHINVERTER_H);
    RSDK_ENUM_VAR("Vertical (Unused)", PATHINVERTER_V);
}
#endif

void PathInverter_Serialize(void) { RSDK_EDITABLE_VAR(PathInverter, VAR_UINT8, type); }
