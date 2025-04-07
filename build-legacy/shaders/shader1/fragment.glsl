precision mediump float;

varying vec2 pos;

const int c_num = 100; // Match the numCircles constant in C++

uniform vec3 circles[c_num];

void main() {
    float color = 1.0;
    for (int i = 0; i < c_num; i++) {
        float d = length(pos - circles[i].xy) - circles[i].z;
        d = step(0.0, d);
        color *= d;
    }

    gl_FragColor = vec4(color, color, color, 1.0);
}