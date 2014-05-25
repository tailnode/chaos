#version 330

in vec2 varyingTexCoords;
uniform sampler2DRect textureImage;
out vec4 color;
void main()
{
    color = texture(textureImage, varyingTexCoords);
}
