package com.example.glessample;

import java.io.IOException;
import java.io.InputStream;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.annotation.SuppressLint;
import android.content.Context;
import android.content.res.AssetManager;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.opengl.GLES20;
import android.opengl.GLSurfaceView;
import android.opengl.Matrix;
import android.util.Log;

public class MyGLRenderer implements GLSurfaceView.Renderer {

	public MyGLRenderer(Context context) {
		mContext = context;
	}

	@Override
	public void onDrawFrame(GL10 arg0) {
		GLES20.glClear(GLES20.GL_COLOR_BUFFER_BIT | GLES20.GL_DEPTH_BUFFER_BIT);

		float[] rotateMatrixByX = new float[16];
		float[] rotateMatrixByY = new float[16];
		float[] rotateMatrix = new float[16];
		// Matrix.setRotateM(rotateMatrix, 0, mAngle, 0, 1, 0);
		Matrix.setRotateM(rotateMatrixByX, 0, mAngle, 1, 0, 0);
		Matrix.setRotateM(rotateMatrixByY, 0, mAngle, 0f,
				(float) Math.cos(Math.toRadians(mAngle)),
				-(float) Math.sin(Math.toRadians(mAngle)));
		Matrix.multiplyMM(rotateMatrix, 0, rotateMatrixByX, 0, rotateMatrixByY,
				0);

		mAngle++;

		mCuboid.draw(rotateMatrix, mViewMatrix, mProjectionMatrix);
//		mMultitexture.draw();
	}

	@SuppressLint("NewApi")
	public void onSurfaceChanged(GL10 arg0, int arg1, int arg2) {
		w = arg1;
		h = arg2;
		GLES20.glViewport(0, 0, arg1, arg2);

		if (arg2 == 0)
			arg2 = 1;
		Matrix.perspectiveM(mProjectionMatrix, 0, 35, (float) arg1 / arg2, 1,
				50);
	}

	@Override
	public void onSurfaceCreated(GL10 arg0, EGLConfig arg1) {
		GLES20.glClearColor(1, 1, 1, 1);
		GLES20.glEnable(GLES20.GL_CULL_FACE);
		mCuboid = new Cuboid(10f, 0.2f, 0.2f);
		Matrix.setLookAtM(mViewMatrix, 0, 0, 0, 10, 0f, 0f, 0f, 0f, 1.0f, 0.0f);
//		mMultitexture = new Multitexture();
	}

	private static String getFileContent(String filePath) {
		byte[] buffer = null;
		try {
			InputStream is = assMnger
					.open(filePath, AssetManager.ACCESS_BUFFER);
			int size = is.available();
			buffer = new byte[size];
			is.read(buffer);
			is.close();
		} catch (IOException e) {
			e.printStackTrace();
		}

		return new String(buffer);
	}

	public static int genShaderProgram(String vertexShaderPath,
			String fragmentShaderPath) {
		String vertexShaderCode = getFileContent(vertexShaderPath);
		String fragmentShaderCode = getFileContent(fragmentShaderPath);
		int vertexShader = GLES20.glCreateShader(GLES20.GL_VERTEX_SHADER);
		int fragmentShader = GLES20.glCreateShader(GLES20.GL_FRAGMENT_SHADER);

		GLES20.glShaderSource(vertexShader, vertexShaderCode);
		GLES20.glCompileShader(vertexShader);
		checkCompile(vertexShader, "vertexShader");

		GLES20.glShaderSource(fragmentShader, fragmentShaderCode);
		GLES20.glCompileShader(fragmentShader);
		checkCompile(fragmentShader, "fragmentShader");

		int program = GLES20.glCreateProgram();
		GLES20.glAttachShader(program, vertexShader);
		GLES20.glAttachShader(program, fragmentShader);
		GLES20.glLinkProgram(program);
		checkLink(program, "glLinkProgram");

		return program;
	}

	private static void checkCompile(int shader, String info) {
		int[] status = new int[1];
		GLES20.glGetShaderiv(shader, GLES20.GL_COMPILE_STATUS, status, 0);
		if (status[0] == 0) {
			int[] length = new int[1];
			GLES20.glGetShaderiv(shader, GLES20.GL_INFO_LOG_LENGTH, length, 0);
			if (length[0] > 0) {
				Log.e(LOG_TAG, info + " " + GLES20.glGetShaderInfoLog(shader));
				throw new RuntimeException("gl error: " + info);
			}
		}
	}

	private static void checkLink(int program, String info) {
		int[] status = new int[1];
		GLES20.glGetProgramiv(program, GLES20.GL_LINK_STATUS, status, 0);
		if (status[0] == 0) {
			int[] length = new int[1];
			GLES20.glGetProgramiv(program, GLES20.GL_INFO_LOG_LENGTH, length, 0);
			if (length[0] > 0) {
				Log.e(LOG_TAG, info + " " + GLES20.glGetProgramInfoLog(program));
				throw new RuntimeException("gl error" + info);
			}
		}
	}

	public static int loadTexture(int textureRawID) {
		InputStream is = mContext.getResources().openRawResource(textureRawID);
		Bitmap bitmap = BitmapFactory.decodeStream(is);

		byte[] buffer = new byte[bitmap.getWidth() * bitmap.getHeight() * 3];

		for (int y = 0; y < bitmap.getHeight(); y++) {
			for (int x = 0; x < bitmap.getWidth(); x++) {
				int pixel = bitmap.getPixel(x, y);
				buffer[(y * bitmap.getWidth() + x) * 3 + 0] = (byte) ((pixel >> 16) & 0xFF);
				buffer[(y * bitmap.getWidth() + x) * 3 + 1] = (byte) ((pixel >> 8) & 0xFF);
				buffer[(y * bitmap.getWidth() + x) * 3 + 2] = (byte) ((pixel >> 0) & 0xFF);
			}
		}

		ByteBuffer bb = ByteBuffer.allocateDirect(buffer.length);
		bb.order(ByteOrder.nativeOrder()).put(buffer).position(0);

		int[] textureID = new int[1];
		GLES20.glGenTextures(1, textureID, 0);
		GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, textureID[0]);
		GLES20.glTexImage2D(GLES20.GL_TEXTURE_2D, 0, GLES20.GL_RGB,
				bitmap.getWidth(), bitmap.getHeight(), 0, GLES20.GL_RGB,
				GLES20.GL_UNSIGNED_BYTE, bb);
		GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_MIN_FILTER, GLES20.GL_LINEAR);
		GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_MAG_FILTER, GLES20.GL_LINEAR);
		GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_WRAP_S, GLES20.GL_REPEAT);
		GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_MAG_FILTER, GLES20.GL_REPEAT);
		
		return textureID[0];
	}

	public void move(float x, float y) {
		mCuboid.move(x, y);
	}

	private float[] mProjectionMatrix = new float[16];
	private float[] mViewMatrix = new float[16];

	private Cuboid mCuboid;
	private Multitexture mMultitexture;
	public static AssetManager assMnger;
	private static Context mContext;
	private static short mAngle = 0; // degree, not radian
	public static int w;
	public static int h;
	private final static String LOG_TAG = "GL_ERROR";
}
