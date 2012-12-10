#version 120

uniform sampler2D texture0;

varying vec2 texCoord0;
varying float alpha0;

void main(void) {
	//Sample the texture
	vec4 outColor = texture2D(texture0, texCoord0.st);
	if (outColor.a > alpha0) {
		outColor.a = alpha0;
	}

	//If the alpha component is too low then discard
	//this fragment
	if (outColor.a < 0.1) {
		discard;
	}
	
	gl_FragColor = outColor;	
}
