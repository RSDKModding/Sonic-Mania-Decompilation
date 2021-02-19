#ifndef OBJECT_H
#define OBJECT_H

#define OBJECT_COUNT (0x400)

//0x800 scene objects, 0x40 reserved ones, and 0x100 spare slots for creation
#define RESERVE_ENTITY_COUNT (0x40)
#define ENTITY_COUNT         (0x940)
#define TEMPENTITY_START     (ENTITY_COUNT - 0x100)
#define SCENEENTITY_COUNT    (TEMPENTITY_START - RESERVE_ENTITY_COUNT)

#define TYPE_COUNT (0x100)
#define EDITABLEVAR_COUNT (0x100)
#define TYPEGROUP_COUNT (0x104)

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
	ACTIVE_NEVER,
	ACTIVE_ALWAYS,
	ACTIVE_NORMAL,
	ACTIVE_PAUSED,
	ACTIVE_BOUNDS,
	ACTIVE_XBOUNDS,
	ACTIVE_YBOUNDS,
    ACTIVE_RBOUNDS,
};

enum DefaultObjects {
    TYPE_DEFAULTOBJECT = 0,
    TYPE_DEVOUTPUT     = 1,
};

struct Object {
    short objectID;
    byte active;
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
    int groundVel;
    int depth;
    ushort group;
    ushort objectID;
    bool32 inBounds;
    int field_3C;
    bool32 tileCollisions;
    bool32 interaction;
    bool32 onGround;
    byte active;
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

extern ForeachStackInfo foreachStackList[0x20];
extern ForeachStackInfo *foreachStackPtr;

extern TypeGroupList typeGroups[TYPEGROUP_COUNT];

extern bool32 validDraw;

void CreateObject(Object **structPtr, const char *name, uint entitySize, uint objectSize, void (*update)(void), void (*lateUpdate)(void),
                  void (*staticUpdate)(void), void (*draw)(void), void(__cdecl *create)(void *), void (*stageLoad)(void), void (*editorDraw)(void),
                  void (*editorLoad)(void), void (*serialize)(void));
void CreateObjectContainer(Object **structPtr, const char *name, uint objectSize);

void LoadStaticObject(byte *obj, uint *hash, int dataPos);

#define RSDK_EDITABLE_VAR(object, type, var) RSDK.SetEditableVar(type, #var, object->objectID, offsetof(Entity##object, var))

inline void SetEditableVar(byte type, const char *name, byte object, int storeOffset)
{
    if (editableVarCount < 255) {
        EditableVarInfo *editableVar = &editableVarList[editableVarCount];
        StrCopy(hashBuffer, name);
        GenerateHash(editableVar->hash, StrLength(name));
        editableVarList[editableVarCount].type   = type;
        editableVarList[editableVarCount].offset = storeOffset;
        editableVarList[editableVarCount].active = true;
        editableVarCount++;
    }
}

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

void DestroyEntity(Entity *entity, ushort type, void *data);
void ResetEntity(ushort slotID, ushort type, void *data);
void SpawnEntity(ushort type, void *data, int x, int y);

inline void CopyEntity(void *destEntity, void *srcEntity, bool32 clearSrcEntity)
{
    if (destEntity && srcEntity) {
        memcpy(destEntity, srcEntity, sizeof(EntityBase));
        if (clearSrcEntity)
            memset(srcEntity, 0, sizeof(EntityBase));
    }
}

bool32 GetActiveObjects(ushort group, Entity **entity);
bool32 GetObjects(ushort type, Entity **entity);

inline void NextForeachLoop() { --foreachStackPtr; }

bool32 CheckOnScreen(Entity *entity, Vector2 *range);
bool32 CheckPosOnScreen(Vector2 *position, Vector2 *range);

#endif // !OBJECT_H
