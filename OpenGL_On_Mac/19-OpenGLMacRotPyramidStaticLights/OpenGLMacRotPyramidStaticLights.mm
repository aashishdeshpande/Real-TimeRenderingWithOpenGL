#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>
#import <QuartzCore/CVDisplayLink.h>
#import <OpenGL/gl3.h>
#import <OpenGL/gl3ext.h>
#import "vmath.h"

enum
{
	VDG_ATTRIBUTE_POSITION = 0,
	VDG_ATTRIBUTE_COLOR,
	VDG_ATTRIBUTE_NORMAL,
	VDG_ATTRIBUTE_TEXTURE0,
};

//'C' style global function declarations
CVReturn MyDisplayLinkCallback(CVDisplayLinkRef, const CVTimeStamp *, const CVTimeStamp *, CVOptionFlags, CVOptionFlags *, void *);
void update(void);

FILE *gpFile = NULL;

GLfloat angle_Pyramid = 0.0f;


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

	[window setTitle:@"macOS Rotating Pyramid static Lights"];
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

	GLuint vao_pyramid;
	
	GLuint vbo_pyramid_position;
	GLuint vbo_pyramid_texture;
    GLuint vbo_pyramid_normal;
    

    GLuint modelMatrixUniform, viewMatrixUniform, projectionMatrixUniform;
    
    GLuint ld0Uniform;
    GLuint la0Uniform;
    GLuint ls0Uniform;
    
    GLuint ld1Uniform;
    GLuint la1Uniform;
    GLuint ls1Uniform;
    
    GLuint ka0Uniform;
    GLuint kd0Uniform;
    GLuint ks0Uniform;
    
    GLuint ka1Uniform;
    GLuint kd1Uniform;
    GLuint ks1Uniform;
    
    GLuint material0_shininess_uniform;
    GLuint material1_shininess_uniform;
    //GLuint gmaterial1_shininess_uniform;
    
    GLuint light0_position_uniform;
    GLuint light1_position_uniform;

	vmath::mat4 projectionMatrix;
    
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
    "in vec3 phong_ads_color0;" \
    "in vec3 phong_ads_color1;" \
    "out vec4 FragColor0;" \
    "void main(void)" \
    "{" \
    "FragColor0 = vec4(phong_ads_color0, 1.0) + vec4(phong_ads_color1, 1.0); " \
    "}";	glShaderSource(fragmentShaderObject, 1, (const GLchar **)&fragmentShaderSourceCode, NULL);
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
    
    la0Uniform = glGetUniformLocation(shaderProgramObject, "u_La0");
    ka0Uniform = glGetUniformLocation(shaderProgramObject, "u_Ka0");
    
    ld0Uniform = glGetUniformLocation(shaderProgramObject, "u_Ld0");
    kd0Uniform = glGetUniformLocation(shaderProgramObject, "u_Kd0");
    
    ls0Uniform = glGetUniformLocation(shaderProgramObject, "u_Ls0");
    ks0Uniform = glGetUniformLocation(shaderProgramObject, "u_Ks0");
    
    
    la1Uniform = glGetUniformLocation(shaderProgramObject, "u_La1");
    ka1Uniform = glGetUniformLocation(shaderProgramObject, "u_Ka1");
    
    ld1Uniform = glGetUniformLocation(shaderProgramObject, "u_Ld1");
    kd1Uniform = glGetUniformLocation(shaderProgramObject, "u_Kd1");
    
    ls1Uniform = glGetUniformLocation(shaderProgramObject, "u_Ls1");
    ks1Uniform = glGetUniformLocation(shaderProgramObject, "u_Ks1");
    
    light0_position_uniform = glGetUniformLocation(shaderProgramObject, "u_light0_position");
    light1_position_uniform = glGetUniformLocation(shaderProgramObject, "u_light1_position");
    
    material0_shininess_uniform = glGetUniformLocation(shaderProgramObject, "u_material0_shininess");
    material1_shininess_uniform = glGetUniformLocation(shaderProgramObject, "u_material1_shininess");

const GLfloat PyramidVertices[] =
    {
        //FRONT FACE
        0.0f, 1.0f, 0.0f,//apex
        -1.0f, -1.0f, 1.0f,//left corner
        1.0f, -1.0f, 1.0f,//right corne
        
        //RIGHT FACE
        0.0f, 1.0f, 0.0f,//apex
        1.0f, -1.0f, 1.0f,//left corner of right face
        1.0f, -1.0f, -1.0f,//right corner of right face
        
        //BACK FACE
        0.0f, 1.0f, 0.0f,//apex
        1.0f, -1.0f, -1.0f,//left corner of back face
        -1.0f, -1.0f, -1.0f,//right coner of back face
        
        //LEFT FACE
        0.0f, 1.0f, 0.0f,//apex
        -1.0f, -1.0f, -1.0f,//left corner of left face
        -1.0f, -1.0f, 1.0f//right corner of left face
    };
    
    const GLfloat PyramidNormal[] =
    {
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
	glGenVertexArrays(1, &vao_pyramid);
	glBindVertexArray(vao_pyramid);

	glGenBuffers(1, &vbo_pyramid_position);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_pyramid_position);
	glBufferData(GL_ARRAY_BUFFER, sizeof(PyramidVertices), PyramidVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(VDG_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glEnableVertexAttribArray(VDG_ATTRIBUTE_POSITION);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

    
    // normal vbo
    glGenBuffers(1, &vbo_pyramid_normal);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_pyramid_normal);
    glBufferData(GL_ARRAY_BUFFER, sizeof(PyramidNormal), PyramidNormal, GL_STATIC_DRAW);
    
    glVertexAttribPointer(VDG_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    
    glEnableVertexAttribArray(VDG_ATTRIBUTE_NORMAL);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
	
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

    glUseProgram(shaderProgramObject);
    
    
    glUniform3fv(la0Uniform, 1, lightAmbient0);
    glUniform3fv(ka0Uniform, 1, material_ambient);
    
    glUniform3fv(ld0Uniform, 1, lightDiffuse0);
    glUniform3fv(kd0Uniform, 1, material_diffuse);
    
    glUniform3fv(ls0Uniform, 1, lightSpecular0);
    glUniform3fv(ks0Uniform, 1, material_specular);
    
    glUniform3fv(la1Uniform, 1, lightAmbient1);
    glUniform3fv(ka1Uniform, 1, material_ambient);
    
    glUniform3fv(ld1Uniform, 1, lightDiffuse1);
    glUniform3fv(kd1Uniform, 1, material_diffuse);
    
    glUniform3fv(ls1Uniform, 1, lightSpecular1);
    glUniform3fv(ks1Uniform, 1, material_specular);
    
    glUniform1f(material0_shininess_uniform, material_shininess);
    glUniform1f(material1_shininess_uniform, material_shininess);
    
    //float lightPosition[] = { 0.0f, 0.0f, 2.0f, 1.0f };
    glUniform4fv(light0_position_uniform, 1, (GLfloat *)lightPosition0);
    glUniform4fv(light1_position_uniform, 1, (GLfloat *)lightPosition1);
    
    vmath::mat4 modelMatrix = vmath::mat4::identity();
    vmath::mat4 viewMatrix = vmath::mat4::identity();
    vmath::mat4 rotationMatrix = vmath::mat4::identity();
    rotationMatrix = vmath::rotate(angle_Pyramid, 0.0f, 1.0f, 0.0f);
    
    modelMatrix = vmath::translate(0.0f, 0.0f, -5.0f);
    modelMatrix = modelMatrix * rotationMatrix;
    
    glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);
    glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);
    
    glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, projectionMatrix);
    
    glBindVertexArray(vao_pyramid);
    // *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
    glDrawArrays(GL_TRIANGLES, 0, 12);
    
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
	if(vao_pyramid)
	{
		glDeleteVertexArrays(1, &vao_pyramid);
		vao_pyramid = 0;
	}

	
	if(vbo_pyramid_position)
	{
		glDeleteBuffers(1, &vbo_pyramid_position);
		vbo_pyramid_position = 0;
	}

	if(vbo_pyramid_normal)
	{
		glDeleteBuffers(1, &vbo_pyramid_normal);
		vbo_pyramid_normal = 0;
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

void update(void)
{
	angle_Pyramid = angle_Pyramid + 0.5f;
	if (angle_Pyramid >= 360.0f)
		angle_Pyramid = 0.0f;

}
