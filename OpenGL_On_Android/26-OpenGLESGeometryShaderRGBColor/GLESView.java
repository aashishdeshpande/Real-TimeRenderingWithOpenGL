package com.example.sknaa.OpenGLESGeometryShaderRGB;

import android.content.Context;
import android.opengl.GLSurfaceView;
import javax.microedition.khronos.opengles.GL10;
import javax.microedition.khronos.egl.EGLConfig;
import android.opengl.GLES32;
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
	private int geometryShaderObject;
	private int fragmentShaderObject;
	private int shaderProgramObject;

	private int[] vaoTriangle = new int[1];
	private int[] vboPosition = new int[1];
	private int[] vboColor = new int[1];
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
	String glslVersion = gl.glGetString(GLES32.GL_SHADING_LANGUAGE_VERSION);
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
	vertexShaderObject = GLES32.glCreateShader(GLES32.GL_VERTEX_SHADER);
	final String vertexShaderSourceCode = String.format
	(
		"#version 320 es"+
		"\n"+
		"in vec4 vPosition;"+
		"in vec4 vColor;"+
		"out vec4 out_Color;"+
		"uniform mat4 u_mvp_matrix;"+
		"void main(void)"+
		"{"+
		"gl_Position = u_mvp_matrix * vPosition;"+
		"out_Color = vColor;"+
		"}"
	);

	GLES32.glShaderSource(vertexShaderObject, vertexShaderSourceCode);
	GLES32.glCompileShader(vertexShaderObject);
	int[] iShaderCompiledStatus = new int[1];
	int[] iInfoLogLength = new int[1];
	String szInfoLog = null;
	GLES32.glGetShaderiv(vertexShaderObject, GLES32.GL_COMPILE_STATUS, iShaderCompiledStatus, 0);
	if(iShaderCompiledStatus[0] == GLES32.GL_FALSE)
	{
		GLES32.glGetShaderiv(vertexShaderObject, GLES32.GL_INFO_LOG_LENGTH, iInfoLogLength, 0);

		if(iInfoLogLength[0] > 0)
		{
			szInfoLog = GLES32.glGetShaderInfoLog(vertexShaderObject);
			System.out.println("OGLESOrthoTriangle: Vertex Shader Compilation Log = "+szInfoLog);
			uninitialize();
			System.exit(0);
		}
	}	


	geometryShaderObject = GLES32.glCreateShader(GLES32.GL_GEOMETRY_SHADER);
	final String geometryShaderSourceCode = String.format
	(
		"#version 320 es"+
		"\n"+
		"layout(triangles)in;"+
		"layout(triangle_strip, max_vertices = 9)out;"+
		"in vec4 out_Color[];"+
		"out vec4 fColor;"+
		"uniform mat4 u_mvp_matrix;"+
		"void main(void)"+
		"{"+
		"for(int vertex = 0; vertex < 3; vertex++)"+
		"{"+
		"fColor = out_Color[vertex];"+
		"gl_Position = u_mvp_matrix * (gl_in[vertex].gl_Position + vec4(0.0, 1.0, 0.0, 0.0));"+
		"EmitVertex();"+
		"fColor = out_Color[vertex];"+
		"gl_Position = u_mvp_matrix * (gl_in[vertex].gl_Position + vec4(-1.0, -1.0, 0.0, 0.0));"+
		"EmitVertex();"+
		"fColor = out_Color[vertex];"+
		"gl_Position = u_mvp_matrix * (gl_in[vertex].gl_Position + vec4(1.0, -1.0, 0.0, 0.0));"+
		"EmitVertex();"+
		"EndPrimitive();"+
		"}"+
		"}"
	);

	GLES32.glShaderSource(geometryShaderObject, geometryShaderSourceCode);
	GLES32.glCompileShader(geometryShaderObject);
	iShaderCompiledStatus = new int[1];
	iInfoLogLength = new int[1];
	szInfoLog = null;
	GLES32.glGetShaderiv(geometryShaderObject, GLES32.GL_COMPILE_STATUS, iShaderCompiledStatus, 0);
	if(iShaderCompiledStatus[0] == GLES32.GL_FALSE)
	{
		GLES32.glGetShaderiv(geometryShaderObject, GLES32.GL_INFO_LOG_LENGTH, iInfoLogLength, 0);

		if(iInfoLogLength[0] > 0)
		{
			szInfoLog = GLES32.glGetShaderInfoLog(geometryShaderObject);
			System.out.println("OGLESOrthoTriangle: Vertex Shader Compilation Log = "+szInfoLog);
			uninitialize();
			System.exit(0);
		}
	}	

	fragmentShaderObject = GLES32.glCreateShader(GLES32.GL_FRAGMENT_SHADER);
	final String fragmentShaderSourceCode = String.format
	(
		"#version 320 es"+
		"\n"+
		"precision highp float;"+
		"out vec4 FragColor;"+
		"in vec4 fColor;"+
		"void main(void)"+
		"{"+
		"FragColor = fColor;"+
		"}"
	);

	GLES32.glShaderSource(fragmentShaderObject,fragmentShaderSourceCode);
	GLES32.glCompileShader(fragmentShaderObject);
	iShaderCompiledStatus[0] = 0;
	iInfoLogLength[0] = 0;
	szInfoLog = null;
	GLES32.glGetShaderiv(fragmentShaderObject,GLES32.GL_COMPILE_STATUS,iShaderCompiledStatus, 0);
	if(iShaderCompiledStatus[0] == GLES32.GL_FALSE)
	{
		GLES32.glGetShaderiv(fragmentShaderObject, GLES32.GL_INFO_LOG_LENGTH, iInfoLogLength, 0);
		if(iInfoLogLength[0] > 0)
		{
			szInfoLog = GLES32.glGetShaderInfoLog(fragmentShaderObject);
			System.out.println("OGLESOrthoTriangle: Fragment Shader Compilation Log = "+szInfoLog);
			uninitialize();
			System.exit(0);
		}
	}

	shaderProgramObject = GLES32.glCreateProgram();
	GLES32.glAttachShader(shaderProgramObject,vertexShaderObject);
	GLES32.glAttachShader(shaderProgramObject,fragmentShaderObject);
	GLES32.glAttachShader(shaderProgramObject, geometryShaderObject);
	GLES32.glBindAttribLocation(shaderProgramObject,GLESMacros.VDG_ATTRIBUTE_VERTEX,"vPosition");
	GLES32.glBindAttribLocation(shaderProgramObject,GLESMacros.VDG_ATTRIBUTE_COLOR, "vColor");
	GLES32.glLinkProgram(shaderProgramObject);
	int[] iShaderProgramLinkStatus = new int[1];
	iInfoLogLength[0] = 0;
	szInfoLog = null;
	GLES32.glGetProgramiv(shaderProgramObject, GLES32.GL_LINK_STATUS, iShaderProgramLinkStatus, 0);
	if(iShaderProgramLinkStatus[0] == GLES32.GL_FALSE)
	{
		GLES32.glGetProgramiv(shaderProgramObject, GLES32.GL_INFO_LOG_LENGTH, iInfoLogLength, 0);
		if(iInfoLogLength[0] > 0)
		{
			szInfoLog = GLES32.glGetProgramInfoLog(shaderProgramObject);
			System.out.println("OGLESOrthoTriangle: Shader Program Link Log = "+szInfoLog);
			uninitialize();
			System.exit(0);
		}
	}

	mvpUnifrom = GLES32.glGetUniformLocation(shaderProgramObject, "u_mvp_matrix");

	final float triangleVertices[] = new float[]
	{
		0.0f, 1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f
	};

	final float triangleColor[] = new float[]
	{
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f
	};

	GLES32.glGenVertexArrays(1,vaoTriangle,0);
	GLES32.glBindVertexArray(vaoTriangle[0]);
	GLES32.glGenBuffers(1,vboPosition,0);
	GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER,vboPosition[0]);
	ByteBuffer byteBuffer = ByteBuffer.allocateDirect(triangleVertices.length * 4);
	byteBuffer.order(ByteOrder.nativeOrder());
	FloatBuffer verticesBuffer = byteBuffer.asFloatBuffer();
	verticesBuffer.put(triangleVertices);
	verticesBuffer.position(0);
	GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, triangleVertices.length * 4, verticesBuffer, GLES32.GL_STATIC_DRAW);
	GLES32.glVertexAttribPointer(GLESMacros.VDG_ATTRIBUTE_VERTEX, 3, GLES32.GL_FLOAT, false, 0, 0);
	GLES32.glEnableVertexAttribArray(GLESMacros.VDG_ATTRIBUTE_VERTEX);
	GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);
	

	GLES32.glGenBuffers(1,vboColor,0);
	GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER,vboColor[0]);
	byteBuffer = ByteBuffer.allocateDirect(triangleColor.length * 4);
	byteBuffer.order(ByteOrder.nativeOrder());
	FloatBuffer colorBuffer = byteBuffer.asFloatBuffer();
	colorBuffer.put(triangleColor);
	colorBuffer.position(0);
	GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, triangleColor.length * 4, colorBuffer, GLES32.GL_STATIC_DRAW);
	GLES32.glVertexAttribPointer(GLESMacros.VDG_ATTRIBUTE_COLOR, 3, GLES32.GL_FLOAT, false, 0, 0);
	GLES32.glEnableVertexAttribArray(GLESMacros.VDG_ATTRIBUTE_COLOR);
	GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);

	GLES32.glBindVertexArray(0);

	GLES32.glEnable(GLES32.GL_DEPTH_TEST);
	GLES32.glDepthFunc(GLES32.GL_LEQUAL);
	GLES32.glEnable(GLES32.GL_CULL_FACE);
	GLES32.glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	Matrix.setIdentityM(perspectiveProjectionMatrix, 0);
}	

private void resize(int width, int height)
{
	if(height == 0)
		height = 1;
	GLES32.glViewport(0, 0, width, height);
	Matrix.perspectiveM(perspectiveProjectionMatrix, 0, 45.0f, (float)width / (float)height, 0.1f, 100.0f);
}

public void display()
{
	GLES32.glClear(GLES32.GL_COLOR_BUFFER_BIT | GLES32.GL_DEPTH_BUFFER_BIT);
	GLES32.glUseProgram(shaderProgramObject);

	float modelViewMatrix[] = new float[16];
	float modelViewProjectionMatrix[] = new float[16];

	Matrix.setIdentityM(modelViewMatrix, 0);
	Matrix.setIdentityM(modelViewProjectionMatrix, 0);
	
	//TRIANGLE BLOCK

	Matrix.translateM(modelViewMatrix, 0, 0.0f, 0.0f, -4.0f);
	Matrix.multiplyMM(modelViewProjectionMatrix, 0, perspectiveProjectionMatrix, 0, modelViewMatrix, 0);

	GLES32.glUniformMatrix4fv(mvpUnifrom, 1, false, modelViewProjectionMatrix, 0);
	GLES32.glBindVertexArray(vaoTriangle[0]);
	GLES32.glDrawArrays(GLES32.GL_TRIANGLES, 0, 3);
	GLES32.glBindVertexArray(0);

	GLES32.glUseProgram(0);
	requestRender();
}

void uninitialize()
{
	if(vaoTriangle[0] != 0)
	{
		GLES32.glDeleteVertexArrays(1, vaoTriangle, 0);
		vaoTriangle[0] = 0;
	}

	if(vboPosition[0] != 0)
	{
		GLES32.glDeleteVertexArrays(1, vboPosition, 0);
		vboPosition[0] = 0;
	}

	if(vboColor[0] != 0)
	{
		GLES32.glDeleteVertexArrays(1, vboColor, 0);
		vboColor[0] = 0;
	}

	if(shaderProgramObject != 0)
	{
		if(vertexShaderObject != 0)
		{
			GLES32.glDetachShader(shaderProgramObject, vertexShaderObject);
			GLES32.glDeleteShader(vertexShaderObject);
			vertexShaderObject = 0;
		}

		if(geometryShaderObject != 0)
		{
			GLES32.glDetachShader(shaderProgramObject, geometryShaderObject);
			GLES32.glDeleteShader(geometryShaderObject);
			geometryShaderObject = 0;
		}

		if(fragmentShaderObject != 0)
		{
			GLES32.glDetachShader(shaderProgramObject, fragmentShaderObject);
			GLES32.glDeleteShader(fragmentShaderObject);
			fragmentShaderObject = 0;
		}
	}

	if(shaderProgramObject != 0)
	{
		GLES32.glDeleteProgram(shaderProgramObject);
		shaderProgramObject = 0;
	}

}

}
