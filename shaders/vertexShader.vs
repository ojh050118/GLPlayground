#version 330 core

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vCol;
out vec3 vPosition;
out vec3 vColor;

uniform mat4 model;

void main() {
    gl_Position = model * vec4(vPos.x, vPos.y, vPos.z, 1.0);
    vPosition = vPos;
    vColor = vCol;
}