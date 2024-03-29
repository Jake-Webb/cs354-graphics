
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
  vec3 theNormal = 2.0 * texture2D(normalMap, vec2(normalMapTexCoord.x * 6.0, normalMapTexCoord.y * -2.0)).rgb - 1.0;
  vec3 lightDir = normalize(lightDirection);
  float diff = 0.0;
  theNormal = normalize(theNormal);
  if(lightDir.z >= 0.0)
      diff = max(dot(theNormal, lightDir), 0.0);
  gl_FragColor = LMa + diff * LMd;
}
