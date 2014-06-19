package com.example.glessample;

import java.io.IOException;
import java.io.InputStream;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.annotation.SuppressLint;
import android.content.res.AssetManager;
import android.opengl.GLES20;
import android.opengl.GLSurfaceView;
import android.opengl.Matrix;
import android.util.Log;

public class MyGLRenderer implements GLSurfaceView.Renderer {

    @Override
	public void onDrawFrame(GL10 arg0) {
		GLES20.glClear(GLES20.GL_COLOR_BUFFER_BIT | GLES20.GL_DEPTH_BUFFER_BIT);
		
		float[] rotateMatrix = new float[16];
		Matrix.setRotateM(rotateMatrix, 0, angle, 0, 1, 0);
        
        mCuboid.draw(rotateMatrix, mViewMatrix, mProjectionMatrix);
        angle++;
	}

    @SuppressLint("NewApi")
    public void onSurfaceChanged(GL10 arg0, int arg1, int arg2) {
	    w = arg1;
	    h = arg2;
	    Log.i("POSITION", "w = " + w + ", h = " + h);
		GLES20.glViewport(0, 0, arg1, arg2);
		
		if (arg2 == 0) arg2 = 1;
		Matrix.perspectiveM(mProjectionMatrix, 0, 35, (float)arg1 / arg2, 1, 50);
	}

	@Override
	public void onSurfaceCreated(GL10 arg0, EGLConfig arg1) {
        GLES20.glClearColor(1, 1, 1, 1);
        mCuboid = new Cuboid(0.2f, 0.3f, 0.4f);
        Matrix.setLookAtM(mViewMatrix, 0, 0, 0, -3, 0f, 0f, 0f, 0f, 1.0f, 0.0f);
	}
	
	private static String getFileContent(String filePath) {
	    byte[] buffer = null;
        try {
    	    InputStream is = assMnger.open(filePath, AssetManager.ACCESS_BUFFER);
    	    int size = is.available();
    	    buffer = new byte[size];
    	    is.read(buffer);
    	    is.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
        
        return new String(buffer);
	}

	public static int genShaderProgram(String vertexShaderPath, String fragmentShaderPath) {
        String vertexShaderCode = getFileContent(vertexShaderPath);
        String fragmentShaderCode = getFileContent(fragmentShaderPath);
        int vertexShader = GLES20.glCreateShader(GLES20.GL_VERTEX_SHADER);
        int fragmentShader = GLES20.glCreateShader(GLES20.GL_FRAGMENT_SHADER);
        checkGlError("glCreateShader");
        GLES20.glShaderSource(vertexShader, vertexShaderCode);
        GLES20.glCompileShader(vertexShader);
        checkGlError("glCompileShader: vertex shader");
        GLES20.glShaderSource(fragmentShader, fragmentShaderCode);
        GLES20.glCompileShader(fragmentShader);
        checkGlError("glCompileShader: fragment shader");
        
        int program = GLES20.glCreateProgram();
        checkGlError("glCreateProgram");
        GLES20.glAttachShader(program, vertexShader);
        checkGlError("glAttachShader: vertex shader");
        GLES20.glAttachShader(program, fragmentShader);
        checkGlError("glAttachShader: fragment shader");
        GLES20.glLinkProgram(program);
        checkGlError("glLinkProgram");
        
	    return program;
	}
	
    public static void checkGlError(String glOperation) {
        int error;
        while ((error = GLES20.glGetError()) != GLES20.GL_NO_ERROR) {
            Log.e("GL_ERROR", glOperation + ": glError " + error);
            throw new RuntimeException(glOperation + ": glError " + error);
        }
    }

	public void move(float x, float y) {
	    mCuboid.move(x, y);
	}
	
	private float[] mProjectionMatrix = new float[16];
    private float[] mViewMatrix = new float[16];
    
    private Cuboid mCuboid;
    public static AssetManager assMnger;
    private static short angle = 0;
    public static int w;
    public static int h;
}
