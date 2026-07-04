
#include "auxiliares.cpp"

#define maximo_de_modelos 16
#define lpn maximo_de_modelos

int N; 
int tx[lpn]; 
float matr[lpn][16];
model *mundo;
shader skybox;

float PX[4]={0,0,0,1},VV[2]={0,0},dm=0.0002,dg=.0125,rt=0.0005;

float V0[4],V1[4],HG[4];
float L0[4],L1[4],EN[4];
float NU[4]={0,0,0,0};
float *SL=V1;
enum modo{ dia=4,noche,prueba }estado;

enum alter{
	geomas=20,geomenos,
	giroxmas,giroxmenos,giroymas,giroymenos,girozmas,girozmenos,
	librexmas,librexmenos,libreymas,libreymenos,librezmas,librezmenos,
	geoturnxmas,geoturnxmenos,geoturnymas,geoturnymenos
};

void set(){
	PX[0]=PX[1]=PX[2]=VV[0]=VV[1]=0.0; PX[3]=1;
	
	L0[0]=0.0;L0[1]=0.0;L0[2]=0.0;L0[3]=1.0;
	L1[0]=0.0;L1[1]=0.0;L1[2]=0.0;L1[3]=1.0;
	
	V0[0]=0.91;V0[1]=0.89;V0[2]=0.3;V0[3]=1.0;
	V1[0]=1.0;V1[1]=1.0;V1[2]=1.0;V1[3]=1.0;
	
	EN[0]=0.03; EN[1]=0.08; V0[2]=0.12; V0[3]=1.0;

	HG[0]=18.0; HG[1]=0.0; HG[2]=2.7; HG[3]=0.0;
}

float getTime(){ return 4.*float(clock())/float(CLOCKS_PER_SEC);}

static int mv; 
static float *who,how,tim,tmp,tm;
static void initMove(float *WHO,float HOW,float TM)
{ who=WHO; how=HOW; tim=getTime(); mv=1; tmp=*who; tm=TM;}

void update(){
	float cT=getTime();
	HG[1]=fabs(cos(cT*0.6));
	if(mv) *who=tmp+(how*(cT-tim)/tm);
	if(mv && (cT-tim)>1.2) mv=0;
}

void alterate(int k){
	if(k==-1) set(); if(k==0) mv=0; 
	if(k==prueba) estado=(estado==prueba)?dia:prueba;
	switch(k){
		case geomas: initMove(PX+2,dm,rt); break;
		case geomenos: initMove(PX+2,-dm,rt); break;
		
		case geoturnxmas: initMove(VV,dg,rt); break;
		case geoturnxmenos: initMove(VV,-dg,rt); break;
		case geoturnymas: initMove(VV+1,dg,rt); break;
		case geoturnymenos: initMove(VV+1,-dg,rt); break;
		
		case librexmas: initMove(L1,dm,rt);break;
		case librexmenos: initMove(L1,-dm,rt);break;
		case libreymas: initMove(L1+1,dm,rt);break;
		case libreymenos: initMove(L1+1,-dm,rt);break;
		case librezmas: initMove(L1+2,dm,rt);break;
		case librezmenos: initMove(L1+2,-dm,rt);break;
		
		default:
		;
	}
}

void agregarLuces(shader &cual){
	cual.addVec4("L0");
	cual.addVec4("L1");
	cual.addVec4("V0");
	cual.addVec4("V1");
	cual.addVec4("HG");
	cual.addVec4("EN");
}
void modvec4(shader &cual){
	cual.enable();
	cual.modVec4("L0",L0);
	cual.modVec4("L1",L1);
	cual.modVec4("V0",V0);
	cual.modVec4("V1",SL);
	cual.modVec4("HG",HG);
	cual.modVec4("EN",EN);
}
void draw(model &cual){
	modvec4(cual.shad);
	cual.render_gl();
}

void drawSkybox(){ 
	glPushMatrix();
		glTranslatef(0,1,0);
		glScalef(15,15,15);
		modvec4(skybox);
		for(int i=0;i<4;i++){
			glBegin(GL_QUADS);
				glTexCoord2f(0,0);
				glVertex3f(-0.5,0.5,-0.5);
				glTexCoord2f(0,1);
				glVertex3f(-0.5,-0.5,-0.5);
				glTexCoord2f(1,1);
				glVertex3f(0.5,-0.5,-0.5);
				glTexCoord2f(1,0);
				glVertex3f(0.5,0.5,-0.5);
			glEnd();
			glRotatef(90,0,1,0);
		}
		glRotatef(90,1,0,0);
			glBegin(GL_QUADS);
				glTexCoord2f(0,0);
				glVertex3f(-0.5,0.5,-0.5);
				glTexCoord2f(0,1);
				glVertex3f(-0.5,-0.5,-0.5);
				glTexCoord2f(1,1);
				glVertex3f(0.5,-0.5,-0.5);
				glTexCoord2f(1,0);
				glVertex3f(0.5,0.5,-0.5);
			glEnd();
	glPopMatrix();
}

void geometry(int mask=(~0)){ 
	
	update();
	
	glPushMatrix();
	
	if(estado==prueba){
	glPushMatrix();
		glTranslatef(L1[0],L1[1],L1[2]);
		glScalef(0.03,0.03,0.03);
		glUseProgram(0);
		glColor3f(1,1,0);
		glutSolidSphere(1,24,24);
	glPopMatrix();
	}
	glTranslatef(0,-0.75,-2);
	glTranslatef(PX[0],PX[1],PX[2]);
	glRotatef(VV[1],1,0,0);
	glRotatef(VV[0],0,1,0);
	
	for(int i=0;i<N;i++) if(mask&(1<<i)){
		glPushMatrix();
			glMultMatrixf(matr[i]);
			draw(mundo[i]);
		glPopMatrix();
	}
	
	if(mask & 0x10 ) drawSkybox();
	
	glPopMatrix();
}

void initGEO(const char *namefile){
	printf("Se Cargara el sistema de geometria guardado en \"%s\"\n",namefile);
	if(mundo) delete[] mundo;
	mundo =  new model[lpn];
	string str,str1;
	ifstream in;
	in.open(namefile); N=0;
	in>>N;
	if(in.eof() || !N) return;
	for(int i=0;i<N;i++){
		in>>(tx[i]);
		in>>str;
		mundo[i]=load_model(str);
		mundo[i].box();
		in>>str>>str1;
		mundo[i].shad.loadFiles(str,str1);
		agregarLuces(mundo[i].shad);
		for(int j=0;j<tx[i];j++){
			in>>str;
			mundo[i].shad.addTexture(str);
		}
		for(int j=0;j<16;j++) in>>(matr[i][j]);
	}
	
	in>>tx[N];
	in>>str>>str1;
	skybox.loadFiles(str,str1);
	agregarLuces(skybox);
	for(int j=0;j<tx[N];j++){
		in>>str;
		skybox.addTexture(str);
	}	
	
	set();
}
