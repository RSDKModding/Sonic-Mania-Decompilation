#include "SonicMania.h"

ObjectUFO_Circuit *UFO_Circuit;

void UFO_Circuit_Update(void)
{
    RSDK_THIS(UFO_Circuit);
    StateMachine_Run(self->state);
    RSDK.ProcessAnimation(&self->animatorUFO);
}

void UFO_Circuit_LateUpdate(void)
{
    RSDK_THIS(UFO_Circuit);
    int32 z         = self->position.y;
    int32 y         = self->height;
    int32 x         = self->position.x;
    self->depth3D = UFO_Camera->matWorld.values[2][1] * (y >> 16) + UFO_Camera->matWorld.values[2][2] * (z >> 16)
                    + UFO_Camera->matWorld.values[2][0] * (x >> 16) + UFO_Camera->matWorld.values[2][3];
    if (self->depth3D >= 0x400) {
        int32 depth = (int32)((UFO_Camera->matWorld.values[0][3] << 8) + (UFO_Camera->matWorld.values[0][2] * (z >> 8) & 0xFFFFFF00)
                    + (UFO_Camera->matWorld.values[0][0] * (x >> 8) & 0xFFFFFF00) + (UFO_Camera->matWorld.values[0][1] * (y >> 8) & 0xFFFFFF00))
                    / self->depth3D;
        self->visible = abs(depth) < 0x100;
    }
}

void UFO_Circuit_StaticUpdate(void) {}

void UFO_Circuit_Draw(void)
{
    RSDK_THIS(UFO_Circuit);
    if (self->depth3D >= 0x4000) {
        RSDK.Prepare3DScene(UFO_Circuit->sceneIndex);
        RSDK.MatrixScaleXYZ(&self->matrix1, 0x200, 0x200, 0x200);
        RSDK.MatrixTranslateXYZ(&self->matrix1, self->position.x, self->height, self->position.y, false);
        RSDK.MatrixRotateY(&self->matrix3, self->angleY);
        RSDK.MatrixMultiply(&self->matrix2, &self->matrix3, &self->matrix1);
        RSDK.MatrixMultiply(&self->matrix2, &self->matrix2, &UFO_Camera->matWorld);
        RSDK.MatrixMultiply(&self->matrix3, &self->matrix3, &UFO_Camera->matView);
        RSDK.AddMeshFrameTo3DScene(self->animatorUFO.animationID, UFO_Circuit->sceneIndex, &self->animatorUFO, S3D_FLATCLR_SHADED_BLENDED_SCREEN,
                                   &self->matrix2, &self->matrix3, 0xFFFFFF);
        RSDK.AddMeshFrameTo3DScene(UFO_Circuit->emeraldModel, UFO_Circuit->sceneIndex, &self->animatorUFO, S3D_FLATCLR_SHADED_BLENDED_SCREEN,
                                   &self->matrix2, &self->matrix3, 0xFFFFFF);
        RSDK.Draw3DScene(UFO_Circuit->sceneIndex);
    }
}

void UFO_Circuit_Create(void *data)
{
    RSDK_THIS(UFO_Circuit);
    if (!SceneInfo->inEditor) {
        self->startPos.x = self->position.x;
        self->startPos.y = self->position.y;
        int32 id             = RSDK.GetEntityID(self);

        Entity *next = (Entity *)RSDK.GetEntityByID(id + 1);
        if (next->objectID == UFO_Circuit->objectID) {
            self->nextNode = next;
        }
        else {
            for (int32 e = id - 1; e > 0; --e) {
                Entity *node = RSDK.GetEntityByID(e);
                if (node->objectID != UFO_Circuit->objectID) {
                    self->nextNode = RSDK.GetEntityByID(e + 1);
                    break;
                }
            }
        }

        Entity *prev = (Entity *)RSDK.GetEntityByID(id - 1);
        if (prev->objectID == UFO_Circuit->objectID) {
            self->prevNode = prev;
        }
        else {
            for (int32 e = id + 1; e < TEMPENTITY_START; ++e) {
                Entity *node = RSDK.GetEntityByID(e);
                if (node->objectID != UFO_Circuit->objectID) {
                    self->prevNode = RSDK.GetEntityByID(e - 1);
                    break;
                }
            }
        }

        if (self->startNode) {
            self->active    = ACTIVE_NORMAL;
            self->visible   = true;
            self->drawOrder = 4;
            if (self->reverse)
                self->curNode = self->prevNode;
            else
                self->curNode = self->nextNode;
            self->groundVel = 0x70000;
            self->field_7C  = 0x70000;
            RSDK.SetModelAnimation(UFO_Circuit->ufoModel, &self->animatorUFO, 128, 0, true, 0);
            UFO_Circuit_Unknown1();
            self->state = UFO_Circuit_Unknown4;
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
    EntityUFO_Circuit *node = (EntityUFO_Circuit *)self->curNode;
    self->angle           = RSDK.ATan2((self->position.x - node->position.x) >> 16, (self->position.y - node->position.y) >> 16);
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
    EntityUFO_Circuit *node = (EntityUFO_Circuit *)self->curNode;
    self->angle           = RSDK.ATan2((self->position.x - node->startPos.x) >> 16, (self->position.y - node->startPos.y) >> 16);
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
        EntityUFO_Circuit *curNode  = self->curNode;
        EntityUFO_Circuit *targetNode = NULL;
        EntityUFO_Circuit *nextNode = curNode->nextNode;

        if (((player->position.x - self->position.x) >> 16) * ((player->position.x - self->position.x) >> 16)
                    + ((player->position.y - self->position.y) >> 16) * ((player->position.y - self->position.y) >> 16)
                >= 0x100000
            && nextNode != curNode) {
            int32 pos  = 0x7FFFFFFF;

            while (nextNode != curNode) {
                int32 pos2 = ((player->position.x - nextNode->position.x) >> 16) * ((player->position.x - nextNode->position.x) >> 16)
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
                int32 id                     = 0;
                EntityUFO_Circuit *nodePtr = self->curNode;
                if (self->reverse) {
                    for (; nodePtr != targetNode; ++id) nodePtr = nodePtr->nextNode;
                }
                else {
                    for (; nodePtr != targetNode; ++id) nodePtr = nodePtr->prevNode;
                }
                if (id > UFO_Circuit->nodeCount >> 1) {
                    self->reverse ^= 1;
                    EntityUFO_Circuit *node = self->curNode;
                    if (self->reverse)
                        self->curNode = node->prevNode;
                    else
                        self->curNode = node->nextNode;
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

    if (self->groundVel >= self->field_7C) {
        if (self->groundVel > self->field_7C) {
            if (self->groundVel - 0x8000 < self->field_7C)
                self->groundVel = self->field_7C;
            else
                self->groundVel -= 0x8000;
        }
    }
    else {
        if (self->groundVel + 0x8000 > self->field_7C)
            self->groundVel = self->field_7C;
        else
            self->groundVel += 0x8000;
    }

    self->field_7C = UFO_Player->maxSpeed - (UFO_Player->maxSpeed - 0x70000) / 3;
    int32 vx           = ((((-self->groundVel >> 8) * RSDK.Cos256(self->angle)) - self->velocity.x) >> 3) + self->velocity.x;
    int32 vy           = ((((-self->groundVel >> 8) * RSDK.Sin256(self->angle)) - self->velocity.y) >> 3) + self->velocity.y;
    self->position.x += vx;
    self->velocity.x = vx;
    self->position.y += vy;
    self->velocity.y = vy;
    self->angleY     = 4 * RSDK.ATan2(vy, -vx);
    if (!UFO_Circuit_Unknown3()) {
        EntityUFO_Circuit *curNode = self->curNode;
        int32 rx                     = (self->position.x - curNode->startPos.x) >> 16;
        int32 ry                     = (self->position.y - curNode->startPos.y) >> 16;
        int32 r                      = self->groundVel >> 5;
        if (rx * rx + ry * ry < r) {
            if (self->reverse)
                self->curNode = curNode->prevNode;
            else
                self->curNode = curNode->nextNode;
            UFO_Circuit_Unknown2();
        }
        if (!UFO_Setup->timedOut) {
            foreach_active(UFO_Player, player)
            {
                int32 ry = (self->height - player->height - 0xA0000) >> 16;
                int32 rx = (self->position.y - player->position.y) >> 16;
                int32 r  = ((self->position.x - player->position.x) >> 16) * ((self->position.x - player->position.x) >> 16) + (rx * rx + ry * ry);
                if (!UFO_Setup->machLevel && r < 0xC000) {
                    self->field_7C += (abs(player->velocity.y) + abs(player->velocity.x)) >> 1;
                }
                if (r >= 0x6000) {
                    if (r > 0xC0000) {
                        if (UFO_Setup->machLevel) {
                            if (UFO_Setup->machLevel == 1)
                                continue;
                            if (abs(player->velocity.y) + abs(player->velocity.x) >= self->field_7C >> 1)
                                continue;
                            if (self->groundVel <= 192 * self->field_7C >> 8)
                                continue;
                            self->groundVel -= UFO_Circuit->field_C;
                        }
                        else {
                            if (self->groundVel > 192 * self->field_7C >> 8) {
                                self->groundVel -= UFO_Circuit->field_8;
                            }
                        }
                    }
                }
                else {
                    player->timer               = 0;
                    player->circuitPtr          = self;
                    player->stateInput          = StateMachine_None;
                    player->state               = UFO_Player_Unknown10;
                    self->state               = UFO_Circuit_Unknown5;
                    SceneInfo->timeEnabled = false;
                }
            }
        }
    }
}
void UFO_Circuit_Unknown5(void)
{
    RSDK_THIS(UFO_Circuit);
    self->velocity.x = self->velocity.y - (self->velocity.y >> 4);
    self->velocity.y = self->velocity.y - (self->velocity.y >> 4);
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
}

#if RETRO_INCLUDE_EDITOR
void UFO_Circuit_EditorDraw(void) {}

void UFO_Circuit_EditorLoad(void) {}
#endif

void UFO_Circuit_Serialize(void)
{
    RSDK_EDITABLE_VAR(UFO_Circuit, VAR_UINT8, mode);
    RSDK_EDITABLE_VAR(UFO_Circuit, VAR_UINT8, throttle);
    RSDK_EDITABLE_VAR(UFO_Circuit, VAR_BOOL, startNode);
    RSDK_EDITABLE_VAR(UFO_Circuit, VAR_BOOL, reverse);
}
