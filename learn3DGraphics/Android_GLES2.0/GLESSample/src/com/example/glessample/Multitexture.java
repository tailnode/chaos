package com.example.glessample;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;
import java.nio.ShortBuffer;

import android.opengl.GLES20;

public class Multitexture {

	public Multitexture() {
		float square[] = {
				-0.5f,  0.5f, 0, 0, 1, // left top
				-0.5f, -0.5f, 0, 0, 0, // left bottom
				 0.5f, -0.5f, 0, 1, 0, // right bottom
				 0.5f,  0.5f, 0, 1, 1, // right top
		};

		short indices[] = {0, 1, 2, 0, 2, 3};

		mVertexs = ByteBuffer.allocateDirect(square.length * 4)
				.order(ByteOrder.nativeOrder()).asFloatBuffer();
		mVertexs.put(square).position(0);
		mIndices = ByteBuffer.allocateDirect(indices.length * 2)
				.order(ByteOrder.nativeOrder()).asShortBuffer();
		mIndices.put(indices).position(0);
		
		mProgram = MyGLRenderer.genShaderProgram("multitexture.vp", "multitexture.fp");
		vertexPositionHandle = GLES20.glGetAttribLocation(mProgram, "vertexPosition");
		textureCoordHandle = GLES20.glGetAttribLocation(mProgram, "textureCoord");
		baseTextureHandle = GLES20.glGetUniformLocation(mProgram, "baseTexture");
		lightTextureHandle = GLES20.glGetUniformLocation(mProgram, "lightTexture");
		GLES20.glUseProgram(mProgram);
		
		baseTexture = MyGLRenderer.loadTexture(R.raw.basemap);
		lightTexture = MyGLRenderer.loadTexture(R.raw.lightmap);
	}
	
	
	public void draw() {
		GLES20.glEnableVertexAttribArray(textureCoordHandle);
		GLES20.glEnableVertexAttribArray(vertexPositionHandle);
		
		mVertexs.position(0);
		GLES20.glVertexAttribPointer(vertexPositionHandle, 3, GLES20.GL_FLOAT, false, 5 * 4, mVertexs);
		mVertexs.position(3);
		GLES20.glVertexAttribPointer(textureCoordHandle, 2, GLES20.GL_FLOAT, false, 5 * 4, mVertexs);		
		
		GLES20.glActiveTexture(GLES20.GL_TEXTURE0);
		GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, baseTexture);
		GLES20.glUniform1i(baseTextureHandle, 0);
		
		GLES20.glActiveTexture(GLES20.GL_TEXTURE1);
		GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, lightTexture);
		GLES20.glUniform1i(lightTextureHandle, 1);
		
		GLES20.glDrawElements(GLES20.GL_TRIANGLES, 6, GLES20.GL_UNSIGNED_SHORT, mIndices);

		GLES20.glDisableVertexAttribArray(textureCoordHandle);
		GLES20.glDisableVertexAttribArray(vertexPositionHandle);
	}

	private FloatBuffer mVertexs;
	private ShortBuffer mIndices;
	private int mProgram;
	private int baseTexture;
	private int lightTexture;
	private int vertexPositionHandle;
	private int textureCoordHandle;
	private int baseTextureHandle;
	private int lightTextureHandle;
}
