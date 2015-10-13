attribute vec4 a_position;
attribute vec2 a_texCoord;

// Varyings
varying vec2 v_texCoord;

void main()
{
    gl_Position = CC_MVPMatrix * a_position;
    v_texCoord = a_texCoord;
    v_texCoord.y = (1.0 - v_texCoord.y);
}
