package com.example.sknaa.AndroidWindow;

import android.app.Activity;
import android.os.Bundle;
import android.content.Context;
import android.opengl.GLSurfaceView;
import javax.microedition.khronos.opengles.GL10;
import javax.microedition.khronos.egl.EGLConfig;
import android.opengl.GLES30;
import android.view.MotionEvent;
import android.view.GestureDetector;
import android.view.GestureDetector.OnGestureListener;
import android.view.GestureDetector.OnDoubleTapListener;

public class GLESView extends GLSurfaceView implements GLSurfaceView.Renderer, OnGestureListener, OnDoubleTapListener
{
	private final Context context;
	private GestureDetector gestureDetector;
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

	public void onSurfaceCreated(GL10 gl, EGLConfig config)
	{
		String version = gl.glGetString(GL10.GL_VERSION);
		System.out.println("OpenGLWindow"+version);
		initialize(gl);
	}

	public void onSurfaceChanged(GL10 unuse, int width, int height)
	{
		resize(width, height);
	}

	public void onDrawFrame(GL10 unused)
	{
		draw();
	}

	public boolean onTouchEvent(MotionEvent e)
	{
		int eventaction = e.getAction();
		if(!gestureDetector.onTouchEvent(e))
			super.onTouchEvent(e);
		return(true);
	}

	public boolean onDoubleTap(MotionEvent e)
	{
		System.out.println("OpenGLWindow"+"Double Tap");
		return(true);
	}

	public boolean onDoubleTapEvent(MotionEvent e)
	{
		return(true);
	}

	public boolean onSingleTapConfirmed(MotionEvent e)
	{
		System.out.println("OpenGLWindow"+"Single Tap");
		return(true);
	}

	public boolean onDown(MotionEvent e)
	{
		return(true);
	}

	public boolean onFling(MotionEvent e1, MotionEvent e2, float velocityX,float velocityY)
	{
		return(true);
	}

	public void onLongPress(MotionEvent e)
	{
		System.out.println("OpenGLWindow"+"Long Press");
	}

	public boolean onScroll(MotionEvent e1, MotionEvent e2, float distanceX, float distanceY)
	{
		System.out.println("OpenGLWindow"+"Scroll");
		System.exit(0);
		return(true);
	}

	public void onShowPress(MotionEvent e)
	{

	}

	public boolean onSingleTapUp(MotionEvent e)
	{
		return(true);
	}

	private void initialize(GL10 gl)
	{
		GLES30.glClearColor(0.0f ,0.0f ,1.0f, 1.0f);
	}

	private void resize(int width, int height)
	{
		GLES30.glViewport(0, 0, width, height);
	}

	public void draw()
	{
		GLES30.glClear(GLES30.GL_COLOR_BUFFER_BIT | GLES30.GL_DEPTH_BUFFER_BIT);
	}
}
