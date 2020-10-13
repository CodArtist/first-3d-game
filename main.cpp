#include<GL/glew.h>
#include<GL/glut.h>
#include<GL/gl.h>
#include<iostream>
#include<GL/glu.h>
#include<math.h>
#include<stdio.h>
#include<windows.h>
#include<SDL/SDL.h>
#include<vector>
#include<fstream>
#include<sstream>
#include<string>


#include <cstdlib>
#include <algorithm>
#include <cstdio>
#ifndef GL_UNSIGNED_SHORT_5_6_5
#define GL_UNSIGNED_SHORT_5_6_5 0x8363
#endif
#ifndef GL_CLAMP_TO_EDGE
#define  GL_CLAMP_TO_EDGE 0x12f
#endif
using namespace std;

int mon;
int gun;
float fac = 1.5;
struct coordinate{
        float x,y,z;
        coordinate(float a,float b,float c) : x(a),y(b),z(c) {};
};

//for faces, it can contain triangles and quads as well, the four variable contain which is that
struct face{
        int facenum;
        bool four;
        int faces[4];
        face(int facen,int f1,int f2,int f3) : facenum(facen){  //constructor for triangle
                faces[0]=f1;
                faces[1]=f2;
                faces[2]=f3;
                four=false;
        }
        face(int facen,int f1,int f2,int f3,int f4) : facenum(facen){ //overloaded constructor for quad
                faces[0]=f1;
                faces[1]=f2;
                faces[2]=f3;
                faces[3]=f4;
                four=true;
        }
};

float angle=0.0;        //we rotate or object with angle degrees

int loadObject(const char* filename)
{
        std::vector<std::string*> coord;        //read every single line of the obj file as a string
        std::vector<coordinate*> vertex;
        std::vector<face*> faces;
        std::vector<coordinate*> normals;       //normal vectors for every face
        std::ifstream in(filename);     //open the .obj file
        if(!in.is_open())       //if not opened, exit with -1
        {
                std::cout << "Nor oepened" << std::endl;
                return -1;
        }
        char buf[256];
        //read in every line to coord
        while(!in.eof())
        {
                in.getline(buf,256);
                coord.push_back(new std::string(buf));
        }
        //go through all of the elements of coord, and decide what kind of element is that
        for(int i=0;i<coord.size();i++)
        {
                if(coord[i]->c_str()[0]=='#')   //if it is a comment (the first character is #)
                        continue;       //we don't care about that
                else if(coord[i]->c_str()[0]=='v' && coord[i]->c_str()[1]==' ') //if vector
                {
                        float tmpx,tmpy,tmpz;
                        sscanf(coord[i]->c_str(),"v %f %f %f",&tmpx,&tmpy,&tmpz);       //read in the 3 float coordinate to tmpx,tmpy,tmpz
                        vertex.push_back(new coordinate(tmpx,tmpy,tmpz));       //and then add it to the end of our vertex list
                }else if(coord[i]->c_str()[0]=='v' && coord[i]->c_str()[1]=='n')        //if normal vector
                {
                        float tmpx,tmpy,tmpz;   //do the same thing
                        sscanf(coord[i]->c_str(),"vn %f %f %f",&tmpx,&tmpy,&tmpz);
                        normals.push_back(new coordinate(tmpx,tmpy,tmpz));
                }else if(coord[i]->c_str()[0]=='f')     //if face
                {
                        int a,b,c,d,e;
                        if(count(coord[i]->begin(),coord[i]->end(),' ')==3)     //if it is a triangle (it has 3 space in it)
                        {
                  sscanf(coord[i]->c_str(),"f %d//%d %d//%d %d//%d",&a,&b,&c,&b,&d,&b);
                                faces.push_back(new face(b,a,c,d));     //read in, and add to the end of the face list
                        }else{
                                sscanf(coord[i]->c_str(),"f %d//%d %d//%d %d//%d %d//%d",&a,&b,&c,&b,&d,&b,&e,&b);
                                faces.push_back(new face(b,a,c,d,e));   //do the same, except we call another constructor, and we use different pattern
                        }
                }
        }
//raw
        int num;        //the id for the list
        num=glGenLists(1);      //generate a uniqe
        glNewList(num,GL_COMPILE);      //and create it
        for(int i=0;i<faces.size();i++)
        {
                if(faces[i]->four)      //if it's a quad draw a quad
                {
                        glBegin(GL_QUADS);
                                //basically all I do here, is use the facenum (so the number of the face) as an index for the normal, so the 1st normal owe to the first face
                                //I subtract 1 because the index start from 0 in C++
                                glNormal3f(normals[faces[i]->facenum-1]->x,normals[faces[i]->facenum-1]->y,normals[faces[i]->facenum-1]->z);
                                //draw the faces
                                glVertex3f(vertex[faces[i]->faces[0]-1]->x,vertex[faces[i]->faces[0]-1]->y,vertex[faces[i]->faces[0]-1]->z);
                                glVertex3f(vertex[faces[i]->faces[1]-1]->x,vertex[faces[i]->faces[1]-1]->y,vertex[faces[i]->faces[1]-1]->z);
                                glVertex3f(vertex[faces[i]->faces[2]-1]->x,vertex[faces[i]->faces[2]-1]->y,vertex[faces[i]->faces[2]-1]->z);
                                glVertex3f(vertex[faces[i]->faces[3]-1]->x,vertex[faces[i]->faces[3]-1]->y,vertex[faces[i]->faces[3]-1]->z);
                        glEnd();
                }else{
                        glBegin(GL_TRIANGLES);
                                glNormal3f(normals[faces[i]->facenum-1]->x,normals[faces[i]->facenum-1]->y,normals[faces[i]->facenum-1]->z);
                                glVertex3f(vertex[faces[i]->faces[0]-1]->x,vertex[faces[i]->faces[0]-1]->y,vertex[faces[i]->faces[0]-1]->z);
                                glVertex3f(vertex[faces[i]->faces[1]-1]->x,vertex[faces[i]->faces[1]-1]->y,vertex[faces[i]->faces[1]-1]->z);
                                glVertex3f(vertex[faces[i]->faces[2]-1]->x,vertex[faces[i]->faces[2]-1]->y,vertex[faces[i]->faces[2]-1]->z);
                        glEnd();
                }
        }
        glEndList();
        //delete everything to avoid memory leaks
        for(int i=0;i<coord.size();i++)
                delete coord[i];
        for(int i=0;i<faces.size();i++)
                delete faces[i];
        for(int i=0;i<normals.size();i++)
                delete normals[i];
        for(int i=0;i<vertex.size();i++)
                delete vertex[i];
        return num;     //return with the id
}







    int i=0; float /*r=10.0,z=-40.0,*/camera=-2.0,camera1=-1.5,ro=0.0,ro1=0.0,y = 0.25,e=0.0 ,m=0,oppx  = 0.0,oppz = -20,cz=5.0,cx=0.0,lcx=0.0,lcz=-1.0,cy=0.0,lcy=0.0;
enum {skyleft=0,skyback,skyright,skyfront,skytop,skybottom};
unsigned int skybox[6];
class Cube
{
public:
    float x , z;
    public:
    void Cubemake(float x1,float z1)
     {x1 = x;
     z1= z;
           glLoadIdentity();
      // glTranslatef(-5.0,0.0,-5.0);

//glTranslatef(-r,0.0,-z);
gluLookAt(cx,cy,cz,
          cx+lcx,cy+lcy,cz+lcz,
          0.0,1.0,0.0);

//glRotatef(ro,0.0,1.0,0.0);
glTranslatef(0.0,camera1,0.0);


    glBegin(GL_QUADS);
    //front
    glNormal3f(0.0,0.0,1.0);
    glColor3f(1.0,0.0,0.5);
    glVertex3f(0.5-x1,3.0,0.5+z1);
    glVertex3f(-0.5-x1,3.0,0.5+z1);
    glVertex3f(-0.5-x1,-3.0,0.5+z1);
    glVertex3f(0.5-x1,-3.0,0.5+z1);

    //back
        glNormal3f(0.0,0.0,-1.0);

        glColor3f(0.0,1.0,0.0);
    glVertex3f(0.5-x1,3.0,-0.5+z1);
    glVertex3f(-0.5-x1,3.0,-0.5+z1);
    glVertex3f(-0.5-x1,-3.0,-0.5+z1);
    glVertex3f(0.5-x1,-3.0,-0.5+z1);

//right
    glNormal3f(1.0,0.0,0.0);

        glColor3f(1.0,1.0,0.0);
    glVertex3f(0.5-x1,3.0,0.5+z1);
    glVertex3f(0.5-x1,-3.0,0.5+z1);
    glVertex3f(0.5-x1,-3.0,-0.5+z1);
    glVertex3f(0.5-x1,3.0,-0.5+z1);

//left
    glNormal3f(-1.0,0.0,0.0);

        glColor3f(1.0,0.0,1.0);
    glVertex3f(-0.5-x1,3.0,0.5+z1);
    glVertex3f(-0.5-x1,-3.0,0.5+z1);
    glVertex3f(-0.5-x1,-3.0,-0.5+z1);
    glVertex3f(-0.5-x1,3.0,-0.5+z1);

  //top
      glNormal3f(0.0,1.0,0.0);

        glColor3f(1.0,0.5,0.0);
    glVertex3f(0.5-x1,3.0,0.5+z1);
    glVertex3f(0.5-x1,3.0,-0.5+z1);
    glVertex3f(-0.5-x1,3.0,-0.5+z1);
    glVertex3f(-0.5-x1,3.0,0.5+z1);

    //bottom
        glNormal3f(0.0,-1.0,0.0);

        glColor3f(1.0,0.0,0.5);
    glVertex3f(0.5-x1,-0.5,0.5+z1);
    glVertex3f(0.5-x1,-0.5,-0.5+z1);
    glVertex3f(-0.5-x1,-0.5,-0.5+z1);
    glVertex3f(-0.5-x1,-0.5,0.5+z1);

    glEnd();



     }
};
Cube cube[200];

void display();
//GLuint loadTexture( const char * filename );
void reshape(int w , int h);
void init();
void key(int keypress,int x,int y);
void mouse(int x, int y);
//void cube(float x, float z1);
float t= 0.0;
   // float angle = 0;

int main(int argc,char**argv)

{/*cube1.x = 0.0;
cube1.z = 0.0;
cube2.x = 10.0;
cube2.z = -40.0;*/
for(int j = 50; j>-50;j=j-5)
{


for(int k = -20 ;k<=25;k=k+5 )
{
    cube[i].x= k;
    cube[i].z= j;
        i++;
}
}
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE|GLUT_DEPTH);
        glutInitWindowSize(500,500);

        glutInitWindowPosition(500,100);
    glutCreateWindow("first OPen Gl");
    init();
      glutDisplayFunc(display);

    glutIdleFunc(display);
    glutReshapeFunc(reshape);
    glutPassiveMotionFunc(mouse);
    glutSpecialFunc(key);


    glutMainLoop();
return 0;
}
float a = 0.0;
void display()
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

               glLoadIdentity();
      // glTranslatef(-5.0,0.0,-5.0);
glEnable(GL_TEXTURE_2D);
   glDisable(GL_DEPTH_TEST);

//glTranslatef(-r,0.0,-z);
gluLookAt(0,0,0,
          lcx,lcy,lcz,
          0.0,1.0,0.0);

//glRotatef(ro,0.0,1.0,0.0);
glTranslatef(0.0,camera1,0.0);
glBindTexture(GL_TEXTURE_2D,skybox[skyfront]);

    glBegin(GL_QUADS);
    //front
   glTexCoord2f(1,1);
    glVertex3f(50,90.0,50);
        glTexCoord2f(0,1);
    glVertex3f(-50,90.0,50);
        glTexCoord2f(0,0);
    glVertex3f(-50,-90.0,50);
        glTexCoord2f(1,0);
    glVertex3f(50,-90.0,50);
glEnd();
glBindTexture(GL_TEXTURE_2D, 0);
    //back
    glBindTexture(GL_TEXTURE_2D,skybox[skyback]);
    glBegin(GL_QUADS);
        glTexCoord2f(0,1);
    glVertex3f(50,90.0,-50);
        glTexCoord2f(0,0);
    glVertex3f(50,-90.0,-50);
        glTexCoord2f(1,0);
    glVertex3f(-50,-90.0,-50);
        glTexCoord2f(1,1);
    glVertex3f(-50,90.0,-50);
glEnd();
//rightgl
glBindTexture(GL_TEXTURE_2D,skybox[skyright]);
        glBegin(GL_QUADS);
        glTexCoord2f(0,1);
    glVertex3f(50,90.0,50);
        glTexCoord2f(0,0);
    glVertex3f(50,-90.0,50);
        glTexCoord2f(1,0);
    glVertex3f(50,-90.0,-50);
        glTexCoord2f(1,1);
    glVertex3f(50,90.0,-50);
glEnd();
//left
    glBindTexture(GL_TEXTURE_2D,skybox[skyleft]);
    glBegin(GL_QUADS);
            glTexCoord2f(1,1);
    glVertex3f(-50,90.0,50);
        glTexCoord2f(0,1);
    glVertex3f(-50,90.0,-50);
        glTexCoord2f(0,0);
    glVertex3f(-50,-90.0,-50);
        glTexCoord2f(1,0);
    glVertex3f(-50,-90.0,50);
glEnd();
  //top
      glBindTexture(GL_TEXTURE_2D,skybox[skytop]);
    glBegin(GL_QUADS);
            glTexCoord2f(1,0);
    glVertex3f(50,90.0,50);
        glTexCoord2f(1,1);
    glVertex3f(50,90.0,-50);
        glTexCoord2f(0,1);
    glVertex3f(-50,90.0,-50);
        glTexCoord2f(0,0);
    glVertex3f(-50,90.0,50);
glEnd();
  //bottom

      glBindTexture(GL_TEXTURE_2D,skybox[skybottom]);
         glBegin(GL_QUADS);
                     glTexCoord2f(1,1);
    glVertex3f(50,-90,50);
                glTexCoord2f(0,1);
    glVertex3f(-50,-90,50);
                glTexCoord2f(0,0);
    glVertex3f(-50,-90,-50);
                glTexCoord2f(1,0);
    glVertex3f(50,-90,-50);

    glEnd();

glDisable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);

   glLoadIdentity();
        float pos[]= {0.0,10.0,0.0,1.0};
    glLightfv(GL_LIGHT5,GL_POSITION,pos);
gluLookAt(cx,0.0,cz,
          cx+lcx,lcy,cz+lcz,
          0.0,1.0,0.0);
    glTranslatef(0.0,-10,-20);
      //glTranslatef()

   // glRotatef(ro1,1.0,0.0,0.0);

    glBegin(GL_POLYGON);
    glColor3f(1.0,0.5,0.5);
    glVertex3f(500.0,0.0,550);
    glVertex3f(500.0,0.0,-500);

    glColor3f(1.0,0.5,0.0);
    glVertex3f(-500.0,0.0,-500);
    glVertex3f(-500.0,0.0,550);

    glEnd();

    glLoadIdentity();
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    gluLookAt(cx,0.0,cz,
          cx+lcx,lcy,cz+lcz,
          0.0,1.0,0.0);

          glTranslatef(0.0,-6.0,5.0);
         //glRotatef(180,0.0,1.0,0.0);
         //glRotatef(a,0.0,0.0,1.0);
            glCallList(mon);


    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT0);


     glLoadIdentity();
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

   /* gluLookAt(cx,0.0,cz,
          cx+lcx,lcy,cz+lcz,
          0.0,1.0,0.0);*/

          glTranslatef(1.0,-3.0,-3.0);
          glRotated(270,0.0,1.0,0.0);
         //glRotatef(180,0.0,1.0,0.0);
         //glRotatef(a,0.0,0.0,1.0);
            glCallList(gun);


    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT0);
   if(sin(a*3.14/180)>0)
 {//fac= fac+0.001;
//ro= ro+0.1;
    ro= ro+a/18000;}
    if(sin(a*3.14/180)<-0.1)
   {//fac= fac+0.001;
//ro= ro-0.1;
   ro= ro-a/18000;
   }
    lcx =fac*sin(ro*3.14/180);
lcz = (fac)*(-cos(ro*3.14/180));

         //  a = a+ 0.1;
/*for(int i=0;i<200;i++ )
{glPushMatrix();
    cube[i].Cubemake(0.0,1.0);
    glPopMatrix();
}*/
   // cube2.Cubemake(10.0,-40.0);


       /* glLoadIdentity();
    glTranslatef(-0.5,0.0,-2.0);
    glRotatef(angle,0.0,1.0,0.0);
  //  glTranslatef(0.0,0.0,-2.0);
    //angle= angle + 0.1;
    glBegin(GL_POLYGON);
    glColor3f(1.0,0.0,0.0);
    glVertex3f(0.5,0.5,0.0);


    glVertex3f(-0.5,0.5,0.0);


    glVertex3f(-0.5,-0.5,0.0);
    glEnd();*/
glutSwapBuffers();
//glFlush();
}
void reshape(int w , int h)
{
    glViewport(0,0,(float)w,(float)h);
   glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
gluPerspective(80,(float)w/(float)h,0.01,200.0);
   // glFrustum(-30.0,30.0,-100,50.0,0.001,50.0);*/
    glMatrixMode(GL_MODELVIEW);
   // glLoadIdentity();

}

 bool loadPPMImageFlipped(std::string &filename, unsigned &width, unsigned &height, std::vector<unsigned char> &imgData) {

    ifstream input(filename.c_str(), ifstream::in | ifstream::binary);
    if(!input) { // cast istream to bool to see if something went wrong
      cerr << "Can not find texture data file " << filename.c_str() << endl;
      return false;
    }
    input.unsetf(std::ios_base::skipws);

    string line;
    input >> line >> std::ws;
    if (line != "P6") {
      cerr << "File is not PPM P6 raw format" << endl;
      return false;
    }

    width = 0;
    height = 0;
    unsigned depth = 0;
    unsigned readItems = 0;
    unsigned char lastCharBeforeBinary;

    while (readItems < 3) {
      input >> std::ws;
      if(input.peek() != '#') {
        if (readItems == 0) input >> width;
        if (readItems == 1) input >> height;
        if (readItems == 2) input >> depth >> lastCharBeforeBinary;
        readItems++;
      }else{ // skip comments
        std::getline(input, line);
      }
    }

    if(depth >= 256) {
	    cerr << "Only 8-bit PPM format is supported" << endl;
      return false;
    }

    unsigned byteCount = width * height * 3;
    imgData.resize(byteCount);
    input.read((char*)&imgData[0], byteCount*sizeof(unsigned char));

    // vertically flip the image because the image origin
    // in OpenGL is the lower-left corner
    unsigned char tmpData;
    for(unsigned y=0; y < height / 2; y++) {
      int sourceIndex = y * width * 3;
      int targetIndex = (height-1-y) * width *3;
      for(unsigned x=0; x < width*3; x++) {
          tmpData = imgData[targetIndex];
          imgData[targetIndex] = imgData[sourceIndex];
          imgData[sourceIndex] = tmpData;
          sourceIndex++;
          targetIndex++;
      }
    }

    return true;
  }




/*unsigned int loadTexture( const char * filename )
{
/*    SDL_Surface * img =  SDL_LoadBMP(filename);
    unsigned int id;
    glGenTextures(1,&id);
    glBindTexture(GL_TEXTURE_2D,id);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,img->w,img->h,0,GL_RGB,GL_UNSIGNED_SHORT_5_6_5,img->pixels);
      glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR );
  SDL_FreeSurface(img);
  return id;*/
/*std:: cout<<"file not found";
 GLuint texture;
  int width, height;
  unsigned char * data;

  FILE * file;
  file = fopen( filename, "rb" );

  if ( file == NULL )std:: cout<<"file not found";

  width = 512;
  height = 512;
  data = (unsigned char *)malloc( width * height * 3 );
  //int size = fseek(file,);
  fread( data, width * height * 3, 1, file );
  fclose( file );

  for(int i = 0; i < width * height ; ++i)
  {
    int index = i*3;
    unsigned char B,R;
    B = data[index];
    R = data[index+2];

    data[index] = R;
    data[index+2] = B;
  }

  glGenTextures( 1, &texture );
  glBindTexture( GL_TEXTURE_2D, texture );
  glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,GL_MODULATE );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST );

        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE );
  gluBuild2DMipmaps( GL_TEXTURE_2D, 3, width, height,GL_BGR_EXT, GL_UNSIGNED_BYTE, data );
  free( data );

  return texture;
}*/


  unsigned int loadTexture(string filename) {

    unsigned width;
    unsigned height;
    int level = 0;
    int border = 0;
    std::vector<unsigned char> imgData;

    // load image data
    if(!loadPPMImageFlipped(filename, width, height, imgData)) return 0;

    // data is aligned in byte order
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    //request textureID
    GLuint textureID;
    glGenTextures( 1, &textureID);

    // bind texture
    glBindTexture( GL_TEXTURE_2D, textureID);

    //define how to filter the texture (important but ignore for now)
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    //texture colors should replace the original color values
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE); //GL_MODULATE

    // specify the 2D texture map
    glTexImage2D(GL_TEXTURE_2D, level, GL_RGB, width, height, border, GL_RGB, GL_UNSIGNED_BYTE, &imgData[0]);

    // return unique texture identifier
    return textureID;
  }

void init()

{

        std::string fileName[6];

                fileName[5]="C:\\Users\\user\\Desktop\\firstopengl\\left.ppm";
        skybox[skyleft] = loadTexture(fileName[5]);



        fileName[0]="C:\\Users\\user\\Desktop\\firstopengl\\back.ppm";
    skybox[skyback] = loadTexture(fileName[0]);

            fileName[1]="C:\\Users\\user\\Desktop\\firstopengl\\right.ppm";
skybox[skyright] = loadTexture(fileName[1]);

        fileName[2]="C:\\Users\\user\\Desktop\\firstopengl\\front.ppm";
skybox[skyfront] = loadTexture(fileName[2]);

        fileName[3]="C:\\Users\\user\\Desktop\\firstopengl\\top.ppm";
skybox[skytop] = loadTexture(fileName[3]);

        fileName[4]="C:\\Users\\user\\Desktop\\firstopengl\\bottom.ppm";
skybox[skybottom] = loadTexture(fileName[4]);

    glClearColor(0.0,0.0,0.0,1.0);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
float pos[]= {1.0,1.0,1.0,1.0};
glLightfv(GL_LIGHT0,GL_AMBIENT,pos);
 mon=loadObject("C:\\Users\\user\\Desktop\\firstopengl\\house1.obj");
 gun=loadObject("C:\\Users\\user\\Desktop\\firstopengl\\gun.obj");

   // glEnable(GL_BLEND);
  //  glEnable(GL_LIGHTING);
    //glEnable(GL_LIGHT5);-
    /*glEnable(GL_COLOR_MATERIAL);

        float diff[]= {1.0,0.0,0.0,1.0};
        glLightfv(GL_LIGHT5,GL_DIFFUSE,diff);
        float amb[]= {0.9,0.9,0.9,1.0};
        glLightfv(GL_LIGHT5,GL_AMBIENT,amb);
*/
}bool keypup = false,keypright= false,keypleft= false;
void key(int keypress,int x, int y)
{         if(keypress==GLUT_KEY_RIGHT)
     {keypright = true;

       //a = a+ 1;
        }
 if(keypress==GLUT_KEY_LEFT)
       {keypleft =  true;

      // a = a- 1;
       }


       if(keypup&&keypright){
cz = cz + lcz/6;
cx =  cx + lcx/6;
cy =  cy + lcy/6;
       a = a+ 1;


       }
       if(keypup&&keypleft){

            cz = cz + lcz/6;
cx =  cx + lcx/6;
cy =  cy + lcy/6;
       a = a- 1;}

        if(keypress==GLUT_KEY_UP)
      {keypup = true;



cz = cz + lcz/6;
cx =  cx + lcx/6;
cy =  cy + lcy/6;
//cube2.z = cube2.z + 0.3;

      }
    if(keypress==GLUT_KEY_DOWN)
        {//oppz = z-0.3;
            //z=z-0.3;
            //cube1.z = cube1.z - 0.3;
            cz= cz - lcz/6;
cx= cx-lcx/6;
cy= cy-lcy/6;
//cube2.z = cube2.z - 0.3;

        }

}

/*void cube(float x,float z1)
{

  glLoadIdentity();
      // glTranslatef(-5.0,0.0,-5.0);

//glTranslatef(-r,0.0,-z);


glRotatef(ro,0.0,1.0,0.0);
glTranslatef(0.0,camera1,0.0);


    glBegin(GL_QUADS);
    //front
    glColor3f(1.0,0.0,0.5);
    glVertex3f(0.5-r,3.0,0.5+z);
    glVertex3f(-0.5-r,3.0,0.5+z);
    glVertex3f(-0.5-r,-3.0,0.5+z);
    glVertex3f(0.5-r,-3.0,0.5+z);

    //back
        glColor3f(0.0,1.0,0.0);
    glVertex3f(0.5-r,3.0,-0.5+z);
    glVertex3f(-0.5-r,3.0,-0.5+z);
    glVertex3f(-0.5-r,-3.0,-0.5+z);
    glVertex3f(0.5-r,-3.0,-0.5+z);

//right
        glColor3f(1.0,1.0,0.0);
    glVertex3f(0.5-r,3.0,0.5+z);
    glVertex3f(0.5-r,-3.0,0.5+z);
    glVertex3f(0.5-r,-3.0,-0.5+z);
    glVertex3f(0.5-r,3.0,-0.5+z);

//left
        glColor3f(1.0,0.0,1.0);
    glVertex3f(-0.5-r,3.0,0.5+z);
    glVertex3f(-0.5-r,-3.0,0.5+z);
    glVertex3f(-0.5-r,-3.0,-0.5+z);
    glVertex3f(-0.5-r,3.0,-0.5+z);

  //top
        glColor3f(1.0,0.5,0.0);
    glVertex3f(0.5-r,3.0,0.5+z);
    glVertex3f(0.5-r,3.0,-0.5+z);
    glVertex3f(-0.5-r,3.0,-0.5+z);
    glVertex3f(-0.5-r,3.0,0.5+z);

    //bottom
        glColor3f(1.0,0.0,0.5);
    glVertex3f(0.5-r,-0.5,0.5+z);
    glVertex3f(0.5-r,-0.5,-0.5+z);
    glVertex3f(-0.5-r,-0.5,-0.5+z);
    glVertex3f(-0.5-r,-0.5,0.5+z);

    glEnd();



}*/
float h=0.0;
void mouse(int x, int y)
{
ro =(x) ;
lcx = (fac)*sin(ro*3.14/180);
lcz =(fac)*(-cos(ro*3.14/180));
/*a= x;
h = h+a;
if(h<720)
a=0.0;*/
/*cube1.x = (sqrt((cube1.x)*(cube1.x) + (cube1.z)*(cube1.z) ))*cos(ro*3.14/180);
cube1.z = (sqrt((cube1.x)*(cube1.x) + (cube1.z)*(cube1.z) ))*sin(ro*3.14/180);

cube2.x = (sqrt((cube2.x)*(cube2.x) + (cube2.z)*(cube2.z) ))*cos(ro*3.14/180);
cube2.z = (sqrt((cube2.x)*(cube2.x) + (cube2.z)*(cube2.z) ))*sin(ro*3.14/180);*/
lcy = fac*sin(y*3.14/180);
/*camera = -y/10 -0.5 +10;
camera1 = -y/10 +10 ;
   //std:: cout<< y/10<<std::endl;
t= x;
if(camera>0)
    camera = -0.01;
    if(camera1>0)
    camera1 = 0.1;*/
}

