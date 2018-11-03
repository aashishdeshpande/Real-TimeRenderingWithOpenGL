#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void getSphereVertexData(float spherePositionCoords[], float sphereNormalCoords[], float sphereTexCoords[], short sphereElements[]);

void processSphereData();

void addTriangle(float[][] single_vertex, float[][] single_normal, float[][] single_texture);

void normalizeVector(float[] v);

bool isFoundIdentical(const float val1, const float val2, const float diff);

int getNumberOfSphereVertices();

int getNumberOfSphereElements();


