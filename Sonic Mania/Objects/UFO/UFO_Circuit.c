#include "SonicMania.h"

ObjectUFO_Circuit *UFO_Circuit;

void UFO_Circuit_Update(void)
{
    RSDK_THIS(UFO_Circuit);
    StateMachine_Run(entity->state);
    RSDK.ProcessAnimation(&entity->animatorUFO);
}

void UFO_Circuit_LateUpdate(void)
{
    RSDK_THIS(UFO_Circuit);
    int z         = entity->position.y;
    int y         = entity->height;
    int x         = entity->position.x;
    entity->depth3D = UFO_Camera->matWorld.values[2][1] * (y >> 16) + UFO_Camera->matWorld.values[2][2] * (z >> 16)
                    + UFO_Camera->matWorld.values[2][0] * (x >> 16) + UFO_Camera->matWorld.values[2][3];
    if (entity->depth3D >= 0x400) {
        int depth = (int)((UFO_Camera->matWorld.values[0][3] << 8) + (UFO_Camera->matWorld.values[0][2] * (z >> 8) & 0xFFFFFF00)
                    + (UFO_Camera->matWorld.values[0][0] * (x >> 8) & 0xFFFFFF00) + (UFO_Camera->matWorld.values[0][1] * (y >> 8) & 0xFFFFFF00))
                    / entity->depth3D;
        entity->visible = abs(depth) < 0x100;
    }
}

void UFO_Circuit_StaticUpdate(void) {}

void UFO_Circuit_Draw(void)
{
    RSDK_THIS(UFO_Circuit);
    if (entity->depth3D >= 0x4000) {
        RSDK.Prepare3DScene(UFO_Circuit->sceneIndex);
        RSDK.MatrixScaleXYZ(&entity->matrix1, 0x200, 0x200, 0x200);
        RSDK.MatrixTranslateXYZ(&entity->matrix1, entity->position.x, entity->height, entity->position.y, false);
        RSDK.MatrixRotateY(&entity->matrix3, entity->angleY);
        RSDK.MatrixMultiply(&entity->matrix2, &entity->matrix3, &entity->matrix1);
        RSDK.MatrixMultiply(&entity->matrix2, &entity->matrix2, &UFO_Camera->matWorld);
        RSDK.MatrixMultiply(&entity->matrix3, &entity->matrix3, &UFO_Camera->matView);
        RSDK.AddMeshFrameTo3DScene(entity->animatorUFO.animationID, UFO_Circuit->sceneIndex, &entity->animatorUFO, S3D_FLATCLR_SHADED_BLENDED_SCREEN,
                                   &entity->matrix2, &entity->matrix3, 0xFFFFFF);
        RSDK.AddMeshFrameTo3DScene(UFO_Circuit->emeraldModel, UFO_Circuit->sceneIndex, &entity->animatorUFO, S3D_FLATCLR_SHADED_BLENDED_SCREEN,
                                   &entity->matrix2, &entity->matrix3, 0xFFFFFF);
        RSDK.Draw3DScene(UFO_Circuit->sceneIndex);
    }
}

void UFO_Circuit_Create(void *data)
{
    RSDK_THIS(UFO_Circuit);
    if (!RSDK_sceneInfo->inEditor) {
        entity->startPos.x = entity->position.x;
        entity->startPos.y = entity->position.y;
        int id             = RSDK.GetEntityID(entity);

        Entity *next = (Entity *)RSDK.GetEntityByID(id + 1);
        if (next->objectID == UFO_Circuit->objectID) {
            entity->nextNode = next;
        }
        else {
            for (int e = id - 1; e > 0; --e) {
                Entity *node = RSDK.GetEntityByID(e);
                if (node->objectID != UFO_Circuit->objectID) {
                    entity->nextNode = RSDK.GetEntityByID(e + 1);
                    break;
                }
            }
        }

        Entity *prev = (Entity *)RSDK.GetEntityByID(id - 1);
        if (prev->objectID == UFO_Circuit->objectID) {
            entity->prevNode = prev;
        }
        else {
            for (int e = id + 1; e < TEMPENTITY_START; ++e) {
                Entity *node = RSDK.GetEntityByID(e);
                if (node->objectID != UFO_Circuit->objectID) {
                    entity->prevNode = RSDK.GetEntityByID(e - 1);
                    break;
                }
            }
        }

        if (entity->startNode) {
            entity->active    = ACTIVE_NORMAL;
            entity->visible   = true;
            entity->drawOrder = 4;
            if (entity->reverse)
                entity->curNode = entity->prevNode;
            else
                entity->curNode = entity->nextNode;
            entity->groundVel = 0x70000;
            entity->field_7C  = 0x70000;
            RSDK.SetModelAnimation(UFO_Circuit->ufoModel, &entity->animatorUFO, 128, 0, true, 0);
            UFO_Circuit_Unknown1();
            entity->state = UFO_Circuit_Unknown4;
        }
    }
}

void UFO_Circuit_StageLoad(void)
{
    UFO_Circuit->ufoModel = RSDK.LoadMesh("Special/UFOChase.bin", SCOPE_STAGE);
    switch (UFO_Setup->specialStageID) {
        case 1: UFO_Circuit->emeraldModel = RSDK.LoadMesh("Special/EmeraldYellow.bin", SCOPE_STAGE); break;
        case 2: UFO_Circuit->emeraldModel = RSDK.LoadMesh("Special/EmeraldBlue.bin", SCOPE_STAGE); break;
        case 3: UFO_Circuit->emeraldModel = RSDK.LoadMesh("Special/EmeraldPurple.bin", SCOPE_STAGE); break;
        case 4: UFO_Circuit->emeraldModel = RSDK.LoadMesh("Special/EmeraldGrey.bin", SCOPE_STAGE); break;
        case 5: UFO_Circuit->emeraldModel = RSDK.LoadMesh("Special/EmeraldCyan.bin", SCOPE_STAGE); break;
        case 6: UFO_Circuit->emeraldModel = RSDK.LoadMesh("Special/EmeraldRed.bin", SCOPE_STAGE); break;
        default: UFO_Circuit->emeraldModel = RSDK.LoadMesh("Special/EmeraldGreen.bin", SCOPE_STAGE); break;
    }
    UFO_Circuit->sceneIndex = RSDK.Create3DScene("View:Special", 4096, SCOPE_STAGE);
    UFO_Circuit->nodeCount  = 0;
    foreach_all(UFO_Circuit, node) { UFO_Circuit->nodeCount++; }
    UFO_Circuit->field_8 = 0;
    UFO_Circuit->field_C = 0;
}

void UFO_Circuit_Unknown1(void)
{
    RSDK_THIS(UFO_Circuit);
    EntityUFO_Circuit *node = (EntityUFO_Circuit *)entity->curNode;
    entity->angle           = RSDK.ATan2((entity->position.x - node->position.x) >> 16, (entity->position.y - node->position.y) >> 16);
    switch (node->throttle) {
        case 2:
            UFO_Circuit->field_8 = 0x2000;
            UFO_Circuit->field_C = 0x8000;
            break;
        case 3:
            UFO_Circuit->field_8 = 0x4000;
            UFO_Circuit->field_C = 0x10000;
            break;
        case 4:
            UFO_Circuit->field_8 = 0x8000;
            UFO_Circuit->field_C = 0x20000;
            break;
        default:
            UFO_Circuit->field_8 = 0;
            UFO_Circuit->field_C = 0;
            break;
    }
}
void UFO_Circuit_Unknown2(void)
{
    RSDK_THIS(UFO_Circuit);
    EntityUFO_Circuit *node = (EntityUFO_Circuit *)entity->curNode;
    entity->angle           = RSDK.ATan2((entity->position.x - node->startPos.x) >> 16, (entity->position.y - node->startPos.y) >> 16);
    switch (node->throttle) {
        case 1:
            UFO_Circuit->field_8 = 0;
            UFO_Circuit->field_C = 0;
            break;
        case 2:
            UFO_Circuit->field_8 = 0x2000;
            UFO_Circuit->field_C = 0x8000;
            break;
        case 3:
            UFO_Circuit->field_8 = 0x4000;
            UFO_Circuit->field_C = 0x10000;
            break;
        case 4:
            UFO_Circuit->field_8 = 0x8000;
            UFO_Circuit->field_C = 0x20000;
            break;
        default: break;
    }
}
bool32 UFO_Circuit_Unknown3(void)
{
    RSDK_THIS(UFO_Circuit);

    foreach_active(UFO_Player, player)
    {
        EntityUFO_Circuit *curNode  = entity->curNode;
        EntityUFO_Circuit *targetNode = NULL;
        EntityUFO_Circuit *nextNode = curNode->nextNode;

        if (((player->position.x - entity->position.x) >> 16) * ((player->position.x - entity->position.x) >> 16)
                    + ((player->position.y - entity->position.y) >> 16) * ((player->position.y - entity->position.y) >> 16)
                >= 0x100000
            && nextNode != curNode) {
            int pos  = 0x7FFFFFFF;

            while (nextNode != curNode) {
                int pos2 = ((player->position.x - nextNode->position.x) >> 16) * ((player->position.x - nextNode->position.x) >> 16)
                           + ((player->position.y - nextNode->position.y) >> 16) * ((player->position.y - nextNode->position.y) >> 16);
                if (pos2 < pos)
                    targetNode = nextNode;
                if (pos2 >= pos)
                    pos2 = pos;
                nextNode = nextNode->nextNode;
                pos      = pos2;
                if (!nextNode)
                    break;
            }

            if (targetNode) {
                int id                     = 0;
                EntityUFO_Circuit *nodePtr = entity->curNode;
                if (entity->reverse) {
                    for (; nodePtr != targetNode; ++id) nodePtr = nodePtr->nextNode;
                }
                else {
                    for (; nodePtr != targetNode; ++id) nodePtr = nodePtr->prevNode;
                }
                if (id > UFO_Circuit->nodeCount >> 1) {
                    entity->reverse ^= 1;
                    EntityUFO_Circuit *node = entity->curNode;
                    if (entity->reverse)
                        entity->curNode = node->prevNode;
                    else
                        entity->curNode = node->nextNode;
                    UFO_Circuit_Unknown2();
                    return true;
                }
            }
        }
    }
    return false;
}
void UFO_Circuit_Unknown4(void)
{
    RSDK_THIS(UFO_Circuit);

    if (entity->groundVel >= entity->field_7C) {
        if (entity->groundVel > entity->field_7C) {
            if (entity->groundVel - 0x8000 < entity->field_7C)
                entity->groundVel = entity->field_7C;
            else
                entity->groundVel -= 0x8000;
        }
    }
    else {
        if (entity->groundVel + 0x8000 > entity->field_7C)
            entity->groundVel = entity->field_7C;
        else
            entity->groundVel += 0x8000;
    }

    entity->field_7C = UFO_Player->maxSpeed - (UFO_Player->maxSpeed - 0x70000) / 3;
    int vx           = ((((-entity->groundVel >> 8) * RSDK.Cos256(entity->angle)) - entity->velocity.x) >> 3) + entity->velocity.x;
    int vy           = ((((-entity->groundVel >> 8) * RSDK.Sin256(entity->angle)) - entity->velocity.y) >> 3) + entity->velocity.y;
    entity->position.x += vx;
    entity->velocity.x = vx;
    entity->position.y += vy;
    entity->velocity.y = vy;
    entity->angleY     = 4 * RSDK.ATan2(vy, -vx);
    if (!UFO_Circuit_Unknown3()) {
        EntityUFO_Circuit *curNode = entity->curNode;
        int rx                     = (entity->position.x - curNode->startPos.x) >> 16;
        int ry                     = (entity->position.y - curNode->startPos.y) >> 16;
        int r                      = entity->groundVel >> 5;
        if (rx * rx + ry * ry < r) {
            if (entity->reverse)
                entity->curNode = curNode->prevNode;
            else
                entity->curNode = curNode->nextNode;
            UFO_Circuit_Unknown2();
        }
        if (!UFO_Setup->timedOut) {
            foreach_active(UFO_Player, player)
            {
                int ry = (entity->height - player->height - 0xA0000) >> 16;
                int rx = (entity->position.y - player->position.y) >> 16;
                int r  = ((entity->position.x - player->position.x) >> 16) * ((entity->position.x - player->position.x) >> 16) + (rx * rx + ry * ry);
                if (!UFO_Setup->machLevel && r < 0xC000) {
                    entity->field_7C += (abs(player->velocity.y) + abs(player->velocity.x)) >> 1;
                }
                if (r >= 0x6000) {
                    if (r > 0xC0000) {
                        if (UFO_Setup->machLevel) {
                            if (UFO_Setup->machLevel == 1)
                                continue;
                            if (abs(player->velocity.y) + abs(player->velocity.x) >= entity->field_7C >> 1)
                                continue;
                            if (entity->groundVel <= 192 * entity->field_7C >> 8)
                                continue;
                            entity->groundVel -= UFO_Circuit->field_C;
                        }
                        else {
                            if (entity->groundVel > 192 * entity->field_7C >> 8) {
                                entity->groundVel -= UFO_Circuit->field_8;
                            }
                        }
                    }
                }
                else {
                    player->timer               = 0;
                    player->circuitPtr          = entity;
                    player->stateInput          = StateMachine_None;
                    player->state               = UFO_Player_Unknown10;
                    entity->state               = UFO_Circuit_Unknown5;
                    RSDK_sceneInfo->timeEnabled = false;
                }
            }
        }
    }
}
void UFO_Circuit_Unknown5(void)
{
    RSDK_THIS(UFO_Circuit);
    entity->velocity.x = entity->velocity.y - (entity->velocity.y >> 4);
    entity->velocity.y = entity->velocity.y - (entity->velocity.y >> 4);
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
}

void UFO_Circuit_EditorDraw(void) {}

void UFO_Circuit_EditorLoad(void) {}

void UFO_Circuit_Serialize(void)
{
    RSDK_EDITABLE_VAR(UFO_Circuit, VAR_UINT8, mode);
    RSDK_EDITABLE_VAR(UFO_Circuit, VAR_UINT8, throttle);
    RSDK_EDITABLE_VAR(UFO_Circuit, VAR_BOOL, startNode);
    RSDK_EDITABLE_VAR(UFO_Circuit, VAR_BOOL, reverse);
}
