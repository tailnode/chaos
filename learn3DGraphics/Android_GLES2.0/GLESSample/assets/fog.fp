varying vec4 varyingBaseColor;
varying float varyingVertexDist;

uniform float maxDist;
uniform float minDist;
uniform vec4 fogColor;

void main() {
    float fogFactor = (maxDist - varyingVertexDist) / (maxDist - minDist);
    fogFactor = clamp(fogFactor, 0.0, 1.0);
    
    vec4 fogColor1 = fogColor * fogFactor;
    
    gl_FragColor = varyingBaseColor * fogColor1 + fogColor1 * (1.0 - fogFactor); 
}