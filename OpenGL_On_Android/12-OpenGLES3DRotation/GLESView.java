package com.example.sknaa.OGLES3DRotation;

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

	private int[] vaoPyramid = new int[1];
	private int[] vaoCube = new int[1];
	private int[] vbo_Position = new int[1];
	private int[] vbo_Color = new int[1];
	private int mvpUnifrom;

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
	System.out.println("OGLESOrthoTriangle: OpenGL-ES Version = "+glesVersion);
	String glslVersion = gl.glGetString(GLES31.GL_SHADING_LANGUAGE_VERSION);
	System.out.println("OGLESOrthoTriangle: GLSL Version = "+glslVersion);
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
		"in vec4 vColor;"+
		"uniform mat4 u_mvp_matrix;"+
		"out vec4 out_Color;"+
		"void main(void)"+
		"{"+
		"gl_Position = u_mvp_matrix * vPosition;"+
		"out_Color = vColor;"+
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
			System.out.println("OGLESOrthoTriangle: Vertex Shader Compilation Log = "+szInfoLog);
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
		"in vec4 out_Color;"+
		"out vec4 FragColor;"+
		"void main(void)"+
		"{"+
		"FragColor = out_Color;"+
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
			System.out.println("OGLESOrthoTriangle: Fragment Shader Compilation Log = "+szInfoLog);
			uninitialize();
			System.exit(0);
		}
	}

	shaderProgramObject = GLES31.glCreateProgram();
	GLES31.glAttachShader(shaderProgramObject,vertexShaderObject);
	GLES31.glAttachShader(shaderProgramObject,fragmentShaderObject);
	GLES31.glBindAttribLocation(shaderProgramObject,GLESMacros.VDG_ATTRIBUTE_VERTEX,"vPosition");
	GLES31.glBindAttribLocation(shaderProgramObject,GLESMacros.VDG_ATTRIBUTE_COLOR, "vColor");
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
			System.out.println("OGLESOrthoTriangle: Shader Program Link Log = "+szInfoLog);
			uninitialize();
			System.exit(0);
		}
	}

	mvpUnifrom = GLES31.glGetUniformLocation(shaderProgramObject, "u_mvp_matrix");

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

	final float pyramidColor[] = new float[]
	{
		1.0f, 0.0f, 0.0f, //red
	0.0f, 1.0f, 0.0f,//green
	0.0f, 0.0f, 1.0f,//blue
								  
	1.0f, 0.0f, 0.0f, //red
	0.0f, 0.0f, 1.0f,//blue
	0.0f, 1.0f, 0.0f,//green

	1.0f, 0.0f, 0.0f,//red
	0.0f, 1.0f, 0.0f,//green
	0.0f, 0.0f, 1.0f,//blue

	1.0f, 0.0f, 0.0f,//red
	0.0f, 0.0f, 1.0f,//blue
	0.0f, 1.0f, 0.0f,//green
	};

	final float cubeVertices[] = new float[]
	{
		1.0f, 1.0f, -1.0f,
	-1.0f, 1.0f, -1.0f,
	-1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,

	//BOTTOM FACE
	1.0f, -1.0f, -1.0f,//right top of bottom face
	-1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f, 1.0f,
	1.0f, -1.0f, 1.0f,

	//FRONT FACE
	1.0f, 1.0f, 1.0f,//right top
	-1.0f, 1.0f, 1.0f,//left top
	-1.0f, -1.0f, 1.0f,//left bottom
	1.0f, -1.0f, 1.0f,

	//BACK FACE
	 1.0f, 1.0f, -1.0f,//right top of back face
	-1.0f, 1.0f, -1.0f,//left top of back face
	-1.0f, -1.0f, -1.0f,//left bottom of back face
	1.0f, -1.0f, -1.0f,//right bottom of back face

	 //RIGHT FACE
	1.0f,1.0f, -1.0f,//right top of right face
	1.0f, 1.0f, 1.0f,//left top of right face
	1.0f, -1.0f, 1.0f,//left bottom of right face
	1.0f, -1.0f, -1.0f,//right bottom of right facce


	 //LEFT FACE
	-1.0f, 1.0f, 1.0f,//right top of left face
	-1.0f, 1.0f, -1.0f,//left top of left face
	-1.0f, -1.0f, -1.0f,//left bottom of left face
	-1.0f, -1.0f, 1.0f//right bottom of left face
	};

	final float CubeColor[] = new float[]
	{
	0.0f, 1.0f, 0.0f,//green
	0.0f, 1.0f, 0.0f,//green
	0.0f, 1.0f, 0.0f,//green
	0.0f, 1.0f, 0.0f,//green

	0.0f, 1.0f, 1.0f,//cyan
	0.0f, 1.0f, 1.0f,//cyan
	0.0f, 1.0f, 1.0f,//cyan
	0.0f, 1.0f, 1.0f,//cyan

	1.0f, 0.0f, 0.0f, //red
	1.0f, 0.0f, 0.0f, //red
	1.0f, 0.0f, 0.0f, //red
	1.0f, 0.0f, 0.0f, //red

	1.0f, 1.0f, 1.0f,//yellow
	1.0f, 1.0f, 1.0f,//yellow
	1.0f, 1.0f, 1.0f,//yellow
	1.0f, 1.0f, 1.0f,//yellow

	0.0f,0.0f,1.0f,//blue
	0.0f,0.0f,1.0f,//blue
	0.0f,0.0f,1.0f,//blue
	0.0f,0.0f,1.0f,//blue

	1.0f,0.0f,1.0f,//magenta
	1.0f,0.0f,1.0f,//magenta
	1.0f,0.0f,1.0f,//magenta
	1.0f,0.0f,1.0f//magenta

	};

	GLES31.glGenVertexArrays(1,vaoPyramid,0);
	GLES31.glBindVertexArray(vaoPyramid[0]);
	GLES31.glGenBuffers(1,vbo_Position,0);
	GLES31.glBindBuffer(GLES31.GL_ARRAY_BUFFER,vbo_Position[0]);
	ByteBuffer byteBuffer = ByteBuffer.allocateDirect(pyramidVertices.length * 4);
	byteBuffer.order(ByteOrder.nativeOrder());
	FloatBuffer verticesBuffer = byteBuffer.asFloatBuffer();
	verticesBuffer.put(pyramidVertices);
	verticesBuffer.position(0);
	GLES31.glBufferData(GLES31.GL_ARRAY_BUFFER, pyramidVertices.length * 4, verticesBuffer, GLES31.GL_STATIC_DRAW);
	GLES31.glVertexAttribPointer(GLESMacros.VDG_ATTRIBUTE_VERTEX, 3, GLES31.GL_FLOAT, false, 0, 0);
	GLES31.glEnableVertexAttribArray(GLESMacros.VDG_ATTRIBUTE_VERTEX);
	GLES31.glBindBuffer(GLES31.GL_ARRAY_BUFFER, 0);

	//vbo for color
	GLES31.glGenBuffers(1,vbo_Color,0);
	GLES31.glBindBuffer(GLES31.GL_ARRAY_BUFFER,vbo_Color[0]);
	ByteBuffer byteBufferColor = ByteBuffer.allocateDirect(pyramidColor.length * 4);
	byteBufferColor.order(ByteOrder.nativeOrder());
	FloatBuffer colorBuffer = byteBufferColor.asFloatBuffer();
	colorBuffer.put(pyramidColor);
	colorBuffer.position(0);
	GLES31.glBufferData(GLES31.GL_ARRAY_BUFFER, pyramidColor.length * 4, colorBuffer, GLES31.GL_STATIC_DRAW);
	GLES31.glVertexAttribPointer(GLESMacros.VDG_ATTRIBUTE_COLOR, 3, GLES31.GL_FLOAT, false, 0, 0);
	GLES31.glEnableVertexAttribArray(GLESMacros.VDG_ATTRIBUTE_COLOR);
	GLES31.glBindBuffer(GLES31.GL_ARRAY_BUFFER, 0);

	GLES31.glBindVertexArray(0);


	GLES31.glGenVertexArrays(1,vaoCube,0);
	GLES31.glBindVertexArray(vaoCube[0]);
	GLES31.glGenBuffers(1,vbo_Position,0);
	GLES31.glBindBuffer(GLES31.GL_ARRAY_BUFFER,vbo_Position[0]);
	byteBuffer = ByteBuffer.allocateDirect(cubeVertices.length * 4);
	byteBuffer.order(ByteOrder.nativeOrder());
	verticesBuffer = byteBuffer.asFloatBuffer();
	verticesBuffer.put(cubeVertices);
	verticesBuffer.position(0);
	GLES31.glBufferData(GLES31.GL_ARRAY_BUFFER, cubeVertices.length * 4, verticesBuffer, GLES31.GL_STATIC_DRAW);
	GLES31.glVertexAttribPointer(GLESMacros.VDG_ATTRIBUTE_VERTEX, 3, GLES31.GL_FLOAT, false, 0, 0);
	GLES31.glEnableVertexAttribArray(GLESMacros.VDG_ATTRIBUTE_VERTEX);
	GLES31.glBindBuffer(GLES31.GL_ARRAY_BUFFER, 0);

		//vbo for color
	GLES31.glGenBuffers(1,vbo_Color,0);
	GLES31.glBindBuffer(GLES31.GL_ARRAY_BUFFER,vbo_Color[0]);
	ByteBuffer byteBufferColorCube = ByteBuffer.allocateDirect(CubeColor.length * 4);
	byteBufferColorCube.order(ByteOrder.nativeOrder());
	FloatBuffer colorBufferCube = byteBufferColorCube.asFloatBuffer();
	colorBufferCube.put(CubeColor);
	colorBufferCube.position(0);
	GLES31.glBufferData(GLES31.GL_ARRAY_BUFFER, CubeColor.length * 4, colorBufferCube, GLES31.GL_STATIC_DRAW);
	GLES31.glVertexAttribPointer(GLESMacros.VDG_ATTRIBUTE_COLOR, 3, GLES31.GL_FLOAT, false, 0, 0);
	GLES31.glEnableVertexAttribArray(GLESMacros.VDG_ATTRIBUTE_COLOR);
	GLES31.glBindBuffer(GLES31.GL_ARRAY_BUFFER, 0);
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

	float modelViewMatrix[] = new float[16];
	float modelViewProjectionMatrix[] = new float[16];
	float rotationMatrix[] = new float[16];
	float scaleMatrix[] = new float[16];

	Matrix.setIdentityM(modelViewMatrix, 0);
	Matrix.setIdentityM(modelViewProjectionMatrix, 0);
	Matrix.setIdentityM(rotationMatrix, 0);


	//TRIANGLE BLOCK
	Matrix.translateM(modelViewMatrix, 0, -1.5f, 0.0f, -5.0f);
	Matrix.rotateM(rotationMatrix, 0, angle_pyramid, 0.0f, 1.0f, 0.0f);
	Matrix.multiplyMM(modelViewMatrix, 0, modelViewMatrix, 0, rotationMatrix, 0);
	Matrix.multiplyMM(modelViewProjectionMatrix, 0, perspectiveProjectionMatrix, 0, modelViewMatrix, 0);

	GLES31.glUniformMatrix4fv(mvpUnifrom, 1, false, modelViewProjectionMatrix, 0);
	GLES31.glBindVertexArray(vaoPyramid[0]);
	GLES31.glDrawArrays(GLES31.GL_TRIANGLES, 0, 12);
	GLES31.glBindVertexArray(0);

	//SQUARE BLOCK

	Matrix.setIdentityM(modelViewMatrix, 0);
	Matrix.setIdentityM(modelViewProjectionMatrix, 0);
	Matrix.setIdentityM(rotationMatrix, 0);
	Matrix.translateM(modelViewMatrix, 0, 1.5f, 0.0f, -5.0f);
	Matrix.scaleM(modelViewMatrix, 0, 0.75f, 0.75f ,0.75f);
	Matrix.rotateM(rotationMatrix, 0, angle_cube, 1.0f, 1.0f, 1.0f);
	Matrix.multiplyMM(modelViewMatrix, 0 ,modelViewMatrix, 0, rotationMatrix, 0);
	Matrix.multiplyMM(modelViewProjectionMatrix, 0, perspectiveProjectionMatrix, 0, modelViewMatrix, 0);
	GLES31.glUniformMatrix4fv(mvpUnifrom, 1, false, modelViewProjectionMatrix, 0);
	GLES31.glBindVertexArray(vaoCube[0]);
	GLES31.glDrawArrays(GLES31.GL_TRIANGLE_FAN, 0, 4);
	GLES31.glDrawArrays(GLES31.GL_TRIANGLE_FAN, 4, 4);
	GLES31.glDrawArrays(GLES31.GL_TRIANGLE_FAN, 8, 4);
	GLES31.glDrawArrays(GLES31.GL_TRIANGLE_FAN, 12, 4);
	GLES31.glDrawArrays(GLES31.GL_TRIANGLE_FAN, 16, 4);
	GLES31.glDrawArrays(GLES31.GL_TRIANGLE_FAN, 20, 4);
	GLES31.glBindVertexArray(0);

	GLES31.glUseProgram(0);
	update();
	requestRender();
}

void uninitialize()
{
	if(vaoPyramid[0] != 0)
	{
		GLES31.glDeleteVertexArrays(1, vaoPyramid, 0);
		vaoPyramid[0] = 0;
	}

	if(vaoCube[0] != 0)
	{
		GLES31.glDeleteVertexArrays(1, vaoCube, 0);
		vaoCube[0] = 0;
	}

	if(vbo_Position[0] != 0)
	{
		GLES31.glDeleteVertexArrays(1, vbo_Position, 0);
		vbo_Position[0] = 0;
	}

	if(vbo_Color[0] != 0)
	{
		GLES31.glDeleteVertexArrays(1, vbo_Color, 0);
		vbo_Color[0] = 0;
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
	angle_pyramid = angle_pyramid + 0.5f;

	if(angle_pyramid >= 360.0f)
	{
		angle_pyramid = 0.0f;
	}

	angle_cube = angle_cube + 0.5f;
	if(angle_cube >= 360.0f)
	{
		angle_cube = 0.0f;
	}
}

}
