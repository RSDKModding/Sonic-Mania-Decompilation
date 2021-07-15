#include "../SonicMania.h"

ObjectMonkeyDude *MonkeyDude;

void MonkeyDude_Update(void)
{
    RSDK_THIS(MonkeyDude);
    StateMachine_Run(entity->state);
}

void MonkeyDude_LateUpdate(void)
{

}

void MonkeyDude_StaticUpdate(void)
{

}

void MonkeyDude_Draw(void)
{
    RSDK_THIS(MonkeyDude);
    if (entity->state == MonkeyDude_State_Coconut) {
        RSDK.DrawSprite(&entity->coconutData, NULL, false);
    }
    else {
        Vector2 drawPos;

        if (!entity->direction)
            drawPos.x = entity->position.x - 0xD0000;
        else
            drawPos.x = entity->position.x + 0xD0000;

        if (!entity->coconutData.frameID)
            drawPos.y = entity->position.y + -0x20000;
        else
            drawPos.y = entity->position.y + -0x40000;
        RSDK.DrawSprite(&MonkeyDude->bodyData, &drawPos, false);

        for (entity->bodyPartID = 0; entity->bodyPartID < 4; ++entity->bodyPartID) {
            if (entity->direction) {
                drawPos.x += RSDK.Sin256(entity->bodyAngles[entity->bodyPartID]) << 11;
            }
            else {
                drawPos.x -= RSDK.Sin256(entity->bodyAngles[entity->bodyPartID]) << 11;
            }

            drawPos.y += RSDK.Cos256(entity->bodyAngles[entity->bodyPartID]) << 11;

            Animator *animator = NULL;
            if (entity->bodyPartID == 3) {
                if (entity->throwCount >= 4)
                    animator = &entity->handData;
                else
                    animator = &MonkeyDude->coconutData;
            }
            else {
                animator = &MonkeyDude->bodyData;
            }
            RSDK.DrawSprite(animator, &drawPos, false);
        }

        RSDK.DrawSprite(&entity->coconutData, NULL, false);

        drawPos.x = entity->position.x;
        drawPos.y = entity->drawY;
        RSDK.DrawSprite(&MonkeyDude->mainData, &drawPos, false);
    }
}

void MonkeyDude_Create(void* data)
{
    RSDK_THIS(MonkeyDude);
    entity->visible = true;
    entity->drawOrder = Zone->drawOrderLow;
    entity->drawFX |= FX_FLIP;
    entity->startPos           = entity->position;
    entity->startDir           = entity->direction;
    entity->active        = ACTIVE_BOUNDS;
    entity->updateRange.x = 0x800000;
    entity->updateRange.y = 0x800000;
    if (entity->nummoves == 0)
        entity->nummoves = 3;

    if (data) {
        RSDK.SetSpriteAnimation(MonkeyDude->aniFrames, 4, &entity->coconutData, true, 0);
        entity->state = MonkeyDude_State_Coconut;
    }
    else {
        RSDK.SetSpriteAnimation(MonkeyDude->aniFrames, 0, &entity->coconutData, true, 0);
        RSDK.SetSpriteAnimation(MonkeyDude->aniFrames, 3, &entity->handData, true, 0);
        entity->state = MonkeyDude_State_Setup;
    }
}

void MonkeyDude_StageLoad(void)
{
    MonkeyDude->aniFrames            = RSDK.LoadSpriteAnimation("AIZ/MonkeyDude.bin", SCOPE_STAGE);
    MonkeyDude->hitbox.left          = -10;
    MonkeyDude->hitbox.top           = -21;
    MonkeyDude->hitbox.right         = 7;
    MonkeyDude->hitbox.bottom        = 4;
    MonkeyDude->coconutHitbox.left   = -4;
    MonkeyDude->coconutHitbox.top    = -4;
    MonkeyDude->coconutHitbox.right  = 4;
    MonkeyDude->coconutHitbox.bottom = 4;
    RSDK.SetSpriteAnimation(MonkeyDude->aniFrames, 2, &MonkeyDude->bodyData, true, 0);
    RSDK.SetSpriteAnimation(MonkeyDude->aniFrames, 1, &MonkeyDude->mainData, true, 0);
    RSDK.SetSpriteAnimation(MonkeyDude->aniFrames, 4, &MonkeyDude->coconutData, true, 0);
    MonkeyDude->sfxDrop = RSDK.GetSFX("Stage/Drop.wav");
    DEBUGMODE_ADD_OBJ(MonkeyDude);
}

void MonkeyDude_DebugDraw(void)
{
    RSDK_THIS(MonkeyDude);
    RSDK.SetSpriteAnimation(MonkeyDude->aniFrames, 0, &entity->coconutData, true, 0);
    RSDK.DrawSprite(&entity->coconutData, NULL, false);
}

void MonkeyDude_DebugSpawn(void)
{
    RSDK_THIS(MonkeyDude);
    EntityMonkeyDude *monkeyDude = CREATE_ENTITY(MonkeyDude, NULL, entity->position.x, entity->position.y);
    monkeyDude->direction        = entity->direction;
    monkeyDude->startDir         = entity->direction;
}

void MonkeyDude_State_Setup(void)
{
    RSDK_THIS(MonkeyDude);
    entity->drawY           = RSDK_sceneInfo->entity->position.y;
    entity->active     = ACTIVE_NORMAL;
    entity->velocity.y = 0x10000;
    entity->moveCount       = 1;
    entity->timer           = 1;
    entity->angleSpeed      = 4;
    entity->activeParts     = 0;
    entity->throwCount      = 0;
    entity->state           = MonkeyDude_State2;
    RSDK.ProcessAnimation(&entity->coconutData);
    if (--entity->timer <= 0) {
        entity->field_FC = 0;
        entity->timer    = 8;
        entity->state    = MonkeyDude_State3;
    }
    MonkeyDude_HandleStates();
    entity->bodyAngles[0]  = 0;
    entity->bodyUnknown[0] = 0;
    entity->bodyStates[0]  = MonkeyDude_State_BodyUnknown;
    entity->bodyAngles[1]  = 0;
    entity->bodyUnknown[1] = 8;
    entity->bodyStates[1]  = MonkeyDude_State_BodyUnknown;
    entity->bodyAngles[2]  = 0;
    entity->bodyUnknown[2] = 16;
    entity->bodyStates[2]  = MonkeyDude_State_BodyUnknown;
    entity->bodyAngles[3]  = 0;
    entity->bodyUnknown[3] = 24;
    entity->bodyStates[3]  = MonkeyDude_State_BodyUnknown;
}

void MonkeyDude_HandleBodyPart(void)
{
    RSDK_THIS(MonkeyDude);
    entity->bodyUnknown[entity->bodyPartID] = entity->bodyPartID;
    switch (entity->throwCount) {
        case 0:
        case 4:
            entity->angleSpeed = 4;
            entity->bodyUnknown[entity->bodyPartID] *= 4;
            entity->bodyUnknown2[entity->bodyPartID] ^= 1;
            if (!entity->bodyPartID) {
                entity->currentAngle = entity->bodyAngles[0];
                if (entity->bodyUnknown2[0]) {
                    entity->currentAngle -= RSDK.Rand(0, 96);
                    if (entity->currentAngle < 32)
                        entity->currentAngle = 32;
                }
                else {
                    entity->currentAngle += RSDK.Rand(0, 96);
                    if (entity->currentAngle > 128)
                        entity->currentAngle = 128;
                }
            }
            if (entity->bodyPartID == 3)
                entity->activeParts = 0;
            break;
        case 1:
            entity->angleSpeed = 5;
            entity->bodyUnknown[entity->bodyPartID] *= 2;
            entity->currentAngle                     = 176;
            entity->bodyUnknown2[entity->bodyPartID] = 0;
            if (entity->bodyPartID == 3) {
                ++entity->throwCount;
                entity->activeParts = 0;
            }
            break;
        case 2:
            entity->bodyUnknown[entity->bodyPartID] *= 2;
            entity->currentAngle                     = 96;
            entity->bodyUnknown2[entity->bodyPartID] = 1;
            if (entity->bodyPartID == 3) {
                ++entity->throwCount;
                entity->activeParts = 0;
            }
            break;
        default:
            if (entity->bodyPartID == 3)
                entity->activeParts = 0;
            break;
    }
}

void MonkeyDude_HandleStates(void)
{
    RSDK_THIS(MonkeyDude);
    foreach_active(Player, player)
    {
        if (Player_CheckBadnikHit(player, entity, &MonkeyDude->hitbox))
            Player_CheckBadnikBreak(entity, player, true);
    }

    if (entity->objectID) {
        if (entity->throwCount) {
            if (entity->throwCount == 3 && entity->bodyAngles[3] <= 164) {
                int spawnX  = entity->position.x + 0xD0000;
                if (!entity->direction)
                    spawnX = entity->position.x - 0xD0000;
                int spawnY = entity->position.y;

                for (int i = 0; i < 4; ++i) {
                    if (entity->direction)
                        spawnX += RSDK.Sin256(entity->bodyAngles[i]) << 11;
                    else
                        spawnX -= RSDK.Sin256(entity->bodyAngles[i]) << 11;
                    spawnY += (RSDK.Cos256(entity->bodyAngles[i]) << 11);
                }

                EntityMonkeyDude *coconut = CREATE_ENTITY(MonkeyDude, intToVoid(1), spawnX, spawnY);
                if (!entity->direction)
                    coconut->velocity.x = -0x20000;
                else
                    coconut->velocity.x = 0x20000;
                coconut->velocity.y = -0x40000;
                coconut->direction  = entity->direction;
                RSDK.PlaySFX(MonkeyDude->sfxDrop, 0, 255);
                ++entity->throwCount;
            }
        }
        else {
            EntityPlayer *target = Player_Unknown3();
            if (!entity->direction) {
                if (target->position.x < entity->position.x) {
                    if (abs(entity->position.x - target->position.x) <= 0x800000) {
                        if (target->position.y > entity->position.y - 0x200000 && target->position.y - entity->position.y <= 0x800000
                            && entity->activeScreens == 1) {
                            entity->throwCount   = 1;
                            entity->currentAngle = entity->bodyAngles[0];
                        }
                    }
                }
            }
        }

        for (entity->bodyPartID = 0; entity->bodyPartID < 4; ++entity->bodyPartID) {
            if (entity->bodyStates[entity->bodyPartID])
                entity->bodyStates[entity->bodyPartID]();
        }

        if (!RSDK.CheckOnScreen(entity, NULL) && !RSDK.CheckPosOnScreen(&entity->startPos, &entity->updateRange)) {
            entity->position  = entity->startPos;
            entity->direction = entity->startDir;
            MonkeyDude_Create(0);
        }
    }
}

void MonkeyDude_State2(void)
{
    RSDK_THIS(MonkeyDude);
    RSDK.ProcessAnimation(&entity->coconutData);
    if (--entity->timer <= 0) {
        entity->field_FC = 0;
        entity->timer    = 8;
        entity->state    = MonkeyDude_State3;
    }
    MonkeyDude_HandleStates();
}

void MonkeyDude_State3(void)
{
    RSDK_THIS(MonkeyDude);
    entity->drawY += entity->velocity.y;
    entity->coconutData.frameID = 0;
    if (--entity->timer <= 0) {
        entity->timer = 8;
        entity->state = MonkeyDude_State4;
    }
    MonkeyDude_HandleStates();
}

void MonkeyDude_State4(void)
{
    RSDK_THIS(MonkeyDude);
    entity->position.y += entity->velocity.y;
    if (--entity->timer <= 0) {
        if (--entity->moveCount > 0) {
            entity->timer = 8;
            entity->state = MonkeyDude_State3;
        }
        else {
            entity->timer           = 60;
            entity->velocity.y      = -entity->velocity.y;
            entity->moveCount       = entity->nummoves;
            entity->state           = MonkeyDude_State2;
        }
    }
    MonkeyDude_HandleStates();
}

void MonkeyDude_State_BodyUnknown(void)
{
    RSDK_THIS(MonkeyDude);
    if (entity->activeParts == 15) {
        entity->bodyStates[entity->bodyPartID] = MonkeyDude_State_BodyUnknown2;
        MonkeyDude_HandleBodyPart();
    }

    if (entity->bodyUnknown[entity->bodyPartID]) {
        entity->bodyUnknown[entity->bodyPartID]--;
    }
    else if (entity->bodyAngles[entity->bodyPartID] >= 128) {
        entity->activeParts |= (1 << entity->bodyPartID);
    }
    else {
        entity->bodyAngles[entity->bodyPartID] += entity->angleSpeed;
    }
}

void MonkeyDude_State_BodyUnknown2(void)
{
    RSDK_THIS(MonkeyDude);
    if (entity->activeParts == 0xF)
        MonkeyDude_HandleBodyPart();

    if (entity->bodyUnknown[entity->bodyPartID]) {
        entity->bodyUnknown[entity->bodyPartID]--;
    }
    else {
        if (entity->bodyUnknown2[entity->bodyPartID]) {
            if (entity->bodyAngles[entity->bodyPartID] <= entity->currentAngle) {
                entity->activeParts |= (1 << entity->bodyPartID);
            }
            else {
                entity->bodyAngles[entity->bodyPartID] -= entity->angleSpeed;
            }
        }
        else {
            if (entity->bodyAngles[entity->bodyPartID] >= entity->currentAngle) {
                entity->activeParts |= (1 << entity->bodyPartID);
            }
            else {
                entity->bodyAngles[entity->bodyPartID] += entity->angleSpeed;
            }
        }
    }
}

void MonkeyDude_State_Coconut(void)
{
    RSDK_THIS(MonkeyDude);
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    entity->velocity.y += 0x3800;
    if (RSDK.CheckOnScreen(entity, NULL)) {
        RSDK.ProcessAnimation(&entity->coconutData);
        foreach_active(Player, player)
        {
            if (Player_CheckCollisionTouch(player, entity, &MonkeyDude->coconutHitbox)) {
                Player_CheckProjectileHit(player, entity);
            }
        }
    }
    else {
        destroyEntity(entity);
    }
}

void MonkeyDude_EditorDraw(void)
{

}

void MonkeyDude_EditorLoad(void)
{

}

void MonkeyDude_Serialize(void) { RSDK_EDITABLE_VAR(MonkeyDude, VAR_ENUM, nummoves); }

