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
        Vertex temp = v1;
        v1 = v2;
        v2 = temp;
    }

    float invSlope1 = (v2.x - v1.x) / (v2.y - v1.y);
    float invSlope2 = (v3.x - v1.x) / (v3.y - v1.y);

    float curX1 = v1.x;
    float curX2 = v1.x;

    for (int scanlineY = v1.y; scanlineY <= v3.y; scanlineY++) {
        for (int x = (int)curX1; x <= (int)curX2; x++) {
            float t = (x - curX1) / (curX2 - curX1);
            float depth = v1.z * (1.0 - t) + v2.z * t;
            updateZBuffer(x, scanlineY, depth);
        }
        curX1 += invSlope1;
        curX2 += invSlope2;
    }
}

void printZBufferToFile(const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        printf("Error opening file for writing.\n");
        return;
    }

    for (int j = HEIGHT - 1; j >= 0; j--) {
        for (int i = 0; i < WIDTH; i++) {
            float normalizedDepth = 1.0 - (zBuffer[i][j].depth - INT_MAX) / (INT_MAX);
            fprintf(file, "%.2f ", normalizedDepth);
        }
        fprintf(file, "\n");
    }

    fclose(file);
}

int main() {
    initZBuffer();

    Vertex v1 = {200, 100, 0.5};
    Vertex v2 = {400, 500, 0.8};
    Vertex v3 = {600, 200, 0.6};

    renderTriangle(v1, v2, v3);

    printZBufferToFile("zbuffer_output.txt");

    return 0;
}

