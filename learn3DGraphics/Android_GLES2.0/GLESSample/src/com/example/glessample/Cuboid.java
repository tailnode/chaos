package com.example.glessample;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.CharBuffer;
import java.nio.FloatBuffer;

import android.opengl.GLES20;
import android.opengl.Matrix;

public class Cuboid {
	
	public Cuboid(float length) {
		this(length, length, length);
	}
	
	public Cuboid(float length, float width, float height) {

		float cuboidCoords[] = {
			-length/2,  width/2, -height/2, // 0 left front bottom
			-length/2,  width/2,  height/2, // 1 left front top
			-length/2, -width/2, -height/2, // 2 left back bottom
			-length/2, -width/2,  height/2, // 3 left back top
			 length/2,  width/2, -height/2, // 4 right front bottom
			 length/2,  width/2,  height/2, // 5 right front top
			 length/2, -width/2, -height/2, // 6 right back bottom
			 length/2, -width/2,  height/2, // 7 right back top
		};
		
		drawOrder = new char[] {
		    1, 0, 4, 5, 1,
		    3, 2, 0, 4, 
		    6, 7, 5, 7,
		    3, 2, 6
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
	}
  
	private FloatBuffer vertexBuffer;
    private CharBuffer drawListBuffer;
    private char[] drawOrder;
    private int mPositionHandle;
    private int mColorHandle;
    private int mMVPMatrixHandle;
    private float[] mTranslateMatrix;
    private int mProgram = MyGLRenderer.genShaderProgram("vertex.vp", "fragment.fp");
    
     public void draw(float[] rotateMatrix, float[] viewMatrix, float[] projectionMatrix) {
        GLES20.glUseProgram(mProgram);
        MyGLRenderer.checkGlError("glUseProgram");

        mPositionHandle = GLES20.glGetAttribLocation(mProgram, "vPosition");

        GLES20.glEnableVertexAttribArray(mPositionHandle);

        GLES20.glVertexAttribPointer(
                mPositionHandle, 3,
                GLES20.GL_FLOAT, false,
                12, vertexBuffer);

        mColorHandle = GLES20.glGetUniformLocation(mProgram, "vColor");

        float color[] = { 0.2f, 0.709803922f, 0.898039216f, 1.0f };
        GLES20.glUniform4fv(mColorHandle, 1, color, 0);

        mMVPMatrixHandle = GLES20.glGetUniformLocation(mProgram, "uMVPMatrix");

        float[] modleMatrix = new float[16];
        float[] mvMatrix = new float[16];
        float[] mvpMatrix = new float[16];
        Matrix.multiplyMM(modleMatrix, 0, mTranslateMatrix, 0, rotateMatrix, 0);
        Matrix.multiplyMM(mvMatrix, 0, viewMatrix, 0, modleMatrix, 0);
        Matrix.multiplyMM(mvpMatrix, 0, projectionMatrix, 0, mvMatrix, 0);
        GLES20.glUniformMatrix4fv(mMVPMatrixHandle, 1, false, mvpMatrix, 0);

        GLES20.glDrawElements(
                GLES20.GL_LINE_STRIP, drawOrder.length,
                GLES20.GL_UNSIGNED_SHORT, drawListBuffer);

        GLES20.glDisableVertexAttribArray(mPositionHandle);
    }   
    void move(float x, float y) {
        if (x < 0) x = 0;
        if (y < 0) y = 0;
        if (x > MyGLRenderer.w) x = MyGLRenderer.w;
        if (y > MyGLRenderer.h) y = MyGLRenderer.h;
        Matrix.setIdentityM(mTranslateMatrix, 0);
        Matrix.translateM(mTranslateMatrix, 0, 1 - x * 2/ MyGLRenderer.w, 1 - y * 2 / MyGLRenderer.h, 0f);
//        Matrix.translateM(mTranslateMatrix, 0, -1, 1, 0f);
    }
}
