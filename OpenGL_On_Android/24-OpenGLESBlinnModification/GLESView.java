package com.example.sknaa.OGLESPhongLightingBlinnModification;

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

	private int numVertices;
	private int numElements;

	private float angle_cube = 0.0f;

	private int[] vao_sphere = new int[1];
    private int[] vbo_sphere_position = new int[1];
    private int[] vbo_sphere_normal = new int[1];
    private int[] vbo_sphere_element = new int[1];

    private float light_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f};
    private float light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f};
    private float light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f};
    private float light_position[] = { 100.0f, 100.0f, 100.0f, 1.0f};

    private float material_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f};
    private float material_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f};
    private float material_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f};
    private float material_shininess = 50.0f;


	private int modelMatrixUniform, viewMatrixUniform, projectionMatrixUniform;
	private int laUniform, ldUniform, lsUniform, kdUniform, lightPositionUniform;
	private int kaUniform, ksUniform, materialShininessUniform;
	private int doubleTapUniform;

	private float perspectiveProjectionMatrix[] = new float[16];

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
		"uniform vec4 u_light_position;"+
		"out vec3 transformed_normals;"+
		"out vec3 light_direction;"+
		"out vec3 viewer_vector;"+
		"void main(void)"+
		"{"+
		"if(u_double_tap == 1)"+
		"{"+
		"vec4 eyeCoordinates = u_view_matrix * u_model_matrix * vPosition;"+
		"transformed_normals = (mat3(u_view_matrix * u_model_matrix) * vNormal);"+
		"light_direction = (vec3(u_light_position) - eyeCoordinates.xyz);"+
		"viewer_vector = -eyeCoordinates.xyz;"+
		"}"+
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
		"in vec3 transformed_normals;"+
		"in vec3 light_direction;"+
		"in vec3 viewer_vector;"+
		"out vec4 FragColor;"+
		"uniform vec3 u_La;"+
		"uniform vec3 u_Ld;"+
		"uniform vec3 u_Ls;"+
		"uniform vec3 u_Ka;"+
		"uniform vec3 u_Ks;"+
		"uniform vec3 u_Kd;"+
		"uniform float u_material_shininess;"+
		"uniform int u_double_tap;"+
		"void main(void)"+
		"{"+
		"vec3 phong_ads_color;"+
		"if(u_double_tap == 1)"+
		"{"+
		"vec3 normalized_transformed_normals = normalize(transformed_normals);"+
		"vec3 normalized_light_direction = normalize(light_direction);"+
		"vec3 normalized_view_vector = normalize(viewer_vector);"+
		"vec3 ambient = u_La * u_Ka;"+
		"float tn_dot_ld = max(dot(normalized_transformed_normals, normalized_light_direction), 0.0);"+
		"vec3 diffuse = u_Ld * u_Kd * tn_dot_ld;"+
		//"vec3 reflection_vector = reflect(-normalized_light_direction, normalized_transformed_normals);"+
		"vec3 halfVector = normalize((-normalized_light_direction + normalized_view_vector));"+
		"vec3 specular = u_Ls * u_Ks * pow(max(dot(halfVector, normalized_view_vector), 0.0), u_material_shininess);"+
		"phong_ads_color = ambient + diffuse + specular;"+
		"}"+
		"else"+
		"{"+
		"phong_ads_color = vec3(1.0 ,1.0, 1.0);"+
		"}"+
		"FragColor = vec4(phong_ads_color, 1.0);"+
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

	doubleTapUniform = GLES31.glGetUniformLocation(shaderProgramObject, "u_double_tap");

	laUniform = GLES31.glGetUniformLocation(shaderProgramObject, "u_La");
	kaUniform = GLES31.glGetUniformLocation(shaderProgramObject, "u_Ka");

	ldUniform = GLES31.glGetUniformLocation(shaderProgramObject, "u_Ld");
	kdUniform = GLES31.glGetUniformLocation(shaderProgramObject, "u_Kd");

	lsUniform = GLES31.glGetUniformLocation(shaderProgramObject, "u_Ls");
	ksUniform = GLES31.glGetUniformLocation(shaderProgramObject, "u_Ks");

	lightPositionUniform = GLES31.glGetUniformLocation(shaderProgramObject, "u_light_position");

	materialShininessUniform = GLES31.glGetUniformLocation(shaderProgramObject, "u_material_shininess");

	Sphere sphere=new Sphere();
    float sphere_vertices[]=new float[1146];
    float sphere_normals[]=new float[1146];
    float sphere_textures[]=new float[764];
    short sphere_elements[]=new short[2280];

    sphere.getSphereVertexData(sphere_vertices, sphere_normals, sphere_textures, sphere_elements);
    numVertices = sphere.getNumberOfSphereVertices();
    numElements = sphere.getNumberOfSphereElements();
	
	 // vao
        GLES31.glGenVertexArrays(1,vao_sphere,0);
        GLES31.glBindVertexArray(vao_sphere[0]);
        
        // position vbo
        GLES31.glGenBuffers(1,vbo_sphere_position,0);
        GLES31.glBindBuffer(GLES31.GL_ARRAY_BUFFER,vbo_sphere_position[0]);
        
        ByteBuffer byteBuffer=ByteBuffer.allocateDirect(sphere_vertices.length * 4);
        byteBuffer.order(ByteOrder.nativeOrder());
        FloatBuffer verticesBuffer=byteBuffer.asFloatBuffer();
        verticesBuffer.put(sphere_vertices);
        verticesBuffer.position(0);
        
        GLES31.glBufferData(GLES31.GL_ARRAY_BUFFER,
                            sphere_vertices.length * 4,
                            verticesBuffer,
                            GLES31.GL_STATIC_DRAW);
        
        GLES31.glVertexAttribPointer(GLESMacros.ARD_ATTRIBUTE_VERTEX,
                                     3,
                                     GLES31.GL_FLOAT,
                                     false,0,0);
        
        GLES31.glEnableVertexAttribArray(GLESMacros.ARD_ATTRIBUTE_VERTEX);
        
        GLES31.glBindBuffer(GLES31.GL_ARRAY_BUFFER,0);
        
        // normal vbo
        GLES31.glGenBuffers(1,vbo_sphere_normal,0);
        GLES31.glBindBuffer(GLES31.GL_ARRAY_BUFFER,vbo_sphere_normal[0]);
        
        byteBuffer=ByteBuffer.allocateDirect(sphere_normals.length * 4);
        byteBuffer.order(ByteOrder.nativeOrder());
        verticesBuffer=byteBuffer.asFloatBuffer();
        verticesBuffer.put(sphere_normals);
        verticesBuffer.position(0);
        
        GLES31.glBufferData(GLES31.GL_ARRAY_BUFFER,
                            sphere_normals.length * 4,
                            verticesBuffer,
                            GLES31.GL_STATIC_DRAW);
        
        GLES31.glVertexAttribPointer(GLESMacros.ARD_ATTRIBUTE_NORMAL,
                                     3,
                                     GLES31.GL_FLOAT,
                                     false,0,0);
        
        GLES31.glEnableVertexAttribArray(GLESMacros.ARD_ATTRIBUTE_NORMAL);
        
        GLES31.glBindBuffer(GLES31.GL_ARRAY_BUFFER,0);
        
        // element vbo
        GLES31.glGenBuffers(1,vbo_sphere_element,0);
        GLES31.glBindBuffer(GLES31.GL_ELEMENT_ARRAY_BUFFER,vbo_sphere_element[0]);
        
        byteBuffer=ByteBuffer.allocateDirect(sphere_elements.length * 2);
        byteBuffer.order(ByteOrder.nativeOrder());
        ShortBuffer elementsBuffer=byteBuffer.asShortBuffer();
        elementsBuffer.put(sphere_elements);
        elementsBuffer.position(0);
        
        GLES31.glBufferData(GLES31.GL_ELEMENT_ARRAY_BUFFER,
                            sphere_elements.length * 2,
                            elementsBuffer,
                            GLES31.GL_STATIC_DRAW);
        
        GLES31.glBindBuffer(GLES31.GL_ELEMENT_ARRAY_BUFFER,0);

        GLES31.glBindVertexArray(0);


	GLES31.glEnable(GLES31.GL_DEPTH_TEST);
	GLES31.glDepthFunc(GLES31.GL_LEQUAL);
	//GLES31.glEnable(GLES31.GL_CULL_FACE);
	GLES31.glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

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
		GLES31.glUniform3fv(laUniform, 1, light_ambient, 0);
		GLES31.glUniform3fv(ldUniform, 1, light_diffuse, 0);
		GLES31.glUniform3fv(lsUniform, 1, light_specular, 0);
		
		GLES31.glUniform3fv(kaUniform, 1, material_ambient, 0);
		GLES31.glUniform3fv(kdUniform, 1, material_diffuse, 0);
		GLES31.glUniform3fv(ksUniform, 1, material_specular, 0);
		
		GLES31.glUniform1f(materialShininessUniform, material_shininess);
		GLES31.glUniform4fv(lightPositionUniform, 1, light_position, 0);
	}

	else
	{
		GLES31.glUniform1i(doubleTapUniform, 0);
	}

	float modelMatrix[] = new float[16];
	float viewMatrix[] = new float[16];


	Matrix.setIdentityM(modelMatrix, 0);
	Matrix.setIdentityM(viewMatrix, 0);
	
	Matrix.translateM(modelMatrix, 0, 0.0f, 0.0f, -2.0f);
	//Matrix.rotateM(rotationMatrix, 0, angle_cube, 1.0f, 1.0f, 1.0f);
	
	GLES31.glUniformMatrix4fv(modelMatrixUniform, 1, false, modelMatrix, 0);
	GLES31.glUniformMatrix4fv(viewMatrixUniform, 1, false, viewMatrix, 0);
	GLES31.glUniformMatrix4fv(projectionMatrixUniform, 1, false, perspectiveProjectionMatrix, 0);
	GLES31.glBindVertexArray(vao_sphere[0]);
        
        // *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
    GLES31.glBindBuffer(GLES31.GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element[0]);
    GLES31.glDrawElements(GLES31.GL_TRIANGLES, numElements, GLES31.GL_UNSIGNED_SHORT, 0);
        
        // unbind vao
    GLES31.glBindVertexArray(0);
	GLES31.glUseProgram(0);
	requestRender();
}

void uninitialize()
{
	

	if(vao_sphere[0] != 0)
	{
		GLES31.glDeleteVertexArrays(1, vao_sphere, 0);
		vao_sphere[0] = 0;
	}

	if(vbo_sphere_position[0] != 0)
	{
		GLES31.glDeleteVertexArrays(1, vbo_sphere_position, 0);
		vbo_sphere_position[0] = 0;
	}

	if(vbo_sphere_normal[0] != 0)
	{
		GLES31.glDeleteVertexArrays(1, vbo_sphere_normal, 0);
		vbo_sphere_normal[0] = 0;
	}

	if(vbo_sphere_element[0] != 0)
	{
		GLES31.glDeleteVertexArrays(1, vbo_sphere_element, 0);
		vbo_sphere_element[0] = 0;
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
}
