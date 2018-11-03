package com.example.sknaa.OGLESStaticSphereRotLights;

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
import java.lang.*;
import java.lang.Math;

import android.opengl.Matrix;

public class GLESView extends GLSurfaceView implements GLSurfaceView.Renderer, OnGestureListener, OnDoubleTapListener
{
	private final Context context;

	private GestureDetector gestureDetector;

	private int vertexShaderObjectPhongFragment;
	private int fragmentShaderObjectPhongFragment;

	private int vertexShaderObjectPhongVertex;
	private int fragmentShaderObjectPhongVertex;

	private int shaderProgramObjectPhongVertex;
	private int shaderProgramObjectPhongFragment;

	private int numVertices;
	private int numElements;

	private float angle_red = 0.0f;
	private float angle_green = 100.0f;
	private float angle_blue = 100.0f;

	private int[] vao_sphere = new int[1];
    private int[] vbo_sphere_position = new int[1];
    private int[] vbo_sphere_normal = new int[1];
    private int[] vbo_sphere_element = new int[1];

    private float light0_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f};
    private float light0_diffuse[] = { 1.0f, 0.0f, 0.0f, 1.0f};
    private float light0_specular[] = { 1.0f, 0.0f, 0.0f, 1.0f};
   // private float light0_position[] = { angle_red, 100.0f, 0.0f, 1.0f };
    private float[] light0_position = new float[4];

    private float light1_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f};
    private float light1_diffuse[] = { 0.0f, 1.0f, 0.0f, 1.0f};
    private float light1_specular[] = { 0.0f, 1.0f, 0.0f, 1.0f};
    //private float light1_position[] = { 0.0f, angle_green, 100.0f, 1.0f };
     private float[] light1_position = new float[4];

    private float light2_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f};
    private float light2_diffuse[] = { 0.0f, 0.0f, 1.0f, 1.0f};
    private float light2_specular[] = { 0.0f, 0.0f, 1.0f, 1.0f};
   // private float light2_position[] = { 0.0f, 100.0f, angle_blue, 1.0f };
    private float[] light2_position = new float[4];

    private float material_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f};
    private float material_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f};
    private float material_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f};
    private float material_shininess = 50.0f;


	private int modelMatrix0Uniform, viewMatrix0Uniform, projectionMatrix0Uniform;
	private int la0Uniform, ld0Uniform, ls0Uniform, kd0Uniform, light0PositionUniform;
	private int ka0Uniform, ks0Uniform, material0ShininessUniform;

	private int doubleTap0Uniform;

	private int modelMatrix1Uniform, viewMatrix1Uniform, projectionMatrix1Uniform;
	private int la1Uniform, ld1Uniform, ls1Uniform, kd1Uniform, light1PositionUniform;
	private int ka1Uniform, ks1Uniform, material1ShininessUniform;
	//private int doubleTap1Uniform;

	//private int modelMatrix2Uniform, viewMatrix2Uniform, projectionMatrix2Uniform;
	private int la2Uniform, ld2Uniform, ls2Uniform, kd2Uniform, light2PositionUniform;
	private int ka2Uniform, ks2Uniform, material2ShininessUniform;
	//private int doubleTap2Uniform;



	//private int modelMatrix0Uniformf, viewMatrix0Uniformf, projectionMatrix0Uniformf;
	private int la0Uniformf, ld0Uniformf, ls0Uniformf, kd0Uniformf, light0PositionUniformf;
	private int ka0Uniformf, ks0Uniformf, material0ShininessUniformf;

	private int doubleTap1Uniform;

	//private int modelMatrix1Uniformf, viewMatrix1Uniformf, projectionMatrix1Uniformf;
	private int la1Uniformf, ld1Uniformf, ls1Uniformf, kd1Uniformf, light1PositionUniformf;
	private int ka1Uniformf, ks1Uniformf, material1ShininessUniformf;
	//private int doubleTap1Uniform;

	//private int modelMatrix2Uniformf, viewMatrix2Uniformf, projectionMatrix2Uniformf;
	private int la2Uniformf, ld2Uniformf, ls2Uniformf, kd2Uniformf, light2PositionUniformf;
	private int ka2Uniformf, ks2Uniformf, material2ShininessUniformf;
	//private int doubleTap2Uniform;

	private float perspectiveProjectionMatrix[] = new float[16];

	private int doubleTap;

	private int singleTap = 0;

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
	vertexShaderObjectPhongVertex = GLES31.glCreateShader(GLES31.GL_VERTEX_SHADER);
	final String vertexShaderSourceCodePhongVertex = String.format
	(
		"#version 310 es"+
		"\n"+
		"in vec4 vPosition;"+
		"in vec3 vNormal;"+
		"uniform mat4 u_model_matrix;"+
		"uniform mat4 u_view_matrix;"+
		"uniform mat4 u_projection_matrix;"+
		"uniform vec3 u_La0;"+
		"uniform vec3 u_La1;"+
		"uniform vec3 u_La2;"+
		"uniform vec3 u_Ka0;"+
		"uniform vec3 u_Ka1;"+
		"uniform vec3 u_Ka2;"+
		"uniform vec3 u_Ld0;"+
		"uniform vec3 u_Ld1;"+
		"uniform vec3 u_Ld2;"+
		"uniform vec3 u_Kd0;"+
		"uniform vec3 u_Kd1;"+
		"uniform vec3 u_Kd2;"+
		"uniform vec3 u_Ls0;"+
		"uniform vec3 u_Ls1;"+
		"uniform vec3 u_Ls2;"+
		"uniform vec3 u_Ks0;"+
		"uniform vec3 u_Ks1;"+
		"uniform vec3 u_Ks2;"+
		"uniform vec4 u_light0_position;"+
		"uniform vec4 u_light1_position;"+
		"uniform vec4 u_light2_position;"+
		"uniform float u_material0_shininess;"+
		"uniform float u_material1_shininess;"+
		"uniform float u_material2_shininess;"+
		"out vec3 phong_ads_color0;"+
		"out vec3 phong_ads_color1;"+
		"out vec3 phong_ads_color2;"+
		"out vec3 phong_ads_color;"+
		"void main(void)"+
		"{"+
		"vec4 eyeCoordinates = u_view_matrix * u_model_matrix * vPosition;"+

		"vec3 transformed_normals = normalize(mat3(u_view_matrix * u_model_matrix) * vNormal);"+

		"vec3 light_direction0 = normalize(vec3(u_light0_position) - eyeCoordinates.xyz);"+
		"vec3 light_direction1 = normalize(vec3(u_light1_position) - eyeCoordinates.xyz);"+
		"vec3 light_direction2 = normalize(vec3(u_light2_position) - eyeCoordinates.xyz);"+

		"float tn_dot_ld0 = max(dot(transformed_normals, light_direction0), 0.0);"+
		"float tn_dot_ld1 = max(dot(transformed_normals, light_direction1), 0.0);"+
		"float tn_dot_ld2 = max(dot(transformed_normals, light_direction2), 0.0);"+

		"vec3 ambient0 = u_La0 * u_Ka0;"+
		"vec3 diffuse0 = u_Ld0 * u_Kd0 * tn_dot_ld0;"+
		"vec3 reflection_vector0 = reflect(-light_direction0, transformed_normals);"+

		"vec3 ambient1 = u_La1 * u_Ka1;"+
		"vec3 diffuse1 = u_Ld1 * u_Kd1 * tn_dot_ld1;"+
		"vec3 reflection_vector1 = reflect(-light_direction1, transformed_normals);"+

		"vec3 ambient2 = u_La2 * u_Ka2;"+
		"vec3 diffuse2 = u_Ld2 * u_Kd2 * tn_dot_ld2;"+
		"vec3 reflection_vector2 = reflect(-light_direction2, transformed_normals);"+


		"vec3 viewer_vector = normalize(-eyeCoordinates.xyz);"+

		"vec3 specular0 = u_Ls0 * u_Ks0 * pow(max(dot(reflection_vector0, viewer_vector), 0.0), u_material0_shininess);"+
		"phong_ads_color0 = ambient0 + diffuse0 + specular0;"+

		"vec3 specular1 = u_Ls1 * u_Ks1 * pow(max(dot(reflection_vector1, viewer_vector), 0.0), u_material1_shininess);"+
		"phong_ads_color1 = ambient1 + diffuse1 + specular1;"+


		"vec3 specular2 = u_Ls2 * u_Ks2 * pow(max(dot(reflection_vector2, viewer_vector), 0.0), u_material2_shininess);"+
		"phong_ads_color2 = ambient2 + diffuse2 + specular2;"+
		"gl_Position = u_projection_matrix * u_view_matrix * u_model_matrix * vPosition;"+
		"}"
	);

	GLES31.glShaderSource(vertexShaderObjectPhongVertex, vertexShaderSourceCodePhongVertex);
	GLES31.glCompileShader(vertexShaderObjectPhongVertex);
	int[] iShaderCompiledStatus = new int[1];
	int[] iInfoLogLength = new int[1];
	String szInfoLog = null;
	GLES31.glGetShaderiv(vertexShaderObjectPhongVertex, GLES31.GL_COMPILE_STATUS, iShaderCompiledStatus, 0);
	if(iShaderCompiledStatus[0] == GLES31.GL_FALSE)
	{
		GLES31.glGetShaderiv(vertexShaderObjectPhongVertex, GLES31.GL_INFO_LOG_LENGTH, iInfoLogLength, 0);

		if(iInfoLogLength[0] > 0)
		{
			szInfoLog = GLES31.glGetShaderInfoLog(vertexShaderObjectPhongVertex);
			System.out.println("OGLES: Vertex Shader Compilation Log Per Vertex = "+szInfoLog);
			uninitialize();
			System.exit(0);
		}
	}	

	fragmentShaderObjectPhongVertex = GLES31.glCreateShader(GLES31.GL_FRAGMENT_SHADER);
	final String fragmentShaderSourceCodePhongVertex = String.format
	(
		"#version 310 es"+
		"\n"+
		"precision highp float;"+
		"in vec3 phong_ads_color0;"+
		"in vec3 phong_ads_color1;"+
		"in vec3 phong_ads_color2;"+
		"out vec4 FragColor;"+
		"void main(void)"+
		"{"+
		"vec4 color;"+
		"color = vec4(phong_ads_color0, 1.0) + vec4(phong_ads_color1, 1.0) + vec4(phong_ads_color2, 1.0);"+
		"FragColor = color;"+
		"}"
	);

	GLES31.glShaderSource(fragmentShaderObjectPhongVertex,fragmentShaderSourceCodePhongVertex);
	GLES31.glCompileShader(fragmentShaderObjectPhongVertex);
	iShaderCompiledStatus[0] = 0;
	iInfoLogLength[0] = 0;
	szInfoLog = null;
	GLES31.glGetShaderiv(fragmentShaderObjectPhongVertex,GLES31.GL_COMPILE_STATUS,iShaderCompiledStatus, 0);
	if(iShaderCompiledStatus[0] == GLES31.GL_FALSE)
	{
		GLES31.glGetShaderiv(fragmentShaderObjectPhongVertex, GLES31.GL_INFO_LOG_LENGTH, iInfoLogLength, 0);
		if(iInfoLogLength[0] > 0)
		{
			szInfoLog = GLES31.glGetShaderInfoLog(fragmentShaderObjectPhongVertex);
			System.out.println("OGLES: Fragment Shader Compilation Log = "+szInfoLog);
			uninitialize();
			System.exit(0);
		}
	}

	shaderProgramObjectPhongVertex = GLES31.glCreateProgram();
	GLES31.glAttachShader(shaderProgramObjectPhongVertex,vertexShaderObjectPhongVertex);
	GLES31.glAttachShader(shaderProgramObjectPhongVertex,fragmentShaderObjectPhongVertex);
	GLES31.glBindAttribLocation(shaderProgramObjectPhongVertex,GLESMacros.ARD_ATTRIBUTE_VERTEX,"vPosition");
	GLES31.glBindAttribLocation(shaderProgramObjectPhongVertex,GLESMacros.ARD_ATTRIBUTE_NORMAL, "vNormal");
	GLES31.glLinkProgram(shaderProgramObjectPhongVertex);
	int[] iShaderProgramLinkStatus = new int[1];
	iInfoLogLength[0] = 0;
	szInfoLog = null;
	GLES31.glGetProgramiv(shaderProgramObjectPhongVertex, GLES31.GL_LINK_STATUS, iShaderProgramLinkStatus, 0);
	if(iShaderProgramLinkStatus[0] == GLES31.GL_FALSE)
	{
		GLES31.glGetProgramiv(shaderProgramObjectPhongVertex, GLES31.GL_INFO_LOG_LENGTH, iInfoLogLength, 0);
		if(iInfoLogLength[0] > 0)
		{
			szInfoLog = GLES31.glGetProgramInfoLog(shaderProgramObjectPhongVertex);
			System.out.println("OGLES: Shader Program Link Log = "+szInfoLog);
			uninitialize();
			System.exit(0);
		}
	}

	modelMatrix0Uniform = GLES31.glGetUniformLocation(shaderProgramObjectPhongVertex, "u_model_matrix");
	viewMatrix0Uniform = GLES31.glGetUniformLocation(shaderProgramObjectPhongVertex, "u_view_matrix");
	projectionMatrix0Uniform = GLES31.glGetUniformLocation(shaderProgramObjectPhongVertex, "u_projection_matrix");

	//doubleTap0Uniform = GLES31.glGetUniformLocation(shaderProgramObjectPhongVertex, "u_double_tap");

	la0Uniform = GLES31.glGetUniformLocation(shaderProgramObjectPhongVertex, "u_La0");
	ka0Uniform = GLES31.glGetUniformLocation(shaderProgramObjectPhongVertex, "u_Ka0");

	ld0Uniform = GLES31.glGetUniformLocation(shaderProgramObjectPhongVertex, "u_Ld0");
	kd0Uniform = GLES31.glGetUniformLocation(shaderProgramObjectPhongVertex, "u_Kd0");

	ls0Uniform = GLES31.glGetUniformLocation(shaderProgramObjectPhongVertex, "u_Ls0");
	ks0Uniform = GLES31.glGetUniformLocation(shaderProgramObjectPhongVertex, "u_Ks0");

	light0PositionUniform = GLES31.glGetUniformLocation(shaderProgramObjectPhongVertex, "u_light0_position");

	material0ShininessUniform = GLES31.glGetUniformLocation(shaderProgramObjectPhongVertex, "u_material0_shininess");



	modelMatrix1Uniform = GLES31.glGetUniformLocation(shaderProgramObjectPhongVertex, "u_model_matrix");
	viewMatrix1Uniform = GLES31.glGetUniformLocation(shaderProgramObjectPhongVertex, "u_view_matrix");
	projectionMatrix1Uniform = GLES31.glGetUniformLocation(shaderProgramObjectPhongVertex, "u_projection_matrix");

	//doubleTap1Uniform = GLES31.glGetUniformLocation(shaderProgramObjectPhongVertex, "u_double_tap");

	la1Uniform = GLES31.glGetUniformLocation(shaderProgramObjectPhongVertex, "u_La1");
	ka1Uniform = GLES31.glGetUniformLocation(shaderProgramObjectPhongVertex, "u_Ka1");

	ld1Uniform = GLES31.glGetUniformLocation(shaderProgramObjectPhongVertex, "u_Ld1");
	kd1Uniform = GLES31.glGetUniformLocation(shaderProgramObjectPhongVertex, "u_Kd1");

	ls1Uniform = GLES31.glGetUniformLocation(shaderProgramObjectPhongVertex, "u_Ls1");
	ks1Uniform = GLES31.glGetUniformLocation(shaderProgramObjectPhongVertex, "u_Ks1");

	light1PositionUniform = GLES31.glGetUniformLocation(shaderProgramObjectPhongVertex, "u_light1_position");
	material1ShininessUniform = GLES31.glGetUniformLocation(shaderProgramObjectPhongVertex, "u_material1_shininess");


	//modelMatrix2Uniform = GLES31.glGetUniformLocation(shaderProgramObjectPhongVertex, "u_model_matrix");
	//viewMatrix2Uniform = GLES31.glGetUniformLocation(shaderProgramObjectPhongVertex, "u_view_matrix");
	//projectionMatrix2Uniform = GLES31.glGetUniformLocation(shaderProgramObjectPhongVertex, "u_projection_matrix");

	//doubleTap2Uniform = GLES31.glGetUniformLocation(shaderProgramObjectPhongVertex, "u_double_tap");

	la2Uniform = GLES31.glGetUniformLocation(shaderProgramObjectPhongVertex, "u_La2");
	ka2Uniform = GLES31.glGetUniformLocation(shaderProgramObjectPhongVertex, "u_Ka2");

	ld2Uniform = GLES31.glGetUniformLocation(shaderProgramObjectPhongVertex, "u_Ld2");
	kd2Uniform = GLES31.glGetUniformLocation(shaderProgramObjectPhongVertex, "u_Kd2");

	ls2Uniform = GLES31.glGetUniformLocation(shaderProgramObjectPhongVertex, "u_Ls2");
	ks2Uniform = GLES31.glGetUniformLocation(shaderProgramObjectPhongVertex, "u_Ks2");

	light2PositionUniform = GLES31.glGetUniformLocation(shaderProgramObjectPhongVertex, "u_light2_position");

	material2ShininessUniform = GLES31.glGetUniformLocation(shaderProgramObjectPhongVertex, "u_material2_shininess");

//2nd Program Object

	vertexShaderObjectPhongFragment = GLES31.glCreateShader(GLES31.GL_VERTEX_SHADER);
	final String vertexShaderSourceCode = String.format
	(
		"#version 310 es"+
		"\n"+
		"in vec4 vPosition;"+
		"in vec3 vNormal;"+
		"uniform mat4 u_model_matrix;"+
		"uniform mat4 u_view_matrix;"+
		"uniform mat4 u_projection_matrix;"+
		"uniform vec4 u_light0_position;"+
		"uniform vec4 u_light1_position;"+
		"uniform vec4 u_light2_position;"+
		"out vec3 transformed_normals;"+
		"out vec3 light_direction0;"+
		"out vec3 light_direction1;"+
		"out vec3 light_direction2;"+
		"out vec3 viewer_vector;"+
		"void main(void)"+
		"{"+
		"vec4 eyeCoordinates = u_view_matrix * u_model_matrix * vPosition;"+
		"transformed_normals = normalize(mat3(u_view_matrix * u_model_matrix) * vNormal);"+
		"light_direction0 = normalize(vec3(u_light0_position) - eyeCoordinates.xyz);"+
		"light_direction1 = normalize(vec3(u_light1_position) - eyeCoordinates.xyz);"+
		"light_direction2 = normalize(vec3(u_light2_position) - eyeCoordinates.xyz);"+
		"viewer_vector = normalize(-eyeCoordinates.xyz);"+
		"gl_Position = u_projection_matrix * u_view_matrix * u_model_matrix * vPosition;"+
		"}"
	);

	GLES31.glShaderSource(vertexShaderObjectPhongFragment, vertexShaderSourceCode);
	GLES31.glCompileShader(vertexShaderObjectPhongFragment);
	iShaderCompiledStatus = new int[1];
	iInfoLogLength = new int[1];
	szInfoLog = null;
	GLES31.glGetShaderiv(vertexShaderObjectPhongFragment, GLES31.GL_COMPILE_STATUS, iShaderCompiledStatus, 0);
	if(iShaderCompiledStatus[0] == GLES31.GL_FALSE)
	{
		GLES31.glGetShaderiv(vertexShaderObjectPhongFragment, GLES31.GL_INFO_LOG_LENGTH, iInfoLogLength, 0);

		if(iInfoLogLength[0] > 0)
		{
			szInfoLog = GLES31.glGetShaderInfoLog(vertexShaderObjectPhongFragment);
			System.out.println("OGLES: Vertex Shader Compilation Log = "+szInfoLog);
			uninitialize();
			System.exit(0);
		}
	}	

	fragmentShaderObjectPhongFragment = GLES31.glCreateShader(GLES31.GL_FRAGMENT_SHADER);
	final String fragmentShaderSourceCode = String.format
	(
		"#version 310 es"+
		"\n"+
		"precision highp float;"+
		"in vec3 transformed_normals;"+
		"in vec3 light_direction0;"+
		"in vec3 light_direction1;"+
		"in vec3 light_direction2;"+
		"in vec3 viewer_vector;"+
		"out vec4 FragColor;"+
		"uniform vec3 u_La0;"+
		"uniform vec3 u_La1;"+
		"uniform vec3 u_La2;"+
		"uniform vec3 u_Ka0;"+
		"uniform vec3 u_Ka1;"+
		"uniform vec3 u_Ka2;"+
		"uniform vec3 u_Ld0;"+
		"uniform vec3 u_Ld1;"+
		"uniform vec3 u_Ld2;"+
		"uniform vec3 u_Kd0;"+
		"uniform vec3 u_Kd1;"+
		"uniform vec3 u_Kd2;"+
		"uniform vec3 u_Ls0;"+
		"uniform vec3 u_Ls1;"+
		"uniform vec3 u_Ls2;"+
		"uniform vec3 u_Ks0;"+
		"uniform vec3 u_Ks1;"+
		"uniform vec3 u_Ks2;"+
		"uniform vec4 u_light0_position;"+
		"uniform vec4 u_light1_position;"+
		"uniform vec4 u_light2_position;"+
		"uniform float u_material0_shininess;"+
		"uniform float u_material1_shininess;"+
		"uniform float u_material2_shininess;"+
		"void main(void)"+
		"{"+
		"vec3 phong_ads_color0;"+
		"vec3 phong_ads_color1;"+
		"vec3 phong_ads_color2;"+
		"vec3 phong_ads_color;"+
		"vec3 normalized_transformed_normals = normalize(transformed_normals);"+
		"vec3 normalized_light_direction0 = normalize(light_direction0);"+
		"vec3 normalized_light_direction1 = normalize(light_direction1);"+
		"vec3 normalized_light_direction2 = normalize(light_direction2);"+
		"vec3 normalized_viewer_vector1 = normalize(viewer_vector);"+
		"vec3 ambient0 = u_La0 * u_Ka0;"+
		"vec3 ambient1 = u_La1 * u_Ka1;"+
		"vec3 ambient2 = u_La2 * u_Ka2;"+

		"float tn_dot_ld0 = max(dot(normalized_transformed_normals, normalized_light_direction0), 0.0);"+
		"float tn_dot_ld1 = max(dot(normalized_transformed_normals, normalized_light_direction1), 0.0);"+
		"float tn_dot_ld2 = max(dot(normalized_transformed_normals, normalized_light_direction2), 0.0);"+

		"vec3 diffuse0 = u_Ld0 * u_Kd0 * tn_dot_ld0;"+
		"vec3 diffuse1 = u_Ld1 * u_Kd1 * tn_dot_ld1;"+
		"vec3 diffuse2 = u_Ld2 * u_Kd2 * tn_dot_ld2;"+

		"vec3 reflection0_vector = reflect(-normalized_light_direction0, normalized_transformed_normals);"+
		"vec3 reflection1_vector = reflect(-normalized_light_direction1, normalized_transformed_normals);"+
		"vec3 reflection2_vector = reflect(-normalized_light_direction2, normalized_transformed_normals);"+

		"vec3 specular0 = u_Ls0 * u_Ks0 * pow(max(dot(reflection0_vector, normalized_viewer_vector1), 0.0), u_material0_shininess);"+
		"vec3 specular1 = u_Ls1 * u_Ks1 * pow(max(dot(reflection1_vector, normalized_viewer_vector1), 0.0), u_material1_shininess);"+
		"vec3 specular2 = u_Ls2 * u_Ks2 * pow(max(dot(reflection2_vector, normalized_viewer_vector1), 0.0), u_material2_shininess);"+

		"phong_ads_color0 = ambient0 + diffuse0 + specular0;"+
		"phong_ads_color1 = ambient1 + diffuse1 + specular1;"+
		"phong_ads_color2 = ambient2 + diffuse2 + specular2;"+
	
		"FragColor = vec4(phong_ads_color0, 1.0) + vec4(phong_ads_color1, 1.0) + vec4(phong_ads_color2, 1.0);"+
		"}"
	);

	GLES31.glShaderSource(fragmentShaderObjectPhongFragment,fragmentShaderSourceCode);
	GLES31.glCompileShader(fragmentShaderObjectPhongFragment);
	iShaderCompiledStatus[0] = 0;
	iInfoLogLength[0] = 0;
	szInfoLog = null;
	GLES31.glGetShaderiv(fragmentShaderObjectPhongFragment,GLES31.GL_COMPILE_STATUS,iShaderCompiledStatus, 0);
	if(iShaderCompiledStatus[0] == GLES31.GL_FALSE)
	{
		GLES31.glGetShaderiv(fragmentShaderObjectPhongFragment, GLES31.GL_INFO_LOG_LENGTH, iInfoLogLength, 0);
		if(iInfoLogLength[0] > 0)
		{
			szInfoLog = GLES31.glGetShaderInfoLog(fragmentShaderObjectPhongFragment);
			System.out.println("OGLES: Fragment Shader Compilation Log = "+szInfoLog);
			uninitialize();
			System.exit(0);
		}
	}

	shaderProgramObjectPhongFragment = GLES31.glCreateProgram();
	GLES31.glAttachShader(shaderProgramObjectPhongFragment,vertexShaderObjectPhongFragment);
	GLES31.glAttachShader(shaderProgramObjectPhongFragment,fragmentShaderObjectPhongFragment);
	GLES31.glBindAttribLocation(shaderProgramObjectPhongFragment,GLESMacros.ARD_ATTRIBUTE_VERTEX,"vPosition");
	GLES31.glBindAttribLocation(shaderProgramObjectPhongFragment,GLESMacros.ARD_ATTRIBUTE_NORMAL, "vNormal");
	GLES31.glLinkProgram(shaderProgramObjectPhongFragment);
	iShaderProgramLinkStatus = new int[1];
	iInfoLogLength[0] = 0;
	szInfoLog = null;
	GLES31.glGetProgramiv(shaderProgramObjectPhongFragment, GLES31.GL_LINK_STATUS, iShaderProgramLinkStatus, 0);
	if(iShaderProgramLinkStatus[0] == GLES31.GL_FALSE)
	{
		GLES31.glGetProgramiv(shaderProgramObjectPhongFragment, GLES31.GL_INFO_LOG_LENGTH, iInfoLogLength, 0);
		if(iInfoLogLength[0] > 0)
		{
			szInfoLog = GLES31.glGetProgramInfoLog(shaderProgramObjectPhongFragment);
			System.out.println("OGLES: Shader Program Link Log = "+szInfoLog);
			uninitialize();
			System.exit(0);
		}
	}

	//doubleTap1Uniform = GLES31.glGetUniformLocation(shaderProgramObjectPhongFragment, "u_double_tap");

	la0Uniformf = GLES31.glGetUniformLocation(shaderProgramObjectPhongFragment, "u_La0");
	ka0Uniformf = GLES31.glGetUniformLocation(shaderProgramObjectPhongFragment, "u_Ka0");

	ld0Uniformf = GLES31.glGetUniformLocation(shaderProgramObjectPhongFragment, "u_Ld0");
	kd0Uniformf = GLES31.glGetUniformLocation(shaderProgramObjectPhongFragment, "u_Kd0");

	ls0Uniformf = GLES31.glGetUniformLocation(shaderProgramObjectPhongFragment, "u_Ls0");
	ks0Uniformf = GLES31.glGetUniformLocation(shaderProgramObjectPhongFragment, "u_Ks0");

	light0PositionUniformf = GLES31.glGetUniformLocation(shaderProgramObjectPhongFragment, "u_light0_position");

	material0ShininessUniformf = GLES31.glGetUniformLocation(shaderProgramObjectPhongFragment, "u_material0_shininess");



	modelMatrix1Uniform = GLES31.glGetUniformLocation(shaderProgramObjectPhongFragment, "u_model_matrix");
	viewMatrix1Uniform = GLES31.glGetUniformLocation(shaderProgramObjectPhongFragment, "u_view_matrix");
	projectionMatrix1Uniform = GLES31.glGetUniformLocation(shaderProgramObjectPhongFragment, "u_projection_matrix");


	la1Uniformf = GLES31.glGetUniformLocation(shaderProgramObjectPhongFragment, "u_La1");
	ka1Uniformf = GLES31.glGetUniformLocation(shaderProgramObjectPhongFragment, "u_Ka1");

	ld1Uniformf = GLES31.glGetUniformLocation(shaderProgramObjectPhongFragment, "u_Ld1");
	kd1Uniformf = GLES31.glGetUniformLocation(shaderProgramObjectPhongFragment, "u_Kd1");

	ls1Uniformf = GLES31.glGetUniformLocation(shaderProgramObjectPhongFragment, "u_Ls1");
	ks1Uniformf = GLES31.glGetUniformLocation(shaderProgramObjectPhongFragment, "u_Ks1");

	light1PositionUniformf = GLES31.glGetUniformLocation(shaderProgramObjectPhongFragment, "u_light1_position");
	material1ShininessUniformf = GLES31.glGetUniformLocation(shaderProgramObjectPhongFragment, "u_material1_shininess");


	la2Uniformf = GLES31.glGetUniformLocation(shaderProgramObjectPhongFragment, "u_La2");
	ka2Uniformf = GLES31.glGetUniformLocation(shaderProgramObjectPhongFragment, "u_Ka2");

	ld2Uniformf = GLES31.glGetUniformLocation(shaderProgramObjectPhongFragment, "u_Ld2");
	kd2Uniformf = GLES31.glGetUniformLocation(shaderProgramObjectPhongFragment, "u_Kd2");

	ls2Uniformf = GLES31.glGetUniformLocation(shaderProgramObjectPhongFragment, "u_Ls2");
	ks2Uniformf = GLES31.glGetUniformLocation(shaderProgramObjectPhongFragment, "u_Ks2");

	light2PositionUniformf = GLES31.glGetUniformLocation(shaderProgramObjectPhongFragment, "u_light2_position");

	material2ShininessUniformf = GLES31.glGetUniformLocation(shaderProgramObjectPhongFragment, "u_material2_shininess");


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
	//float[] xRotation = new float[] { 1.0f, (float)Math.sin(angle_red), (float)Math.cos(angle_red), 0.0f};

	//float[] yRotation = new float[] {(float)Math.cos(angle_green), 1.0f, (float)Math.sin(angle_green), 0.0f};
	

	//float[] zRotation = new float[] {(float)Math.cos(angle_blue), (float)Math.sin(angle_blue), 1.0f, 0.0f};

	float[] light0_position = { 0.0f, 2 * (float)(Math.sin(angle_red)), 2 *(float)(Math.cos(angle_red)), 1.0f };
	float[] light1_position = { 2 * (float)Math.cos(angle_green), 0.0f, 2 * (float)Math.sin(angle_green), 1.0f };
	float[] light2_position = { 2 * (float)Math.cos(angle_blue), 2 * (float)Math.sin(angle_blue), 0.0f, 1.0f };

	if(singleTap == 0)
	{
		GLES31.glUseProgram(shaderProgramObjectPhongVertex);
	
		//GLES31.glUniform1i(doubleTap0Uniform, 1);
		GLES31.glUniform3fv(la0Uniform, 1, light0_ambient, 0);
		GLES31.glUniform3fv(ld0Uniform, 1, light0_diffuse, 0);
		GLES31.glUniform3fv(ls0Uniform, 1, light0_specular, 0);
		
		GLES31.glUniform3fv(ka0Uniform, 1, material_ambient, 0);
		GLES31.glUniform3fv(kd0Uniform, 1, material_diffuse, 0);
		GLES31.glUniform3fv(ks0Uniform, 1, material_specular, 0);
		
		GLES31.glUniform1f(material0ShininessUniform, material_shininess);
		//float[] lightAlongX = new float[4];

		//lightAlongX[0] = light0_position[0] * xRotation[0];
		//lightAlongX[1] = light0_position[1] * xRotation[1];
		//lightAlongX[2] = light0_position[2] * xRotation[2];
		//lightAlongX[3] = light0_position[3] * xRotation[3];
		
		GLES31.glUniform4fv(light0PositionUniform, 1, light0_position, 0);

		GLES31.glUniform3fv(la1Uniform, 1, light1_ambient, 0);
		GLES31.glUniform3fv(ld1Uniform, 1, light1_diffuse, 0);
		GLES31.glUniform3fv(ls1Uniform, 1, light1_specular, 0);
		
		GLES31.glUniform3fv(ka1Uniform, 1, material_ambient, 0);
		GLES31.glUniform3fv(kd1Uniform, 1, material_diffuse, 0);
		GLES31.glUniform3fv(ks1Uniform, 1, material_specular, 0);
		
		GLES31.glUniform1f(material1ShininessUniform, material_shininess);

		//float[] lightAlongY = new float[4];

		//lightAlongY[0] = light1_position[0] * yRotation[0];
		//lightAlongY[1] = light1_position[1] * yRotation[1];
		//lightAlongY[2] = light1_position[2] * yRotation[2];
		//lightAlongY[3] = light1_position[3] * yRotation[3];

		GLES31.glUniform4fv(light1PositionUniform, 1, light1_position, 0);

		GLES31.glUniform3fv(la2Uniform, 1, light2_ambient, 0);
		GLES31.glUniform3fv(ld2Uniform, 1, light2_diffuse, 0);
		GLES31.glUniform3fv(ls2Uniform, 1, light2_specular, 0);
		
		GLES31.glUniform3fv(ka2Uniform, 1, material_ambient, 0);
		GLES31.glUniform3fv(kd2Uniform, 1, material_diffuse, 0);
		GLES31.glUniform3fv(ks2Uniform, 1, material_specular, 0);
		
		GLES31.glUniform1f(material2ShininessUniform, material_shininess);

		//float[] lightAlongZ = new float[4];

		//lightAlongZ[0] = light2_position[0] * zRotation[0];
		//lightAlongZ[1] = light2_position[1] * zRotation[1];
		//lightAlongZ[2] = light2_position[2] * zRotation[2];
		//lightAlongZ[3] = light2_position[3] * zRotation[3];
		GLES31.glUniform4fv(light1PositionUniform, 1, light2_position, 0);
		//GLES31.glUniform4fv(light2PositionUniform, 1, light2_position, 0);
	
	float modelMatrix[] = new float[16];
	float viewMatrix[] = new float[16];


	Matrix.setIdentityM(modelMatrix, 0);
	Matrix.setIdentityM(viewMatrix, 0);
	
	Matrix.translateM(modelMatrix, 0, 0.0f, 0.0f, -2.0f);
	//Matrix.rotateM(rotationMatrix, 0, angle_cube, 1.0f, 1.0f, 1.0f);
	
	GLES31.glUniformMatrix4fv(modelMatrix0Uniform, 1, false, modelMatrix, 0);
	GLES31.glUniformMatrix4fv(viewMatrix0Uniform, 1, false, viewMatrix, 0);
	GLES31.glUniformMatrix4fv(projectionMatrix0Uniform, 1, false, perspectiveProjectionMatrix, 0);
	GLES31.glBindVertexArray(vao_sphere[0]);
        
        // *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
    GLES31.glBindBuffer(GLES31.GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element[0]);
    GLES31.glDrawElements(GLES31.GL_TRIANGLES, numElements, GLES31.GL_UNSIGNED_SHORT, 0);
        
        // unbind vao
    GLES31.glBindVertexArray(0);
	GLES31.glUseProgram(0);
}


	if(singleTap == 1)
	{
		GLES31.glUseProgram(shaderProgramObjectPhongFragment);
	
		//GLES31.glUniform1i(doubleTap1Uniform, 1);
		GLES31.glUniform3fv(la0Uniformf, 1, light0_ambient, 0);
		GLES31.glUniform3fv(ld0Uniformf, 1, light0_diffuse, 0);
		GLES31.glUniform3fv(ls0Uniformf, 1, light0_specular, 0);
		
		GLES31.glUniform3fv(ka0Uniformf, 1, material_ambient, 0);
		GLES31.glUniform3fv(kd0Uniformf, 1, material_diffuse, 0);
		GLES31.glUniform3fv(ks0Uniformf, 1, material_specular, 0);
		
		GLES31.glUniform1f(material0ShininessUniformf, material_shininess);

		//float[] lightAlongX = new float[4];

		//lightAlongX[0] = light0_position[0] * xRotation[0];
		//lightAlongX[1] = light0_position[1] * xRotation[1];
		//lightAlongX[2] = light0_position[2] * xRotation[2];
		//lightAlongX[3] = light0_position[3] * xRotation[3];
		
		GLES31.glUniform4fv(light0PositionUniformf, 1, light0_position, 0);
		//GLES31.glUniform4fv(light0PositionUniformf, 1, light0_position, 0);

		//GLES31.glUniform1i(doubleTapUniform, 1);
		GLES31.glUniform3fv(la1Uniformf, 1, light1_ambient, 0);
		GLES31.glUniform3fv(ld1Uniformf, 1, light1_diffuse, 0);
		GLES31.glUniform3fv(ls1Uniformf, 1, light1_specular, 0);
		
		GLES31.glUniform3fv(ka1Uniformf, 1, material_ambient, 0);
		GLES31.glUniform3fv(kd1Uniformf, 1, material_diffuse, 0);
		GLES31.glUniform3fv(ks1Uniformf, 1, material_specular, 0);
		
		GLES31.glUniform1f(material1ShininessUniformf, material_shininess);
		//float[] lightAlongY = new float[4];

		//lightAlongY[0] = light1_position[0] * yRotation[0];
		//lightAlongY[1] = light1_position[1] * yRotation[1];
		//lightAlongY[2] = light1_position[2] * yRotation[2];
		//lightAlongY[3] = light1_position[3] * yRotation[3];

		GLES31.glUniform4fv(light1PositionUniformf, 1, light1_position, 0);

		//GLES31.glUniform4fv(light1PositionUniformf, 1, light1_position, 0);

		//GLES31.glUniform1i(doubleTapUniform, 1);
		GLES31.glUniform3fv(la2Uniformf, 1, light2_ambient, 0);
		GLES31.glUniform3fv(ld2Uniformf, 1, light2_diffuse, 0);
		GLES31.glUniform3fv(ls2Uniformf, 1, light2_specular, 0);
		
		GLES31.glUniform3fv(ka2Uniformf, 1, material_ambient, 0);
		GLES31.glUniform3fv(kd2Uniformf, 1, material_diffuse, 0);
		GLES31.glUniform3fv(ks2Uniformf, 1, material_specular, 0);
		
		GLES31.glUniform1f(material2ShininessUniformf, material_shininess);
		//float[] lightAlongZ = new float[4];

		//lightAlongZ[0] = light2_position[0] * zRotation[0];
		//lightAlongZ[1] = light2_position[1] * zRotation[1];
		//lightAlongZ[2] = light2_position[2] * zRotation[2];
		//lightAlongZ[3] = light2_position[3] * zRotation[3];
		GLES31.glUniform4fv(light1PositionUniformf, 1, light2_position, 0);
		//GLES31.glUniform4fv(light2PositionUniformf, 1, light2_position, 0);

	float modelMatrix[] = new float[16];
	float viewMatrix[] = new float[16];


	Matrix.setIdentityM(modelMatrix, 0);
	Matrix.setIdentityM(viewMatrix, 0);
	
	Matrix.translateM(modelMatrix, 0, 0.0f, 0.0f, -2.0f);
	//Matrix.rotateM(rotationMatrix, 0, angle_cube, 1.0f, 1.0f, 1.0f);
	
	GLES31.glUniformMatrix4fv(modelMatrix1Uniform, 1, false, modelMatrix, 0);
	GLES31.glUniformMatrix4fv(viewMatrix1Uniform, 1, false, viewMatrix, 0);
	GLES31.glUniformMatrix4fv(projectionMatrix1Uniform, 1, false, perspectiveProjectionMatrix, 0);
	GLES31.glBindVertexArray(vao_sphere[0]);
        
        // *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
    GLES31.glBindBuffer(GLES31.GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element[0]);
    GLES31.glDrawElements(GLES31.GL_TRIANGLES, numElements, GLES31.GL_UNSIGNED_SHORT, 0);
        
        // unbind vao
    GLES31.glBindVertexArray(0);
	GLES31.glUseProgram(0);
	}

	requestRender();
	update();
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


	if(shaderProgramObjectPhongVertex != 0)
	{
		if(vertexShaderObjectPhongVertex != 0)
		{
			GLES31.glDetachShader(shaderProgramObjectPhongVertex, vertexShaderObjectPhongVertex);
			GLES31.glDeleteShader(vertexShaderObjectPhongVertex);
			vertexShaderObjectPhongVertex = 0;
		}

		if(fragmentShaderObjectPhongVertex != 0)
		{
			GLES31.glDetachShader(shaderProgramObjectPhongVertex, fragmentShaderObjectPhongVertex);
			GLES31.glDeleteShader(fragmentShaderObjectPhongVertex);
			fragmentShaderObjectPhongVertex = 0;
		}
	}

	if(shaderProgramObjectPhongVertex != 0)
	{
		GLES31.glDeleteProgram(shaderProgramObjectPhongVertex);
		shaderProgramObjectPhongVertex = 0;
	}

	if(shaderProgramObjectPhongFragment != 0)
	{
		if(vertexShaderObjectPhongFragment != 0)
		{
			GLES31.glDetachShader(shaderProgramObjectPhongFragment, vertexShaderObjectPhongFragment);
			GLES31.glDeleteShader(vertexShaderObjectPhongFragment);
			vertexShaderObjectPhongFragment = 0;
		}

		if(fragmentShaderObjectPhongFragment != 0)
		{
			GLES31.glDetachShader(shaderProgramObjectPhongFragment, fragmentShaderObjectPhongFragment);
			GLES31.glDeleteShader(fragmentShaderObjectPhongFragment);
			fragmentShaderObjectPhongFragment = 0;
		}
	}

	if(shaderProgramObjectPhongFragment != 0)
	{
		GLES31.glDeleteProgram(shaderProgramObjectPhongFragment);
		shaderProgramObjectPhongFragment = 0;
	}

}

void update()
{
	angle_blue = angle_blue + 0.05f;
	if(angle_blue > 360.0f)
		angle_blue =angle_blue - 360.0f;

	angle_red = angle_red + 0.05f;
	if(angle_red > 360.0f)
		angle_red =angle_red - 360.0f;

	angle_green = angle_green + 0.05f;
	if(angle_green > 360.0f)
		angle_green =angle_green - 360.0f;
}
}

