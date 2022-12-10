uniform sampler2D texture;
//uniform float pixel_threshold;
uniform float u_time;

void main()
{
    float pixel_threshold = abs(0.1 * sin(u_time));

    float factor = 1.0 / (pixel_threshold + 0.001);
    vec2 pos = floor(gl_TexCoord[0].xy * factor + 0.5) / factor;
    gl_FragColor = texture2D(texture, pos) * gl_Color;
}
