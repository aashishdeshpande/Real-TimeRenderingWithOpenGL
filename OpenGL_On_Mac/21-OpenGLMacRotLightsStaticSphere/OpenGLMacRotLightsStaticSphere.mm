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

GLfloat angle_red = 0.0f;
GLfloat angle_green = 0.0f;
GLfloat angle_blue = 0.0f;

GLfloat lightAmbient0[] = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat lightDiffuse0[] = { 1.0f, 0.0f, 0.0f, 1.0f };
GLfloat lightSpecular0[] = { 1.0f, 0.0f, 0.0f, 1.0f };

GLfloat lightAmbient1[] = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat lightDiffuse1[] = { 0.0f, 1.0f, 0.0f, 1.0f };
GLfloat lightSpecular1[] = { 0.0f, 1.0f, 0.0f, 1.0f };

GLfloat lightAmbient2[] = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat lightDiffuse2[] = { 0.0f, 0.0f, 1.0f, 1.0f };
GLfloat lightSpecular2[] = { 0.0f, 0.0f, 1.0f, 1.0f };

GLfloat material_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat material_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat material_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat material_shininess = 50.0f;

float sphere_textures[764];
float sphere_vertices[1146];
float sphere_normals[1146];
unsigned short sphere_elements[2280];

//bool perVertex = true;
//bool perFragment = false;

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

	[window setTitle:@"macOS PerFragment And PerVertex Toggle"];
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

	GLuint vertexShaderObjectPhongFragment;
	GLuint fragmentShaderObjectPhongFragment;

	GLuint vertexShaderObjectPhongVertex;
	GLuint fragmentShaderObjectPhongVertex;

	GLuint shaderProgramObjectPhongVertex;
	GLuint shaderProgramObjectPhongFragment;


	GLuint vao_sphere;
	GLuint vbo_sphere_position;
	GLuint vbo_sphere_normal;
	GLuint vbo_sphere_element;

	GLuint modelMatrixUniform, viewMatrixUniform, projectionMatrixUniform;
    
    GLuint modelMatrixUniformf, viewMatrixUniformf, projectionMatrixUniformf;
    

	GLuint ld0Uniform;
	GLuint la0Uniform;
	GLuint ls0Uniform;

	GLuint ld1Uniform;
	GLuint la1Uniform;
	GLuint ls1Uniform;

	GLuint ld2Uniform;
	GLuint la2Uniform;
	GLuint ls2Uniform;

	GLuint ka0Uniform;
	GLuint kd0Uniform;
	GLuint ks0Uniform;

	GLuint ka1Uniform;
	GLuint kd1Uniform;
	GLuint ks1Uniform;

	GLuint ka2Uniform;
	GLuint kd2Uniform;
	GLuint ks2Uniform;

	GLuint material0_shininess_uniform;
	GLuint material1_shininess_uniform;
	GLuint material2_shininess_uniform;

	GLuint ld0Uniformf;
	GLuint la0Uniformf;
	GLuint ls0Uniformf;

	GLuint ld1Uniformf;
	GLuint la1Uniformf;
	GLuint ls1Uniformf;

	GLuint ld2Uniformf;
	GLuint la2Uniformf;
	GLuint ls2Uniformf;

	GLuint ka0Uniformf;
	GLuint kd0Uniformf;
	GLuint ks0Uniformf;

	GLuint ka1Uniformf;
	GLuint kd1Uniformf;
	GLuint ks1Uniformf;

	GLuint ka2Uniformf;
	GLuint kd2Uniformf;
	GLuint ks2Uniformf;

	GLuint material0_shininess_uniformf;
	GLuint material1_shininess_uniformf;
	GLuint material2_shininess_uniformf;
	//GLuint gmaterial1_shininess_uniform;

	GLuint light0_position_uniform;
	GLuint light1_position_uniform;
	GLuint light2_position_uniform;

	GLuint light0_position_uniformf;
	GLuint light1_position_uniformf;
	GLuint light2_position_uniformf;

	
    
    vmath::mat4 projectionMatrix;

    bool light;
    
    bool perVertex;
    bool perFragment;
	
    
    GLuint numVertices;
    GLuint numElements;

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

	
	vertexShaderObjectPhongVertex = glCreateShader(GL_VERTEX_SHADER);

	// provide source code to shader
	const GLchar *vertexShaderSourceCodePerVertex =
		"#version 410 core" \
		"\n" \
		"in vec4 vPosition;" \
		"in vec3 vNormal;" \
		"uniform mat4 u_model_matrix;" \
		"uniform mat4 u_view_matrix;" \
		"uniform mat4 u_projection_matrix;" \
		"uniform vec3 u_La0;" \
		"uniform vec3 u_La1;" \
		"uniform vec3 u_La2;" \
		"uniform vec3 u_Ka0;" \
		"uniform vec3 u_Ka1;" \
		"uniform vec3 u_Ka2;" \
		"uniform vec3 u_Ld0;" \
		"uniform vec3 u_Ld1;" \
		"uniform vec3 u_Ld2;" \
		"uniform vec3 u_Kd0;" \
		"uniform vec3 u_Kd1;" \
		"uniform vec3 u_Kd2;" \
		"uniform vec3 u_Ls0;" \
		"uniform vec3 u_Ls1;" \
		"uniform vec3 u_Ls2;" \
		"uniform vec3 u_Ks0;" \
		"uniform vec3 u_Ks1;" \
		"uniform vec3 u_Ks2;" \
		"uniform vec4 u_light0_position;" \
		"uniform vec4 u_light1_position;" \
		"uniform vec4 u_light2_position;" \
		"uniform float u_material0_shininess;" \
		"uniform float u_material1_shininess;" \
		"uniform float u_material2_shininess;" \
		"out vec3 phong_ads_color0;" \
		"out vec3 phong_ads_color1;" \
		"out vec3 phong_ads_color2;" \
		"void main(void)" \
		"{" \
		"vec4 eyeCoordinates = u_view_matrix * u_model_matrix * vPosition;" \

		"vec3 transformed_normals = normalize(mat3(u_view_matrix * u_model_matrix) * vNormal);" \

		"vec3 light_direction0 = normalize(vec3(u_light0_position) - eyeCoordinates.xyz);" \
		"vec3 light_direction1 = normalize(vec3(u_light1_position) - eyeCoordinates.xyz);" \
		"vec3 light_direction2 = normalize(vec3(u_light2_position) - eyeCoordinates.xyz);" \

		"float tn_dot_ld0 = max(dot(transformed_normals, light_direction0), 0.0);" \
		"float tn_dot_ld1 = max(dot(transformed_normals, light_direction1), 0.0);" \
		"float tn_dot_ld2 = max(dot(transformed_normals, light_direction2), 0.0);" \

		"vec3 ambient0 = u_La0 * u_Ka0;" \
		"vec3 diffuse0 = u_Ld0 * u_Kd0 * tn_dot_ld0;" \
		"vec3 reflection_vector0 = reflect(-light_direction0, transformed_normals);" \

		"vec3 ambient1 = u_La1 * u_Ka1;" \
		"vec3 diffuse1 = u_Ld1 * u_Kd1 * tn_dot_ld1;" \
		"vec3 reflection_vector1 = reflect(-light_direction1, transformed_normals);" \

		"vec3 ambient2 = u_La2 * u_Ka2;" \
		"vec3 diffuse2 = u_Ld2 * u_Kd2 * tn_dot_ld2;" \
		"vec3 reflection_vector2 = reflect(-light_direction2, transformed_normals);" \


		"vec3 viewer_vector = normalize(-eyeCoordinates.xyz);"

		"vec3 specular0 = u_Ls0 * u_Ks0 * pow(max(dot(reflection_vector0, viewer_vector), 0.0), u_material0_shininess);" \
		"phong_ads_color0 = ambient0 + diffuse0 + specular0;" \

		"vec3 specular1 = u_Ls1 * u_Ks1 * pow(max(dot(reflection_vector1, viewer_vector), 0.0), u_material1_shininess);" \
		"phong_ads_color1 = ambient1 + diffuse1 + specular1;" \


		"vec3 specular2 = u_Ls2 * u_Ks2 * pow(max(dot(reflection_vector2, viewer_vector), 0.0), u_material2_shininess);" \
		"phong_ads_color2 = ambient2 + diffuse2 + specular2;" \

		"gl_Position = u_projection_matrix * u_view_matrix * u_model_matrix * vPosition;" \
		"}";
	glShaderSource(vertexShaderObjectPhongVertex, 1, (const GLchar **)&vertexShaderSourceCodePerVertex, NULL);

	// compile shader
	glCompileShader(vertexShaderObjectPhongVertex);
	GLint iInfoLogLength = 0;
	GLint iShaderCompiledStatus = 0;
	char *szInfoLog = NULL;
	glGetShaderiv(vertexShaderObjectPhongVertex, GL_COMPILE_STATUS, &iShaderCompiledStatus);
	if (iShaderCompiledStatus == GL_FALSE)
	{
		glGetShaderiv(vertexShaderObjectPhongVertex, GL_INFO_LOG_LENGTH, &iInfoLogLength);
		if (iInfoLogLength > 0)
		{
			szInfoLog = (char *)malloc(iInfoLogLength);
			if (szInfoLog != NULL)
			{
				GLsizei written;
				glGetShaderInfoLog(vertexShaderObjectPhongVertex, iInfoLogLength, &written, szInfoLog);
				fprintf(gpFile, "Vertex Shader Compilation Log : %s\n", szInfoLog);
                free(szInfoLog);
                [self release];
                [NSApp terminate:self];
			}
		}
	}

	// *** FRAGMENT SHADER ***
	// create shader
	fragmentShaderObjectPhongVertex = glCreateShader(GL_FRAGMENT_SHADER);

	// provide source code to shader
	const GLchar *fragmentShaderSourceCodePerVertex =
		"#version 410 core" \
		"\n" \
		"in vec3 phong_ads_color0;" \
		"in vec3 phong_ads_color1;" \
		"in vec3 phong_ads_color2;" \
		"out vec4 FragColor0;" \
		"void main(void)" \
		"{" \
		"FragColor0 = vec4(phong_ads_color0, 1.0) + vec4(phong_ads_color1, 1.0) + vec4(phong_ads_color2, 1.0); " \
		"}";
	glShaderSource(fragmentShaderObjectPhongVertex, 1, (const GLchar **)&fragmentShaderSourceCodePerVertex, NULL);

	// compile shader
	glCompileShader(fragmentShaderObjectPhongVertex);
	glGetShaderiv(fragmentShaderObjectPhongVertex, GL_COMPILE_STATUS, &iShaderCompiledStatus);
	if (iShaderCompiledStatus == GL_FALSE)
	{
		glGetShaderiv(fragmentShaderObjectPhongVertex, GL_INFO_LOG_LENGTH, &iInfoLogLength);
		if (iInfoLogLength > 0)
		{
			szInfoLog = (char *)malloc(iInfoLogLength);
			if (szInfoLog != NULL)
			{
				GLsizei written;
				glGetShaderInfoLog(fragmentShaderObjectPhongVertex, iInfoLogLength, &written, szInfoLog);
				fprintf(gpFile, "Fragment Shader Compilation Log : %s\n", szInfoLog);
                free(szInfoLog);
                [self release];
                [NSApp terminate:self];
			}
		}
	}

	// *** SHADER PROGRAM ***
	// create
	shaderProgramObjectPhongVertex = glCreateProgram();

	// attach vertex shader to shader program
	glAttachShader(shaderProgramObjectPhongVertex, vertexShaderObjectPhongVertex);

	// attach fragment shader to shader program
	glAttachShader(shaderProgramObjectPhongVertex, fragmentShaderObjectPhongVertex);

	// pre-link binding of shader program object with vertex shader position attribute
	glBindAttribLocation(shaderProgramObjectPhongVertex, VDG_ATTRIBUTE_POSITION, "vPosition");
	glBindAttribLocation(shaderProgramObjectPhongVertex, VDG_ATTRIBUTE_NORMAL, "vNormal");

	// link shader
	glLinkProgram(shaderProgramObjectPhongVertex);
	GLint iShaderProgramLinkStatus = 0;
	glGetProgramiv(shaderProgramObjectPhongVertex, GL_LINK_STATUS, &iShaderProgramLinkStatus);
	if (iShaderProgramLinkStatus == GL_FALSE)
	{
		glGetProgramiv(shaderProgramObjectPhongVertex, GL_INFO_LOG_LENGTH, &iInfoLogLength);
		if (iInfoLogLength>0)
		{
			szInfoLog = (char *)malloc(iInfoLogLength);
			if (szInfoLog != NULL)
			{
				GLsizei written;
				glGetProgramInfoLog(shaderProgramObjectPhongVertex, iInfoLogLength, &written, szInfoLog);
				fprintf(gpFile, "Shader Program Link Log : %s\n", szInfoLog);
                free(szInfoLog);
                [self release];
                [NSApp terminate:self];
			}
		}
	}


	// *** VERTEX SHADER ***
	// create shader
	vertexShaderObjectPhongFragment = glCreateShader(GL_VERTEX_SHADER);

	// provide source code to shader
	const GLchar *vertexShaderSourceCode =
		"#version 410 core" \
		"\n" \
		"in vec4 vPosition;" \
		"in vec3 vNormal;" \
		"uniform mat4 u_model_matrix;" \
		"uniform mat4 u_view_matrix;" \
		"uniform mat4 u_projection_matrix;" \
		"uniform vec4 u_light0_position;" \
		"uniform vec4 u_light1_position;" \
		"uniform vec4 u_light2_position;" \
		"out vec3 transformed_normals;" \
		"out vec3 light_direction0;" \
		"out vec3 light_direction1;" \
		"out vec3 light_direction2;" \
		"out vec3 viewer_vector;" \
		"void main(void)" \
		"{" \
		"vec4 eyeCoordinates = u_view_matrix * u_model_matrix * vPosition;" \
		"transformed_normals = normalize(mat3(u_view_matrix * u_model_matrix) * vNormal);" \
		"light_direction0 = normalize(vec3(u_light0_position) - eyeCoordinates.xyz);" \
		"light_direction1 = normalize(vec3(u_light1_position) - eyeCoordinates.xyz);" \
		"light_direction2 = normalize(vec3(u_light2_position) - eyeCoordinates.xyz);" \
		"viewer_vector = normalize(-eyeCoordinates.xyz);"
		"gl_Position = u_projection_matrix * u_view_matrix * u_model_matrix * vPosition;" \
		"}";
	glShaderSource(vertexShaderObjectPhongFragment, 1, (const GLchar **)&vertexShaderSourceCode, NULL);

	// compile shader
	glCompileShader(vertexShaderObjectPhongFragment);
	iInfoLogLength = 0;
	iShaderCompiledStatus = 0;
	szInfoLog = NULL;
	glGetShaderiv(vertexShaderObjectPhongFragment, GL_COMPILE_STATUS, &iShaderCompiledStatus);
	if (iShaderCompiledStatus == GL_FALSE)
	{
		glGetShaderiv(vertexShaderObjectPhongFragment, GL_INFO_LOG_LENGTH, &iInfoLogLength);
		if (iInfoLogLength > 0)
		{
			szInfoLog = (char *)malloc(iInfoLogLength);
			if (szInfoLog != NULL)
			{
				GLsizei written;
				glGetShaderInfoLog(vertexShaderObjectPhongFragment, iInfoLogLength, &written, szInfoLog);
				fprintf(gpFile, "Vertex Shader Compilation Log : %s\n", szInfoLog);
                free(szInfoLog);
                [self release];
                [NSApp terminate:self];
			}
		}
	}

	// *** FRAGMENT SHADER ***
	// create shader
	fragmentShaderObjectPhongFragment = glCreateShader(GL_FRAGMENT_SHADER);

	// provide source code to shader
	const GLchar *fragmentShaderSourceCode =
		"#version 410 core" \
		"\n" \
		"in vec3 transformed_normals;" \
		"in vec3 light_direction0;" \
		"in vec3 light_direction1;" \
		"in vec3 light_direction2;" \
		"in vec3 viewer_vector;" \
		"out vec4 FragColor;" \
		"uniform vec3 u_La0;" \
		"uniform vec3 u_La1;" \
		"uniform vec3 u_La2;" \
		"uniform vec3 u_Ka0;" \
		"uniform vec3 u_Ka1;" \
		"uniform vec3 u_Ka2;" \
		"uniform vec3 u_Ld0;" \
		"uniform vec3 u_Ld1;" \
		"uniform vec3 u_Ld2;" \
		"uniform vec3 u_Kd0;" \
		"uniform vec3 u_Kd1;" \
		"uniform vec3 u_Kd2;" \
		"uniform vec3 u_Ls0;" \
		"uniform vec3 u_Ls1;" \
		"uniform vec3 u_Ls2;" \
		"uniform vec3 u_Ks0;" \
		"uniform vec3 u_Ks1;" \
		"uniform vec3 u_Ks2;" \
		"uniform vec4 u_light0_position;" \
		"uniform vec4 u_light1_position;" \
		"uniform vec4 u_light2_position;" \
		"uniform float u_material0_shininess;" \
		"uniform float u_material1_shininess;" \
		"uniform float u_material2_shininess;" \
		"void main(void)" \
		"{" \
		"vec3 phong_ads_color0;" \
		"vec3 phong_ads_color1;" \
		"vec3 phong_ads_color2;" \
		"vec3 normalized_transformed_normals = normalize(transformed_normals);" \
		"vec3 normalized_light_direction0 = normalize(light_direction0);" \
		"vec3 normalized_light_direction1 = normalize(light_direction1);" \
		"vec3 normalized_light_direction2 = normalize(light_direction2);" \

		"vec3 normalized_viewer_vector1 = normalize(viewer_vector);" \
		"vec3 ambient0 = u_La0 * u_Ka0;" \
		"vec3 ambient1 = u_La1 * u_Ka1;" \
		"vec3 ambient2 = u_La2 * u_Ka2;" \

		"float tn_dot_ld0 = max(dot(normalized_transformed_normals, normalized_light_direction0), 0.0);" \
		"float tn_dot_ld1 = max(dot(normalized_transformed_normals, normalized_light_direction1), 0.0);" \
		"float tn_dot_ld2 = max(dot(normalized_transformed_normals, normalized_light_direction2), 0.0);" \

		"vec3 diffuse0 = u_Ld0 * u_Kd0 * tn_dot_ld0;" \
		"vec3 diffuse1 = u_Ld1 * u_Kd1 * tn_dot_ld1;" \
		"vec3 diffuse2 = u_Ld2 * u_Kd2 * tn_dot_ld2;" \

		"vec3 reflection0_vector = reflect(-normalized_light_direction0, normalized_transformed_normals);" \
		"vec3 reflection1_vector = reflect(-normalized_light_direction1, normalized_transformed_normals);" \
		"vec3 reflection2_vector = reflect(-normalized_light_direction2, normalized_transformed_normals);" \

		"vec3 specular0 = u_Ls0 * u_Ks0 * pow(max(dot(reflection0_vector, normalized_viewer_vector1), 0.0), u_material0_shininess);" \
		"vec3 specular1 = u_Ls1 * u_Ks1 * pow(max(dot(reflection1_vector, normalized_viewer_vector1), 0.0), u_material1_shininess);" \
		"vec3 specular2 = u_Ls2 * u_Ks2 * pow(max(dot(reflection2_vector, normalized_viewer_vector1), 0.0), u_material2_shininess);" \

		"phong_ads_color0 = ambient0 + diffuse0 + specular0;" \
		"phong_ads_color1 = ambient1 + diffuse1 + specular1;" \
		"phong_ads_color2 = ambient2 + diffuse2 + specular2;" \

		"FragColor = vec4(phong_ads_color0, 1.0) + vec4(phong_ads_color1, 1.0) + vec4(phong_ads_color2, 1.0);" \
		"}";
	glShaderSource(fragmentShaderObjectPhongFragment, 1, (const GLchar **)&fragmentShaderSourceCode, NULL);

	// compile shader
	glCompileShader(fragmentShaderObjectPhongFragment);
	glGetShaderiv(fragmentShaderObjectPhongFragment, GL_COMPILE_STATUS, &iShaderCompiledStatus);
	if (iShaderCompiledStatus == GL_FALSE)
	{
		glGetShaderiv(fragmentShaderObjectPhongFragment, GL_INFO_LOG_LENGTH, &iInfoLogLength);
		if (iInfoLogLength > 0)
		{
			szInfoLog = (char *)malloc(iInfoLogLength);
			if (szInfoLog != NULL)
			{
				GLsizei written;
				glGetShaderInfoLog(fragmentShaderObjectPhongFragment, iInfoLogLength, &written, szInfoLog);
				fprintf(gpFile, "Fragment Shader Compilation Log : %s\n", szInfoLog);
                free(szInfoLog);
                [self release];
                [NSApp terminate:self];
			}
		}
	}



	// *** SHADER PROGRAM ***
	// create
	shaderProgramObjectPhongFragment = glCreateProgram();

	// attach vertex shader to shader program
	glAttachShader(shaderProgramObjectPhongFragment, vertexShaderObjectPhongFragment);

	// attach fragment shader to shader program
	glAttachShader(shaderProgramObjectPhongFragment, fragmentShaderObjectPhongFragment);

	// pre-link binding of shader program object with vertex shader position attribute
	//glBindAttribLocation(gShaderProgramObjectPhongVertex, ARD_ATTRIBUTE_POSITION, "vPosition");
	//glBindAttribLocation(gShaderProgramObjectPhongVertex, ARD_ATTRIBUTE_NORMAL, "vNormal");

	glBindAttribLocation(shaderProgramObjectPhongFragment, VDG_ATTRIBUTE_POSITION, "vPosition");
	glBindAttribLocation(shaderProgramObjectPhongFragment, VDG_ATTRIBUTE_NORMAL, "vNormal");

	// link shader
	glLinkProgram(shaderProgramObjectPhongFragment);
	iShaderProgramLinkStatus = 0;
	glGetProgramiv(shaderProgramObjectPhongFragment, GL_LINK_STATUS, &iShaderProgramLinkStatus);
	if (iShaderProgramLinkStatus == GL_FALSE)
	{
		glGetProgramiv(shaderProgramObjectPhongFragment, GL_INFO_LOG_LENGTH, &iInfoLogLength);
		if (iInfoLogLength>0)
		{
			szInfoLog = (char *)malloc(iInfoLogLength);
			if (szInfoLog != NULL)
			{
				GLsizei written;
				glGetProgramInfoLog(shaderProgramObjectPhongFragment, iInfoLogLength, &written, szInfoLog);
				fprintf(gpFile, "Shader Program Link Log : %s\n", szInfoLog);
                free(szInfoLog);
                [self release];
                [NSApp terminate:self];;
			}
		}
	}


	modelMatrixUniform = glGetUniformLocation(shaderProgramObjectPhongVertex, "u_model_matrix");
	viewMatrixUniform = glGetUniformLocation(shaderProgramObjectPhongVertex, "u_view_matrix");
	projectionMatrixUniform = glGetUniformLocation(shaderProgramObjectPhongVertex, "u_projection_matrix");

	
	la1Uniform = glGetUniformLocation(shaderProgramObjectPhongVertex, "u_La1");
	ka1Uniform = glGetUniformLocation(shaderProgramObjectPhongVertex, "u_Ka1");

	ld1Uniform = glGetUniformLocation(shaderProgramObjectPhongVertex, "u_Ld1");
	kd1Uniform = glGetUniformLocation(shaderProgramObjectPhongVertex, "u_Kd1");

	ls1Uniform = glGetUniformLocation(shaderProgramObjectPhongVertex, "u_Ls1");
	ks1Uniform = glGetUniformLocation(shaderProgramObjectPhongVertex, "u_Ks1");

	light1_position_uniform = glGetUniformLocation(shaderProgramObjectPhongVertex, "u_light1_position");

	material1_shininess_uniform = glGetUniformLocation(shaderProgramObjectPhongVertex, "u_material1_shininess");

	la0Uniform = glGetUniformLocation(shaderProgramObjectPhongVertex, "u_La0");
	ka0Uniform = glGetUniformLocation(shaderProgramObjectPhongVertex, "u_Ka0");

	ld0Uniform = glGetUniformLocation(shaderProgramObjectPhongVertex, "u_Ld0");
	kd0Uniform = glGetUniformLocation(shaderProgramObjectPhongVertex, "u_Kd0");

	ls0Uniform = glGetUniformLocation(shaderProgramObjectPhongVertex, "u_Ls0");
	ks0Uniform = glGetUniformLocation(shaderProgramObjectPhongVertex, "u_Ks0");

	light0_position_uniform = glGetUniformLocation(shaderProgramObjectPhongVertex, "u_light0_position");

	material0_shininess_uniform = glGetUniformLocation(shaderProgramObjectPhongVertex, "u_material0_shininess");

	la2Uniform = glGetUniformLocation(shaderProgramObjectPhongVertex, "u_La2");
	ka2Uniform = glGetUniformLocation(shaderProgramObjectPhongVertex, "u_Ka2");

	ld2Uniform = glGetUniformLocation(shaderProgramObjectPhongVertex, "u_Ld2");
	kd2Uniform = glGetUniformLocation(shaderProgramObjectPhongVertex, "u_Kd2");

	ls2Uniform = glGetUniformLocation(shaderProgramObjectPhongVertex, "u_Ls2");
	ks2Uniform = glGetUniformLocation(shaderProgramObjectPhongVertex, "u_Ks2");

	light2_position_uniform = glGetUniformLocation(shaderProgramObjectPhongVertex, "u_light2_position");

	material2_shininess_uniform = glGetUniformLocation(shaderProgramObjectPhongVertex, "u_material2_shininess");


	//uniform for fragment shader

	modelMatrixUniformf = glGetUniformLocation(shaderProgramObjectPhongFragment, "u_model_matrix");
	viewMatrixUniformf = glGetUniformLocation(shaderProgramObjectPhongFragment, "u_view_matrix");
	projectionMatrixUniformf = glGetUniformLocation(shaderProgramObjectPhongFragment, "u_projection_matrix");


	la1Uniformf = glGetUniformLocation(shaderProgramObjectPhongFragment, "u_La1");
	ka1Uniformf = glGetUniformLocation(shaderProgramObjectPhongFragment, "u_Ka1");

	ld1Uniformf = glGetUniformLocation(shaderProgramObjectPhongFragment, "u_Ld1");
	kd1Uniformf = glGetUniformLocation(shaderProgramObjectPhongFragment, "u_Kd1");

	ls1Uniformf = glGetUniformLocation(shaderProgramObjectPhongFragment, "u_Ls1");
	ks1Uniformf = glGetUniformLocation(shaderProgramObjectPhongFragment, "u_Ks1");

	light1_position_uniformf = glGetUniformLocation(shaderProgramObjectPhongFragment, "u_light1_position");

	material1_shininess_uniformf = glGetUniformLocation(shaderProgramObjectPhongFragment, "u_material1_shininess");

	la0Uniformf = glGetUniformLocation(shaderProgramObjectPhongFragment, "u_La0");
	ka0Uniformf = glGetUniformLocation(shaderProgramObjectPhongFragment, "u_Ka0");

	ld0Uniformf = glGetUniformLocation(shaderProgramObjectPhongFragment, "u_Ld0");
	kd0Uniformf = glGetUniformLocation(shaderProgramObjectPhongFragment, "u_Kd0");

	ls0Uniformf = glGetUniformLocation(shaderProgramObjectPhongFragment, "u_Ls0");
	ks0Uniformf = glGetUniformLocation(shaderProgramObjectPhongFragment, "u_Ks0");

	light0_position_uniformf = glGetUniformLocation(shaderProgramObjectPhongFragment, "u_light0_position");

	material0_shininess_uniformf = glGetUniformLocation(shaderProgramObjectPhongFragment, "u_material0_shininess");

	la2Uniformf = glGetUniformLocation(shaderProgramObjectPhongFragment, "u_La2");
	ka2Uniformf = glGetUniformLocation(shaderProgramObjectPhongFragment, "u_Ka2");

	ld2Uniformf = glGetUniformLocation(shaderProgramObjectPhongFragment, "u_Ld2");
	kd2Uniformf = glGetUniformLocation(shaderProgramObjectPhongFragment, "u_Kd2");

	ls2Uniformf = glGetUniformLocation(shaderProgramObjectPhongFragment, "u_Ls2");
	ks2Uniformf = glGetUniformLocation(shaderProgramObjectPhongFragment, "u_Ks2");

	light2_position_uniformf = glGetUniformLocation(shaderProgramObjectPhongFragment, "u_light2_position");

	material2_shininess_uniformf = glGetUniformLocation(shaderProgramObjectPhongFragment, "u_material2_shininess");

	Sphere *sphere = [[Sphere alloc] init];

	[sphere getSphereVertexData:sphere_vertices :sphere_normals :sphere_textures :sphere_elements];
	numVertices = [sphere getNumberOfSphereVertices];
	numElements = [sphere getNumberOfSphereElements];

		// vao
	glGenVertexArrays(1, &vao_sphere);
	glBindVertexArray(vao_sphere);

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
	glGenBuffers(1, &vbo_sphere_element);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element);
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
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
//	MessageBox(NULL, TEXT("Exiting Init"), TEXT("Error"), MB_OK);
	projectionMatrix = vmath::mat4::identity();

    perVertex = true;
    perFragment = false;
    

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
	[[self openGLContext]makeCurrentContext];

	CGLLockContext((CGLContextObj)[[self openGLContext]CGLContextObj]);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GLfloat lightPosition0[4] = {0.0f, (float)2 * sin(angle_red), (float)2 * cos(angle_red), 1.0f};
	GLfloat lightPosition1[4] = { 2 * (float)cos(angle_green), 0.0f, (float)2* sin(angle_green), 1.0 };
	GLfloat lightPosition2[4] = { 2 * (float)cos(angle_blue) , (float)2* sin(angle_blue), 0.0f, 1.0 };

	if (perVertex == true)
	{
		glUseProgram(shaderProgramObjectPhongVertex);
	}

	else
	{
		glUseProgram(shaderProgramObjectPhongFragment);
	}

		glUniform3fv(la0Uniform, 1, lightAmbient0);
		glUniform3fv(ka0Uniform, 1, material_ambient);

		glUniform3fv(ld0Uniform, 1, lightDiffuse0);
		glUniform3fv(kd0Uniform, 1, material_diffuse);

		glUniform3fv(ls0Uniform, 1, lightSpecular0);
		glUniform3fv(ks0Uniform, 1, material_specular);

		glUniform1f(material0_shininess_uniform, material_shininess);

		//float lightPosition[] = { 0.0f, 0.0f, 2.0f, 1.0f };
		glUniform4fv(light0_position_uniform, 1, (GLfloat *)lightPosition0);
	
		glUniform3fv(la1Uniform, 1, lightAmbient1);
		glUniform3fv(ka1Uniform, 1, material_ambient);

		glUniform3fv(ld1Uniform, 1, lightDiffuse1);
		glUniform3fv(kd1Uniform, 1, material_diffuse);

		glUniform3fv(ls1Uniform, 1, lightSpecular1);
		glUniform3fv(ks1Uniform, 1, material_specular);

		glUniform1f(material1_shininess_uniform, material_shininess);

		//float lightPosition[] = { 0.0f, 0.0f, 2.0f, 1.0f };
		glUniform4fv(light1_position_uniform, 1, (GLfloat *)lightPosition1);

		glUniform3fv(la2Uniform, 1, lightAmbient2);
		glUniform3fv(ka2Uniform, 1, material_ambient);

		glUniform3fv(ld2Uniform, 1, lightDiffuse2);
		glUniform3fv(kd2Uniform, 1, material_diffuse);

		glUniform3fv(ls2Uniform, 1, lightSpecular2);
		glUniform3fv(ks2Uniform, 1, material_specular);

		glUniform1f(material2_shininess_uniform, material_shininess);

		//float lightPosition[] = { 0.0f, 0.0f, 2.0f, 1.0f };
		glUniform4fv(light2_position_uniform, 1, (GLfloat *)lightPosition2);

		///////////////////////////////////////////////////////////////////
		glUniform3fv(la0Uniformf, 1, lightAmbient0);
		glUniform3fv(ka0Uniformf, 1, material_ambient);

		glUniform3fv(ld0Uniformf, 1, lightDiffuse0);
		glUniform3fv(kd0Uniformf, 1, material_diffuse);

		glUniform3fv(ls0Uniformf, 1, lightSpecular0);
		glUniform3fv(ks0Uniformf, 1, material_specular);

		glUniform1f(material0_shininess_uniformf, material_shininess);

		//float lightPosition[] = { 0.0f, 0.0f, 2.0f, 1.0f };
		glUniform4fv(light0_position_uniformf, 1, (GLfloat *)lightPosition0);

		glUniform3fv(la1Uniformf, 1, lightAmbient1);
		glUniform3fv(ka1Uniformf, 1, material_ambient);

		glUniform3fv(ld1Uniformf, 1, lightDiffuse1);
		glUniform3fv(kd1Uniformf, 1, material_diffuse);

		glUniform3fv(ls1Uniformf, 1, lightSpecular1);
		glUniform3fv(ks1Uniformf, 1, material_specular);

		glUniform1f(material1_shininess_uniformf, material_shininess);

		//float lightPosition[] = { 0.0f, 0.0f, 2.0f, 1.0f };
		glUniform4fv(light1_position_uniformf, 1, (GLfloat *)lightPosition1);

		glUniform3fv(la2Uniformf, 1, lightAmbient2);
		glUniform3fv(ka2Uniformf, 1, material_ambient);

		glUniform3fv(ld2Uniformf, 1, lightDiffuse2);
		glUniform3fv(kd2Uniformf, 1, material_diffuse);

		glUniform3fv(ls2Uniformf, 1, lightSpecular2);
		glUniform3fv(ks2Uniformf, 1, material_specular);

		glUniform1f(material2_shininess_uniformf, material_shininess);

		//float lightPosition[] = { 0.0f, 0.0f, 2.0f, 1.0f };
		glUniform4fv(light2_position_uniformf, 1, (GLfloat *)lightPosition2);



	vmath::mat4 modelMatrix = vmath::mat4::identity();
	vmath::mat4 viewMatrix = vmath::mat4::identity();

	modelMatrix = vmath::translate(0.0f, 0.0f, -2.0f);

	glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);

	glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, projectionMatrix);

	glBindVertexArray(vao_sphere);

	// *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element);
	glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

	// *** unbind vao ***
	glBindVertexArray(0);

	// stop using OpenGL program object
	glUseProgram(0);

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
	//static bool bIsLKeyPressed = false;
	int key = (int)[[theEvent characters]characterAtIndex:0];

	switch(key)
	{
		case 27:
			[[self window]toggleFullScreen:self];
			break;

		case 'Q':
		case 'q':
			[self release];
			[NSApp terminate:self];
			break;

		case 'F':
		case 'f':
			if (perFragment == false)
			{
				perFragment = true;
				perVertex = false;
			}
			break;

		case 'V':
		case 'v':
			if (perVertex == false)
			{
				perVertex = true;
				perFragment = false;
			}
			break;


		/*case 'L':
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
			break;*/

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
	if(vao_sphere)
	{
		glDeleteVertexArrays(1, &vao_sphere);
		vao_sphere = 0;
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

	if(vbo_sphere_element)
	{
		glDeleteBuffers(1, &vbo_sphere_element);
		vbo_sphere_element = 0;
	}
	// detach vertex shader from shader program object
	glDetachShader(shaderProgramObjectPhongFragment, vertexShaderObjectPhongFragment);
	// detach fragment  shader from shader program object
	glDetachShader(shaderProgramObjectPhongFragment, fragmentShaderObjectPhongFragment);

	glDetachShader(shaderProgramObjectPhongVertex, vertexShaderObjectPhongVertex);
	// detach fragment  shader from shader program object
	glDetachShader(shaderProgramObjectPhongVertex, fragmentShaderObjectPhongVertex);

	// delete vertex shader object
	glDeleteShader(vertexShaderObjectPhongFragment);
	vertexShaderObjectPhongFragment = 0;
	// delete fragment shader object
	glDeleteShader(fragmentShaderObjectPhongFragment);
	fragmentShaderObjectPhongFragment = 0;

	// delete shader program object
	glDeleteProgram(shaderProgramObjectPhongFragment);
	shaderProgramObjectPhongFragment = 0;

	glDeleteShader(vertexShaderObjectPhongVertex);
	vertexShaderObjectPhongVertex = 0;
	// delete fragment shader object
	glDeleteShader(fragmentShaderObjectPhongVertex);
	fragmentShaderObjectPhongVertex = 0;

	// delete shader program object
	glDeleteProgram(shaderProgramObjectPhongVertex);
	shaderProgramObjectPhongVertex = 0;

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


