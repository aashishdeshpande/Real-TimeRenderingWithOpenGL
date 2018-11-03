package com.example.sknaa.OGLESDiffuseLighting;

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

import android.opengl.Matrix;

public class GLESView extends GLSurfaceView implements GLSurfaceView.Renderer, OnGestureListener, OnDoubleTapListener
{
	private final Context context;

	private GestureDetector gestureDetector;

	private int vertexShaderObject;
	private int fragmentShaderObject;
	private int shaderProgramObject;

	private float angle_pyramid = 0.0f;
	private float angle_cube = 0.0f;

	private int[] vao_cube = new int[1];
	private int[] vbo_cube_position = new int[1];
	private int[] vbo_cube_normal = new int[1];
	private int modelViewMatrixUniform, projectionMatrixUniform;
	private int ldUniform, kdUniform, lightPositionUniform;

	private int doubleTapUniform;

	private float perspectiveProjectionMatrix[] = new float[16];

	private float angleCube = 0.0f;

	private int singleTap;
	private int doubleTap;

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
	doubleTap++;
	if(doubleTap > 1)
		doubleTap = 0;
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
	singleTap++;
	if(singleTap > 1)
		singleTap = 0;
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
		"uniform mat4 u_model_view_matrix;"+
		"uniform mat4 u_projection_matrix;"+
		"uniform mediump int u_double_tap;"+
		"uniform vec3 u_Ld;"+
		"uniform vec3 u_Kd;"+
		"uniform vec4 u_light_position;"+
		"out vec3 diffuse_light;"+
		"void main(void)"+
		"{"+
		"if(u_double_tap == 1)"+
		"{"+
		"vec4 eyeCoordinates = u_model_view_matrix * vPosition;"+
		"vec3 tnorm = normalize(mat3(u_model_view_matrix) * vNormal);"+
		"vec3 s = normalize(vec3(u_light_position - eyeCoordinates));"+
		"diffuse_light = u_Ld * u_Kd * max(dot(s, tnorm), 0.0);"+
		"}"+
		"gl_Position = u_projection_matrix * u_model_view_matrix * vPosition;"+
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
		"in vec3 diffuse_light;"+
		"out vec4 FragColor;"+
		"uniform int u_double_tap;"+
		"void main(void)"+
		"{"+
		"vec4 color;"+
		"if(u_double_tap == 1)"+
		"{"+
		"color = vec4(diffuse_light, 1.0);"+
		"}"+
		"else"+
		"{"+
		"color = vec4(1.0 ,1.0, 1.0, 1.0);"+
		"}"+
		"FragColor = color;"+
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
	GLES31.glBindAttribLocation(shaderProgramObject,GLESMacros.ARD_ATTRIBUTE_COLOR, "vNormal");
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

	modelViewMatrixUniform = GLES31.glGetUniformLocation(shaderProgramObject, "u_model_view_matrix");
	projectionMatrixUniform = GLES31.glGetUniformLocation(shaderProgramObject, "u_projection_matrix");
	doubleTapUniform = GLES31.glGetUniformLocation(shaderProgramObject, "u_double_tap");
	ldUniform = GLES31.glGetUniformLocation(shaderProgramObject, "u_Ld");
	kdUniform = GLES31.glGetUniformLocation(shaderProgramObject, "u_Kd");
	lightPositionUniform = GLES31.glGetUniformLocation(shaderProgramObject, "u_light_position");

	final float cubeVertices[] = new float[]
	{
		1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,

		1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		1.0f, 1.0f, 1.0f,//right top
		-1.0f, 1.0f, 1.0f,//left top
		-1.0f, -1.0f, 1.0f,//left bottom
		1.0f, -1.0f, 1.0f,

		1.0f, -1.0f, -1.0f,//right top of back face
		-1.0f, -1.0f, -1.0f,//left top of back face
		-1.0f, 1.0f, -1.0f,//left bottom of back face
		1.0f, 1.0f, -1.0f,//right bottom of back face
					 
		-1.0f, 1.0f, 1.0f,//right top of right face
		-1.0f, 1.0f, -1.0f,//left top of right face
		-1.0f, -1.0f, -1.0f,//left bottom of right face
		-1.0f, -1.0f, 1.0f,//right bottom of right fac

		1.0f, 1.0f, -1.0f,//right top of left face
		1.0f, 1.0f, 1.0f,//left top of left face
		1.0f, -1.0f, 1.0f,//left bottom of left face
		1.0f, -1.0f, -1.0f//right bottom of left face

	};

	final float CubeNormal[] = new float[]
	{
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,

		0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f,

		0.0f, 0.0f ,1.0f,
		0.0f, 0.0f ,1.0f,
		0.0f, 0.0f ,1.0f,
		0.0f, 0.0f ,1.0f,

		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,

		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,

		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,

	};

	GLES31.glGenVertexArrays(1,vao_cube,0);
	GLES31.glBindVertexArray(vao_cube[0]);
	GLES31.glGenBuffers(1,vbo_cube_position,0);
	GLES31.glBindBuffer(GLES31.GL_ARRAY_BUFFER,vbo_cube_position[0]);
	ByteBuffer byteBuffer = ByteBuffer.allocateDirect(cubeVertices.length * 4);
	byteBuffer.order(ByteOrder.nativeOrder());
	FloatBuffer verticesBuffer = byteBuffer.asFloatBuffer();
	verticesBuffer.put(cubeVertices);
	verticesBuffer.position(0);
	GLES31.glBufferData(GLES31.GL_ARRAY_BUFFER, cubeVertices.length * 4, verticesBuffer, GLES31.GL_STATIC_DRAW);
	GLES31.glVertexAttribPointer(GLESMacros.ARD_ATTRIBUTE_VERTEX, 3, GLES31.GL_FLOAT, false, 0, 0);
	GLES31.glEnableVertexAttribArray(GLESMacros.ARD_ATTRIBUTE_VERTEX);
	GLES31.glBindBuffer(GLES31.GL_ARRAY_BUFFER, 0);

		//vbo for normal
	GLES31.glGenBuffers(1,vbo_cube_normal,0);
	GLES31.glBindBuffer(GLES31.GL_ARRAY_BUFFER,vbo_cube_normal[0]);
	ByteBuffer byteBufferColorCube = ByteBuffer.allocateDirect(CubeNormal.length * 4);
	byteBufferColorCube.order(ByteOrder.nativeOrder());
	FloatBuffer colorBufferCube = byteBufferColorCube.asFloatBuffer();
	colorBufferCube.put(CubeNormal);
	colorBufferCube.position(0);
	GLES31.glBufferData(GLES31.GL_ARRAY_BUFFER, CubeNormal.length * 4, colorBufferCube, GLES31.GL_STATIC_DRAW);
	GLES31.glVertexAttribPointer(GLESMacros.ARD_ATTRIBUTE_COLOR, 3, GLES31.GL_FLOAT, false, 0, 0);
	GLES31.glEnableVertexAttribArray(GLESMacros.ARD_ATTRIBUTE_COLOR);
	GLES31.glBindBuffer(GLES31.GL_ARRAY_BUFFER, 0);
	GLES31.glBindVertexArray(0);

	GLES31.glEnable(GLES31.GL_DEPTH_TEST);
	GLES31.glDepthFunc(GLES31.GL_LEQUAL);
	//GLES31.glEnable(GLES31.GL_CULL_FACE);
	GLES31.glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	singleTap = 0;
	doubleTap = 0;

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

	if(doubleTap == 1)
	{
		GLES31.glUniform1i(doubleTapUniform, 1);
		GLES31.glUniform3f(ldUniform, 1.0f, 1.0f, 1.0f);
		GLES31.glUniform3f(kdUniform, 0.5f, 0.5f, 0.5f);
		float[] lightPosition = { 0.0f, 0.0f, 2.0f ,1.0f };
		GLES31.glUniform4fv(lightPositionUniform, 1, lightPosition, 0);
	}

	else
	{
		GLES31.glUniform1i(doubleTapUniform, 0);
	}

	float modelMatrix[] = new float[16];
	float modelViewMatrix[] = new float[16];
	
	float rotationMatrix[] = new float[16];
	float scaleMatrix[] = new float[16];

	Matrix.setIdentityM(modelViewMatrix, 0);
	Matrix.setIdentityM(modelMatrix, 0);
	Matrix.setIdentityM(rotationMatrix, 0);
	Matrix.setIdentityM(scaleMatrix, 0);

	//CUBE BLOCK

	Matrix.translateM(modelMatrix, 0, 0.0f, 0.0f, -5.0f);
	//Matrix.rotateM(rotationMatrix, 0, angle_cube, 1.0f, 1.0f, 1.0f);
	Matrix.rotateM(rotationMatrix, 0, angle_cube, 1.0f, 0.0f, 0.0f);
	Matrix.multiplyMM(modelViewMatrix, 0 ,modelMatrix, 0, rotationMatrix, 0);
	Matrix.rotateM(rotationMatrix, 0, angle_cube, 0.0f, 1.0f, 0.0f);
	Matrix.multiplyMM(modelViewMatrix, 0 ,modelMatrix, 0, rotationMatrix, 0);
	Matrix.rotateM(rotationMatrix, 0, angle_cube, 0.0f, 0.0f, 1.0f);
	Matrix.multiplyMM(modelViewMatrix, 0 ,modelMatrix, 0, rotationMatrix, 0);
	
	GLES31.glUniformMatrix4fv(modelViewMatrixUniform, 1, false, modelViewMatrix, 0);
	GLES31.glUniformMatrix4fv(projectionMatrixUniform, 1, false, perspectiveProjectionMatrix, 0);
	GLES31.glBindVertexArray(vao_cube[0]);
	GLES31.glDrawArrays(GLES31.GL_TRIANGLE_FAN, 0, 4);
	GLES31.glDrawArrays(GLES31.GL_TRIANGLE_FAN, 4, 4);
	GLES31.glDrawArrays(GLES31.GL_TRIANGLE_FAN, 8, 4);
	GLES31.glDrawArrays(GLES31.GL_TRIANGLE_FAN, 12, 4);
	GLES31.glDrawArrays(GLES31.GL_TRIANGLE_FAN, 16, 4);
	GLES31.glDrawArrays(GLES31.GL_TRIANGLE_FAN, 20, 4);
	GLES31.glBindVertexArray(0);

	GLES31.glUseProgram(0);
	if(singleTap == 1)
		update();
	requestRender();
}

void uninitialize()
{
	

	if(vao_cube[0] != 0)
	{
		GLES31.glDeleteVertexArrays(1, vao_cube, 0);
		vao_cube[0] = 0;
	}

	if(vbo_cube_position[0] != 0)
	{
		GLES31.glDeleteVertexArrays(1, vbo_cube_position, 0);
		vbo_cube_position[0] = 0;
	}

	if(vbo_cube_normal[0] != 0)
	{
		GLES31.glDeleteVertexArrays(1, vbo_cube_normal, 0);
		vbo_cube_normal[0] = 0;
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
	
	angle_cube = angle_cube + 0.5f;
	if(angle_cube >= 360.0f)
	{
		angle_cube = 0.0f;
	}
}

}
