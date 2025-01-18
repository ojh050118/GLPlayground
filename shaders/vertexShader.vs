#version 330 core

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vCol;
out vec3 vPosition;
out vec3 vColor;

uniform mat4 model;
uniform mat4 projection;

void main() {
    gl_Position = projection * model * vec4(vPos.x, vPos.y, vPos.z, 1.0);
    vPosition = vPos;
    vColor = vCol;
    vColor = vec3(clamp(vPos, 0.0f, 1.0f));
}