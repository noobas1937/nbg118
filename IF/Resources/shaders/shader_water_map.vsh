#define SIMPLE

#ifdef GL_ES
#else
#define highp
#define mediump
#define lowp
#endif
//uniform   mat4 u_mvp;

attribute vec4 a_position;
attribute vec2 a_texCoord;

varying vec2 v_texcoord0;
varying vec3 v_position;
varying float v_result;
varying highp vec2 v_normalCoord1;
varying highp vec2 v_normalCoord2;
varying vec3 v_lightVec;
varying vec4 v_waterLightSea;
varying vec4 v_waterLightShore;
varying vec4 v_waterDarkSea;
varying vec4 v_waterDarkShore;

varying float v_reflectionPower; //a by ljf

uniform highp float u_time;

void main()
{
	gl_Position = CC_MVPMatrix * a_position;
	v_texcoord0 = a_texCoord;
	v_position = a_position.xyz;
	
	float _Anim = u_time;
	float _ResX = 0.02;
	float _ResY = 0.04;
	float x = v_texcoord0.x;
	float y = v_texcoord0.y;
	float mov0 = x+y+cos(sin(_Anim)*2.)*100.+sin(x/100.)*1000.;
	float mov1 = y / _ResY * 5.0 + _Anim;
	float mov2 = x / _ResX * 5.0;
	float c1 = abs((sin(mov1+_Anim) +mov2) * 0.5 -mov1-mov2+_Anim);
	float c2 = abs(sin(c1+sin(mov0/1000.+_Anim)+sin(y/40.+_Anim)+sin((x+y)/100.)*3.));
	//float c3 = abs(sin(c2+cos(mov1+mov2+c2)+cos(mov2)+sin(x/1000.)));

	float c4 = 0.5 * sin(sqrt(x * x + y * y) * 150.0 - u_time) + 0.5;
//	float c4 = (sin(sqrt(x * x + y * y) * 150.0 - u_time) + 1.0) * 0.5;

	float c5 = 0.5 * sin((x + cos(y + u_time * 0.01) + u_time * 0.01) * 100.0) + 0.5;
//	float c5 = (sin((x + cos(y + u_time * 0.01) + u_time * 0.01) * 100.0) + 1.0) * 0.5;

	c1 = 0.5 * sin(c1) + 0.5;
//	c1 = (sin(c1) + 1.0) * 0.5;
	//begin d by ljf
	//v_result = c4;//(c1 + c4 + c5) * 0.333;
	//end d by ljf
	//begin a by ljf
	//v_result = 0.5 * sin(x * 50.0 * 3.1416 - u_time) + 0.5;
	//v_result = 0.5 * sin(x * 100.0 * 3.1416 - u_time) + 0.5;
	v_result = 0.5 * sin(x * 100.0 * 3.1416 - u_time) + 0.5;
	//end a by ljf
	//begin d by ljf
	//v_normalCoord1 = v_texcoord0 * 25.0;
	
	//v_normalCoord1.x -= 0.01 * u_time * 0.5;
	//v_normalCoord1.y += 0.02 * u_time * 0.5;
	//end d by ljf
	v_normalCoord2 = v_texcoord0 * 20.0;
	v_normalCoord2.x += 0.015 * u_time * 0.4;
	v_normalCoord2.y += 0.02 * u_time * 0.6;

	//begin d by ljf
	//v_normalCoord1 = vec2(v_normalCoord1.x + c1 * 0.01, v_normalCoord1.y + c1 * 0.01) * 1.5; 
	//end d by ljf
	v_normalCoord2 = vec2(v_normalCoord2.x + c2 * 0.003, v_normalCoord2.y + c2 * 0.003) * 1.5;
    //begin a by ljf
    v_normalCoord1 = v_texcoord0 * 25.0;
    v_normalCoord1.x -= 0.01 * u_time * 0.5;
	v_normalCoord1.y += 0.02 * u_time * 0.5;
	v_normalCoord1 = vec2(v_normalCoord1.x + c1 * 0.01, v_normalCoord1.y + c1 * 0.01) * 1.0;
    //end a by ljf
    v_lightVec = vec3(v_texcoord0.x * 3.0 + (v_position.x + 1.0), v_texcoord0.y, 0.0);
    v_lightVec = normalize(vec3(2.5, -1.0, 1.8) - v_lightVec);
	
	float v_dimmingFactor = 1.35 - sqrt((v_texcoord0.x - 0.5) * (v_texcoord0.x - 0.5) + (v_texcoord0.y - 0.5) * (v_texcoord0.y - 0.5));
	v_dimmingFactor *= v_dimmingFactor;
	v_dimmingFactor = min(v_dimmingFactor, 1.0);
    
    //v_waterLightSea = vec4(float(0x2A) / 255.0, float(0x74) / 255.0, float(0xB8) / 255.0, 1.0) * 1.3 * v_dimmingFactor; //d by ljf
    v_waterLightSea = vec4(float(0) / 255.0, float(116) / 255.0, float(145) / 255.0, 1.0) * 1.3 * v_dimmingFactor; //a by ljf
    //v_waterLightShore = vec4(float(0xC0) / 255.0, float(0xF4) / 255.0, float(0xE3) / 255.0, 1.0) * v_dimmingFactor; //d by ljf
     v_waterLightShore = vec4(float(26) / 255.0, float(150) / 255.0, float(196) / 255.0, 1.0) ; //a by ljf

    //v_waterDarkSea = vec4(float(0x1D) / 255.0, float(0x36) / 255.0, float(0x5B) / 255.0, 1.0) * 1.3 * v_dimmingFactor; //d by ljf
    v_waterDarkSea = vec4(float(0) / 255.0, float(17) / 255.0, float(23) / 255.0, 1.0) * 1.3 * v_dimmingFactor; //a by ljf
    //v_waterDarkShore = vec4(float(0x0B) / 255.0, float(0xAC) / 255.0, float(0x88) / 255.0, 1.0) * v_dimmingFactor; //d by ljf
    v_waterDarkShore = vec4(float(0) / 255.0, float(90) / 255.0, float(177) / 255.0, 1.0) ; //a by ljf

	#ifdef SIMPLE
	float temppi = 0.5;
	v_waterDarkSea = mix(v_waterDarkSea, v_waterDarkShore, temppi);
	v_waterLightSea = mix(v_waterLightSea, v_waterLightShore, temppi);
	#endif

	//begin a by ljf
    float ystretch = 0.2;//0.4
    v_reflectionPower = clamp((1.0 - length(vec2(v_position.x * 0.7 + (v_texcoord0.x - 0.5) * 1.5, v_position.y * ystretch) - vec2(0.0, ystretch))) * 3.0, 1.3, 1.6);
    //end a by ljf
}
