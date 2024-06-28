#version 330 core

out vec4 fragColor;
in vec3 vColor;

uniform float time;

void main() {
    fragColor = vec4(vColor.x * abs(tan(time)), vColor.y * abs(tan(time * 2)), vColor.z * abs(tan(time * 3)), 1.0f);
}