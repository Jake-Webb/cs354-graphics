// The main ray tracer.

#pragma warning (disable: 4786)

#include "RayTracer.h"
#include "scene/light.h"
#include "scene/material.h"
#include "scene/ray.h"

#include "parser/Tokenizer.h"
#include "parser/Parser.h"

#include "ui/TraceUI.h"
#include <cmath>
#include <algorithm>

extern TraceUI* traceUI;

#include <iostream>
#include <fstream>

using namespace std;

// Use this variable to decide if you want to print out
// debugging messages.  Gets set in the "trace single ray" mode
// in TraceGLWindow, for example.
bool debugMode = false;

// Trace a top-level ray through pixel(i,j), i.e. normalized window coordinates (x,y),
// through the projection plane, and out into the scene.  All we do is
// enter the main ray-tracing method, getting things started by plugging
// in an initial ray weight of (0.0,0.0,0.0) and an initial recursion depth of 0.

Vec3d RayTracer::trace(double x, double y)
{
  // Clear out the ray cache in the scene for debugging purposes,
  if (TraceUI::m_debug) scene->intersectCache.clear();
  ray r(Vec3d(0,0,0), Vec3d(0,0,0), ray::VISIBILITY);
  scene->getCamera().rayThrough(x,y,r);
  Vec3d ret = traceRay(r, traceUI->getDepth());
  ret.clamp();
  return ret;
}

Vec3d RayTracer::tracePixel(int i, int j)
{
	Vec3d col(0,0,0);

	if( ! sceneLoaded() ) return col;

	double x = double(i)/double(buffer_width);
	double y = double(j)/double(buffer_height);

	unsigned char *pixel = buffer + ( i + j * buffer_width ) * 3;

	col = trace(x, y);

	pixel[0] = (int)( 255.0 * col[0]);
	pixel[1] = (int)( 255.0 * col[1]);
	pixel[2] = (int)( 255.0 * col[2]);
	return col;
}


// Do recursive ray tracing!  You'll want to insert a lot of code here
// (or places called from here) to handle reflection, refraction, etc etc.
Vec3d RayTracer::traceRay(ray& r, int depth)
{
	isect i;
	Vec3d colorC;

	if(scene->intersect(r, i)) {
		// YOUR CODE HERE

		// An intersection occurred!  We've got work to do.  For now,
		// this code gets the material for the surface that was intersected,
		// and asks that material to provide a color for the ray.  

		// This is a great place to insert code for recursive ray tracing.
		// Instead of just returning the result of shade(), add some
		// more steps: add in the contributions from reflected and refracted
		// rays.
	  
	const Material& m = i.getMaterial();
	colorC = m.shade(scene, r, i);
    if(depth < 1)
        return colorC;
    Vec3d pointOnObject = r.at(i.t);

    Vec3d reflectedDirectionCi(0.0f,0.0f,0.0f);
    Vec3d reflectedDirectionSi(0.0f,0.0f,0.0f);
    Vec3d reflectedDir(0.0f,0.0f,0.0f);
    updateReflectionParams(r,i,reflectedDirectionCi,reflectedDirectionSi,reflectedDir);

    Vec3d refractedDirectionSt(0.0f,0.0f,0.0f);
    Vec3d refractedDirectionCt(0.0f,0.0f,0.0f);
    Vec3d refractedDir(0.0f,0.0f,0.0f);
    bool shootRefractedRay = updateRefractionParams(r,i,m,reflectedDirectionSi,refractedDirectionSt,refractedDirectionCt, refractedDir);

    ray reflectedRay(pointOnObject, reflectedDir, ray::REFLECTION);
    ray refractedRay(pointOnObject, refractedDir, ray::REFRACTION);

    Vec3d reflectionIntensity = m.kr(i);
    Vec3d refractionIntensity(0.0f,0.0f,0.0f);

    reflectionIntensity %=  traceRay(reflectedRay, depth - 1);
    if(shootRefractedRay){
        refractionIntensity = m.kt(i);
        refractionIntensity %= traceRay(refractedRay, depth - 1);}

    colorC += reflectionIntensity;
    colorC += refractionIntensity;
    return colorC;
	} else {
		// No intersection.  This ray travels to infinity, so we color
		// it according to the background color, which in this (simple) case
		// is just black.
		colorC = Vec3d(0.0, 0.0, 0.0);
	}
	return colorC;
}

void RayTracer::updateReflectionParams(const ray& r, const isect& i,Vec3d& reflectedDirectionCi, Vec3d& reflectedDirectionSi, Vec3d& reflectedDir){
    reflectedDirectionCi = (((-1)*r.getDirection())*i.N)*i.N;
    reflectedDirectionSi = reflectedDirectionCi + r.getDirection();
    reflectedDir = reflectedDirectionCi+reflectedDirectionSi;
    reflectedDir.normalize();
}

bool RayTracer::updateRefractionParams(const ray& r, const isect& i, const Material& m, const Vec3d& reflectedDirectionSi, Vec3d& refractedDirectionSt, Vec3d& refractedDirectionCt, Vec3d& refractedDir){
    if((m.kt(i)[0] <= 0.0f&&m.kt(i)[1]<=0.0f&&m.kt(i)[2]<=0.0f) || checkTotalInternal(r,i))
        return false;

    if(i.N * r.getDirection() < 0.0f){
        double ratioIndex = i.getMaterial().index(i);
        ratioIndex = 1.0f/(double)ratioIndex;
        refractedDirectionSt = ratioIndex * reflectedDirectionSi;
        double temp = refractedDirectionSt*refractedDirectionSt;
        if(temp>1)
            temp = 0;
        refractedDirectionCt = (-1.0f * i.N) * std::sqrt((1.0f - temp));
    } else if(i.N * r.getDirection() > 0.0f) {
        double ratioIndex = i.getMaterial().index(i);
        refractedDirectionSt = ratioIndex * reflectedDirectionSi;
        double temp = refractedDirectionSt*refractedDirectionSt;
        if(temp>1)
            temp = 0;
        refractedDirectionCt = i.N * std::sqrt((1.0f - temp));}

    refractedDir = refractedDirectionSt + refractedDirectionCt;
    refractedDir.normalize();
    return true;
}

bool RayTracer::checkTotalInternal(const ray &r, const isect &i){
    if(i.N * r.getDirection() > 0.0f){
        double incidentAngle = i.N * r.getDirection();
        double ratioIndex = i.getMaterial().index(i);
        if((1.0f - (ratioIndex*ratioIndex*(1.0f - incidentAngle * incidentAngle)))<0.0f)
            return true;}
    return false;
}

RayTracer::RayTracer()
	: scene(0), buffer(0), buffer_width(256), buffer_height(256), m_bBufferReady(false)
{}

RayTracer::~RayTracer()
{
	delete scene;
	delete [] buffer;
}

void RayTracer::getBuffer( unsigned char *&buf, int &w, int &h )
{
	buf = buffer;
	w = buffer_width;
	h = buffer_height;
}

double RayTracer::aspectRatio()
{
	return sceneLoaded() ? scene->getCamera().getAspectRatio() : 1;
}

bool RayTracer::loadScene( char* fn ) {
	ifstream ifs( fn );
	if( !ifs ) {
		string msg( "Error: couldn't read scene file " );
		msg.append( fn );
		traceUI->alert( msg );
		return false;
	}
	
	// Strip off filename, leaving only the path:
	string path( fn );
	if( path.find_last_of( "\\/" ) == string::npos ) path = ".";
	else path = path.substr(0, path.find_last_of( "\\/" ));

	// Call this with 'true' for debug output from the tokenizer
	Tokenizer tokenizer( ifs, false );
    Parser parser( tokenizer, path );
	try {
		delete scene;
		scene = 0;
		scene = parser.parseScene();
	} 
	catch( SyntaxErrorException& pe ) {
		traceUI->alert( pe.formattedMessage() );
		return false;
	}
	catch( ParserException& pe ) {
		string msg( "Parser: fatal exception " );
		msg.append( pe.message() );
		traceUI->alert( msg );
		return false;
	}
	catch( TextureMapException e ) {
		string msg( "Texture mapping exception: " );
		msg.append( e.message() );
		traceUI->alert( msg );
		return false;
	}

	if( !sceneLoaded() ) return false;

	return true;
}

void RayTracer::traceSetup(int w, int h)
{
	if (buffer_width != w || buffer_height != h)
	{
		buffer_width = w;
		buffer_height = h;
		bufferSize = buffer_width * buffer_height * 3;
		delete[] buffer;
		buffer = new unsigned char[bufferSize];
	}
	memset(buffer, 0, w*h*3);
	m_bBufferReady = true;
}

