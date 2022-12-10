uniform sampler2D texture;
//uniform float blink_alpha;
uniform float u_time;

// https://iquilezles.org/articles/functions/
float expImpulse( float x, float k )
{
    float h = k*x;
    return h*exp(1.0-h);
}

void main()
{
    float val = u_time;
    float mod = 1.0;
    while(val > mod){ val = val - mod; }

    float blink_alpha = expImpulse(val,4.0);
    //float blink_alpha = abs(0.8 * sin(0.5 * u_time));
    //float blink_alpha = 0.8;

    vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);
    pixel.r = max(pixel.r, blink_alpha);
    pixel.g = max(pixel.g, blink_alpha);
    pixel.b = max(pixel.b, blink_alpha);
    gl_FragColor = pixel;
}
