
uniform vec4 LMa; // Light-Material ambient
uniform vec4 LMd; // Light-Material diffuse
uniform vec4 LMs; // Light-Material specular
uniform float shininess;

uniform sampler2D normalMap;
uniform sampler2D decal;
uniform sampler2D heightField;
uniform samplerCube envmap;

uniform mat3 objectToWorld;

varying vec2 normalMapTexCoord;
varying vec3 lightDirection;
varying vec3 eyeDirection;
varying vec3 halfAngle;
varying vec3 c0, c1, c2;

void main()
{
  mat3 M = mat3(c0, c1, c2);
    vec3 eyeDirection_normalized = normalize(eyeDirection);
    vec3 bNorm = 2.0 * texture2D(normalMap,vec2(normalMapTexCoord.x*6.0, normalMapTexCoord.y*-2.0)).rgb - 1.0;
    bNorm = normalize(bNorm);
    vec3 reflect_vector = M*reflect(eyeDirection_normalized,bNorm);
    reflect_vector = normalize(objectToWorld*reflect_vector);

    vec3 lightDirectionNorm = normalize(lightDirection);
    float diffuse = 0.0;
    if ( lightDirectionNorm.z >= 0.0){
        diffuse = max( dot(bNorm,lightDirectionNorm), 0.0);}
    vec3 halfNorm = normalize(halfAngle);
    float specular = max(dot(vec3(0,0,1),halfNorm),0);

    gl_FragColor = 0.5*(LMa + diffuse*LMd) + 0.5*pow(specular,shininess)*LMs + 0.6*textureCube(envmap, -1.0*reflect_vector);
}
