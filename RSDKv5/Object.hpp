#ifndef OBJECT_H
#define OBJECT_H

#define OBJECT_COUNT (0x400)

//0x800 scene objects, 0x40 reserved ones, and 0x100 spare slots for creation
#define RESERVE_ENTITY_COUNT (0x40)
#define TEMPENTITY_COUNT     (0x100)
#define SCENEENTITY_COUNT    (0x800)
#define ENTITY_COUNT         (RESERVE_ENTITY_COUNT + SCENEENTITY_COUNT + TEMPENTITY_COUNT)
#define TEMPENTITY_START     (ENTITY_COUNT - TEMPENTITY_COUNT)

#define TYPE_COUNT (0x100)
#define EDITABLEVAR_COUNT (0x100)
#define TYPEGROUP_COUNT (0x104)

#define FOREACH_STACK_COUNT (0x400)

enum StaticObjectTypes {
    SVAR_UINT8,
    SVAR_UINT16,
    SVAR_UINT32,
    SVAR_INT8,
    SVAR_INT16,
    SVAR_INT32,
    SVAR_BOOL,
    SVAR_PTR,
    SVAR_VEC2,
    SVAR_TEXT,
    SVAR_ANIMATOR,
    SVAR_HITBOX,
    SVAR_UNKNOWN,
};

enum TypeGroups {
    GROUP_ALL     = 0,
    GROUP_CUSTOM1 = TYPE_COUNT,
    GROUP_CUSTOM2,
    GROUP_CUSTOM3,
    GROUP_CUSTOM4,
};

enum AttributeTypes {
	VAR_UINT8,
	VAR_UINT16,
	VAR_UINT32,
	VAR_INT8,
	VAR_INT16,
	VAR_INT32,
	VAR_ENUM,
	VAR_BOOL,
	VAR_STRING,
	VAR_VECTOR2,
	VAR_UNKNOWN,
	VAR_COLOUR,
};

enum ActiveFlags {
    ACTIVE_NEVER,   // never update
    ACTIVE_ALWAYS,  // always update (even if paused/frozen)
    ACTIVE_NORMAL,  // always update (unless paused/frozen)
    ACTIVE_PAUSED,  // update if paused
    ACTIVE_BOUNDS,  // update if in x & y bounds
    ACTIVE_XBOUNDS, // update only if in x bounds (y bounds dont matter)
    ACTIVE_YBOUNDS, // update only if in y bounds (x bounds dont matter)
    ACTIVE_RBOUNDS, // update based on radius boundaries (updateRange.x = radius)
};

enum DefaultObjects {
    TYPE_DEFAULTOBJECT = 0,
#if RETRO_REV02
    TYPE_DEVOUTPUT,
#endif
    TYPE_TESTOBJECT,
    TYPE_DEFAULTCOUNT, //max
};

struct UnknownStruct {
    short v1;
    short v2;
    short v3;
    short v4;
    short v5;
    short v6;
    short v7;
    short v8;
    short v9;
};

struct Object {
    short objectID;
    byte active;
};

struct Entity {
    Vector2 position;
    Vector2 scale;
    Vector2 velocity;
    Vector2 updateRange;
    int angle;
    int alpha;
    int rotation;
    int groundVel;
    int depth;
    ushort group;
    ushort objectID;
    bool32 inBounds;
    bool32 isPermanent;
    bool32 tileCollisions;
    bool32 interaction;
    bool32 onGround;
    byte active;
#if RETRO_REV02
    byte filter;
#endif
    byte direction;
    byte drawOrder;
    byte collisionLayers;
    byte collisionPlane;
    byte collisionMode;
    byte drawFX;
    byte inkEffect;
    byte visible;
    byte activeScreens;
};

struct EntityBase : Entity {
    void *data[0x100];
};

struct ObjectInfo {
    uint hash[4];
    void (*update)(void);
    void (*lateUpdate)(void);
    void (*staticUpdate)(void);
    void (*draw)(void);
    void(*create)(void *);
    void (*stageLoad)(void);
    void (*editorDraw)(void);
    void (*editorLoad)(void);
    void (*serialize)(void);
    Object **type;
    int entitySize;
    int objectSize;
};        

struct EditableVarInfo {
    uint hash[4];
    int offset;
    int active;
    byte type;
};

struct ForeachStackInfo {
    int id;
};

struct TypeGroupList {
    int entryCount;
    ushort entries[ENTITY_COUNT];
};

extern int objectCount;
extern ObjectInfo objectList[OBJECT_COUNT];
extern int globalObjectCount;
extern int globalObjectIDs[OBJECT_COUNT];
extern int stageObjectIDs[OBJECT_COUNT];

extern EntityBase objectEntityList[ENTITY_COUNT];

extern EditableVarInfo *editableVarList;
extern int editableVarCount;

extern ForeachStackInfo foreachStackList[FOREACH_STACK_COUNT];
extern ForeachStackInfo *foreachStackPtr;

extern TypeGroupList typeGroups[TYPEGROUP_COUNT];

extern bool32 validDraw;

void RegisterObject(Object **structPtr, const char *name, uint entitySize, uint objectSize, void (*update)(void), void (*lateUpdate)(void),
                  void (*staticUpdate)(void), void (*draw)(void), void(*create)(void *), void (*stageLoad)(void), void (*editorDraw)(void),
                  void (*editorLoad)(void), void (*serialize)(void));
#if RETRO_REV02
void RegisterObjectContainer(Object **structPtr, const char *name, uint objectSize);
#endif

void LoadStaticObject(byte *obj, uint *hash, int dataPos);

#define RSDK_EDITABLE_VAR(object, type, var) RSDK.SetEditableVar(type, #var, object->objectID, offsetof(Entity##object, var))

inline void SetEditableVar(byte type, const char *name, byte object, int offset)
{
    if (editableVarCount < 255) {
        EditableVarInfo *var = &editableVarList[editableVarCount];
        GEN_HASH(name, var->hash);
        var->type   = type;
        var->offset = offset;
        var->active = true;
        editableVarCount++;
    }
}

void SetActiveVariable(int objectID, const char *name);
void AddEnumVar(const char *name);

void InitObjects();
void ProcessObjects();
void ProcessPausedObjects();
void ProcessFrozenObjects();
void ProcessObjectDrawLists();

ushort GetObjectByName(const char *name);
inline Entity* GetObjectByID(ushort objectID)
{
    return &objectEntityList[objectID < ENTITY_COUNT ? objectID : (ENTITY_COUNT - 1)];
}

inline int GetEntityID(EntityBase *entityPtr) { return (int)(entityPtr - objectEntityList < ENTITY_COUNT ? entityPtr - objectEntityList : 0); }

int GetEntityCount(ushort type, bool32 isActive);

void ResetEntityPtr(Entity *entity, ushort type, void *data);
void ResetEntitySlot(ushort slotID, ushort type, void *data);
void CreateEntity(ushort type, void *data, int x, int y);

inline void CopyEntity(void *destEntity, void *srcEntity, bool32 clearSrcEntity)
{
    if (destEntity && srcEntity) {
        memcpy(destEntity, srcEntity, sizeof(EntityBase));
        if (clearSrcEntity)
            memset(srcEntity, 0, sizeof(EntityBase));
    }
}

bool32 GetActiveEntities(ushort group, Entity **entity);
bool32 GetEntities(ushort type, Entity **entity);

inline void BreakForeachLoop() { --foreachStackPtr; }

bool32 CheckOnScreen(Entity *entity, Vector2 *range);
bool32 CheckPosOnScreen(Vector2 *position, Vector2 *range);

#endif // !OBJECT_H
