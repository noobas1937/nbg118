#ifdef GL_ES
precision mediump float;
#else
#define highp
#define mediump
#define lowp
#endif

//#define SIMPLE

// All of these are in SIMPLE too
uniform lowp sampler2D normal;
varying lowp float v_result;
varying highp vec2 v_normalCoord;
#ifdef SIMPLE
varying lowp vec3 v_waterLightSea;
varying lowp vec3 v_waterDarkSea;
#endif

#ifndef SIMPLE
// These are used in medium/high version of the shader
uniform lowp sampler2D texture0;
varying highp vec2 v_texcoord0;
#endif

varying float v_reflectionPower;

void main()
{
#ifdef SIMPLE
    lowp vec4 normalMapValue = texture2D(normal, v_normalCoord);
    //gl_FragColor.rgb = mix(v_waterLightSea, v_waterDarkSea, (normalMapValue.x * v_result) + (normalMapValue.y * (1.0 - v_result)));
	//gl_FragColor.a = 1.0;
    gl_FragColor.rgb = mix(v_waterLightSea, v_waterDarkSea, (normalMapValue.x * v_result) + (normalMapValue.y * (1.0 - v_result)))
        + min(0.4, exp2(log2(((normalMapValue.z * v_result) + (normalMapValue.w * (1.0 - v_result))) * v_reflectionPower) * 5.0));
    gl_FragColor.a = 1.0;
#else
    lowp vec4 normalMapValue = texture2D(normal, v_normalCoord);
//    lowp vec4 darkBlue = vec4(0.0705882, 0.2313725, 0.3607843, 0.2);
//    lowp vec4 lightBlue = vec4(0.2196078, 0.5764706, 0.9294118, 0.8);
//    gl_FragColor = mix(lightBlue, darkBlue, (normalMapValue.x * v_result) + (normalMapValue.y * (1.0 - v_result)))
    gl_FragColor = mix(vec4(0.0, 0.0, 0.0, 0.0), texture2D(texture0, v_texcoord0), (normalMapValue.x * v_result) + (normalMapValue.y * (1.0 - v_result)))
	+ min(0.4, exp2(log2(((normalMapValue.z * v_result) + (normalMapValue.w * (1.0 - v_result))) * v_reflectionPower) * 5.0));
#endif
}
