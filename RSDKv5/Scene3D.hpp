#ifndef SCENE3D_H
#define SCENE3D_H

#define SCENE3D_MAX (0x20)
#define MODEL_MAX   (0x100)

struct Matrix {
    int values[4][4];
};

struct ModelVertex {
    float x;
    float y;
    float z;
    float nx;
    float ny;
    float nz;
};

struct TexCoord {
    float x;
    float y;
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


struct Scene3D {
    uint hash[4];
    Scene3DVertex *vertices;
    int field_14;
    int field_18;
    byte *faceVertCounts;
    int unknown1;
    int unknown2;
    int unknown3;
    int unknown4;
    int unknown5;
    int unknown6;
    int unknown7;
    int unknown8;
    int unknown9;
    int unknown10;
    int unknown11;
    ushort field_4C;
    ushort indexCount;
    ushort field_50;
    byte drawMode;
    byte scope;
};

extern Model modelList[MODEL_MAX];
extern Scene3D scene3DList[SCENE3D_MAX];

void setIdentityMatrix(Matrix *matrix);
void matrixMultiply(Matrix *dest, Matrix *matrixA, Matrix *matrixB);
void matrixTranslateXYZ(Matrix *Matrix, int x, int y, int z, bool32 setIdentity);
void matrixScaleXYZ(Matrix *matrix, int scaleX, int scaleY, int scaleZ);
void matrixRotateX(Matrix *matrix, short angle);
void matrixRotateY(Matrix *matrix, short angle);
void matrixRotateZ(Matrix *matrix, short angle);
void matrixRotateXYZ(Matrix *matrix, short rotationX, short rotationY, short rotationZ);
void matrixInverse(Matrix *dest, Matrix *matrix);
void matrixCopy(Matrix *matDst, Matrix* matSrc);

ushort LoadMesh(const char *filepath, Scopes scope);
ushort Create3DScene(const char *name, ushort faceCnt, Scopes scope);
void Init3DScene(ushort sceneID);
//void View_Something1;
//void View_Something2;
//void View_Something3;
void SetupMesh(ushort animID, ushort sceneID, byte drawMode, Matrix *matWorld, Matrix *matView, uint colour);
void SetupMeshAnimation(ushort animID, ushort sceneID, EntityAnimationData *data, byte drawMode, Matrix *matWorld, Matrix *matView, uint colour);
void Draw3DScene(ushort sceneID);

#endif
