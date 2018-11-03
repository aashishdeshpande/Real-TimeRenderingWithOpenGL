//
//  GLESView.m
//  PerspectiveTriangle
//
//

#import <OpenGLES/ES3/gl.h>
#import <OpenGLES/ES3/glext.h>

#import "GLESView.h"
#import "Sphere.h"
#import "vmath.h"


enum
{
    VDG_ATTRIBUTE_POSITION = 0,
    VDG_ATTRIBUTE_COLOR,
    VDG_ATTRIBUTE_NORMAL,
    VDG_ATTRIBUTE_TEXTURE0,
};

GLfloat lightAmbient[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat lightDiffuse[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat lightSpecular[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat lightPosition[4] = { 100.0f, 100.0f, 100.0f, 1.0f };

GLfloat material_ambient[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat material_diffuse[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat material_specular[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat material_shininess = 50.0f;

float sphere_textures[764];
float sphere_vertices[1146];
float sphere_normals[1146];
unsigned short sphere_elements[2280];

GLuint singleTap = 0;

@implementation GLESView
{
    EAGLContext *eaglContext;
    
    GLuint defaultFramebuffer;
    GLuint colorRenderbuffer;
    GLuint depthRenderbuffer;
    
    id displayLink;
    NSInteger animationFrameInterval;
    BOOL isAnimating;
    
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
    
    GLuint ldUniform, kdUniform, lightPositionUniform;
    
    GLuint ldUniformf, kdUniformf, lightPositionUniformf;
    
    GLuint laUniform, kaUniform, lsUniform, ksUniform, materialShininessUniform;
    
    GLuint laUniformf, kaUniformf, lsUniformf, ksUniformf, materialShininessUniformf;
    
    GLuint doubleTapUniform, doubleTapUniformf;
    GLuint doubleTap;
    
    vmath::mat4 projectionMatrix;
    
    GLuint numVertices;
    GLuint numElements;
    
}

/*
 // Only override drawRect: if you perform custom drawing.
 // An empty implementation adversely affects performance during animation.
 - (void)drawRect:(CGRect)rect {
 // Drawing code
 }
 */

-(id)initWithFrame:(CGRect)frame;
{
    self = [super initWithFrame:frame];
    
    if(self)
    {
        CAEAGLLayer *eaglLayer = (CAEAGLLayer *)super.layer;
        
        eaglLayer.opaque = YES;
        eaglLayer.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys:[NSNumber numberWithBool:FALSE], kEAGLDrawablePropertyRetainedBacking, kEAGLColorFormatRGBA8, kEAGLDrawablePropertyColorFormat, nil];
        
        eaglContext = [[EAGLContext alloc]initWithAPI:kEAGLRenderingAPIOpenGLES3];
        if(eaglContext == nil)
        {
            [self release];
            return(nil);
        }
        [EAGLContext setCurrentContext:eaglContext];
        
        glGenFramebuffers(1, &defaultFramebuffer);
        glGenRenderbuffers(1, &colorRenderbuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, defaultFramebuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, colorRenderbuffer);
        
        [eaglContext renderbufferStorage:GL_RENDERBUFFER fromDrawable:eaglLayer];
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colorRenderbuffer);
        
        GLint backingWidth;
        GLint backingHeight;
        
        glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &backingWidth);
        glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &backingHeight);
        
        glGenRenderbuffers(1, &depthRenderbuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, depthRenderbuffer);
        glRenderbufferStorage(GL_RENDERBUFFER , GL_DEPTH_COMPONENT16, backingWidth, backingHeight);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderbuffer);
        
        if(glCheckFramebufferStatus(GL_FRAMEBUFFER)!=GL_FRAMEBUFFER_COMPLETE)
        {
            printf("Failed To Create Complete Framebuffer Object %x\n", glCheckFramebufferStatus(GL_FRAMEBUFFER));
            glDeleteFramebuffers(1, &defaultFramebuffer);
            glDeleteFramebuffers(1, &colorRenderbuffer);
            glDeleteFramebuffers(1, &depthRenderbuffer);
            //[self release];
            return(nil);
        }
        printf("Renderer : %s | GL VERSION : %s | GLSL Version : %s\n",glGetString(GL_RENDERER), glGetString(GL_VERSION), glGetString(GL_SHADING_LANGUAGE_VERSION));
        
        isAnimating = NO;
        animationFrameInterval = 60;
        
        //VERTEX SAHDER
        
        vertexShaderObjectPhongVertex = glCreateShader(GL_VERTEX_SHADER);
        
        // provide source code to shader
        const GLchar *vertexShaderSourceCodePerVertex =
        "#version 300 es" \
        "\n" \
        "in vec4 vPosition;" \
        "in vec3 vNormal;" \
        "uniform mat4 u_model_matrix;" \
        "uniform mat4 u_view_matrix;" \
        "uniform mat4 u_projection_matrix;" \
        "uniform mediump int u_double_tap;" \
        "uniform vec3 u_La;" \
        "uniform vec3 u_Ka;" \
        "uniform vec3 u_Ld;" \
        "uniform vec3 u_Kd;" \
        "uniform vec3 u_Ls;" \
        "uniform vec3 u_Ks;" \
        "uniform vec4 u_light_position;" \
        "uniform float u_material_shininess;" \
        "out vec3 phong_ads_color;" \
        "void main(void)" \
        "{" \
        "if(u_double_tap == 1)" \
        "{" \
        "vec4 eyeCoordinates = u_view_matrix * u_model_matrix * vPosition;" \
        "vec3 transformed_normals = normalize(mat3(u_view_matrix * u_model_matrix) * vNormal);" \
        "vec3 light_direction = normalize(vec3(u_light_position) - eyeCoordinates.xyz);" \
        "float tn_dot_ld = max(dot(transformed_normals, light_direction), 0.0);" \
        "vec3 ambient = u_La * u_Ka;" \
        "vec3 diffuse = u_Ld * u_Kd * tn_dot_ld;" \
        "vec3 reflection_vector = reflect(-light_direction, transformed_normals);" \
        "vec3 viewer_vector = normalize(-eyeCoordinates.xyz);"
        "vec3 specular = u_Ls * u_Ks * pow(max(dot(reflection_vector, viewer_vector), 0.0), u_material_shininess);" \
        "phong_ads_color = ambient + diffuse + specular;" \
        "}" \
        "else" \
        "{" \
        "phong_ads_color = vec3(1.0, 1.0, 1.0);" \
        "}" \
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
                    printf("Vertex Shader Compilation Log : %s\n", szInfoLog);
                    free(szInfoLog);
                    [self release];
                }
            }
        }
        
        // *** FRAGMENT SHADER ***
        // create shader
        fragmentShaderObjectPhongVertex = glCreateShader(GL_FRAGMENT_SHADER);
        
        // provide source code to shader
        const GLchar *fragmentShaderSourceCodePerVertex =
        "#version 300 es" \
        "\n" \
        "precision highp float;" \
        "in vec3 phong_ads_color;" \
        "out vec4 FragColor;" \
        "uniform int u_double_tap;" \
        "void main(void)" \
        "{" \
        "if(u_double_tap == 1)" \
        "{" \
        "FragColor = vec4(phong_ads_color, 1.0);" \
        "}" \
        "else" \
        "{" \
        "FragColor = vec4(1.0,1.0,1.0,1.0);" \
        "}" \
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
                    printf("Fragment Shader Compilation Log : %s\n", szInfoLog);
                    free(szInfoLog);
                    [self release];
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
                    printf("Shader Program Link Log : %s\n", szInfoLog);
                    free(szInfoLog);
                    [self release];
                }
            }
        }
        
        
        // *** VERTEX SHADER ***
        // create shader
        vertexShaderObjectPhongFragment = glCreateShader(GL_VERTEX_SHADER);
        
        // provide source code to shader
        const GLchar *vertexShaderSourceCode =
        "#version 300 es" \
        "\n" \
        "in vec4 vPosition;" \
        "in vec3 vNormal;" \
        "uniform mat4 u_model_matrix;" \
        "uniform mat4 u_view_matrix;" \
        "uniform mat4 u_projection_matrix;" \
        "uniform vec4 u_light_position;" \
        "uniform mediump int u_double_tap;" \
        "out vec3 transformed_normals;" \
        "out vec3 light_direction;" \
        "out vec3 viewer_vector;" \
        "void main(void)" \
        "{" \
        "if(u_double_tap == 1)" \
        "{" \
        "vec4 eyeCoordinates = u_view_matrix * u_model_matrix * vPosition;" \
        "transformed_normals = normalize(mat3(u_view_matrix * u_model_matrix) * vNormal);" \
        "light_direction = normalize(vec3(u_light_position) - eyeCoordinates.xyz);" \
        "viewer_vector = normalize(-eyeCoordinates.xyz);"
        "}" \
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
                    printf("Vertex Shader Compilation Log : %s\n", szInfoLog);
                    free(szInfoLog);
                    [self release];
                }
            }
        }
        
        // *** FRAGMENT SHADER ***
        // create shader
        fragmentShaderObjectPhongFragment = glCreateShader(GL_FRAGMENT_SHADER);
        
        // provide source code to shader
        const GLchar *fragmentShaderSourceCode =
        "#version 300 es" \
        "\n" \
        "precision highp float;" \
        "in vec3 transformed_normals;" \
        "in vec3 light_direction;" \
        "in vec3 viewer_vector;" \
        "out vec4 FragColor;" \
        "uniform vec3 u_La;" \
        "uniform vec3 u_Ka;" \
        "uniform vec3 u_Ld;" \
        "uniform vec3 u_Kd;" \
        "uniform vec3 u_Ls;" \
        "uniform vec3 u_Ks;" \
        "uniform float u_material_shininess;" \
        "uniform int u_double_tap;" \
        "void main(void)" \
        "{" \
        "vec3 phong_ads_color;" \
        "if(u_double_tap == 1)" \
        "{" \
        "vec3 normalized_transformed_normals = normalize(transformed_normals);" \
        "vec3 normalized_light_direction = normalize(light_direction);" \
        "vec3 normalized_viewer_vector = normalize(viewer_vector);" \
        "vec3 ambient = u_La * u_Ka;" \
        "float tn_dot_ld = max(dot(normalized_transformed_normals, normalized_light_direction), 0.0);" \
        "vec3 diffuse = u_Ld * u_Kd * tn_dot_ld;" \
        "vec3 reflection_vector = reflect(-normalized_light_direction, normalized_transformed_normals);" \
        "vec3 specular = u_Ls * u_Ks * pow(max(dot(reflection_vector, normalized_viewer_vector), 0.0), u_material_shininess);" \
        "phong_ads_color = ambient + diffuse + specular;" \
        "}"
        "else" \
        "{" \
        "phong_ads_color = vec3(1.0, 1.0, 1.0);" \
        "}"\
        "FragColor = vec4(phong_ads_color, 1.0);" \
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
                    printf("Fragment Shader Compilation Log : %s\n", szInfoLog);
                    free(szInfoLog);
                    [self release];
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
                    printf("Shader Program Link Log : %s\n", szInfoLog);
                    free(szInfoLog);
                    [self release];
                }
            }
        }
        
        
        modelMatrixUniformf = glGetUniformLocation(shaderProgramObjectPhongFragment, "u_model_matrix");
        viewMatrixUniformf = glGetUniformLocation(shaderProgramObjectPhongFragment, "u_view_matrix");
        projectionMatrixUniformf = glGetUniformLocation(shaderProgramObjectPhongFragment, "u_projection_matrix");
        
        doubleTapUniformf = glGetUniformLocation(shaderProgramObjectPhongFragment, "u_double_tap");
        
        
        laUniformf = glGetUniformLocation(shaderProgramObjectPhongFragment, "u_La");
        kaUniformf = glGetUniformLocation(shaderProgramObjectPhongFragment, "u_Ka");
        
        ldUniformf = glGetUniformLocation(shaderProgramObjectPhongFragment, "u_Ld");
        kdUniformf = glGetUniformLocation(shaderProgramObjectPhongFragment, "u_Kd");
        
        lsUniformf = glGetUniformLocation(shaderProgramObjectPhongFragment, "u_Ls");
        ksUniformf = glGetUniformLocation(shaderProgramObjectPhongFragment, "u_Ks");
        
        lightPositionUniformf = glGetUniformLocation(shaderProgramObjectPhongFragment, "u_light_position");
        
        materialShininessUniformf = glGetUniformLocation(shaderProgramObjectPhongFragment, "u_material_shininess");
        
        
        modelMatrixUniform = glGetUniformLocation(shaderProgramObjectPhongVertex, "u_model_matrix");
        viewMatrixUniform = glGetUniformLocation(shaderProgramObjectPhongVertex, "u_view_matrix");
        projectionMatrixUniform = glGetUniformLocation(shaderProgramObjectPhongVertex, "u_projection_matrix");
        
        doubleTapUniform = glGetUniformLocation(shaderProgramObjectPhongVertex, "u_double_tap");
        
        
        laUniform = glGetUniformLocation(shaderProgramObjectPhongVertex, "u_La");
        kaUniform = glGetUniformLocation(shaderProgramObjectPhongVertex, "u_Ka");
        
        ldUniform = glGetUniformLocation(shaderProgramObjectPhongVertex, "u_Ld");
        kdUniform = glGetUniformLocation(shaderProgramObjectPhongVertex, "u_Kd");
        
        lsUniform = glGetUniformLocation(shaderProgramObjectPhongVertex, "u_Ls");
        ksUniform = glGetUniformLocation(shaderProgramObjectPhongVertex, "u_Ks");
        
        lightPositionUniform = glGetUniformLocation(shaderProgramObjectPhongVertex, "u_light_position");
        
        materialShininessUniform = glGetUniformLocation(shaderProgramObjectPhongVertex, "u_material_shininess");
        

        
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
        glClearDepthf(1.0f);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        glEnable(GL_CULL_FACE);
        projectionMatrix = vmath::mat4::identity();
        
        doubleTap = 0;
        
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        
        UITapGestureRecognizer *singleTapGestureRecognizer = [[UITapGestureRecognizer alloc]initWithTarget:self action:@selector(onSingleTap:)];
        [singleTapGestureRecognizer setNumberOfTapsRequired:1];
        [singleTapGestureRecognizer setNumberOfTouchesRequired:1];
        [singleTapGestureRecognizer setDelegate:self];
        [self addGestureRecognizer:singleTapGestureRecognizer];
        
        UITapGestureRecognizer *doubleTapGestureRecognizer = [[UITapGestureRecognizer alloc]initWithTarget:self action:@selector(onDoubleTap:)];
        [doubleTapGestureRecognizer setNumberOfTapsRequired:2];
        [doubleTapGestureRecognizer setNumberOfTouchesRequired:1];
        [doubleTapGestureRecognizer setDelegate:self];
        [self addGestureRecognizer:doubleTapGestureRecognizer];
        
        //this will allow to differentiate between single tap and double tap
        [singleTapGestureRecognizer requireGestureRecognizerToFail:doubleTapGestureRecognizer];
        
        //swipe gesture
        UISwipeGestureRecognizer *swipeGestureRecognizer = [[UISwipeGestureRecognizer alloc]initWithTarget:self action:@selector(onSwipe:)];
        [self addGestureRecognizer:swipeGestureRecognizer];
        
        //long press gesture
        UILongPressGestureRecognizer *longPressGestureRecognizer = [[UILongPressGestureRecognizer alloc]initWithTarget:self action:@selector(onLongPress:)];
        [self addGestureRecognizer:longPressGestureRecognizer];
        
    }
    return(self);
}

-(void)drawRect:(CGRect)rect
{
    
}

+(Class)layerClass
{
    return([CAEAGLLayer class]);
}

-(void)drawView:(id)sender
{
    [EAGLContext setCurrentContext:eaglContext];
    
    glBindFramebuffer(GL_FRAMEBUFFER, defaultFramebuffer);
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    if (singleTap == 0)
    {
        glUseProgram(shaderProgramObjectPhongVertex);
        
        if (doubleTap == 1)
        {
            glUniform1i(doubleTapUniform, 1);
            
            glUniform3fv(laUniform, 1, lightAmbient);
            glUniform3fv(kaUniform, 1, material_ambient);
            
            glUniform3fv(ldUniform, 1, lightDiffuse);
            glUniform3fv(kdUniform, 1, material_diffuse);
            
            glUniform3fv(lsUniform, 1, lightSpecular);
            glUniform3fv(ksUniform, 1, material_specular);
            
            glUniform1f(materialShininessUniform, material_shininess);
            
            //float lightPosition[] = { 0.0f, 0.0f, 2.0f, 1.0f };
            glUniform4fv(lightPositionUniform, 1, (GLfloat *)lightPosition);
        }
        
        else
        {
            glUniform1i(doubleTapUniform, 0);
        }
        
        vmath::mat4 modelMatrix = vmath::mat4::identity();
        vmath::mat4 viewMatrix = vmath::mat4::identity();
        
        modelMatrix = vmath::translate(0.0f, 0.0f, -2.0f);
        
        glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);
        glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);
        
        glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, projectionMatrix);
        
        
        glBindVertexArray(vao_sphere);
        
        // *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element);
        glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_SHORT, 0);
        
        // *** unbind vao ***
        glBindVertexArray(0);
        
        // stop using OpenGL program object
        glUseProgram(0);
        
    }
    
    else
    {
        glUseProgram(shaderProgramObjectPhongFragment);
        
        if (doubleTap == 1)
        {
            glUniform1i(doubleTapUniformf, 1);
            
            glUniform3fv(laUniformf, 1, lightAmbient);
            glUniform3fv(kaUniformf, 1, material_ambient);
            
            glUniform3fv(ldUniformf, 1, lightDiffuse);
            glUniform3fv(kdUniformf, 1, material_diffuse);
            
            glUniform3fv(lsUniformf, 1, lightSpecular);
            glUniform3fv(ksUniformf, 1, material_specular);
            
            glUniform1f(materialShininessUniformf, material_shininess);
            
            //float lightPosition[] = { 0.0f, 0.0f, 2.0f, 1.0f };
            glUniform4fv(lightPositionUniformf, 1, (GLfloat *)lightPosition);
        }
        
        else
        {
            glUniform1i(doubleTapUniformf, 0);
        }
        
        vmath::mat4 modelMatrix = vmath::mat4::identity();
        vmath::mat4 viewMatrix = vmath::mat4::identity();
        
        modelMatrix = vmath::translate(0.0f, 0.0f, -2.0f);
        
        glUniformMatrix4fv(modelMatrixUniformf, 1, GL_FALSE, modelMatrix);
        glUniformMatrix4fv(viewMatrixUniformf, 1, GL_FALSE, viewMatrix);
        
        glUniformMatrix4fv(projectionMatrixUniformf, 1, GL_FALSE, projectionMatrix);
        
        
        glBindVertexArray(vao_sphere);
        
        // *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element);
        glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_SHORT, 0);
        
        // *** unbind vao ***
        glBindVertexArray(0);
        
        // stop using OpenGL program object
        glUseProgram(0);
    }
    glBindRenderbuffer(GL_RENDERBUFFER, colorRenderbuffer);
    [eaglContext presentRenderbuffer:GL_RENDERBUFFER];
}

-(void)layoutSubviews
{
    GLint width;
    GLint height;
    
    glBindRenderbuffer(GL_RENDERBUFFER, colorRenderbuffer);
    [eaglContext renderbufferStorage:GL_RENDERBUFFER fromDrawable:(CAEAGLLayer *)self.layer];
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &width);
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &height);
    glGenRenderbuffers(1, &depthRenderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthRenderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16 , width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderbuffer);
    
    glViewport(0, 0, width, height);
    
    GLfloat perWidth = (GLfloat)width;
    GLfloat perHeight = (GLfloat)height;
    
    projectionMatrix = vmath::perspective(45.0f, (GLfloat)perWidth / (GLfloat)perHeight, 0.1f, 100.0f);
    
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER)!=GL_FRAMEBUFFER_COMPLETE)
    {
        printf("Failed To Create Complete Framebuffer Object %x", glCheckFramebufferStatus(GL_FRAMEBUFFER));
    }
    [self drawView:nil];
}

-(void)startAnimation
{
    if(!isAnimating)
    {
        displayLink = [NSClassFromString(@"CADisplayLink")displayLinkWithTarget:self selector:@selector(drawView:)];
        [displayLink setPreferredFramesPerSecond:animationFrameInterval];
        [displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
        isAnimating = YES;
    }
}

-(void)stopAnimation
{
    if(isAnimating)
    {
        [displayLink invalidate];
        displayLink = nil;
        
        isAnimating = NO;
        
    }
}

-(BOOL)acceptsFirstResponder
{
    return(YES);
}

-(void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
    
}

-(void)onSingleTap:(UITapGestureRecognizer *)gr
{
    singleTap++;
    if(singleTap > 1)
        singleTap = 0;
}

-(void)onDoubleTap:(UITapGestureRecognizer *)gr
{
    doubleTap++;
    if(doubleTap > 1)
        doubleTap = 0;
}

-(void)onSwipe:(UISwipeGestureRecognizer *)gr
{
    [self release];
    exit(0);
}

-(void)onLongPress:(UILongPressGestureRecognizer *)gr
{
    
}

-(void)dealloc
{
    if(depthRenderbuffer)
    {
        glDeleteRenderbuffers(1, &depthRenderbuffer);
        depthRenderbuffer = 0;
    }
    
    if(colorRenderbuffer)
    {
        glDeleteRenderbuffers(1, &colorRenderbuffer);
        colorRenderbuffer = 0;
    }
    
    if(defaultFramebuffer)
    {
        glDeleteFramebuffers(1, &defaultFramebuffer);
        defaultFramebuffer = 0;
    }
    
    if([EAGLContext currentContext]==eaglContext)
    {
        [EAGLContext setCurrentContext:nil];
    }
    
    [eaglContext release];
    eaglContext = nil;
    
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
    
    [super dealloc];
}

@end







