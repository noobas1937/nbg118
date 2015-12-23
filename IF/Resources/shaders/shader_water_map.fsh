#define SIMPLE

#ifdef GL_ES
precision mediump float;
#else
#define highp
#define mediump
#define lowp
#endif

uniform sampler2D texture0;
uniform sampler2D normal;

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
#ifdef SIMPLE
    vec4 normalMapValue = texture2D(normal, v_normalCoord1.xy);
    //gl_FragColor = mix(v_waterDarkSea, v_waterLightSea, (normalMapValue.y * v_result) + (normalMapValue.w + (-v_result * normalMapValue.w))); //d by ljf
    //gl_FragColor = mix(v_waterDarkSea, v_waterLightSea, (normalMapValue.y * 0.5) + (normalMapValue.w + (-0.5 * normalMapValue.w)));
    //gl_FragColor = v_waterDarkSea;
//    gl_FragColor = mix(v_waterDarkSea, v_waterLightSea, (normalMapValue.y * v_result) + (normalMapValue.w * (1.0 - v_result)));

	//gl_FragColor = mix(v_waterDarkSea, v_waterLightSea, (normalMapValue.y * v_result) + (normalMapValue.w + (-v_result * normalMapValue.w)))
	//gl_FragColor.rgb = mix(v_waterLightSea, v_waterDarkSea, (normalMapValue.x * v_result) + (normalMapValue.y * (1.0 - v_result))).rgb;
	gl_FragColor.rgb = mix(v_waterDarkSea, v_waterLightSea, (normalMapValue.y * v_result) + (normalMapValue.w + (-v_result * normalMapValue.w))).rgb
        + (min(0.4, exp2(log2(((normalMapValue.z * v_result) + (normalMapValue.w * (1.0 - v_result))) * v_reflectionPower * 0.95) * 13.0)));
        //+ (min(0.4, exp2(log2(((normalMapValue.z * v_result) + (normalMapValue.w * (1.0 - v_result))) * v_reflectionPower ) * 5.0)));
    gl_FragColor.a = 1.0;
    
#else
	#ifdef MEDIUM
	float shoreFactor = texture2D(texture0, v_texcoord0).b;
    vec4 normalMapValue = texture2D(normal, v_normalCoord1.xy);
    gl_FragColor = mix(mix(v_waterDarkSea, v_waterDarkShore, shoreFactor), mix(v_waterLightSea, v_waterLightShore, shoreFactor), (normalMapValue.y * v_result) + (normalMapValue.w + (-v_result * normalMapValue.w)));	

//    gl_FragColor = mix(mix(v_waterDarkSea, v_waterDarkShore, shoreFactor), mix(v_waterLightSea, v_waterLightShore, shoreFactor), (normalMapValue.y * v_result) + (normalMapValue.w * (1.0 - v_result)));
	#else
	float shoreFactor = texture2D(texture0, v_texcoord0).b;
	float dotProduct = dot(normalize(vec3(mix(texture2D(normal, v_normalCoord1).xy, texture2D(normal, v_normalCoord2).zw, v_result) - 0.5, 1.0 - (shoreFactor * 0.2))), v_lightVec);
	gl_FragColor = mix(mix(v_waterDarkSea, v_waterDarkShore, shoreFactor), mix(v_waterLightSea, v_waterLightShore, shoreFactor), pow((dotProduct + 1.0) * 0.5, 7.0)) + vec4(max(0.0, (dotProduct - 0.85) * 4.66666));
	#endif
#endif
}
