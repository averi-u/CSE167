#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <deque>
#include <stack>
#include <GL/glut.h>
#include <FreeImage.h>
#include <math.h>
#include <cmath>
#include <limits>

using namespace std; 

#define MAINPROGRAM 
#include "variables.h" 
#include "readfile.h"   
#include "Transform.h"

#define DEV_MODE 0
#define DEV_MODE_WIDTH 160
#define DEV_MODE_HEIGHT 120

#define DEFAULT_ATTENUATION 1,0,0
	
#define	 DEFAULT_AMBIENT .2,.2,.2
#define DEFAULT_MAX_DEPTH 5

typedef struct {
	bool isIntersected;
	float t;
	Base* obj;

	vec3 p_intersect;
	vec3 normal; vec3 direction;

} Intersec;

int imgWidth;
int imgHeight;
const float  eps=1e-3;
//const float  eps=1e-5;

void ckDev(){
	if (DEV_MODE){
		imgWidth = DEV_MODE_WIDTH;
		imgHeight = DEV_MODE_HEIGHT;
	}else{
		imgWidth = sceneWidth;

		imgHeight = sceneHeight;  }
}

void ckDf(){

	if (outputFileName){
		std::cout << "Filename=" << outputFileName  << "\n";
	} else {

		outputFileName = "rt.png";    }
}

//Generate rays fr Cam
vec3 pixRay(int i, int j){
	//given in the slides
    float fovyRad = fovy * pi / 180.0, aspectRatio = (float)imgWidth / imgHeight;

    vec3 w = glm::normalize(eyeinit - center), u = glm::normalize(glm::cross(upinit, w)), v = glm::cross(w, u);

    float alpha = glm::tan(fovyRad/2) * aspectRatio * ((float)(j+0.5 -imgWidth/2)/(imgWidth / 2));

    float beta  = glm::tan(fovyRad/2) * ((float)(imgHeight/2 -(i+0.5))/(imgHeight/2));

    return alpha * u + beta * v - w;
}


void transTri()
{
	for (int i=0; i<triangles.size(); i++){

		Triangle* t = &(triangles[i]);

		t->applyTrans();
	}

}

Intersec* intsTri(vec3 P0, vec3 direction, Triangle T) {
    Intersec* r = new Intersec();
    r->isIntersected = false;

    vec3 Ap = vec3(T.A[0] / T.A[3], T.A[1] / T.A[3], T.A[2] / T.A[3]);
    vec3 Bp = vec3(T.B[0] / T.B[3], T.B[1] / T.B[3], T.B[2] / T.B[3]);
    vec3 Cp = vec3(T.C[0] / T.C[3], T.C[1] / T.C[3], T.C[2] / T.C[3]);
    vec3 normal = T.getNormal(Ap);
    float dotProduct = glm::dot(direction, normal);

    if (abs(dotProduct) >= eps) {
        float t = (glm::dot(Ap, normal) - glm::dot(P0, normal)) / dotProduct;
        vec3 P = P0 + t * direction, R = P - Ap, Q1 = Bp - Ap, Q2 = Cp - Ap;
        float det = glm::dot(Q1, Q1) * glm::dot(Q2, Q2) - pow(glm::dot(Q1, Q2), 2);

        if (det != 0) {
            vec2 w = (1 / det) * mat2(glm::dot(Q2, Q2), -glm::dot(Q1, Q2), -glm::dot(Q1, Q2), glm::dot(Q1, Q1)) * vec2(glm::dot(R, Q1), glm::dot(R, Q2));
            if (w[0] >= 0 && w[1] >= 0 && (w[0] + w[1] <= 1)) {
                r->isIntersected = true;
                r->t = t;
                r->p_intersect = P0 + t * direction;
            }
        }
    }
    return r;
}


Intersec* intsSphere(vec3 P0, vec3 direction, Sphere s){
	Intersec* r = new Intersec();
	r->isIntersected = false;

	mat4 transform = glm::inverse(s.transform);

	vec4 P0h = vec4(P0[0],P0[1],P0[2],1);
	vec4 dH  = vec4(direction[0], direction[1], direction[2], 0);
	P0h = transform * P0h;
	dH = transform * dH;
	vec4 ch = vec4(s.center[0], s.center[1], s.center[2], 1);

	float a = glm::dot(dH,dH);
	float b = 2 * glm::dot(dH , (P0h - ch));
	float c = glm::dot((P0h - ch),(P0h - ch)) - pow(s.radius,2);
	float delta = pow(b,2) - 4 * a * c;
	if (abs(delta) < eps){
		float t = -b / (2 * a);
		r->t = t;
		r->isIntersected = true;
	}
	else if (delta > 0){
		float t1 = -b/(2*a) + sqrt(delta)/(2*a);
		float t2 = -b/(2*a) - sqrt(delta)/(2*a);
		float t = (t1 < t2 ? t1 : t2);
		r->t = t;
		r->isIntersected = true;
	}
	if (r->isIntersected){
		vec4 P0ht = s.transform * P0h;
		vec4 dHt = s.transform * dH;
		
		vec4 Pt = P0ht + (r->t) * dHt;
		r->p_intersect = vec3(Pt[0]/Pt[3], Pt[1]/Pt[3], Pt[2]/Pt[3]);
	}
	return r;
}

vec3 getSpecularDir(vec3 incident, vec3 normal){
    return glm::normalize(incident - 2 * glm::dot(normal, incident) * normal);
}

Intersec* intersecFind(vec3 P0, vec3 direction, bool setN, bool stopFir, float maxDist) {
    Intersec* resIntesec = NULL;
    Base* hitObject = NULL;
    float minDist = std::numeric_limits<float>::max(); 
    bool found = false;
    
    for (int i = 0; i < triangles.size(); i++) {
        Intersec* r = intsTri(P0, direction, triangles[i]);
        if (r->isIntersected && r->t > 0 && r->t < minDist && r->t < maxDist) {
            minDist = r->t;
            hitObject = &triangles[i];
            if (resIntesec) delete resIntesec;
            resIntesec = new Intersec;
            resIntesec->t = minDist;
            resIntesec->obj = hitObject;
			resIntesec->direction = direction;
            resIntesec->isIntersected = true;
            resIntesec->p_intersect = r->p_intersect;
            if (setN) resIntesec->normal = resIntesec->obj->getNormal(resIntesec->p_intersect);
            found = true;
            if (stopFir) break;
        }
        if (r) delete r;
    }
    
    if (!found || !stopFir) {
        for (int i = 0; i < spheres.size(); i++) {
            Intersec* r = intsSphere(P0, direction, spheres[i]);

            if (r->isIntersected && r->t > 0 && r->t < minDist && r->t < maxDist) {
                minDist = r->t;
                hitObject = &spheres[i];
                if (resIntesec) delete resIntesec;
                resIntesec = new Intersec;
                resIntesec->t = minDist;
                resIntesec->obj = hitObject;
				resIntesec->direction = direction;
                resIntesec->isIntersected = true;
                resIntesec->p_intersect = r->p_intersect;
                if (setN) resIntesec->normal = resIntesec->obj->getNormal(resIntesec->p_intersect);
				//modified
				// else resIntesec->normal = normal;
                found = true;
                if (stopFir) break;
            }
            if (r) delete r;
        }
    }
    
    return resIntesec;
}


BYTE* getRGB(vec3 color)
{
	BYTE* res = (BYTE*)malloc(3*sizeof(BYTE));

	res[0] = (BYTE) (color[2] * 255);
	res[1] = (BYTE) (color[1] * 255);
	res[2] = (BYTE) (color[0] * 255);
	return res;
}

float getAttenua(float d){

	return 1.0/(attenuation[0] + attenuation[1] * d + attenuation[2] * pow(d,2));
}

vec3 colorize(Intersec *r, int depth, vec3 pointOfView) {
    vec3 color = vec3(0);

    if (depth) {
        for (int i = 0; i < numused; i++) {
            vec3 direction, posLight;
            bool isDirectional = lightposn[4 * i + 3] == 0;
            posLight = isDirectional ? vec3(lightposn[4 * i], lightposn[4 * i + 1], lightposn[4 * i + 2]) : vec3(lightposn[4 * i] / lightposn[4 * i + 3], lightposn[4 * i + 1] / lightposn[4 * i + 3],
                                                                                                             lightposn[4 * i + 2] / lightposn[4 * i + 3]);

            direction = glm::normalize(isDirectional ? posLight : (posLight - r->p_intersect));
            vec3 offset = eps * direction;
            // vec3 d = r->p_intersect + (offset * 1.0001f);
			vec3 d = r->p_intersect + offset;
            float maxDist = glm::length(posLight - r->p_intersect);
            Intersec *isFree = intersecFind(d, direction, false, true, maxDist);

            if (!isFree || !isFree->isIntersected || isFree->t > maxDist) {
                vec3 I = vec3(lightcolor[4 * i], lightcolor[4 * i + 1], lightcolor[4 * i + 2]);
                float dot = glm::dot(r->normal, direction);
                vec3 diffuseTerm = (dot > 0 ? r->obj->diffuse * dot : vec3(0));

                vec3 H = glm::normalize(direction + glm::normalize(pointOfView - r->p_intersect));
                dot = glm::dot(r->normal, H);

                vec3 specularTerm = (dot > 0 ? r->obj->specular * pow(dot, r->obj->shininess) : vec3(0));
                float attenuation = (isDirectional ? 1.0 : getAttenua(maxDist));

                color += attenuation * I * (diffuseTerm + specularTerm);
            }
        }
        color += r->obj->ambient + r->obj->emission;
		// vec3 newDir=getSpecularDir(r->direction, r->normal);
        vec3 newDir = glm::normalize(glm::reflect(r->direction, r->normal));
		
		// std::cout << "newDir: (" << r->direction.x << ", " << r->direction.y << ", " << r->direction.z << ")" << std::endl;
        Intersec *newIntersection = intersecFind(r->p_intersect + eps* newDir, newDir, true, false, numeric_limits<float>::max());

        if (newIntersection) {
            color += r->obj->specular * colorize(newIntersection, depth - 1, r->p_intersect);

			//color += r->obj->specular * colorize(newIntersection, depth - 1, newDir);
			// std::cout << "yes, Intsect found" << endl;
			// std::cout << " Color: " << color.r << ", " << color.g << ", " << color.b << endl;
        }
		
    }

    color = glm::clamp(color, vec3(0), vec3(1));
    return color;
}


void rayTrace(BYTE* pixels){
	// #pragma omp parallel for private(j)
	#pragma omp parallel for collapse(2)
    for (int i=0; i<imgHeight; i++){
	// for (int i=imgHeight/4+10; i<imgHeight/3-10; i++){
		// if(i!=imgHeight-1) continue;
		std::cout <<  "Pr: " <<i<<"\n";

		for (int j=0; j<imgWidth; j++){
		// for (int j=imgHeight/4+10; j<imgWidth/3-10; j++){
			int base = (i * imgWidth + j)*3; //map 1D array to 2D coordinates
			// base: the total pixels before the current pixel (i, j)
			pixels[base] = 0;
			pixels[base+1] = 0;
			pixels[base+2] = 0;


			vec3 p1 = pixRay(i, j); //vectors of rays
			Intersec* r = intersecFind(eyeinit, p1, true, false, numeric_limits<float>::max());
			ß

			if (r && r->obj){
				vec3 color = colorize(r, maxDepth, eyeinit);
				BYTE* res = getRGB(color);
				pixels[base] = res[0];
				pixels[base+1] = res[1];
				pixels[base+2] = res[2];
				
				// printf("%f %f %f\n",res[0],res[1],res[2]);
				free(res);
			}
		}
        
    }
}

int main(int argc , char* argv[]){


	if (argc < 2){
		cerr << "Usage: raytrace inputfile\n";
	}
	else{
		attenuation = vec3(DEFAULT_ATTENUATION);
		ambient = vec3(DEFAULT_AMBIENT);
		maxDepth = DEFAULT_MAX_DEPTH;
		
		readfile(argv[1]);
		ckDev();
		ckDf();	

		transTri();
		BYTE pixels[3*imgWidth*imgHeight];
		rayTrace(pixels);
		
		FreeImage_Initialise();
		// std::cout<<"Image width is "<<imgWidth<<", Img height is "<<imgHeight<<std::endl;
		FIBITMAP *img = FreeImage_ConvertFromRawBits(pixels, imgWidth, imgHeight,imgWidth*3, 24, 0xFF0000,0x00FF00, 0x0000FF, true);
		FreeImage_Save(FIF_PNG, img, outputFileName, 0);
		FreeImage_DeInitialise();
	}
	return 0;
}
