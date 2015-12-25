#ifdef GL_ES
precision mediump float;
#endif

varying vec4 v_fragmentColor;
varying vec2 v_texCoord;

uniform vec2 blurSize;
//uniform vec2 resolution;
//uniform vec2 center;

void main() {
	
	
	//gl_FragColor = texture2D(CC_Texture0, v_texCoord                 )  * v_fragmentColor;
	
    
	float margin = 0.15;
	float rmargin = 1.0 - margin;
	//if(v_texCoord.x < (1.0 - margin) && v_texCoord.x > margin && v_texCoord.y < (1.0 - margin) && v_texCoord.y > margin )
	if(v_texCoord.y < (1.0 - margin) && v_texCoord.y > margin )
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
			
			
			float distant1 = 0.0;
			float distant2 = 0.0;
			/*
			if(v_texCoord.x >= (rmargin))
			{
				distant1 = v_texCoord.x - rmargin;
			}
			if(v_texCoord.x <= (margin))
			{
				distant1 = margin - v_texCoord.x;
			}
			*/
			
			if(v_texCoord.y >= (rmargin))
			{
				distant2 = v_texCoord.y - rmargin;
			}
			if(v_texCoord.y <= (margin))
			{
				distant2 = margin - v_texCoord.y;
			}
			
			
			float distant = (distant1 + distant2) ;
			distant = distant * distant * 200.0;
			distant = min(distant, 1.0);
			vec2 newBlurSize = blurSize;
			
			vec4 sum = vec4(0.0);
			vec4 substract = vec4(0,0,0,0);
			/*
			sum += texture2D(CC_Texture0, v_texCoord - 4.0 * newBlurSize) * 0.05;
			sum += texture2D(CC_Texture0, v_texCoord - 3.0 * newBlurSize) * 0.09;
			sum += texture2D(CC_Texture0, v_texCoord - 2.0 * newBlurSize) * 0.12;
			sum += texture2D(CC_Texture0, v_texCoord - 1.0 * newBlurSize) * 0.15;
			sum += orign * 0.16;
			sum += texture2D(CC_Texture0, v_texCoord + 1.0 * newBlurSize) * 0.15;
			sum += texture2D(CC_Texture0, v_texCoord + 2.0 * newBlurSize) * 0.12;
			sum += texture2D(CC_Texture0, v_texCoord + 3.0 * newBlurSize) * 0.09;
			sum += texture2D(CC_Texture0, v_texCoord + 4.0 * newBlurSize) * 0.05;
			*/

			sum += texture2D(CC_Texture0, vec2(v_texCoord.x - newBlurSize.x, v_texCoord.y )) * 0.12;
			sum += texture2D(CC_Texture0, vec2(v_texCoord.x + newBlurSize.x, v_texCoord.y )) * 0.12;
			sum += texture2D(CC_Texture0, vec2(v_texCoord.x , v_texCoord.y - newBlurSize.y  )) * 0.12;
			sum += texture2D(CC_Texture0, vec2(v_texCoord.x , v_texCoord.y +  newBlurSize.y )) * 0.12;
			sum += orign * 0.15;
			sum += texture2D(CC_Texture0, vec2(v_texCoord.x - newBlurSize.x, v_texCoord.y - newBlurSize.y )) * 0.09;
			sum += texture2D(CC_Texture0, vec2(v_texCoord.x + newBlurSize.x, v_texCoord.y +  newBlurSize.y)) * 0.09;
			sum += texture2D(CC_Texture0, vec2(v_texCoord.x - newBlurSize.x, v_texCoord.y + newBlurSize.y  )) * 0.09;
			sum += texture2D(CC_Texture0, vec2(v_texCoord.x +  newBlurSize.x, v_texCoord.y -  newBlurSize.y )) * 0.09;			

			//sum = vec4(1.0, 0.0, 0.0, 1.0);
			vec4 temp = vec4(0,0,0,0);
			temp = (sum - substract) ;
			temp = mix(orign, temp,  distant);
			temp = temp * v_fragmentColor;

			gl_FragColor = temp;
		}
	
	}

	
}

