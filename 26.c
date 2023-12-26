#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

#define WIDTH 800
#define HEIGHT 600

typedef struct {
    float x, y, z;
} Vertex;

typedef struct {
    float depth;
} ZBufferPixel;

ZBufferPixel zBuffer[WIDTH][HEIGHT];

void initZBuffer() {
    for (int i = 0; i < WIDTH; i++) {
        for (int j = 0; j < HEIGHT; j++) {
            zBuffer[i][j].depth = INT_MAX;
        }
    }
}

void updateZBuffer(int x, int y, float depth) {
    if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
        if (depth < zBuffer[x][y].depth) {
            zBuffer[x][y].depth = depth;
        }
    }
}

void renderTriangle(Vertex v1, Vertex v2, Vertex v3) {
    // Sorting vertices by y-coordinate
    if (v1.y > v2.y) {
        Vertex temp = v1;
        v1 = v2;
        v2 = temp;
    }
    if (v2.y > v3.y) {
        Vertex temp = v2;
        v2 = v3;
        v3 = temp;
    }
    if (v1.y > v2.y) {
