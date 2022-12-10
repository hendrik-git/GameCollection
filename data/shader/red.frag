//#version 330 core
#ifdef GL_ES
precision mediump float;
#endif

uniform float u_time;
uniform sampler2D texture;

void main() 
{
  // lookup the pixel in the texture
  vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);
  pixel.r = max(1 - abs(sin(u_time)), pixel.r);
  pixel.g = min(abs(sin(u_time)), pixel.g);
  pixel.b = min(abs(sin(u_time)), pixel.b);
  gl_FragColor = pixel;
}