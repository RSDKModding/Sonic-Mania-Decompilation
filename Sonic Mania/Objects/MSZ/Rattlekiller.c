#include "SonicMania.h"

ObjectRattlekiller *Rattlekiller;

void Rattlekiller_Update(void)
{
    RSDK_THIS(Rattlekiller);

    if (self->timer) {
        self->timer--;
        if (self->bodyStates[0] == 4) {
            foreach_active(Player, player)
            {
                if (abs(self->bodyPositions[0].x - player->position.x) < 0x600000) {
                    if (abs(self->bodyPositions[0].y - player->position.y) < 0x600000) {
                        self->playerPos.x = player->position.x;
                        self->playerPos.y = player->position.y;
                        foreach_break;
                    }
                }
            }
        }
    }
    else {
        foreach_active(Player, player)
        {
            if (self->bodyPositions[0].x <= self->startPos.x) {
                if (player->position.x >= self->startPos.x || player->position.x - self->startPos.x <= -0x600000)
                    continue;
            }
            else if (player->position.x <= self->startPos.x || player->position.x - self->startPos.x >= 0x600000) {
                continue;
            }

            if (abs(self->bodyPositions[0].y - player->position.y) < 0x600000) {
                int delay = 30;
                for (int i = 0; i < Rattlekiller_SegmentCount; ++i) {
                    self->bodyDelays[i] = 30;
                    self->field_DC[i].x = self->bodyPositions[i].x;
                    self->field_DC[i].y = self->bodyPositions[0].y;
                    self->bodyStates[i]   = 4;

                    delay += 2;
                }

                self->playerPos.x = player->position.x;
                self->playerPos.y = player->position.y;
                self->timer       = 480;
                foreach_break;
            }
        }
    }

    foreach_active(Player, player)
    {
        for (int i = 0; i < Rattlekiller_SegmentCount; ++i) {
            switch (self->bodyStates[i]) {
                case 0:
                    if (--self->bodyDelays[i] <= 0)
                        self->bodyStates[i] = (self->field_12C[i].y < 0) + 2;
                    break;
                case 1:
                    self->bodyAngles[i] = (self->bodyAngles[i] + 2) & 0xFF;
                    self->bodyPositions[i].y += self->field_12C[i].y;
                    self->bodyPositions[i].x = self->startPos.x - 0x1800 * RSDK.Sin256(self->bodyAngles[i]);
                    self->field_1A4[i]       = RSDK.Cos256(self->bodyAngles[i]);
                    if (self->timer < 320) {
                        if (--self->bodyDelays[i] <= 0)
                            self->bodyStates[i] = (self->field_12C[i].y < 0) + 2;
                    }
                    break;
                case 2:
                    self->bodyAngles[i] = (self->bodyAngles[i] + 2) & 0xFF;
                    self->field_12C[i].y += 0x400;
                    if (self->field_12C[i].y > 0x8000)
                        self->field_12C[i].y = 0x8000;

                    self->bodyPositions[i].y += self->field_12C[i].y;
                    self->bodyPositions[i].x = self->startPos.x - 0x1800 * RSDK.Sin256(self->bodyAngles[i]);
                    self->field_1A4[i]       = RSDK.Cos256(self->bodyAngles[i]);
                    if (self->bodyPositions[i].y >= self->startPos2.y)
                        self->bodyStates[i] = 3;
                    break;
                case 3:
                    self->bodyAngles[i] = (self->bodyAngles[i] + 2) & 0xFF;
                    self->field_12C[i].y -= 0x400;
                    if (self->field_12C[i].y < -0x8000)
                        self->field_12C[i].y = -0x8000;

                    self->bodyPositions[i].y += self->field_12C[i].y;
                    self->bodyPositions[i].x = self->startPos.x - 0x1800 * RSDK.Sin256(self->bodyAngles[i]);
                    self->field_1A4[i]       = RSDK.Cos256(self->bodyAngles[i]);
                    if (self->bodyPositions[i].y <= self->startPos.y)
                        self->bodyStates[i] = 2;
                    break;
                case 4:
                    if (--self->bodyDelays[i] > 0) {
                        if (self->bodyDelays[i] < 15 && i > 4) {
                            self->bodyAngles[i]          = (self->bodyAngles[i] + 4) & 0xFF;
                            self->bodyPositions[i].x = self->startPos.x - 0x1800 * RSDK.Sin256(self->bodyAngles[i]);
                            self->field_1A4[i]       = RSDK.Cos256(self->bodyAngles[i]);
                        }
                    }
                    else if (i) {
                        self->bodyStates[i] = 5;
                    }
                    else {
                        self->bodyStates[0] = 6;
                        RSDK.SetSpriteAnimation(Rattlekiller->aniFrames, (self->bodyPositions[0].x > self->startPos.x) + 2, self->animators,
                                                true, 0);
                        RSDK.PlaySfx(Rattlekiller->sfxRocketJet, false, 255);
                    }
                    break;
                case 5:
                    if (self->bodyStates[0] < 6) {
                        self->bodyPositions[i].y += (self->bodyPositions[0].y - self->bodyPositions[i].y) >> 3;
                        if (self->bodyStates[i - 1] == 1) {
                            if (abs(self->bodyPositions[0].y - self->bodyPositions[i].y) < 0x20000) {
                                self->bodyPositions[i].y = self->bodyPositions[0].y;
                                self->field_12C[i].x     = 0;
                                self->field_12C[i].y     = 0;
                                self->bodyStates[i]      = 1;
                                self->bodyDelays[i]      = 8 * i + self->bodyDelays[0];
                                self->bodyAngles[i]      = self->bodyAngles[i - 1] - 16;
                            }
                        }
                    }
                    else if (i < Rattlekiller_SegmentCount - 1) {
                        self->bodyPositions[i].x = (self->bodyPositions[i - 1].x >> 1) + (self->bodyPositions[i + 1].x >> 1);
                        self->bodyPositions[i].y = (self->bodyPositions[i - 1].y >> 1) + (self->bodyPositions[i + 1].y >> 1);
                    }
                    break;
                case 6: {
                    int rx = (self->playerPos.x - self->bodyPositions[i].x) >> 16;
                    int ry = (self->playerPos.y - self->bodyPositions[i].y) >> 16;
                    if (rx * rx + ry * ry >= 64) {
                        int angle              = RSDK.ATan2(rx, ry);
                        self->field_12C[i].x = RSDK.Cos256(angle) << 10;
                        self->bodyPositions[i].x += self->field_12C[i].x;
                        self->bodyPositions[i].y += RSDK.Sin256(angle) << 10;
                        self->field_12C[i].y = RSDK.Sin256(angle) << 10;
                        RSDK.ProcessAnimation(self->animators);
                    }
                    else {
                        self->bodyStates[i] = 7;
                        RSDK.ProcessAnimation(self->animators);
                    }
                    break;
                }
                case 7: {
                    int rx = (self->field_DC[i].x - self->bodyPositions[i].x) >> 16;
                    int ry = (self->field_DC[Rattlekiller_SegmentCount - 1].y - self->bodyPositions[i].y) >> 16;
                    if (rx * rx + ry * ry >= 64) {
                        int angle              = RSDK.ATan2(rx, ry);
                        self->field_12C[i].x = RSDK.Cos256(angle) << 10;
                        self->bodyPositions[i].x += self->field_12C[i].x;
                        self->bodyPositions[i].y += RSDK.Sin256(angle) << 10;
                        self->field_12C[i].y = RSDK.Sin256(angle) << 10;
                        RSDK.ProcessAnimation(self->animators);
                    }
                    else {
                        self->bodyStates[i] = 1;
                        if (i > 0)
                            self->bodyAngles[i] = self->bodyAngles[i - 1] - 16;
                        self->bodyPositions[i].x = self->field_DC[i].x;
                        self->bodyPositions[i].y = self->field_DC[Rattlekiller_SegmentCount - 1].y;
                        self->bodyDelays[i]      = 8 * i;
                        self->field_12C[i].x     = 0;
                        self->field_12C[i].y     = 0;
                        RSDK.SetSpriteAnimation(Rattlekiller->aniFrames, 0, self->animators, true, 0);
                    }
                    break;
                }
                default: break;
            }
        }

        self->position.x = self->bodyPositions[0].x;
        self->position.y = self->bodyPositions[0].y;
        if (Player_CheckBadnikTouch(player, self, &Rattlekiller->hitbox) && Player_CheckBadnikBreak(self, player, false)) {
            for (int i = 1; i < Rattlekiller_SegmentCount; ++i) {
                EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, self->bodyPositions[i].x, self->bodyPositions[i].y);
                RSDK.SetSpriteAnimation(Rattlekiller->aniFrames, self->bodyAnimators[i]->animationID, &debris->animator, true,
                                        self->bodyAnimators[i]->frameID);
                debris->velocity.x    = RSDK.Rand(-0x20000, 0x20000);
                debris->velocity.y    = RSDK.Rand(-0x20000, -0x10000);
                debris->gravity       = 0x4800;
                debris->drawOrder     = Zone->drawOrderHigh;
                debris->updateRange.x = 0x400000;
                debris->updateRange.y = 0x400000;
            }

            destroyEntity(self);
            self->active = ACTIVE_NEVER2;
            foreach_break;
        }
        else {
            for (int i = 1; i < Rattlekiller_SegmentCount; ++i) {
                if (self->field_1A4[i] > 0) {
                    self->position.x = self->bodyPositions[i].x;
                    self->position.y = self->bodyPositions[i].y;
                    if (Player_CheckCollisionTouch(player, self, &Rattlekiller->hitbox)) {
#if RETRO_USE_PLUS
                        if (!Player_CheckMightyUnspin(0x200, player, 2, &player->uncurlTimer))
#endif
                            Player_CheckHit(player, self);
                    }
                }
            }
        }
    }

    self->position.x = self->bodyPositions[0].x;
    self->position.y = self->bodyPositions[0].y;
    Rattlekiller_HandleSorting();
}

void Rattlekiller_LateUpdate(void) {}

void Rattlekiller_StaticUpdate(void)
{
    foreach_active(Rattlekiller, rattlekiller) { RSDK.AddDrawListRef(Zone->drawOrderLow - 1, RSDK.GetEntityID(rattlekiller)); }
}

void Rattlekiller_Draw(void)
{
    RSDK_THIS(Rattlekiller);

    if (SceneInfo->currentDrawGroup == self->drawOrder) {
        for (int i = 0; i < Rattlekiller_SegmentCount; ++i) {
            int id = self->bodyIDs[i];
            if (self->field_1A4[id] > 0) {
                Animator *animator = self->bodyAnimators[id];
                if (animator->animationID < 2)
                    animator->frameID = self->bodyAngles[id] >> 4;
                RSDK.DrawSprite(animator, &self->bodyPositions[id], false);
            }
        }
    }
    else {
        for (int i = 0; i < Rattlekiller_SegmentCount; ++i) {
            int id = self->bodyIDs[i];
            if (self->field_1A4[id] <= 0) {
                Animator *animator = self->bodyAnimators[id];
                if (animator->animationID < 2)
                    animator->frameID = self->bodyAngles[id] >> 4;
                RSDK.DrawSprite(animator, &self->bodyPositions[id], false);
            }
        }
    }
}

void Rattlekiller_Create(void *data)
{
    RSDK_THIS(Rattlekiller);
    if (!SceneInfo->inEditor) {
        self->visible       = true;
        self->drawOrder     = Zone->drawOrderLow;
        self->startPos      = self->position;
        self->startPos2     = self->position;
        self->active        = ACTIVE_BOUNDS;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;
        if (data)
            self->length = voidToInt(data);
        else if (!self->length)
            self->length = 128;

        self->startPos.y -= self->length << 15;
        self->startPos2.y += self->length << 15;

        int delay = 0;
        for (int i = 0; i < Rattlekiller_SegmentCount; ++i) {
            self->bodyIDs[i]       = i;
            self->bodyAnimators[i] = &self->animators[1];
            self->bodyDelays[i]    = delay;
            self->bodyPositions[i] = self->startPos;

            delay += 8;
        }

        self->bodyAnimators[0] = &self->animators[0];
        RSDK.SetSpriteAnimation(Rattlekiller->aniFrames, 0, &self->animators[0], true, 0);
        RSDK.SetSpriteAnimation(Rattlekiller->aniFrames, 1, &self->animators[1], true, 0);
        RSDK.SetSpriteAnimation(Rattlekiller->aniFrames, 4, &self->animators[2], true, 0);
    }
}

void Rattlekiller_StageLoad(void)
{
    Rattlekiller->aniFrames     = RSDK.LoadSpriteAnimation("MSZ/Rattlekiller.bin", SCOPE_STAGE);
    Rattlekiller->hitbox.left   = -8;
    Rattlekiller->hitbox.top    = -8;
    Rattlekiller->hitbox.right  = 8;
    Rattlekiller->hitbox.bottom = 8;
    Rattlekiller->sfxRocketJet  = RSDK.GetSFX("Stage/RocketJet.wav");
    DEBUGMODE_ADD_OBJ(Rattlekiller);
}

void Rattlekiller_DebugSpawn(void)
{
    RSDK_THIS(DebugMode);
    EntityRattlekiller *rattlekiller = CREATE_ENTITY(Rattlekiller, intToVoid(0x80), self->position.x, self->position.y);
    rattlekiller->direction          = FLIP_NONE;
}

void Rattlekiller_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(Rattlekiller->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
}

void Rattlekiller_HandleSorting(void)
{
    RSDK_THIS(Rattlekiller);

    for (int i = 0; i < Rattlekiller_SegmentCount; ++i) {
        for (int i2 = Rattlekiller_SegmentCount - 1; i2 > i; --i2) {
            int idA = self->bodyIDs[i2 - 1];
            int idB = self->bodyIDs[i2];
            if (self->field_1A4[idA] > self->field_1A4[idB]) {
                self->bodyIDs[i2 - 1] = idB;
                self->bodyIDs[i2]     = idA;
            }
        }
    }
}

#if RETRO_INCLUDE_EDITOR
void Rattlekiller_EditorDraw(void)
{
    RSDK_THIS(Rattlekiller);
    RSDK.SetSpriteAnimation(Rattlekiller->aniFrames, 0, &self->animators[0], false, 0);
    RSDK.SetSpriteAnimation(Rattlekiller->aniFrames, 1, &self->animators[1], false, 0);
    RSDK.SetSpriteAnimation(Rattlekiller->aniFrames, 4, &self->animators[2], false, 0);

    self->startPos      = self->position;
    self->startPos2     = self->position;

    self->startPos.y -= self->length << 15;
    self->startPos2.y += self->length << 15;

    int delay = 0;
    for (int i = 0; i < Rattlekiller_SegmentCount; ++i) {
        self->bodyIDs[i]       = i;
        self->bodyAnimators[i] = &self->animators[1];
        self->bodyDelays[i]    = delay;
        self->bodyPositions[i] = self->startPos;

        delay += 8;
    }

    
    Rattlekiller_HandleSorting();
    Rattlekiller_Draw();
}

void Rattlekiller_EditorLoad(void) { Rattlekiller->aniFrames = RSDK.LoadSpriteAnimation("MSZ/Rattlekiller.bin", SCOPE_STAGE); }
#endif

void Rattlekiller_Serialize(void)
{
    RSDK_EDITABLE_VAR(Rattlekiller, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(Rattlekiller, VAR_ENUM, length);
}
