#include "SonicMania.h"

ObjectDNARiser *DNARiser;

void DNARiser_Update(void)
{
    RSDK_THIS(DNARiser);

    if (self->field_A8)
        self->field_A8--;
    if (self->timer2 > 0)
        self->timer2--;
    StateMachine_Run(self->state);
    if (self->popped) {
        if (self->animator.frameID == self->animator.loopIndex)
            self->popped = false;
        else
            RSDK.ProcessAnimation(&self->animator);
    }
}

void DNARiser_LateUpdate(void) {}

void DNARiser_StaticUpdate(void)
{
    foreach_active(DNARiser, entity) { RSDK.AddDrawListRef(Zone->drawOrderHigh, RSDK.GetEntityID(entity)); }
}

void DNARiser_Draw(void)
{
    RSDK_THIS(DNARiser);
    StateMachine_Run(self->stateDraw);
}

void DNARiser_Create(void *data)
{
    RSDK_THIS(DNARiser);
    self->visible   = true;
    self->active    = ACTIVE_BOUNDS;
    self->drawOrder = Zone->drawOrderLow + 1;
    self->field_CC  = 0x2E0000;
    if (SceneInfo->inEditor) {
        self->speed.x = 0;
        if (!self->speed.y)
            self->speed.y = 0x60000;
        if (!self->height)
            self->height = 128;
    }
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;
    self->state         = DNARiser_State_Setup;
}

void DNARiser_StageLoad(void)
{
    DNARiser->aniFrames     = RSDK.LoadSpriteAnimation("CPZ/DNARiser.bin", SCOPE_STAGE);
    DNARiser->hitbox.top    = -16;
    DNARiser->hitbox.bottom = 16;
    DNARiser->hitbox.left   = -16;
    DNARiser->hitbox.right  = 16;
    DNARiser->active        = ACTIVE_ALWAYS;
    DNARiser->sfxBurst      = RSDK.GetSFX("CPZ/DNABurst.wav");
    DNARiser->sfxGrab       = RSDK.GetSFX("CPZ/DNAGrab.wav");
    DNARiser->sfxScan       = RSDK.GetSFX("CPZ/DNAScan.wav");
    DNARiser->sfxTiny[0]    = RSDK.GetSFX("CPZ/DNATiny0.wav");
    DNARiser->sfxTiny[1]    = RSDK.GetSFX("CPZ/DNATiny1.wav");
    DNARiser->sfxTiny[2]    = RSDK.GetSFX("CPZ/DNATiny2.wav");
    DNARiser->sfxTiny[3]    = RSDK.GetSFX("CPZ/DNATiny3.wav");
    DNARiser->sfxTiny[4]    = RSDK.GetSFX("CPZ/DNATiny4.wav");
    DNARiser->sfxTiny[5]    = RSDK.GetSFX("CPZ/DNATiny5.wav");
}

void DNARiser_State_BubbleBurst(void)
{
    RSDK_THIS(DNARiser);
    RSDK.PlaySfx(DNARiser->sfxBurst, 0, 255);
    self->popPos = self->position;
    self->popped = true;
    RSDK.SetSpriteAnimation(DNARiser->aniFrames, 5, &self->animator, true, 0);
}

void DNARiser_State_Burst_Helix(void)
{
    RSDK_THIS(DNARiser);
    RSDK.PlaySfx(DNARiser->sfxBurst, 0, 255);
    self->popPos = self->position;
    self->popped = true;
    RSDK.SetSpriteAnimation(DNARiser->aniFrames, 5, &self->animator, true, 0);
    self->vector_D8  = self->speed;
    self->field_60   = 1;
    self->velocity.x = 0;
    self->velocity.y = 0;
    self->state      = DNARiser_State_Unknown4;
    self->timer      = 0;
}

Vector2 DNARiser_CalculateScale(Vector2 *vec)
{
    RSDK_THIS(DNARiser);

    Vector2 resultVec;
    int32 x = 512;
    int32 y = 512;
    if (vec) {
        x = vec->x;
        y = vec->y;
    }

    int32 val     = (30 - self->field_A8);
    int32 sine    = RSDK.Sin1024(((val * (0x40000000 / ((10983 * val + 286520) >> 7))) >> 11) & 0x3FF);
    resultVec.x = (((val - 30) * (sine << 6) / 100 + 0x10000) * x) >> 16;
    resultVec.y = (((30 - val) * (sine << 6) / 100 + 0x10000) * y) >> 16;
    return resultVec;
}

void DNARiser_State_Setup(void)
{
    RSDK_THIS(DNARiser);
    self->vector_D0     = self->position;
    self->active        = ACTIVE_BOUNDS;
    self->speed.x       = 0;
    self->velocity.x    = 0;
    self->speed.y       = -abs(self->speed.y);
    self->timer         = 0;
    self->curHeight     = 0;
    self->field_AC      = 0;
    self->field_E4      = 0;
    self->child         = 0;
    self->field_E0      = 0;
    self->activePlayers = 0;
    self->field_60      = 0;
    self->field_B6      = 0;
    self->timer2        = 0;
    self->field_B0      = 0;
    self->velocity.y    = 0;
    self->stateDraw     = DNARiser_StateDraw_Main;
    self->state         = DNARiser_HandleInteractions;
    self->field_70      = 512;
    RSDK.SetSpriteAnimation(DNARiser->aniFrames, 0, &self->animator2, true, 0);
    DNARiser_HandleInteractions();
}

void DNARiser_HandleInteractions(void)
{
    RSDK_THIS(DNARiser);

    foreach_active(Player, player)
    {
        int32 playerID = RSDK.GetEntityID(player);

        if (!((1 << playerID) & self->activePlayers)) {
            if (Player_CheckCollisionTouch(player, self, &DNARiser->hitbox)) {
                RSDK.PlaySfx(DNARiser->sfxGrab, 0, 255);
                self->field_A8 = 30;
                self->timer2   = 30;
                RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->playerAnimator, false, 0);
                player->groundVel        = 0;
                player->drawOrder        = 1;
                player->jumpAbilityTimer = 1;
                player->onGround         = false;
                player->tileCollisions   = false;
                player->state            = Player_State_None;
                player->nextAirState     = 0;
                player->nextGroundState  = 0;
                self->activePlayers |= 1 << playerID;
            }
        }

        if (((1 << playerID) & self->activePlayers)) {
            if (player->state == Player_State_None) {
                player->position.x = self->position.x;
                player->position.y = self->position.y;
                player->velocity.x = 0;
                player->velocity.y = 0;
                if (!player->sidekick) {
                    self->curHeight = 0;
                    self->active    = 2;
                    self->state     = DNARiser_State_Unknown3;
                }
            }
            else {
                self->activePlayers &= ~(1 << playerID);
            }
        }
    }
}

void DNARiser_State_Unknown3(void)
{
    RSDK_THIS(DNARiser);
    if (self->curHeight >= self->height << 16) {
        DNARiser_State_Burst_Helix();
    }
    else {
        if (self->velocity.y >= abs(self->speed.y)) {
            self->velocity.y = abs(self->speed.y);
        }
        else {
            self->velocity.y += 0x1200;
        }

        self->curHeight += self->velocity.y;
        if (self->height << 16 < self->curHeight)
            self->curHeight = self->height << 16;

        int32 angle = (((0x57262 * (self->curHeight >> 16)) >> 16) + 250) & 0x3FF;
        if (angle >= 0x200u && !self->field_B0) {
            self->field_B0 = 1;
            RSDK.PlaySfx(DNARiser->sfxScan, 0, 255);
        }
        else if (angle <= 0x1FF && self->field_B0) {
            self->field_B0 = 0;
            RSDK.PlaySfx(DNARiser->sfxScan, 0, 255);
        }

        self->position.y = self->vector_D0.y - self->curHeight;
        for (; self->field_E0 <= (self->curHeight >> 18) + 1; ++self->field_E0) {
            if (!(self->field_E0 & 1) || !(self->field_E0 % 5)) {
                if (!self->timer2) {
                    int32 sfxID = 0;
                    if (!self->field_B6) {
                        do
#if RETRO_USE_PLUS
                            sfxID = RSDK.Random(0, 5, &Zone->randKey);
#else
                            sfxID = RSDK.Rand(0, 5);
#endif
                        while (sfxID == self->field_B6);
                    }
                    RSDK.PlaySfx(DNARiser->sfxTiny[sfxID], 0, 255);
                    self->field_B6 = sfxID;
#if RETRO_USE_PLUS
                    self->timer2   = RSDK.Random(2, 8, &Zone->randKey);
#else
                    self->timer2 = RSDK.Rand(2, 8);
#endif
                }
                EntityDNARiser *child = CREATE_ENTITY(DNARiser, self, self->vector_D0.x, self->vector_D0.y - (self->field_E0 << 18));
                child->state          = DNARiser_State_SetupChild;
                child->active         = ACTIVE_NORMAL;
                child->field_A4       = self->field_E0;
                child->parent         = (Entity *)self;
                child->angle          = (((0x57262 * (4 * self->field_E0)) >> 16) + 100) & 0x3FF;
                child->field_CC       = 0x2E0000;
                child->drawOrder      = self->drawOrder - 1;
                if (!self->field_E4)
                    self->field_E4 = (Entity *)child;

                if (self->child) {
                    child->field_F0                             = self->child;
                    ((EntityDNARiser *)self->child)->field_EC = (Entity *)child;
                }
                self->child = (Entity *)child;
            }
        }
    }

    bool32 flag      = false;
    self->field_70 = (0x10000 - 85 * ((self->curHeight / self->height) >> 8)) >> 7;

    foreach_active(Player, player)
    {
        int32 playerID = RSDK.GetEntityID(player);
        if (!((1 << playerID) & self->activePlayers)) {
            if (Player_CheckCollisionTouch(player, self, &DNARiser->hitbox)) {
                RSDK.PlaySfx(DNARiser->sfxGrab, 0, 255);
                self->timer2   = 30;
                self->field_A8 = 30;
                self->activePlayers |= (1 << playerID);
            }
        }

        if (((1 << playerID) & self->activePlayers)) {
            bool32 skipFlag = false;
            if (!Player_CheckValidState(player)) {
                self->activePlayers &= ~(1 << playerID);
                if (self->activePlayers)
                    skipFlag = true;
                else
                    DNARiser_State_Burst_Helix();
            }
            if (!skipFlag) {
                if (player->jumpPress) {
                    player->velocity.y = 0;
                    player->velocity.x = 0;
                    Player_StartJump(player);
                    DNARiser_State_Burst_Helix();
                }

                if (self->state == DNARiser_State_Unknown4) {
                    flag = true;
                }
                else {
                    RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->playerAnimator, false, 0);
                    player->groundVel        = 0;
                    player->drawOrder        = 1;
                    player->jumpAbilityTimer = 1;
                    player->onGround         = false;
                    player->tileCollisions   = false;
                    player->state            = Player_State_None;
                    player->nextAirState     = StateMachine_None;
                    player->nextGroundState  = StateMachine_None;
                    player->velocity.x       = 0;
                    player->velocity.y       = -self->velocity.y;
                    player->position.x       = self->position.x;
                    player->position.y       = self->position.y;
                }
            }
        }
    }

    if (flag) {
        foreach_active(Player, player)
        {
            int32 playerID = RSDK.GetEntityID(player);
            if (((1 << playerID) & self->activePlayers)) {
                player->drawOrder      = Zone->playerDrawLow;
                player->tileCollisions = true;
                player->state          = Player_State_Air;
                player->velocity.x     = self->speed.x;
                player->velocity.y     = self->speed.y;
                self->activePlayers &= ~(1 << playerID);
            }
        }
    }
}

void DNARiser_State_Unknown4(void)
{
    RSDK_THIS(DNARiser);

    if (self->timer <= 0) {
        if (!self->timer) {
            if (!self->field_AC) {
                EntityDNARiser *child = (EntityDNARiser *)self->child;
                if (child) {
                    self->field_AC   = 1;
                    child->timer       = 0;
                    child->velocity.x  = 0;
                    child->state       = DNARiser_State_Unknown8;
                    child->velocity.y  = self->vector_D8.y >> 1;
                    child->vector_D8.y = self->vector_D8.y >> 1;
                }
            }
            if (self->position.y < self->vector_D0.y) {
                self->velocity.y += 0x38000;
                self->position.y += self->velocity.y;
            }

            int32 posY = self->vector_D0.y;
            if (self->position.y < self->vector_D0.y)
                posY = self->position.y;

            self->position.y = posY;
            self->curHeight  = self->vector_D0.y - posY;
            if (posY == self->vector_D0.y) {
                self->state = DNARiser_State_Unknown5;
                self->timer = 60;
            }
        }
    }
    else {
        self->timer--;
    }
}

void DNARiser_State_Unknown5(void)
{
    RSDK_THIS(DNARiser);
    if (!self->field_E4) {
        if (self->timer <= 0) {
            if (RSDK.CheckOnScreen(self, &self->updateRange)) {
                DNARiser_State_BubbleBurst();
                DNARiser_State_Setup();
                self->field_A8 = 30;
            }
            else {
                self->state = DNARiser_State_Setup;
            }
        }
        else {
            self->timer--;
        }
    }
}

void DNARiser_State_SetupChild(void)
{
    RSDK_THIS(DNARiser);
    self->active    = ACTIVE_NORMAL;
    self->stateDraw = DNARiser_StateDraw_Helix;
    self->state     = DNARiser_State_None;
}

void DNARiser_State_None(void) {}

void DNARiser_State_Unknown8(void)
{
    RSDK_THIS(DNARiser);
    EntityDNARiser *parent = (EntityDNARiser *)self->parent;

    if (self->timer <= 0) {
        if (self->field_AC) {
            if (!RSDK.CheckOnScreen(self, &self->updateRange))
                destroyEntity(self);
        }
        else {
            EntityDNARiser *child = (EntityDNARiser *)self->field_F0;
            self->field_AC      = 1;
            if (child) {
                while (true) {
                    if (RSDK.CheckOnScreen(child, &child->updateRange)) {
                        child->velocity.y  = 0;
                        child->timer       = 1;
                        child->state       = DNARiser_State_Unknown8;
                        child->velocity.x  = 0;
                        child->velocity.y  = (self->vector_D8.y >> 1) + (self->vector_D8.y >> 2);
                        child->vector_D8.y = (self->vector_D8.y >> 1) + (self->vector_D8.y >> 2);
                        break;
                    }
                    else {
                        EntityDNARiser *grandChild = (EntityDNARiser *)child->field_F0;
                        if (child == (EntityDNARiser *)parent->child)
                            parent->child = (Entity *)grandChild;
                        destroyEntity(child);
                        child = grandChild;
                        if (!grandChild)
                            break;
                    }
                }
            }

            if (!child) {
                parent->field_E4 = NULL;
            }

            if (self->field_AC) {
                if (!RSDK.CheckOnScreen(self, &self->updateRange))
                    destroyEntity(self);
            }
        }
    }
    else {
        self->timer--;
    }
    self->velocity.y += 0x3800;
    self->position.y += self->velocity.y;
    self->field_CC += 0x20000;
}

void DNARiser_StateDraw_Main(void)
{
    RSDK_THIS(DNARiser);

    bool32 flag = false;
    int32 angle   = ((0x57262 * (self->curHeight >> 16)) >> 16) + 100;

    Animator animator;
    memset(&animator, 0, sizeof(animator));
    if ((uint32)(angle & 0x3FF) - 256 <= 0x200)
        flag = true;

    int32 sineOff       = (RSDK.Sin1024(angle & 0x3FF) << 6) * (self->field_CC >> 16);
    self->alpha     = 0x100;
    self->inkEffect = INK_NONE;

    Vector2 drawPos;
    drawPos = self->position;

    self->scale.x = 0x200;
    self->scale.y = 0x200;
    self->drawFX  = FX_NONE;
    if (SceneInfo->currentDrawGroup == Zone->drawOrderHigh) {
        if (flag) {
            drawPos.x = sineOff + self->position.x;
            drawPos.y = self->position.y;
            RSDK.SetSpriteAnimation(DNARiser->aniFrames, 4, &animator, true, 0);
        }
        else {
            drawPos.x = self->position.x - sineOff;
            drawPos.y = self->position.y;
            RSDK.SetSpriteAnimation(DNARiser->aniFrames, 2, &animator, true, 0);
        }

        RSDK.DrawSprite(&animator, &drawPos, false);
        if (!self->field_60) {
            drawPos           = self->position;
            self->drawFX    = FX_SCALE;
            self->inkEffect = INK_BLEND;
            self->alpha     = 128;
            Vector2 vec;
            vec.x         = self->field_70;
            vec.y         = self->field_70;
            self->scale = DNARiser_CalculateScale(&vec);
            RSDK.DrawSprite(&self->animator2, &drawPos, false);
        }
        if (self->popped) {
            drawPos           = self->popPos;
            self->scale.x   = 0x200;
            self->scale.y   = 0x200;
            self->drawFX    = FX_NONE;
            self->inkEffect = INK_NONE;
            RSDK.DrawSprite(&self->animator, &drawPos, false);
        }
    }
    else {
        if (flag) {
            drawPos.y = self->position.y;
            drawPos.x = self->position.x - sineOff;
            RSDK.SetSpriteAnimation(DNARiser->aniFrames, 2, &animator, true, 0);
        }
        else {
            drawPos.x = sineOff + self->position.x;
            drawPos.y = self->position.y;
            RSDK.SetSpriteAnimation(DNARiser->aniFrames, 4, &animator, true, 0);
        }
        RSDK.DrawSprite(&animator, &drawPos, false);
    }
}

void DNARiser_StateDraw_Helix(void)
{
    RSDK_THIS(DNARiser);

    bool32 flag            = false;
    EntityDNARiser *parent = (EntityDNARiser *)self->parent;

    Animator animator;
    memset(&animator, 0, sizeof(animator));
    if ((self->angle - 256) <= 0x200)
        flag = true;

    int32 sineOff       = (RSDK.Sin1024(self->angle) << 6) * (self->field_CC >> 16);
    self->alpha     = 0x100;
    self->inkEffect = INK_NONE;

    Vector2 drawPos;
    drawPos = self->position;

    self->scale.x = 0x200;
    self->scale.y = 0x200;
    self->drawFX  = FX_NONE;
    self->drawFX  = INK_NONE;
    if (SceneInfo->currentDrawGroup != Zone->drawOrderHigh && !(self->field_A4 & 1)) {
        if (flag) {
            drawPos.x = self->position.x - sineOff;
            drawPos.y = self->position.y;
            RSDK.SetSpriteAnimation(DNARiser->aniFrames, 2, &animator, true, 0);
        }
        else {
            drawPos.x = sineOff + self->position.x;
            drawPos.y = self->position.y;
            RSDK.SetSpriteAnimation(DNARiser->aniFrames, 4, &animator, true, 0);
        }
        RSDK.DrawSprite(&animator, &drawPos, false);
    }

    if (!(self->field_A4 % 5)) {
        int32 distance = (parent->field_E0 - self->field_A4 >= 8) ? 4 : ((parent->field_E0 - self->field_A4) / 2);
        int32 sine     = (RSDK.Sin1024(self->angle) << 6) * ((self->field_CC >> 16) - 12);
        drawPos.y    = self->position.y;
        drawPos.x    = sine + self->position.x;
        if (flag)
            drawPos.x = self->position.x - sine;
        int32 startX = drawPos.x;
        sine       = 2 * sine / 7;

        bool32 flagArray[8];
        flagArray[0] = distance >= 1;
        flagArray[1] = distance >= 2;
        flagArray[2] = distance >= 3;
        flagArray[3] = distance >= 4;
        flagArray[4] = flagArray[3];
        flagArray[5] = flagArray[2];
        flagArray[6] = flagArray[1];
        flagArray[7] = flagArray[0];

        for (int32 i = 0; i < 8; ++i) {
            bool32 flagA = (flag && i <= 3) || (!flag && i >= 4);
            bool32 flagB = SceneInfo->currentDrawGroup == Zone->drawOrderHigh ? ((flag && !flagA) || (!flag && flagA))
                                                                                   : ((flag && flagA) || (!flag && !flagA));

            if (flagArray[i] && flagB) {
                if (!flagA)
                    RSDK.SetSpriteAnimation(DNARiser->aniFrames, 3, &animator, true, 0);
                else
                    RSDK.SetSpriteAnimation(DNARiser->aniFrames, 1, &animator, true, 0);
                drawPos.x = startX + sine * i * (2 * flag - 1);
                RSDK.DrawSprite(&animator, &drawPos, false);
            }
        }
    }

    if (SceneInfo->currentDrawGroup == Zone->drawOrderHigh && !(self->field_A4 & 1)) {
        if (flag) {
            drawPos.x = sineOff + self->position.x;
            drawPos.y = self->position.y;
            RSDK.SetSpriteAnimation(DNARiser->aniFrames, 4, &animator, true, 0);
        }
        else {
            drawPos.x = self->position.x - sineOff;
            drawPos.y = self->position.y;
            RSDK.SetSpriteAnimation(DNARiser->aniFrames, 2, &animator, true, 0);
        }
        RSDK.DrawSprite(&animator, &drawPos, false);
    }
}

void DNARiser_EditorDraw(void)
{
    RSDK_THIS(DNARiser);

    self->vector_D0     = self->position;
    self->active        = ACTIVE_BOUNDS;
    self->speed.x       = 0;
    self->velocity.x    = 0;
    self->speed.y       = -abs(self->speed.y);
    self->timer         = 0;
    self->curHeight     = 0;
    self->field_AC      = 0;
    self->field_E4      = 0;
    self->child         = 0;
    self->field_E0      = 0;
    self->activePlayers = 0;
    self->field_60      = 0;
    self->field_B6      = 0;
    self->timer2        = 0;
    self->field_B0      = 0;
    self->velocity.y    = 0;
    self->field_70      = 0x200;
    RSDK.SetSpriteAnimation(DNARiser->aniFrames, 0, &self->animator2, true, 0);

    DNARiser_StateDraw_Main();
}

void DNARiser_EditorLoad(void) { DNARiser->aniFrames = RSDK.LoadSpriteAnimation("CPZ/DNARiser.bin", SCOPE_STAGE); }

void DNARiser_Serialize(void)
{
    RSDK_EDITABLE_VAR(DNARiser, VAR_UINT32, height);
    RSDK_EDITABLE_VAR(DNARiser, VAR_VECTOR2, speed);
}
