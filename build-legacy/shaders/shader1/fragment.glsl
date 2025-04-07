precision mediump float;

varying vec2 pos;

uniform vec3 colors[2];

void main() {
    gl_FragColor = vec4(colors[0], 1.);
}