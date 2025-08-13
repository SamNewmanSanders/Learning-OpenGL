#pragma once

// 24 unique vertices (positions + normals), 4 per face
inline float cubeVertices[] = {
    // positions          // normals
    // back face (0, 0, -1)
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  // 0
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  // 1
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  // 2
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  // 3

    // front face (0, 0, 1)
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  // 4
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  // 5
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  // 6
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  // 7

    // left face (-1, 0, 0)
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  // 8
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  // 9
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  // 10
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  // 11

    // right face (1, 0, 0)
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  // 12
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  // 13
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  // 14
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  // 15

    // bottom face (0, -1, 0)
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  // 16
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  // 17
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  // 18
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  // 19

    // top face (0, 1, 0)
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  // 20
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  // 21
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  // 22
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f   // 23
};

// Indices for drawing the cube using triangles (6 faces × 2 triangles × 3 indices)
inline unsigned int cubeIndices[] = {
    0, 1, 2,  2, 3, 0,        // back face
    4, 5, 6,  6, 7, 4,        // front face
    8, 9, 10, 10, 11, 8,      // left face
    12, 13, 14, 14, 15, 12,   // right face
    16, 17, 18, 18, 19, 16,   // bottom face
    20, 21, 22, 22, 23, 20    // top face
};
