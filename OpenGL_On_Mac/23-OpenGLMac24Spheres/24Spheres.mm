#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>
#import <QuartzCore/CVDisplayLink.h>
#import <OpenGL/gl3.h>
#import <OpenGL/gl3ext.h>
#import "vmath.h"
#import "Sphere.h"

enum
{
	VDG_ATTRIBUTE_POSITION = 0,
	VDG_ATTRIBUTE_COLOR,
	VDG_ATTRIBUTE_NORMAL,
	VDG_ATTRIBUTE_TEXTURE0,
};

//'C' style global function declarations
CVReturn MyDisplayLinkCallback(CVDisplayLinkRef, const CVTimeStamp *, const CVTimeStamp *, CVOptionFlags, CVOptionFlags *, void *);

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

float sphere_textures[764];
float sphere_vertices[1146];
float sphere_normals[1146];
unsigned short sphere_elements[2280];

bool x_axis = GL_FALSE;
bool y_axis = GL_FALSE;
bool z_axis = GL_FALSE;

GLfloat angleX_axis;
GLfloat angleY_axis;
GLfloat angleZ_axis;

GLuint giWindowWidth = 0;
GLuint giWindowHeight = 0;


FILE *gpFile = NULL;

@interface AppDelegate : NSObject <NSApplicationDelegate, NSWindowDelegate>
@end

@interface GLView : NSOpenGLView
@end

int main(int argc, const char* argv[])
{
	NSAutoreleasePool *pPool = [[NSAutoreleasePool alloc]init];

	NSApp = [NSApplication sharedApplication];

	[NSApp setDelegate : [[AppDelegate alloc] init]];

	[NSApp run];

	[pPool release];

	return(0);
}

@implementation AppDelegate
{
	@private
		NSWindow *window;
		GLView *glView;	
}

-(void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
	//log file
	NSBundle *mainBundle = [NSBundle mainBundle];
	NSString *appDirName = [mainBundle bundlePath];
	NSString *parentDirPath = [appDirName stringByDeletingLastPathComponent];
	NSString *logFileNameWithPath = [NSString stringWithFormat:@"%@/Log.txt", parentDirPath];
	const char *pszLogFileNameWithPath = [logFileNameWithPath cStringUsingEncoding:NSASCIIStringEncoding];
	gpFile = fopen(pszLogFileNameWithPath, "w");
	if(gpFile == NULL)
	{
		printf("Can not Create Log File.\nExiting....\n");
		[self release];
		[NSApp terminate:self];
	}

	fprintf(gpFile, "Program Is Started Successfully\n");

	//window
	NSRect win_rect;
	win_rect = NSMakeRect(0.0, 0.0, 800.0, 600.0);

	window = [[NSWindow alloc]initWithContentRect:win_rect
	 			styleMask:NSWindowStyleMaskTitled | NSWindowStyleMaskClosable | NSWindowStyleMaskMiniaturizable | NSWindowStyleMaskResizable
	 			backing:NSBackingStoreBuffered
	 			defer:NO];

	[window setTitle:@"macOS 24Spheres"];
	[window center];

	glView = [[GLView alloc]initWithFrame:win_rect];

	[window setContentView:glView];
	[window setDelegate:self];
	[window makeKeyAndOrderFront:self];
}

-(void)applicationWillTerminate:(NSNotification *)aNotification
{
	fprintf(gpFile, "Program Is Terminated Successfully\n");

	if(gpFile)
	{
		fclose(gpFile);
		gpFile = NULL;
	}
}

-(void)windowWillClose:(NSNotification *)notification
{
	[NSApp terminate:self];
}

-(void)dealloc
{
	[glView release];

	[window release];

	[super dealloc];
}
@end

@implementation GLView
{
@private
	CVDisplayLinkRef displayLink;

	GLuint vertexShaderObject;
	GLuint fragmentShaderObject;
	GLuint shaderProgramObject;

	GLuint gVao_sphere;
	GLuint vbo_sphere_position;
	GLuint vbo_sphere_normal;
	GLuint gVbo_sphere_element;

	GLuint modelMatrixUniform, viewMatrixUniform, projectionMatrixUniform;

	GLuint ldUniform, gKdUniform, light_position_uniform;

	GLuint laUniform, gKaUniform, lsUniform, gKsUniform, gmaterial_shininess_uniform;

	GLuint lKeyPressedUniform;

	vmath::mat4 projectionMatrix;

	bool light;
    
    
    GLuint gNumVertices;
    GLuint gNumElements;


	/*GLfloat lightAmbient[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	GLfloat lightDiffuse[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat lightSpecular[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat lightPosition[4] = { 100.0f, 100.0f, 100.0f, 1.0f };

	GLfloat material_ambient[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	GLfloat material_diffuse[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat material_specular[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat material_shininess = 50.0f;*/


}

-(id)initWithFrame:(NSRect)frame;
{
	self = [super initWithFrame:frame];

	if(self)
	{
		[[self window]setContentView:self];

		NSOpenGLPixelFormatAttribute attrs[] =
		{
			NSOpenGLPFAOpenGLProfile,
			NSOpenGLProfileVersion4_1Core,

			NSOpenGLPFAScreenMask, CGDisplayIDToOpenGLDisplayMask(kCGDirectMainDisplay),
			NSOpenGLPFANoRecovery,
			NSOpenGLPFAAccelerated,
			NSOpenGLPFAColorSize,24,
			NSOpenGLPFADepthSize,24,
			NSOpenGLPFAAlphaSize,8,
			NSOpenGLPFADoubleBuffer,
			0};

			NSOpenGLPixelFormat *pixelFormat = [[[NSOpenGLPixelFormat alloc]initWithAttributes:attrs]autorelease];

			if(pixelFormat == nil)
			{
				fprintf(gpFile, "No Valid OpenGL Pixel Format Is Available. Exiting....");
				[self release];
				[NSApp terminate:self];
			}

			NSOpenGLContext *glContext = [[[NSOpenGLContext alloc]initWithFormat:pixelFormat shareContext:nil]autorelease];

			[self setPixelFormat:pixelFormat];

			[self setOpenGLContext:glContext];
	}

	return(self);
}

-(CVReturn)getFrameForTime:(const CVTimeStamp *)pOutputTime
{
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc]init];

	[self drawView];

	[pool release];

	return(kCVReturnSuccess);
}

-(void)prepareOpenGL
{
    int getNumberOfSphereVertices(void);
    int getNumberOfSphereElements(void);
    void getSphereVertexData(float spherePositionCoords[], float sphereNormalCoords[], float sphereTexCoords[], unsigned short sphereElements[]);
    
	fprintf(gpFile, "OpenGL Version : %s\n", glGetString(GL_VERSION));
	fprintf(gpFile, "GLSL Version   : %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

	[[self openGLContext]makeCurrentContext];

	GLint swapInt = 1;
	[[self openGLContext]setValues:&swapInt forParameter:NSOpenGLCPSwapInterval];

	/*Vertex shader*/
	vertexShaderObject = glCreateShader(GL_VERTEX_SHADER);

	const GLchar *vertexShaderSourceCode =
    "#version 410 core" \
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
	glShaderSource(vertexShaderObject, 1, (const GLchar **)&vertexShaderSourceCode, NULL);
	glCompileShader(vertexShaderObject);
	GLint iInfoLength = 0;
	GLint iShaderCompiledStatus = 0;
	char *szInfoLog = NULL;
	glGetShaderiv(vertexShaderObject, GL_COMPILE_STATUS, &iShaderCompiledStatus);
	if (iShaderCompiledStatus == GL_FALSE)
	{
		glGetShaderiv(vertexShaderObject, GL_INFO_LOG_LENGTH, &iInfoLength);
			if (iInfoLength > 0)
			{
				szInfoLog = (char *)malloc(iInfoLength);
				if (szInfoLog != NULL)
				{
					GLsizei written;
					glGetShaderInfoLog(vertexShaderObject, iInfoLength, &written, szInfoLog);
					fprintf(gpFile, "Vertex Shader Compilation Log : %s", szInfoLog);
					free(szInfoLog);
					[self release];
					[NSApp terminate:self];
				}
			}
		}


	//FRAGMENT SHADER
	iInfoLength = 0;
	iShaderCompiledStatus = 0;
	szInfoLog = 0;

	fragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER);

	const GLchar *fragmentShaderSourceCode =
    	"#version 410 core" \
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
		"uniform float u_material_shininess;" \
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
		"vec3 specular = u_Ls * u_Ks1 * pow(max(dot(reflection_vector, normalized_viewer_vector), 0.0), u_material_shininess);" \
		"phong_ads_color = ambient + diffuse + specular;" \
		"}"
		"else" \
		"{" \
		"phong_ads_color = vec3(1.0, 1.0, 1.0);" \
		"}"\
		"FragColor = vec4(phong_ads_color, 1.0);" \
		"}";
	glShaderSource(fragmentShaderObject, 1, (const GLchar **)&fragmentShaderSourceCode, NULL);
	glCompileShader(fragmentShaderObject);
	glGetShaderiv(fragmentShaderObject, GL_COMPILE_STATUS, &iShaderCompiledStatus);
	if (iShaderCompiledStatus == GL_FALSE)
	{
		glGetShaderiv(fragmentShaderObject, GL_INFO_LOG_LENGTH, &iInfoLength);
		if (iInfoLength > 0)
		{
			szInfoLog = (char *)malloc(iInfoLength);
			if (szInfoLog != NULL)
			{
				GLsizei written;
				glGetShaderInfoLog(fragmentShaderObject, iInfoLength, &written, szInfoLog);
				fprintf(gpFile, "Fragment Shader Compilation Log : %s\n",szInfoLog);
				free(szInfoLog);
				[self release];
				[NSApp terminate:self];
			}
		}
	}

	//SHADER PROGRAM
	shaderProgramObject = glCreateProgram();
	glAttachShader(shaderProgramObject, vertexShaderObject);
	glAttachShader(shaderProgramObject, fragmentShaderObject);
	glBindAttribLocation(shaderProgramObject, VDG_ATTRIBUTE_POSITION, "vPosition");
	glBindAttribLocation(shaderProgramObject, VDG_ATTRIBUTE_NORMAL, "vNormal");

	glLinkProgram(shaderProgramObject);
	
	GLint iShaderProgramLinkStatus = 0;
	glGetProgramiv(shaderProgramObject, GL_LINK_STATUS, &iShaderProgramLinkStatus);
	if (iShaderProgramLinkStatus == GL_FALSE)
	{
		glGetProgramiv(shaderProgramObject, GL_INFO_LOG_LENGTH, &iInfoLength);
		if (iInfoLength > 0)
		{
			szInfoLog = (char *)malloc(iInfoLength);
			if (szInfoLog != NULL)
			{
				GLsizei written;
				glGetProgramInfoLog(shaderProgramObject, iInfoLength, &written, szInfoLog);
				fprintf(gpFile, "Shader Program Link Log : %s\n", szInfoLog);
				free(szInfoLog);
				[self release];
				[NSApp terminate:self];
			}
		}
	}
	modelMatrixUniform = glGetUniformLocation(shaderProgramObject, "u_model_matrix");
	viewMatrixUniform = glGetUniformLocation(shaderProgramObject, "u_view_matrix");
	projectionMatrixUniform = glGetUniformLocation(shaderProgramObject, "u_projection_matrix");

	lKeyPressedUniform = glGetUniformLocation(shaderProgramObject, "u_lighting_enabled");


	laUniform = glGetUniformLocation(shaderProgramObject, "u_La");
	gKaUniform = glGetUniformLocation(shaderProgramObject, "u_Ka1");

	ldUniform = glGetUniformLocation(shaderProgramObject, "u_Ld");
	gKdUniform = glGetUniformLocation(shaderProgramObject, "u_Kd1");

	lsUniform = glGetUniformLocation(shaderProgramObject, "u_Ls");
	gKsUniform = glGetUniformLocation(shaderProgramObject, "u_Ks1");

	light_position_uniform = glGetUniformLocation(shaderProgramObject, "u_light_position");

	gmaterial_shininess_uniform = glGetUniformLocation(shaderProgramObject, "u_material_shininess");
	
	Sphere *sphere = [[Sphere alloc] init];

	[sphere getSphereVertexData:sphere_vertices :sphere_normals :sphere_textures :sphere_elements];
	gNumVertices = [sphere getNumberOfSphereVertices];
	gNumElements = [sphere getNumberOfSphereElements];

		// vao
	glGenVertexArrays(1, &gVao_sphere);
	glBindVertexArray(gVao_sphere);

	// position vbo
	glGenBuffers(1, &vbo_sphere_position);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_sphere_position);
	glBufferData(GL_ARRAY_BUFFER, sizeof(sphere_vertices), sphere_vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(VDG_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glEnableVertexAttribArray(VDG_ATTRIBUTE_POSITION);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// normal vbo
	glGenBuffers(1, &vbo_sphere_normal);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_sphere_normal);
	glBufferData(GL_ARRAY_BUFFER, sizeof(sphere_normals), sphere_normals, GL_STATIC_DRAW);

	glVertexAttribPointer(VDG_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glEnableVertexAttribArray(VDG_ATTRIBUTE_NORMAL);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// element vbo
	glGenBuffers(1, &gVbo_sphere_element);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(sphere_elements), sphere_elements, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// unbind vao
	glBindVertexArray(0);

	//glShadeModel(GL_SMOOTH);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	//glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	//glEnable(GL_CULL_FACE);
	glClearColor(0.5f, 0.5f, 0.5f, 0.0f);
//	MessageBox(NULL, TEXT("Exiting Init"), TEXT("Error"), MB_OK);
	projectionMatrix = vmath::mat4::identity();

	light = false;
    
    angleX_axis = 0.0f;
    angleY_axis = 0.0f;
    angleZ_axis = 0.0f;

	CVDisplayLinkCreateWithActiveCGDisplays(&displayLink);
	CVDisplayLinkSetOutputCallback(displayLink, &MyDisplayLinkCallback, self);
	CGLContextObj cglContext = (CGLContextObj)[[self openGLContext]CGLContextObj];
	CGLPixelFormatObj cglPixelFormat = (CGLPixelFormatObj)[[self pixelFormat]CGLPixelFormatObj];
	CVDisplayLinkSetCurrentCGDisplayFromOpenGLContext(displayLink, cglContext, cglPixelFormat);
	CVDisplayLinkStart(displayLink);

}

-(void)reshape
{
	CGLLockContext((CGLContextObj)[[self openGLContext]CGLContextObj]);

	NSRect rect = [self bounds];

	GLfloat width = rect.size.width;
	GLfloat height = rect.size.height;

    giWindowWidth = rect.size.width;
	giWindowHeight = rect.size.height;

	if(height == 0)
		height = 1;

	glViewport(0, 0, (GLsizei)width, (GLsizei)height);

	projectionMatrix = vmath::perspective(45.0f, (GLfloat)width/(GLfloat)height, 0.1f, 100.0f);
	CGLUnlockContext((CGLContextObj)[[self openGLContext] CGLContextObj]);	

}

-(void)drawRect:(NSRect)dirtyRect
{
	[self drawView];
}

-(void)drawView
{
	GLuint currentWidth = 0;
	GLuint currentHeight = 0;

	void update();
	[[self openGLContext]makeCurrentContext];

	CGLLockContext((CGLContextObj)[[self openGLContext]CGLContextObj]);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(shaderProgramObject);

	if (light == true)
	{
		glUniform1i(lKeyPressedUniform, 1);

		glUniform3fv(laUniform, 1, lightAmbient);
		//glUniform3fv(gKa1Uniform, 1, material1_ambient);

		glUniform3fv(ldUniform, 1, lightDiffuse);
		//glUniform3fv(gKd1Uniform, 1, material1_diffuse);

		glUniform3fv(lsUniform, 1, lightSpecular);
		//glUniform3fv(gKs1Uniform, 1, material1_specular);

		//glUniform1f(gmaterial1_shininess_uniform, material1_shininess);

		//float lightPosition[] = { 0.0f, 0.0f, 2.0f, 1.0f };


		if (x_axis == GL_TRUE)
		{
			lightPosition[0] = 0.0f;
			lightPosition[1] = (float)100 * sin(angleX_axis);
			lightPosition[2] = (float)100 * cos(angleX_axis);
			lightPosition[3] = 1.0f;
		}

		if (y_axis == GL_TRUE)
		{
			lightPosition[0] = (float)100 * cos(angleY_axis);
			lightPosition[1] = 0.0f;
			lightPosition[2] = (float)100 * sin(angleY_axis);
			lightPosition[3] = 1.0f;
		}

		if (z_axis == GL_TRUE)
		{
			lightPosition[0] = (float)100 * cos(angleZ_axis);
			lightPosition[1] = (float)100 * sin(angleZ_axis);
			lightPosition[2] = 0.0f;
			lightPosition[3] = 1.0f;
		}

		glUniform4fv(light_position_uniform, 1, (GLfloat *)lightPosition);
	}

	else
	{
		glUniform1i(lKeyPressedUniform, 0);

	}

	vmath::mat4 modelMatrix = vmath::mat4::identity();
	vmath::mat4 viewMatrix = vmath::mat4::identity();
	vmath::mat4 scaleMatrix = vmath::mat4::identity();
	modelMatrix = vmath::translate(0.0f, 0.0f, -3.0f);
	glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);
	glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, projectionMatrix);
//////////////////1st sphere////////////////////////////////

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

		update();

	CGLFlushDrawable((CGLContextObj)[[self openGLContext]CGLContextObj]);
	CGLUnlockContext((CGLContextObj)[[self openGLContext]CGLContextObj]);
}

-(BOOL)acceptsFirstResponder
{
	[[self window]makeFirstResponder:self];
	return(YES);
}

-(void)keyDown:(NSEvent *)theEvent
{
	static bool bIsLKeyPressed = false;
	int key = (int)[[theEvent characters]characterAtIndex:0];

	switch(key)
	{
		case 27:
			[self release];
			[NSApp terminate:self];
			break;

		case 'F':
		case 'f':
			[[self window]toggleFullScreen:self];
			break;

		case 'L':
		case 'l':
			if (bIsLKeyPressed == false)
			{
				light = true;
				bIsLKeyPressed = true;
			}
			else
			{
				light = false;
				bIsLKeyPressed = false;
			}
			break;

		case 'X':
		case 'x':
			x_axis = GL_TRUE;
			y_axis = GL_FALSE;
			z_axis = GL_FALSE;
			break;

		case 'Y':
		case 'y':
			y_axis = GL_TRUE;
			x_axis = GL_FALSE;
			z_axis = GL_FALSE;
			break;

		case 'Z':
		case 'z':
			z_axis = GL_TRUE;
			y_axis = GL_FALSE;
			x_axis = GL_FALSE;
			break;


		default:
			break;
	}
}

-(void)mouseDown:(NSEvent *)theEvent
{

}

-(void)mouseDragged:(NSEvent *)theEvent
{

}

-(void)rightMouseDown:(NSEvent *)theEvent
{

}

-(void)dealloc
{
	if(gVao_sphere)
	{
		glDeleteVertexArrays(1, &gVao_sphere);
		gVao_sphere = 0;
	}

	if(vbo_sphere_position)
	{
		glDeleteBuffers(1, &vbo_sphere_position);
		vbo_sphere_position = 0;
	}

	if(vbo_sphere_normal)
	{
		glDeleteBuffers(1, &vbo_sphere_normal);
		vbo_sphere_normal = 0;
	}

	if(gVbo_sphere_element)
	{
		glDeleteBuffers(1, &gVbo_sphere_element);
		gVbo_sphere_element = 0;
	}
	glDetachShader(shaderProgramObject, vertexShaderObject);
	glDetachShader(shaderProgramObject, fragmentShaderObject);
	
	glDeleteShader(vertexShaderObject);
	vertexShaderObject = 0;
	
	glDeleteShader(fragmentShaderObject);
	fragmentShaderObject = 0;

	glDeleteProgram(shaderProgramObject);
	shaderProgramObject = 0;

	CVDisplayLinkStop(displayLink);
	CVDisplayLinkRelease(displayLink);

	[super dealloc];

}
@end

CVReturn MyDisplayLinkCallback(CVDisplayLinkRef displayLink, const CVTimeStamp *pNow, const CVTimeStamp *pOutputTime, CVOptionFlags flagsIn, CVOptionFlags *pFlagsOut, void *pDisplayLinkContext)
{
	CVReturn result = [(GLView *)pDisplayLinkContext getFrameForTime:pOutputTime];
	return(result);
}

void update()
{
	angleX_axis = angleX_axis + 0.1f;
	if (angleX_axis >= 360.0f)
	{
		angleX_axis = angleX_axis - 360.0f;
	}


	angleY_axis = angleY_axis + 0.1f;
	if (angleY_axis >= 360.0f)
	{
		angleY_axis = angleY_axis - 360.0f;
	}

	angleZ_axis = angleZ_axis + 0.1f;
	if (angleZ_axis >= 360.0f)
	{
		angleZ_axis = angleZ_axis - 360.0f;
	}
}

