uniform sampler2D baseTexture;
uniform sampler2D lightTexture;
varying vec2 varyingTexCoord;

void main() {
    vec4 baseColor = texture2D(baseTexture, varyingTexCoord);
    vec4 lightColor = texture2D(lightTexture, varyingTexCoord);
    gl_FragColor = baseColor * (lightColor + 0.25);
}