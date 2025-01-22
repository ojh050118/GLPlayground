#version 330 core

out vec4 fragColor;
in vec3 vPosition;
in vec3 vColor;
in vec2 vTextureCoord;


uniform float time;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main() {
    //fragColor = vec4(vColor.x * abs(tan(time)), vColor.y * abs(tan(time * 2)), vColor.z * abs(tan(time * 3)), 1.0f);
    //fragColor = vec4(vColor, 1.0f);
    fragColor = mix(texture(texture1, vTextureCoord), texture(texture2, vTextureCoord), 0.25);
}