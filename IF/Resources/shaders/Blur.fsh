#ifdef GL_ES
precision mediump float;
#endif

varying vec4 v_fragmentColor;
varying vec2 v_texCoord;

uniform vec2 blurSize;

void main() {
	
	
	//gl_FragColor = texture2D(CC_Texture0, v_texCoord                 )  * v_fragmentColor;
	
    
	float margin = 0.1;
	float rmargin = 1.0 - margin;
	if(v_texCoord. x < (1.0 - margin) && v_texCoord.x > margin && v_texCoord. y < (1.0 - margin) && v_texCoord.y > margin )
	{
		gl_FragColor = texture2D(CC_Texture0, v_texCoord) * v_fragmentColor;
	}
	else
	{
		vec4 orign = texture2D(CC_Texture0, v_texCoord                 ) ;
		float alpha = orign.a;
		if(alpha < 0.05)
		{
			gl_FragColor = vec4(0 , 0 , 0 , 0);
			
		}
		else
		{
			vec4 sum = vec4(0.0);
			vec4 substract = vec4(0,0,0,0);

			sum += texture2D(CC_Texture0, v_texCoord - 4.0 * blurSize) * 0.05;
			sum += texture2D(CC_Texture0, v_texCoord - 3.0 * blurSize) * 0.09;
			sum += texture2D(CC_Texture0, v_texCoord - 2.0 * blurSize) * 0.12;
			sum += texture2D(CC_Texture0, v_texCoord - 1.0 * blurSize) * 0.15;
			sum += orign * 0.16;
			sum += texture2D(CC_Texture0, v_texCoord + 1.0 * blurSize) * 0.15;
			sum += texture2D(CC_Texture0, v_texCoord + 2.0 * blurSize) * 0.12;
			sum += texture2D(CC_Texture0, v_texCoord + 3.0 * blurSize) * 0.09;
			sum += texture2D(CC_Texture0, v_texCoord + 4.0 * blurSize) * 0.05;

			vec4 temp = vec4(0,0,0,0);
			temp = (sum - substract) ;
			//temp = mix(orign, temp,  distant);
			temp = temp * v_fragmentColor;

			gl_FragColor = temp;
		}
	
	}

	
}

