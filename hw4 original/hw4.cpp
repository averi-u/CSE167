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

vec3 Triangle::getNormal() {
	return glm::normalize(glm::cross(vec3(B - A), vec3(C - A)));
}


vec3 Triangle::getNormal(vec3 P){

	return this->normal;

}


Sphere::~Sphere(){

}

Triangle::~Triangle(){

}