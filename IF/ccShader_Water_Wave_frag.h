//
//  ccShader_Water_Wave_frag.h
//  IF
//
//  Created by 李锐奇 on 14/11/11.
//
//
"                               \n\
#ifdef GL_ES                    \n\
precision mediump float;        \n\
#else                           \n\
#define highp                   \n\
#define mediump                 \n\
#define lowp                    \n\
#endif                          \n\
#ifndef SIMPLE                  \n\
#define LIGHTMAP                \n\
#define REFLECTION              \n\
#endif             \n\
#ifdef FOAM                     \n\
#ifndef SIMPLE                  \n\
#define USE_FOAM                \n\
#endif              \n\
#endif                 \n\
                                \n\
uniform lowp sampler2D normal0;     \n\
#ifdef USE_FOAM                     \n\
uniform lowp sampler2D foam;        \n\
#endif                              \n\
                                    \n\
varying vec4 v_wave;                \n\
varying highp vec2 v_bumpUv1;       \n\
#ifdef USE_FOAM                     \n\
varying highp vec2 v_foamUv;        \n\
varying float v_foamPower;          \n\
#endif                              \n\
varying vec3 v_darkColor;           \n\
varying vec3 v_lightColor;          \n\
varying float v_reflectionPower;    \n\
                                    \n\
#ifdef LIGHTMAP                     \n\
uniform lowp sampler2D lightmap;    \n\
varying vec2 v_worldPos;            \n\
#endif                              \n\
                                    \n\
void main()                             \n\
{                                           \n\
    vec4 normalMapValue = texture2D(normal0, v_bumpUv1.xy);                 \n\
    gl_FragColor = vec4(mix(v_lightColor, v_darkColor, (normalMapValue.x * v_wave.y) + (normalMapValue.y * (1.0 - v_wave.y))), v_wave.x)        \n\
                                                                                                                                                \n\
#ifdef REFLECTION                                                                                                                               \n\
    + exp2(log2(((normalMapValue.z * v_wave.y) + (normalMapValue.w * (1.0 - v_wave.y))) * v_wave.z) * v_wave.w + v_reflectionPower);            \n\
#else                                                                                                                                           \n\
    ;                                                                                                                                           \n\
#endif                                                                                                                                          \n\
#ifdef USE_FOAM                                                                                                                                 \n\
    gl_FragColor = mix(gl_FragColor, vec4(0.95, 0.95, 0.95, gl_FragColor.a), min(1.0, texture2D(foam, v_foamUv).r * v_foamPower))               \n\
#ifdef LIGHTMAP                                                                                                                                 \n\
    * (texture2D(lightmap, v_worldPos) * 1.3);                                                                                                  \n\
#else                                                                                                                                           \n\
    ;                                                                                                                                           \n\
#endif // LIGHTMAP                                                                                                                              \n\
#endif // USE_FOAM                                                                                                                              \n\
}                                                                                                                                               \n\
";
