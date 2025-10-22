#ifndef HW4_HPP
#define HW4_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

typedef glm::vec3 vec3;
typedef glm::mat4 mat4;
typedef glm::vec4 vec4;

class Base {
public:
	virtual ~Base() {};
	vec3 diffuse;
	vec3 ambient;
	vec3 specular;
	vec3 emission;
	float shininess;
	char* type;
	mat4 transform;

	virtual vec3 getNormal(vec3) = 0;
};

class Sphere : public Base {
public:
	Sphere(vec3, float);
	virtual ~Sphere();

	vec3 ct;
	float rd;

	virtual vec3 getNormal(vec3);
};

class Triangle : public Base {
public:
	Triangle(vec4, vec4, vec4);

	virtual ~Triangle();

	void applyTransform();
	virtual vec3 getNormal(vec3);

	void setNormal();
	vec3 getNormal();

	vec4 A, B, C;
	vec3 normal;
};

#endif