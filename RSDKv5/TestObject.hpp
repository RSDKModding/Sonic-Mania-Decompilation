#ifndef OBJ_TESTOBJECT_H
#define OBJ_TESTOBJECT_H

// Object Class
struct ObjectTestObject : Object {
    ushort entityID;
    uint timer;
    ushort spriteIndex;
    ushort sheetIndex;
    Hitbox hitboxOuter;
    Hitbox hitboxInner;
    ushort fgLayers;
    ushort fgLow;
    ushort fgHigh;
    ushort fgLowID;
    ushort fgHighID;
};

// Entity Class
struct EntityTestObject : Entity {
    EntityAnimationData data;
};

// Object Entity
extern ObjectTestObject *TestObject;

// Standard Entity Events
void TestObject_Update();
void TestObject_LateUpdate();
void TestObject_StaticUpdate();
void TestObject_Draw();
void TestObject_Create(void *data);
void TestObject_StageLoad();
void TestObject_EditorDraw();
void TestObject_EditorLoad();
void TestObject_Serialize();

// Extra Entity Functions


#endif //!OBJ_TESTOBJECT_H
