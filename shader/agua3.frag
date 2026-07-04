
uniform sampler2D TextureUnit0;
uniform sampler2D TextureUnit1;
uniform sampler2D TextureUnit2;

uniform vec4 HG;
uniform vec4 EN;
uniform vec4 V0;
uniform vec4 V1;

varying vec3 norm;
varying vec3 vvec;
varying vec3 lve0;
varying vec3 lve1;

void main (void){
	
	float timeFactor = (1-HG.y*0.4)*0.94;
	
	vec2 uvcd=(gl_TexCoord[0].xy+(timeFactor-0.5)*0.03)*HG.z;
	
	vec4 value=texture2D(TextureUnit0,vec2(uvcd.x,uvcd.y));
	vec4 valur=texture2D(TextureUnit1,vec2(uvcd.x,uvcd.y));
	vec4 color=vec4(0.3,0.4,0.9,1);
	//texture2D(TextureUnit2,vec2(uvcd.x,uvcd.y));
	
	value=normalize((value-.5)*2.);
	valur=normalize((valur-.5)*2.);
	
	
	vec3 nr,lv,vv,lb;
	nr=normalize( (value + timeFactor*valur).rbg ).xyz;
	//normalize( ( value*HG.y + ((1-HG.y)*valur) ).rbg );
	
	nr=normalize(gl_NormalMatrix*nr.xyz);
	lv=normalize(lve0);
	lb=normalize(lve1);
	vv=normalize(vvec);
	

	float its1=dot(nr,lv);
	//its1=sqrt(its1*its1);
	its1=clamp(its1,0,1);
	its1=its1+pow(clamp(dot(-lv,reflect(vv,nr)),0,1),HG.x/6);
	
	float its2=dot(nr,lb);
	//its2=sqrt(its2*its2);
	its2=clamp(its2,0,1);
	its2=its2+pow(clamp(dot(-lb,reflect(vv,nr)),0,1),HG.x/6);
	
	gl_FragColor=EN+((its1*0.45+its2*0.55)*color);
}

