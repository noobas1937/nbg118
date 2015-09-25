//
//  ccShader_Water_Wave_vert.h
//  IF
//
//  Created by 李锐奇 on 14/11/11.
//
//
"\n\
#ifdef GL_ES\n\
#ifdef SIMPLE\n\
precision mediump float;\n\
#else\n\
precision highp float;\n\
#endif\n\
#else\n\
#define highp\n\
#define mediump\n\
#define lowp\n\
#endif\n\
#ifndef SIMPLE\n\
#define LIGHTMAP\n\
#define REFLECTION\n\
#endif // SIMPLE\n\
#ifdef FOAM\n\
#ifndef SIMPLE\n\
#define USE_FOAM\n\
#endif \n\
#endif \n\
attribute vec4 a_position;\n\
attribute vec2 a_texCoord;\n\
attribute vec4 a_color;\n\
varying vec4 v_wave;\n\
varying highp vec2 v_bumpUv1;\n\
#ifdef USE_FOAM\n\
varying highp vec2 v_foamUv;\n\
varying float v_foamPower;\n\
#endif\n\
varying vec3 v_darkColor;\n\
varying vec3 v_lightColor;\n\
varying float v_reflectionPower;\n\
#ifdef LIGHTMAP\n\
varying highp vec2 v_worldPos;\n\
#endif\n\
uniform highp float u_time;\n\
uniform mediump float u_1DivLevelWidth;\n\
uniform mediump float u_1DivLevelHeight;\n\
uniform mediump float WAVE_HEIGHT;\n\
uniform mediump float WAVE_MOVEMENT;\n\
uniform mediump vec3 SHORE_DARK;\n\
uniform mediump vec3 SHORE_LIGHT;\n\
uniform mediump vec3 SEA_DARK;\n\
uniform mediump vec3 SEA_LIGHT;\n\
uniform mediump vec3 u_lightPos;\n\
void main()\n\
{\n\
    vec4 pos = a_position;\n\
    float animTime = a_texCoord.y + u_time;\n\
    highp float wave = cos(animTime);\n\
    float waveHeightFactor = (wave + 1.0) * 0.5; \n\
    pos.y += WAVE_MOVEMENT * waveHeightFactor * a_color.g * a_color.b;\n\
    pos.z += wave * WAVE_HEIGHT * a_color.b;\n\
    gl_Position = CC_MVPMatrix * pos;\n\
    float maxValue = 0.55;\n\
    v_wave.x = 1.0 - (a_color.a - maxValue) * (1.0 / maxValue);\n\
    v_wave.x = v_wave.x * v_wave.x;\n\
    v_wave.x = v_wave.x * 0.8 + 0.2;\n\
    v_wave.x -= wave * a_color.b * 0.1;\n\
    v_wave.x = min(1.0, v_wave.x);\n\
    vec2 texcoordMap = vec2(a_position.x * u_1DivLevelWidth, a_position.y * u_1DivLevelHeight) * 4.0;\n\
    v_bumpUv1.xy =  texcoordMap + vec2(0.0, u_time * 0.005) * 1.5;			\n\
#ifdef USE_FOAM\n\
    v_foamUv = (texcoordMap + vec2(u_time * 0.005)) * 5.5;\n\
    float foamFactor = a_color.r * 2.0 + pow(a_color.r, 3.0);\n\
    foamFactor += min(5.0, pow((1.0 - waveHeightFactor) * a_color.g * 3.5, 3.0)) * 0.2 * (1.0 - a_color.r);\n\
    v_foamPower = foamFactor * 0.8 * 3.0;//vec4(foamFactor * 0.6, foamFactor * 0.6, foamFactor * 0.8, 0.0);//foamFactor * 0.1);\n\
    float temppi = (a_color.a - maxValue) * (1.0 / maxValue);\n\
    v_foamPower += min(1.0, min(1.0, max(0.0, -wave + 0.5) * 4.0) * temppi) * 0.5;\n\
    v_foamPower = max(0.0, v_foamPower * a_color.b);\n\
#endif\n\
    vec3 lightDir = normalize(vec3(-1.0, 1.0, 0.0));\n\
    vec3 lightVec = normalize(u_lightPos - pos.xyz);\n\
    v_wave.z = (1.0 - abs(dot(lightDir, lightVec)));\n\
    v_wave.z = v_wave.z * 0.2 + (v_wave.z * v_wave.z) * 0.8;\n\
    v_wave.z += 1.1 - (length(u_lightPos - pos.xyz) * 0.008);\n\
    v_wave.w = (1.0 + (1.0 - v_wave.z * 0.5) * 7.0);\n\
#ifdef LIGHTMAP\n\
    v_worldPos = vec2(pos.x * u_1DivLevelWidth, pos.y * u_1DivLevelHeight);\n\
#endif\n\
    v_wave.y = (cos((a_position.x + u_time) * a_position.y * 0.003 + u_time) + 1.0) * 0.5;\n\
    float blendFactor = 1.0 - min(1.0, a_color.a * 1.6);\n\
    float tx = a_position.x * u_1DivLevelWidth - 0.5;\n\
    float ty = a_position.y * u_1DivLevelHeight - 0.5;\n\
    float tmp = (tx * tx + ty * ty) / (0.75 * 0.75);\n\
    float blendFactorMul = step(1.0, tmp);\n\
    tmp = pow(tmp, 3.0);\n\
    float blendFactor2 = max(blendFactor - (1.0 - tmp) * 0.5, 0.0);\n\
    blendFactor = mix(blendFactor2, blendFactor, blendFactorMul);\n\
    v_darkColor = mix(SHORE_DARK, SEA_DARK, blendFactor);\n\
    v_lightColor = mix(SHORE_LIGHT, SEA_LIGHT, blendFactor);\n\
    v_reflectionPower = ((1.0 - a_color.a) + blendFactor) * 0.5;\n\
    v_reflectionPower = log2(v_reflectionPower);\n\
}\n\
";
