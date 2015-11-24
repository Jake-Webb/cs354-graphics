
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
  vec3 eyeDir = normalize(eyeDirection);
  vec3 theNormal = 2.0 * texture2D(normalMap, vec2(normalMapTexCoord.x * 6.0, normalMapTexCoord.y * -2.0)).rgb - 1.0;
  theNormal = normalize(theNormal);
  vec3 reflectVector = M*reflect(eyeDir, theNormal);
  vec3 lightDir = normalize(lightDirection);
  float diff = 0.0;
  vec3 halfNorm = normalize(halfAngle);
  float spec = max(dot(vec3(0,0,1), halfNorm), 0);
  reflectVector = normalize(objectToWorld * reflectVector);
  if(lightDir.z >= 0.0){
    diff = max( dot(theNormal, lightDir), 0.0);}
  gl_FragColor = 0.5 * (LMa + diff * LMd) + 0.5 * pow(spec, shininess) * LMs + 0.6 * textureCube(envmap, -1.0 * reflectVector);
}
