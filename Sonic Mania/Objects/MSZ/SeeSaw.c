#include "SonicMania.h"

ObjectSeeSaw *SeeSaw;

void SeeSaw_Update(void)
{
    RSDK_THIS(SeeSaw);

    if (entity->field_6C || entity->field_74) {
        if (entity->field_6C == entity->field_74) {
            entity->field_80 = 1;
        }
        else {
            if (entity->field_6C > entity->field_74)
                entity->field_80 = 0;
            else
                entity->field_80 = 2;
        }
    }
    else if (entity->field_70 != (entity->field_74 || entity->field_6C)) {
        entity->field_80 = 1;
    }

    if (entity->field_78 != entity->field_80) {
        entity->field_78 = entity->field_78 <= entity->field_80 ? entity->field_78 + 1 : entity->field_78 - 1;

        if (entity->state == SeeSaw_Unknown3) {
            switch (entity->field_80) {
                case 0:
                    if (entity->field_84 < 0xA0000) {
                        entity->velocity.x = -0xCC00;
                        entity->velocity.y = -0xAF000;
                    }
                    else {
                        entity->velocity.x = -0xA000;
                        entity->velocity.y = -0xE0000;
                    }
                    break;
                case 1:
                    entity->velocity.x = -0x11400;
                    entity->velocity.y = -0x81800;
                    break;
                case 2:
                    if (entity->field_84 < 0x9C000) {
                        entity->velocity.x = -0xF400;
                        entity->velocity.y = -0x96000;
                    }
                    else {
                        entity->velocity.x = -0x8000;
                        entity->velocity.y = -0xA2000;
                    }
                    break;
            }

            if (entity->orbPos.x < entity->position.x)
                entity->velocity.x = -entity->velocity.x;
            entity->state  = SeeSaw_Unknown5;
            entity->active = ACTIVE_NORMAL;
        }
    }

    if (entity->orbPos.x < entity->position.x) {
        switch (entity->field_78) {
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
        switch (entity->field_78) {
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

    StateMachine_Run(entity->state);
    RSDK.ProcessAnimation(&entity->animator3);
    entity->field_6C = 0;
    entity->field_70 = 0;
    entity->field_74 = 0;
    entity->field_8C = 0;

    foreach_active(Player, player)
    {
        SeeSaw_SetupHitbox(player->position.x, entity->field_7C);
        if (SeeSaw->hitbox1.right) {
            if (player->velocity.y > entity->field_84)
                SeeSaw->value3 = player->velocity.y + 0x7000;
            else
                SeeSaw->value3 = entity->field_84;

            if (Player_CheckCollisionPlatform(player, entity, &SeeSaw->hitbox1)) {
#if RETRO_USE_PLUS
                if (player->state == Player_State_MightyHammerDrop) {
                    if (player->position.x >= entity->position.x ? entity->field_78 != 2 : entity->field_78 != 0)
                        player->state = Player_State_Air;
                }
#endif

                entity->field_8C = 1;
                if (entity->field_78 != entity->field_7C) {
                    int top = SeeSaw->hitbox1.top;
                    SeeSaw_SetupHitbox(player->position.x, entity->field_78);
                    player->position.y += (SeeSaw->hitbox1.top - top) << 16;
                }
                player->position.y += 0x20000;

                if (entity->field_88) {
                    SeeSaw->value3 = entity->velocity.y;
                    if (abs(player->position.x - entity->position.x) >= 0x80000) {
                        if (player->position.x >= entity->position.x ? entity->field_88 == 2 : entity->field_88 == 1)
                            SeeSaw->value3 = 0;
                    }

                    if (SeeSaw->value3) {
                        player->state    = Player_State_Air;
                        player->onGround = false;
                        if (entity->state == SeeSaw_Unknown4 || entity->field_68)
                            RSDK.SetSpriteAnimation(player->spriteIndex, ANI_JUMP, &player->playerAnimator, true, 0);
                        else
                            RSDK.SetSpriteAnimation(player->spriteIndex, ANI_SPRINGTWIRL, &player->playerAnimator, true, 0);
                        RSDK.PlaySfx(SeeSaw->sfxSpring, false, 255);
                        player->velocity.y = -SeeSaw->value3;
                    }
                }
                else {
                    if (entity->state == SeeSaw_Unknown2) {
                        entity->state = SeeSaw_Unknown3;
                    }
                    else if (entity->state == SeeSaw_Unknown5) {
                        SeeSaw->value3 = entity->field_84;
                    }

                    if (abs(player->position.x - entity->position.x) >= 0x80000) {
                        if (player->position.x >= entity->position.x) {
                            ++entity->field_74;
                            if (entity->field_80 != 2)
                                entity->field_84 = SeeSaw->value3;
                        }
                        else {
                            ++entity->field_6C;
                            if (entity->field_80 != 0)
                                entity->field_84 = SeeSaw->value3;
                        }
                    }
                    else {
                        ++entity->field_70;
                    }
                }
            }
        }
    }

    entity->field_7C = entity->field_78;
    entity->field_88 = 0;

    switch (entity->field_7C) {
        case 0: entity->rotation = 480; break;
        case 1: entity->rotation = 0; break;
        case 2: entity->rotation = 32; break;
    }

    if (entity->state != SeeSaw_Unknown4) {
        int storeX       = entity->position.x;
        int storeY       = entity->position.y;
        entity->position = entity->orbPos;

        foreach_active(Player, player)
        {
            if (Player_CheckCollisionTouch(player, entity, &SeeSaw->hitbox2)) {
#if RETRO_USE_PLUS
                if (!Player_CheckMightyUnspin(768, player, 2, &player->uncurlTimer))
#endif
                    Player_CheckHit(player, entity);
            }
        }
        entity->position.x = storeX;
        entity->position.y = storeY;
    }
}

void SeeSaw_LateUpdate(void) {}

void SeeSaw_StaticUpdate(void) {}

void SeeSaw_Draw(void)
{
    RSDK_THIS(SeeSaw);
    RSDK.DrawSprite(&entity->animator3, &entity->orbPos, false);
    RSDK.DrawSprite(&entity->animator2, NULL, false);
    RSDK.DrawSprite(&entity->animator1, NULL, false);
}

void SeeSaw_Create(void *data)
{
    RSDK_THIS(SeeSaw);

    entity->drawFX = FX_FLIP;
    if (!RSDK_sceneInfo->inEditor) {
        RSDK.SetSpriteAnimation(SeeSaw->aniFrames, 0, &entity->animator1, true, 0);
        RSDK.SetSpriteAnimation(SeeSaw->aniFrames, 1, &entity->animator2, true, 0);
        RSDK.SetSpriteAnimation(SeeSaw->aniFrames, 2, &entity->animator3, true, 0);
        entity->active        = ACTIVE_BOUNDS;
        entity->updateRange.x = 0x800000;
        entity->updateRange.y = 0x800000;
        entity->drawFX        = FX_ROTATE | FX_FLIP;
        entity->visible       = true;
        entity->drawOrder     = Zone->drawOrderHigh;
        if (entity->side == 1) {
            entity->field_80 = 2;
            entity->orbPos.x = entity->position.x;
        }
        entity->state = SeeSaw_Unknown2;
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
        int distance = clampVal((((playerX - entity->position.x) >> 16) + 48) >> 1, 0, 47);

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
    entity->orbPos.x = entity->position.x + SeeSaw->value2.x;
    entity->orbPos.y = entity->position.y + SeeSaw->value2.y;
    entity->field_84 = 0;
}

void SeeSaw_Unknown3(void) {}

void SeeSaw_Unknown4(void) {}

void SeeSaw_Unknown5(void)
{
    RSDK_THIS(SeeSaw);
    entity->orbPos.x += entity->velocity.x;
    entity->orbPos.y += entity->velocity.y;
    entity->velocity.y += 0x3800;
    if (entity->velocity.y > 0) {
        SeeSaw->value2.y += entity->position.y;
        if (entity->orbPos.y >= SeeSaw->value2.y) {
            entity->orbPos.y = SeeSaw->value2.y;
            entity->state    = SeeSaw_Unknown2;
            if (entity->field_68 && entity->velocity.x)
                entity->field_68 = 0;

            if (entity->orbPos.x >= entity->position.x) {
                entity->field_80 = 2;
                entity->field_88 = 2;
            }
            else {
                entity->field_80 = 0;
                entity->field_88 = 1;
            }
            entity->active = ACTIVE_BOUNDS;
        }
    }
}

#if RETRO_INCLUDE_EDITOR
void SeeSaw_EditorDraw(void)
{
    RSDK_THIS(SeeSaw);
    entity->orbPos.x = entity->position.x;
    entity->orbPos.y = entity->position.y;
    RSDK.SetSpriteAnimation(SeeSaw->aniFrames, 0, &entity->animator1, true, 0);
    RSDK.SetSpriteAnimation(SeeSaw->aniFrames, 1, &entity->animator2, true, 0);
    RSDK.SetSpriteAnimation(SeeSaw->aniFrames, 2, &entity->animator3, true, 0);

    SeeSaw_Draw();
}

void SeeSaw_EditorLoad(void) { SeeSaw->aniFrames = RSDK.LoadSpriteAnimation("MSZ/SeeSaw.bin", SCOPE_STAGE); }
#endif

void SeeSaw_Serialize(void) { RSDK_EDITABLE_VAR(SeeSaw, VAR_UINT8, side); }
