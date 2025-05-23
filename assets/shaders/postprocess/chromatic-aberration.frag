#version 330

// The texture holding the scene pixels
uniform sampler2D tex;

// Read "assets/shaders/fullscreen.vert" to know what "tex_coord" holds;
in vec2 tex_coord;
out vec4 frag_color;

// How far (in the texture space) is the distance (on the x-axis) between
// the pixels from which the red/green (or green/blue) channels are sampled
#define STRENGTH 0.005

// Chromatic aberration mimics some old cameras where the lens disperses light
// differently based on its wavelength. In this shader, we will implement a
// cheap version of that effect 

void main(){
    //TODO: Modify this shader to apply chromatic abberation
    // To apply this effect, we only read the green channel from the correct pixel (as defined by tex_coord)
    // To get the red channel, we move by amount STRENGTH to the left then sample another pixel from which we take the red channel
    // To get the blue channel, we move by amount STRENGTH to the right then sample another pixel from which we take the blue channel
    float r,b;
    vec4 g_a;
    vec2 left=vec2(-STRENGTH,0),right=vec2(STRENGTH,0);

    r=texture(tex, tex_coord+left).r;
    g_a=texture(tex, tex_coord);
    b=texture(tex, tex_coord+right).b;
    frag_color=vec4(r,g_a.g,b,g_a.a);
}