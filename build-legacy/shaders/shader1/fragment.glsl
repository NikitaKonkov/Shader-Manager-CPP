#version 300 es

precision mediump float;

in vec2 pos;
out vec4 colour;

uniform sampler2D image;
uniform vec2 aspect;

uniform vec2 centre;
uniform float t;

const float maxRadius = 0.5;

// SDF from https://iquilezles.org/articles/distfunctions2d/
float sdBox(vec2 p, vec2 b) {
    vec2 d = abs(p)-b;
    return length(max(d,0.0)) + min(max(d.x,d.y),0.0);
}

float getOffsetStrength(float t, vec2 dir) {
  float d = length(dir/aspect) - t * maxRadius; // SDF of circle
  // Doesn't have to be a circle!!
  // float d = sdBox(dir/aspect, vec2(t * maxRadius));
  
  d *= 1. - smoothstep(0., 0.05, abs(d)); // Mask the ripple
  
  d *= smoothstep(0., 0.05, t); // Smooth intro
  d *= 1. - smoothstep(0.5, 1., t); // Smooth outro
  return d;
}

void main() {
  // 0. Normal sampling
  // colour = texture(image, pos);
  
  // 1. Simple distortion
  // float x = sin(pos.y * 12.56) * 0.02 * smoothstep(pos.y - 0.3, pos.y, t);
  // float y = sin(pos.x * 12.56) * 0.02;
  // vec2 offset = vec2(x, y);
  // colour = texture(image, pos + offset);
  
  // 2. Shockwave
  // vec2 centre = vec2(0.5);
  // vec2 dir = centre - pos;
  // float d = length(dir/aspect) - t * maxRadius;//0.2; // SDF of circle
  // d *= 1. - smoothstep(0., 0.05, abs(d)); // Smooth the ripple
  // dir = normalize(dir);
  // colour = texture(image, pos + dir * d);
  

  // 3. Animation
  // vec2 dir = centre - pos;
  // float d = getOffsetStrength(t, dir);
  // dir = normalize(dir);
  // colour = texture(image, pos + dir * d);
  
  // 4. Chromatic aberation
  vec2 dir = centre - pos;
  float tOffset = 0.05 * sin(t * 3.14);
  float rD = getOffsetStrength(t + tOffset, dir);
  float gD = getOffsetStrength(t, dir);
  float bD = getOffsetStrength(t - tOffset, dir);
  
  dir = normalize(dir);
  
  float r = texture(image, pos + dir * rD).r;
  float g = texture(image, pos + dir * gD).g;
  float b = texture(image, pos + dir * bD).b;
  
  float shading = gD * 8.;
  
  colour = vec4(r, g, b, 1.);
  colour.rgb += shading;
  
}






  
  