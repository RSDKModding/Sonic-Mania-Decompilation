#include "SonicMania.h"

ObjectSeeSaw *SeeSaw;

void SeeSaw_Update(void)
{
    RSDK_THIS(SeeSaw);

    if (self->field_6C || self->field_74) {
        if (self->field_6C == self->field_74) {
            self->field_80 = 1;
        }
        else {
            if (self->field_6C > self->field_74)
                self->field_80 = 0;
            else
                self->field_80 = 2;
        }
    }
    else if (self->field_70 != (self->field_74 || self->field_6C)) {
        self->field_80 = 1;
    }

    if (self->field_78 != self->field_80) {
        self->field_78 = self->field_78 <= self->field_80 ? self->field_78 + 1 : self->field_78 - 1;

        if (self->state == SeeSaw_Unknown3) {
            switch (self->field_80) {
                case 0:
                    if (self->field_84 < 0xA0000) {
                        self->velocity.x = -0xCC00;
                        self->velocity.y = -0xAF000;
                    }
                    else {
                        self->velocity.x = -0xA000;
                        self->velocity.y = -0xE0000;
                    }
                    break;
                case 1:
                    self->velocity.x = -0x11400;
                    self->velocity.y = -0x81800;
                    break;
                case 2:
                    if (self->field_84 < 0x9C000) {
                        self->velocity.x = -0xF400;
                        self->velocity.y = -0x96000;
                    }
                    else {
                        self->velocity.x = -0x8000;
                        self->velocity.y = -0xA2000;
                    }
                    break;
            }

            if (self->orbPos.x < self->position.x)
                self->velocity.x = -self->velocity.x;
            self->state  = SeeSaw_Unknown5;
            self->active = ACTIVE_NORMAL;
        }
    }

    if (self->orbPos.x < self->position.x) {
        switch (self->field_78) {
            case 0:
                SeeSaw->value2.x = -0x280000;
                SeeSaw->value2.y = -0x20000;
                break;
            case 1:
                SeeSaw->value2.x = -0x280000;
                SeeSaw->value2.y = -0x160000;
                break;
            case 2:
                SeeSaw->value2.x = -0x280000;
                SeeSaw->value2.y = -0x290000;
                break;
        }
    }
    else {
        switch (self->field_78) {
            case 0:
                SeeSaw->value2.x = 0x280000;
                SeeSaw->value2.y = -0x290000;
                break;
            case 1:
                SeeSaw->value2.x = 0x280000;
                SeeSaw->value2.y = -0x160000;
                break;
            case 2:
                SeeSaw->value2.x = 0x280000;
                SeeSaw->value2.y = -0x20000;
                break;
        }
    }

    StateMachine_Run(self->state);
    RSDK.ProcessAnimation(&self->animator3);
    self->field_6C = 0;
    self->field_70 = 0;
    self->field_74 = 0;
    self->field_8C = 0;

    foreach_active(Player, player)
    {
        SeeSaw_SetupHitbox(player->position.x, self->field_7C);
        if (SeeSaw->hitbox1.right) {
            if (player->velocity.y > self->field_84)
                SeeSaw->value3 = player->velocity.y + 0x7000;
            else
                SeeSaw->value3 = self->field_84;

            if (Player_CheckCollisionPlatform(player, self, &SeeSaw->hitbox1)) {
#if RETRO_USE_PLUS
                if (player->state == Player_State_MightyHammerDrop) {
                    if (player->position.x >= self->position.x ? self->field_78 != 2 : self->field_78 != 0)
                        player->state = Player_State_Air;
                }
#endif

                self->field_8C = 1;
                if (self->field_78 != self->field_7C) {
                    int top = SeeSaw->hitbox1.top;
                    SeeSaw_SetupHitbox(player->position.x, self->field_78);
                    player->position.y += (SeeSaw->hitbox1.top - top) << 16;
                }
                player->position.y += 0x20000;

                if (self->field_88) {
                    SeeSaw->value3 = self->velocity.y;
                    if (abs(player->position.x - self->position.x) >= 0x80000) {
                        if (player->position.x >= self->position.x ? self->field_88 == 2 : self->field_88 == 1)
                            SeeSaw->value3 = 0;
                    }

                    if (SeeSaw->value3) {
                        player->state    = Player_State_Air;
                        player->onGround = false;
                        if (self->state == SeeSaw_Unknown4 || self->field_68)
                            RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->playerAnimator, true, 0);
                        else
                            RSDK.SetSpriteAnimation(player->aniFrames, ANI_SPRINGTWIRL, &player->playerAnimator, true, 0);
                        RSDK.PlaySfx(SeeSaw->sfxSpring, false, 255);
                        player->velocity.y = -SeeSaw->value3;
                    }
                }
                else {
                    if (self->state == SeeSaw_Unknown2) {
                        self->state = SeeSaw_Unknown3;
                    }
                    else if (self->state == SeeSaw_Unknown5) {
                        SeeSaw->value3 = self->field_84;
                    }

                    if (abs(player->position.x - self->position.x) >= 0x80000) {
                        if (player->position.x >= self->position.x) {
                            ++self->field_74;
                            if (self->field_80 != 2)
                                self->field_84 = SeeSaw->value3;
                        }
                        else {
                            ++self->field_6C;
                            if (self->field_80 != 0)
                                self->field_84 = SeeSaw->value3;
                        }
                    }
                    else {
                        ++self->field_70;
                    }
                }
            }
        }
    }

    self->field_7C = self->field_78;
    self->field_88 = 0;

    switch (self->field_7C) {
        case 0: self->rotation = 480; break;
        case 1: self->rotation = 0; break;
        case 2: self->rotation = 32; break;
    }

    if (self->state != SeeSaw_Unknown4) {
        int storeX       = self->position.x;
        int storeY       = self->position.y;
        self->position = self->orbPos;

        foreach_active(Player, player)
        {
            if (Player_CheckCollisionTouch(player, self, &SeeSaw->hitbox2)) {
#if RETRO_USE_PLUS
                if (!Player_CheckMightyUnspin(0x300, player, 2, &player->uncurlTimer))
#endif
                    Player_CheckHit(player, self);
            }
        }
        self->position.x = storeX;
        self->position.y = storeY;
    }
}

void SeeSaw_LateUpdate(void) {}

void SeeSaw_StaticUpdate(void) {}

void SeeSaw_Draw(void)
{
    RSDK_THIS(SeeSaw);
    RSDK.DrawSprite(&self->animator3, &self->orbPos, false);
    RSDK.DrawSprite(&self->animator2, NULL, false);
    RSDK.DrawSprite(&self->animator1, NULL, false);
}

void SeeSaw_Create(void *data)
{
    RSDK_THIS(SeeSaw);

    self->drawFX = FX_FLIP;
    if (!SceneInfo->inEditor) {
        RSDK.SetSpriteAnimation(SeeSaw->aniFrames, 0, &self->animator1, true, 0);
        RSDK.SetSpriteAnimation(SeeSaw->aniFrames, 1, &self->animator2, true, 0);
        RSDK.SetSpriteAnimation(SeeSaw->aniFrames, 2, &self->animator3, true, 0);
        self->active        = ACTIVE_BOUNDS;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;
        self->drawFX        = FX_ROTATE | FX_FLIP;
        self->visible       = true;
        self->drawOrder     = Zone->drawOrderHigh;
        if (self->side == 1) {
            self->field_80 = 2;
            self->orbPos.x = self->position.x;
        }
        self->state = SeeSaw_Unknown2;
    }
}

void SeeSaw_StageLoad(void)
{
    SeeSaw->aniFrames      = RSDK.LoadSpriteAnimation("MSZ/SeeSaw.bin", SCOPE_STAGE);
    SeeSaw->hitbox2.left   = -8;
    SeeSaw->hitbox2.top    = -8;
    SeeSaw->hitbox2.right  = 8;
    SeeSaw->hitbox2.bottom = 8;
    SeeSaw->sfxSpring      = RSDK.GetSFX("Global/Spring.wav");
}

void SeeSaw_SetupHitbox(int playerX, bool32 flag)
{
    RSDK_THIS(SeeSaw);
    if (flag == true) {
        SeeSaw->hitbox1.top   = -8;
        SeeSaw->hitbox1.right = 48;
    }
    else {
        int distance = clampVal((((playerX - self->position.x) >> 16) + 48) >> 1, 0, 47);

        if (flag)
            SeeSaw->hitbox1.top = SeeSaw->value1[distance] + 12;
        else
            SeeSaw->hitbox1.top = SeeSaw->value1[47 - distance] + 12;
        SeeSaw->hitbox1.right = 40;
    }
    SeeSaw->hitbox1.bottom = SeeSaw->hitbox1.top + 24;
    SeeSaw->hitbox1.left   = -SeeSaw->hitbox1.right;
}

void SeeSaw_Unknown2(void)
{
    RSDK_THIS(SeeSaw);
    self->orbPos.x = self->position.x + SeeSaw->value2.x;
    self->orbPos.y = self->position.y + SeeSaw->value2.y;
    self->field_84 = 0;
}

void SeeSaw_Unknown3(void) {}

void SeeSaw_Unknown4(void) {}

void SeeSaw_Unknown5(void)
{
    RSDK_THIS(SeeSaw);
    self->orbPos.x += self->velocity.x;
    self->orbPos.y += self->velocity.y;
    self->velocity.y += 0x3800;
    if (self->velocity.y > 0) {
        SeeSaw->value2.y += self->position.y;
        if (self->orbPos.y >= SeeSaw->value2.y) {
            self->orbPos.y = SeeSaw->value2.y;
            self->state    = SeeSaw_Unknown2;
            if (self->field_68 && self->velocity.x)
                self->field_68 = 0;

            if (self->orbPos.x >= self->position.x) {
                self->field_80 = 2;
                self->field_88 = 2;
            }
            else {
                self->field_80 = 0;
                self->field_88 = 1;
            }
            self->active = ACTIVE_BOUNDS;
        }
    }
}

#if RETRO_INCLUDE_EDITOR
void SeeSaw_EditorDraw(void)
{
    RSDK_THIS(SeeSaw);
    self->orbPos.x = self->position.x;
    self->orbPos.y = self->position.y;
    RSDK.SetSpriteAnimation(SeeSaw->aniFrames, 0, &self->animator1, true, 0);
    RSDK.SetSpriteAnimation(SeeSaw->aniFrames, 1, &self->animator2, true, 0);
    RSDK.SetSpriteAnimation(SeeSaw->aniFrames, 2, &self->animator3, true, 0);

    SeeSaw_Draw();
}

void SeeSaw_EditorLoad(void) { SeeSaw->aniFrames = RSDK.LoadSpriteAnimation("MSZ/SeeSaw.bin", SCOPE_STAGE); }
#endif

void SeeSaw_Serialize(void) { RSDK_EDITABLE_VAR(SeeSaw, VAR_UINT8, side); }
