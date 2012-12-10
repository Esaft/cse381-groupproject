#version 130

precision highp float;


uniform sampler2D texture0;

in vec2 texCoord0;
in float alpha0;

out vec4 outColor;

void main(void) {
	//Sample the texture
	outColor = texture(texture0, texCoord0.st);
	if (outColor.a > alpha0) {
		outColor.a = alpha0;
	}

	// Discard if alpha is too low
	if (outColor.a < 0.1) {
		discard;
	}

}
