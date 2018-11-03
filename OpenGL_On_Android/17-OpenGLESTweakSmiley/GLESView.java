package com.example.sknaa.OGLESTweakSmiley;

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

import android.graphics.BitmapFactory;
import android.graphics.Bitmap;
import android.opengl.GLUtils;

public class GLESView extends GLSurfaceView implements GLSurfaceView.Renderer, OnGestureListener, OnDoubleTapListener
{
	private final Context context;

	private GestureDetector gestureDetector;

	private int vertexShaderObject;
	private int fragmentShaderObject;
	private int shaderProgramObject;

	
	private int[] vaoSmiley = new int[1];

	private int[] vbo_Smiley_Position = new int[1];
	private int[] vbo_Smiley_Texture = new int[1];

	private int mvpUnifrom;
	private int texture0_sampler_uniform;

	private int[] texture_smiley = new int[1];

	private float perspectiveProjectionMatrix[] = new float[16];

	static final int checkImageWidth = 64;
	static final int checkImageHeight = 64;
	byte[][][] checkImage = new byte [checkImageHeight][checkImageWidth][4];
	private byte[] array_buffer = new byte[16384];

	private int[] texName = new int[1];
	private int n = 0;
	private int singleTap;

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
	singleTap++;
	if(singleTap > 4)
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
		"in vec2 vTexture0_Coord;"+
		"out vec2 out_texture0_coord;"+
		"uniform mat4 u_mvp_matrix;"+
		"out vec4 out_Color;"+
		"void main(void)"+
		"{"+
		"gl_Position = u_mvp_matrix * vPosition;"+
		"out_texture0_coord = vTexture0_Coord;"+
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
		"in vec2 out_texture0_coord;"+
		"uniform highp sampler2D u_texture0_sampler;"+
		"out vec4 FragColor;"+
		"void main(void)"+
		"{"+
		"FragColor = texture(u_texture0_sampler, out_texture0_coord);"+
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
	GLES31.glBindAttribLocation(shaderProgramObject,GLESMacros.VDG_ATTRIBUTE_TEXTURE, "vTexture0_Coord");
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
	texture0_sampler_uniform = GLES31.glGetUniformLocation(shaderProgramObject, "u_texture0_sampler");

	texture_smiley[0] = loadGLTexture(R.raw.smiley);
	
	final float smileyVertices[] = new float[]
	{
		1.0f, 1.0f, 0.0f,
		-1.0f,1.0f,0.0f,
		-1.0f,-1.0f,0.0f,
		1.0f,-1.0f,0.0f
	};

	/*final float smileyTexCoords[] = new float[]
	{
		1.0f,1.0f,
		0.0f,1.0f,
		0.0f,0.0f,
		1.0f,0.0f
	};*/

	GLES31.glGenVertexArrays(1,vaoSmiley,0);
	GLES31.glBindVertexArray(vaoSmiley[0]);
	GLES31.glGenBuffers(1,vbo_Smiley_Position,0);
	GLES31.glBindBuffer(GLES31.GL_ARRAY_BUFFER,vbo_Smiley_Position[0]);
	ByteBuffer byteBuffer = ByteBuffer.allocateDirect(48);
	byteBuffer.order(ByteOrder.nativeOrder());
	FloatBuffer verticesBuffer = byteBuffer.asFloatBuffer();
	verticesBuffer.put(smileyVertices);
	verticesBuffer.position(0);
	GLES31.glBufferData(GLES31.GL_ARRAY_BUFFER, smileyVertices.length * 4, verticesBuffer, GLES31.GL_STATIC_DRAW);
	GLES31.glVertexAttribPointer(GLESMacros.VDG_ATTRIBUTE_VERTEX, 3, GLES31.GL_FLOAT, false, 0, 0);
	GLES31.glEnableVertexAttribArray(GLESMacros.VDG_ATTRIBUTE_VERTEX);
	GLES31.glBindBuffer(GLES31.GL_ARRAY_BUFFER, 0);

	//vbo for texture
	GLES31.glGenBuffers(1,vbo_Smiley_Texture,0);
	GLES31.glBindBuffer(GLES31.GL_ARRAY_BUFFER,vbo_Smiley_Texture[0]);
	//ByteBuffer byteBufferColor = ByteBuffer.allocateDirect(smileyTexCoords.length * 4);
	//byteBufferColor.order(ByteOrder.nativeOrder());
	//FloatBuffer colorBuffer = byteBufferColor.asFloatBuffer();
	//colorBuffer.put(smileyTexCoords);
	//colorBuffer.position(0);
	GLES31.glBufferData(GLES31.GL_ARRAY_BUFFER, 32, null, GLES31.GL_DYNAMIC_DRAW);
	GLES31.glVertexAttribPointer(GLESMacros.VDG_ATTRIBUTE_TEXTURE, 2, GLES31.GL_FLOAT, false, 0, 0);
	GLES31.glEnableVertexAttribArray(GLESMacros.VDG_ATTRIBUTE_TEXTURE);
	GLES31.glBindBuffer(GLES31.GL_ARRAY_BUFFER, 0);

	GLES31.glBindVertexArray(0);

	LoadGLTextures();

	GLES31.glEnable(GLES31.GL_DEPTH_TEST);
	GLES31.glDepthFunc(GLES31.GL_LEQUAL);
	//GLES31.glEnable(GLES31.GL_CULL_FACE);
	GLES31.glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	Matrix.setIdentityM(perspectiveProjectionMatrix, 0);
}	

public void LoadGLTextures()
{
	ByteBuffer byteBufferfillBuffer;
	System.out.println("OGLES: Before LoadGLTextures in CheckImage");
	MakeCheckImage();
	System.out.println("OGLES: After MakeCheckImage");
	arrayGen();

	System.out.println("OGLES: After arrayGen");
	byteBufferfillBuffer = ByteBuffer.allocateDirect(array_buffer.length * 4);
	byteBufferfillBuffer.order(ByteOrder.nativeOrder());
	//fillBuffer = byteBufferfillBuffer.asFloatBuffer();
	byteBufferfillBuffer.put(array_buffer);
	byteBufferfillBuffer.position(0);

	GLES31.glPixelStorei(GLES31.GL_UNPACK_ALIGNMENT, 1);

	GLES31.glGenTextures(1, texName, 0);

	GLES31.glBindTexture(GLES31.GL_TEXTURE_2D, texName[0]);

	GLES31.glTexParameteri(GLES31.GL_TEXTURE_2D, GLES31.GL_TEXTURE_WRAP_S, GLES31.GL_REPEAT);
	GLES31.glTexParameteri(GLES31.GL_TEXTURE_2D, GLES31.GL_TEXTURE_WRAP_T, GLES31.GL_REPEAT);
	GLES31.glTexParameteri(GLES31.GL_TEXTURE_2D, GLES31.GL_TEXTURE_MAG_FILTER, GLES31.GL_NEAREST);
	GLES31.glTexParameteri(GLES31.GL_TEXTURE_2D, GLES31.GL_TEXTURE_MIN_FILTER, GLES31.GL_NEAREST);

	GLES31.glTexImage2D(GLES31.GL_TEXTURE_2D, 0, GLES31.GL_RGBA, checkImageWidth, checkImageHeight, 0, GLES31.GL_RGBA, GLES31.GL_UNSIGNED_BYTE, byteBufferfillBuffer);
	
	//GLES31.glTexEnvf(GLES31.GL_TEXTURE_ENV, GLES31.GL_TEXTURE_ENV_MODE, GLES31.GL_REPLACE);
	System.out.println("OGLES: Exiting LoadGLTextures");

}

public void MakeCheckImage()
{
	System.out.println("OGLES: In MakeCheckImage");
	int i, j;
	int c = 0;
	boolean firstoperand;
	boolean secondoperand;
	//boolean firstoperand = true;
	//boolean secondoperand = true;
	for (i = 0; i<checkImageHeight; i++)
	{
		for (j = 0; j<checkImageWidth; j++)
		{
			//c = (((i & 0x8) == 0) ^ ((j & 0x8) == 0)) * 255;
			firstoperand = ((i & 0x8) == 0);
			secondoperand =((j & 0x8) == 0);

			int first = firstoperand ? 0 : 1;
			int second = secondoperand ? 0 : 1;
			//if((firstoperand == 0 && secondoperand == 0)) 
			//{
				//c = ((firstoperand ^ secondoperand) * 255);
			//}
			c = (first ^ second) * 255;
			//int a = firstoperand ? (i & 0x8) : 0;
			//int b = secondoperand ? (j & 0x8) : 0;

			//c = a ^ b;

			checkImage[i][j][0] = (byte)255;
			checkImage[i][j][1] = (byte)255;
			checkImage[i][j][2] = (byte)255;
			checkImage[i][j][3] = (byte)255;
		}
	}

	/*for(int l = 0; l < 64; l++)
	{
		for(int m = 0; m < 64; m++)
		{
			for(int n = 0; n< 4; n++)
			{
				array_buffer[num++] = checkImage[l][m][n];
			}
		}
	}*/	
}

public void arrayGen()
{
	System.out.println("OGLES: In arrayGen");
	for(int i = 0; i < 64; i++)
	{
		for(int j = 0; j < 64; j++)
		{
			for(int k = 0; k< 4; k++)
			{
				array_buffer[n++] = checkImage[i][j][k];
			}
			//n = n + 1;
		}
	}	
}


private int loadGLTexture(int imageFileResourceID)
{
	BitmapFactory.Options options = new BitmapFactory.Options();
	options.inScaled = false;

	Bitmap bitmap = BitmapFactory.decodeResource(context.getResources(), imageFileResourceID, options);
	int[] texture = new int[1];

	GLES31.glGenTextures(1, texture, 0);
	GLES31.glPixelStorei(GLES31.GL_UNPACK_ALIGNMENT, 1);
	GLES31.glBindTexture(GLES31.GL_TEXTURE_2D, texture[0]);
	GLES31.glTexParameteri(GLES31.GL_TEXTURE_2D, GLES31.GL_TEXTURE_MAG_FILTER, GLES31.GL_LINEAR);
	GLES31.glTexParameteri(GLES31.GL_TEXTURE_2D, GLES31.GL_TEXTURE_MIN_FILTER, GLES31.GL_LINEAR_MIPMAP_LINEAR);
	GLUtils.texImage2D(GLES31.GL_TEXTURE_2D, 0, bitmap, 0);
	GLES31.glGenerateMipmap(GLES31.GL_TEXTURE_2D);
	return(texture[0]);
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

	Matrix.setIdentityM(modelViewMatrix, 0);
	Matrix.setIdentityM(modelViewProjectionMatrix, 0);
	
	//Smiley BLOCK
	Matrix.translateM(modelViewMatrix, 0, 0.0f, 0.0f, -3.0f);
	
	Matrix.multiplyMM(modelViewProjectionMatrix, 0, perspectiveProjectionMatrix, 0, modelViewMatrix, 0);

	GLES31.glUniformMatrix4fv(mvpUnifrom, 1, false, modelViewProjectionMatrix, 0);
	GLES31.glBindVertexArray(vaoSmiley[0]);

	float smileyTexoords[] = new float[8];

	float smileyTexcoordsSquare[] = {
		1.0f,1.0f,
		0.0f,1.0f,
		0.0f,0.0f,
		1.0f,0.0f,
	};

	
	if(singleTap == 1)
	{
			smileyTexoords[0] = 0.5f;
			smileyTexoords[1] = 0.5f;
			smileyTexoords[2] = 0.0f;
			smileyTexoords[3] = 0.5f;

			smileyTexoords[4] = 0.0f;
			smileyTexoords[5] = 0.0f;

			smileyTexoords[6] = 0.5f;
			smileyTexoords[7] = 0.0f;

		GLES31.glBindBuffer(GLES31.GL_ARRAY_BUFFER, vbo_Smiley_Texture[0]);
		ByteBuffer byteBuffer = ByteBuffer.allocateDirect(smileyTexoords.length * 4);
		byteBuffer.order(ByteOrder.nativeOrder());
		FloatBuffer verticesBuffer = byteBuffer.asFloatBuffer();
		verticesBuffer.put(smileyTexoords);
		verticesBuffer.position(0);
		GLES31.glBufferData(GLES31.GL_ARRAY_BUFFER, smileyTexoords.length * 4, verticesBuffer, GLES31.GL_DYNAMIC_DRAW);
		GLES31.glBindBuffer(GLES31.GL_ARRAY_BUFFER, 0);
		GLES31.glActiveTexture(GLES31.GL_TEXTURE0);
		GLES31.glBindTexture(GLES31.GL_TEXTURE_2D, texture_smiley[0]);
		GLES31.glUniform1i(texture0_sampler_uniform, 0);
		GLES31.glDrawArrays(GLES31.GL_TRIANGLE_FAN, 0, 4);
	}

	else if(singleTap == 2)
	{
			smileyTexoords[0] = 1.0f;
			smileyTexoords[1] = 1.0f;

			smileyTexoords[2] = 0.0f;
			smileyTexoords[3] = 1.0f;

			smileyTexoords[4] = 0.0f;
			smileyTexoords[5] = 0.0f;

			smileyTexoords[6] = 1.0f;
			smileyTexoords[7] = 0.0f;

		GLES31.glBindBuffer(GLES31.GL_ARRAY_BUFFER, vbo_Smiley_Texture[0]);
		ByteBuffer byteBuffer = ByteBuffer.allocateDirect(smileyTexoords.length * 4);
		byteBuffer.order(ByteOrder.nativeOrder());
		FloatBuffer verticesBuffer = byteBuffer.asFloatBuffer();
		verticesBuffer.put(smileyTexoords);
		verticesBuffer.position(0);
		GLES31.glBufferData(GLES31.GL_ARRAY_BUFFER, smileyTexoords.length * 4, verticesBuffer, GLES31.GL_DYNAMIC_DRAW);
		GLES31.glBindBuffer(GLES31.GL_ARRAY_BUFFER, 0);
		GLES31.glActiveTexture(GLES31.GL_TEXTURE0);
		GLES31.glBindTexture(GLES31.GL_TEXTURE_2D, texture_smiley[0]);
		GLES31.glUniform1i(texture0_sampler_uniform, 0);
		GLES31.glDrawArrays(GLES31.GL_TRIANGLE_FAN, 0, 4);
	}

	else if(singleTap == 3)
	{
			smileyTexoords[0] = 2.0f;
			smileyTexoords[1] = 2.0f;

			smileyTexoords[2] = 0.0f;
			smileyTexoords[3] = 2.0f;

			smileyTexoords[4] = 0.0f;
			smileyTexoords[5] = 0.0f;

			smileyTexoords[6] = 2.0f;
			smileyTexoords[7] = 0.0f;


		GLES31.glBindBuffer(GLES31.GL_ARRAY_BUFFER, vbo_Smiley_Texture[0]);
		ByteBuffer byteBuffer = ByteBuffer.allocateDirect(smileyTexoords.length * 4);
		byteBuffer.order(ByteOrder.nativeOrder());
		FloatBuffer verticesBuffer = byteBuffer.asFloatBuffer();
		verticesBuffer.put(smileyTexoords);
		verticesBuffer.position(0);
		GLES31.glBufferData(GLES31.GL_ARRAY_BUFFER, smileyTexoords.length * 4, verticesBuffer, GLES31.GL_DYNAMIC_DRAW);
		GLES31.glBindBuffer(GLES31.GL_ARRAY_BUFFER, 0);
		GLES31.glActiveTexture(GLES31.GL_TEXTURE0);
		GLES31.glBindTexture(GLES31.GL_TEXTURE_2D, texture_smiley[0]);
		GLES31.glUniform1i(texture0_sampler_uniform, 0);
		GLES31.glDrawArrays(GLES31.GL_TRIANGLE_FAN, 0, 4);
	}

	else if(singleTap == 4)
	{
			smileyTexoords[0] = 0.5f;
			smileyTexoords[1] = 0.5f;

			smileyTexoords[2] = 0.5f;
			smileyTexoords[3] = 0.5f;

			smileyTexoords[4] = 0.5f;
			smileyTexoords[5] = 0.5f;

			smileyTexoords[6] = 0.5f;
			smileyTexoords[7] = 0.5f;

		GLES31.glBindBuffer(GLES31.GL_ARRAY_BUFFER, vbo_Smiley_Texture[0]);
		ByteBuffer byteBuffer = ByteBuffer.allocateDirect(smileyTexoords.length * 4);
		byteBuffer.order(ByteOrder.nativeOrder());
		FloatBuffer verticesBuffer = byteBuffer.asFloatBuffer();
		verticesBuffer.put(smileyTexoords);
		verticesBuffer.position(0);
		GLES31.glBufferData(GLES31.GL_ARRAY_BUFFER, smileyTexoords.length * 4, verticesBuffer, GLES31.GL_DYNAMIC_DRAW);
		GLES31.glBindBuffer(GLES31.GL_ARRAY_BUFFER, 0);
		GLES31.glActiveTexture(GLES31.GL_TEXTURE0);
		GLES31.glBindTexture(GLES31.GL_TEXTURE_2D, texture_smiley[0]);
		GLES31.glUniform1i(texture0_sampler_uniform, 0);
		GLES31.glDrawArrays(GLES31.GL_TRIANGLE_FAN, 0, 4);
	}

	else
	{
		GLES31.glBindBuffer(GLES31.GL_ARRAY_BUFFER, vbo_Smiley_Texture[0]);
		ByteBuffer byteBuffer = ByteBuffer.allocateDirect(smileyTexcoordsSquare.length * 4);
		byteBuffer.order(ByteOrder.nativeOrder());
		FloatBuffer verticesBuffer = byteBuffer.asFloatBuffer();
		verticesBuffer.put(smileyTexoords);
		verticesBuffer.position(0);
		GLES31.glBufferData(GLES31.GL_ARRAY_BUFFER, smileyTexcoordsSquare.length * 4, verticesBuffer, GLES31.GL_DYNAMIC_DRAW);
		GLES31.glBindBuffer(GLES31.GL_ARRAY_BUFFER, 0);
		GLES31.glActiveTexture(GLES31.GL_TEXTURE0);
		GLES31.glBindTexture(GLES31.GL_TEXTURE_2D, texName[0]);
		GLES31.glUniform1i(texture0_sampler_uniform, 0);
		GLES31.glDrawArrays(GLES31.GL_TRIANGLE_FAN, 0, 4);
	}

	GLES31.glBindVertexArray(0);

	GLES31.glUseProgram(0);
	requestRender();
}

void uninitialize()
{
	
	if(vaoSmiley[0] != 0)
	{
		GLES31.glDeleteVertexArrays(1, vaoSmiley, 0);
		vaoSmiley[0] = 0;
	}

	if(vbo_Smiley_Position[0] != 0)
	{
		GLES31.glDeleteVertexArrays(1, vbo_Smiley_Position, 0);
		vbo_Smiley_Position[0] = 0;
	}

	if(vbo_Smiley_Texture[0] != 0)
	{
		GLES31.glDeleteVertexArrays(1, vbo_Smiley_Texture, 0);
		vbo_Smiley_Texture[0] = 0;
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

	if(texture_smiley[0] != 0)
	{
		GLES31.glDeleteTextures(1, texture_smiley, 0);
		texture_smiley[0] = 0;
	}

}

}
