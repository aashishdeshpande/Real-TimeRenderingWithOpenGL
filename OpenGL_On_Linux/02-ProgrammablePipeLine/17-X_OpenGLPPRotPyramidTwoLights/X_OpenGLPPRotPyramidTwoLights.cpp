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

#define WIN_WIDTH 800
#define WIN_HEIGHT 600

using namespace vmath;

enum {
	VDG_ATTRIBUTE_POSITION = 0,
	VDG_ATTRIBUTE_COLOR,
	VDG_ATTRIBUTE_NORMAL,
	VDG_ATTRIBUTE_TEXTURE0,
};

//global variable declarations
FILE *gpFile = NULL;

GLuint gVertexShaderObject;
GLuint gFragmentShaderObject;
GLuint gShaderProgramObject;

GLuint gVaoPyramid;
//GLuint gVaoCube;
GLuint gVbo_Position;
GLuint gVbo_Normal;
//GLuint gMVPUniform;

float angle_Pyramid = 0.0f;
//float angle_Cube = 0.0f;
GLuint gModelMatrixUniform, gViewMatrixUniform, gProjectionMatrixUniform;

GLuint gLd0Uniform;
GLuint gLa0Uniform;
GLuint gLs0Uniform;

GLuint gLd1Uniform;
GLuint gLa1Uniform;
GLuint gLs1Uniform;

GLuint gKa0Uniform;
GLuint gKd0Uniform;
GLuint gKs0Uniform;

GLuint gKa1Uniform;
GLuint gKd1Uniform;
GLuint gKs1Uniform;

GLuint gmaterial0_shininess_uniform;
GLuint gmaterial1_shininess_uniform;
//GLuint gmaterial1_shininess_uniform;

GLuint glight0_position_uniform;
GLuint glight1_position_uniform;

//bool gbLight;

GLfloat lightAmbient0[] = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat lightDiffuse0[] = { 1.0f, 0.0f, 0.0f, 1.0f };
GLfloat lightSpecular0[] = { 1.0f, 0.0f, 0.0f, 1.0f };
GLfloat lightPosition0[] = { 100.0f, 100.0f, 100.0f, 1.0f };

GLfloat lightAmbient1[] = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat lightDiffuse1[] = { 0.0f, 0.0f, 1.0f, 1.0f };
GLfloat lightSpecular1[] = { 0.0f, 0.0f, 1.0f, 1.0f };
GLfloat lightPosition1[] = { -100.0f, 100.0f, 100.0f, 1.0f };

GLfloat material_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat material_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat material_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat material_shininess = 50.0f;

mat4 gPerspectiveProjectionMatrix;

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
	void update(void);
	void uninitialize(void);
	
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
		
		update();
		display();
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
	
	XStoreName(gpDisplay,gWindow,"OpenGL Window Programmable PipeLine : Rotating Pyramid Two Lights");
	
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

	gVertexShaderObject = glCreateShader(GL_VERTEX_SHADER);

	const GLchar *vertexShaderSourceCode =
		"#version 450 core" \
		"\n" \
		"in vec4 vPosition;" \
		"in vec3 vNormal;" \
		"uniform mat4 u_model_matrix;" \
		"uniform mat4 u_view_matrix;" \
		"uniform mat4 u_projection_matrix;" \
		"uniform vec3 u_La0;" \
		"uniform vec3 u_La1;" \
		"uniform vec3 u_Ka0;" \
		"uniform vec3 u_Ka1;" \
		"uniform vec3 u_Ld0;" \
		"uniform vec3 u_Ld1;" \
		"uniform vec3 u_Kd0;" \
		"uniform vec3 u_Kd1;" \
		"uniform vec3 u_Ls0;" \
		"uniform vec3 u_Ls1;" \
		"uniform vec3 u_Ks0;" \
		"uniform vec3 u_Ks1;" \
		"uniform vec4 u_light0_position;" \
		"uniform vec4 u_light1_position;" \
		"uniform float u_material0_shininess;" \
		"uniform float u_material1_shininess;" \
		"out vec3 phong_ads_color0;" \
		"out vec3 phong_ads_color1;" \
		"void main(void)" \
		"{" \
		"vec4 eyeCoordinates = u_view_matrix * u_model_matrix * vPosition;" \
	
		"vec3 transformed_normals = normalize(mat3(u_view_matrix * u_model_matrix) * vNormal);" \

		"vec3 light_direction0 = normalize(vec3(u_light0_position) - eyeCoordinates.xyz);" \
		"vec3 light_direction1 = normalize(vec3(u_light1_position) - eyeCoordinates.xyz);" \

		"float tn_dot_ld0 = max(dot(transformed_normals, light_direction0), 0.0);" \
		"float tn_dot_ld1 = max(dot(transformed_normals, light_direction1), 0.0);" \

		"vec3 ambient0 = u_La0 * u_Ka0;" \
		"vec3 diffuse0 = u_Ld0 * u_Kd0 * tn_dot_ld0;" \
		"vec3 reflection_vector0 = reflect(-light_direction0, transformed_normals);" \

		"vec3 ambient1 = u_La1 * u_Ka1;" \
		"vec3 diffuse1 = u_Ld1 * u_Kd1 * tn_dot_ld1;" \
		"vec3 reflection_vector1 = reflect(-light_direction1, transformed_normals);" \

		"vec3 viewer_vector = normalize(-eyeCoordinates.xyz);"
	
		"vec3 specular0 = u_Ls0 * u_Ks0 * pow(max(dot(reflection_vector0, viewer_vector), 0.0), u_material0_shininess);" \
		"phong_ads_color0 = ambient0 + diffuse0 + specular0;" \

		"vec3 specular1 = u_Ls1 * u_Ks1 * pow(max(dot(reflection_vector1, viewer_vector), 0.0), u_material1_shininess);" \
		"phong_ads_color1 = ambient1 + diffuse1 + specular1;" \
		
		"gl_Position = u_projection_matrix * u_view_matrix * u_model_matrix * vPosition;" \
		"}";
	glShaderSource(gVertexShaderObject, 1, (const GLchar **)&vertexShaderSourceCode, NULL);
	glCompileShader(gVertexShaderObject);
	GLint iInfoLength = 0;
	GLint iShaderCompiledStatus = 0;
	char *szInfoLog = NULL;
	glGetShaderiv(gVertexShaderObject, GL_COMPILE_STATUS, &iShaderCompiledStatus);
	if (iShaderCompiledStatus == GL_FALSE)
	{
		glGetShaderiv(gVertexShaderObject, GL_INFO_LOG_LENGTH, &iInfoLength);
			if (iInfoLength > 0)
			{
				szInfoLog = (char *)malloc(iInfoLength);
				if (szInfoLog != NULL)
				{
					GLsizei written;
					glGetShaderInfoLog(gVertexShaderObject, iInfoLength, &written, szInfoLog);
					fprintf(gpFile, "Vertex Shader Compilation Log : %s", szInfoLog);
					free(szInfoLog);
					uninitialize();
					exit(0);
				}
			}
		}

	//FRAGMENT SHADER
	gFragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER);

	const GLchar *fragmentShaderSourceCode =
		"#version 450 core" \
		"\n" \
		"in vec3 phong_ads_color0;" \
		"in vec3 phong_ads_color1;" \
		"out vec4 FragColor0;" \
		"void main(void)" \
		"{" \
		"FragColor0 = vec4(phong_ads_color0, 1.0) + vec4(phong_ads_color1, 1.0); " \
		"}";
	glShaderSource(gFragmentShaderObject, 1, (const GLchar **)&fragmentShaderSourceCode, NULL);
	glCompileShader(gFragmentShaderObject);
	glGetShaderiv(gFragmentShaderObject, GL_COMPILE_STATUS, &iShaderCompiledStatus);
	if (iShaderCompiledStatus == GL_FALSE)
	{
		glGetShaderiv(gFragmentShaderObject, GL_INFO_LOG_LENGTH, &iInfoLength);
		if (iInfoLength > 0)
		{
			szInfoLog = (char *)malloc(iInfoLength);
			if (szInfoLog != NULL)
			{
				GLsizei written;
				glGetShaderInfoLog(gFragmentShaderObject, iInfoLength, &written, szInfoLog);
				fprintf(gpFile, "Fragment Shader Compilation Log : %s\n",szInfoLog);
				free(szInfoLog);
				uninitialize();
				exit(0);
			}
		}
	}

	//SHADER PROGRAM
	gShaderProgramObject = glCreateProgram();
	glAttachShader(gShaderProgramObject, gVertexShaderObject);
	glAttachShader(gShaderProgramObject, gFragmentShaderObject);
	glBindAttribLocation(gShaderProgramObject, VDG_ATTRIBUTE_POSITION, "vPosition");
	glBindAttribLocation(gShaderProgramObject, VDG_ATTRIBUTE_NORMAL, "vNormal");
	glLinkProgram(gShaderProgramObject);
	
	GLint iShaderProgramLinkStatus = 0;
	glGetProgramiv(gShaderProgramObject, GL_LINK_STATUS, &iShaderProgramLinkStatus);
	if (iShaderProgramLinkStatus == GL_FALSE)
	{
		glGetProgramiv(gShaderProgramObject, GL_INFO_LOG_LENGTH, &iInfoLength);
		if (iInfoLength > 0)
		{
			szInfoLog = (char *)malloc(iInfoLength);
			if (szInfoLog != NULL)
			{
				GLsizei written;
				glGetProgramInfoLog(gShaderProgramObject, iInfoLength, &written, szInfoLog);
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

	gLa0Uniform = glGetUniformLocation(gShaderProgramObject, "u_La0");
	gKa0Uniform = glGetUniformLocation(gShaderProgramObject, "u_Ka0");

	gLd0Uniform = glGetUniformLocation(gShaderProgramObject, "u_Ld0");
	gKd0Uniform = glGetUniformLocation(gShaderProgramObject, "u_Kd0");

	gLs0Uniform = glGetUniformLocation(gShaderProgramObject, "u_Ls0");
	gKs0Uniform = glGetUniformLocation(gShaderProgramObject, "u_Ks0");


	gLa1Uniform = glGetUniformLocation(gShaderProgramObject, "u_La1");
	gKa1Uniform = glGetUniformLocation(gShaderProgramObject, "u_Ka1");

	gLd1Uniform = glGetUniformLocation(gShaderProgramObject, "u_Ld1");
	gKd1Uniform = glGetUniformLocation(gShaderProgramObject, "u_Kd1");

	gLs1Uniform = glGetUniformLocation(gShaderProgramObject, "u_Ls1");
	gKs1Uniform = glGetUniformLocation(gShaderProgramObject, "u_Ks1");

	glight0_position_uniform = glGetUniformLocation(gShaderProgramObject, "u_light0_position");
	glight1_position_uniform = glGetUniformLocation(gShaderProgramObject, "u_light1_position");

	gmaterial0_shininess_uniform = glGetUniformLocation(gShaderProgramObject, "u_material0_shininess");
	gmaterial1_shininess_uniform = glGetUniformLocation(gShaderProgramObject, "u_material1_shininess");


	const GLfloat PyramidVertices[] =
	{
	0.0f, 1.0f, 0.0f,//apex
	-1.0f, -1.0f, 1.0f,//left corner 
	1.0f, -1.0f, 1.0f,//right corne
					  
	0.0f, 1.0f, 0.0f,//apex
	1.0f, -1.0f, 1.0f,//left corner of right face
	1.0f, -1.0f, -1.0f,//right corner of right face
								  
	0.0f, 1.0f, 0.0f,//apex
	1.0f, -1.0f, -1.0f,//left corner of back face
	-1.0f, -1.0f, -1.0f,//right coner of back face
									
	0.0f, 1.0f, 0.0f,//apex
	-1.0f, -1.0f, -1.0f,//left corner of left face 
	-1.0f, -1.0f, 1.0f//right corner of left face
	};
	
	const GLfloat PyramidNormal[] = {
	0.0f, 0.447214f, 0.894427f,
	0.0f, 0.447214f, 0.894427f,
	0.0f, 0.447214f, 0.894427f,

	0.894427f, 0.447214f, 0.0f,
	0.894427f, 0.447214f, 0.0f,
	0.894427f, 0.447214f, 0.0f,

	0.0f, 0.447214f, -0.894427f,
	0.0f, 0.447214f, -0.894427f,
	0.0f, 0.447214f, -0.894427f,

	-0.894427f, 0.447214f, 0.0f,
	-0.894427f, 0.447214f, 0.0f,
	-0.894427f, 0.447214f, 0.0f
	 };

	glGenVertexArrays(1, &gVaoPyramid);
	glBindVertexArray(gVaoPyramid);

	glGenBuffers(1, &gVbo_Position);
	glBindBuffer(GL_ARRAY_BUFFER, gVbo_Position);
	glBufferData(GL_ARRAY_BUFFER, sizeof(PyramidVertices),PyramidVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(VDG_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(VDG_ATTRIBUTE_POSITION);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	glGenBuffers(1, &gVbo_Normal);
	glBindBuffer(GL_ARRAY_BUFFER, gVbo_Normal);
	glBufferData(GL_ARRAY_BUFFER, sizeof(PyramidNormal), PyramidNormal, GL_STATIC_DRAW);
	glVertexAttribPointer(VDG_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(VDG_ATTRIBUTE_NORMAL);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	glBindVertexArray(0);
	
	
	//code
	glShadeModel(GL_SMOOTH);
	// set-up depth buffer
	glClearDepth(1.0f);
	// enable depth testing
	glEnable(GL_DEPTH_TEST);
	// depth test to do
	glDepthFunc(GL_LEQUAL);
	// set really nice percpective calculations ?
	glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);
	// We will always cull back faces for better performance
	//glEnable(GL_CULL_FACE);

	// set background clearing color
	glClearColor(0.0f,0.0f,0.0f,0.0f); // blue
	// set PerspectiveMatrix to identitu matrix
	gPerspectiveProjectionMatrix = mat4::identity();
	// resize
	resize(WIN_WIDTH, WIN_HEIGHT);
}

void display(void)
{
	//code
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// start using OpenGL program object
	glUseProgram(gShaderProgramObject);


	glUniform3fv(gLa0Uniform, 1, lightAmbient0);
	glUniform3fv(gKa0Uniform, 1, material_ambient);

	glUniform3fv(gLd0Uniform, 1, lightDiffuse0);
	glUniform3fv(gKd0Uniform, 1, material_diffuse);

	glUniform3fv(gLs0Uniform, 1, lightSpecular0);
	glUniform3fv(gKs0Uniform, 1, material_specular);

	glUniform3fv(gLa1Uniform, 1, lightAmbient1);
	glUniform3fv(gKa1Uniform, 1, material_ambient);

	glUniform3fv(gLd1Uniform, 1, lightDiffuse1);
	glUniform3fv(gKd1Uniform, 1, material_diffuse);

	glUniform3fv(gLs1Uniform, 1, lightSpecular1);
	glUniform3fv(gKs1Uniform, 1, material_specular);

	glUniform1f(gmaterial0_shininess_uniform, material_shininess);
	glUniform1f(gmaterial1_shininess_uniform, material_shininess);

		//float lightPosition[] = { 0.0f, 0.0f, 2.0f, 1.0f };
	glUniform4fv(glight0_position_uniform, 1, (GLfloat *)lightPosition0);
	glUniform4fv(glight1_position_uniform, 1, (GLfloat *)lightPosition1);

	mat4 modelMatrix = mat4::identity();
	mat4 viewMatrix = mat4::identity();
	mat4 rotationMatrix = mat4::identity();
	rotationMatrix = vmath::rotate(angle_Pyramid, 0.0f, 1.0f, 0.0f);

	modelMatrix = translate(0.0f, 0.0f, -5.0f);
	modelMatrix = modelMatrix * rotationMatrix;

	glUniformMatrix4fv(gModelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(gViewMatrixUniform, 1, GL_FALSE, viewMatrix);

	glUniformMatrix4fv(gProjectionMatrixUniform, 1, GL_FALSE, gPerspectiveProjectionMatrix);

	glBindVertexArray(gVaoPyramid);
	// *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
	glDrawArrays(GL_TRIANGLES, 0, 12);

	// *** unbind vao ***
	glBindVertexArray(0);

	// stop using OpenGL program object
	glUseProgram(0);

	glXSwapBuffers(gpDisplay,gWindow);
}

void resize(int width, int height)
{
	if (height == 0)
		height = 1;
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);
	gPerspectiveProjectionMatrix = vmath::perspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
}


void uninitialize(void)
{
	//code
	
	
	//Releasing Shader related objects
	if (gVaoPyramid)
	{
		glDeleteVertexArrays(1, &gVaoPyramid);
		gVaoPyramid = 0;
	}

	// destroy vbo
	if (gVbo_Position)
	{
		glDeleteBuffers(1, &gVbo_Position);
		gVbo_Position = 0;
	}

	if (gVbo_Normal)
	{
		glDeleteBuffers(1, &gVbo_Normal);
		gVbo_Normal = 0;
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

void update(void)
{
	angle_Pyramid = angle_Pyramid + 0.05f;
	
	if(angle_Pyramid >= 360.0f)
	angle_Pyramid = angle_Pyramid - 360.0f;

}
