package com.example.sknaa.HelloGalaxy;

import android.app.Activity;
import android.os.Bundle;
import android.graphics.Color;
import android.widget.TextView;
import android.view.Gravity;
import android.content.Context;


public class MyView extends TextView
{
	MyView(Context context)
	{
		super(context);
		setTextColor(Color.rgb(0,255,0));
		setText("Hello Galaxy!");
		setTextSize(60);
		setGravity(Gravity.CENTER);
	}
}