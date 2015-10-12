/*
uniform sampler2D u_texture1;
uniform sampler2D u_lightTexture;   
uniform vec4 v_LightColor;
uniform vec2 v_animLight;     
varying vec2 v_texCoord;

void main(void) 
{
    vec4 lightcolor = texture2D(u_lightTexture, v_texCoord + v_animLight.xy) * v_LightColor;
	gl_FragColor = texture2D(u_texture1, v_texCoord) + lightcolor;
}
*/

#ifdef GL_ES                                
precision lowp float;                       
#endif                                      
                              
uniform vec2 texOffset;                 
uniform sampler2D u_lightTexture;   
uniform vec4 v_LightColor;
// uniform vec2 v_animLight;    

varying vec2 v_texCoord;     
varying vec4 v_fragmentColor;

void main()                                 
{                                           
    vec2 texcoord = mod(texOffset + v_texCoord, 1.0);  
    vec4 lightcolor = texture2D(u_lightTexture, texcoord) * v_LightColor;
    gl_FragColor = v_fragmentColor + lightcolor;            
}
