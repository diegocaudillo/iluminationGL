
#ifndef FARO_AUX
#define FARO_AUX

#include <openctm.h>

#include <GL/glew.h>
#include <GL/glut.h>

#include <SOIL.h>

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <string>
#include <vector>
#include <fstream>

using namespace std;

string textFileRead(string S){ 
	string s; char a; 
	FILE *f=fopen(S.c_str(),"r"); 
	if(f){
	while( fscanf(f,"%c",&a)>0 ) 
	s+=a; fclose(f); 
	}
	return s; 
}

GLenum loadShader(string vert,string frag){ 
	printf("Load Vertex: %s \n", vert.c_str()); 
	printf("Load Fragment: %s\n", frag.c_str()); 
	string vs,fs; 
	GLenum v = glCreateShader(GL_VERTEX_SHADER); 
	GLenum f = glCreateShader(GL_FRAGMENT_SHADER); 
	vs = textFileRead(vert.c_str()); 
	fs = textFileRead(frag.c_str());
	const char * vv = vs.c_str();
	const char * ff = fs.c_str();
	glShaderSource(v, 1, &vv,NULL);
	glShaderSource(f, 1, &ff,NULL);
	glCompileShader(v); 
	glCompileShader(f); 
	GLenum p = glCreateProgram();
	glAttachShader(p,v);
	glAttachShader(p,f);
	glLinkProgram(p);
	return p; 
}

GLenum stringShader(string vs,string fs){
	GLenum v = glCreateShader(GL_VERTEX_SHADER); 
	GLenum f = glCreateShader(GL_FRAGMENT_SHADER); 
	const char * vv = vs.c_str();
	const char * ff = fs.c_str();
	glShaderSource(v, 1, &vv,NULL);
	glShaderSource(f, 1, &ff,NULL);
	glCompileShader(v); 
	glCompileShader(f); 
	GLenum p = glCreateProgram();
	glAttachShader(p,v);
	glAttachShader(p,f);
	glLinkProgram(p);
	return p; 
}

GLenum loadTexture(string path){
	GLenum to= SOIL_load_OGL_texture
	(
		path.c_str(),
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);
	if(0==to) printf( "SOIL loading error: '%s' with (%s)\n", SOIL_last_result() ,path.c_str());
	else printf("SOIL loading image: %i  (%s)[ OK ]\n",to,path.c_str());
	return to;
}

void getXYZ(float *ret,float *org){
	float roca[16]; int i,j,k;
	glGetFloatv(GL_MODELVIEW_MATRIX,roca);
	for(i=0,j=0;i<4;i++)
		for(ret[i]=0.0,k=0;k<4;k++,j++) 
			ret[i]+=roca[j]*org[k];
}


class shader{
	private:
		GLenum id;
		string name,vertex,fragment;
		vector< pair< pair< string,GLenum > ,float*> > vec4;
		vector< pair<GLenum,GLenum> > textures;
	public:
		shader(){id=0;} shader(const string nm){name=nm;id=0;}
		~shader(){
			for(GLuint i=0;i<vec4.size();i++)
				if(vec4[i].second) delete[] vec4[i].second;
		}
		int loadFiles(string VertexFile,string FragmentFile){
			vertex=textFileRead(VertexFile);
			fragment=textFileRead(FragmentFile);
			id=stringShader(vertex,fragment);
			return id;
		}
		int setShaders(string v,string f){
			vertex=v; fragment=f;
			id=stringShader(vertex,fragment);
			return id;
		}
		void addTexture(const string &who){
			GLenum tx,un; char str[32];
			sprintf(str,"TextureUnit%i",this->textures.size());
			tx=loadTexture(who);
			un=glGetUniformLocation(id,str);
			this->textures.push_back(make_pair(tx,un));
			//printf("DEB PR=%i TX=(%i,%i) \n",id,tx,un);
		}
		void addVec4(string UniformName){
			GLenum a;
			a=glGetUniformLocation(this->id,UniformName.c_str());
			vec4.push_back(make_pair(make_pair(UniformName,a),new float[4]));
		}
		void modVec4(string uname,float *vec){
			GLuint i,j;
			for(i=0;i<vec4.size();i++)
				if(vec4[i].first.first==uname){
					for(j=0;j<4;j++) vec4[i].second[j]=vec[j];
					return;
				}
		}
		void actTex(int mask=3){
			GLuint i,j;
			for(i=0,j=this->textures.size();i<j;i++){
				glActiveTexture(GL_TEXTURE0+i);
				glBindTexture(GL_TEXTURE_2D,this->textures[i].first);
				if(mask&1) glTexParameteri(GL_TEXTURE_2D,
										GL_TEXTURE_WRAP_S, GL_REPEAT);
				if(mask&2) glTexParameteri(GL_TEXTURE_2D, 
										GL_TEXTURE_WRAP_T, GL_REPEAT);
				glUniform1i(this->textures[i].second,i);
			}
		}
		void actVec(int mask=3){
			GLuint i;
			for(i=0;i<vec4.size();i++)
				glUniform4f(
					vec4[i].first.second,
					vec4[i].second[0],vec4[i].second[1],vec4[i].second[2],
					vec4[i].second[3]);
		}
		void enable(int mask=3){
			glUseProgram(id);
			actTex(mask);
			actVec(mask);
		}
		void disable(){ glUseProgram(0); }
};

class model{
	protected:
		int nvert;
		float *positions;
		float *normals;
		float *uvcoords;
		float cmax[3],cmin[3];
		void free(){
			if(positions) delete[] positions;
			if(normals) delete[] normals;
			if(uvcoords) delete[] uvcoords;
		}
	public:
		float delta[3];
		shader shad;
		model(){ nvert=0;positions=normals=uvcoords=0; }
		~model(){  }
		const model &operator=(const model&ref){
			this->nvert=ref.nvert; int i;
			
			if(ref.positions){
				this->positions=new float[(this->nvert)*3];
			for(i=0;i<(this->nvert*3);i++)
				this->positions[i]=ref.positions[i];
			}
				
			if(ref.normals){
				this->normals=new float[(this->nvert)*3];
			for(i=0;i<(this->nvert*3);i++)
				this->normals[i]=ref.normals[i];
			}
				
			if(ref.uvcoords){
				this->uvcoords=new float[(this->nvert)*2];
			for(i=0;i<(this->nvert*2);i++)
				this->uvcoords[i]=ref.uvcoords[i];
			}
				
			return ref;
		}
		void generate(int ntriangles,int *indices,float *srcP=0,float *srcN=0,float *srcT=0){
			int i,k;
			nvert=ntriangles*3; 
			positions=new float[nvert*3];
			if(srcN) normals=new float[nvert*3];
			if(srcT) uvcoords=new float[nvert*2];
			for(i=0;i<nvert;i++){
					for(k=0;k<3;k++){
						positions[(3*i)+k]=srcP[(3*indices[i])+k];
						if(srcN){
							normals[(3*i)+k]=srcN[(3*indices[i])+k];
							//printf("%f ",normals[(3*i)+k]);
						}
						if(srcT && k<2)
							uvcoords[(2*i)+k]=srcT[(2*indices[i])+k];
					}
					//printf("\n");
			}
		}
		void box(){
			int i,j;
			for(j=0;j<3;j++) cmax[j]=cmin[j]=positions[j];
			for(i=0;i<nvert;i++) 
				for(j=0;j<3;j++){
					cmax[j]=max(cmax[j],positions[(i*3)+j]);
					cmin[j]=min(cmin[j],positions[(i*3)+j]);
				}
			for(j=0;j<3;j++) delta[j]=cmax[j]-cmin[j];
		}
		int N(){return nvert;}
		const float *getV(){ return positions;}
		const float *getN(){ return normals;}
		const float *getT(){ return uvcoords;}
		void render_gl(int scale=1){
			shad.enable();
			glPushMatrix();
				if(scale==2) glScaled(1.0/delta[0],1.0/delta[1],1.0/delta[2]);
				if(scale==1){ float sep=max(delta[0],max(delta[1],delta[2]));
					glScaled(1.0/sep,1.0/sep,1.0/sep);
				}
				if(positions){
					glEnableClientState(GL_VERTEX_ARRAY);
					glVertexPointer(3,GL_FLOAT,0,positions);
				} 
				if(normals){
					glEnableClientState(GL_NORMAL_ARRAY);
					glNormalPointer(GL_FLOAT,0,normals);
				} 
				if(uvcoords){
					glEnableClientState(GL_TEXTURE_COORD_ARRAY);
					glTexCoordPointer(2,GL_FLOAT,0,uvcoords);
				} 
				if(positions)
					glDrawArrays(GL_TRIANGLES,0,nvert);
			glPopMatrix();
			shad.disable();
		}
};
	
model load_model(string cual){
	model tmp;
	CTMcontext context;
	CTMuint vertCount, triCount,* indices;
	CTMfloat * vertices;
	CTMfloat * coordUV;
	CTMfloat * normals;
	
	context = ctmNewContext(CTM_IMPORT);
	
	ctmLoad(context,cual.c_str());
	if(ctmGetError(context) == CTM_NONE){
		vertCount = ctmGetInteger(context, CTM_VERTEX_COUNT);
		vertices = (CTMfloat*) ctmGetFloatArray(context, CTM_VERTICES);
		triCount = ctmGetInteger(context, CTM_TRIANGLE_COUNT);
		indices = (CTMuint*) ctmGetIntegerArray(context, CTM_INDICES);
		coordUV = (CTMfloat*) ctmGetFloatArray(context, CTM_UV_MAP_1);
		normals =(CTMfloat*) ctmGetFloatArray(context, CTM_NORMALS );
		tmp.generate(triCount,(int*)indices,vertices,normals,coordUV);
	} else {
		fprintf(stderr,"ctmGetError = %i \n",ctmGetError(context));
	}
	//ctmFreeContext(context);
	return tmp;
}

#endif
