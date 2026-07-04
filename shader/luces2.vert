
uniform vec4 L0;
uniform vec4 L1;

varying vec3 norm;
varying vec3 vvec;
varying vec3 lve0;
varying vec3 lve1;

void main(void){
	gl_TexCoord[0]=gl_MultiTexCoord0;
	
	vvec=(gl_ModelViewMatrix* gl_Vertex).xyz;
	norm=normalize(gl_NormalMatrix*gl_Normal);
	lve0=(L0-(gl_ModelViewMatrix*gl_Vertex)).xyz;
	lve1=(L1-(gl_ModelViewMatrix*gl_Vertex)).xyz;
	
	gl_Position  =  gl_ModelViewProjectionMatrix * gl_Vertex;
}
