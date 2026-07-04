
uniform sampler2D TextureUnit0;
uniform sampler2D TextureUnit1;

uniform vec4 HG;
uniform vec4 EN;
uniform vec4 V0;
uniform vec4 V1;

varying vec3 norm;
varying vec3 vvec;
varying vec3 lve0;
varying vec3 lve1;

void main (void){
	
	vec4 value=texture2D(TextureUnit0,vec2(gl_TexCoord[0]));
	vec4 color=texture2D(TextureUnit1,vec2(gl_TexCoord[0]));
	
	value=normalize((value-.5)*2.);
	
	
	vec3 nr,lv,vv,lb;
	nr=normalize(norm);
	vv=normalize(vvec);
	lv=normalize(-lve0);
	lb=normalize(-lve1);
	
	vec3 tg,bn;
	bn=normalize(vec3(-nr.y-nr.z,nr.x-nr.z,nr.x+nr.y));
	tg=normalize(cross(nr,bn));
	
	mat3 mt=mat3(nr,tg,bn);
	nr=normalize(value.rbg*mt);
	//lv=normalize(mt*lv);
	//lb=normalize(mt*lb);
	
	float its1=dot(nr,lv);
	its1=sqrt(its1*its1);
	its1=clamp(its1,0,1);
	its1=its1+pow(clamp(dot(-lv,reflect(vv,nr)),0,1),HG.x);
	
	float its2=dot(nr,lb);
	its2=sqrt(its2*its2);
	its2=clamp(its2,0,1);
	its2=its2+pow(clamp(dot(-lb,reflect(vv,nr)),0,1),HG.x);
	
	gl_FragColor=EN+((its1*0.45+its2*0.55)*color);
}
