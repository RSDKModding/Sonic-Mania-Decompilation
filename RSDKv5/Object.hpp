#ifndef OBJECT_H
#define OBJECT_H

#define OBJECT_COUNT (0x400)
#define ENTITY_COUNT (0x940)
#define EDITABLEVAR_COUNT (0x100)

enum AttributeTypes {
	ATTRIBUTE_U8,
	ATTRIBUTE_U16,
	ATTRIBUTE_U32,
	ATTRIBUTE_S8,
	ATTRIBUTE_S16,
	ATTRIBUTE_S32,
	ATTRIBUTE_VAR,
	ATTRIBUTE_BOOL,
	ATTRIBUTE_STRING,
	ATTRIBUTE_VECTOR2,
	ATTRIBUTE_UNKNOWN,
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
    int onGround;
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
    void(*create)(void *);
    void (*stageLoad)(void);
    void (*editorDraw)(void);
    void (*editorLoad)(void);
    void (*serialize)(void);
    Object *type;
    int entitySize;
    int objectSize;
};        

struct EditableVarInfo {
    uint hash[4];
    int offset;
    int active;
    byte attribType;
};

extern int objectCount;
extern ObjectInfo objectList[OBJECT_COUNT];
extern int globalObjectCount;
extern int globalObjectIDs[OBJECT_COUNT];
extern int stageObjectCount;
extern int stageObjectIDs[OBJECT_COUNT];

extern EntityBase objectEntityList[ENTITY_COUNT];

extern int tempEntityID;

extern EditableVarInfo editableVarList[EDITABLEVAR_COUNT];
extern int editableVarCount;

void CreateObject(Object *structPtr, const char *name, uint entitySize, uint objectSize, void (*update)(void), void (*lateUpdate)(void),
                  void (*staticUpdate)(void), void (*draw)(void), void(__cdecl *create)(void *), void (*stageLoad)(void), void (*editorDraw)(void),
                  void (*editorLoad)(void), void (*serialize)(void));
void CreateObjectContainer(Object *structPtr, const char *name, uint objectSize);

void LoadStaticObject(byte *obj, uint *hash, int dataPos);

inline void SetEditableVar(byte type, const char *name, byte object, int storeOffset)
{
    if (editableVarCount < 255) {
        EditableVarInfo *editableVar = &editableVarList[editableVarCount];
        memset(&hashBuffer, 0, 0x400u);
        int len = StrLength(name);
        memcpy(&hashBuffer, name, len);
        GenerateHash(editableVar->hash, len);
        editableVarList[editableVarCount].attribType = type;
        editableVarList[editableVarCount].offset     = storeOffset;
        editableVarList[editableVarCount].active     = 1;
        editableVarCount++;
    }
}

void InitObjects();
void ProcessObjects();
void ProcessPausedObjects();
void ProcessFrozenObjects();
void ProcessObjectDrawLists();

inline ushort GetObjectByName(const char *name)
{
    StrCopy(hashBuffer, name);
    uint hash[4];
    GenerateHash(hash, StrLength(hashBuffer));

    for (int o = 0; o < stageObjectCount; ++o) {
        if (memcmp(hash, objectList[stageObjectIDs[o]].hash, 4 * sizeof(int)) == 0)
            return o;
    }
    return 0xFFFF;
}

inline Entity* GetObjectByID(ushort objectID)
{
    return &objectEntityList[objectID < ENTITY_COUNT ? objectID : (ENTITY_COUNT - 1)];
}

inline int GetEntityID(EntityBase *entityPtr) { return entityPtr - objectEntityList < ENTITY_COUNT ? entityPtr - objectEntityList : 0; }

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
#endif // !OBJECT_H