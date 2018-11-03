package com.example.sknaa.OGLESRotPyramidTwoLights;

import android.content.Context;
import android.opengl.GLSurfaceView;
import javax.microedition.khronos.opengles.GL10;
import javax.microedition.khronos.egl.EGLConfig;
import android.opengl.GLES31;
import android.view.MotionEvent;
import android.view.GestureDetector;
import android.view.GestureDetector.OnGestureListener;
import android.view.GestureDetector.OnDoubleTapListener;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;
import java.nio.ShortBuffer;

import android.opengl.Matrix;

public class GLESView extends GLSurfaceView implements GLSurfaceView.Renderer, OnGestureListener, OnDoubleTapListener
{
	private final Context context;

	private GestureDetector gestureDetector;

	private int vertexShaderObject;
	private int fragmentShaderObject;
	private int shaderProgramObject;

	private float angle_pyramid = 0.0f;

	private int[] vao_pyramid = new int[1];
    private int[] vbo_pyramid_position = new int[1];
    private int[] vbo_pyramid_normal = new int[1];
   
    private float light0_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f};
    private float light0_diffuse[] = { 1.0f, 0.0f, 0.0f, 1.0f};
    private float light0_specular[] = { 1.0f, 0.0f, 0.0f, 1.0f};
    private float light0_position[] = { 100.0f, 100.0f, 100.0f, 1.0f};

    private float light1_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f};
    private float light1_diffuse[] = { 0.0f, 0.0f, 1.0f, 1.0f};
    private float light1_specular[] = { 0.0f, 0.0f, 1.0f, 1.0f};
    private float light1_position[] = { -100.0f, 100.0f, 100.0f, 1.0f};

    private float material_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f};
    private float material_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f};
    private float material_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f};
    private float material_shininess = 50.0f;


	private int modelMatrixUniform, viewMatrixUniform, projectionMatrixUniform;
	private int la0Uniform, ld0Uniform, ls0Uniform, la1Uniform, ld1Uniform, ls1Uniform;
	private int light0PositionUniform, light1PositionUniform;
	private int ka0Uniform, kd0Uniform, ks0Uniform, ka1Uniform, kd1Uniform, ks1Uniform;	
	private int material0ShininessUniform, material1ShininessUniform;
	
	private float perspectiveProjectionMatrix[] = new float[16];

	public GLESView(Context drawingContext)
	{
		super(drawingContext);
		context = drawingContext;
		setEGLContextClientVersion(3);
		setRenderer(this);
		setRenderMode(GLSurfaceView.RENDERMODE_WHEN_DIRTY);
		gestureDetector = new GestureDetector(context, this, null, false);
		gestureDetector.setOnDoubleTapListener(this);
	}


@Override
public void onSurfaceCreated(GL10 gl, EGLConfig config)
{
	String glesVersion = gl.glGetString(GL10.GL_VERSION);
	System.out.println("OGLES: OpenGL-ES Version = "+glesVersion);
	String glslVersion = gl.glGetString(GLES31.GL_SHADING_LANGUAGE_VERSION);
	System.out.println("OGLES: GLSL Version = "+glslVersion);
	initialize(gl);
}

@Override
public void onSurfaceChanged(GL10 unused, int width, int height)
{
	resize(width, height);
}

@Override
public void onDrawFrame(GL10 unused)
{
	display();
}

@Override
public boolean onTouchEvent(MotionEvent e)
{
	int eventaction = e.getAction();
	if(!gestureDetector.onTouchEvent(e))
		super.onTouchEvent(e);
	return(true);
}

@Override
public boolean onDoubleTap(MotionEvent e)
{
	return(true);
}

@Override
public boolean onDoubleTapEvent(MotionEvent e)
{
	return(true);
}

@Override
public boolean onSingleTapConfirmed(MotionEvent e)
{
	return(true);
}

@Override
public boolean onDown(MotionEvent e)
{
	return(true);
}

@Override 
public boolean onFling(MotionEvent e1, MotionEvent e2, float velocityX, float velocityY)
{
	return(true);
}

@Override
public void onLongPress(MotionEvent e)
{

}

@Override
public boolean onScroll(MotionEvent e1, MotionEvent e2, float distanceX, float distanceY)
{
	uninitialize();
	System.exit(0);
	return(true);
}

@Override 
public void onShowPress(MotionEvent e)
{

}

@Override
public boolean onSingleTapUp(MotionEvent e)
{
	return(true);
}

private void initialize(GL10 gl)
{
	vertexShaderObject = GLES31.glCreateShader(GLES31.GL_VERTEX_SHADER);
	final String vertexShaderSourceCode = String.format
	(
		"#version 310 es"+
		"\n"+
		"in vec4 vPosition;"+
		"in vec3 vNormal;"+
		"uniform mat4 u_model_matrix;"+
		"uniform mat4 u_view_matrix;"+
		"uniform mat4 u_projection_matrix;"+
		"uniform mediump int u_double_tap;"+
		"uniform vec3 u_La0;"+
		"uniform vec3 u_La1;"+
		"uniform vec3 u_Ld0;"+
		"uniform vec3 u_Ld1;"+
		"uniform vec3 u_Ls0;"+
		"uniform vec3 u_Ls1;"+
		"uniform vec3 u_Ka0;"+
		"uniform vec3 u_Ka1;"+
		"uniform vec3 u_Ks0;"+
		"uniform vec3 u_Ks1;"+
		"uniform vec3 u_Kd0;"+
		"uniform vec3 u_Kd1;"+
		"uniform vec4 u_light0_position;"+
		"uniform vec4 u_light1_position;"+
		"uniform float u_material0_shininess;"+
		"uniform float u_material1_shininess;"+
		"out vec3 phong_ads_color0;"+
		"out vec3 phong_ads_color1;"+
		"void main(void)"+
		"{"+
		"vec4 eyeCoordinates = u_view_matrix * u_model_matrix * vPosition;"+
		"vec3 transformed_normals = normalize(mat3(u_view_matrix * u_model_matrix) * vNormal);"+
		"vec3 light0_direction = normalize(vec3(u_light0_position) - eyeCoordinates.xyz);"+
		"vec3 light1_direction = normalize(vec3(u_light1_position) - eyeCoordinates.xyz);"+
		"float tn_dot_ld0 = max(dot(transformed_normals, light0_direction), 0.0);"+
		"float tn_dot_ld1 = max(dot(transformed_normals, light1_direction), 0.0);"+
		"vec3 ambient0 = u_La0 * u_Ka0;"+
		"vec3 ambient1 = u_La1 * u_Ka1;"+
		"vec3 diffuse0 = u_Ld0 * u_Kd0 * tn_dot_ld0;"+
		"vec3 diffuse1 = u_Ld1 * u_Kd1 * tn_dot_ld1;"+
		"vec3 reflection0_vector = reflect(-light0_direction, transformed_normals);"+
		"vec3 reflection1_vector = reflect(-light1_direction, transformed_normals);"+
		"vec3 viewer_vector = normalize(-eyeCoordinates.xyz);"+
		"vec3 specular0 = u_Ls0 * u_Ks0 * pow(max(dot(reflection0_vector, viewer_vector), 0.0), u_material0_shininess);"+
		"vec3 specular1 = u_Ls1 * u_Ks1 * pow(max(dot(reflection1_vector, viewer_vector), 0.0), u_material1_shininess);"+
		"phong_ads_color0 = ambient0 + diffuse0 + specular0;"+
		"phong_ads_color1 = ambient1 + diffuse1 + specular1;"+
		"gl_Position = u_projection_matrix * u_view_matrix * u_model_matrix * vPosition;"+
		"}"
	);

	GLES31.glShaderSource(vertexShaderObject, vertexShaderSourceCode);
	GLES31.glCompileShader(vertexShaderObject);
	int[] iShaderCompiledStatus = new int[1];
	int[] iInfoLogLength = new int[1];
	String szInfoLog = null;
	GLES31.glGetShaderiv(vertexShaderObject, GLES31.GL_COMPILE_STATUS, iShaderCompiledStatus, 0);
	if(iShaderCompiledStatus[0] == GLES31.GL_FALSE)
	{
		GLES31.glGetShaderiv(vertexShaderObject, GLES31.GL_INFO_LOG_LENGTH, iInfoLogLength, 0);

		if(iInfoLogLength[0] > 0)
		{
			szInfoLog = GLES31.glGetShaderInfoLog(vertexShaderObject);
			System.out.println("OGLES: Vertex Shader Compilation Log = "+szInfoLog);
			uninitialize();
			System.exit(0);
		}
	}	

	fragmentShaderObject = GLES31.glCreateShader(GLES31.GL_FRAGMENT_SHADER);
	final String fragmentShaderSourceCode = String.format
	(
		"#version 310 es"+
		"\n"+
		"precision highp float;"+
		"in vec3 phong_ads_color0;"+
		"in vec3 phong_ads_color1;"+
		"out vec4 FragColor;"+
		"void main(void)"+
		"{"+
		"FragColor = vec4(phong_ads_color0, 1.0) + vec4(phong_ads_color1, 1.0);"+
		"}"
	);

	GLES31.glShaderSource(fragmentShaderObject,fragmentShaderSourceCode);
	GLES31.glCompileShader(fragmentShaderObject);
	iShaderCompiledStatus[0] = 0;
	iInfoLogLength[0] = 0;
	szInfoLog = null;
	GLES31.glGetShaderiv(fragmentShaderObject,GLES31.GL_COMPILE_STATUS,iShaderCompiledStatus, 0);
	if(iShaderCompiledStatus[0] == GLES31.GL_FALSE)
	{
		GLES31.glGetShaderiv(fragmentShaderObject, GLES31.GL_INFO_LOG_LENGTH, iInfoLogLength, 0);
		if(iInfoLogLength[0] > 0)
		{
			szInfoLog = GLES31.glGetShaderInfoLog(fragmentShaderObject);
			System.out.println("OGLES: Fragment Shader Compilation Log = "+szInfoLog);
			uninitialize();
			System.exit(0);
		}
	}

	shaderProgramObject = GLES31.glCreateProgram();
	GLES31.glAttachShader(shaderProgramObject,vertexShaderObject);
	GLES31.glAttachShader(shaderProgramObject,fragmentShaderObject);
	GLES31.glBindAttribLocation(shaderProgramObject,GLESMacros.ARD_ATTRIBUTE_VERTEX,"vPosition");
	GLES31.glBindAttribLocation(shaderProgramObject,GLESMacros.ARD_ATTRIBUTE_NORMAL, "vNormal");
	GLES31.glLinkProgram(shaderProgramObject);
	int[] iShaderProgramLinkStatus = new int[1];
	iInfoLogLength[0] = 0;
	szInfoLog = null;
	GLES31.glGetProgramiv(shaderProgramObject, GLES31.GL_LINK_STATUS, iShaderProgramLinkStatus, 0);
	if(iShaderProgramLinkStatus[0] == GLES31.GL_FALSE)
	{
		GLES31.glGetProgramiv(shaderProgramObject, GLES31.GL_INFO_LOG_LENGTH, iInfoLogLength, 0);
		if(iInfoLogLength[0] > 0)
		{
			szInfoLog = GLES31.glGetProgramInfoLog(shaderProgramObject);
			System.out.println("OGLES: Shader Program Link Log = "+szInfoLog);
			uninitialize();
			System.exit(0);
		}
	}

	modelMatrixUniform = GLES31.glGetUniformLocation(shaderProgramObject, "u_model_matrix");
	viewMatrixUniform = GLES31.glGetUniformLocation(shaderProgramObject, "u_view_matrix");
	projectionMatrixUniform = GLES31.glGetUniformLocation(shaderProgramObject, "u_projection_matrix");

	la0Uniform = GLES31.glGetUniformLocation(shaderProgramObject, "u_La0");
	ka0Uniform = GLES31.glGetUniformLocation(shaderProgramObject, "u_Ka0");

	ld0Uniform = GLES31.glGetUniformLocation(shaderProgramObject, "u_Ld0");
	kd0Uniform = GLES31.glGetUniformLocation(shaderProgramObject, "u_Kd0");

	ls0Uniform = GLES31.glGetUniformLocation(shaderProgramObject, "u_Ls0");
	ks0Uniform = GLES31.glGetUniformLocation(shaderProgramObject, "u_Ks0");

	light0PositionUniform = GLES31.glGetUniformLocation(shaderProgramObject, "u_light0_position");

	material0ShininessUniform = GLES31.glGetUniformLocation(shaderProgramObject, "u_material0_shininess");


	la1Uniform = GLES31.glGetUniformLocation(shaderProgramObject, "u_La1");
	ka1Uniform = GLES31.glGetUniformLocation(shaderProgramObject, "u_Ka1");

	ld1Uniform = GLES31.glGetUniformLocation(shaderProgramObject, "u_Ld1");
	kd1Uniform = GLES31.glGetUniformLocation(shaderProgramObject, "u_Kd1");

	ls1Uniform = GLES31.glGetUniformLocation(shaderProgramObject, "u_Ls1");
	ks1Uniform = GLES31.glGetUniformLocation(shaderProgramObject, "u_Ks1");

	light1PositionUniform = GLES31.glGetUniformLocation(shaderProgramObject, "u_light1_position");

	material1ShininessUniform = GLES31.glGetUniformLocation(shaderProgramObject, "u_material1_shininess");

	final float pyramidVertices[] = new float[]
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

	final float pyramidNormal[] = new float[]
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
	
	 // vao
        GLES31.glGenVertexArrays(1,vao_pyramid,0);
        GLES31.glBindVertexArray(vao_pyramid[0]);
        
        // position vbo
        GLES31.glGenBuffers(1,vbo_pyramid_position,0);
        GLES31.glBindBuffer(GLES31.GL_ARRAY_BUFFER,vbo_pyramid_position[0]);
        
        ByteBuffer byteBuffer=ByteBuffer.allocateDirect(pyramidVertices.length * 4);
        byteBuffer.order(ByteOrder.nativeOrder());
        FloatBuffer verticesBuffer=byteBuffer.asFloatBuffer();
        verticesBuffer.put(pyramidVertices);
        verticesBuffer.position(0);
      
        GLES31.glBufferData(GLES31.GL_ARRAY_BUFFER,
                            pyramidVertices.length * 4,
                            verticesBuffer,
                            GLES31.GL_STATIC_DRAW);
        
        GLES31.glVertexAttribPointer(GLESMacros.ARD_ATTRIBUTE_VERTEX,
                                     3,
                                     GLES31.GL_FLOAT,
                                     false,0,0);
        
        GLES31.glEnableVertexAttribArray(GLESMacros.ARD_ATTRIBUTE_VERTEX);
        
        GLES31.glBindBuffer(GLES31.GL_ARRAY_BUFFER,0);
        
        // normal vbo
        GLES31.glGenBuffers(1,vbo_pyramid_normal,0);
        GLES31.glBindBuffer(GLES31.GL_ARRAY_BUFFER,vbo_pyramid_normal[0]);
        
        byteBuffer=ByteBuffer.allocateDirect(pyramidNormal.length * 4);
        byteBuffer.order(ByteOrder.nativeOrder());
        verticesBuffer=byteBuffer.asFloatBuffer();
        verticesBuffer.put(pyramidNormal);
        verticesBuffer.position(0);
        
        GLES31.glBufferData(GLES31.GL_ARRAY_BUFFER,
                            pyramidNormal.length * 4,
                            verticesBuffer,
                            GLES31.GL_STATIC_DRAW);
        
        GLES31.glVertexAttribPointer(GLESMacros.ARD_ATTRIBUTE_NORMAL,
                                     3,
                                     GLES31.GL_FLOAT,
                                     false,0,0);
        
        GLES31.glEnableVertexAttribArray(GLESMacros.ARD_ATTRIBUTE_NORMAL);
        
        GLES31.glBindBuffer(GLES31.GL_ARRAY_BUFFER,0);
       
        GLES31.glBindVertexArray(0);


	GLES31.glEnable(GLES31.GL_DEPTH_TEST);
	GLES31.glDepthFunc(GLES31.GL_LEQUAL);
	//GLES31.glEnable(GLES31.GL_CULL_FACE);
	GLES31.glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	Matrix.setIdentityM(perspectiveProjectionMatrix, 0);
}	

private void resize(int width, int height)
{
	if(height == 0)
		height = 1;
	GLES31.glViewport(0, 0, width, height);
	Matrix.perspectiveM(perspectiveProjectionMatrix, 0, 45.0f, (float)width / (float)height, 0.1f, 100.0f);
}

public void display()
{
	GLES31.glClear(GLES31.GL_COLOR_BUFFER_BIT | GLES31.GL_DEPTH_BUFFER_BIT);
	GLES31.glUseProgram(shaderProgramObject);

	
		
		GLES31.glUniform3fv(la0Uniform, 1, light0_ambient, 0);
		GLES31.glUniform3fv(ld0Uniform, 1, light0_diffuse, 0);
		GLES31.glUniform3fv(ls0Uniform, 1, light0_specular, 0);
		
		GLES31.glUniform3fv(ka0Uniform, 1, material_ambient, 0);
		GLES31.glUniform3fv(kd0Uniform, 1, material_diffuse, 0);
		GLES31.glUniform3fv(ks0Uniform, 1, material_specular, 0);
		
		GLES31.glUniform1f(material0ShininessUniform, material_shininess);
		GLES31.glUniform4fv(light0PositionUniform, 1, light0_position, 0);

		GLES31.glUniform3fv(la1Uniform, 1, light1_ambient, 0);
		GLES31.glUniform3fv(ld1Uniform, 1, light1_diffuse, 0);
		GLES31.glUniform3fv(ls1Uniform, 1, light1_specular, 0);
		
		GLES31.glUniform3fv(ka1Uniform, 1, material_ambient, 0);
		GLES31.glUniform3fv(kd1Uniform, 1, material_diffuse, 0);
		GLES31.glUniform3fv(ks1Uniform, 1, material_specular, 0);
		
		GLES31.glUniform1f(material1ShininessUniform, material_shininess);
		GLES31.glUniform4fv(light1PositionUniform, 1, light1_position, 0);
	

	float modelMatrix[] = new float[16];
	float viewMatrix[] = new float[16];
	float rotationMatrix[] = new float[16];

	Matrix.setIdentityM(modelMatrix, 0);
	Matrix.setIdentityM(viewMatrix, 0);
	Matrix.setIdentityM(rotationMatrix, 0);
	
	Matrix.translateM(modelMatrix, 0, 0.0f, 0.0f, -5.0f);
	Matrix.rotateM(rotationMatrix, 0, angle_pyramid, 0.0f, 1.0f, 0.0f);
	Matrix.multiplyMM(modelMatrix, 0, modelMatrix, 0, rotationMatrix, 0);
	
	GLES31.glUniformMatrix4fv(modelMatrixUniform, 1, false, modelMatrix, 0);
	GLES31.glUniformMatrix4fv(viewMatrixUniform, 1, false, viewMatrix, 0);
	GLES31.glUniformMatrix4fv(projectionMatrixUniform, 1, false, perspectiveProjectionMatrix, 0);
	GLES31.glBindVertexArray(vao_pyramid[0]);
        
        // *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
    GLES31.glBindBuffer(GLES31.GL_ARRAY_BUFFER, vao_pyramid[0]);
   	GLES31.glDrawArrays(GLES31.GL_TRIANGLES, 0, 12);
        // unbind vao
    GLES31.glBindVertexArray(0);
	GLES31.glUseProgram(0);
	update();
	requestRender();
}

void uninitialize()
{
	

	if(vao_pyramid[0] != 0)
	{
		GLES31.glDeleteVertexArrays(1, vao_pyramid, 0);
		vao_pyramid[0] = 0;
	}

	if(vbo_pyramid_position[0] != 0)
	{
		GLES31.glDeleteVertexArrays(1, vbo_pyramid_position, 0);
		vbo_pyramid_position[0] = 0;
	}

	if(vbo_pyramid_normal[0] != 0)
	{
		GLES31.glDeleteVertexArrays(1, vbo_pyramid_normal, 0);
		vbo_pyramid_normal[0] = 0;
	}

	if(shaderProgramObject != 0)
	{
		if(vertexShaderObject != 0)
		{
			GLES31.glDetachShader(shaderProgramObject, vertexShaderObject);
			GLES31.glDeleteShader(vertexShaderObject);
			vertexShaderObject = 0;
		}

		if(fragmentShaderObject != 0)
		{
			GLES31.glDetachShader(shaderProgramObject, fragmentShaderObject);
			GLES31.glDeleteShader(fragmentShaderObject);
			fragmentShaderObject = 0;
		}
	}

	if(shaderProgramObject != 0)
	{
		GLES31.glDeleteProgram(shaderProgramObject);
		shaderProgramObject = 0;
	}

}

private void update()
{
	angle_pyramid = angle_pyramid + 1.0f;

	if(angle_pyramid >= 360.0f)
	{
		angle_pyramid = 0.0f;
	}

}

}
