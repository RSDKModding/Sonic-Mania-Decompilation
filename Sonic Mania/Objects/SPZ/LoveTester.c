#include "SonicMania.h"

ObjectLoveTester *LoveTester;

void LoveTester_Update(void)
{
    RSDK_THIS(LoveTester);

    StateMachine_Run(entity->state);
    StateMachine_Run(entity->stateLate);

    for (int i = 0; i < 10; ++i) {
        RSDK.ProcessAnimation(&entity->animators[i]);
    }
}

void LoveTester_LateUpdate(void) {}

void LoveTester_StaticUpdate(void)
{
    foreach_active(LoveTester, loveTester) { RSDK.AddDrawListRef(Zone->drawOrderHigh, RSDK.GetEntityID(loveTester)); }
}

void LoveTester_Draw(void)
{
    RSDK_THIS(LoveTester);

    if (entity->state == LoveTester_State2_Unknown)
        RSDK.DrawSprite(&entity->animator1, NULL, false);
    else
        LoveTester_DrawSprites();
}

void LoveTester_Create(void *data)
{
    RSDK_THIS(LoveTester);

    entity->active        = ACTIVE_BOUNDS;
    entity->drawOrder     = Zone->drawOrderLow;
    entity->storedPos.x   = entity->position.x;
    entity->storedPos.y   = entity->position.y;
    entity->visible       = true;
    entity->drawFX        = FX_FLIP;
    entity->updateRange.x = 0x800000;
    entity->updateRange.y = 0x800000;
    LoveTester_SetupHitboxes();
    if (data) {
        entity->updateRange.x = 0x100000;
        entity->updateRange.y = 0x100000;
        entity->active        = ACTIVE_NORMAL;
        entity->drawOrder     = Zone->drawOrderHigh;
        entity->state         = LoveTester_State2_Unknown;
    }
    else {
        entity->state = LoveTester_State_Unknown1;
    }
}

void LoveTester_StageLoad(void)
{
    LoveTester->aniFrames = RSDK.LoadSpriteAnimation("SPZ2/LoveTester.bin", SCOPE_STAGE);
    LoveTester_SetupHitboxes();
    LoveTester_SetupPositions();
    LoveTester->field_20.x = 0;
    LoveTester->field_20.y = -0x300000;
    LoveTester->field_28.x = 0;
    LoveTester->field_28.y = 0x320000;
    LoveTester->active     = ACTIVE_ALWAYS;
    LoveTester->sfxScore   = RSDK.GetSFX("SPZ/Score.wav");
}

void LoveTester_SetupHitboxes(void)
{
    RSDK_THIS(LoveTester);

    LoveTester->hitbox1.left   = -16;
    LoveTester->hitbox1.top    = -16;
    LoveTester->hitbox1.right  = 16;
    LoveTester->hitbox1.bottom = 16;
    LoveTester->hitbox2.left   = -24;
    LoveTester->hitbox2.top    = -20;
    LoveTester->hitbox2.right  = -18;
    LoveTester->hitbox2.bottom = 20;
    LoveTester->hitbox3.left   = 18;
    LoveTester->hitbox3.top    = -20;
    LoveTester->hitbox3.right  = 24;
    LoveTester->hitbox3.bottom = 20;
}

void LoveTester_SetupPositions(void)
{
    RSDK_THIS(LoveTester);

    LoveTester->positions[0].x = -0x1C0000;
    LoveTester->positions[0].y = -0x1F0000;
    LoveTester->positions[1].x = 0x1C0000;
    LoveTester->positions[1].y = -0x1F0000;
    LoveTester->positions[2].x = -0x1C0000;
    LoveTester->positions[2].y = -0xF0000;
    LoveTester->positions[3].x = 0x1C0000;
    LoveTester->positions[3].y = -0xF0000;
    LoveTester->positions[4].x = -0x1C0000;
    LoveTester->positions[4].y = 0x10000;
    LoveTester->positions[5].x = 0x1C0000;
    LoveTester->positions[5].y = 0x10000;
    LoveTester->positions[6].x = -0x1C0000;
    LoveTester->positions[6].y = 0x110000;
    LoveTester->positions[7].x = 0x1C0000;
    LoveTester->positions[7].y = 0x110000;
    LoveTester->positions[8].x = -0x1C0000;
    LoveTester->positions[8].y = 0x210000;
    LoveTester->positions[9].x = 0x1C0000;
    LoveTester->positions[9].y = 0x210000;
}

void LoveTester_DrawSprites(void)
{
    RSDK_THIS(LoveTester);

    int storeX = entity->position.x;
    int storeY = entity->position.y;
    if (RSDK_sceneInfo->currentDrawGroup != Zone->drawOrderHigh || RSDK_sceneInfo->inEditor) {
        RSDK.SetSpriteAnimation(LoveTester->aniFrames, 1, &entity->animator1, true, 0);
        RSDK.DrawSprite(&entity->animator1, NULL, false);

        for (int i = 0; i < 10; ++i) {
            entity->position.x = storeX;
            entity->position.y = storeY;
            entity->position.x = storeX + LoveTester->positions[i].x;
            entity->position.y = storeY + LoveTester->positions[i].y;
            RSDK.DrawSprite(&entity->animators[i], NULL, false);
        }

        entity->position.x = storeX;
        entity->position.y = storeY;
        entity->position.x = storeX + LoveTester->field_20.x;
        entity->position.y = storeY + LoveTester->field_20.y;
        LoveTester_Unknown2(entity->playerID, entity->field_74, entity->field_79);

        entity->position.x = storeX;
        entity->position.y = storeY;
        entity->position.x = storeX + LoveTester->field_28.x;
        entity->position.y = storeY + LoveTester->field_28.y;
        LoveTester_Unknown2(entity->field_80, entity->field_7C, entity->field_81);

        entity->position.x = storeX;
        entity->position.y = storeY;
    }

    if (RSDK_sceneInfo->currentDrawGroup == Zone->drawOrderHigh || RSDK_sceneInfo->inEditor) {
        RSDK.SetSpriteAnimation(LoveTester->aniFrames, 0, &entity->animator1, true, 0);
        RSDK.DrawSprite(&entity->animator1, NULL, false);
    }
}

void LoveTester_Unknown2(uint8 list, bool32 flag, uint8 frame)
{
    RSDK_THIS(LoveTester);

    RSDK.SetSpriteAnimation(LoveTester->aniFrames, 3, &entity->animator1, true, 0);
    RSDK.DrawSprite(&entity->animator1, NULL, false);
    if (flag) {
        RSDK.SetSpriteAnimation(LoveTester->aniFrames, 3, &entity->animator1, true, 1);
        RSDK.DrawSprite(&entity->animator1, NULL, false);
    }

    switch (list) {
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
            RSDK.SetSpriteAnimation(LoveTester->aniFrames, list + 4, &entity->animator1, true, frame);
            RSDK.DrawSprite(&entity->animator1, NULL, false);
            break;
        default: break;
    }
}

void LoveTester_CheckPlayerCollisions(void)
{
    RSDK_THIS(LoveTester);

    foreach_active(Player, player)
    {
        int storeX = player->position.x;
        int storeY = player->position.y;
        int velX   = player->velocity.x;
        int velY   = player->velocity.y;
        int side2  = Player_CheckCollisionBox(player, entity, &LoveTester->hitbox2);
        int side   = Player_CheckCollisionBox(player, entity, &LoveTester->hitbox3) | side2;
        if (side == C_TOP || side == C_BOTTOM) {
            player->onGround   = false;
            player->velocity.x = velX;
            player->velocity.y = velY;
            player->position.x = storeX;
            player->position.y = storeY;
        }
    }
}

void LoveTester_CheckPlayerCollisions2(bool32 flag)
{
    RSDK_THIS(LoveTester);

    foreach_active(Player, player)
    {
        int playerID = RSDK.GetEntityID(player);
        if (flag || !player->sidekick) {

            if (!((1 << playerID) & entity->activePlayers)) {
                if (Player_CheckBadnikTouch(player, entity, &LoveTester->hitbox1)) {
                    entity->activePlayers |= 1 << playerID;
                    RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->playerAnimator, true, 0);
                    player->nextGroundState = 0;
                    player->nextAirState    = 0;
                    player->velocity.x >>= 15;
                    player->velocity.y >>= 15;
                    player->onGround = false;
                    player->state    = Player_State_None;
                    if (!entity->playerPtr) {
                        entity->playerPtr = (Entity *)player;
                        if (player->camera) {
                            player->camera->targetPtr   = (Entity *)entity;
                            player->camera->offsetYFlag = false;
                        }
                    }
                }
            }

            if ((1 << playerID) & entity->activePlayers) {
                RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->playerAnimator, false, 0);
                player->position.x += (entity->position.x - player->position.x) >> 1;
                player->position.y += (entity->position.y + 0x10000 - player->position.y) >> 1;
                player->state = Player_State_None;
                if (entity->field_70) {
                    entity->activePlayers &= ~(1 << playerID);
                    player->state          = Player_State_Air;
                    player->tileCollisions = true;
                    player->onGround       = false;
                    player->velocity.x     = 0;
                    player->velocity.y     = 0;
                    if (player->camera) {
                        if (entity->playerPtr == (Entity *)player) {
                            player->camera->targetPtr   = (Entity *)player;
                            player->camera->offsetYFlag = true;
                        }
                    }
                }
            }
        }
    }
}

void LoveTester_GiveScore(void *p)
{
    RSDK_THIS(LoveTester);
    EntityPlayer *player = (EntityPlayer *)p;

    EntityScoreBonus *bonus = CREATE_ENTITY(ScoreBonus, NULL, player->position.x, player->position.y);
    bonus->drawOrder        = Zone->drawOrderHigh;
    bonus->animator.frameID = 0;
    Player_GiveScore(player, 100);
    RSDK.PlaySfx(LoveTester->sfxScore, false, 255);
}

void LoveTester_Unknown6(void)
{
    RSDK_THIS(LoveTester);

    for (int velX = 0, frame = 0; velX < 0x10000; velX += 0x4000, ++frame) {
        EntityLoveTester *child = CREATE_ENTITY(LoveTester, intToVoid(true), entity->position.x, entity->position.y);
        RSDK.SetSpriteAnimation(LoveTester->aniFrames, 4, &child->animator1, true, frame & 1);
        child->velocity.x = velX - 0x6000;
    }
}

void LoveTester_State_Unknown1(void)
{
    RSDK_THIS(LoveTester);

    for (int i = 0; i < 10; ++i) {
        RSDK.SetSpriteAnimation(LoveTester->aniFrames, 2, &entity->animators[i], true, 5);
    }

    entity->playerPtr     = NULL;
    entity->field_70      = 0;
    entity->activePlayers = 0;
    entity->state         = LoveTester_State_Unknown2;
    LoveTester_State_Unknown2();
}

void LoveTester_State_Unknown2(void)
{
    RSDK_THIS(LoveTester);

    LoveTester_CheckPlayerCollisions();
    LoveTester_CheckPlayerCollisions2(false);
    if (entity->playerPtr) {
        entity->timer    = 0;
        entity->field_74 = 1;
        entity->field_7C = 1;
        entity->playerID = 0;
        entity->field_79 = 0;
        entity->field_80 = 0;
        entity->field_81 = 0;
        entity->state    = LoveTester_State_Unknown3;
    }
}

void LoveTester_State_Unknown3(void)
{
    RSDK_THIS(LoveTester);

    LoveTester_CheckPlayerCollisions();
    LoveTester_CheckPlayerCollisions2(true);

    if (entity->timer >= 8) {
        EntityPlayer *player = (EntityPlayer *)entity->playerPtr;
        entity->timer        = 0;
        entity->field_74     = 1;
        entity->field_7C     = 1;

        switch (player->characterID) {
            case ID_SONIC: entity->playerID = 1; break;
            case ID_TAILS: entity->playerID = 2; break;
            case ID_KNUCKLES: entity->playerID = 3; break;
        }
        entity->field_82 = entity->playerID;

        do {
            entity->field_82 = RSDK.Rand(1, 6);
        } while (entity->field_82 == entity->playerID);
        entity->state = LoveTester_State_Unknown4;
    }
    else {
        entity->field_74 = !(entity->timer & 2);
        entity->field_7C = !(entity->timer & 2);
        entity->timer++;
    }
}

void LoveTester_State_Unknown4(void)
{
    RSDK_THIS(LoveTester);

    LoveTester_CheckPlayerCollisions();
    LoveTester_CheckPlayerCollisions2(true);

    if (entity->timer < 168) {
        int val     = entity->timer & 0x80000003;
        bool32 flag = !val;
        if (val < 0)
            flag = ((val - 1) | 0xFFFFFFFC) == -1;

        if (flag) {
            int startVal = entity->field_80;
            int id       = entity->playerID;
            while (true) {
                entity->field_80 = id;
                if (id != entity->playerID && id != startVal)
                    break;
                id = RSDK.Rand(1, 6);
            }
        }

        if (entity->timer >= 128)
            entity->field_80 = 0;
        if (entity->timer) {
            if (entity->timer == 80) {
                entity->field_88  = 0;
                entity->stateLate = LoveTester_StateLate_Unknown2;
            }
            else {
                if (entity->timer == 128) {
                    entity->field_88  = 0;
                    entity->stateLate = LoveTester_StateLate_Unknown3;
                }
            }
        }
        else {
            entity->field_88  = 0;
            entity->stateLate = LoveTester_StateLate_Unknown1;
        }
        entity->timer++;
    }
    else {
        EntityPlayer *player = (EntityPlayer *)entity->playerPtr;
        entity->field_88     = 0;
        entity->stateLate    = 0;
        switch (player->characterID) {
            case ID_SONIC:
                if (entity->field_82 == 4)
                    entity->state = LoveTester_State_Unknown7;
                else if (entity->field_82 == 5)
                    entity->state = LoveTester_State_Unknown5;
                else
                    entity->state = LoveTester_State_Unknown6;
                break;
            case ID_TAILS:
                if (entity->field_82 == 1)
                    entity->state = LoveTester_State_Unknown5;
                else if (entity->field_82 == 4)
                    entity->state = LoveTester_State_Unknown5;
                else
                    entity->state = LoveTester_State_Unknown6;
                break;
            case ID_KNUCKLES:
                if (entity->field_82 == 1)
                    entity->state = LoveTester_State_Unknown7;
                else if (entity->field_82 == 4)
                    entity->state = LoveTester_State_Unknown5;
                else
                    entity->state = LoveTester_State_Unknown6;
                break;
        }

        if (entity->state == LoveTester_State_Unknown5) {
            entity->field_79 = 1;
            LoveTester_Unknown6();
        }
        else if (entity->state == LoveTester_State_Unknown7) {
            entity->field_79 = 2;
        }

        entity->field_80 = entity->field_82;
        entity->timer    = 0;
    }
}

void LoveTester_State_Unknown5(void)
{
    RSDK_THIS(LoveTester);
    EntityPlayer *player = (EntityPlayer *)entity->playerPtr;

    if (entity->timer >= 64) {
        entity->timer    = 0;
        entity->field_80 = entity->field_82;
        entity->field_70 = 1;
        entity->state    = LoveTester_State_Unknown8;
        LoveTester_CheckPlayerCollisions();
        LoveTester_CheckPlayerCollisions2(true);
    }
    else {
        int timer = entity->timer % 32;
        if (timer) {
            if (timer == 16) {
                entity->field_80 = 6;
                LoveTester_GiveScore(player);
            }
            ++entity->timer;
            LoveTester_CheckPlayerCollisions();
            LoveTester_CheckPlayerCollisions2(true);
        }
        else {
            entity->field_80 = entity->field_82;
            LoveTester_GiveScore(player);
            ++entity->timer;
            LoveTester_CheckPlayerCollisions();
            LoveTester_CheckPlayerCollisions2(true);
        }
    }
}

void LoveTester_State_Unknown6(void)
{
    RSDK_THIS(LoveTester);

    if (entity->timer >= 24) {
        entity->timer    = 0;
        entity->field_70 = 1;
        entity->state    = LoveTester_State_Unknown8;
        LoveTester_CheckPlayerCollisions();
        LoveTester_CheckPlayerCollisions2(true);
    }
    else {
        if (!entity->timer)
            LoveTester_GiveScore(entity->playerPtr);
        ++entity->timer;
        LoveTester_CheckPlayerCollisions();
        LoveTester_CheckPlayerCollisions2(true);
    }
}

void LoveTester_State_Unknown7(void)
{
    RSDK_THIS(LoveTester);

    if (entity->timer >= 64) {
        entity->timer    = 0;
        entity->field_80 = entity->field_82;
        entity->field_70 = 1;
        entity->state    = LoveTester_State_Unknown8;
        LoveTester_CheckPlayerCollisions();
        LoveTester_CheckPlayerCollisions2(true);
    }
    else if (entity->timer % 32) {
        if ((entity->timer % 32) == 16)
            entity->field_80 = 7;
        entity->timer++;
        LoveTester_CheckPlayerCollisions();
        LoveTester_CheckPlayerCollisions2(true);
    }
    else {
        entity->field_80 = entity->field_82;
        entity->timer++;
        LoveTester_CheckPlayerCollisions();
        LoveTester_CheckPlayerCollisions2(true);
    }
}

void LoveTester_State_Unknown8(void)
{
    RSDK_THIS(LoveTester);

    LoveTester_CheckPlayerCollisions();
    foreach_active(Player, player)
    {
        if (player == (EntityPlayer *)entity->playerPtr) {
            if (!Player_CheckCollisionTouch(player, entity, &LoveTester->hitbox1)) {
                entity->activePlayers = 0;
                entity->state         = LoveTester_State_Unknown1;
            }
        }
    }
}

void LoveTester_State2_Unknown(void)
{
    RSDK_THIS(LoveTester);

    entity->position.x += entity->velocity.x;
    entity->velocity.y -= 0x700;
    entity->position.y += entity->velocity.y;
    if (!RSDK.CheckOnScreen(entity, &entity->updateRange))
        destroyEntity(entity);
}

void LoveTester_StateLate_Unknown1(void)
{
    RSDK_THIS(LoveTester);

    switch (entity->field_88 % 40) {
        case 0:
            RSDK.SetSpriteAnimation(LoveTester->aniFrames, 2, &entity->animators[1], true, 0);
            RSDK.SetSpriteAnimation(LoveTester->aniFrames, 2, &entity->animators[8], true, 0);
            break;
        case 8:
            RSDK.SetSpriteAnimation(LoveTester->aniFrames, 2, &entity->animators[3], true, 0);
            RSDK.SetSpriteAnimation(LoveTester->aniFrames, 2, &entity->animators[6], true, 0);
            break;
        case 16:
            RSDK.SetSpriteAnimation(LoveTester->aniFrames, 2, &entity->animators[5], true, 0);
            RSDK.SetSpriteAnimation(LoveTester->aniFrames, 2, &entity->animators[4], true, 0);
            break;
        case 24:
            RSDK.SetSpriteAnimation(LoveTester->aniFrames, 2, &entity->animators[7], true, 0);
            RSDK.SetSpriteAnimation(LoveTester->aniFrames, 2, &entity->animators[2], true, 0);
            break;
        case 32:
            RSDK.SetSpriteAnimation(LoveTester->aniFrames, 2, &entity->animators[9], true, 0);
            RSDK.SetSpriteAnimation(LoveTester->aniFrames, 2, &entity->animators[0], true, 0);
            break;
    }
    ++entity->field_88;
}

void LoveTester_StateLate_Unknown2(void)
{
    RSDK_THIS(LoveTester);

    switch (entity->field_88 % 24) {
        case 0:
            RSDK.SetSpriteAnimation(LoveTester->aniFrames, 2, entity->animators, true, 0);
            RSDK.SetSpriteAnimation(LoveTester->aniFrames, 2, &entity->animators[1], true, 0);
            RSDK.SetSpriteAnimation(LoveTester->aniFrames, 2, &entity->animators[8], true, 0);
            RSDK.SetSpriteAnimation(LoveTester->aniFrames, 2, &entity->animators[9], true, 0);
            break;
        case 8:
            RSDK.SetSpriteAnimation(LoveTester->aniFrames, 2, &entity->animators[2], true, 0);
            RSDK.SetSpriteAnimation(LoveTester->aniFrames, 2, &entity->animators[3], true, 0);
            RSDK.SetSpriteAnimation(LoveTester->aniFrames, 2, &entity->animators[6], true, 0);
            RSDK.SetSpriteAnimation(LoveTester->aniFrames, 2, &entity->animators[7], true, 0);
            break;
        case 16:
            RSDK.SetSpriteAnimation(LoveTester->aniFrames, 2, &entity->animators[4], true, 0);
            RSDK.SetSpriteAnimation(LoveTester->aniFrames, 2, &entity->animators[5], true, 0);
            break;
    }
    ++entity->field_88;
}

void LoveTester_StateLate_Unknown3(void)
{
    RSDK_THIS(LoveTester);

    switch (entity->field_88 % 20) {
        case 0:
            RSDK.SetSpriteAnimation(LoveTester->aniFrames, 2, &entity->animators[0], true, 0);
            RSDK.SetSpriteAnimation(LoveTester->aniFrames, 2, &entity->animators[1], true, 0);
            break;
        case 2:
            RSDK.SetSpriteAnimation(LoveTester->aniFrames, 2, &entity->animators[2], true, 0);
            RSDK.SetSpriteAnimation(LoveTester->aniFrames, 2, &entity->animators[3], true, 0);
            break;
        case 4:
            RSDK.SetSpriteAnimation(LoveTester->aniFrames, 2, &entity->animators[4], true, 0);
            RSDK.SetSpriteAnimation(LoveTester->aniFrames, 2, &entity->animators[5], true, 0);
            break;
        case 6:
            RSDK.SetSpriteAnimation(LoveTester->aniFrames, 2, &entity->animators[6], true, 0);
            RSDK.SetSpriteAnimation(LoveTester->aniFrames, 2, &entity->animators[7], true, 0);
            break;
        case 8:
            RSDK.SetSpriteAnimation(LoveTester->aniFrames, 2, &entity->animators[8], true, 0);
            RSDK.SetSpriteAnimation(LoveTester->aniFrames, 2, &entity->animators[9], true, 0);
            break;
    }
    ++entity->field_88;
}

#if RETRO_INCLUDE_EDITOR
void LoveTester_EditorDraw(void)
{
    RSDK_THIS(LoveTester);

    for (int i = 0; i < 10; ++i) {
        RSDK.SetSpriteAnimation(LoveTester->aniFrames, 2, &entity->animators[i], true, 5);
    }

    LoveTester_Draw();
}

void LoveTester_EditorLoad(void)
{
    LoveTester->aniFrames = RSDK.LoadSpriteAnimation("SPZ2/LoveTester.bin", SCOPE_STAGE);

    LoveTester_SetupHitboxes();
    LoveTester_SetupPositions();
}
#endif

void LoveTester_Serialize(void) {}
