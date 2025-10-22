#include "Transform.h"
#include "hw4.hpp"

Sphere::Sphere(vec3 ct, float rd){

	this->ct = ct;
	this->rd = rd;
	this->type = "sphere";
}


vec3 Sphere::getNormal(vec3 P){
	vec3 diff = glm::normalize(P - this->ct);
	vec3 normal =  glm::normalize(diff);

	normal = glm::transpose(glm::inverse(this->transform)) * vec4(normal, 0);
	return glm::normalize(normal);
}

Triangle::Triangle(vec4 A, vec4 B, vec4 C){
	this->A = A;
	this->B = B;
	this->C = C;
	this->type = "triangle";
}

vec4 getTransformedVertex(vec4 v, mat4 m){
	vec4 aux =  m* v;
	float x = aux.x / aux.w;

	float y = aux.y / aux.w; float z = aux.z / aux.w;
	vec3 res = vec3(x,y,z);
	return aux;
	
}
void Triangle::applyTransform(){

	this->A = getTransformedVertex(this->A,this->transform);

	this->B = getTransformedVertex(this->B,this->transform);

	this->C = getTransformedVertex(this->C,this->transform);

	this->setNormal();
}

void Triangle::setNormal(){
	this->normal = getNormal();

}

vec3 Triangle::getNormal(){
	vec3 Ap = vec3(A[0],A[1],A[2]);
	vec3 Bp = vec3(B[0],B[1],B[2]);

	vec3 Cp = vec3(C[0],C[1],C[2]);
	vec3 normal = glm::cross((Bp - Ap),(Cp - Ap));
	normal = glm::normalize(normal);

	return normal;
	
	

}

vec3 Triangle::getNormal(vec3 P){

	return this->normal;

}


Sphere::~Sphere(){

}

Triangle::~Triangle(){

}