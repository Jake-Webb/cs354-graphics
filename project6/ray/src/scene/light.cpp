#include <cmath>

#include "light.h"

using namespace std;

double DirectionalLight::distanceAttenuation(const Vec3d& P) const
{
  // distance to light is infinite, so f(di) goes to 0.  Return 1.
  return 1.0;
}


Vec3d DirectionalLight::shadowAttenuation(const ray& r, const Vec3d& p) const
{
  // YOUR CODE HERE:
  // You should implement shadow-handling code here.

  ray rayToLight(p,getDirection(p),ray::SHADOW);
    isect i;
    if(scene->intersect( rayToLight, i )) {
        Vec3d returnColor(0.0f,0.0f,0.0f);
        Vec3d kTransmit = i.getMaterial().kt(i);
        double currentPoint = i.t;
        double distBetween = 1.0f;
        isect internalI;
        Vec3d pointOnObject = rayToLight.at(currentPoint);
        ray temp = ray(pointOnObject,getDirection(pointOnObject),ray::SHADOW);
        if(scene->intersect(temp, internalI))
            distBetween = internalI.t;
        double attentuation = std::min(1.0,(double)1.0/(double)(0.2 + 0.2f*distBetween + 0.6f*distBetween*distBetween));
        if(kTransmit[0]>0||kTransmit[1]>0||kTransmit[2]>0) {
            returnColor = attentuation * getColor();
            returnColor%=kTransmit;
        }
        return returnColor;
    }
  return Vec3d(1,1,1);
}

Vec3d DirectionalLight::getColor() const
{
  return color;
}

Vec3d DirectionalLight::getDirection(const Vec3d& P) const
{
  // for directional light, direction doesn't depend on P
  return -orientation;
}

double PointLight::distanceAttenuation(const Vec3d& P) const
{

  // YOUR CODE HERE

  // You'll need to modify this method to attenuate the intensity 
  // of the light based on the distance between the source and the 
  // point P.  For now, we assume no attenuation and just return 1.0

  Vec3d tempVec = position - P;
  double d = tempVec.length();
  return std::min(1.0,(double)1.0/(double)(constantTerm + linearTerm*d + quadraticTerm*d*d));
}

Vec3d PointLight::getColor() const
{
  return color;
}

Vec3d PointLight::getDirection(const Vec3d& P) const
{
  Vec3d ret = position - P;
  ret.normalize();
  return ret;
}


Vec3d PointLight::shadowAttenuation(const ray& r, const Vec3d& p) const
{
  // YOUR CODE HERE:
  // You should implement shadow-handling code here.

  Vec3d tempVec = position - p;
  double t = tempVec.length();
  tempVec.normalize();
  ray rayToLight(p,tempVec,ray::SHADOW);
  isect i;
  if(scene->intersect( rayToLight, i )){
      Vec3d pointOnObject = rayToLight.at(i.t);
      if(t>i.t)
        return Vec3d(0,0,0);
  }
  return Vec3d(1,1,1);
}
