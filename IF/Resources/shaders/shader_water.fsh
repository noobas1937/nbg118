#ifdef GL_ES
precision mediump float;
#else
#define highp
#define mediump
#define lowp
#endif

//#define FOAM
//#define SIMPLE

#ifndef SIMPLE
//#ifndef MEDIUM
#define LIGHTMAP
//#endif // MEDIUM
#define REFLECTION
#endif // SIMPLE
#ifdef FOAM
#ifndef SIMPLE
#define USE_FOAM
#endif // SIMPLE
#endif // FOAM

uniform lowp sampler2D normal0;
//uniform lowp sampler2D CC_Texture0;
uniform lowp sampler2D shape; //ljf

#ifdef USE_FOAM
	uniform lowp sampler2D foam;
#endif

varying vec4 v_wave;
varying highp vec2 v_bumpUv1;
#ifdef USE_FOAM
varying highp vec2 v_foamUv;
varying float v_foamPower;
#endif
varying vec3 v_darkColor;
varying vec3 v_lightColor;
varying float v_reflectionPower;

varying vec3 v_mvpPos; //ljf

varying vec2 v_texCoord; //ljf

#ifdef LIGHTMAP
uniform lowp sampler2D lightmap;
varying vec2 v_worldPos;
#endif



void main()
{
	

    vec4 normalMapValue = texture2D(normal0, v_bumpUv1.xy);
    gl_FragColor = vec4(mix(v_lightColor, v_darkColor, (normalMapValue.x * v_wave.y) + (normalMapValue.y * (1.0 - v_wave.y))), v_wave.x) 
    
	#ifdef REFLECTION
	
//	pow(x,y)*z is calculated here as exp2(log2(x) * y + log2(z))
	
	//+ exp2(log2(((normalMapValue.z * v_wave.y) + (normalMapValue.w * (1.0 - v_wave.y))) * v_wave.z) * v_wave.w + v_reflectionPower) * 2.0
	+ exp2(log2(((normalMapValue.z * v_wave.y) + (normalMapValue.w * (1.0 - v_wave.y))) * v_wave.z) * v_wave.w ) * 2.5
	;
	
	
	//exp(-1.0 * v_mvpPos.x * v_mvpPos.x - 1.0 * v_mvpPos.y * v_mvpPos.y)
	//+ exp2(log2(((normalMapValue.z * v_wave.y) + (normalMapValue.w * (1.0 - v_wave.y))) * 1.0 * exp(-16.0 * v_mvpPos.x * v_mvpPos.x - 16.0 * v_mvpPos.y * v_mvpPos.y)) + v_reflectionPower);
	
//	+ vec4(pow(((normalMapValue.z * v_wave.y) + (normalMapValue.w * (1.0 - v_wave.y))) * v_wave.z, v_wave.w)) * v_reflectionPower;
	#else
	;
	#endif
	#ifdef USE_FOAM
	gl_FragColor = mix(gl_FragColor, vec4(0.95, 0.95, 0.95, gl_FragColor.a), min(1.0, texture2D(foam, v_foamUv).r * v_foamPower))
	#ifdef LIGHTMAP
	 * (texture2D(lightmap, v_worldPos) * 1.3);
	#else
	;
	#endif // LIGHTMAP
	#endif // USE_FOAM

	//begin ljf
	//gl_FragColor = vec4(x, 0.0, 0.0, 1.0);
	vec4 shapeValue = texture2D(shape, v_texCoord);
	gl_FragColor.a = shapeValue.x * 0.5;
	//gl_FragColor = texture2D(shape, v_texCoord);
	//gl_FragColor.a = 0.5;
	//end ljf
}
