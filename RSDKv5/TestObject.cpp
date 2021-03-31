#include "RetroEngine.hpp"

ObjectTestObject *TestObject;


void TestObject_Update()
{
    EntityTestObject *entity = (EntityTestObject *)sceneInfo.entity;

    entity->velocity.x = 0;
    entity->velocity.y = 0;
    entity->groundVel  = 0;
    if (controller[0].keyUp.down) {
        if (entity->position.y > 0) {
            // entity->position.y -= 4 << 0x10;
            entity->velocity.y = -(4 << 0x10);
        }
    }
    else if (controller[0].keyDown.down) {
        // entity->position.y += 4 << 0x10;
        entity->velocity.y = (4 << 0x10);
    }

    if (controller[0].keyLeft.down) {
        if (entity->position.x > 0) {
            // entity->position.x -= 4 << 0x10;
            entity->groundVel  = -(4 << 0x10);
            entity->velocity.x = -(4 << 0x10);
        }
    }
    else if (controller[0].keyRight.down) {
        // entity->position.x += 4 << 0x10;
        entity->groundVel  = (4 << 0x10);
        entity->velocity.x = (4 << 0x10);
    }

    if (controller[0].keyA.press) {
        entity->direction++;
        entity->direction &= 3;
        if (!entity->direction) {
            entity->drawFX &= ~FX_FLIP;
        }
        else {
            entity->drawFX |= FX_FLIP;
        }
    }

    //entity->sensorY = 0x100000;
    //entity->sensorX[0] = 0xA0000;
    //entity->sensorX[1] = 0x50000;
    //entity->sensorX[2] = 0;
    //entity->sensorX[3] = -0x50000;
    //entity->sensorX[4] = -0xA0000;

    entity->tileCollisions = !controller[0].keyB.down;
    ProcessTileCollisions(entity, &TestObject->hitboxOuter, &TestObject->hitboxInner);
    //ObjectTileGrip(entity, TestObject->fgLayers, CMODE_FLOOR, 0, 0, 0x100000, 10);
}
void TestObject_LateUpdate() {}
void TestObject_StaticUpdate()
{
    //Entity *entity        = GetObjectByID(TestObject->entityID);
    //screens[0].position.x = (entity->position.x >> 0x10) - screens[0].centerX;
    //screens[0].position.y = (entity->position.y >> 0x10) - screens[0].centerY;
    //
    //if (screens[0].position.y < 0)
    //    screens[0].position.y = 0;
    //if (screens[0].position.x < 0)
    //    screens[0].position.x = 0;
    //if (controller[0].keyUp.down) {
    //    if (screens[0].position.y > 0)
    //        screens[0].position.y -= 4;
    //}
    //else if (controller[0].keyDown.down) {
    //    screens[0].position.y += 4;
    //}
    //
    //if (controller[0].keyLeft.down) {
    //    if (screens[0].position.x > 0)
    //        screens[0].position.x -= 4;
    //}
    //else if (controller[0].keyRight.down) {
    //    screens[0].position.x += 4;
    //}

    TestObject->timer++;
    //DrawAniTile(TestObject->sheetIndex, 2, 0, 32 * (TestObject->timer % 8), 32, 32);
}
void TestObject_Draw() { 
    //EntityTestObject *entity = (EntityTestObject *)sceneInfo.entity;
    //Vector2 drawPos;
    //DrawLine(currentScreen->centerX, currentScreen->centerY, currentScreen->centerX + 0x10, currentScreen->centerY + 0x20, 0x00FF00, 0xFF, INK_NONE,
    //         true);
    //DrawLine(currentScreen->centerX, currentScreen->centerY, currentScreen->centerX + (cos(TestObject->timer) * 0x40),
    //         currentScreen->centerY + (sin(TestObject->timer) * 0x40), 0x00FF00, 0xFF, INK_NONE, true); 
    //DrawLine(currentScreen->centerX, currentScreen->centerY, currentScreen->centerX * 3, currentScreen->centerY * 3, 0x00FF00, 0xFF, INK_NONE, true); 
    //DrawRectangle(currentScreen->centerX, currentScreen->centerY, 0x40 + (TestObject->timer / 0x08) % 0x80, 0x40 + (TestObject->timer / 0x04) % 0x80,
    //              0x00FF00, 0xFF, INK_NONE, true);

    //drawPos.x = currentScreen->centerX;
    //drawPos.y = currentScreen->centerY;
    //entity->drawFX    = FX_FLIP;
    //entity->direction = (TestObject->timer / 4) % 4;
    //DrawSprite(&entity->data, NULL, false);
    //DrawDeformedSprite(2, INK_NONE, 0xFF);

    //if (tileLayers[0].layout)
    //    DrawTile(tileLayers[0].layout, 4, 4, NULL, &drawPos, true);

    //DrawCircleOutline(currentScreen->centerX, currentScreen->centerY, 0x10 + (TestObject->timer / 0x08) % 0x80,
    //                  0x20 + (TestObject->timer / 0x04) % 0x80, 0xFF0000, 0xFF, INK_NONE, true);
    //int r = 0x20 + (TestObject->timer) % 0x100;
    //DrawCircleOutline(currentScreen->centerX, currentScreen->centerY, r, r + 1, 0xFF0000, 0xFF, INK_NONE, true);
    //DrawCircle(currentScreen->centerX, currentScreen->centerY, 0x10 + (TestObject->timer / 0x02) % 0x80, 0xFF0000, 0xFF, INK_NONE, true);

    Vector2 vertPos[4];
    uint vertClr[4];
    vertPos[0].x = (int)(10 + TestObject->timer * 0.3) << 0x10;
    vertPos[0].y = (int)(10 + TestObject->timer * 0.3) << 0x10;
    vertPos[1].x = (int)(10 + TestObject->timer * 0.4) << 0x10;
    vertPos[1].y = (int)(60 + TestObject->timer * 0.3) << 0x10;
    vertPos[2].x = (int)(60 + TestObject->timer * 0.5) << 0x10;
    vertPos[2].y = (int)(10 + TestObject->timer * 0.3) << 0x10;
    vertPos[3].x = (int)(60 + TestObject->timer * 0.6) << 0x10;
    vertPos[3].y = (int)(60 + TestObject->timer * 0.3) << 0x10;

    vertClr[0] = 0xFF0000;
    vertClr[1] = 0x00FF00;
    vertClr[2] = 0x0000FF;
    vertClr[3] = 0x000000;

    //DrawFace(vertPos, 4, 0xFF, 0x80, 0xFF, 0x40, INK_BLEND);
    //DrawBlendedFace(vertPos, vertClr, 3, 0xFF, INK_NONE);
}
void TestObject_Create(void *data)
{
    EntityTestObject *entity    = (EntityTestObject *)sceneInfo.entity;
    entity->active = ACTIVE_ALWAYS;
    entity->visible   = true;
    entity->drawOrder = 14;
    entity->tileCollisions      = true;
    entity->collisionLayers     = TestObject->fgLayers;
    SetSpriteAnimation(TestObject->spriteIndex, 0, &entity->data, true, 0);
    TestObject->entityID = GetEntityID((EntityBase*)entity);
}
void TestObject_StageLoad()
{
    TestObject->timer       = 0;
    TestObject->active      = ACTIVE_ALWAYS;
    TestObject->spriteIndex = LoadAnimation("Players/Sonic.bin", SCOPE_STAGE);
    TestObject->sheetIndex  = LoadSpriteSheet("GHZ/AniTiles.gif", SCOPE_STAGE);

    TestObject->hitboxOuter.left   = -10;
    TestObject->hitboxOuter.top    = -20;
    TestObject->hitboxOuter.right  = 10;
    TestObject->hitboxOuter.bottom = 20;

    TestObject->hitboxInner.left   = -9;
    TestObject->hitboxInner.top    = -20;
    TestObject->hitboxInner.right  = 9;
    TestObject->hitboxInner.bottom = 20;

    TestObject->fgLow  = GetSceneLayerID("FG Low");
    TestObject->fgHigh = GetSceneLayerID("FG High");

    if (TestObject->fgLowID) {
        TestObject->fgLowID = 1 << TestObject->fgLowID;
    }

    if (TestObject->fgHigh) {
        TestObject->fgHighID = 1 << TestObject->fgHigh;
    }

    TestObject->fgLayers = 1 << TestObject->fgLow;
    TestObject->fgLayers |= 1 << TestObject->fgHigh;

    CreateEntity(TestObject->objectID, NULL, 0, 0);
}
void TestObject_EditorDraw() {}
void TestObject_EditorLoad() {}
void TestObject_Serialize() {}




