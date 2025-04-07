#version 300 es

in vec3 aPosition;
in vec2 aTexCoord;

out vec2 pos;

void main() {
    pos = aTexCoord;
    vec4 position = vec4(aPosition, 1);
    gl_Position = position;
}


