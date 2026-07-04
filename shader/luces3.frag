
uniform sampler2D TextureUnit0;

uniform vec4 HG;
uniform vec4 EN;
uniform vec4 V0;
uniform vec4 V1;

varying vec3 norm;
varying vec3 vvec;
varying vec3 lve0;
varying vec3 lve1;

void main (void){
	
	vec4 color=texture2D(TextureUnit0,vec2(gl_TexCoord[0]));
	
	float dis=sqrt(dot(lve0,lve0));
	
	float its=1./(0.3*dis);
	
	gl_FragColor=vec4(0.2,0.3,0.6,1)+(its*color);
}

