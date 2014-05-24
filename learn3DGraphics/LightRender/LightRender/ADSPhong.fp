#version 330

uniform vec4 ambientColor;
uniform vec4 diffuseColor;
uniform vec4 specularColor;

in vec3 vertexNormalEye;
in vec3 lightDirEye;

out vec4 color;

void main()
{ 
   // ---------caculate diffuse color--------------

    float intensity = max(0.0, dot(normalize(lightDirEye), normalize(vertexNormalEye)));

    color.rgb = intensity * diffuseColor.rgb;
    color.a = diffuseColor.a;
    // ---------------------------------------------

    // ------------add ambientColor-----------------
    color += ambientColor;
    // ---------------------------------------------

    // ------------add specularColor----------------
    vec3 reflection = normalize(reflect(-normalize(lightDirEye), normalize(vertexNormalEye)));
    float spec = max(0.0, dot(normalize(vertexNormalEye), reflection));
    if (intensity != 0) {
        float fSpec = pow(spec, 128.0);
        color.rgb += vec3(fSpec, fSpec, fSpec); 
    }
    // ---------------------------------------------
}
