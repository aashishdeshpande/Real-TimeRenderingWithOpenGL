#include <iostream>
#include <stdio.h>
#include <stdlib.h> 
#include <memory.h> 

#include <X11/Xlib.h>
#include <X11/Xutil.h> 
#include <X11/XKBlib.h>
#include <X11/keysym.h> 

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glx.h>

//namespaces
using namespace std;

//global variable declarations
bool gbLighting = false;
bool bFullscreen=false;
GLboolean gbCube = GL_FALSE;
GLboolean gbSphere = GL_FALSE;
GLboolean gbPyramid = GL_TRUE;

Display *gpDisplay=NULL;
XVisualInfo *gpXVisualInfo=NULL;
Colormap gColormap;
Window gWindow;
int giWindowWidth=800;
int giWindowHeight=600;

GLfloat cubeAngle = 0.0f;
GLfloat pyramidAngle = 0.0f;
GLfloat sphereAngle = 0.0f;


GLfloat light0_ambient[] = { 0.0f,0.0f,0.0f,1.0f };
GLfloat light0_diffuse[] = { 1.0f,0.0f,0.0f,1.0f };
GLfloat light0_specular[] = { 1.0f,0.0f,0.0f,1.0f };
GLfloat light0_position[] = { 2.0f,1.0f,1.0f,0.0f };

GLfloat light1_ambient[] = { 0.0f,0.0f,0.0f,1.0f };
GLfloat light1_diffuse[] = { 0.0f,1.0f,0.0f,1.0f };
GLfloat light1_specular[] = { 0.0f,1.0f,0.0f,1.0f };
GLfloat light1_position[] = { -2.0f,1.0f,1.0f,0.0f };

GLfloat material_ambient[] = { 0.0f,0.0f,0.0f,1.0f };
GLfloat material_diffuse[] = { 1.0f,1.0f,1.0f,1.0f };
GLfloat material_specular[] = { 1.0f,1.0f,1.0f,1.0f };
GLfloat material_shininess[] = { 50.0f };

GLXContext gGLXContext;

GLUquadric *quadric = NULL;
//entry-point function
int main(void)
{
	//function prototypes
	void CreateWindow(void);
	void ToggleFullscreen(void);
	void initialize(void);
	void display(void);
	void resize(int,int);
	void uninitialize();
	void update(void);
	
	//variable declarations
	int winWidth=giWindowWidth;
	int winHeight=giWindowHeight;
	
	bool bDone=false;
	
	//code
	CreateWindow();

	//initialize
	initialize();
	
	//Message Loop
	XEvent event;
	KeySym keysym;
	
	while(bDone==false)
	{
		while(XPending(gpDisplay))
		{
			XNextEvent(gpDisplay,&event);
			switch(event.type)
			{
				case MapNotify:
					break;
				case KeyPress:
					keysym=XkbKeycodeToKeysym(gpDisplay,event.xkey.keycode,0,0);
				switch(keysym)
					{
						case XK_Escape:
							bDone=true;
							break;
						case XK_F:
						case XK_f:
							if(bFullscreen==false)
							{
								ToggleFullscreen();
								bFullscreen=true;
							}
							else
							{
								ToggleFullscreen();
								bFullscreen=false;
							}
							break;
							
						case XK_L:
						case XK_l:
						
							if(gbLighting == false)
							{
							glEnable(GL_LIGHTING);
							gbLighting = true;
							}
							
							else
							{
							gbLighting = false;
							glDisable(GL_LIGHTING); 
							}
							break;
							
						case XK_P:
						case XK_p:
							
							gbPyramid = GL_TRUE;
							gbCube = GL_FALSE;
							gbSphere = GL_FALSE;
						
							break;
							
						case XK_S:
						case XK_s:
							gbSphere = GL_TRUE;
							gbCube = GL_FALSE;
							gbPyramid = GL_FALSE;
							break;
							
						case XK_C:
						case XK_c:
							gbCube = GL_TRUE;
							gbSphere = GL_FALSE;
							gbPyramid = GL_FALSE;
							break;
						default:
							break;
					}
					break;
				case ButtonPress: 
					switch(event.xbutton.button)
					{
						case 1: 
						    break;
						case 2: 
						    break;
						case 3: 
						    break;
						default:
						    break;
					}
					break;
				case MotionNotify: 
					break;
				case ConfigureNotify: 
					winWidth=event.xconfigure.width;
					winHeight=event.xconfigure.height;
					resize(winWidth,winHeight);
					break;
				case Expose: 
					break;
				case DestroyNotify:
					break;
				case 33:
					bDone=true;
					break;
				default:
					break;
			}
		
		}

		display();
		update();
	}
	return(0);
}

void CreateWindow(void)
{
	//function prorttypes
	void uninitialize(void);

	//variable declarations
	XSetWindowAttributes winAttribs;
	int defaultScreen;
	int defaultDepth;
	int styleMask;

	static int frameBufferAttributes[]=
	{
		GLX_RGBA,
		GLX_RED_SIZE, 8,
		GLX_GREEN_SIZE, 8,
		GLX_BLUE_SIZE, 8,
		GLX_ALPHA_SIZE, 8,
		GLX_DOUBLEBUFFER,True,
		GLX_DEPTH_SIZE,24,
		None 
	}; 
	
	//code
	gpDisplay=XOpenDisplay(NULL);
	if(gpDisplay==NULL)
	{
		printf("ERROR : Unable To Open X Display.\nExitting Now...\n");
		uninitialize();
		exit(1);
	}
	
	defaultScreen=XDefaultScreen(gpDisplay);
	
	gpXVisualInfo=glXChooseVisual(gpDisplay,defaultScreen,frameBufferAttributes);
		
	winAttribs.border_pixel=0;
	winAttribs.background_pixmap=0;
	winAttribs.colormap=XCreateColormap(gpDisplay, 
					    RootWindow(gpDisplay, gpXVisualInfo->screen), 
					    gpXVisualInfo->visual,
 					    AllocNone);
	gColormap=winAttribs.colormap;

	winAttribs.background_pixel=BlackPixel(gpDisplay,defaultScreen);

	winAttribs.event_mask= ExposureMask | VisibilityChangeMask | ButtonPressMask | KeyPressMask | PointerMotionMask |
			       StructureNotifyMask;
	
	styleMask=CWBorderPixel | CWBackPixel | CWEventMask | CWColormap;
	
	gWindow=XCreateWindow(gpDisplay,
			      RootWindow(gpDisplay,gpXVisualInfo->screen),
			      0,
			      0,
			      giWindowWidth,
			      giWindowHeight,
			      0,
			      gpXVisualInfo->depth,
			      InputOutput,
			      gpXVisualInfo->visual,
			      styleMask,
			      &winAttribs);
	if(!gWindow)
	{
		printf("ERROR : Failed To Create Main Window.\nExitting Now...\n");
		uninitialize();
		exit(1);
	}
	
	XStoreName(gpDisplay,gWindow,"X_OpenGL Window:2D Rotating Shapes");
	
	Atom windowManagerDelete=XInternAtom(gpDisplay,"WM_DELETE_WINDOW",True);
	XSetWMProtocols(gpDisplay,gWindow,&windowManagerDelete,1);
	
	XMapWindow(gpDisplay,gWindow);
}

void ToggleFullscreen(void)
{
	//variable declarations
	Atom wm_state;
	Atom fullscreen;
	XEvent xev={0};
	
	//code
	wm_state=XInternAtom(gpDisplay,"_NET_WM_STATE",False);
	memset(&xev,0,sizeof(xev));
	
	xev.type=ClientMessage;
	xev.xclient.window=gWindow;
	xev.xclient.message_type=wm_state;
	xev.xclient.format=32;
	xev.xclient.data.l[0]=bFullscreen ? 0 : 1;
	
	fullscreen=XInternAtom(gpDisplay,"_NET_WM_STATE_FULLSCREEN",False);
	xev.xclient.data.l[1]=fullscreen;
	
	XSendEvent(gpDisplay,
	           RootWindow(gpDisplay,gpXVisualInfo->screen),
	           False,
	           StructureNotifyMask,
	           &xev);
}

void initialize(void)
{
	//function prototype
	void resize(int, int);
	
	//code
	gGLXContext=glXCreateContext(gpDisplay,gpXVisualInfo,NULL,GL_TRUE);
	
	glXMakeCurrent(gpDisplay,gWindow,gGLXContext);
	
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0f);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	quadric = gluNewQuadric();
	
	//STEP 2: Creation/Configuration/Positioning/Enabling One or More Light Sources.
	
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
	glEnable(GL_LIGHT0);
	
	glLightfv(GL_LIGHT1, GL_AMBIENT, light1_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light1_specular);
	glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
	glEnable(GL_LIGHT1);
	
	//STEP 4: Creation/Configuration Of Material Properties.
	
	glMaterialfv(GL_FRONT, GL_AMBIENT, material_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, material_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, material_shininess);

	resize(giWindowWidth,giWindowHeight);
}

void display(void)
{
	//code
	void drawPyramid();
	void drawCube();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	//STEP 1: Specifying Normals For Respective Geometry
	
	if(gbPyramid == GL_TRUE)
	{
	glTranslatef(0.0f, 0.0f, -6.0f);
	glRotatef(pyramidAngle, 0.0f, 1.0f, 0.0f);
	glBegin(GL_TRIANGLES);
	//FRONT FACE
	glNormal3f(0.0f,0.0f,1.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);//apex
	glVertex3f(-1.0f, -1.0f, 1.0f);//left corner 
	glVertex3f(1.0f, -1.0f, 1.0f);//right corner

								  //RIGHT FACE
	glNormal3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);//apex
	glVertex3f(1.0f, -1.0f, 1.0f);//left corner of right face
	glVertex3f(1.0f, -1.0f, -1.0f);//right corner of right face

								   //BACK FACE
	glNormal3f(0.0f, 0.0f, -1.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);//apex
	glVertex3f(1.0f, -1.0f, -1.0f);//left corner of back face
	glVertex3f(-1.0f, -1.0f, -1.0f);//right coner of back face

									//LEFT FACE
	glNormal3f(-1.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);//apex
	glVertex3f(-1.0f, -1.0f, -1.0f);//left corner of left face 
	glVertex3f(-1.0f, -1.0f, 1.0f);//right corner of left face	
	glEnd();
	}


	else if(gbCube == GL_TRUE)
	{
	
	glTranslatef(0.0f, 0.0f, -6.0f);
	glScalef(0.75f, 0.75f, 0.75f);
	glRotatef(cubeAngle, 0.0f, 1.0f, 0.0f);
	
	glBegin(GL_QUADS);

	//TOP FACE
	glNormal3f(0.0f, 1.0f, 0.0f);
	glVertex3f(1.0f, 1.0f, -1.0f);
	glVertex3f(-1.0f, 1.0f, -1.0f);
	glVertex3f(-1.0f, 1.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, 1.0f);

	//BOTTOM FACE
	glNormal3f(0.0f, -1.0f, 0.0f);
	glVertex3f(1.0f, -1.0f, -1.0f);//right top of bottom face
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, 1.0f);

	//FRONT FACE
	glNormal3f(0.0f, 0.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, 1.0f);//right top
	glVertex3f(-1.0f, 1.0f, 1.0f);//left top
	glVertex3f(-1.0f, -1.0f, 1.0f);//left bottom
	glVertex3f(1.0f, -1.0f, 1.0f);

	//BACK FACE
	glNormal3f(0.0f, 0.0f, -1.0f);
	glVertex3f(1.0f, 1.0f, -1.0f);//right top of back face
	glVertex3f(-1.0f, 1.0f, -1.0f);//left top of back face
	glVertex3f(-1.0f, -1.0f, -1.0f);//left bottom of back face
	glVertex3f(1.0f, -1.0f, -1.0f);//right bottom of back face

								   //RIGHT FACE
	glNormal3f(1.0f, 0.0f, 0.0f);
	glVertex3f(1.0f, 1.0f, -1.0f);//right top of right face
	glVertex3f(1.0f, 1.0f, 1.0f);//left top of right face
	glVertex3f(1.0f, -1.0f, 1.0f);//left bottom of right face
	glVertex3f(1.0f, -1.0f, -1.0f);//right bottom of right facce


								   //LEFT FACE
	glNormal3f(-1.0f, 0.0f, 0.0f);
	glVertex3f(-1.0f, 1.0f, 1.0f);//right top of left face
	glVertex3f(-1.0f, 1.0f, -1.0f);//left top of left face
	glVertex3f(-1.0f, -1.0f, -1.0f);//left bottom of left face
	glVertex3f(-1.0f, -1.0f, 1.0f);//right bottom of left face


	glEnd();
	}
	
	else if(gbSphere == GL_TRUE)
	{
	glTranslatef(0.0f, 0.0f, -6.0f);
	glRotatef(sphereAngle, 0.0f, 1.0f, 0.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	gluSphere(quadric, 0.50, 100, 100);
	}
	
	else
	{
	
	}
	
	glXSwapBuffers(gpDisplay,gWindow);
}





void resize(int width,int height)
{
	//code
	if (height == 0)
		height = 1;
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
}

void uninitialize(void)
{
	GLXContext currentGLXContext;
	currentGLXContext=glXGetCurrentContext();

	if(currentGLXContext!=NULL && currentGLXContext==gGLXContext)
	{
		glXMakeCurrent(gpDisplay,0,0);
	}
	
	if(gGLXContext)
	{
		glXDestroyContext(gpDisplay,gGLXContext);
	}
	
	if(gWindow)
	{
		XDestroyWindow(gpDisplay,gWindow);
	}
	
	if(gColormap)
	{
		XFreeColormap(gpDisplay,gColormap);
	}
	
	if(gpXVisualInfo)
	{
		free(gpXVisualInfo);
		gpXVisualInfo=NULL;	
	}

	if(gpDisplay)
	{
		XCloseDisplay(gpDisplay);
		gpDisplay=NULL;
	}
	
	if(quadric)
	{
		gluDeleteQuadric(quadric);
	}
}

void update(void)
{
	cubeAngle = cubeAngle + 0.05f;
	if (cubeAngle >= 360)
	{
		cubeAngle = 0.0f;
	}

	pyramidAngle = pyramidAngle + 0.05f;
	if (pyramidAngle >= 360)
	{
		pyramidAngle = 0.0f;
	}
	
	sphereAngle = sphereAngle + 0.05f;
	if (sphereAngle >= 360)
	{
		sphereAngle = 0.0f;
	}
}
