#include "SonicMania.h"

#if RETRO_USE_PLUS
ObjectSchrodingersCapsule *SchrodingersCapsule;

void SchrodingersCapsule_Update(void)
{
    RSDK_THIS(SchrodingersCapsule);
    StateMachine_Run(entity->state);

    if (entity->animator1.frameID != 1) {
        bool32 flag = false;
        foreach_active(Player, player)
        {
            if (Player_CheckCollisionBox(player, entity, &entity->hitbox1) == 1)
                flag = true;

            if (entity->state == SchrodingersCapsule_Unknown2) {
                if (Player_CheckCollisionBox(player, entity, &entity->hitbox2) == 1) {
                    entity->field_60   = 0x80000;
                    player->velocity.x = 0;
                    player->groundVel  = 0;
                    CutsceneSeq_LockPlayerControl(player);
                    flag               = false;
                    player->state      = Player_State_Ground;
                    player->stateInput = StateMachine_None;
                    entity->active     = ACTIVE_NORMAL;
                    entity->state      = SchrodingersCapsule_Unknown3;
                }
                else {
                    if (Player_CheckCollisionTouch(player, entity, &entity->hitbox3)) {
                        Hitbox *playerHitbox = Player_GetHitbox(player);
                        entity->field_60     = ((playerHitbox->bottom + 48) << 16) - entity->position.y + player->position.y;
                        if (entity->field_60 <= 0x80000) {
                            if (entity->field_60 < 0)
                                entity->field_60 = 0;
                            entity->field_60 &= 0xFFFF0000;
                        }
                        else {
                            entity->field_60 = 0x80000;
                            entity->field_60 &= 0xFFFF0000;
                        }
                    }
                    else {
                        if (entity->field_60 > 0)
                            entity->field_60 -= 0x10000;
                    }

                    if (flag) {
                        EntityPhantomRuby *ruby = (EntityPhantomRuby *)EncoreIntro->phantomRuby;
                        if (ruby->velocity.y > 0)
                            ruby->velocity.y = -ruby->velocity.y;
                        ruby->state = EncoreIntro_PhantomRuby_Unknown4;
                    }
                }
            }
            else {
                flag = false;
                Player_CheckCollisionBox(player, entity, &entity->hitbox2);
            }
        }
    }
}

void SchrodingersCapsule_LateUpdate(void) {}

void SchrodingersCapsule_StaticUpdate(void) {}

void SchrodingersCapsule_Draw(void)
{
    RSDK_THIS(SchrodingersCapsule);
    Vector2 drawPos;

    if (!entity->animator1.frameID) {
        drawPos = entity->position;
        drawPos.y += entity->field_60;
        RSDK.DrawSprite(&entity->animator3, &drawPos, false);

        entity->animator1.frameID = 2;
        RSDK.DrawSprite(&entity->animator1, NULL, false);

        if ((Zone->timer & 8)) {
            entity->animator1.frameID = 3;
            RSDK.DrawSprite(&entity->animator1, NULL, false);
        }

        drawPos = entity->position;
        drawPos.x += 0xE0000;
        RSDK.DrawSprite(&entity->animator4, &drawPos, false);

        drawPos.x -= 0x1C0000;
        RSDK.DrawSprite(&entity->animator5, &drawPos, false);

        entity->animator1.frameID = 0;
    }
    RSDK.DrawSprite(&entity->animator1, NULL, false);

    if (!entity->animator1.frameID) {
        entity->inkEffect = INK_ADD;
        entity->alpha     = 128;
        RSDK.DrawSprite(&entity->animator2, NULL, false);

        entity->inkEffect = INK_NONE;
    }
}

void SchrodingersCapsule_Create(void *data)
{
    RSDK_THIS(SchrodingersCapsule);
    entity->drawFX = FX_FLIP;
    if (!RSDK_sceneInfo->inEditor) {
        RSDK.SetSpriteAnimation(SchrodingersCapsule->aniFrames, 0, &entity->animator1, true, 0);
        RSDK.SetSpriteAnimation(SchrodingersCapsule->aniFrames, 1, &entity->animator3, true, 0);
        RSDK.SetSpriteAnimation(SchrodingersCapsule->aniFrames, 2, &entity->animator2, true, 0);
        RSDK.SetSpriteAnimation(SchrodingersCapsule->aniFrames, 3, &entity->animator4, true, 0);
        RSDK.SetSpriteAnimation(SchrodingersCapsule->aniFrames, 4, &entity->animator5, true, 0);
        entity->hitbox1.left   = -40;
        entity->hitbox1.top    = -40;
        entity->hitbox1.right  = 40;
        entity->hitbox1.bottom = 40;
        entity->hitbox2.left   = -16;
        entity->hitbox2.top    = -44;
        entity->hitbox2.right  = 16;
        entity->hitbox2.bottom = -30;
        entity->hitbox3.left   = -15;
        entity->hitbox3.top    = -60;
        entity->hitbox3.right  = 15;
        entity->hitbox3.bottom = -30;
        entity->state          = SchrodingersCapsule_Unknown1;
        entity->active         = ACTIVE_BOUNDS;
        entity->updateRange.x  = 0x800000;
        entity->updateRange.y  = 0x800000;
        entity->visible        = true;
        entity->drawOrder      = Zone->drawOrderLow;
    }
}

void SchrodingersCapsule_StageLoad(void)
{
    SchrodingersCapsule->aniFrames     = RSDK.LoadSpriteAnimation("AIZ/SchrodingersCapsule.bin", SCOPE_STAGE);
    SchrodingersCapsule->sfxExplosion2 = RSDK.GetSFX("Stage/Explosion2.wav");
    SchrodingersCapsule->sfxExplosion3 = RSDK.GetSFX("Stage/Explosion3.wav");
}

void SchrodingersCapsule_Unknown1(void)
{
    RSDK_THIS(SchrodingersCapsule);
    entity->state = SchrodingersCapsule_Unknown2;
}

void SchrodingersCapsule_Unknown2(void)
{
    RSDK_THIS(SchrodingersCapsule);
    RSDK.ProcessAnimation(&entity->animator2);
    RSDK.ProcessAnimation(&entity->animator4);
    RSDK.ProcessAnimation(&entity->animator5);

    for (int32 p = 0; p < Player->playerCount; ++p) {
        EntityPlayer *player = RSDK_GET_ENTITY(p, Player);
        if (!player->sidekick) {
            if (abs(entity->position.x - player->position.x) < 0x1000000) {
                if (abs(entity->position.y - player->position.y) < 0x1000000 && entity->position.x - (Zone->screenBoundsR1[p] << 16) < 0x1000000) {
                    Zone->playerBoundActiveL[p] = true;
                    Zone->playerBoundActiveR[p] = true;
                    Zone->screenBoundsL1[p]     = (entity->position.x >> 16) - RSDK_screens[p].centerX;
                    Zone->screenBoundsR1[p]     = (entity->position.x >> 16) + RSDK_screens[p].centerX;
                }
            }
        }
    }
}

void SchrodingersCapsule_Unknown3(void)
{
    RSDK_THIS(SchrodingersCapsule);
    RSDK.ProcessAnimation(&entity->animator2);
    RSDK.ProcessAnimation(&entity->animator4);
    RSDK.ProcessAnimation(&entity->animator5);
    RSDK.SetSpriteAnimation(0xFFFF, 0, &entity->animator2, true, 0);
    entity->state               = SchrodingersCapsule_Unknown4;
    RSDK_sceneInfo->timeEnabled = false;
}

void SchrodingersCapsule_Unknown4(void)
{
    RSDK_THIS(SchrodingersCapsule);
    RSDK.ProcessAnimation(&entity->animator2);
    RSDK.ProcessAnimation(&entity->animator4);
    RSDK.ProcessAnimation(&entity->animator5);
    if (!(entity->timer % 3)) {
        EntityExplosion *explosion = CREATE_ENTITY(Explosion, intToVoid((2 * (RSDK.Rand(0, 256) > 192) + EXPLOSION_ENEMY)),
                                                   (RSDK.Rand(-24, 24) << 16) + entity->position.x, (RSDK.Rand(-24, 24) << 16) + entity->position.y);
        explosion->drawOrder = Zone->drawOrderHigh;

        RSDK.PlaySfx(SchrodingersCapsule->sfxExplosion2, 0, 255);
    }

    if (++entity->timer == 60) {
        entity->timer             = 0;
        entity->state             = StateMachine_None;
        entity->animator1.frameID = 1;
        RSDK.SetSpriteAnimation(0xFFFF, 0xFFFF, &entity->animator4, true, 0);
        RSDK.SetSpriteAnimation(0xFFFF, 0xFFFF, &entity->animator5, true, 0);

        EntityPlayer *buddy1 = RSDK_GET_ENTITY(SLOT_PLAYER3, Player);
        buddy1->objectID     = Player->objectID;
        Player_ChangeCharacter(buddy1, ID_MIGHTY);
        buddy1->position.x      = entity->position.x + 0xE0000;
        buddy1->position.y      = entity->position.y;
        buddy1->playerID        = RSDK.GetEntityID(buddy1);
        buddy1->active          = ACTIVE_NORMAL;
        buddy1->tileCollisions  = true;
        buddy1->interaction     = true;
        buddy1->visible         = true;
        buddy1->controllerID    = buddy1->playerID + 1;
        buddy1->drawOrder       = Zone->playerDrawLow;
        buddy1->scale.x         = 0x200;
        buddy1->scale.y         = 0x200;
        buddy1->state           = Player_State_Ground;
        buddy1->collisionLayers = Zone->fgLayers;
        buddy1->drawFX          = FX_ROTATE | FX_FLIP;
        buddy1->velocity.x      = 0x1C000;
        buddy1->velocity.y      = -0x40000;
        buddy1->direction       = FLIP_X;
        RSDK.SetSpriteAnimation(buddy1->aniFrames, ANI_HURT, &buddy1->playerAnimator, true, 0);

        EntityPlayer *buddy2 = RSDK_GET_ENTITY(SLOT_PLAYER4, Player);
        buddy2->objectID     = Player->objectID;
        Player_ChangeCharacter(buddy2, ID_RAY);
        buddy2->position.x      = entity->position.x - 0xE0000;
        buddy2->position.y      = entity->position.y;
        buddy2->playerID        = RSDK.GetEntityID(buddy2);
        buddy2->active          = ACTIVE_NORMAL;
        buddy2->tileCollisions  = true;
        buddy2->interaction     = true;
        buddy2->visible         = true;
        buddy2->controllerID    = buddy2->playerID + 1;
        buddy2->drawOrder       = Zone->playerDrawLow;
        buddy2->scale.x         = 0x200;
        buddy2->scale.y         = 0x200;
        buddy2->state           = Player_State_Ground;
        buddy2->collisionLayers = Zone->fgLayers;
        buddy2->drawFX          = FX_ROTATE | FX_FLIP;
        buddy2->velocity.x      = -0x24000;
        buddy2->velocity.y      = -0x40000;
        RSDK.SetSpriteAnimation(buddy2->aniFrames, ANI_HURT, &buddy2->playerAnimator, true, 0);

        Music_FadeOut(0.025);
        RSDK.PlaySfx(SchrodingersCapsule->sfxExplosion3, 0, 255);

        EntityPlayer *player = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
        player->position.x   = entity->position.x;
        Player_ChangeCharacter(player, player->characterID);
        RSDK.SetSpriteAnimation(player->aniFrames, ANI_HURT, &player->playerAnimator, true, 0);
        player->velocity.x = 0;
        player->velocity.y = 0;
        player->groundVel  = 0;
        player->stateInput = StateMachine_None;
        player->left       = false;
        player->right      = false;
        player->jumpPress  = false;
        player->jumpHold   = false;

        EntityCamera *camera = RSDK_GET_ENTITY(SLOT_CAMERA1, Camera);
        camera->position.x += 0x100000;

        entity->timer = 0;
        entity->state = SchrodingersCapsule_Unknown5;

        EntityFXFade *fade = (EntityFXFade *)RSDK.CreateEntity(FXFade->objectID, intToVoid(0xF0F0F0), entity->position.x, entity->position.y);
        fade->speedIn      = 256;
        fade->speedOut     = 32;
    }
}

void SchrodingersCapsule_Unknown5(void)
{
    RSDK_THIS(SchrodingersCapsule);
    if (++entity->timer == 90) {
        entity->timer = 0;
        entity->state = StateMachine_None;

        EntityPlayer *buddy1 = RSDK_GET_ENTITY(SLOT_PLAYER3, Player);
        buddy1->state        = Player_State_Victory;
        RSDK.SetSpriteAnimation(buddy1->aniFrames, ANI_VICTORY, &buddy1->playerAnimator, true, 0);

        EntityPlayer *buddy2 = RSDK_GET_ENTITY(SLOT_PLAYER4, Player);
        buddy2->state        = Player_State_Victory;
        RSDK.SetSpriteAnimation(buddy2->aniFrames, ANI_VICTORY, &buddy2->playerAnimator, true, 0);

        ActClear->actID       = 1;
        ActClear->forceNoSave = true;
        Music_PlayTrack(TRACK_ACTCLEAR);
        RSDK.ResetEntitySlot(SLOT_ACTCLEAR, ActClear->objectID, NULL);
    }
}

#if RETRO_INCLUDE_EDITOR
void SchrodingersCapsule_EditorDraw(void)
{
    RSDK_THIS(SchrodingersCapsule);

    RSDK.SetSpriteAnimation(SchrodingersCapsule->aniFrames, 0, &entity->animator1, true, 0);
    RSDK.SetSpriteAnimation(SchrodingersCapsule->aniFrames, 1, &entity->animator3, true, 0);
    RSDK.SetSpriteAnimation(SchrodingersCapsule->aniFrames, 2, &entity->animator2, true, 0);
    RSDK.SetSpriteAnimation(SchrodingersCapsule->aniFrames, 3, &entity->animator4, true, 0);
    RSDK.SetSpriteAnimation(SchrodingersCapsule->aniFrames, 4, &entity->animator5, true, 0);
    SchrodingersCapsule_Draw();
}

void SchrodingersCapsule_EditorLoad(void) { SchrodingersCapsule->aniFrames = RSDK.LoadSpriteAnimation("AIZ/SchrodingersCapsule.bin", SCOPE_STAGE); }
#endif

void SchrodingersCapsule_Serialize(void) {}
#endif
