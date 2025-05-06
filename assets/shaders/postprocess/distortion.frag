#version 330 core

uniform sampler2D tex;
uniform float time;                 
uniform float intensity;                 
uniform vec2 screenSize;                 

in vec2 texCoords;
out vec4 fragColor;

void main() {
    // Sample original color
    vec3 color = texture(tex, texCoords).rgb;
    
    // Create blood-red vignette effect
    vec2 uv = texCoords * 2.0 - 1.0;  // Convert to [-1,1] range
    float vignette = 1.0 - dot(uv, uv) * 0.5;  // Circular falloff
    
    // Pulsing effect based on time
    float pulse = sin(time * 10.0) * 0.1 + 0.9;
    
    // Blood color with vignette
    vec3 damageColor = mix(
        vec3(0.7, 0.1, 0.1),  // Dark red
        vec3(1.0, 0.3, 0.3),  // Bright red
        pulse * vignette
    );
    
    // Edge detection for extra effect (optional)
    float edge = 0.0;
    vec2 pixelSize = 1.0 / screenSize;
    for (int x = -1; x <= 1; x++) {
        for (int y = -1; y <= 1; y++) {
            if (x == 0 && y == 0) continue;
            vec2 offset = vec2(x, y) * pixelSize * 2.0;
            edge += length(color - texture(tex, texCoords + offset).rgb);
        }
    }
    edge = clamp(edge * 0.5, 0.0, 0.3);
    
    // Combine effects
    fragColor = vec4(
        mix(
            color,
            mix(damageColor, vec3(1.0, 0.5, 0.5), edge),
            intensity  // Controls overall strength
        ),
        1.0
    );
}