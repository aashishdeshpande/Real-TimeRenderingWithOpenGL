package com.example.sknaa.MultipleTexts;

import android.app.Activity;
import android.os.Bundle;
import android.view.Window;
import android.view.WindowManager;
import android.content.pm.ActivityInfo;
import android.graphics.Color;

public class MainActivity extends Activity {
	
	private MyView myView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
		this.requestWindowFeature(Window.FEATURE_NO_TITLE);
		this.getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);
		MainActivity.this.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);
		getWindow().getDecorView().setBackgroundColor(Color.rgb(0,0,0));
      //setContentView(R.layout.activity_main);
	  myView = new MyView(this);
	  setContentView(myView);
    }

	@Override
	protected void onPause()
	{
		super.onPause();
	}

	@Override

	protected void onResume()
	{
		super.onResume();
	}

}

