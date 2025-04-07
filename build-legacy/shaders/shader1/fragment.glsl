#version 300 es

precision mediump float;

in vec2 pos;
out vec4 colour;

uniform sampler2D image;
uniform vec2 aspect;

uniform vec2 centre;
uniform float t;

// New uniforms for mouse interaction
uniform vec2 clickPos;
uniform float clickTime;
uniform float currentTime;

const float maxRadius = 0.5;
const float waveDuration = 1.0; // Duration of the wave animation in seconds

// SDF from https://iquilezles.org/articles/distfunctions2d/
float sdBox(vec2 p, vec2 b) {
    vec2 d = abs(p)-b;
    return length(max(d,0.0)) + min(max(d.x,d.y),0.0);
}

float getOffsetStrength(float waveTime, vec2 dir) {
    float d = length(dir/aspect) - waveTime * maxRadius; // SDF of circle
    // Doesn't have to be a circle!!
    // float d = sdBox(dir/aspect, vec2(waveTime * maxRadius));
    
    d *= 1.0 - smoothstep(0.0, 0.05, abs(d)); // Mask the ripple
    
    d *= smoothstep(0.0, 0.05, waveTime); // Smooth intro
    d *= 1.0 - smoothstep(0.5, 1.0, waveTime); // Smooth outro
    return d;
}

void main() {
    // Calculate time since last click
    float timeSinceClick = currentTime - clickTime;
    
    // Normalize to 0-1 range over the wave duration
    float waveTime = clamp(timeSinceClick / waveDuration, 0.0, 1.0);
    
    // Use click position if available, otherwise use center
    vec2 waveCenter = (clickTime > 0.0) ? clickPos : centre;
    
    // 4. Chromatic aberration from click position
    vec2 dir = waveCenter - pos;
    
    // Skip effect if direction is too small (avoid division by zero)
    if (length(dir) < 0.001) {
        colour = texture(image, pos);
        return;
    }
    
    float tOffset = 0.05 * sin(waveTime * 3.14);
    float rD = getOffsetStrength(waveTime + tOffset, dir);
    float gD = getOffsetStrength(waveTime, dir);
    float bD = getOffsetStrength(waveTime - tOffset, dir);
    
    dir = normalize(dir);
    
    float r = texture(image, pos + dir * rD).r;
    float g = texture(image, pos + dir * gD).g;
    float b = texture(image, pos + dir * bD).b;
    
    float shading = gD * 8.0;
    
    colour = vec4(r, g, b, 1.0);
    colour.rgb = clamp(colour.rgb + vec3(shading), 0.0, 1.0);
}