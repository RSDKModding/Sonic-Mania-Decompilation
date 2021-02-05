#ifndef OBJECT_H
#define OBJECT_H

#define OBJECT_COUNT (0x400)
#define ENTITY_COUNT (0x9A0)

enum AttributeTypes {
	ATTRIBUTE_UINT8,
	ATTRIBUTE_UINT16,
	ATTRIBUTE_UINT32,
	ATTRIBUTE_INT8,
	ATTRIBUTE_INT16,
	ATTRIBUTE_INT32,
	ATTRIBUTE_VAR,
	ATTRIBUTE_BOOL,
	ATTRIBUTE_STRING,
	ATTRIBUTE_VECTOR2,
	ATTRIBUTE_VECTOR3,
	ATTRIBUTE_COLOUR,
};

enum ActiveFlags {
	ACTIVE_NEVER3 = -1,
	ACTIVE_NEVER,
	ACTIVE_ALWAYS,
	ACTIVE_NORMAL,
	ACTIVE_PAUSED,
	ACTIVE_BOUNDS,
	ACTIVE_XBOUNDS,
	ACTIVE_YBOUNDS,
	ACTIVE_BOUNDS2,
};

struct Object {
    short objectID;
    byte priority;
    byte field_3;
};

struct Entity {
    Vector2 position;
    Vector2 scale;
    Vector2 velocity;
    Vector2 updateRange;
    int angle;
    int alpha;
    int rotation;
    int speed;
    int depth;
    ushort entityID;
    ushort objectID;
    int onScreen;
    int field_3C;
    int tileCollisions;
    int interaction;
    int grounded;
    byte priority;
    byte filter;
    byte direction;
    byte drawOrder;
    byte collisionLayers;
    byte collisionPlane;
    byte collisionMode;
    byte drawFX;
    byte inkEffect;
    byte visible;
    byte activeScreens;
    byte field_57;
};

struct EntityBase : Entity {
    byte data[0x400];
};

struct ObjectInfo {
    uint hash[4];
    void (*update)(void);
    void (*lateUpdate)(void);
    void (*staticUpdate)(void);
    void (*draw)(void);
    void(__cdecl *create)(void *);
    void (*stageLoad)(void);
    void (*editorDraw)(void);
    void (*editorLoad)(void);
    void (*serialize)(void);
    Object *type;
    int entitySize;
    int objectSize;
};              

extern int objectCount;
extern ObjectInfo objectList[OBJECT_COUNT];
extern int globalObjectCount;
extern int globalObjectIDs[OBJECT_COUNT];
extern int stageObjectCount;
extern int stageObjectIDs[OBJECT_COUNT];

extern EntityBase objectEntityList[ENTITY_COUNT];

void CreateObject(Object *structPtr, const char *name, uint entitySize, uint objectSize, void (*update)(void), void (*lateUpdate)(void),
                  void (*staticUpdate)(void), void (*draw)(void), void(__cdecl *create)(void *), void (*stageLoad)(void), void (*editorDraw)(void),
                  void (*editorLoad)(void), void (*serialize)(void));
void CreateObjectContainer(Object *structPtr, const char *name, uint objectSize);

#endif // !OBJECT_H