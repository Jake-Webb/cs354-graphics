
attribute vec2 parametric;

uniform vec3 lightPosition;  // Object-space
uniform vec3 eyePosition;    // Object-space
uniform vec2 torusInfo;

varying vec2 normalMapTexCoord;

varying vec3 lightDirection;
varying vec3 halfAngle;
varying vec3 eyeDirection;
varying vec3 c0, c1, c2;

void main()
{
  normalMapTexCoord = vec2(parametric.x, parametric.y);
  vec2 mappedUV = radians(parametric * 360.0);
  float cosU = cos(mappedUV.x);
  float cosV = cos(mappedUV.y);
  float sinU = sin(mappedUV.x);
  float sinV = sin(mappedUV.y);
  vec3 posVec = vec3(torusInfo.x * cosU + torusInfo.y * cosU * cosV, torusInfo.x * sinU + torusInfo.y * cosV * sinU, torusInfo.y * sinV);
  gl_Position = gl_ModelViewProjectionMatrix * vec4(posVec.x, posVec.y, posVec.z, 1);
  vec3 tang = vec3(-1.0 * (torusInfo.x + torusInfo.y * cosV) * sinU, (torusInfo.x + torusInfo.y * cosV) * cosU, 0.0);
  vec3 grad = vec3(-1.0 * torusInfo.y * sinV * cosU, -1.0 * torusInfo.y * sinV * sinU, torusInfo.y * cosV);
  tang = normalize(tang);
  grad = normalize(grad);
  vec3 normal = cross(tang, grad);
  vec3 binormal = cross(normal, tang);
  mat3 M = mat3(tang, binormal, normal);
  vec3 vert3 = M * posVec;
  eyeDirection = transpose(M) * (eyePosition - vert3);
  lightDirection = transpose(M) * (lightPosition - vert3);
  halfAngle = (eyeDirection + lightDirection) / 2.0;
  c0 = tang;
  c1 = binormal;
  c2 = normal;
}

