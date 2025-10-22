// Transform.cpp: implementation of the Transform class.


#include "Transform.h"

// const float PI = 3.14159265 ;

//Please implement the following functions:

// Helper rotation function.  
mat3 Transform::rotate(const float degrees, const vec3& axis) {
	// YOUR CODE FOR HW1 HERE
    mat3 I(1.0);
	float rad = glm::radians(degrees);

    float x = axis.x;
    float y = axis.y;
    float z = axis.z;
    
    mat3 mt1 = cos(rad)*I;
    mat3 mt2 = mat3(x*x, x*y, x*z, x*y, y*y, y*z, x*z, y*z, z*z);
	mat3 mt3 = mat3(0.0, z, -y, -z, 0.0, x, y, -x, 0.0);

	mat3 rot = mt1 + (1-cos(rad)) * mt2 + sin(rad) * mt3;
	// You will change this return call
	return rot;
}

// Transforms the camera left around the "crystal ball" interface
void Transform::left(float degrees, vec3& eye, vec3& up) {
	// YOUR CODE FOR HW1 HERE
	// printf("Coordinates: %.2f, %.2f, %.2f; distance: %.2f\n", eye.x, eye.y, eye.z, sqrt(pow(eye.x, 2) + pow(eye.y, 2) + pow(eye.z, 2)));

	eye = rotate(degrees, up) * eye; 
}

// Transforms the camera up around the "crystal ball" interface
void Transform::up(float degrees, vec3& eye, vec3& up) {
	// YOUR CODE FOR HW1 HERE 
	// printf("Coordinates: %.2f, %.2f, %.2f; distance: %.2f\n", eye.x, eye.y, eye.z, sqrt(pow(eye.x, 2) + pow(eye.y, 2) + pow(eye.z, 2)));
	// vec3 eyecenterAxis = eye - center;
	vec3 axis = normalize(cross(eye, up));

	mat3 rotation = rotate(degrees, axis); 
	eye = rotation*eye; 
	up = normalize(rotation*up); 

}

// Your implementation of the glm::lookAt matrix
mat4 Transform::lookAt(vec3 eye, vec3 up) {
	// YOUR CODE FOR HW1 HERE
	vec3 center = vec3(0);
	vec3 a = eye - center;
	vec3 b = normalize(up); 
	vec3 c = cross(a, b);

	//eye to center - c, u
	//left and right - a, v
	//up - b, w

	vec3 w = normalize(a);
	vec3 u = normalize(cross(b, w));
	vec3 v = cross(w,u);
	vec3 e = eye; 

	mat4 final = mat4(u.x, v.x, w.x, 0,
						u.y, v.y, w.y, 0,
						u.z, v.z, w.z, 0,
						-1*u.x*e.x-u.y*e.y-u.z*e.z, -1*v.x*e.x - v.y*e.y - v.z*e.z, -1*w.x*e.x-w.y*e.y-w.z*e.z, 1);



	// You will change this return call
	return final;
}

Transform::Transform()	
{

}

Transform::~Transform()
{

}
