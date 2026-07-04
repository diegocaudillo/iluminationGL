
#define debug

#include "auxiliares.cpp"
#include "geometria.cpp"

#ifdef debug
#include "menu.cpp"
#endif

string captureStr; 
int inCap;

static void resize(int width, int height){
    const float ar = (float) width / (float) height;
    
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45,ar,0.1,100);
    //glFrustum(-ar, ar, -1.0, 1.0, 0.1 , 30.0);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity() ;
}

int full=1;

int ultimo,actual,view=0;
char cadena[256];

static void display(void){
	ultimo =clock();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	
	if( inCap==1 ){
		glTranslatef(0,0,-.5);
		glPushMatrix();
			glTranslatef(0,0.1,0);
			glScalef(.5,.10,1);
			write("Inserte el nombre de la geometria a cargar");
		glPopMatrix();
		glPushMatrix();
			glTranslatef(0,-0.1,0);
			glScalef(.5,.10,1);
			glScalef(float(captureStr.size())/50.,1,1);
			write(captureStr.c_str());
		glPopMatrix();
		glutSwapBuffers();
		return;
	} else if( inCap==2 ){
		glTranslatef(0,0,-.5);
		glScalef(1.3,0.8,1);
		glPushMatrix();
			glTranslatef(0,0.12,0);
			glScalef(.5,.09,1);
			captureStr="Acerca de: Sistema de escenarios en 3d";
			glScalef(float(captureStr.size())/50.,1,1);
			write(captureStr.c_str());
		glPopMatrix();
		glPushMatrix();
			glTranslatef(0,0.05,0);
			glScalef(.5,.10,1);
			captureStr="Esta es una herramienta para probar shaders";
			glScalef(float(captureStr.size())/50.,1,1);
			write(captureStr.c_str());
		glPopMatrix();
		glPushMatrix();
			glTranslatef(0,-0.05,0);
			glScalef(.5,.10,1);
			captureStr=("Y multiples geometrias desde archivos preparados");
			glScalef(float(captureStr.size())/50.,1,1);
			write(captureStr.c_str());
		glPopMatrix();
		glPushMatrix();
			glTranslatef(0,-0.15,0);
			glScalef(.5,.09,1);
			captureStr=("Guardados en un formato especial");
			glScalef(float(captureStr.size())/50.,1,1);
			write(captureStr.c_str());
		glPopMatrix();
		glutSwapBuffers();
		return;
	}
	
	geometry(~0);

#ifdef debug
	actual=clock();
	glTranslatef(.38,-.45,-1.5);
	glScalef(0.43,0.15,1);
	if((actual-ultimo)>1000 && view){
		sprintf(cadena,"%3.2lf fps",double(CLOCKS_PER_SEC/4)/double(actual-ultimo));
		write(cadena);
	}
#endif

	glutSwapBuffers();
	
}


static void key(unsigned char key, int x, int y){
	if(inCap==1 && (isalpha(key) || key=='/' || key=='.') ){
		captureStr.push_back(key);
	} else if(inCap==1 && key==8){
		string tmp; GLint i;
		for(i=0;i<(int(captureStr.size())-1);i++) 
			tmp.push_back(captureStr[i]);
		captureStr = tmp;
	} else switch (key){
        case 27 :  case 'q': case 'Q': exit(0);	break;
		
		case 'e': case 'E': alterate(0); break;
		
		case 'a': case 'A': alterate(geoturnxmas); break;
		case 'd': case 'D': alterate(geoturnxmenos); break;
		case 'w': case 'W': alterate(geomas); break;
		case 's': case 'S': alterate(geomenos); break;
		
		case 'r': case 'R': alterate(geoturnymas); break;
		case 'f': case 'F': alterate(geoturnymenos); break; 
		
        case 'v': case 'V': view=!view; break;
        case 't': case 'T': alterate(prueba); break;
        
        case 'h': case 'H': inCap=2; break;
        case 'o': case 'O':
			inCap=1; captureStr="";
        break;
        
        case 13:
			if(inCap==1 && captureStr!=""){
				initGEO(captureStr.c_str());
				inCap=0;
			} else if(inCap) inCap=0;
        break;
        
        case 'p':  case 'P':
			if(full==0){ glutFullScreen(); full=1; }
			else { glutPositionWindow(10,10);
				glutReshapeWindow(1000,1000/1.6); full=0; }
			break; 
		break; 
		default:
			;
    }
    glutPostRedisplay();
}

static void special(int key,int x,int y){
	switch(key){
		case GLUT_KEY_LEFT: alterate(librexmenos); break;
		case GLUT_KEY_RIGHT: alterate(librexmas); break;
		case GLUT_KEY_UP: alterate(librezmenos); break;
		case GLUT_KEY_DOWN: alterate(librezmas); break;
		case GLUT_KEY_PAGE_UP: alterate(libreymas); break;
		case GLUT_KEY_PAGE_DOWN: alterate(libreymenos); break;
		case GLUT_KEY_HOME: alterate(-1); break;
		default:
		;
	}
}

static void idle(void){
    glutPostRedisplay();
}

void initGL(){
    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutIdleFunc(idle);
    glutSpecialFunc(special);

    glClearColor(0.0,0.0,0.0,1.0);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    //glDepthFunc(GL_LEQUAL);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

	glewInit();
}

int main(int argc, char *argv[]){
    glutInit(&argc, argv);
    glutInitWindowSize(1000,1000/1.6);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("Pharos\' Geometry");

	srand(time(0));

	initGL();
	initGEO("samples/faro.cfg");

#ifdef debug
	initLETRAS();
#endif
	
    glutFullScreen();
    glutMainLoop();
    
	
    return EXIT_SUCCESS;
}
