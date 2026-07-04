
#include "auxiliares.cpp"

shader letras;
static float dy=0.1485,sty=0.0;
static float dx=0.0333,stx=0.0;

char CD[8][39]={
	"ABCDEFGHIJKLMNOPQRSTUVWXYZ",
	"abcdefghijklmnopqrstuvwxyz",
	"0123456789                ",
	".,;_<>(){}[]¡!¿?\'\" +-*/= @",
	"ÁÉÍÓÚ áéíóú Üü Ññ   "
};

static void LETRA(int i,int j,float p=0.){
	float sx,sy,ex,ey;
	sx=j*dx+stx; ex=sx+dx; 
	sy=i*dy+sty; ey=sy+dy;
	sy=1-sy; ey=1-ey;
	glBegin(GL_QUADS);
		glTexCoord2f(sx,sy);
		glVertex3f(p-.5,.5,0);
		glTexCoord2f(sx,ey);
		glVertex3f(p-.5,-.5,0);
		glTexCoord2f(ex,ey);
		glVertex3f(p+.5,-.5,0);
		glTexCoord2f(ex,sy);
		glVertex3f(p+.5,.5,0);
	glEnd();
}

void write(const char *str){
	int i,sz,j,k;
	letras.enable();
	letras.actTex();
	sz=strlen(str);
	glPushMatrix();
		glTranslatef(-0.5,0,0.001);
		glScaled(1./float(sz),1,1);
		for(i=0;i<sz;i++){
			if(isupper(str[i])) LETRA(0,str[i]-'A',i);
			else if(islower(str[i])) LETRA(1,str[i]-'a',i);
			else if(isdigit(str[i])) LETRA(2,str[i]-'0',i);
			else if(str[i]==' ') LETRA(2,11,i);
			else{
				for(j=0,k=1;k && j<26;j++) if(str[i]==CD[3][j])
					{ k=0; LETRA(3,j,i); }
				for(j=0;k && j<26;j++) if(str[i]==CD[4][j])
					{ k=0; LETRA(4,j,i); }
				if(k) LETRA(3,21,i);
			}
		}
	glPopMatrix();
	letras.disable();
}

void initLETRAS(){
	letras.loadFiles("shader/letras.vert","shader/letras.frag");
	letras.addTexture("textures/letras.bmp");
}
