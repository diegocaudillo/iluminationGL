
uniform sampler2D TextureUnit0;

void main (void){
	vec4 col=texture2D(TextureUnit0,vec2(gl_TexCoord[0]));
	gl_FragColor=col;//gl_Color;
}

