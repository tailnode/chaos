package com.example.glessample;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.CharBuffer;
import java.nio.FloatBuffer;

import android.annotation.SuppressLint;
import android.opengl.GLES20;
import android.opengl.Matrix;

public class Cuboid {
	
	public Cuboid(float length) {
		this(length, length, length);
	}
	
	public Cuboid(float length, float width, float height) {

		// vertex position normal color
		float cuboidCoords[] = {
            // left face
			-length/2, -height/2,  width/2, -1,  0,  0, 1, 0, 0, 1, //  0 left  front bottom
			-length/2,  height/2,  width/2, -1,  0,  0, 1, 0, 0, 1, //  1 left  front top
			-length/2, -height/2, -width/2, -1,  0,  0, 1, 0, 0, 1, //  2 left  back  bottom
			-length/2,  height/2, -width/2, -1,  0,  0, 1, 0, 0, 1, //  3 left  back  top
            // right face                              
			 length/2, -height/2,  width/2,  1,  0,  0, 0, 1, 0, 1, //  4 right front bottom
			 length/2,  height/2,  width/2,  1,  0,  0, 0, 1, 0, 1, //  5 right front top
			 length/2, -height/2, -width/2,  1,  0,  0, 0, 1, 0, 1, //  6 right back  bottom
			 length/2,  height/2, -width/2,  1,  0,  0, 0, 1, 0, 1, //  7 right back  top
            // front face                              
			-length/2, -height/2,  width/2,  0,  0,  1, 0, 0, 1, 1, //  8 left  front bottom
			-length/2,  height/2,  width/2,  0,  0,  1, 0, 0, 1, 1, //  9 left  front top
			 length/2, -height/2,  width/2,  0,  0,  1, 0, 0, 1, 1, // 10 right front bottom
			 length/2,  height/2,  width/2,  0,  0,  1, 0, 0, 1, 1, // 11 right front top
	        // back face                               
			-length/2, -height/2, -width/2,  0,  0, -1, 1, 1, 0, 1, // 12 left  back  bottom
			-length/2,  height/2, -width/2,  0,  0, -1, 1, 1, 0, 1, // 13 left  back  top
			 length/2, -height/2, -width/2,  0,  0, -1, 1, 1, 0, 1, // 14 right back  bottom
			 length/2,  height/2, -width/2,  0,  0, -1, 1, 1, 0, 1, // 15 right back  top
	        // top face                                
			-length/2,  height/2,  width/2,  0,  1,  0, 1, 0, 1, 1, // 16 left  front top
			-length/2,  height/2, -width/2,  0,  1,  0, 1, 0, 1, 1, // 17 left  back  top
			 length/2,  height/2,  width/2,  0,  1,  0, 1, 0, 1, 1, // 18 right front top
			 length/2,  height/2, -width/2,  0,  1,  0, 1, 0, 1, 1, // 19 right back  top
	        // bottom face                             
			-length/2, -height/2,  width/2,  0, -1,  0, 0, 1, 1, 1, // 20 left  front bottom
			-length/2, -height/2, -width/2,  0, -1,  0, 0, 1, 1, 1, // 21 left  back  bottom
			 length/2, -height/2,  width/2,  0, -1,  0, 0, 1, 1, 1, // 22 right front bottom
			 length/2, -height/2, -width/2,  0, -1,  0, 0, 1, 1, 1, // 23 right back  bottom
		};
		mVertexStride = cuboidCoords.length / 24 * 4;
		
		drawOrder = new char[] {
            0, 1, 2, 2, 1, 3, // left face
            5, 4, 6, 6, 7, 5, // right face
            9, 8, 10, 10, 11, 9, // front face
            12, 13, 14, 14, 13, 15, // back face
            17, 16, 18, 17, 18, 19, // top face
            20, 21, 22, 22, 21, 23 // bottom face
		};
		
		ByteBuffer vertexbb = ByteBuffer.allocateDirect(cuboidCoords.length * 4);
		vertexbb.order(ByteOrder.nativeOrder());
		vertexBuffer = vertexbb.asFloatBuffer();
		vertexBuffer.put(cuboidCoords).position(0);
		
		ByteBuffer orderbb = ByteBuffer.allocateDirect(drawOrder.length * 2);
		orderbb.order(ByteOrder.nativeOrder());
		drawListBuffer = orderbb.asCharBuffer();
		drawListBuffer.put(drawOrder).position(0);
		
		mTranslateMatrix = new float[16];
		Matrix.setIdentityM(mTranslateMatrix, 0);
		
		int vbo[] = new int[2];
		GLES20.glGenBuffers(2, vbo, 0);
		
		GLES20.glBindBuffer(GLES20.GL_ARRAY_BUFFER, vbo[0]);
		GLES20.glBufferData(GLES20.GL_ARRAY_BUFFER, cuboidCoords.length * 4, vertexbb, GLES20.GL_STATIC_DRAW);

		GLES20.glBindBuffer(GLES20.GL_ELEMENT_ARRAY_BUFFER, vbo[1]);
		GLES20.glBufferData(GLES20.GL_ELEMENT_ARRAY_BUFFER, drawOrder.length * 2, orderbb, GLES20.GL_STATIC_DRAW);
		
		mPrograms = new int[SHADER_NUM];
		mPrograms[SHADER_SIMPLE] = MyGLRenderer.genShaderProgram("vertex.vp", "fragment.fp");
		mPrograms[SHADER_LIGHT] = MyGLRenderer.genShaderProgram("diffuseLightShader.vp", "diffuseLightShader.fp");
	    
	    selectProgram(SHADER_LIGHT);
		
		vertexbb.clear();
		orderbb.clear();
	}
  
	private FloatBuffer vertexBuffer;
    private CharBuffer drawListBuffer;
    private char[] drawOrder;
    
    private int mPositionHandle;
    private int mNormalHandle;
    private int mColorHandle;
    private int mLightPosHandle;
    private int mMVPMatrixHandle;
    private int mMvMatrixHandle;
    private int mProgramID;
    
    private float[] mTranslateMatrix;
    private int mVertexStride;
    private static final int SHADER_SIMPLE = 0;
    private static final int SHADER_LIGHT = 1;
    private static final int SHADER_NUM = 2;
    private int[] mPrograms;
    
    private void selectProgram(int program) {
    	mProgramID = program;
		GLES20.glUseProgram(mPrograms[program]);
    }
    
    public void draw(float[] rotateMatrix, float[] viewMatrix, float[] projectionMatrix) {
    	switch (mProgramID) {
    	case SHADER_LIGHT:
    		drawWithLight(rotateMatrix, viewMatrix, projectionMatrix);
    		break;
    	case SHADER_SIMPLE:
    		drawSimple(rotateMatrix, viewMatrix, projectionMatrix);
    		break;
    	}
    }
    
	@SuppressLint("NewApi")
	private void drawSimple(float[] rotateMatrix, float[] viewMatrix,
			float[] projectionMatrix) {
		int program = mPrograms[mProgramID];
		mPositionHandle = GLES20.glGetAttribLocation(program, "vPosition");
		mColorHandle = GLES20.glGetAttribLocation(program, "vVertexColor");

		GLES20.glEnableVertexAttribArray(mPositionHandle);
		GLES20.glEnableVertexAttribArray(mColorHandle);

		GLES20.glVertexAttribPointer(mPositionHandle, 3, GLES20.GL_FLOAT,
				false, mVertexStride, 0);

		GLES20.glVertexAttribPointer(mColorHandle, 4, GLES20.GL_FLOAT, false,
				mVertexStride, 24);

		mMVPMatrixHandle = GLES20.glGetUniformLocation(program, "uMVPMatrix");

		float[] modleMatrix = new float[16];
		float[] mvMatrix = new float[16];
		float[] mvpMatrix = new float[16];
		Matrix.multiplyMM(modleMatrix, 0, mTranslateMatrix, 0, rotateMatrix, 0);
		Matrix.multiplyMM(mvMatrix, 0, viewMatrix, 0, modleMatrix, 0);
		Matrix.multiplyMM(mvpMatrix, 0, projectionMatrix, 0, mvMatrix, 0);
		GLES20.glUniformMatrix4fv(mMVPMatrixHandle, 1, false, mvpMatrix, 0);

		GLES20.glDrawElements(GLES20.GL_TRIANGLES, drawOrder.length,
				GLES20.GL_UNSIGNED_SHORT, 0);

		GLES20.glDisableVertexAttribArray(mPositionHandle);
		GLES20.glDisableVertexAttribArray(mColorHandle);
	}
     
	@SuppressLint("NewApi")
	private void drawWithLight(float[] rotateMatrix, float[] viewMatrix,
			float[] projectionMatrix) {
		int program = mPrograms[mProgramID];
		mPositionHandle = GLES20.glGetAttribLocation(program, "vertexPos");
		mNormalHandle = GLES20.glGetAttribLocation(program, "vertexNormal");
		mColorHandle = GLES20.glGetAttribLocation(program, "vertexColor");

		mLightPosHandle = GLES20.glGetUniformLocation(program, "lightPos");
		mMVPMatrixHandle = GLES20.glGetUniformLocation(program, "mvpMatrix");
		mMvMatrixHandle = GLES20.glGetUniformLocation(program, "mvMatrix");

		GLES20.glEnableVertexAttribArray(mPositionHandle);
		GLES20.glEnableVertexAttribArray(mNormalHandle);
		GLES20.glEnableVertexAttribArray(mColorHandle);

		GLES20.glVertexAttribPointer(mPositionHandle, 3, GLES20.GL_FLOAT,
				false, mVertexStride, 0);
		GLES20.glVertexAttribPointer(mNormalHandle, 3, GLES20.GL_FLOAT, false,
				mVertexStride, 12);
		GLES20.glVertexAttribPointer(mColorHandle, 4, GLES20.GL_FLOAT, false,
				mVertexStride, 24);

		float[] modleMatrix = new float[16];
		float[] mvMatrix = new float[16];
		float[] mvpMatrix = new float[16];
		Matrix.multiplyMM(modleMatrix, 0, mTranslateMatrix, 0, rotateMatrix, 0);
		Matrix.multiplyMM(mvMatrix, 0, viewMatrix, 0, modleMatrix, 0);
		Matrix.multiplyMM(mvpMatrix, 0, projectionMatrix, 0, mvMatrix, 0);

		float[] lightPosition = { 10, 10, 10 };
		GLES20.glUniformMatrix4fv(mLightPosHandle, 1, false, lightPosition, 0);
		GLES20.glUniformMatrix4fv(mMVPMatrixHandle, 1, false, mvpMatrix, 0);
		GLES20.glUniformMatrix4fv(mMvMatrixHandle, 1, false, mvMatrix, 0);

		GLES20.glDrawElements(GLES20.GL_TRIANGLES, drawOrder.length,
				GLES20.GL_UNSIGNED_SHORT, 0);

		GLES20.glDisableVertexAttribArray(mPositionHandle);
		GLES20.glDisableVertexAttribArray(mNormalHandle);
		GLES20.glDisableVertexAttribArray(mColorHandle);
	}
    
    void move(float x, float y) {
        if (x < 0) x = 0;
        if (y < 0) y = 0;
        if (x > MyGLRenderer.w) x = MyGLRenderer.w;
        if (y > MyGLRenderer.h) y = MyGLRenderer.h;
        Matrix.setIdentityM(mTranslateMatrix, 0);
        Matrix.translateM(mTranslateMatrix, 0, x * 2/ MyGLRenderer.w - 1, 1 - y * 2 / MyGLRenderer.h, 0f);
    }
}
