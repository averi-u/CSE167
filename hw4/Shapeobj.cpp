#include "Transform.h"
#include "Shapeobj.hpp"

Sphere::Sphere(vec3 center, float radius){

	this->center = center;
	this->radius = radius;
	this->type = "sphere";
}

Triangle::Triangle(vec4 A, vec4 B, vec4 C){
	this->A = A;
	this->B = B;
	this->C = C;
	this->type = "triangle";
}

vec3 Sphere::getNormal(vec3 P) {
    vec4 Papp = glm::inverse(transform) * vec4(P, 1);
    vec3 P3app = vec3(Papp) / Papp[3];
    vec3 normal = glm::normalize(P3app - center);
    vec4 n4 = glm::transpose(glm::inverse(transform)) * vec4(normal, 0);
    return glm::normalize(vec3(n4));
}

vec3 Triangle::getNormal(vec3 P){
	return this->normal;
}


vec4 getTransVertex(vec4 v, mat4 m){
	vec4 newVertex =  m * v;
	float x = newVertex.x/newVertex.w;
	float y = newVertex.y /newVertex.w;
	float z = newVertex.z/ newVertex.w;
	vec3 res = vec3(x,y,z);
	return newVertex;
}
void Triangle::applyTrans(){
	//apply the transformation matrix stored in the transform member variable of the class to each of the triangle's vertices 
	this->A = getTransVertex(this->A,this->transform );
	this->B = getTransVertex(this->B,this->transform );
	this->C = getTransVertex(this->C,this->transform );

	vec3 Ap = vec3(A[0],A[1],A[2]);
	vec3 Bp = vec3(B[0],B[1],B[2]);
	vec3 Cp = vec3(C[0],C[1],C[2]);

	this->normal = glm::normalize(glm::cross((Bp - Ap),(Cp - Ap)));
	
}




Sphere::~Sphere(){

}

Triangle::~Triangle(){

}

