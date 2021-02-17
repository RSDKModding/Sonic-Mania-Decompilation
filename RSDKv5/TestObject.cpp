#include "RetroEngine.hpp"

ObjectTestObject *TestObject;


void TestObject_Update()
{
}
void TestObject_LateUpdate() {}
void TestObject_StaticUpdate()
{
    if (controller[0].keyUp.down) {
        if (screens[0].position.y > 0)
            screens[0].position.y -= 4;
    }
    else if (controller[0].keyDown.down) {
        screens[0].position.y += 4;
    }

    if (controller[0].keyLeft.down) {
        if (screens[0].position.x > 0)
            screens[0].position.x -= 4;
    }
    else if (controller[0].keyRight.down) {
        screens[0].position.x += 4;
    }

    TestObject->timer++;
    //DrawAniTile(TestObject->sheetIndex, 2, 0, 32 * (TestObject->timer % 8), 32, 32);
}
void TestObject_Draw() { 
    EntityTestObject *entity = (EntityTestObject *)sceneInfo.entity;
    Vector2 drawPos;
    //DrawLine(currentScreen->centerX, currentScreen->centerY, currentScreen->centerX + (sin(TestObject->timer / 0x08) * 0x10),
    //         currentScreen->centerY + (cos(TestObject->timer / 0x08) * 0x10), 0x00FF00, 0xFF, INK_NONE, true); 
    //DrawRectangle(currentScreen->centerX, currentScreen->centerY, 0x40 + (TestObject->timer / 0x08) % 0x80, 0x40 + (TestObject->timer / 0x04) % 0x80,
    //              0x00FF00, 0xFF, INK_NONE, true);

    //drawPos.x = currentScreen->centerX;
    //drawPos.y = currentScreen->centerY;
    //entity->drawFX    = FX_FLIP;
    //entity->direction = (TestObject->timer / 4) % 4;
    //DrawSprite(&entity->data, &drawPos, false);
    //DrawDeformedSprite(2, INK_NONE, 0xFF);

    //if (tileLayers[0].layout)
    //    DrawTile(tileLayers[0].layout, 4, 4, NULL, &drawPos, true);

    //DrawCircleOutline(currentScreen->centerX, currentScreen->centerY, 0x10 + (TestObject->timer / 0x08) % 0x80,
    //                  0x20 + (TestObject->timer / 0x04) % 0x80, 0xFF0000, 0xFF, INK_NONE, true);
    DrawCircle(currentScreen->centerX, currentScreen->centerY, 0x10 + (TestObject->timer / 0x02) % 0x80, 0xFF0000, 0xFF, INK_NONE, true);

}
void TestObject_Create(void *data)
{
    EntityTestObject *entity    = (EntityTestObject *)sceneInfo.entity;
    entity->active = ACTIVE_ALWAYS;
    entity->visible   = true;
    entity->drawOrder = 4;
    SetSpriteAnimation(TestObject->spriteIndex, 0, &entity->data, true, 0);
}
void TestObject_StageLoad()
{
    TestObject->timer       = 0;
    TestObject->active      = ACTIVE_ALWAYS;
    TestObject->spriteIndex = LoadAnimation("Players/Sonic.bin", SCOPE_STAGE);
    TestObject->sheetIndex  = LoadSpriteSheet("GHZ/AniTiles.gif", SCOPE_STAGE);
}
void TestObject_EditorDraw() {}
void TestObject_EditorLoad() {}
void TestObject_Serialize() {}





