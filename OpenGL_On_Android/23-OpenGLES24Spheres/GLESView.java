package com.example.sknaa.OGLESPhongLightingtwentyfourSpheres;

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
import java.lang.Math;
import android.opengl.Matrix;

public class GLESView extends GLSurfaceView implements GLSurfaceView.Renderer, OnGestureListener, OnDoubleTapListener
{
	private final Context context;

	private GestureDetector gestureDetector;

	private int gVertexShaderObject;
	private int gFragmentShaderObject;
	private int gShaderProgramObject;

	private int numVertices;
	private int numElements;

	private float angleX_axis = 0.0f;
	private float angleY_axis = 0.0f;
	private float angleZ_axis = 0.0f;

	private int[] vao_sphere = new int[1];
    private int[] vbo_sphere_position = new int[1];
    private int[] vbo_sphere_normal = new int[1];
    private int[] vbo_sphere_element = new int[1];

    private float light_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f};
    private float light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f};
    private float light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f};
    private float[] lightPosition = new float[4];

private float material1_ambient[] = { 0.0215f, 0.1745f, 0.0215f, 1.0f };
private float material1_diffuse[] = { 0.07568f, 0.061424f, 0.07568f, 1.0f };
private float material1_specular[] = { 0.633f, 0.727811f, 0.633f, 1.0f };
private float material1_shininess = 76.8f;

//Jade
private float material2_ambient[] = { 0.135f, 0.2225f ,0.1575f, 1.0f };
private float material2_diffuse[] = { 0.54f, 0.89f, 0.63f, 1.0f };
private float material2_specular[] = { 0.316288f, 0.316288f, 0.316288f, 1.0f };
private float material2_shininess = 12.8f;

//Obsidian
private float material3_ambient[] = { 0.05375f, 0.05f, 0.06625f, 1.0f };
private float material3_diffuse[] = { 0.18275f, 0.17f, 0.22525f, 1.0f };
private float material3_specular[] = { 0.332741f, 0.328634f, 0.346435f, 1.0f };
private float material3_shininess =  38.4f;

//Pearl
private float material4_ambient[] = { 0.25f, 0.20725f, 0.20725f, 1.0f };
private float material4_diffuse[] = { 1.0f, 0.829f, 0.829f, 1.0f };
private float material4_specular[] = { 0.296648f, 0.296648f, 0.296648f, 1.0f };
private float material4_shininess = 11.264f;

//Ruby
private float material5_ambient[] = { 0.1745f, 0.01175f, 0.01175f, 1.0f };
private float material5_diffuse[] = { 0.61424f, 0.04136f, 0.04136f, 1.0f };
private float material5_specular[] = { 0.727811f, 0.626959f, 0.626959f, 1.0f };
private float material5_shininess =  76.8f;

//Turquoise
private float material6_ambient[] = { 0.1f, 0.18725f, 0.1745f, 1.0f };
private float material6_diffuse[] = { 0.396f, 0.74151f, 0.69102f, 1.0f };
private float material6_specular[] = { 0.297254f, 0.30829f, 0.306678f, 1.0f };
private float material6_shininess =  12.8f;

//Brass
private float material7_ambient[] = { 0.329412f, 0.223529f, 0.027451f, 1.0f };
private float material7_diffuse[] = { 0.780392f, 0.568627f, 0.113725f, 1.0f };
private float material7_specular[] = { 0.992157f, 0.941176f, 0.807843f, 1.0f };
private float material7_shininess =  27.897443616f;

//Bronze
private float material8_ambient[] = { 0.2125f, 0.1275f, 0.054f, 1.0f };
private float material8_diffuse[] = { 0.714f, 0.4284f, 0.18144f, 1.0f };
private float material8_specular[] = { 0.393548f, 0.271906f, 0.166721f, 1.0f };
private float material8_shininess =  25.6f;

//Chrome
private float material9_ambient[] = { 0.25f, 0.25f, 0.25f, 1.0f };
private float material9_diffuse[] = { 0.4f, 0.4f, 0.4f, 1.0f };
private float material9_specular[] = { 0.774597f, 0.774597f, 0.774597f, 1.0f };
private float material9_shininess = 76.8f;

//Copper
private float material10_ambient[] = { 0.19125f, 0.0735f, 0.0225f, 1.0f };
private float material10_diffuse[] = { 0.7038f, 0.27048f, 0.0828f, 1.0f };
private float material10_specular[] = { 0.256777f, 0.137622f, 0.086014f, 1.0f };
private float material10_shininess = 12.8f;

//Gold
private float material11_ambient[] = { 0.24725f, 0.1995f, 0.0745f, 1.0f };
private float material11_diffuse[] = { 0.75164f, 0.60648f, 0.22648f, 1.0f };
private float material11_specular[] = { 0.628281f, 0.555802f, 0.366065f, 1.0f };
private float material11_shininess = 51.2f;

//Silver
private float material12_ambient[] = { 0.19225f, 0.19225f, 0.19225f, 1.0f };
private float material12_diffuse[] = { 0.50754f, 0.50754f, 0.50754f, 1.0f };
private float material12_specular[] = { 0.508273f, 0.508273f, 0.508273f, 1.0f };
private float material12_shininess = 51.2f;

//Black
private float material13_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
private float material13_diffuse[] = { 0.01f, 0.01f, 0.01f, 1.0f };
private float material13_specular[] = { 0.50f, 0.50f, 0.50f, 1.0f };
private float material13_shininess = 32.0f;

//Cyan
private float material14_ambient[] = { 0.0f, 0.1f, 0.06f, 1.0f };
private float material14_diffuse[] = { 0.0f, 0.50980392f, 0.50980392f, 1.0f };
private float material14_specular[] = { 0.50196078f, 0.50196078f, 0.50196078f, 1.0f };
private float material14_shininess =  32.0f;

//Green
private float material15_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
private float material15_diffuse[] = { 0.1f, 0.35f, 0.1f, 1.0f };
private float material15_specular[] = { 0.45f, 0.55f, 0.45f, 1.0f };
private float material15_shininess = 32.0f;

//Red
private float material16_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
private float material16_diffuse[] = { 0.5f, 0.0f, 0.0f, 1.0f };
private float material16_specular[] = { 0.7f, 0.6f, 0.6f, 1.0f };
private float material16_shininess = 32.0f;

//White
private float material17_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
private float material17_diffuse[] = { 0.55f, 0.55f, 0.55f, 1.0f };
private float material17_specular[] = { 0.70f, 0.70f, 0.70f, 1.0f };
private float material17_shininess = 32.0f;

//Yellow Plastic
private float material18_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
private float material18_diffuse[] = { 0.5f, 0.5f, 0.0f, 1.0f };
private float material18_specular[] = { 0.60f, 0.60f, 0.50f, 1.0f };
private float material18_shininess = 32.0f;

//Black
private float material19_ambient[] = { 0.02f, 0.02f, 0.02f, 1.0f };
private float material19_diffuse[] = { 0.01f, 0.01f, 0.01f, 1.0f };
private float material19_specular[] = { 0.4f, 0.4f, 0.4f, 1.0f };
private float material19_shininess = 10.0f;

//Cyan
private float material20_ambient[] = { 0.0f, 0.05f, 0.05f, 1.0f };
private float material20_diffuse[] = { 0.4f, 0.5f, 0.5f, 1.0f };
private float material20_specular[] = { 0.04f, 0.7f, 0.7f, 1.0f };
private float material20_shininess = 10.0f;

//Green
private float material21_ambient[] = { 0.0f, 0.05f, 0.0f, 1.0f };
private float material21_diffuse[] = { 0.4f, 0.5f, 0.4f, 1.0f };
private float material21_specular[] = { 0.04f, 0.7f, 0.04f, 1.0f };
private float material21_shininess = 10.0f;

//Red
private float material22_ambient[] = { 0.05f, 0.0f, 0.0f, 1.0f };
private float material22_diffuse[] = { 0.5f, 0.4f, 0.4f, 1.0f };
private float material22_specular[] = { 0.7f, 0.04f, 0.04f, 1.0f };
private float material22_shininess = 10.0f;

//White
private float material23_ambient[] = { 0.05f, 0.05f, 0.05f, 1.0f };
private float material23_diffuse[] = { 0.5f, 0.5f, 0.5f, 1.0f };
private float material23_specular[] = { 0.7f, 0.7f, 0.7f, 1.0f };
private float material23_shininess = 10.0f;

//Yellow Rubber
private float material24_ambient[] = { 0.05f, 0.05f, 0.0f, 1.0f };
private float material24_diffuse[] = { 0.5f, 0.5f, 0.4f, 1.0f };
private float material24_specular[] = { 0.7f, 0.7f, 0.04f, 1.0f };
private float material24_shininess = 10.0f;


private int gModelMatrixUniform, gViewMatrixUniform, gProjectionMatrixUniform;

private int gLdUniform;
private int gLaUniform;
private int gLsUniform;

private int gKa1Uniform;
private int gKd1Uniform;
private int gKs1Uniform;
private int gmaterial1_shininess_uniform;

private int gKa2Uniform;
private int gKd2Uniform;
private int gKs2Uniform;
private int gmaterial2_shininess_uniform;

private int gKa3Uniform;
private int gKd3Uniform;
private int gKs3Uniform;
private int gmaterial3_shininess_uniform;

private int gKa4Uniform;
private int gKd4Uniform;
private int gKs4Uniform;
private int gmaterial4_shininess_uniform;

private int gKa5Uniform;
private int gKd5Uniform;
private int gKs5Uniform;
private int gmaterial5_shininess_uniform;

private int gKa6Uniform;
private int gKd6Uniform;
private int gKs6Uniform;
private int gmaterial6_shininess_uniform;

private int gKa7Uniform;
private int gKd7Uniform;
private int gKs7Uniform;
private int gmaterial7_shininess_uniform;

private int gKa8Uniform;
private int gKd8Uniform;
private int gKs8Uniform;
private int gmaterial8_shininess_uniform;

private int gKa9Uniform;
private int gKd9Uniform;
private int gKs9Uniform;
private int gmaterial9_shininess_uniform;

private int gKa10Uniform;
private int gKd10Uniform;
private int gKs10Uniform;
private int gmaterial10_shininess_uniform;

private int gKa11Uniform;
private int gKd11Uniform;
private int gKs11Uniform;
private int gmaterial11_shininess_uniform;

private int gKa12Uniform;
private int gKd12Uniform;
private int gKs12Uniform;
private int gmaterial12_shininess_uniform;

private int gKa13Uniform;
private int gKd13Uniform;
private int gKs13Uniform;
private int gmaterial13_shininess_uniform;

private int gKa14Uniform;
private int gKd14Uniform;
private int gKs14Uniform;
private int gmaterial14_shininess_uniform;

private int gKa15Uniform;
private int gKd15Uniform;
private int gKs15Uniform;
private int gmaterial15_shininess_uniform;

private int gKa16Uniform;
private int gKd16Uniform;
private int gKs16Uniform;
private int gmaterial16_shininess_uniform;

private int gKa17Uniform;
private int gKd17Uniform;
private int gKs17Uniform;
private int gmaterial17_shininess_uniform;

private int gKa18Uniform;
private int gKd18Uniform;
private int gKs18Uniform;
private int gmaterial18_shininess_uniform;

private int gKa19Uniform;
private int gKd19Uniform;
private int gKs19Uniform;
private int gmaterial19_shininess_uniform;

private int gKa20Uniform;
private int gKd20Uniform;
private int gKs20Uniform;
private int gmaterial20_shininess_uniform;

private int gKa21Uniform;
private int gKd21Uniform;
private int gKs21Uniform;
private int gmaterial21_shininess_uniform;

private int gKa22Uniform;
private int gKd22Uniform;
private int gKs22Uniform;
private int gmaterial22_shininess_uniform;

private int gKa23Uniform;
private int gKd23Uniform;
private int gKs23Uniform;
private int gmaterial23_shininess_uniform;

private int gKa24Uniform;
private int gKd24Uniform;
private int gKs24Uniform;
private int gmaterial24_shininess_uniform;

private int glight_position_uniform;
private int gSphereNo;
	
	private int doubleTapUniform;

	private float perspectiveProjectionMatrix[] = new float[16];

	private int doubleTap;
	private int singleTap = 1;

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
	System.out.println("OGLES: Before initialize call");
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
	if(singleTap > 3)
		singleTap = 1;
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
	System.out.println("OGLES: In Initialize : Begining");
	System.out.println("OGLES: In Initialize : Before Vertex Shader Compilation");

	gVertexShaderObject = GLES31.glCreateShader(GLES31.GL_VERTEX_SHADER);
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

	GLES31.glShaderSource(gVertexShaderObject, vertexShaderSourceCode);
	GLES31.glCompileShader(gVertexShaderObject);
	int[] iShaderCompiledStatus = new int[1];
	int[] iInfoLogLength = new int[1];
	String szInfoLog = null;
	GLES31.glGetShaderiv(gVertexShaderObject, GLES31.GL_COMPILE_STATUS, iShaderCompiledStatus, 0);
	if(iShaderCompiledStatus[0] == GLES31.GL_FALSE)
	{
		GLES31.glGetShaderiv(gVertexShaderObject, GLES31.GL_INFO_LOG_LENGTH, iInfoLogLength, 0);

		if(iInfoLogLength[0] > 0)
		{
			szInfoLog = GLES31.glGetShaderInfoLog(gVertexShaderObject);
			System.out.println("OGLES: Vertex Shader Compilation Log = "+szInfoLog);
			uninitialize();
			System.exit(0);
		}
	}	

	System.out.println("OGLES: In Initialize : After Vertex Shader Compilation");
	System.out.println("OGLES: In Initialize : Before Fragment Shader Compilation");

	gFragmentShaderObject = GLES31.glCreateShader(GLES31.GL_FRAGMENT_SHADER);
	final String fragmentShaderSourceCode = String.format
	(
		"#version 310 es"+
		"\n"+
		"precision highp float;"+
		"in vec3 transformed_normals;"+
		"in vec3 light_direction;"+
		"in vec3 viewer_vector;"+
		"out vec4 FragColor;"+
		"uniform vec3 u_La;" +
		"uniform vec3 u_Ka1;" +
		"uniform vec3 u_Ka2;" +
		"uniform vec3 u_Ka3;" +
		"uniform vec3 u_Ka4;" +
		"uniform vec3 u_Ka5;" +
		"uniform vec3 u_Ka6;" +
		"uniform vec3 u_Ka7;" +
		"uniform vec3 u_Ka8;" +
		"uniform vec3 u_Ka9;" +
		"uniform vec3 u_Ka10;" +
		"uniform vec3 u_Ka11;" +
		"uniform vec3 u_Ka12;" +
		"uniform vec3 u_Ka13;" +
		"uniform vec3 u_Ka14;" +
		"uniform vec3 u_Ka15;" +
		"uniform vec3 u_Ka16;" +
		"uniform vec3 u_Ka17;" +
		"uniform vec3 u_Ka18;" +
		"uniform vec3 u_Ka19;" +
		"uniform vec3 u_Ka20;" +
		"uniform vec3 u_Ka21;" +
		"uniform vec3 u_Ka22;" +
		"uniform vec3 u_Ka23;" +
		"uniform vec3 u_Ka24;" +
		"uniform vec3 u_Ld;" +
		"uniform vec3 u_Kd1;" +
		"uniform vec3 u_Kd2;" +
		"uniform vec3 u_Kd3;" +
		"uniform vec3 u_Kd4;" +
		"uniform vec3 u_Kd5;" +
		"uniform vec3 u_Kd6;" +
		"uniform vec3 u_Kd7;" +
		"uniform vec3 u_Kd8;" +
		"uniform vec3 u_Kd9;" +
		"uniform vec3 u_Kd10;" +
		"uniform vec3 u_Kd11;" +
		"uniform vec3 u_Kd12;" +
		"uniform vec3 u_Kd13;" +
		"uniform vec3 u_Kd14;" +
		"uniform vec3 u_Kd15;" +
		"uniform vec3 u_Kd16;" +
		"uniform vec3 u_Kd17;" +
		"uniform vec3 u_Kd18;" +
		"uniform vec3 u_Kd19;" +
		"uniform vec3 u_Kd20;" +
		"uniform vec3 u_Kd21;" +
		"uniform vec3 u_Kd22;" +
		"uniform vec3 u_Kd23;" +
		"uniform vec3 u_Kd24;" +
		"uniform vec3 u_Ls;" +
		"uniform vec3 u_Ks1;" +
		"uniform vec3 u_Ks2;" +
		"uniform vec3 u_Ks3;" +
		"uniform vec3 u_Ks4;" +
		"uniform vec3 u_Ks5;" +
		"uniform vec3 u_Ks6;" +
		"uniform vec3 u_Ks7;" +
		"uniform vec3 u_Ks8;" +
		"uniform vec3 u_Ks9;" +
		"uniform vec3 u_Ks10;" +
		"uniform vec3 u_Ks11;" +
		"uniform vec3 u_Ks12;" +
		"uniform vec3 u_Ks13;" +
		"uniform vec3 u_Ks14;" +
		"uniform vec3 u_Ks15;" +
		"uniform vec3 u_Ks16;" +
		"uniform vec3 u_Ks17;" +
		"uniform vec3 u_Ks18;" +
		"uniform vec3 u_Ks19;" +
		"uniform vec3 u_Ks20;" +
		"uniform vec3 u_Ks21;" +
		"uniform vec3 u_Ks22;" +
		"uniform vec3 u_Ks23;" +
		"uniform vec3 u_Ks24;" +
		"uniform float u_material1_shininess;" +
		"uniform float u_material2_shininess;" +
		"uniform float u_material3_shininess;" +
		"uniform float u_material4_shininess;" +
		"uniform float u_material5_shininess;" +
		"uniform float u_material6_shininess;" +
		"uniform float u_material7_shininess;" +
		"uniform float u_material8_shininess;" +
		"uniform float u_material9_shininess;" +
		"uniform float u_material10_shininess;" +
		"uniform float u_material11_shininess;" +
		"uniform float u_material12_shininess;" +
		"uniform float u_material13_shininess;" +
		"uniform float u_material14_shininess;" +
		"uniform float u_material15_shininess;" +
		"uniform float u_material16_shininess;" +
		"uniform float u_material17_shininess;" +
		"uniform float u_material18_shininess;" +
		"uniform float u_material19_shininess;" +
		"uniform float u_material20_shininess;" +
		"uniform float u_material21_shininess;" +
		"uniform float u_material22_shininess;" +
		"uniform float u_material23_shininess;" +
		"uniform float u_material24_shininess;" +
		"uniform int sphere;"+
		"uniform int u_double_tap;"+
		"void main(void)"+
		"{"+
		"vec3 phong_ads_color;"+
		"if(u_double_tap == 1)"+
		"{"+
		"if(sphere == 1)"+
		"{"+
		"vec3 normalized_transformed_normals = normalize(transformed_normals);" +
		"vec3 normalized_light_direction = normalize(light_direction);" +
		"vec3 normalized_viewer_vector = normalize(viewer_vector);" +
		"vec3 ambient = u_La * u_Ka1;" +
		"float tn_dot_ld = max(dot(normalized_transformed_normals, normalized_light_direction), 0.0);" +
		"vec3 diffuse = u_Ld * u_Kd1 * tn_dot_ld;" +
		"vec3 reflection_vector = reflect(-normalized_light_direction, normalized_transformed_normals);" +
		"vec3 specular = u_Ls * u_Ks1 * pow(max(dot(reflection_vector, normalized_viewer_vector), 0.0), u_material1_shininess);" +
		"phong_ads_color = ambient + diffuse + specular;" +
		"}"+

		"else if(sphere == 2 )" +
		"{"+
		"vec3 normalized_transformed_normals = normalize(transformed_normals);" +
		"vec3 normalized_light_direction = normalize(light_direction);" +
		"vec3 normalized_viewer_vector = normalize(viewer_vector);" +
		"vec3 ambient = u_La * u_Ka2;" +
		"float tn_dot_ld = max(dot(normalized_transformed_normals, normalized_light_direction), 0.0);" +
		"vec3 diffuse = u_Ld * u_Kd2 * tn_dot_ld;" +
		"vec3 reflection_vector = reflect(-normalized_light_direction, normalized_transformed_normals);" +
		"vec3 specular = u_Ls * u_Ks2 * pow(max(dot(reflection_vector, normalized_viewer_vector), 0.0), u_material2_shininess);" +
		"phong_ads_color = ambient + diffuse + specular;" +
		"}"+

		"else if(sphere == 3 )" +
		"{"+
		"vec3 normalized_transformed_normals = normalize(transformed_normals);" +
		"vec3 normalized_light_direction = normalize(light_direction);" +
		"vec3 normalized_viewer_vector = normalize(viewer_vector);" +
		"vec3 ambient = u_La * u_Ka3;" +
		"float tn_dot_ld = max(dot(normalized_transformed_normals, normalized_light_direction), 0.0);" +
		"vec3 diffuse = u_Ld * u_Kd3 * tn_dot_ld;" +
		"vec3 reflection_vector = reflect(-normalized_light_direction, normalized_transformed_normals);" +
		"vec3 specular = u_Ls * u_Ks3 * pow(max(dot(reflection_vector, normalized_viewer_vector), 0.0), u_material3_shininess);" +
		"phong_ads_color = ambient + diffuse + specular;" +
		"}"+

		"else if(sphere == 4)" +
		"{"+
		"vec3 normalized_transformed_normals = normalize(transformed_normals);" +
		"vec3 normalized_light_direction = normalize(light_direction);" +
		"vec3 normalized_viewer_vector = normalize(viewer_vector);" +
		"vec3 ambient = u_La * u_Ka4;" +
		"float tn_dot_ld = max(dot(normalized_transformed_normals, normalized_light_direction), 0.0);" +
		"vec3 diffuse = u_Ld * u_Kd4 * tn_dot_ld;" +
		"vec3 reflection_vector = reflect(-normalized_light_direction, normalized_transformed_normals);" +
		"vec3 specular = u_Ls * u_Ks4 * pow(max(dot(reflection_vector, normalized_viewer_vector), 0.0), u_material4_shininess);" +
		"phong_ads_color = ambient + diffuse + specular;" +
		"}"+

		"else if(sphere == 5)" +
		"{"+
		"vec3 normalized_transformed_normals = normalize(transformed_normals);" +
		"vec3 normalized_light_direction = normalize(light_direction);" +
		"vec3 normalized_viewer_vector = normalize(viewer_vector);" +
		"vec3 ambient = u_La * u_Ka5;" +
		"float tn_dot_ld = max(dot(normalized_transformed_normals, normalized_light_direction), 0.0);" +
		"vec3 diffuse = u_Ld * u_Kd5 * tn_dot_ld;" +
		"vec3 reflection_vector = reflect(-normalized_light_direction, normalized_transformed_normals);" +
		"vec3 specular = u_Ls * u_Ks5 * pow(max(dot(reflection_vector, normalized_viewer_vector), 0.0), u_material5_shininess);" +
		"phong_ads_color = ambient + diffuse + specular;" +
		"}"+

			"else if(sphere == 6)" +
			"{"+
			"vec3 normalized_transformed_normals = normalize(transformed_normals);" +
			"vec3 normalized_light_direction = normalize(light_direction);" +
			"vec3 normalized_viewer_vector = normalize(viewer_vector);" +
			"vec3 ambient = u_La * u_Ka6;" +
			"float tn_dot_ld = max(dot(normalized_transformed_normals, normalized_light_direction), 0.0);" +
			"vec3 diffuse = u_Ld * u_Kd6 * tn_dot_ld;" +
			"vec3 reflection_vector = reflect(-normalized_light_direction, normalized_transformed_normals);" +
			"vec3 specular = u_Ls * u_Ks6 * pow(max(dot(reflection_vector, normalized_viewer_vector), 0.0), u_material6_shininess);" +
			"phong_ads_color = ambient + diffuse + specular;" +
			"}"+

			"else if(sphere == 7)" +
			"{"+
			"vec3 normalized_transformed_normals = normalize(transformed_normals);" +
			"vec3 normalized_light_direction = normalize(light_direction);" +
			"vec3 normalized_viewer_vector = normalize(viewer_vector);" +
			"vec3 ambient = u_La * u_Ka7;" +
			"float tn_dot_ld = max(dot(normalized_transformed_normals, normalized_light_direction), 0.0);" +
			"vec3 diffuse = u_Ld * u_Kd7 * tn_dot_ld;" +
			"vec3 reflection_vector = reflect(-normalized_light_direction, normalized_transformed_normals);" +
			"vec3 specular = u_Ls * u_Ks7 * pow(max(dot(reflection_vector, normalized_viewer_vector), 0.0), u_material7_shininess);" +
			"phong_ads_color = ambient + diffuse + specular;" +
			"}"+

			"else if(sphere == 8)" +
			"{"+
			"vec3 normalized_transformed_normals = normalize(transformed_normals);" +
			"vec3 normalized_light_direction = normalize(light_direction);" +
			"vec3 normalized_viewer_vector = normalize(viewer_vector);" +
			"vec3 ambient = u_La * u_Ka8;" +
			"float tn_dot_ld = max(dot(normalized_transformed_normals, normalized_light_direction), 0.0);" +
			"vec3 diffuse = u_Ld * u_Kd8 * tn_dot_ld;" +
			"vec3 reflection_vector = reflect(-normalized_light_direction, normalized_transformed_normals);" +
			"vec3 specular = u_Ls * u_Ks8 * pow(max(dot(reflection_vector, normalized_viewer_vector), 0.0), u_material8_shininess);" +
			"phong_ads_color = ambient + diffuse + specular;" +
			"}"+

			"else if(sphere == 9)" +
			"{"+
			"vec3 normalized_transformed_normals = normalize(transformed_normals);" +
			"vec3 normalized_light_direction = normalize(light_direction);" +
			"vec3 normalized_viewer_vector = normalize(viewer_vector);" +
			"vec3 ambient = u_La * u_Ka9;" +
			"float tn_dot_ld = max(dot(normalized_transformed_normals, normalized_light_direction), 0.0);" +
			"vec3 diffuse = u_Ld * u_Kd9 * tn_dot_ld;" +
			"vec3 reflection_vector = reflect(-normalized_light_direction, normalized_transformed_normals);" +
			"vec3 specular = u_Ls * u_Ks9 * pow(max(dot(reflection_vector, normalized_viewer_vector), 0.0), u_material9_shininess);" +
			"phong_ads_color = ambient + diffuse + specular;" +
			"}"+

			"else if(sphere == 10)" +
			"{"+
			"vec3 normalized_transformed_normals = normalize(transformed_normals);" +
			"vec3 normalized_light_direction = normalize(light_direction);" +
			"vec3 normalized_viewer_vector = normalize(viewer_vector);" +
			"vec3 ambient = u_La * u_Ka10;" +
			"float tn_dot_ld = max(dot(normalized_transformed_normals, normalized_light_direction), 0.0);" +
			"vec3 diffuse = u_Ld * u_Kd10 * tn_dot_ld;" +
			"vec3 reflection_vector = reflect(-normalized_light_direction, normalized_transformed_normals);" +
			"vec3 specular = u_Ls * u_Ks10 * pow(max(dot(reflection_vector, normalized_viewer_vector), 0.0), u_material10_shininess);" +
			"phong_ads_color = ambient + diffuse + specular;" +
			"}"+

			"else if(sphere == 11)" +
			"{"+
			"vec3 normalized_transformed_normals = normalize(transformed_normals);" +
			"vec3 normalized_light_direction = normalize(light_direction);" +
			"vec3 normalized_viewer_vector = normalize(viewer_vector);" +
			"vec3 ambient = u_La * u_Ka11;" +
			"float tn_dot_ld = max(dot(normalized_transformed_normals, normalized_light_direction), 0.0);" +
			"vec3 diffuse = u_Ld * u_Kd11 * tn_dot_ld;" +
			"vec3 reflection_vector = reflect(-normalized_light_direction, normalized_transformed_normals);" +
			"vec3 specular = u_Ls * u_Ks11 * pow(max(dot(reflection_vector, normalized_viewer_vector), 0.0), u_material11_shininess);" +
			"phong_ads_color = ambient + diffuse + specular;" +
			"}"+

			"else if(sphere == 12)" +
			"{"+
			"vec3 normalized_transformed_normals = normalize(transformed_normals);" +
			"vec3 normalized_light_direction = normalize(light_direction);" +
			"vec3 normalized_viewer_vector = normalize(viewer_vector);" +
			"vec3 ambient = u_La * u_Ka12;" +
			"float tn_dot_ld = max(dot(normalized_transformed_normals, normalized_light_direction), 0.0);" +
			"vec3 diffuse = u_Ld * u_Kd12 * tn_dot_ld;" +
			"vec3 reflection_vector = reflect(-normalized_light_direction, normalized_transformed_normals);" +
			"vec3 specular = u_Ls * u_Ks12 * pow(max(dot(reflection_vector, normalized_viewer_vector), 0.0), u_material12_shininess);" +
			"phong_ads_color = ambient + diffuse + specular;" +
			"}"+

			"else if(sphere == 13)" +
			"{"+
			"vec3 normalized_transformed_normals = normalize(transformed_normals);" +
			"vec3 normalized_light_direction = normalize(light_direction);" +
			"vec3 normalized_viewer_vector = normalize(viewer_vector);" +
			"vec3 ambient = u_La * u_Ka13;" +
			"float tn_dot_ld = max(dot(normalized_transformed_normals, normalized_light_direction), 0.0);" +
			"vec3 diffuse = u_Ld * u_Kd13 * tn_dot_ld;" +
			"vec3 reflection_vector = reflect(-normalized_light_direction, normalized_transformed_normals);" +
			"vec3 specular = u_Ls * u_Ks13 * pow(max(dot(reflection_vector, normalized_viewer_vector), 0.0), u_material13_shininess);" +
			"phong_ads_color = ambient + diffuse + specular;" +
			"}"+

				"else if(sphere == 14)" +
				"{"+
				"vec3 normalized_transformed_normals = normalize(transformed_normals);" +
				"vec3 normalized_light_direction = normalize(light_direction);" +
				"vec3 normalized_viewer_vector = normalize(viewer_vector);" +
				"vec3 ambient = u_La * u_Ka14;" +
				"float tn_dot_ld = max(dot(normalized_transformed_normals, normalized_light_direction), 0.0);" +
				"vec3 diffuse = u_Ld * u_Kd14 * tn_dot_ld;" +
				"vec3 reflection_vector = reflect(-normalized_light_direction, normalized_transformed_normals);" +
				"vec3 specular = u_Ls * u_Ks14 * pow(max(dot(reflection_vector, normalized_viewer_vector), 0.0), u_material14_shininess);" +
				"phong_ads_color = ambient + diffuse + specular;" +
				"}"+

				"else if(sphere == 15)" +
				"{"+
				"vec3 normalized_transformed_normals = normalize(transformed_normals);" +
				"vec3 normalized_light_direction = normalize(light_direction);" +
				"vec3 normalized_viewer_vector = normalize(viewer_vector);" +
				"vec3 ambient = u_La * u_Ka15;" +
				"float tn_dot_ld = max(dot(normalized_transformed_normals, normalized_light_direction), 0.0);" +
				"vec3 diffuse = u_Ld * u_Kd15 * tn_dot_ld;" +
				"vec3 reflection_vector = reflect(-normalized_light_direction, normalized_transformed_normals);" +
				"vec3 specular = u_Ls * u_Ks15 * pow(max(dot(reflection_vector, normalized_viewer_vector), 0.0), u_material15_shininess);" +
				"phong_ads_color = ambient + diffuse + specular;" +
				"}"+

				"else if(sphere == 16)" +
				"{"+
				"vec3 normalized_transformed_normals = normalize(transformed_normals);" +
				"vec3 normalized_light_direction = normalize(light_direction);" +
				"vec3 normalized_viewer_vector = normalize(viewer_vector);" +
				"vec3 ambient = u_La * u_Ka16;" +
				"float tn_dot_ld = max(dot(normalized_transformed_normals, normalized_light_direction), 0.0);" +
				"vec3 diffuse = u_Ld * u_Kd16 * tn_dot_ld;" +
				"vec3 reflection_vector = reflect(-normalized_light_direction, normalized_transformed_normals);" +
				"vec3 specular = u_Ls * u_Ks16 * pow(max(dot(reflection_vector, normalized_viewer_vector), 0.0), u_material16_shininess);" +
				"phong_ads_color = ambient + diffuse + specular;" +
				"}"+

				"else if(sphere == 17)" +
				"{"+
				"vec3 normalized_transformed_normals = normalize(transformed_normals);" +
				"vec3 normalized_light_direction = normalize(light_direction);" +
				"vec3 normalized_viewer_vector = normalize(viewer_vector);" +
				"vec3 ambient = u_La * u_Ka17;" +
				"float tn_dot_ld = max(dot(normalized_transformed_normals, normalized_light_direction), 0.0);" +
				"vec3 diffuse = u_Ld * u_Kd17 * tn_dot_ld;" +
				"vec3 reflection_vector = reflect(-normalized_light_direction, normalized_transformed_normals);" +
				"vec3 specular = u_Ls * u_Ks17 * pow(max(dot(reflection_vector, normalized_viewer_vector), 0.0), u_material17_shininess);" +
				"phong_ads_color = ambient + diffuse + specular;" +
				"}"+

				"else if(sphere == 18)" +
				"{"+
				"vec3 normalized_transformed_normals = normalize(transformed_normals);" +
				"vec3 normalized_light_direction = normalize(light_direction);" +
				"vec3 normalized_viewer_vector = normalize(viewer_vector);" +
				"vec3 ambient = u_La * u_Ka18;" +
				"float tn_dot_ld = max(dot(normalized_transformed_normals, normalized_light_direction), 0.0);" +
				"vec3 diffuse = u_Ld * u_Kd18 * tn_dot_ld;" +
				"vec3 reflection_vector = reflect(-normalized_light_direction, normalized_transformed_normals);" +
				"vec3 specular = u_Ls * u_Ks18 * pow(max(dot(reflection_vector, normalized_viewer_vector), 0.0), u_material18_shininess);" +
				"phong_ads_color = ambient + diffuse + specular;" +
				"}"+

				"else if(sphere == 19)" +
				"{"+
				"vec3 normalized_transformed_normals = normalize(transformed_normals);" +
				"vec3 normalized_light_direction = normalize(light_direction);" +
				"vec3 normalized_viewer_vector = normalize(viewer_vector);" +
				"vec3 ambient = u_La * u_Ka19;" +
				"float tn_dot_ld = max(dot(normalized_transformed_normals, normalized_light_direction), 0.0);" +
				"vec3 diffuse = u_Ld * u_Kd19 * tn_dot_ld;" +
				"vec3 reflection_vector = reflect(-normalized_light_direction, normalized_transformed_normals);" +
				"vec3 specular = u_Ls * u_Ks19 * pow(max(dot(reflection_vector, normalized_viewer_vector), 0.0), u_material19_shininess);" +
				"phong_ads_color = ambient + diffuse + specular;" +
				"}"+

				"else if(sphere == 20)" +
				"{"+
				"vec3 normalized_transformed_normals = normalize(transformed_normals);" +
				"vec3 normalized_light_direction = normalize(light_direction);" +
				"vec3 normalized_viewer_vector = normalize(viewer_vector);" +
				"vec3 ambient = u_La * u_Ka20;" +
				"float tn_dot_ld = max(dot(normalized_transformed_normals, normalized_light_direction), 0.0);" +
				"vec3 diffuse = u_Ld * u_Kd20 * tn_dot_ld;" +
				"vec3 reflection_vector = reflect(-normalized_light_direction, normalized_transformed_normals);" +
				"vec3 specular = u_Ls * u_Ks20 * pow(max(dot(reflection_vector, normalized_viewer_vector), 0.0), u_material20_shininess);" +
				"phong_ads_color = ambient + diffuse + specular;" +
				"}"+

				"else if(sphere == 21)" +
				"{"+
				"vec3 normalized_transformed_normals = normalize(transformed_normals);" +
				"vec3 normalized_light_direction = normalize(light_direction);" +
				"vec3 normalized_viewer_vector = normalize(viewer_vector);" +
				"vec3 ambient = u_La * u_Ka21;" +
				"float tn_dot_ld = max(dot(normalized_transformed_normals, normalized_light_direction), 0.0);" +
				"vec3 diffuse = u_Ld * u_Kd21 * tn_dot_ld;" +
				"vec3 reflection_vector = reflect(-normalized_light_direction, normalized_transformed_normals);" +
				"vec3 specular = u_Ls * u_Ks21 * pow(max(dot(reflection_vector, normalized_viewer_vector), 0.0), u_material21_shininess);" +
				"phong_ads_color = ambient + diffuse + specular;" +
				"}"+

					"else if(sphere == 22)" +
					"{"+
					"vec3 normalized_transformed_normals = normalize(transformed_normals);" +
					"vec3 normalized_light_direction = normalize(light_direction);" +
					"vec3 normalized_viewer_vector = normalize(viewer_vector);" +
					"vec3 ambient = u_La * u_Ka22;" +
					"float tn_dot_ld = max(dot(normalized_transformed_normals, normalized_light_direction), 0.0);" +
					"vec3 diffuse = u_Ld * u_Kd22 * tn_dot_ld;" +
					"vec3 reflection_vector = reflect(-normalized_light_direction, normalized_transformed_normals);" +
					"vec3 specular = u_Ls * u_Ks22 * pow(max(dot(reflection_vector, normalized_viewer_vector), 0.0), u_material22_shininess);" +
					"phong_ads_color = ambient + diffuse + specular;" +
					"}"+

					"else if(sphere == 23)" +
					"{"+
					"vec3 normalized_transformed_normals = normalize(transformed_normals);" +
					"vec3 normalized_light_direction = normalize(light_direction);" +
					"vec3 normalized_viewer_vector = normalize(viewer_vector);" +
					"vec3 ambient = u_La * u_Ka23;" +
					"float tn_dot_ld = max(dot(normalized_transformed_normals, normalized_light_direction), 0.0);" +
					"vec3 diffuse = u_Ld * u_Kd23 * tn_dot_ld;" +
					"vec3 reflection_vector = reflect(-normalized_light_direction, normalized_transformed_normals);" +
					"vec3 specular = u_Ls * u_Ks23 * pow(max(dot(reflection_vector, normalized_viewer_vector), 0.0), u_material23_shininess);" +
					"phong_ads_color = ambient + diffuse + specular;" +
					"}"+

					"else if(sphere == 24)" +
					"{"+
					"vec3 normalized_transformed_normals = normalize(transformed_normals);" +
					"vec3 normalized_light_direction = normalize(light_direction);" +
					"vec3 normalized_viewer_vector = normalize(viewer_vector);" +
					"vec3 ambient = u_La * u_Ka24;" +
					"float tn_dot_ld = max(dot(normalized_transformed_normals, normalized_light_direction), 0.0);" +
					"vec3 diffuse = u_Ld * u_Kd24 * tn_dot_ld;" +
					"vec3 reflection_vector = reflect(-normalized_light_direction, normalized_transformed_normals);" +
					"vec3 specular = u_Ls * u_Ks24 * pow(max(dot(reflection_vector, normalized_viewer_vector), 0.0), u_material24_shininess);" +
					"phong_ads_color = ambient + diffuse + specular;" +
					"}"+
		"}"+
		"else"+
		"{"+
		"phong_ads_color = vec3(1.0 ,1.0, 1.0);"+
		"}"+
		"FragColor = vec4(phong_ads_color, 1.0);"+
		"}"
	);

	GLES31.glShaderSource(gFragmentShaderObject,fragmentShaderSourceCode);
	GLES31.glCompileShader(gFragmentShaderObject);
	iShaderCompiledStatus[0] = 0;
	iInfoLogLength[0] = 0;
	szInfoLog = null;
	GLES31.glGetShaderiv(gFragmentShaderObject,GLES31.GL_COMPILE_STATUS,iShaderCompiledStatus, 0);
	if(iShaderCompiledStatus[0] == GLES31.GL_FALSE)
	{
		GLES31.glGetShaderiv(gFragmentShaderObject, GLES31.GL_INFO_LOG_LENGTH, iInfoLogLength, 0);
		if(iInfoLogLength[0] > 0)
		{
			szInfoLog = GLES31.glGetShaderInfoLog(gFragmentShaderObject);
			System.out.println("OGLES: Fragment Shader Compilation Log = "+szInfoLog);
			uninitialize();
			System.exit(0);
		}
	}
	System.out.println("OGLES: In Initialize : After Fragment Shader Compilation");

	gShaderProgramObject = GLES31.glCreateProgram();
	GLES31.glAttachShader(gShaderProgramObject,gVertexShaderObject);
	GLES31.glAttachShader(gShaderProgramObject,gFragmentShaderObject);
	GLES31.glBindAttribLocation(gShaderProgramObject,GLESMacros.ARD_ATTRIBUTE_VERTEX,"vPosition");
	GLES31.glBindAttribLocation(gShaderProgramObject,GLESMacros.ARD_ATTRIBUTE_NORMAL, "vNormal");
	GLES31.glLinkProgram(gShaderProgramObject);
	int[] iShaderProgramLinkStatus = new int[1];
	iInfoLogLength[0] = 0;
	szInfoLog = null;
	GLES31.glGetProgramiv(gShaderProgramObject, GLES31.GL_LINK_STATUS, iShaderProgramLinkStatus, 0);
	if(iShaderProgramLinkStatus[0] == GLES31.GL_FALSE)
	{
		GLES31.glGetProgramiv(gShaderProgramObject, GLES31.GL_INFO_LOG_LENGTH, iInfoLogLength, 0);
		if(iInfoLogLength[0] > 0)
		{
			szInfoLog = GLES31.glGetProgramInfoLog(gShaderProgramObject);
			System.out.println("OGLES: Shader Program Link Log = "+szInfoLog);
			uninitialize();
			System.exit(0);
		}
	}

	System.out.println("OGLES: In Initialize : Before glGetUniformLocation");
	gModelMatrixUniform = GLES31.glGetUniformLocation(gShaderProgramObject, "u_model_matrix");
	gViewMatrixUniform = GLES31.glGetUniformLocation(gShaderProgramObject, "u_view_matrix");
	gProjectionMatrixUniform = GLES31.glGetUniformLocation(gShaderProgramObject, "u_projection_matrix");

	//gLKeyPressedUniform = GLES31.glGetUniformLocation(gShaderProgramObject, "u_lighting_enabled");
	gSphereNo = GLES31.glGetUniformLocation(gShaderProgramObject, "sphere");
	doubleTapUniform = GLES31.glGetUniformLocation(gShaderProgramObject, "u_double_tap");
	gLaUniform = GLES31.glGetUniformLocation(gShaderProgramObject, "u_La");
	gKa1Uniform = GLES31.glGetUniformLocation(gShaderProgramObject, "u_Ka1");
	gKa2Uniform = GLES31.glGetUniformLocation(gShaderProgramObject, "u_Ka2");
	gKa3Uniform = GLES31.glGetUniformLocation(gShaderProgramObject, "u_Ka3");
	gKa4Uniform = GLES31.glGetUniformLocation(gShaderProgramObject, "u_Ka4");
	gKa5Uniform = GLES31.glGetUniformLocation(gShaderProgramObject, "u_Ka5");
	gKa6Uniform = GLES31.glGetUniformLocation(gShaderProgramObject, "u_Ka6");
	gKa7Uniform = GLES31.glGetUniformLocation(gShaderProgramObject, "u_Ka7");
	gKa8Uniform = GLES31.glGetUniformLocation(gShaderProgramObject, "u_Ka8");
	gKa9Uniform = GLES31.glGetUniformLocation(gShaderProgramObject, "u_Ka9");
	gKa10Uniform = GLES31.glGetUniformLocation(gShaderProgramObject, "u_Ka10");
	gKa11Uniform = GLES31.glGetUniformLocation(gShaderProgramObject, "u_Ka11");
	gKa12Uniform = GLES31.glGetUniformLocation(gShaderProgramObject, "u_Ka12");
	gKa13Uniform = GLES31.glGetUniformLocation(gShaderProgramObject, "u_Ka13");
	gKa14Uniform = GLES31.glGetUniformLocation(gShaderProgramObject, "u_Ka14");
	gKa15Uniform = GLES31.glGetUniformLocation(gShaderProgramObject, "u_Ka15");
	gKa16Uniform = GLES31.glGetUniformLocation(gShaderProgramObject, "u_Ka16");
	gKa17Uniform = GLES31.glGetUniformLocation(gShaderProgramObject, "u_Ka17");
	gKa18Uniform = GLES31.glGetUniformLocation(gShaderProgramObject, "u_Ka18");
	gKa19Uniform = GLES31.glGetUniformLocation(gShaderProgramObject, "u_Ka19");
	gKa20Uniform = GLES31.glGetUniformLocation(gShaderProgramObject, "u_Ka20");
	gKa21Uniform = GLES31.glGetUniformLocation(gShaderProgramObject, "u_Ka21");
	gKa22Uniform = GLES31.glGetUniformLocation(gShaderProgramObject, "u_Ka22");
	gKa23Uniform = GLES31.glGetUniformLocation(gShaderProgramObject, "u_Ka23");
	gKa24Uniform = GLES31.glGetUniformLocation(gShaderProgramObject, "u_Ka24");

	gLdUniform = GLES31.glGetUniformLocation(gShaderProgramObject, "u_Ld");
	gKd1Uniform = GLES31.glGetUniformLocation(gShaderProgramObject, "u_Kd1");
	gKd2Uniform = GLES31.glGetUniformLocation(gShaderProgramObject, "u_Kd2");
	gKd3Uniform = GLES31.glGetUniformLocation(gShaderProgramObject, "u_Kd3");
	gKd4Uniform = GLES31.glGetUniformLocation(gShaderProgramObject, "u_Kd4");
	gKd5Uniform = GLES31.glGetUniformLocation(gShaderProgramObject, "u_Kd5");
	gKd6Uniform = GLES31.glGetUniformLocation(gShaderProgramObject, "u_Kd6");
	gKd7Uniform = GLES31.glGetUniformLocation(gShaderProgramObject, "u_Kd7");
	gKd8Uniform = GLES31.glGetUniformLocation(gShaderProgramObject, "u_Kd8");
	gKd9Uniform = GLES31.glGetUniformLocation(gShaderProgramObject, "u_Kd9");
	gKd10Uniform = GLES31.glGetUniformLocation(gShaderProgramObject, "u_Kd10");
	gKd11Uniform = GLES31.glGetUniformLocation(gShaderProgramObject, "u_Kd11");
	gKd12Uniform = GLES31.glGetUniformLocation(gShaderProgramObject, "u_Kd12");
	gKd13Uniform = GLES31.glGetUniformLocation(gShaderProgramObject, "u_Kd13");
	gKd14Uniform = GLES31.glGetUniformLocation(gShaderProgramObject, "u_Kd14");
	gKd15Uniform = GLES31.glGetUniformLocation(gShaderProgramObject, "u_Kd15");
	gKd16Uniform = GLES31.glGetUniformLocation(gShaderProgramObject, "u_Kd16");
	gKd17Uniform = GLES31.glGetUniformLocation(gShaderProgramObject, "u_Kd17");
	gKd18Uniform = GLES31.glGetUniformLocation(gShaderProgramObject, "u_Kd18");
	gKd19Uniform = GLES31.glGetUniformLocation(gShaderProgramObject, "u_Kd19");
	gKd20Uniform = GLES31.glGetUniformLocation(gShaderProgramObject, "u_Kd20");
	gKd21Uniform = GLES31.glGetUniformLocation(gShaderProgramObject, "u_Kd21");
	gKd22Uniform = GLES31.glGetUniformLocation(gShaderProgramObject, "u_Kd22");
	gKd23Uniform = GLES31.glGetUniformLocation(gShaderProgramObject, "u_Kd23");
	gKd24Uniform = GLES31.glGetUniformLocation(gShaderProgramObject, "u_Kd24");

	gLsUniform = GLES31.glGetUniformLocation(gShaderProgramObject, "u_Ls");
	gKs1Uniform = GLES31.glGetUniformLocation(gShaderProgramObject, "u_Ks1");
	gKs2Uniform = GLES31.glGetUniformLocation(gShaderProgramObject, "u_Ks2");
	gKs3Uniform = GLES31.glGetUniformLocation(gShaderProgramObject, "u_Ks3");
	gKs4Uniform = GLES31.glGetUniformLocation(gShaderProgramObject, "u_Ks4");
	gKs5Uniform = GLES31.glGetUniformLocation(gShaderProgramObject, "u_Ks5");
	gKs6Uniform = GLES31.glGetUniformLocation(gShaderProgramObject, "u_Ks6");
	gKs7Uniform = GLES31.glGetUniformLocation(gShaderProgramObject, "u_Ks7");
	gKs8Uniform = GLES31.glGetUniformLocation(gShaderProgramObject, "u_Ks8");
	gKs9Uniform = GLES31.glGetUniformLocation(gShaderProgramObject, "u_Ks9");
	gKs10Uniform = GLES31.glGetUniformLocation(gShaderProgramObject, "u_Ks10");
	gKs11Uniform = GLES31.glGetUniformLocation(gShaderProgramObject, "u_Ks11");
	gKs12Uniform = GLES31.glGetUniformLocation(gShaderProgramObject, "u_Ks12");
	gKs13Uniform = GLES31.glGetUniformLocation(gShaderProgramObject, "u_Ks13");
	gKs14Uniform = GLES31.glGetUniformLocation(gShaderProgramObject, "u_Ks14");
	gKs15Uniform = GLES31.glGetUniformLocation(gShaderProgramObject, "u_Ks15");
	gKs16Uniform = GLES31.glGetUniformLocation(gShaderProgramObject, "u_Ks16");
	gKs17Uniform = GLES31.glGetUniformLocation(gShaderProgramObject, "u_Ks17");
	gKs18Uniform = GLES31.glGetUniformLocation(gShaderProgramObject, "u_Ks18");
	gKs19Uniform = GLES31.glGetUniformLocation(gShaderProgramObject, "u_Ks19");
	gKs20Uniform = GLES31.glGetUniformLocation(gShaderProgramObject, "u_Ks20");
	gKs21Uniform = GLES31.glGetUniformLocation(gShaderProgramObject, "u_Ks21");
	gKs22Uniform = GLES31.glGetUniformLocation(gShaderProgramObject, "u_Ks22");
	gKs23Uniform = GLES31.glGetUniformLocation(gShaderProgramObject, "u_Ks23");
	gKs24Uniform = GLES31.glGetUniformLocation(gShaderProgramObject, "u_Ks24");

	glight_position_uniform = GLES31.glGetUniformLocation(gShaderProgramObject, "u_light_position");

	gmaterial1_shininess_uniform = GLES31.glGetUniformLocation(gShaderProgramObject, "u_material1_shininess");
	gmaterial2_shininess_uniform = GLES31.glGetUniformLocation(gShaderProgramObject, "u_material2_shininess");
	gmaterial3_shininess_uniform = GLES31.glGetUniformLocation(gShaderProgramObject, "u_material3_shininess");
	gmaterial4_shininess_uniform = GLES31.glGetUniformLocation(gShaderProgramObject, "u_material4_shininess");
	gmaterial5_shininess_uniform = GLES31.glGetUniformLocation(gShaderProgramObject, "u_material5_shininess");
	gmaterial6_shininess_uniform = GLES31.glGetUniformLocation(gShaderProgramObject, "u_material6_shininess");
	gmaterial7_shininess_uniform = GLES31.glGetUniformLocation(gShaderProgramObject, "u_material7_shininess");
	gmaterial8_shininess_uniform = GLES31.glGetUniformLocation(gShaderProgramObject, "u_material8_shininess");
	gmaterial9_shininess_uniform = GLES31.glGetUniformLocation(gShaderProgramObject, "u_material9_shininess");
	gmaterial10_shininess_uniform = GLES31.glGetUniformLocation(gShaderProgramObject, "u_material10_shininess");
	gmaterial11_shininess_uniform = GLES31.glGetUniformLocation(gShaderProgramObject, "u_material11_shininess");
	gmaterial12_shininess_uniform = GLES31.glGetUniformLocation(gShaderProgramObject, "u_material12_shininess");
	gmaterial13_shininess_uniform = GLES31.glGetUniformLocation(gShaderProgramObject, "u_material13_shininess");
	gmaterial14_shininess_uniform = GLES31.glGetUniformLocation(gShaderProgramObject, "u_material14_shininess");
	gmaterial15_shininess_uniform = GLES31.glGetUniformLocation(gShaderProgramObject, "u_material15_shininess");
	gmaterial16_shininess_uniform = GLES31.glGetUniformLocation(gShaderProgramObject, "u_material16_shininess");
	gmaterial17_shininess_uniform = GLES31.glGetUniformLocation(gShaderProgramObject, "u_material17_shininess");
	gmaterial18_shininess_uniform = GLES31.glGetUniformLocation(gShaderProgramObject, "u_material18_shininess");
	gmaterial19_shininess_uniform = GLES31.glGetUniformLocation(gShaderProgramObject, "u_material19_shininess");
	gmaterial20_shininess_uniform = GLES31.glGetUniformLocation(gShaderProgramObject, "u_material20_shininess");
	gmaterial21_shininess_uniform = GLES31.glGetUniformLocation(gShaderProgramObject, "u_material21_shininess");
	gmaterial22_shininess_uniform = GLES31.glGetUniformLocation(gShaderProgramObject, "u_material22_shininess");
	gmaterial23_shininess_uniform = GLES31.glGetUniformLocation(gShaderProgramObject, "u_material23_shininess");
	gmaterial24_shininess_uniform = GLES31.glGetUniformLocation(gShaderProgramObject, "u_material24_shininess");
	System.out.println("In Initialize : After glGetUniformLocation");

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
	GLES31.glClearColor(0.5f, 0.5f, 0.5f, 0.0f);

	doubleTap = 0;

	Matrix.setIdentityM(perspectiveProjectionMatrix, 0);
	System.out.println("OGLES: In Initialize : End");
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
	GLES31.glUseProgram(gShaderProgramObject);

	if(doubleTap == 1)
	{
		GLES31.glUniform1i(doubleTapUniform, 1);
		GLES31.glUniform3fv(gLaUniform, 1, light_ambient, 0);
		GLES31.glUniform3fv(gLdUniform, 1, light_diffuse, 0);
		GLES31.glUniform3fv(gLsUniform, 1, light_specular, 0);
	
		

		if (singleTap == 1)
		{
			lightPosition[0] = 0.0f;
			lightPosition[1] = 100 * (float)(Math.sin(angleX_axis));
			lightPosition[2] = 100 * (float)(Math.cos(angleX_axis));
			lightPosition[3] = 1.0f;
		}

		if (singleTap == 2)
		{
			lightPosition[0] = 100 * (float)(Math.cos(angleY_axis));
			lightPosition[1] = 0.0f;
			lightPosition[2] = 100 * (float)(Math.sin(angleY_axis));
			lightPosition[3] = 1.0f;
		}

		if (singleTap == 3)
		{
			lightPosition[0] = 100 * (float)(Math.cos(angleZ_axis));
			lightPosition[1] = 100 * (float)(Math.sin(angleZ_axis));
			lightPosition[2] = 0.0f;
			lightPosition[3] = 1.0f;
		}

		GLES31.glUniform4fv(glight_position_uniform, 1, lightPosition, 0);
	}

	else
	{
		GLES31.glUniform1i(doubleTapUniform, 0);
	}

	float modelMatrix[] = new float[16];
	float viewMatrix[] = new float[16];
	//float[] scaleMatrix = new float[] { 0.50f, 0.50f, 0.50f };
/////////////////////////////////////////////////////1st sphere/////////////////////////////////////////
	Matrix.setIdentityM(modelMatrix, 0);
	Matrix.setIdentityM(viewMatrix, 0);
	
	Matrix.translateM(modelMatrix, 0, -3.0f, 2.0f, -6.0f);
	Matrix.scaleM(modelMatrix, 0, 0.75f, 0.75f, 0.75f);
	//Matrix.rotateM(rotationMatrix, 0, angle_cube, 1.0f, 1.0f, 1.0f);
	//Matrix.multiplyMM(modelMatrix, 0, modelMatrix, 0, scaleMatrix, 0);
	GLES31.glUniformMatrix4fv(gModelMatrixUniform, 1, false, modelMatrix, 0);
	GLES31.glUniformMatrix4fv(gViewMatrixUniform, 1, false, viewMatrix, 0);
	GLES31.glUniform1i(gSphereNo, 1);
	GLES31.glUniform3fv(gKa1Uniform, 1, material1_ambient, 0);
	GLES31.glUniform3fv(gKd1Uniform, 1, material1_diffuse, 0);
	GLES31.glUniform3fv(gKs1Uniform, 1, material1_specular, 0);
	GLES31.glUniform1f(gmaterial1_shininess_uniform, material1_shininess);
	GLES31.glUniformMatrix4fv(gProjectionMatrixUniform, 1, false, perspectiveProjectionMatrix, 0);
	GLES31.glBindVertexArray(vao_sphere[0]);
        
        // *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
    GLES31.glBindBuffer(GLES31.GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element[0]);
    GLES31.glDrawElements(GLES31.GL_TRIANGLES, numElements, GLES31.GL_UNSIGNED_SHORT, 0);
        
        // unbind vao
    GLES31.glBindVertexArray(0);

  ////////////////////////////2nd Sphere////////////////////////////////////////////

    Matrix.setIdentityM(modelMatrix, 0);
	Matrix.setIdentityM(viewMatrix, 0);
	
	Matrix.translateM(modelMatrix, 0, -1.0f, 2.0f, -6.0f);
	Matrix.scaleM(modelMatrix, 0, 0.75f, 0.75f, 0.75f);
	//Matrix.rotateM(rotationMatrix, 0, angle_cube, 1.0f, 1.0f, 1.0f);
	//Matrix.multiplyMM(modelMatrix, 0, modelMatrix, 0, scaleMatrix, 0);
	GLES31.glUniformMatrix4fv(gModelMatrixUniform, 1, false, modelMatrix, 0);
	GLES31.glUniformMatrix4fv(gViewMatrixUniform, 1, false, viewMatrix, 0);
	GLES31.glUniform1i(gSphereNo, 2);
	GLES31.glUniform3fv(gKa2Uniform, 1, material2_ambient, 0);
	GLES31.glUniform3fv(gKd2Uniform, 1, material2_diffuse, 0);
	GLES31.glUniform3fv(gKs2Uniform, 1, material2_specular, 0);
	GLES31.glUniform1f(gmaterial2_shininess_uniform, material2_shininess);
	GLES31.glUniformMatrix4fv(gProjectionMatrixUniform, 1, false, perspectiveProjectionMatrix, 0);
	GLES31.glBindVertexArray(vao_sphere[0]);
        
        // *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
    GLES31.glBindBuffer(GLES31.GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element[0]);
    GLES31.glDrawElements(GLES31.GL_TRIANGLES, numElements, GLES31.GL_UNSIGNED_SHORT, 0);
        
        // unbind vao
    GLES31.glBindVertexArray(0);




////////////////////////////3nd Sphere////////////////////////////////////////////

    Matrix.setIdentityM(modelMatrix, 0);
	Matrix.setIdentityM(viewMatrix, 0);
	
	Matrix.translateM(modelMatrix, 0, 1.0f, 2.0f, -6.0f);
	Matrix.scaleM(modelMatrix, 0, 0.75f, 0.75f, 0.75f);
	//Matrix.rotateM(rotationMatrix, 0, angle_cube, 1.0f, 1.0f, 1.0f);
	//Matrix.multiplyMM(modelMatrix, 0, modelMatrix, 0, scaleMatrix, 0);
	GLES31.glUniformMatrix4fv(gModelMatrixUniform, 1, false, modelMatrix, 0);
	GLES31.glUniformMatrix4fv(gViewMatrixUniform, 1, false, viewMatrix, 0);
	GLES31.glUniform1i(gSphereNo, 3);
	GLES31.glUniform3fv(gKa3Uniform, 1, material3_ambient, 0);
	GLES31.glUniform3fv(gKd3Uniform, 1, material3_diffuse, 0);
	GLES31.glUniform3fv(gKs3Uniform, 1, material3_specular, 0);
	GLES31.glUniform1f(gmaterial3_shininess_uniform, material3_shininess);
	GLES31.glUniformMatrix4fv(gProjectionMatrixUniform, 1, false, perspectiveProjectionMatrix, 0);
	GLES31.glBindVertexArray(vao_sphere[0]);
        
        // *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
    GLES31.glBindBuffer(GLES31.GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element[0]);
    GLES31.glDrawElements(GLES31.GL_TRIANGLES, numElements, GLES31.GL_UNSIGNED_SHORT, 0);
        
        // unbind vao
    GLES31.glBindVertexArray(0);



////////////////////////////4th Sphere////////////////////////////////////////////

    Matrix.setIdentityM(modelMatrix, 0);
	Matrix.setIdentityM(viewMatrix, 0);
	
	Matrix.translateM(modelMatrix, 0, 3.0f, 2.0f, -6.0f);
	Matrix.scaleM(modelMatrix, 0, 0.75f, 0.75f, 0.75f);
	//Matrix.rotateM(rotationMatrix, 0, angle_cube, 1.0f, 1.0f, 1.0f);
	//Matrix.multiplyMM(modelMatrix, 0, modelMatrix, 0, scaleMatrix, 0);
	GLES31.glUniformMatrix4fv(gModelMatrixUniform, 1, false, modelMatrix, 0);
	GLES31.glUniformMatrix4fv(gViewMatrixUniform, 1, false, viewMatrix, 0);
	GLES31.glUniform1i(gSphereNo, 4);
	GLES31.glUniform3fv(gKa4Uniform, 1, material4_ambient, 0);
	GLES31.glUniform3fv(gKd4Uniform, 1, material4_diffuse, 0);
	GLES31.glUniform3fv(gKs4Uniform, 1, material4_specular, 0);
	GLES31.glUniform1f(gmaterial4_shininess_uniform, material4_shininess);
	GLES31.glUniformMatrix4fv(gProjectionMatrixUniform, 1, false, perspectiveProjectionMatrix, 0);
	GLES31.glBindVertexArray(vao_sphere[0]);
        
        // *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
    GLES31.glBindBuffer(GLES31.GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element[0]);
    GLES31.glDrawElements(GLES31.GL_TRIANGLES, numElements, GLES31.GL_UNSIGNED_SHORT, 0);
        
        // unbind vao
    GLES31.glBindVertexArray(0);

   ////////////////////////////5th Sphere////////////////////////////////////////////

    Matrix.setIdentityM(modelMatrix, 0);
	Matrix.setIdentityM(viewMatrix, 0);
	
	Matrix.translateM(modelMatrix, 0, -3.0f, 1.2f, -6.0f);
	Matrix.scaleM(modelMatrix, 0, 0.75f, 0.75f, 0.75f);
	//Matrix.rotateM(rotationMatrix, 0, angle_cube, 1.0f, 1.0f, 1.0f);
	//Matrix.multiplyMM(modelMatrix, 0, modelMatrix, 0, scaleMatrix, 0);
	GLES31.glUniformMatrix4fv(gModelMatrixUniform, 1, false, modelMatrix, 0);
	GLES31.glUniformMatrix4fv(gViewMatrixUniform, 1, false, viewMatrix, 0);
	GLES31.glUniform1i(gSphereNo, 5);
	GLES31.glUniform3fv(gKa5Uniform, 1, material5_ambient, 0);
	GLES31.glUniform3fv(gKd5Uniform, 1, material5_diffuse, 0);
	GLES31.glUniform3fv(gKs5Uniform, 1, material5_specular, 0);
	GLES31.glUniform1f(gmaterial5_shininess_uniform, material5_shininess);
	GLES31.glUniformMatrix4fv(gProjectionMatrixUniform, 1, false, perspectiveProjectionMatrix, 0);
	GLES31.glBindVertexArray(vao_sphere[0]);
        
        // *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
    GLES31.glBindBuffer(GLES31.GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element[0]);
    GLES31.glDrawElements(GLES31.GL_TRIANGLES, numElements, GLES31.GL_UNSIGNED_SHORT, 0);
        
        // unbind vao
    GLES31.glBindVertexArray(0);

////////////////////////////6th Sphere////////////////////////////////////////////

    Matrix.setIdentityM(modelMatrix, 0);
	Matrix.setIdentityM(viewMatrix, 0);
	
	Matrix.translateM(modelMatrix, 0, -1.0f, 1.2f, -6.0f);
	Matrix.scaleM(modelMatrix, 0, 0.75f, 0.75f, 0.75f);
	//Matrix.rotateM(rotationMatrix, 0, angle_cube, 1.0f, 1.0f, 1.0f);
	//Matrix.multiplyMM(modelMatrix, 0, modelMatrix, 0, scaleMatrix, 0);
	GLES31.glUniformMatrix4fv(gModelMatrixUniform, 1, false, modelMatrix, 0);
	GLES31.glUniformMatrix4fv(gViewMatrixUniform, 1, false, viewMatrix, 0);
	GLES31.glUniform1i(gSphereNo, 6);
	GLES31.glUniform3fv(gKa6Uniform, 1, material6_ambient, 0);
	GLES31.glUniform3fv(gKd6Uniform, 1, material6_diffuse, 0);
	GLES31.glUniform3fv(gKs6Uniform, 1, material6_specular, 0);
	GLES31.glUniform1f(gmaterial6_shininess_uniform, material6_shininess);
	GLES31.glUniformMatrix4fv(gProjectionMatrixUniform, 1, false, perspectiveProjectionMatrix, 0);
	GLES31.glBindVertexArray(vao_sphere[0]);
        
        // *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
    GLES31.glBindBuffer(GLES31.GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element[0]);
    GLES31.glDrawElements(GLES31.GL_TRIANGLES, numElements, GLES31.GL_UNSIGNED_SHORT, 0);
        
        // unbind vao
    GLES31.glBindVertexArray(0);

////////////////////////////7th Sphere////////////////////////////////////////////

    Matrix.setIdentityM(modelMatrix, 0);
	Matrix.setIdentityM(viewMatrix, 0);
	
	Matrix.translateM(modelMatrix, 0, 1.0f, 1.2f, -6.0f);
	Matrix.scaleM(modelMatrix, 0, 0.75f, 0.75f, 0.75f);
	//Matrix.rotateM(rotationMatrix, 0, angle_cube, 1.0f, 1.0f, 1.0f);
	//Matrix.multiplyMM(modelMatrix, 0, modelMatrix, 0, scaleMatrix, 0);
	GLES31.glUniformMatrix4fv(gModelMatrixUniform, 1, false, modelMatrix, 0);
	GLES31.glUniformMatrix4fv(gViewMatrixUniform, 1, false, viewMatrix, 0);
	GLES31.glUniform1i(gSphereNo, 7);
	GLES31.glUniform3fv(gKa7Uniform, 1, material7_ambient, 0);
	GLES31.glUniform3fv(gKd7Uniform, 1, material7_diffuse, 0);
	GLES31.glUniform3fv(gKs7Uniform, 1, material7_specular, 0);
	GLES31.glUniform1f(gmaterial7_shininess_uniform, material7_shininess);
	GLES31.glUniformMatrix4fv(gProjectionMatrixUniform, 1, false, perspectiveProjectionMatrix, 0);
	GLES31.glBindVertexArray(vao_sphere[0]);
        
        // *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
    GLES31.glBindBuffer(GLES31.GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element[0]);
    GLES31.glDrawElements(GLES31.GL_TRIANGLES, numElements, GLES31.GL_UNSIGNED_SHORT, 0);
        
        // unbind vao
    GLES31.glBindVertexArray(0);

////////////////////////////8th Sphere////////////////////////////////////////////

    Matrix.setIdentityM(modelMatrix, 0);
	Matrix.setIdentityM(viewMatrix, 0);
	
	Matrix.translateM(modelMatrix, 0, 3.0f, 1.2f, -6.0f);
	Matrix.scaleM(modelMatrix, 0, 0.75f, 0.75f, 0.75f);
	//Matrix.rotateM(rotationMatrix, 0, angle_cube, 1.0f, 1.0f, 1.0f);
	//Matrix.multiplyMM(modelMatrix, 0, modelMatrix, 0, scaleMatrix, 0);
	GLES31.glUniformMatrix4fv(gModelMatrixUniform, 1, false, modelMatrix, 0);
	GLES31.glUniformMatrix4fv(gViewMatrixUniform, 1, false, viewMatrix, 0);
	GLES31.glUniform1i(gSphereNo, 8);
	GLES31.glUniform3fv(gKa8Uniform, 1, material8_ambient, 0);
	GLES31.glUniform3fv(gKd8Uniform, 1, material8_diffuse, 0);
	GLES31.glUniform3fv(gKs8Uniform, 1, material8_specular, 0);
	GLES31.glUniform1f(gmaterial8_shininess_uniform, material8_shininess);
	GLES31.glUniformMatrix4fv(gProjectionMatrixUniform, 1, false, perspectiveProjectionMatrix, 0);
	GLES31.glBindVertexArray(vao_sphere[0]);
        
        // *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
    GLES31.glBindBuffer(GLES31.GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element[0]);
    GLES31.glDrawElements(GLES31.GL_TRIANGLES, numElements, GLES31.GL_UNSIGNED_SHORT, 0);
        
        // unbind vao
    GLES31.glBindVertexArray(0);





////////////////////////////9th Sphere////////////////////////////////////////////

    Matrix.setIdentityM(modelMatrix, 0);
	Matrix.setIdentityM(viewMatrix, 0);
	
	Matrix.translateM(modelMatrix, 0, -3.0f, 0.4f, -6.0f);
	Matrix.scaleM(modelMatrix, 0, 0.75f, 0.75f, 0.75f);
	//Matrix.rotateM(rotationMatrix, 0, angle_cube, 1.0f, 1.0f, 1.0f);
	//Matrix.multiplyMM(modelMatrix, 0, modelMatrix, 0, scaleMatrix, 0);
	GLES31.glUniformMatrix4fv(gModelMatrixUniform, 1, false, modelMatrix, 0);
	GLES31.glUniformMatrix4fv(gViewMatrixUniform, 1, false, viewMatrix, 0);
	GLES31.glUniform1i(gSphereNo, 9);
	GLES31.glUniform3fv(gKa9Uniform, 1, material9_ambient, 0);
	GLES31.glUniform3fv(gKd9Uniform, 1, material9_diffuse, 0);
	GLES31.glUniform3fv(gKs9Uniform, 1, material9_specular, 0);
	GLES31.glUniform1f(gmaterial9_shininess_uniform, material9_shininess);
	GLES31.glUniformMatrix4fv(gProjectionMatrixUniform, 1, false, perspectiveProjectionMatrix, 0);
	GLES31.glBindVertexArray(vao_sphere[0]);
        
        // *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
    GLES31.glBindBuffer(GLES31.GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element[0]);
    GLES31.glDrawElements(GLES31.GL_TRIANGLES, numElements, GLES31.GL_UNSIGNED_SHORT, 0);
        
        // unbind vao
    GLES31.glBindVertexArray(0);


////////////////////////////10th Sphere////////////////////////////////////////////

    Matrix.setIdentityM(modelMatrix, 0);
	Matrix.setIdentityM(viewMatrix, 0);
	
	Matrix.translateM(modelMatrix, 0, -1.0f, 0.4f, -6.0f);
	Matrix.scaleM(modelMatrix, 0, 0.75f, 0.75f, 0.75f);
	//Matrix.rotateM(rotationMatrix, 0, angle_cube, 1.0f, 1.0f, 1.0f);
	//Matrix.multiplyMM(modelMatrix, 0, modelMatrix, 0, scaleMatrix, 0);
	GLES31.glUniformMatrix4fv(gModelMatrixUniform, 1, false, modelMatrix, 0);
	GLES31.glUniformMatrix4fv(gViewMatrixUniform, 1, false, viewMatrix, 0);
	GLES31.glUniform1i(gSphereNo, 10);
	GLES31.glUniform3fv(gKa10Uniform, 1, material10_ambient, 0);
	GLES31.glUniform3fv(gKd10Uniform, 1, material10_diffuse, 0);
	GLES31.glUniform3fv(gKs10Uniform, 1, material10_specular, 0);
	GLES31.glUniform1f(gmaterial10_shininess_uniform, material10_shininess);
	GLES31.glUniformMatrix4fv(gProjectionMatrixUniform, 1, false, perspectiveProjectionMatrix, 0);
	GLES31.glBindVertexArray(vao_sphere[0]);
        
        // *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
    GLES31.glBindBuffer(GLES31.GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element[0]);
    GLES31.glDrawElements(GLES31.GL_TRIANGLES, numElements, GLES31.GL_UNSIGNED_SHORT, 0);
        
        // unbind vao
    GLES31.glBindVertexArray(0);

////////////////////////////11th Sphere////////////////////////////////////////////

    Matrix.setIdentityM(modelMatrix, 0);
	Matrix.setIdentityM(viewMatrix, 0);
	
	Matrix.translateM(modelMatrix, 0, 1.0f, 0.4f, -6.0f);
	Matrix.scaleM(modelMatrix, 0, 0.75f, 0.75f, 0.75f);
	//Matrix.rotateM(rotationMatrix, 0, angle_cube, 1.0f, 1.0f, 1.0f);
	//Matrix.multiplyMM(modelMatrix, 0, modelMatrix, 0, scaleMatrix, 0);
	GLES31.glUniformMatrix4fv(gModelMatrixUniform, 1, false, modelMatrix, 0);
	GLES31.glUniformMatrix4fv(gViewMatrixUniform, 1, false, viewMatrix, 0);
	GLES31.glUniform1i(gSphereNo, 11);
	GLES31.glUniform3fv(gKa11Uniform, 1, material11_ambient, 0);
	GLES31.glUniform3fv(gKd11Uniform, 1, material11_diffuse, 0);
	GLES31.glUniform3fv(gKs11Uniform, 1, material11_specular, 0);
	GLES31.glUniform1f(gmaterial11_shininess_uniform, material11_shininess);
	GLES31.glUniformMatrix4fv(gProjectionMatrixUniform, 1, false, perspectiveProjectionMatrix, 0);
	GLES31.glBindVertexArray(vao_sphere[0]);
        
        // *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
    GLES31.glBindBuffer(GLES31.GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element[0]);
    GLES31.glDrawElements(GLES31.GL_TRIANGLES, numElements, GLES31.GL_UNSIGNED_SHORT, 0);
        
        // unbind vao
    GLES31.glBindVertexArray(0);


////////////////////////////12th Sphere////////////////////////////////////////////

    Matrix.setIdentityM(modelMatrix, 0);
	Matrix.setIdentityM(viewMatrix, 0);
	
	Matrix.translateM(modelMatrix, 0, 3.0f, 0.4f, -6.0f);
	Matrix.scaleM(modelMatrix, 0, 0.75f, 0.75f, 0.75f);
	//Matrix.rotateM(rotationMatrix, 0, angle_cube, 1.0f, 1.0f, 1.0f);
	//Matrix.multiplyMM(modelMatrix, 0, modelMatrix, 0, scaleMatrix, 0);
	GLES31.glUniformMatrix4fv(gModelMatrixUniform, 1, false, modelMatrix, 0);
	GLES31.glUniformMatrix4fv(gViewMatrixUniform, 1, false, viewMatrix, 0);
	GLES31.glUniform1i(gSphereNo, 12);
	GLES31.glUniform3fv(gKa12Uniform, 1, material12_ambient, 0);
	GLES31.glUniform3fv(gKd12Uniform, 1, material12_diffuse, 0);
	GLES31.glUniform3fv(gKs12Uniform, 1, material12_specular, 0);
	GLES31.glUniform1f(gmaterial12_shininess_uniform, material12_shininess);
	GLES31.glUniformMatrix4fv(gProjectionMatrixUniform, 1, false, perspectiveProjectionMatrix, 0);
	GLES31.glBindVertexArray(vao_sphere[0]);
        
        // *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
    GLES31.glBindBuffer(GLES31.GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element[0]);
    GLES31.glDrawElements(GLES31.GL_TRIANGLES, numElements, GLES31.GL_UNSIGNED_SHORT, 0);
        
        // unbind vao
    GLES31.glBindVertexArray(0);


 ////////////////////////////13th Sphere////////////////////////////////////////////

    Matrix.setIdentityM(modelMatrix, 0);
	Matrix.setIdentityM(viewMatrix, 0);
	
	Matrix.translateM(modelMatrix, 0, -3.0f, -0.4f, -6.0f);
	Matrix.scaleM(modelMatrix, 0, 0.75f, 0.75f, 0.75f);
	//Matrix.rotateM(rotationMatrix, 0, angle_cube, 1.0f, 1.0f, 1.0f);
	//Matrix.multiplyMM(modelMatrix, 0, modelMatrix, 0, scaleMatrix, 0);
	GLES31.glUniformMatrix4fv(gModelMatrixUniform, 1, false, modelMatrix, 0);
	GLES31.glUniformMatrix4fv(gViewMatrixUniform, 1, false, viewMatrix, 0);
	GLES31.glUniform1i(gSphereNo, 13);
	GLES31.glUniform3fv(gKa13Uniform, 1, material13_ambient, 0);
	GLES31.glUniform3fv(gKd13Uniform, 1, material13_diffuse, 0);
	GLES31.glUniform3fv(gKs13Uniform, 1, material13_specular, 0);
	GLES31.glUniform1f(gmaterial13_shininess_uniform, material13_shininess);
	GLES31.glUniformMatrix4fv(gProjectionMatrixUniform, 1, false, perspectiveProjectionMatrix, 0);
	GLES31.glBindVertexArray(vao_sphere[0]);
        
        // *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
    GLES31.glBindBuffer(GLES31.GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element[0]);
    GLES31.glDrawElements(GLES31.GL_TRIANGLES, numElements, GLES31.GL_UNSIGNED_SHORT, 0);
        
        // unbind vao
    GLES31.glBindVertexArray(0);


////////////////////////////14th Sphere////////////////////////////////////////////

    Matrix.setIdentityM(modelMatrix, 0);
	Matrix.setIdentityM(viewMatrix, 0);
	
	Matrix.translateM(modelMatrix, 0, -1.0f, -0.4f, -6.0f);
	Matrix.scaleM(modelMatrix, 0, 0.75f, 0.75f, 0.75f);
	//Matrix.rotateM(rotationMatrix, 0, angle_cube, 1.0f, 1.0f, 1.0f);
	//Matrix.multiplyMM(modelMatrix, 0, modelMatrix, 0, scaleMatrix, 0);
	GLES31.glUniformMatrix4fv(gModelMatrixUniform, 1, false, modelMatrix, 0);
	GLES31.glUniformMatrix4fv(gViewMatrixUniform, 1, false, viewMatrix, 0);
	GLES31.glUniform1i(gSphereNo, 14);
	GLES31.glUniform3fv(gKa14Uniform, 1, material14_ambient, 0);
	GLES31.glUniform3fv(gKd14Uniform, 1, material14_diffuse, 0);
	GLES31.glUniform3fv(gKs14Uniform, 1, material14_specular, 0);
	GLES31.glUniform1f(gmaterial14_shininess_uniform, material14_shininess);
	GLES31.glUniformMatrix4fv(gProjectionMatrixUniform, 1, false, perspectiveProjectionMatrix, 0);
	GLES31.glBindVertexArray(vao_sphere[0]);
        
        // *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
    GLES31.glBindBuffer(GLES31.GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element[0]);
    GLES31.glDrawElements(GLES31.GL_TRIANGLES, numElements, GLES31.GL_UNSIGNED_SHORT, 0);
        
        // unbind vao
    GLES31.glBindVertexArray(0);



 ////////////////////////////15th Sphere////////////////////////////////////////////

    Matrix.setIdentityM(modelMatrix, 0);
	Matrix.setIdentityM(viewMatrix, 0);
	
	Matrix.translateM(modelMatrix, 0, 1.0f, -0.4f, -6.0f);
	Matrix.scaleM(modelMatrix, 0, 0.75f, 0.75f, 0.75f);
	//Matrix.rotateM(rotationMatrix, 0, angle_cube, 1.0f, 1.0f, 1.0f);
	//Matrix.multiplyMM(modelMatrix, 0, modelMatrix, 0, scaleMatrix, 0);
	GLES31.glUniformMatrix4fv(gModelMatrixUniform, 1, false, modelMatrix, 0);
	GLES31.glUniformMatrix4fv(gViewMatrixUniform, 1, false, viewMatrix, 0);
	GLES31.glUniform1i(gSphereNo, 15);
	GLES31.glUniform3fv(gKa15Uniform, 1, material15_ambient, 0);
	GLES31.glUniform3fv(gKd15Uniform, 1, material15_diffuse, 0);
	GLES31.glUniform3fv(gKs15Uniform, 1, material15_specular, 0);
	GLES31.glUniform1f(gmaterial15_shininess_uniform, material15_shininess);
	GLES31.glUniformMatrix4fv(gProjectionMatrixUniform, 1, false, perspectiveProjectionMatrix, 0);
	GLES31.glBindVertexArray(vao_sphere[0]);
        
        // *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
    GLES31.glBindBuffer(GLES31.GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element[0]);
    GLES31.glDrawElements(GLES31.GL_TRIANGLES, numElements, GLES31.GL_UNSIGNED_SHORT, 0);
        
        // unbind vao
    GLES31.glBindVertexArray(0);



  ////////////////////////////16th Sphere////////////////////////////////////////////

    Matrix.setIdentityM(modelMatrix, 0);
	Matrix.setIdentityM(viewMatrix, 0);
	
	Matrix.translateM(modelMatrix, 0, 3.0f, -0.4f, -6.0f);
	Matrix.scaleM(modelMatrix, 0, 0.75f, 0.75f, 0.75f);
	//Matrix.rotateM(rotationMatrix, 0, angle_cube, 1.0f, 1.0f, 1.0f);
	//Matrix.multiplyMM(modelMatrix, 0, modelMatrix, 0, scaleMatrix, 0);
	GLES31.glUniformMatrix4fv(gModelMatrixUniform, 1, false, modelMatrix, 0);
	GLES31.glUniformMatrix4fv(gViewMatrixUniform, 1, false, viewMatrix, 0);
	GLES31.glUniform1i(gSphereNo, 16);
	GLES31.glUniform3fv(gKa16Uniform, 1, material16_ambient, 0);
	GLES31.glUniform3fv(gKd16Uniform, 1, material16_diffuse, 0);
	GLES31.glUniform3fv(gKs16Uniform, 1, material16_specular, 0);
	GLES31.glUniform1f(gmaterial16_shininess_uniform, material16_shininess);
	GLES31.glUniformMatrix4fv(gProjectionMatrixUniform, 1, false, perspectiveProjectionMatrix, 0);
	GLES31.glBindVertexArray(vao_sphere[0]);
        
        // *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
    GLES31.glBindBuffer(GLES31.GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element[0]);
    GLES31.glDrawElements(GLES31.GL_TRIANGLES, numElements, GLES31.GL_UNSIGNED_SHORT, 0);
        
        // unbind vao
    GLES31.glBindVertexArray(0);




////////////////////////////17th Sphere////////////////////////////////////////////

    Matrix.setIdentityM(modelMatrix, 0);
	Matrix.setIdentityM(viewMatrix, 0);
	
	Matrix.translateM(modelMatrix, 0, -3.0f, -1.2f, -6.0f);
	Matrix.scaleM(modelMatrix, 0, 0.75f, 0.75f, 0.75f);
	//Matrix.rotateM(rotationMatrix, 0, angle_cube, 1.0f, 1.0f, 1.0f);
	//Matrix.multiplyMM(modelMatrix, 0, modelMatrix, 0, scaleMatrix, 0);
	GLES31.glUniformMatrix4fv(gModelMatrixUniform, 1, false, modelMatrix, 0);
	GLES31.glUniformMatrix4fv(gViewMatrixUniform, 1, false, viewMatrix, 0);
	GLES31.glUniform1i(gSphereNo, 17);
	GLES31.glUniform3fv(gKa17Uniform, 1, material17_ambient, 0);
	GLES31.glUniform3fv(gKd17Uniform, 1, material17_diffuse, 0);
	GLES31.glUniform3fv(gKs17Uniform, 1, material17_specular, 0);
	GLES31.glUniform1f(gmaterial17_shininess_uniform, material17_shininess);
	GLES31.glUniformMatrix4fv(gProjectionMatrixUniform, 1, false, perspectiveProjectionMatrix, 0);
	GLES31.glBindVertexArray(vao_sphere[0]);
        
        // *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
    GLES31.glBindBuffer(GLES31.GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element[0]);
    GLES31.glDrawElements(GLES31.GL_TRIANGLES, numElements, GLES31.GL_UNSIGNED_SHORT, 0);
        
        // unbind vao
    GLES31.glBindVertexArray(0);

////////////////////////////18th Sphere////////////////////////////////////////////

    Matrix.setIdentityM(modelMatrix, 0);
	Matrix.setIdentityM(viewMatrix, 0);
	
	Matrix.translateM(modelMatrix, 0, -1.0f, -1.2f, -6.0f);
	Matrix.scaleM(modelMatrix, 0, 0.75f, 0.75f, 0.75f);
	//Matrix.rotateM(rotationMatrix, 0, angle_cube, 1.0f, 1.0f, 1.0f);
	//Matrix.multiplyMM(modelMatrix, 0, modelMatrix, 0, scaleMatrix, 0);
	GLES31.glUniformMatrix4fv(gModelMatrixUniform, 1, false, modelMatrix, 0);
	GLES31.glUniformMatrix4fv(gViewMatrixUniform, 1, false, viewMatrix, 0);
	GLES31.glUniform1i(gSphereNo, 18);
	GLES31.glUniform3fv(gKa18Uniform, 1, material18_ambient, 0);
	GLES31.glUniform3fv(gKd18Uniform, 1, material18_diffuse, 0);
	GLES31.glUniform3fv(gKs18Uniform, 1, material18_specular, 0);
	GLES31.glUniform1f(gmaterial18_shininess_uniform, material18_shininess);
	GLES31.glUniformMatrix4fv(gProjectionMatrixUniform, 1, false, perspectiveProjectionMatrix, 0);
	GLES31.glBindVertexArray(vao_sphere[0]);
        
        // *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
    GLES31.glBindBuffer(GLES31.GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element[0]);
    GLES31.glDrawElements(GLES31.GL_TRIANGLES, numElements, GLES31.GL_UNSIGNED_SHORT, 0);
        
        // unbind vao
    GLES31.glBindVertexArray(0);


 ////////////////////////////19th Sphere////////////////////////////////////////////

    Matrix.setIdentityM(modelMatrix, 0);
	Matrix.setIdentityM(viewMatrix, 0);
	
	Matrix.translateM(modelMatrix, 0, 1.0f, -1.2f, -6.0f);
	Matrix.scaleM(modelMatrix, 0, 0.75f, 0.75f, 0.75f);
	//Matrix.rotateM(rotationMatrix, 0, angle_cube, 1.0f, 1.0f, 1.0f);
	//Matrix.multiplyMM(modelMatrix, 0, modelMatrix, 0, scaleMatrix, 0);
	GLES31.glUniformMatrix4fv(gModelMatrixUniform, 1, false, modelMatrix, 0);
	GLES31.glUniformMatrix4fv(gViewMatrixUniform, 1, false, viewMatrix, 0);
	GLES31.glUniform1i(gSphereNo, 19);
	GLES31.glUniform3fv(gKa19Uniform, 1, material19_ambient, 0);
	GLES31.glUniform3fv(gKd19Uniform, 1, material19_diffuse, 0);
	GLES31.glUniform3fv(gKs19Uniform, 1, material19_specular, 0);
	GLES31.glUniform1f(gmaterial19_shininess_uniform, material19_shininess);
	GLES31.glUniformMatrix4fv(gProjectionMatrixUniform, 1, false, perspectiveProjectionMatrix, 0);
	GLES31.glBindVertexArray(vao_sphere[0]);
        
        // *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
    GLES31.glBindBuffer(GLES31.GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element[0]);
    GLES31.glDrawElements(GLES31.GL_TRIANGLES, numElements, GLES31.GL_UNSIGNED_SHORT, 0);
        
        // unbind vao
    GLES31.glBindVertexArray(0);


////////////////////////////20th Sphere////////////////////////////////////////////

    Matrix.setIdentityM(modelMatrix, 0);
	Matrix.setIdentityM(viewMatrix, 0);
	
	Matrix.translateM(modelMatrix, 0, 3.0f, -1.2f, -6.0f);
	Matrix.scaleM(modelMatrix, 0, 0.75f, 0.75f, 0.75f);
	//Matrix.rotateM(rotationMatrix, 0, angle_cube, 1.0f, 1.0f, 1.0f);
	//Matrix.multiplyMM(modelMatrix, 0, modelMatrix, 0, scaleMatrix, 0);
	GLES31.glUniformMatrix4fv(gModelMatrixUniform, 1, false, modelMatrix, 0);
	GLES31.glUniformMatrix4fv(gViewMatrixUniform, 1, false, viewMatrix, 0);
	GLES31.glUniform1i(gSphereNo, 20);
	GLES31.glUniform3fv(gKa20Uniform, 1, material20_ambient, 0);
	GLES31.glUniform3fv(gKd20Uniform, 1, material20_diffuse, 0);
	GLES31.glUniform3fv(gKs20Uniform, 1, material20_specular, 0);
	GLES31.glUniform1f(gmaterial20_shininess_uniform, material20_shininess);
	GLES31.glUniformMatrix4fv(gProjectionMatrixUniform, 1, false, perspectiveProjectionMatrix, 0);
	GLES31.glBindVertexArray(vao_sphere[0]);
        
        // *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
    GLES31.glBindBuffer(GLES31.GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element[0]);
    GLES31.glDrawElements(GLES31.GL_TRIANGLES, numElements, GLES31.GL_UNSIGNED_SHORT, 0);
        
        // unbind vao
    GLES31.glBindVertexArray(0);

////////////////////////////21st Sphere////////////////////////////////////////////

    Matrix.setIdentityM(modelMatrix, 0);
	Matrix.setIdentityM(viewMatrix, 0);
	
	Matrix.translateM(modelMatrix, 0, -3.0f, -2.0f, -6.0f);
	Matrix.scaleM(modelMatrix, 0, 0.75f, 0.75f, 0.75f);
	//Matrix.rotateM(rotationMatrix, 0, angle_cube, 1.0f, 1.0f, 1.0f);
	//Matrix.multiplyMM(modelMatrix, 0, modelMatrix, 0, scaleMatrix, 0);
	GLES31.glUniformMatrix4fv(gModelMatrixUniform, 1, false, modelMatrix, 0);
	GLES31.glUniformMatrix4fv(gViewMatrixUniform, 1, false, viewMatrix, 0);
	GLES31.glUniform1i(gSphereNo, 21);
	GLES31.glUniform3fv(gKa21Uniform, 1, material21_ambient, 0);
	GLES31.glUniform3fv(gKd21Uniform, 1, material21_diffuse, 0);
	GLES31.glUniform3fv(gKs21Uniform, 1, material21_specular, 0);
	GLES31.glUniform1f(gmaterial21_shininess_uniform, material21_shininess);
	GLES31.glUniformMatrix4fv(gProjectionMatrixUniform, 1, false, perspectiveProjectionMatrix, 0);
	GLES31.glBindVertexArray(vao_sphere[0]);
        
        // *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
    GLES31.glBindBuffer(GLES31.GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element[0]);
    GLES31.glDrawElements(GLES31.GL_TRIANGLES, numElements, GLES31.GL_UNSIGNED_SHORT, 0);
        
        // unbind vao
    GLES31.glBindVertexArray(0);


 ////////////////////////////22nd Sphere////////////////////////////////////////////

    Matrix.setIdentityM(modelMatrix, 0);
	Matrix.setIdentityM(viewMatrix, 0);
	
	Matrix.translateM(modelMatrix, 0, -1.0f, -2.0f, -6.0f);
	Matrix.scaleM(modelMatrix, 0, 0.75f, 0.75f, 0.75f);
	//Matrix.rotateM(rotationMatrix, 0, angle_cube, 1.0f, 1.0f, 1.0f);
	//Matrix.multiplyMM(modelMatrix, 0, modelMatrix, 0, scaleMatrix, 0);
	GLES31.glUniformMatrix4fv(gModelMatrixUniform, 1, false, modelMatrix, 0);
	GLES31.glUniformMatrix4fv(gViewMatrixUniform, 1, false, viewMatrix, 0);
	GLES31.glUniform1i(gSphereNo, 22);
	GLES31.glUniform3fv(gKa22Uniform, 1, material22_ambient, 0);
	GLES31.glUniform3fv(gKd22Uniform, 1, material22_diffuse, 0);
	GLES31.glUniform3fv(gKs22Uniform, 1, material22_specular, 0);
	GLES31.glUniform1f(gmaterial22_shininess_uniform, material22_shininess);
	GLES31.glUniformMatrix4fv(gProjectionMatrixUniform, 1, false, perspectiveProjectionMatrix, 0);
	GLES31.glBindVertexArray(vao_sphere[0]);
        
        // *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
    GLES31.glBindBuffer(GLES31.GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element[0]);
    GLES31.glDrawElements(GLES31.GL_TRIANGLES, numElements, GLES31.GL_UNSIGNED_SHORT, 0);
        
        // unbind vao
    GLES31.glBindVertexArray(0);

////////////////////////////23rd Sphere////////////////////////////////////////////

    Matrix.setIdentityM(modelMatrix, 0);
	Matrix.setIdentityM(viewMatrix, 0);
	
	Matrix.translateM(modelMatrix, 0, 1.0f, -2.0f, -6.0f);
	Matrix.scaleM(modelMatrix, 0, 0.75f, 0.75f, 0.75f);
	//Matrix.rotateM(rotationMatrix, 0, angle_cube, 1.0f, 1.0f, 1.0f);
	//Matrix.multiplyMM(modelMatrix, 0, modelMatrix, 0, scaleMatrix, 0);
	GLES31.glUniformMatrix4fv(gModelMatrixUniform, 1, false, modelMatrix, 0);
	GLES31.glUniformMatrix4fv(gViewMatrixUniform, 1, false, viewMatrix, 0);
	GLES31.glUniform1i(gSphereNo, 23);
	GLES31.glUniform3fv(gKa23Uniform, 1, material23_ambient, 0);
	GLES31.glUniform3fv(gKd23Uniform, 1, material23_diffuse, 0);
	GLES31.glUniform3fv(gKs23Uniform, 1, material23_specular, 0);
	GLES31.glUniform1f(gmaterial23_shininess_uniform, material23_shininess);
	GLES31.glUniformMatrix4fv(gProjectionMatrixUniform, 1, false, perspectiveProjectionMatrix, 0);
	GLES31.glBindVertexArray(vao_sphere[0]);
        
        // *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
    GLES31.glBindBuffer(GLES31.GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element[0]);
    GLES31.glDrawElements(GLES31.GL_TRIANGLES, numElements, GLES31.GL_UNSIGNED_SHORT, 0);
        
        // unbind vao
    GLES31.glBindVertexArray(0);

////////////////////////////24th Sphere////////////////////////////////////////////

    Matrix.setIdentityM(modelMatrix, 0);
	Matrix.setIdentityM(viewMatrix, 0);
	
	Matrix.translateM(modelMatrix, 0, 3.0f, -2.0f, -6.0f);
	Matrix.scaleM(modelMatrix, 0, 0.75f, 0.75f, 0.75f);
	//Matrix.rotateM(rotationMatrix, 0, angle_cube, 1.0f, 1.0f, 1.0f);
	//Matrix.multiplyMM(modelMatrix, 0, modelMatrix, 0, scaleMatrix, 0);
	GLES31.glUniformMatrix4fv(gModelMatrixUniform, 1, false, modelMatrix, 0);
	GLES31.glUniformMatrix4fv(gViewMatrixUniform, 1, false, viewMatrix, 0);
	GLES31.glUniform1i(gSphereNo, 24);
	GLES31.glUniform3fv(gKa24Uniform, 1, material24_ambient, 0);
	GLES31.glUniform3fv(gKd24Uniform, 1, material24_diffuse, 0);
	GLES31.glUniform3fv(gKs24Uniform, 1, material24_specular, 0);
	GLES31.glUniform1f(gmaterial24_shininess_uniform, material24_shininess);
	GLES31.glUniformMatrix4fv(gProjectionMatrixUniform, 1, false, perspectiveProjectionMatrix, 0);
	GLES31.glBindVertexArray(vao_sphere[0]);
        
        // *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
    GLES31.glBindBuffer(GLES31.GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element[0]);
    GLES31.glDrawElements(GLES31.GL_TRIANGLES, numElements, GLES31.GL_UNSIGNED_SHORT, 0);
        
        // unbind vao
    GLES31.glBindVertexArray(0);



	GLES31.glUseProgram(0);
	requestRender();
	spin();
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


	if(gShaderProgramObject != 0)
	{
		if(gVertexShaderObject != 0)
		{
			GLES31.glDetachShader(gShaderProgramObject, gVertexShaderObject);
			GLES31.glDeleteShader(gVertexShaderObject);
			gVertexShaderObject = 0;
		}

		if(gFragmentShaderObject != 0)
		{
			GLES31.glDetachShader(gShaderProgramObject, gFragmentShaderObject);
			GLES31.glDeleteShader(gFragmentShaderObject);
			gFragmentShaderObject = 0;
		}
	}

	if(gShaderProgramObject != 0)
	{
		GLES31.glDeleteProgram(gShaderProgramObject);
		gShaderProgramObject = 0;
	}

}

void spin()
{

		angleX_axis = angleX_axis + 0.05f;

		if (angleX_axis >= 360.0f)
		{
			angleX_axis = angleX_axis - 360.0f;
		}
	

		angleY_axis = angleY_axis + 0.05f;

		if (angleY_axis >= 360.0f)
		{
			angleY_axis = angleY_axis - 360.0f;
		}
	
		angleZ_axis = angleZ_axis + 0.05f;

		if (angleZ_axis >= 360.0f)
		{
			angleZ_axis = angleZ_axis - 360.0f;
		}
	
}

}
