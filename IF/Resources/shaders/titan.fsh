#ifdef GL_ES
varying mediump vec2 v_texture_coord;
#else
varying vec2 v_texture_coord;
#endif

uniform vec4 u_color;
uniform float duration;
uniform sampler2D caustics;
uniform vec4 caustics_color;
void main(void)
{
    vec4 color = texture2D(caustics, v_texture_coord) * caustics_color * duration;
    gl_FragColor = u_color * texture2D(CC_Texture0, v_texture_coord) * (1.0 + color);
}