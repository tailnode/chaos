package com.example.glessample;

import java.util.Timer;
import java.util.TimerTask;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.view.MotionEvent;

public class MyGLSurfaceView extends GLSurfaceView {

	public MyGLSurfaceView(Context context) {
		super(context);
		setEGLContextClientVersion(2);
		mRenderer = new MyGLRenderer();
		setRenderer(mRenderer);
		setRenderMode(GLSurfaceView.RENDERMODE_WHEN_DIRTY);
        timer.schedule(task, 0, 25);
	}
	
    public boolean onTouchEvent(MotionEvent e) {
        float x = e.getX();
        float y = e.getY();

        switch (e.getAction()) {
            case MotionEvent.ACTION_MOVE:
            case MotionEvent.ACTION_DOWN:
                mRenderer.move(x, y);
                requestRender();
                break;                
        }
        return true;
    }
    
	private MyGLRenderer mRenderer;

    Handler handler = new Handler() {
        public void handleMessage(Message msg) {
            switch (msg.what) {
            case REDRAW:
                requestRender();
                break;
            }
        }
    };
    
    TimerTask task = new TimerTask() {
        
        @Override
        public void run() {
            Message msg = new Message();
            msg.what = REDRAW;
            handler.sendMessage(msg);
        }
    };
    
    Timer timer = new Timer(true);
    private static final int REDRAW = 1;
}
