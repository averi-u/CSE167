// Transform.cpp: implementation of the Transform class.

// Note: when you construct a matrix using mat4() or mat3(), it will be COLUMN-MAJOR
// Keep this in mind in readfile.cpp and display.cpp
// See FAQ for more details or if you're having problems.

#include "Transform.h"
// using namespace glm;

// Helper rotation function.  Please implement this.  
mat3 Transform::rotate(const float degrees, const vec3& axis) 
{
  mat3 I(1.0);
	float rad = glm::radians(degrees);

    float x = axis.x;
    float y = axis.y;
    float z = axis.z;
    
  mat3 mt1 = cos(rad)*I;
  mat3 mt2 = mat3(x*x, x*y, x*z, x*y, y*y, y*z, x*z, y*z, z*z);
	mat3 mt3 = mat3(0.0, z, -y, -z, 0.0, x, y, -x, 0.0);

	mat3 ret = mt1 + (1-cos(rad)) * mt2 + sin(rad) * mt3;
	// You will change this return call
	return ret;
}

void Transform::left(float degrees, vec3& eye, vec3& up) 
{
  // YOUR CODE FOR HW2 HERE
  // Likely the same as in HW 1.  
  eye = rotate(degrees, up) * eye; 
}

void Transform::up(float degrees, vec3& eye, vec3& up) 
{
  // YOUR CODE FOR HW2 HERE 
  // Likely the same as in HW 1.  
  vec3 axis = normalize(cross(eye, up));

	mat3 rotation = rotate(degrees, axis); 
	eye = rotation*eye; 
	up = normalize(rotation*up); 
}

mat4 Transform::lookAt(const vec3 &eye, const vec3 &center, const vec3 &up) 
{
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

mat4 Transform::perspective(float fovy, float aspect, float zNear, float zFar)
{
  mat4 ret;
  // YOUR CODE FOR HW2 HERE
  // New, to implement the perspective transform as well.  

  float deg = glm::radians(fovy)/2;
  float a = aspect; 
  ret = mat4( 1/(a*tan(deg)),0,0,0, 
          0, 1/tan(deg), 0, 0, 
          0,0, -((zFar+zNear)/(zFar-zNear)), -1, 
          0,0, -((2*zFar*zNear)/(zFar-zNear)), 0);

  return ret;
}

mat4 Transform::scale(const float &sx, const float &sy, const float &sz) 
{
  mat4 ret;
  // YOUR CODE FOR HW2 HERE
  // Implement scaling 
  ret = mat4(sx, 0, 0, 0,
              0, sy, 0, 0,
              0, 0, sz, 0, 
              0, 0,0, 1);
  return ret;
}

mat4 Transform::translate(const float &tx, const float &ty, const float &tz) 
{
  mat4 ret;
  // YOUR CODE FOR HW2 HERE
  // Implement translation 
  ret = mat4(1, 0, 0,0,
              0,1,0,0,
              0,0,1,0,
              tx, ty, tz, 1);
  return ret;
}

// To normalize the up direction and construct a coordinate frame.  
// As discussed in the lecture.  May be relevant to create a properly 
// orthogonal and normalized up. 
// This function is provided as a helper, in case you want to use it. 
// Using this function (in readfile.cpp or display.cpp) is optional.  

vec3 Transform::upvector(const vec3 &up, const vec3 & zvec) 
{
  vec3 x = glm::cross(up,zvec); 
  vec3 y = glm::cross(zvec,x); 
  vec3 ret = glm::normalize(y); 
  return ret; 
}


Transform::Transform()
{

}

Transform::~Transform()
{

}
