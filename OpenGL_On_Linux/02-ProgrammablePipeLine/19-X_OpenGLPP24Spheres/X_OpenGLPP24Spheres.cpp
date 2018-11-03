//headers
#include <iostream>
#include <stdio.h> //for printf()
#include <stdlib.h> //for exit()
#include <memory.h> //for memset()

//headers for XServer
#include <X11/Xlib.h> //analogous to windows.h
#include <X11/Xutil.h> //for visuals
#include <X11/XKBlib.h> //XkbKeycodeToKeysym()
#include <X11/keysym.h> //for 'Keysym'

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glx.h> //for 'glx' functions

#include "vmath.h" //for mathematics
#include "Sphere.h"

#define WIN_WIDTH 800
#define WIN_HEIGHT 600

using namespace vmath;

enum {
	ARD_ATTRIBUTE_POSITION = 0,
	ARD_ATTRIBUTE_COLOR,
	ARD_ATTRIBUTE_NORMAL,
	ARD_ATTRIBUTE_TEXTURE0,
};

//global variable declarations
float sphere_vertices[1146];
float sphere_normals[1146];
float sphere_textures[764];
unsigned short sphere_elements[2280];


FILE *gpFile = NULL;

GLuint gVertexShaderObject;
GLuint gFragmentShaderObject;
GLuint gShaderProgramObject;

GLuint gVao_sphere;

GLuint gVbo_sphere_position;
GLuint gVbo_sphere_normal;
GLuint gVbo_sphere_element;

GLuint gNumVertices;
GLuint gNumElements;

GLuint gModelMatrixUniform, gViewMatrixUniform, gProjectionMatrixUniform;

GLuint gLdUniform;
GLuint gLaUniform;
GLuint gLsUniform;

GLuint gKaUniform;
GLuint gKdUniform;
GLuint gKsUniform;
GLuint gmaterial_shininess_uniform;

GLuint glight_position_uniform;

GLuint gLKeyPressedUniform;
bool gbLight;

bool gbXaxis = GL_FALSE;
bool gbYaxis = GL_FALSE;
bool gbZaxis = GL_FALSE;

GLfloat gfangleX_axis = 0.0f;
GLfloat gfangleY_axis = 0.0f;
GLfloat gfangleZ_axis = 0.0f;

GLuint giWindowWidth = 0;
GLuint giWindowHeight = 0;

mat4 gPerspectiveProjectionMatrix;

GLfloat lightAmbient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat lightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat lightSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat lightPosition[] = { 100.0f, 100.0f, 100.0f, 1.0f };

//Emrald
GLfloat material1_ambient[] = { 0.0215f, 0.1745f, 0.0215f, 1.0f };
GLfloat material1_diffuse[] = { 0.07568f, 0.061424f, 0.07568f, 1.0f };
GLfloat material1_specular[] = { 0.633f, 0.727811f, 0.633f, 1.0f };
GLfloat material1_shininess =  76.8f;

//Jade
GLfloat material2_ambient[] = { 0.135f, 0.2225f ,0.1575f, 1.0f };
GLfloat material2_diffuse[] = { 0.54f, 0.89f, 0.63f, 1.0f };
GLfloat material2_specular[] = { 0.316288f, 0.316288f, 0.316288f, 1.0f };
GLfloat material2_shininess =  12.8f;

//Obsidian
GLfloat material3_ambient[] = { 0.05375f, 0.05f, 0.06625f, 1.0f };
GLfloat material3_diffuse[] = { 0.18275f, 0.17f, 0.22525f, 1.0f };
GLfloat material3_specular[] = { 0.332741f, 0.328634f, 0.346435f, 1.0f };
GLfloat material3_shininess = 38.4f;

//Pearl
GLfloat material4_ambient[] = { 0.25f, 0.20725f, 0.20725f, 1.0f };
GLfloat material4_diffuse[] = { 1.0f, 0.829f, 0.829f, 1.0f };
GLfloat material4_specular[] = { 0.296648f, 0.296648f, 0.296648f, 1.0f };
GLfloat material4_shininess = 11.264f;

//Ruby
GLfloat material5_ambient[] = { 0.1745f, 0.01175f, 0.01175f, 1.0f };
GLfloat material5_diffuse[] = { 0.61424f, 0.04136f, 0.04136f, 1.0f };
GLfloat material5_specular[] = { 0.727811f, 0.626959f, 0.626959f, 1.0f };
GLfloat material5_shininess = 76.8f ;

//Turquoise
GLfloat material6_ambient[] = { 0.1f, 0.18725f, 0.1745f, 1.0f };
GLfloat material6_diffuse[] = { 0.396f, 0.74151f, 0.69102f, 1.0f };
GLfloat material6_specular[] = { 0.297254f, 0.30829f, 0.306678f, 1.0f };
GLfloat material6_shininess = 12.8f;

//Brass
GLfloat material7_ambient[] = { 0.329412f, 0.223529f, 0.027451f, 1.0f };
GLfloat material7_diffuse[] = { 0.780392f, 0.568627f, 0.113725f, 1.0f };
GLfloat material7_specular[] = { 0.992157f, 0.941176f, 0.807843f, 1.0f };
GLfloat material7_shininess = 27.897443616f;

//Bronze
GLfloat material8_ambient[] = { 0.2125f, 0.1275f, 0.054f, 1.0f };
GLfloat material8_diffuse[] = { 0.714f, 0.4284f, 0.18144f, 1.0f };
GLfloat material8_specular[] = { 0.393548f, 0.271906f, 0.166721f, 1.0f };
GLfloat material8_shininess =  25.6f;

//Chrome
GLfloat material9_ambient[] = { 0.25f, 0.25f, 0.25f, 1.0f };
GLfloat material9_diffuse[] = { 0.4f, 0.4f, 0.4f, 1.0f };
GLfloat material9_specular[] = { 0.774597f, 0.774597f, 0.774597f, 1.0f };
GLfloat material9_shininess = 76.8f;

//Copper
GLfloat material10_ambient[] = { 0.19125f, 0.0735f, 0.0225f, 1.0f };
GLfloat material10_diffuse[] = { 0.7038f, 0.27048f, 0.0828f, 1.0f };
GLfloat material10_specular[] = { 0.256777f, 0.137622f, 0.086014f, 1.0f };
GLfloat material10_shininess = 12.8f;

//Gold
GLfloat material11_ambient[] = { 0.24725f, 0.1995f, 0.0745f, 1.0f };
GLfloat material11_diffuse[] = { 0.75164f, 0.60648f, 0.22648f, 1.0f };
GLfloat material11_specular[] = { 0.628281f, 0.555802f, 0.366065f, 1.0f };
GLfloat material11_shininess = 51.2f;

//Silver
GLfloat material12_ambient[] = { 0.19225f, 0.19225f, 0.19225f, 1.0f };
GLfloat material12_diffuse[] = { 0.50754f, 0.50754f, 0.50754f, 1.0f };
GLfloat material12_specular[] = { 0.508273f, 0.508273f, 0.508273f, 1.0f };
GLfloat material12_shininess = 51.2f ;

//Black
GLfloat material13_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat material13_diffuse[] = { 0.01f, 0.01f, 0.01f, 1.0f };
GLfloat material13_specular[] = { 0.50f, 0.50f, 0.50f, 1.0f };
GLfloat material13_shininess = 32.0f;

//Cyan
GLfloat material14_ambient[] = { 0.0f, 0.1f, 0.06f, 1.0f };
GLfloat material14_diffuse[] = { 0.0f, 0.50980392f, 0.50980392f, 1.0f };
GLfloat material14_specular[] = { 0.50196078f, 0.50196078f, 0.50196078f, 1.0f };
GLfloat material14_shininess = 32.0f;

//Green
GLfloat material15_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat material15_diffuse[] = { 0.1f, 0.35f, 0.1f, 1.0f };
GLfloat material15_specular[] = { 0.45f, 0.55f, 0.45f, 1.0f };
GLfloat material15_shininess = 32.0f;

//Red
GLfloat material16_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat material16_diffuse[] = { 0.5f, 0.0f, 0.0f, 1.0f };
GLfloat material16_specular[] = { 0.7f, 0.6f, 0.6f, 1.0f };
GLfloat material16_shininess = 32.0f;

//White
GLfloat material17_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat material17_diffuse[] = { 0.55f, 0.55f, 0.55f, 1.0f };
GLfloat material17_specular[] = { 0.70f, 0.70f, 0.70f, 1.0f };
GLfloat material17_shininess = 32.0f;

//Yellow Plastic
GLfloat material18_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat material18_diffuse[] = { 0.5f, 0.5f, 0.0f, 1.0f };
GLfloat material18_specular[] = { 0.60f, 0.60f, 0.50f, 1.0f };
GLfloat material18_shininess = 32.0f;

//Black
GLfloat material19_ambient[] = { 0.02f, 0.02f, 0.02f, 1.0f };
GLfloat material19_diffuse[] = { 0.01f, 0.01f, 0.01f, 1.0f };
GLfloat material19_specular[] = { 0.4f, 0.4f, 0.4f, 1.0f };
GLfloat material19_shininess = 10.0f;

//Cyan
GLfloat material20_ambient[] = { 0.0f, 0.05f, 0.05f, 1.0f };
GLfloat material20_diffuse[] = { 0.4f, 0.5f, 0.5f, 1.0f };
GLfloat material20_specular[] = { 0.04f, 0.7f, 0.7f, 1.0f };
GLfloat material20_shininess = 10.0f;

//Green
GLfloat material21_ambient[] = { 0.0f, 0.05f, 0.0f, 1.0f };
GLfloat material21_diffuse[] = { 0.4f, 0.5f, 0.4f, 1.0f };
GLfloat material21_specular[] = { 0.04f, 0.7f, 0.04f, 1.0f };
GLfloat material21_shininess = 10.0f;

//Red
GLfloat material22_ambient[] = { 0.05f, 0.0f, 0.0f, 1.0f };
GLfloat material22_diffuse[] = { 0.5f, 0.4f, 0.4f, 1.0f };
GLfloat material22_specular[] = { 0.7f, 0.04f, 0.04f, 1.0f };
GLfloat material22_shininess = 10.0f;

//White
GLfloat material23_ambient[] = { 0.05f, 0.05f, 0.05f, 1.0f };
GLfloat material23_diffuse[] = { 0.5f, 0.5f, 0.5f, 1.0f };
GLfloat material23_specular[] = { 0.7f, 0.7f, 0.7f, 1.0f };
GLfloat material23_shininess = 10.0f;

//Yellow Rubber
GLfloat material24_ambient[] = { 0.05f, 0.05f, 0.0f, 1.0f };
GLfloat material24_diffuse[] = { 0.5f, 0.5f, 0.4f, 1.0f };
GLfloat material24_specular[] = { 0.7f, 0.7f, 0.04f, 1.0f };
GLfloat material24_shininess = 10.0f;


Display *gpDisplay=NULL;
XVisualInfo *gpXVisualInfo=NULL;
Colormap gColormap;
Window gWindow;
typedef GLXContext (*glXCreateContextAttribsARBProc)(Display*, GLXFBConfig, GLXContext, Bool, const int*);
glXCreateContextAttribsARBProc glXCreateContextAttribsARB=NULL;
GLXFBConfig gGLXFBConfig;
GLXContext gGLXContext; //parallel to HGLRC

bool gbFullscreen = false;
//entry-point function
int main(int argc, char *argv[])
{
	//function prototype
	void CreateWindow(void);
	void ToggleFullscreen(void);
	void initialize(void);
	void resize(int,int);
	void display(void);
	void spin(void);
	void uninitialize(void);
	
	static bool bIsAKeyPressed = false;
	static bool bIsLKeyPressed = false;
	//code
	// create log file
	gpFile=fopen("Log.txt", "w");
	if (gpFile==NULL)
	{
		printf("Log File Can Not Be Created. EXitting Now ...\n");
		exit(0);
	}
	else
	{
		fprintf(gpFile, "Log File Is Successfully Opened.\n");
	}
	
	// create the window
	CreateWindow();
	
	//initialize()
	initialize();
	
	//Message Loop

	//variable declarations
	XEvent event; //parallel to 'MSG' structure
	KeySym keySym;
	int winWidth;
	int winHeight;
	bool bDone=false;
	
	while(bDone==false)
	{
		while(XPending(gpDisplay))
		{
			XNextEvent(gpDisplay,&event); //parallel to GetMessage()
			switch(event.type) //parallel to 'iMsg'
			{
				case MapNotify: //parallel to WM_CREATE
					break;
				case KeyPress: //parallel to WM_KEYDOWN
					keySym=XkbKeycodeToKeysym(gpDisplay,event.xkey.keycode,0,0);
					switch(keySym)
					{
						case XK_Escape:
							bDone=true;
							break;
						case XK_F:
						case XK_f:
							if(gbFullscreen==false)
							{
								ToggleFullscreen();
								gbFullscreen=true;
							}
							else
							{
								ToggleFullscreen();
								gbFullscreen=false;
							}
							break;
							
						case XK_L:
						case XK_l:
							if(bIsLKeyPressed == false)
							{
								gbLight = true;
								bIsLKeyPressed = true;
							}
							else
							{
								gbLight = false;
								bIsLKeyPressed = false;
							}
							break;
							
						case XK_X:
						case XK_x:
							gbXaxis = GL_TRUE;
							gbYaxis = GL_FALSE;
							gbZaxis = GL_FALSE;
						
							break;
							
							
						case XK_Y:
						case XK_y:
							gbYaxis = GL_TRUE;
							gbXaxis = GL_FALSE;
							gbZaxis = GL_FALSE;
							break;
							
							
						case XK_Z:
						case XK_z:
							gbZaxis = GL_TRUE;
							gbYaxis = GL_FALSE;
							gbXaxis = GL_FALSE;
							break;
						
						default:
							break;
					}
					break;
				case ButtonPress:
					switch(event.xbutton.button)
					{
						case 1: //Left Button
							break;
						case 2: //Middle Button
							break;
						case 3: //Right Button
							break;
						default: 
							break;
					}
					break;
				case MotionNotify: //parallel to WM_MOUSEMOVE
					break;
				case ConfigureNotify: //parallel to WM_SIZE
					winWidth=event.xconfigure.width;
					winHeight=event.xconfigure.height;
					resize(winWidth,winHeight);
					break;
				case Expose: //parallel to WM_PAINT
					break;
				case DestroyNotify:
					break;
				case 33: //close button, system menu -> close
					bDone=true;
					break;
				default:
					break;
			}
		}
		
		display();
		spin();
	}
	
	uninitialize();
	return(0);
}

void CreateWindow(void)
{
	//function prototype
	void uninitialize(void);
	
	//variable declarations
	XSetWindowAttributes winAttribs;
	GLXFBConfig *pGLXFBConfigs=NULL;
	GLXFBConfig bestGLXFBConfig;
	XVisualInfo *pTempXVisualInfo=NULL;
	int iNumFBConfigs=0;
	int styleMask;
	int i;
	
	static int frameBufferAttributes[]={
		GLX_X_RENDERABLE,True,
		GLX_DRAWABLE_TYPE,GLX_WINDOW_BIT,
		GLX_RENDER_TYPE,GLX_RGBA_BIT,
		GLX_X_VISUAL_TYPE,GLX_TRUE_COLOR,
		GLX_RED_SIZE,8,
		GLX_GREEN_SIZE,8,
		GLX_BLUE_SIZE,8,
		GLX_ALPHA_SIZE,8,
		GLX_DEPTH_SIZE,24,
		GLX_STENCIL_SIZE,8,
		GLX_DOUBLEBUFFER,True,
		//GLX_SAMPLE_BUFFERS,1,
		//GLX_SAMPLES,4,
		None}; // array must be terminated by 0
	
	//code
	gpDisplay=XOpenDisplay(NULL);
	if(gpDisplay==NULL)
	{
		printf("ERROR : Unable To Obtain X Display.\n");
		uninitialize();
		exit(1);
	}
	
	// get a new framebuffer config that meets our attrib requirements
	pGLXFBConfigs=glXChooseFBConfig(gpDisplay,DefaultScreen(gpDisplay),frameBufferAttributes,&iNumFBConfigs);
	if(pGLXFBConfigs==NULL)
	{
		printf( "Failed To Get Valid Framebuffer Config. Exitting Now ...\n");
		uninitialize();
		exit(1);
	}
	printf("%d Matching FB Configs Found.\n",iNumFBConfigs);
	
	// pick that FB config/visual with the most samples per pixel
	int bestFramebufferconfig=-1,worstFramebufferConfig=-1,bestNumberOfSamples=-1,worstNumberOfSamples=999;
	for(i=0;i<iNumFBConfigs;i++)
	{
		pTempXVisualInfo=glXGetVisualFromFBConfig(gpDisplay,pGLXFBConfigs[i]);
		if(pTempXVisualInfo)
		{
			int sampleBuffer,samples;
			glXGetFBConfigAttrib(gpDisplay,pGLXFBConfigs[i],GLX_SAMPLE_BUFFERS,&sampleBuffer);
			glXGetFBConfigAttrib(gpDisplay,pGLXFBConfigs[i],GLX_SAMPLES,&samples);
			printf("Matching Framebuffer Config=%d : Visual ID=0x%lu : SAMPLE_BUFFERS=%d : SAMPLES=%d\n",i,pTempXVisualInfo->visualid,sampleBuffer,samples);
			if(bestFramebufferconfig < 0 || sampleBuffer && samples > bestNumberOfSamples)
			{
				bestFramebufferconfig=i;
				bestNumberOfSamples=samples;
			}
			if( worstFramebufferConfig < 0 || !sampleBuffer || samples < worstNumberOfSamples)
			{
				worstFramebufferConfig=i;
			    worstNumberOfSamples=samples;
			}
		}
		XFree(pTempXVisualInfo);
	}
	bestGLXFBConfig = pGLXFBConfigs[bestFramebufferconfig];
	// set global GLXFBConfig
	gGLXFBConfig=bestGLXFBConfig;
	
	// be sure to free FBConfig list allocated by glXChooseFBConfig()
	XFree(pGLXFBConfigs);
	
	gpXVisualInfo=glXGetVisualFromFBConfig(gpDisplay,bestGLXFBConfig);
	printf("Chosen Visual ID=0x%lu\n",gpXVisualInfo->visualid );
	
	//setting window's attributes
	winAttribs.border_pixel=0;
	winAttribs.background_pixmap=0;
	winAttribs.colormap=XCreateColormap(gpDisplay,
										RootWindow(gpDisplay,gpXVisualInfo->screen), //you can give defaultScreen as well
										gpXVisualInfo->visual,
										AllocNone); //for 'movable' memory allocation
										
	winAttribs.event_mask=StructureNotifyMask | KeyPressMask | ButtonPressMask |
						  ExposureMask | VisibilityChangeMask | PointerMotionMask;
	
	styleMask=CWBorderPixel | CWEventMask | CWColormap;
	gColormap=winAttribs.colormap;										           
	
	gWindow=XCreateWindow(gpDisplay,
						  RootWindow(gpDisplay,gpXVisualInfo->screen),
						  0,
						  0,
						  WIN_WIDTH,
						  WIN_HEIGHT,
						  0, //border width
						  gpXVisualInfo->depth, //depth of visual (depth for Colormap)          
						  InputOutput, //class(type) of your window
						  gpXVisualInfo->visual,
						  styleMask,
						  &winAttribs);
	if(!gWindow)
	{
		printf("Failure In Window Creation.\n");
		uninitialize();
		exit(1);
	}
	
	XStoreName(gpDisplay,gWindow,"OpenGL Window Programmable PipeLine : Phong Lighting(Per Fragment)");
	
	Atom windowManagerDelete=XInternAtom(gpDisplay,"WM_WINDOW_DELETE",True);
	XSetWMProtocols(gpDisplay,gWindow,&windowManagerDelete,1);
	
	XMapWindow(gpDisplay,gWindow);
}

void ToggleFullscreen(void)
{
	//code
	Atom wm_state=XInternAtom(gpDisplay,"_NET_WM_STATE",False); //normal window state
	
	XEvent event;
	memset(&event,0,sizeof(XEvent));
	
	event.type=ClientMessage;
	event.xclient.window=gWindow;
	event.xclient.message_type=wm_state;
	event.xclient.format=32; //32-bit
	event.xclient.data.l[0]=gbFullscreen ? 0 : 1;

	Atom fullscreen=XInternAtom(gpDisplay,"_NET_WM_STATE_FULLSCREEN",False);	
	event.xclient.data.l[1]=fullscreen;
	
	//parallel to SendMessage()
	XSendEvent(gpDisplay,
			   RootWindow(gpDisplay,gpXVisualInfo->screen),
			   False, //do not send this message to Sibling windows
			   StructureNotifyMask, //resizing mask (event_mask)
			   &event);	
}

void initialize(void)
{
	// function declarations
	void uninitialize(void);
	void resize(int,int);
	
	//code
	// create a new GL context 4.5 for rendering
	glXCreateContextAttribsARB = (glXCreateContextAttribsARBProc)glXGetProcAddressARB((GLubyte *)"glXCreateContextAttribsARB");
	
	GLint attribs[] = {
		GLX_CONTEXT_MAJOR_VERSION_ARB,4,
		GLX_CONTEXT_MINOR_VERSION_ARB,5,
		GLX_CONTEXT_PROFILE_MASK_ARB, GLX_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB,
		0 }; // array must be terminated by 0
		
	gGLXContext = glXCreateContextAttribsARB(gpDisplay,gGLXFBConfig,0,True,attribs);

	if(!gGLXContext) // fallback to safe old style 2.x context
	{
		// When a context version below 3.0 is requested, implementations will return 
		// the newest context version compatible with OpenGL versions less than version 3.0.
		GLint attribs[] = {
			GLX_CONTEXT_MAJOR_VERSION_ARB,1,
			GLX_CONTEXT_MINOR_VERSION_ARB,0,
			0 }; // array must be terminated by 0
		printf("Failed To Create GLX 4.5 context. Hence Using Old-Style GLX Context\n");
		gGLXContext = glXCreateContextAttribsARB(gpDisplay,gGLXFBConfig,0,True,attribs);
	}
	else // successfully created 4.1 context
	{
		printf("OpenGL Context 4.5 Is Created.\n");
	}
	
	// verifying that context is a direct context
	if(!glXIsDirect(gpDisplay,gGLXContext))
	{
		printf("Indirect GLX Rendering Context Obtained\n");
	}
	else
	{
		printf("Direct GLX Rendering Context Obtained\n" );
	}
	
	glXMakeCurrent(gpDisplay,gWindow,gGLXContext);
	
	GLenum gl_error = glewInit();
	if(gl_error != GLEW_OK)
	{
	
	
	} 
	else
	{
	printf("\nGLEW Initialized\n");
	}
	
	//VERTEX SAHDER

	// *** VERTEX SHADER ***
	// create shader
	gVertexShaderObject = glCreateShader(GL_VERTEX_SHADER);

	// provide source code to shader
	const GLchar *vertexShaderSourceCode =
		"#version 430 core" \
		"\n" \
		"in vec4 vPosition;" \
		"in vec3 vNormal;" \
		"uniform mat4 u_model_matrix;" \
		"uniform mat4 u_view_matrix;" \
		"uniform mat4 u_projection_matrix;" \
		"uniform vec4 u_light_position;" \
		"uniform int u_lighting_enabled;" \
		"out vec3 transformed_normals;" \
		"out vec3 light_direction;" \
		"out vec3 viewer_vector;" \
		"void main(void)" \
		"{" \
		"if(u_lighting_enabled == 1)" \
		"{" \
		"vec4 eyeCoordinates = u_view_matrix * u_model_matrix * vPosition;" \
		"transformed_normals = normalize(mat3(u_view_matrix * u_model_matrix) * vNormal);" \
		"light_direction = normalize(vec3(u_light_position) - eyeCoordinates.xyz);" \
		"viewer_vector = normalize(-eyeCoordinates.xyz);"
		"}" \
		"gl_Position = u_projection_matrix * u_view_matrix * u_model_matrix * vPosition;" \
		"}";
	glShaderSource(gVertexShaderObject, 1, (const GLchar **)&vertexShaderSourceCode, NULL);

	// compile shader
	glCompileShader(gVertexShaderObject);
	GLint iInfoLogLength = 0;
	GLint iShaderCompiledStatus = 0;
	char *szInfoLog = NULL;
	glGetShaderiv(gVertexShaderObject, GL_COMPILE_STATUS, &iShaderCompiledStatus);
	if (iShaderCompiledStatus == GL_FALSE)
	{
		glGetShaderiv(gVertexShaderObject, GL_INFO_LOG_LENGTH, &iInfoLogLength);
		if (iInfoLogLength > 0)
		{
			szInfoLog = (char *)malloc(iInfoLogLength);
			if (szInfoLog != NULL)
			{
				GLsizei written;
				glGetShaderInfoLog(gVertexShaderObject, iInfoLogLength, &written, szInfoLog);
				fprintf(gpFile, "Vertex Shader Compilation Log : %s\n", szInfoLog);
				free(szInfoLog);
				uninitialize();
				exit(0);
			}
		}
	}

	// *** FRAGMENT SHADER ***
	// create shader
	gFragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER);

	// provide source code to shader
	const GLchar *fragmentShaderSourceCode =
		"#version 430 core" \
		"\n" \
		"in vec3 transformed_normals;" \
		"in vec3 light_direction;" \
		"in vec3 viewer_vector;" \
		"out vec4 FragColor;" \
		"uniform vec3 u_La;" \
		"uniform vec3 u_Ka1;" \
		"uniform vec3 u_Ld;" \
		"uniform vec3 u_Kd1;" \
		"uniform vec3 u_Ls;" \
		"uniform vec3 u_Ks1;" \
		"uniform float u_material1_shininess;" \
		"uniform int u_lighting_enabled;" \
		"void main(void)" \
		"{" \
		"vec3 phong_ads_color;" \
		"if(u_lighting_enabled == 1)" \
		"{" \
		"vec3 normalized_transformed_normals = normalize(transformed_normals);" \
		"vec3 normalized_light_direction = normalize(light_direction);" \
		"vec3 normalized_viewer_vector = normalize(viewer_vector);" \
		"vec3 ambient = u_La * u_Ka1;" \
		"float tn_dot_ld = max(dot(normalized_transformed_normals, normalized_light_direction), 0.0);" \
		"vec3 diffuse = u_Ld * u_Kd1 * tn_dot_ld;" \
		"vec3 reflection_vector = reflect(-normalized_light_direction, normalized_transformed_normals);" \
		"vec3 specular = u_Ls * u_Ks1 * pow(max(dot(reflection_vector, normalized_viewer_vector), 0.0), u_material1_shininess);" \
		"phong_ads_color = ambient + diffuse + specular;" \
		"}"
		"else" \
		"{" \
		"phong_ads_color = vec3(1.0, 1.0, 1.0);" \
		"}"\
		"FragColor = vec4(phong_ads_color, 1.0);" \
		"}";
	glShaderSource(gFragmentShaderObject, 1, (const GLchar **)&fragmentShaderSourceCode, NULL);

	// compile shader
	glCompileShader(gFragmentShaderObject);
	glGetShaderiv(gFragmentShaderObject, GL_COMPILE_STATUS, &iShaderCompiledStatus);
	if (iShaderCompiledStatus == GL_FALSE)
	{
		glGetShaderiv(gFragmentShaderObject, GL_INFO_LOG_LENGTH, &iInfoLogLength);
		if (iInfoLogLength > 0)
		{
			szInfoLog = (char *)malloc(iInfoLogLength);
			if (szInfoLog != NULL)
			{
				GLsizei written;
				glGetShaderInfoLog(gFragmentShaderObject, iInfoLogLength, &written, szInfoLog);
				fprintf(gpFile, "Fragment Shader Compilation Log : %s\n", szInfoLog);
				free(szInfoLog);
				uninitialize();
				exit(0);
			}
		}
	}

	// *** SHADER PROGRAM ***
	// create
	gShaderProgramObject = glCreateProgram();

	// attach vertex shader to shader program
	glAttachShader(gShaderProgramObject, gVertexShaderObject);

	// attach fragment shader to shader program
	glAttachShader(gShaderProgramObject, gFragmentShaderObject);

	// pre-link binding of shader program object with vertex shader position attribute
	glBindAttribLocation(gShaderProgramObject, ARD_ATTRIBUTE_POSITION, "vPosition");
	glBindAttribLocation(gShaderProgramObject, ARD_ATTRIBUTE_NORMAL, "vNormal");

	// link shader
	glLinkProgram(gShaderProgramObject);
	GLint iShaderProgramLinkStatus = 0;
	glGetProgramiv(gShaderProgramObject, GL_LINK_STATUS, &iShaderProgramLinkStatus);
	if (iShaderProgramLinkStatus == GL_FALSE)
	{
		glGetProgramiv(gShaderProgramObject, GL_INFO_LOG_LENGTH, &iInfoLogLength);
		if (iInfoLogLength>0)
		{
			szInfoLog = (char *)malloc(iInfoLogLength);
			if (szInfoLog != NULL)
			{
				GLsizei written;
				glGetProgramInfoLog(gShaderProgramObject, iInfoLogLength, &written, szInfoLog);
				fprintf(gpFile, "Shader Program Link Log : %s\n", szInfoLog);
				free(szInfoLog);
				uninitialize();
				exit(0);
			}
		}
	}


	gModelMatrixUniform = glGetUniformLocation(gShaderProgramObject, "u_model_matrix");
	gViewMatrixUniform = glGetUniformLocation(gShaderProgramObject, "u_view_matrix");
	gProjectionMatrixUniform = glGetUniformLocation(gShaderProgramObject, "u_projection_matrix");

	gLKeyPressedUniform = glGetUniformLocation(gShaderProgramObject, "u_lighting_enabled");


	gLaUniform = glGetUniformLocation(gShaderProgramObject, "u_La");
	gKaUniform = glGetUniformLocation(gShaderProgramObject, "u_Ka1");

	gLdUniform = glGetUniformLocation(gShaderProgramObject, "u_Ld");
 	gKdUniform = glGetUniformLocation(gShaderProgramObject, "u_Kd1");

	gLsUniform = glGetUniformLocation(gShaderProgramObject, "u_Ls");
    gKsUniform = glGetUniformLocation(gShaderProgramObject, "u_Ks1");

	glight_position_uniform = glGetUniformLocation(gShaderProgramObject, "u_light_position");

	gmaterial_shininess_uniform = glGetUniformLocation(gShaderProgramObject, "u_material1_shininess");

	getSphereVertexData(sphere_vertices, sphere_normals, sphere_textures, sphere_elements);
	gNumVertices = getNumberOfSphereVertices();
	gNumElements = getNumberOfSphereElements();

	// vao
	glGenVertexArrays(1, &gVao_sphere);
	glBindVertexArray(gVao_sphere);

	// position vbo
	glGenBuffers(1, &gVbo_sphere_position);
	glBindBuffer(GL_ARRAY_BUFFER, gVbo_sphere_position);
	glBufferData(GL_ARRAY_BUFFER, sizeof(sphere_vertices), sphere_vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(ARD_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glEnableVertexAttribArray(ARD_ATTRIBUTE_POSITION);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// normal vbo
	glGenBuffers(1, &gVbo_sphere_normal);
	glBindBuffer(GL_ARRAY_BUFFER, gVbo_sphere_normal);
	glBufferData(GL_ARRAY_BUFFER, sizeof(sphere_normals), sphere_normals, GL_STATIC_DRAW);

	glVertexAttribPointer(ARD_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glEnableVertexAttribArray(ARD_ATTRIBUTE_NORMAL);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// element vbo
	glGenBuffers(1, &gVbo_sphere_element);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(sphere_elements), sphere_elements, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// unbind vao
	glBindVertexArray(0);


	//MessageBox(NULL, TEXT("After Sqaure final"), TEXT("In Init"), MB_OK);

	glShadeModel(GL_SMOOTH);
	// set-up depth buffer
	glClearDepth(1.0f);
	// enable depth testing
	glEnable(GL_DEPTH_TEST);
	// depth test to do
	glDepthFunc(GL_LEQUAL);
	// We will always cull back faces for better performance
	//glEnable(GL_CULL_FACE);

	// set background color to which it will display even if it will empty. THIS LINE CAN BE IN drawRect().
	glClearColor(0.5f, 0.5f, 0.5f, 0.0f); // black

	gPerspectiveProjectionMatrix = mat4::identity();

	gbLight = false;
	// resize
	resize(WIN_WIDTH, WIN_HEIGHT);
}

void display(void)
{
	GLuint currentWidth = 0;
	GLuint currentHeight = 0;

	//code
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// start using OpenGL program object
	glUseProgram(gShaderProgramObject);

	if (gbLight == true)
	{
		glUniform1i(gLKeyPressedUniform, 1);

		glUniform3fv(gLaUniform, 1, lightAmbient);
		//glUniform3fv(gKa1Uniform, 1, material1_ambient);

		glUniform3fv(gLdUniform, 1, lightDiffuse);
		//glUniform3fv(gKd1Uniform, 1, material1_diffuse);

		glUniform3fv(gLsUniform, 1, lightSpecular);
		//glUniform3fv(gKs1Uniform, 1, material1_specular);

		//glUniform1f(gmaterial1_shininess_uniform, material1_shininess);

		//float lightPosition[] = { 0.0f, 0.0f, 2.0f, 1.0f };


		if (gbXaxis == GL_TRUE)
		{
			lightPosition[0] = 0.0f;
			lightPosition[1] = (float)100 * sin(gfangleX_axis);
			lightPosition[2] = (float)100 * cos(gfangleX_axis);
			lightPosition[3] = 1.0f;
		}

		if (gbYaxis == GL_TRUE)
		{
			lightPosition[0] = (float)100 * cos(gfangleY_axis);
			lightPosition[1] = 0.0f;
			lightPosition[2] = (float)100 * sin(gfangleY_axis);
			lightPosition[3] = 1.0f;
		}

		if (gbZaxis == GL_TRUE)
		{
			lightPosition[0] = (float)100 * cos(gfangleZ_axis);
			lightPosition[1] = (float)100 * sin(gfangleZ_axis);
			lightPosition[2] = 0.0f;
			lightPosition[3] = 1.0f;
		}

		glUniform4fv(glight_position_uniform, 1, (GLfloat *)lightPosition);
	}

	else
	{
		glUniform1i(gLKeyPressedUniform, 0);
	}
	
	mat4 modelMatrix = mat4::identity();
	mat4 scaleMatrix = mat4::identity();
	mat4 viewMatrix = mat4::identity();
	modelMatrix = translate(0.0f, 0.0f, -3.0f);
	//modelMatrix = modelMatrix * scale(0.50f, 0.50f, 0.50f);
	glUniformMatrix4fv(gViewMatrixUniform, 1, GL_FALSE, viewMatrix);
	glUniformMatrix4fv(gModelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(gProjectionMatrixUniform, 1, GL_FALSE, gPerspectiveProjectionMatrix);
	
	glBindVertexArray(gVao_sphere);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
	//////////////////1st sphere////////////////////////////////
		glViewport(0, 0, (GLsizei)giWindowWidth / 6, (GLsizei)giWindowHeight / 4);
		glUniform3fv(gKaUniform, 1, material1_ambient);
		glUniform3fv(gKdUniform, 1, material1_diffuse);
		glUniform3fv(gKsUniform, 1, material1_specular);
		glUniform1f(gmaterial_shininess_uniform, material1_shininess);

		// *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
		glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

	////////////////////////////////2nd sphere////////////////////
		currentWidth = currentWidth + giWindowWidth / 6;
		glViewport(currentWidth, 0, (GLsizei)giWindowWidth / 6, (GLsizei)giWindowHeight / 4);
		glUniform3fv(gKaUniform, 1, material2_ambient);
		glUniform3fv(gKdUniform, 1, material2_diffuse);
		glUniform3fv(gKsUniform, 1, material2_specular);
		glUniform1f(gmaterial_shininess_uniform, material2_shininess);
		// *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
		glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);
	
	/////////////////////////////3rd sphere//////////////////////
			currentWidth = currentWidth + giWindowWidth / 6;
			glViewport(currentWidth, 0, (GLsizei)giWindowWidth / 6, (GLsizei)giWindowHeight / 4);
			glUniform3fv(gKaUniform, 1, material3_ambient);
			glUniform3fv(gKdUniform, 1, material3_diffuse);
			glUniform3fv(gKsUniform, 1, material3_specular);
			glUniform1f(gmaterial_shininess_uniform, material3_shininess);
			// *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
			glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);
	
		////////////////////////////////////////////4th sphere/////////////////
			currentWidth = currentWidth + giWindowWidth / 6;
			glViewport(currentWidth, 0, (GLsizei)giWindowWidth / 6, (GLsizei)giWindowHeight / 4);
			glUniform3fv(gKaUniform, 1, material4_ambient);
			glUniform3fv(gKdUniform, 1, material4_diffuse);
			glUniform3fv(gKsUniform, 1, material4_specular);
			glUniform1f(gmaterial_shininess_uniform, material4_shininess);

			// *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
			glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);
		
/////////////////////////////////////5th Sphere///////////////////////////////////
		currentWidth = currentWidth + giWindowWidth / 6;
		glViewport(currentWidth, 0, (GLsizei)giWindowWidth / 6, (GLsizei)giWindowHeight / 4);
		glUniform3fv(gKaUniform, 1, material5_ambient);
		glUniform3fv(gKdUniform, 1, material5_diffuse);
		glUniform3fv(gKsUniform, 1, material5_specular);
		glUniform1f(gmaterial_shininess_uniform, material5_shininess);
		// *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
		glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

		//////////////////////////////////////6th Sphere//////////////////////////////
			currentWidth = currentWidth + giWindowWidth / 6;
			glViewport(currentWidth, 0, (GLsizei)giWindowWidth / 6, (GLsizei)giWindowHeight / 4);
			glUniform3fv(gKaUniform, 1, material6_ambient);
			glUniform3fv(gKdUniform, 1, material6_diffuse);
			glUniform3fv(gKsUniform, 1, material6_specular);
			glUniform1f(gmaterial_shininess_uniform, material6_shininess);
			// *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
			glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

		//////////////////////////////////////////7th Sphere////////////////////////////
			currentHeight = currentHeight + giWindowHeight / 4;
			currentWidth = 0;
			glViewport(0, currentHeight, (GLsizei)giWindowWidth / 6, (GLsizei)giWindowHeight / 4);
			glUniform3fv(gKaUniform, 1, material7_ambient);
			glUniform3fv(gKdUniform, 1, material7_diffuse);
			glUniform3fv(gKsUniform, 1, material7_specular);
			glUniform1f(gmaterial_shininess_uniform, material7_shininess);
			// *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
			glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);
		
		//////////////////////////////////////////8th Sphere//////////////////////////////////////
			currentWidth = currentWidth + giWindowWidth / 6;
			glViewport(currentWidth, currentHeight, (GLsizei)giWindowWidth / 6, (GLsizei)giWindowHeight / 4);
			glUniform3fv(gKaUniform, 1, material8_ambient);
			glUniform3fv(gKdUniform, 1, material8_diffuse);
			glUniform3fv(gKsUniform, 1, material8_specular);
			glUniform1f(gmaterial_shininess_uniform, material8_shininess);
			// *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
			glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);
	
		/////////////////////////////////////9th Sphere///////////////////////////////////
		currentWidth = currentWidth + giWindowWidth / 6;
		glViewport(currentWidth, currentHeight, (GLsizei)giWindowWidth / 6, (GLsizei)giWindowHeight / 4);
		glUniform3fv(gKaUniform, 1, material9_ambient);
		glUniform3fv(gKdUniform, 1, material9_diffuse);
		glUniform3fv(gKsUniform, 1, material9_specular);
		glUniform1f(gmaterial_shininess_uniform, material9_shininess);
		// *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
		glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

		///////////////////////////////////////10th Sphere///////////////////////////////
			currentWidth = currentWidth + giWindowWidth / 6;
			glViewport(currentWidth, currentHeight, (GLsizei)giWindowWidth / 6, (GLsizei)giWindowHeight / 4);
			glUniform3fv(gKaUniform, 1, material10_ambient);
			glUniform3fv(gKdUniform, 1, material10_diffuse);
			glUniform3fv(gKsUniform, 1, material10_specular);
			glUniform1f(gmaterial_shininess_uniform, material10_shininess);
			// *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
			glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);
		
		////////////////////////////////////////11th Sphere///////////////////////////////////
			currentWidth = currentWidth + giWindowWidth / 6;
			glViewport(currentWidth, currentHeight, (GLsizei)giWindowWidth / 6, (GLsizei)giWindowHeight / 4);
			glUniform3fv(gKaUniform, 1, material11_ambient);
			glUniform3fv(gKdUniform, 1, material11_diffuse);
			glUniform3fv(gKsUniform, 1, material11_specular);
			glUniform1f(gmaterial_shininess_uniform, material11_shininess);
			// *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
			glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);
		
		/////////////////////////////////////////////////12th Sphere/////////////////////////////
			currentWidth = currentWidth + giWindowWidth / 6;
			glViewport(currentWidth, currentHeight, (GLsizei)giWindowWidth / 6, (GLsizei)giWindowHeight / 4);
			glUniformMatrix4fv(gModelMatrixUniform, 1, GL_FALSE, modelMatrix);
			glUniform3fv(gKaUniform, 1, material12_ambient);
			glUniform3fv(gKdUniform, 1, material12_diffuse);
			glUniform3fv(gKsUniform, 1, material12_specular);
			glUniform1f(gmaterial_shininess_uniform, material12_shininess);
			// *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
			glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);
		
		/////////////////////////////////////13th Sphere///////////////////////////////////
		currentHeight = currentHeight + giWindowHeight / 4;
		currentWidth = 0;
		glViewport(0, currentHeight, (GLsizei)giWindowWidth / 6, (GLsizei)giWindowHeight / 4);
		glUniform3fv(gKaUniform, 1, material13_ambient);
		glUniform3fv(gKdUniform, 1, material13_diffuse);
		glUniform3fv(gKsUniform, 1, material13_specular);
		glUniform1f(gmaterial_shininess_uniform, material13_shininess);
		// *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
		glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

		/////////////////////////////////////14th Sphere///////////////////////////////////
		currentWidth = currentWidth + giWindowWidth / 6;
		glViewport(currentWidth, currentHeight, (GLsizei)giWindowWidth / 6, (GLsizei)giWindowHeight / 4);
		glUniform3fv(gKaUniform, 1, material14_ambient);
		glUniform3fv(gKdUniform, 1, material14_diffuse);
		glUniform3fv(gKsUniform, 1, material14_specular);
		glUniform1f(gmaterial_shininess_uniform, material14_shininess);
		// *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
		glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

		/////////////////////////////////////15th Sphere///////////////////////////////////
		currentWidth = currentWidth + giWindowWidth / 6;
		glViewport(currentWidth, currentHeight, (GLsizei)giWindowWidth / 6, (GLsizei)giWindowHeight / 4);
		glUniform3fv(gKaUniform, 1, material15_ambient);
		glUniform3fv(gKdUniform, 1, material15_diffuse);
		glUniform3fv(gKsUniform, 1, material15_specular);
		glUniform1f(gmaterial_shininess_uniform, material15_shininess);
		// *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
		glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);
		/////////////////////////////////////16th Sphere///////////////////////////////////
		currentWidth = currentWidth + giWindowWidth / 6;
		glViewport(currentWidth, currentHeight, (GLsizei)giWindowWidth / 6, (GLsizei)giWindowHeight / 4);
		glUniform3fv(gKaUniform, 1, material16_ambient);
		glUniform3fv(gKdUniform, 1, material16_diffuse);
		glUniform3fv(gKsUniform, 1, material16_specular);
		glUniform1f(gmaterial_shininess_uniform, material16_shininess);
		// *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
		glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

		/////////////////////////////////////17th Sphere///////////////////////////////////
		currentWidth = currentWidth + giWindowWidth / 6;
		glViewport(currentWidth, currentHeight, giWindowWidth / 6, giWindowHeight / 4);
		glUniform3fv(gKaUniform, 1, material17_ambient);
		glUniform3fv(gKdUniform, 1, material17_diffuse);
		glUniform3fv(gKsUniform, 1, material17_specular);
		glUniform1f(gmaterial_shininess_uniform, material17_shininess);
		// *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
		glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);
		/////////////////////////////////////18th Sphere///////////////////////////////////
		currentWidth = currentWidth + giWindowWidth / 6;
		glViewport(currentWidth, currentHeight,(GLsizei)giWindowWidth / 6, (GLsizei)giWindowHeight / 4);
		glUniform3fv(gKaUniform, 1, material18_ambient);
		glUniform3fv(gKdUniform, 1, material18_diffuse);
		glUniform3fv(gKsUniform, 1, material18_specular);
		glUniform1f(gmaterial_shininess_uniform, material18_shininess);
		// *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
		glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);
		/////////////////////////////////////19th Sphere///////////////////////////////////
		currentHeight = currentHeight + giWindowHeight / 4;
		currentWidth = 0;
		glViewport(0, currentHeight, (GLsizei)giWindowWidth / 6, (GLsizei)giWindowHeight / 4);
		glUniform3fv(gKaUniform, 1, material19_ambient);
		glUniform3fv(gKdUniform, 1, material19_diffuse);
		glUniform3fv(gKsUniform, 1, material19_specular);
		glUniform1f(gmaterial_shininess_uniform, material19_shininess);
		// *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
		glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

		/////////////////////////////////////20th Sphere///////////////////////////////////
		currentWidth = currentWidth + giWindowWidth / 6;
		glViewport(currentWidth, currentHeight, (GLsizei)giWindowWidth / 6, (GLsizei)giWindowHeight / 4);
		glUniform3fv(gKaUniform, 1, material20_ambient);
		glUniform3fv(gKdUniform, 1, material20_diffuse);
		glUniform3fv(gKsUniform, 1, material20_specular);
		glUniform1f(gmaterial_shininess_uniform, material20_shininess);
		// *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
		glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

		/////////////////////////////////////21th Sphere///////////////////////////////////
		currentWidth = currentWidth + giWindowWidth / 6;
		glViewport(currentWidth, currentHeight, (GLsizei)giWindowWidth / 6, (GLsizei)giWindowHeight / 4);
		glUniform3fv(gKaUniform, 1, material21_ambient);
		glUniform3fv(gKdUniform, 1, material21_diffuse);
		glUniform3fv(gKsUniform, 1, material21_specular);
		glUniform1f(gmaterial_shininess_uniform, material21_shininess);
		// *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
		glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

		/////////////////////////////////////22th Sphere///////////////////////////////////
		currentWidth = currentWidth + giWindowWidth / 6;
		glViewport(currentWidth, currentHeight, (GLsizei)giWindowWidth / 6, (GLsizei)giWindowHeight / 4);
		glUniform3fv(gKaUniform, 1, material22_ambient);
		glUniform3fv(gKdUniform, 1, material22_diffuse);
		glUniform3fv(gKsUniform, 1, material22_specular);
		glUniform1f(gmaterial_shininess_uniform, material22_shininess);
		// *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
		glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

		/////////////////////////////////////23th Sphere///////////////////////////////////
		currentWidth = currentWidth + giWindowWidth / 6;
		glViewport(currentWidth, currentHeight, (GLsizei)giWindowWidth / 6, (GLsizei)giWindowHeight / 4);
		glUniform3fv(gKaUniform, 1, material23_ambient);
		glUniform3fv(gKdUniform, 1, material23_diffuse);
		glUniform3fv(gKsUniform, 1, material23_specular);
		glUniform1f(gmaterial_shininess_uniform, material23_shininess);
		// *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
		glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

		/////////////////////////////////////24th Sphere///////////////////////////////////
		currentWidth = currentWidth + giWindowWidth / 6;
		glViewport(currentWidth, currentHeight, (GLsizei)giWindowWidth / 6, (GLsizei)giWindowHeight / 4);
		glUniform3fv(gKaUniform, 1, material24_ambient);
		glUniform3fv(gKdUniform, 1, material24_diffuse);
		glUniform3fv(gKsUniform, 1, material24_specular);
		glUniform1f(gmaterial_shininess_uniform, material24_shininess);
		// *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
		glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

		// *** unbind vao ***
		glBindVertexArray(0);

	// stop using OpenGL program object
	glUseProgram(0);

	glXSwapBuffers(gpDisplay,gWindow);
}

void resize(int width, int height)
{
	giWindowWidth = width;
	giWindowHeight = height;

	if (height == 0)
		height = 1;
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);
	gPerspectiveProjectionMatrix = vmath::perspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
}


void uninitialize(void)
{
	//code
	
	
	if (gVao_sphere)
	{
		glDeleteVertexArrays(1, &gVao_sphere);
		gVao_sphere = 0;
	}

	// destroy vbo
	if (gVbo_sphere_element)
	{
		glDeleteBuffers(1, &gVbo_sphere_element);
		gVbo_sphere_element = 0;
	}

	if (gVbo_sphere_normal)
	{
		glDeleteBuffers(1, &gVbo_sphere_normal);
		gVbo_sphere_normal = 0;
	}

	if (gVbo_sphere_position)
	{
		glDeleteBuffers(1, &gVbo_sphere_position);
		gVbo_sphere_position = 0;
	}

	// detach vertex shader from shader program object
	glDetachShader(gShaderProgramObject, gVertexShaderObject);
	// detach fragment  shader from shader program object
	glDetachShader(gShaderProgramObject, gFragmentShaderObject);

	// delete vertex shader object
	glDeleteShader(gVertexShaderObject);
	gVertexShaderObject = 0;
	// delete fragment shader object
	glDeleteShader(gFragmentShaderObject);
	gFragmentShaderObject = 0;

	// delete shader program object
	glDeleteProgram(gShaderProgramObject);
	gShaderProgramObject = 0;

	// unlink shader program
	glUseProgram(0);
	
	// Releasing OpenGL related and XWindow related objects 	
	GLXContext currentContext=glXGetCurrentContext();
	if(currentContext!=NULL && currentContext==gGLXContext)
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

	if (gpFile)
	{
		fprintf(gpFile, "Log File Is Successfully Closed.\n");
		fclose(gpFile);
		gpFile = NULL;
	}
}

void spin(void)
{

	gfangleX_axis = gfangleX_axis + 0.001f;

	if (gfangleX_axis >= 360.0f)
	{
		gfangleX_axis = gfangleX_axis - 360.0f;
	}


	gfangleY_axis = gfangleY_axis + 0.001f;

	if (gfangleY_axis >= 360.0f)
	{
		gfangleY_axis = gfangleY_axis - 360.0f;
	}

	gfangleZ_axis = gfangleZ_axis + 0.001f;

	if (gfangleZ_axis >= 360.0f)
	{
		gfangleZ_axis = gfangleZ_axis - 360.0f;
	}

}

