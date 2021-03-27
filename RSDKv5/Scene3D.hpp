#ifndef SCENE3D_H
#define SCENE3D_H

#define SCENE3D_MAX (0x20)
#define MODEL_MAX   (0x100)
#define SCENE3D_VERT_MAX (0x4000)

enum Scene3DDrawTypes {
    S3D_FLATCLR_WIREFRAME               = 0x0,
    S3D_FLATCLR                         = 0x1,
    S3D_UNKNOWN_2                       = 0x2,
    S3D_UNKNOWN_3                       = 0x3,
    S3D_FLATCLR_SHADED_WIREFRAME        = 0x4,
    S3D_FLATCLR_SHADED                  = 0x5,
    S3D_FLATCLR_SHADED_BLENDED          = 0x6,
    S3D_FLATCLR_SCREEN_WIREFRAME        = 0x7,
    S3D_FLATCLR_SCREEN                  = 0x8,
    S3D_FLATCLR_SHADED_SCREEN_WIREFRAME = 0x9,
    S3D_FLATCLR_SHADED_SCREEN           = 0xA,
    S3D_FLATCLR_SHADED_BLENDED_SCREEN   = 0xB,
};

struct ScanEdge {
    int start;
    int end;
    int start1;
    int end1;
    int start2;
    int end2;
    int start3;
    int end3;
};

struct Matrix {
    int values[4][4];
};

struct ModelVertex {
    int x;
    int y;
    int z;
    int nx;
    int ny;
    int nz;
};

struct TexCoord {
    int x;
    int y;
};

struct Model {
    uint hash[4];
    ModelVertex *vertices;
    TexCoord *texCoords;
    Colour *colours;
    ushort *indices;
    ushort vertCount;
    ushort indexCount;
    ushort frameCount;
    byte flags;
    byte faceVertCount;
    byte scope;
};

struct Scene3DVertex {
    int x;
    int y;
    int z;
    int nx;
    int ny;
    int nz;
    int tx;
    int ty;
    uint colour;
};

struct ZBufferEntry {
    int depth;
    int index;
};

struct Scene3D {
    uint hash[4];
    Scene3DVertex *vertices;
    Scene3DVertex *normals;
    ZBufferEntry *zBuffer;
    byte *faceVertCounts;
    int unknownX;
    int unknownY;
    int ambientX;
    int ambientY;
    int ambientZ;
    int diffuseX;
    int diffuseY;
    int diffuseZ;
    int specularX;
    int specularY;
    int specularZ;
    ushort vertLimit;
    ushort vertexCount;
    ushort faceCount;
    byte drawMode;
    byte scope;
};

extern Model modelList[MODEL_MAX];
extern Scene3D scene3DList[SCENE3D_MAX];

extern ScanEdge scanEdgeBuffer[SCREEN_YSIZE];

void ProcessScanEdge(int x1, int y1, int x2, int y2);
void ProcessScanEdgeClr(uint c1, uint c2, int x1, int y1, int x2, int y2);

void setIdentityMatrix(Matrix *matrix);
void matrixMultiply(Matrix *dest, Matrix *matrixA, Matrix *matrixB);
void matrixTranslateXYZ(Matrix *Matrix, int x, int y, int z, bool32 setIdentity);
void matrixScaleXYZ(Matrix *matrix, int scaleX, int scaleY, int scaleZ);
void matrixRotateX(Matrix *matrix, short angle);
void matrixRotateY(Matrix *matrix, short angle);
void matrixRotateZ(Matrix *matrix, short angle);
void matrixRotateXYZ(Matrix *matrix, short rotationX, short rotationY, short rotationZ);
void matrixInverse(Matrix *dest, Matrix *matrix);
void matrixCopy(Matrix *matDst, Matrix *matSrc);

ushort LoadMesh(const char *filepath, Scopes scope);
ushort Create3DScene(const char *name, ushort faceCnt, Scopes scope);
inline void Init3DScene(ushort sceneID)
{
    if (sceneID < SCENE3D_MAX) {
        Scene3D *scn     = &scene3DList[sceneID];
        scn->vertexCount = 0;
        scn->faceCount   = 0;
        memset(scn->vertices, 0, sizeof(Scene3DVertex) * scn->vertLimit);
        memset(scn->normals, 0, sizeof(Scene3DVertex) * scn->vertLimit);
        memset(scn->faceVertCounts, 0, sizeof(byte) * scn->vertLimit);
        memset(scn->zBuffer, 0, sizeof(ZBufferEntry) * scn->vertLimit);
    }
}

inline void SetModelAnimation(ushort model, AnimationData *data, short animSpeed, byte loopIndex, bool32 forceApply, ushort frameID)
{
    if (model >= MODEL_MAX) {
        if (data)
            data->framePtrs = 0;
        return;
    }
    if (!data)
        return;

    if (data->animationID == model && !forceApply)
        return;
    data->framePtrs       = (SpriteFrame *)1;
    data->animationTimer  = 0;
    data->frameID         = frameID;
    data->frameCount      = modelList[model].frameCount;
    data->animationSpeed  = animSpeed;
    data->prevAnimationID = data->animationID;
    data->frameDelay      = 0x100;
    data->loopIndex       = loopIndex;
    data->animationID     = model;
}
inline void SetAmbientUnknown(ushort sceneID, byte x, byte y, byte z)
{
    if (sceneID < SCENE3D_MAX) {
        Scene3D *scn  = &scene3DList[sceneID];
        scn->ambientX = x;
        scn->ambientY = y;
        scn->ambientZ = z;
    }
}
inline void SetDiffuseUnknown(ushort sceneID, byte x, byte y, byte z)
{
    if (sceneID < SCENE3D_MAX) {
        Scene3D *scn  = &scene3DList[sceneID];
        scn->diffuseX = x;
        scn->diffuseY = y;
        scn->diffuseZ = z;
    }
}
inline void SetSpecularUnknown(ushort sceneID, byte x, byte y, byte z)
{
    if (sceneID < SCENE3D_MAX) {
        Scene3D *scn   = &scene3DList[sceneID];
        scn->specularX  = x;
        scn->specularY = y;
        scn->specularZ = z;
    }
}
void SetupMesh(ushort animID, ushort sceneID, byte drawMode, Matrix *matWorld, Matrix *matView, uint colour);
void SetupMeshAnimation(ushort animID, ushort sceneID, AnimationData *data, byte drawMode, Matrix *matWorld, Matrix *matView, uint colour);
void Draw3DScene(ushort sceneID);

#endif
