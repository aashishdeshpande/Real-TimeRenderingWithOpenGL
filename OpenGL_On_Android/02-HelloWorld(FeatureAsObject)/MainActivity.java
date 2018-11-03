package com.example.sknaa.HelloWorld;

import android.app.Activity;
import android.os.Bundle;
import android.widget.TextView;
import android.graphics.Color;
import android.view.Gravity;
import android.view.Window;
import android.view.WindowManager;
import android.content.pm.ActivityInfo;

public class MainActivity extends Activity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
       // setContentView(R.layout.activity_main);
	   this.requestWindowFeature(Window.FEATURE_NO_TITLE);
	   this.getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);
	   MainActivity.this.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);
	   getWindow().getDecorView().setBackgroundColor(Color.rgb(0,0,0));
	   TextView myTextView = new TextView(this);
	   myTextView.setText("Hello World!");
	   myTextView.setTextSize(60);
	   myTextView.setTextColor(Color.GREEN);
	   myTextView.setGravity(Gravity.CENTER);
	   setContentView(myTextView);

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
