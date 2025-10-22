#ifndef Shapeobj_HPP
#define Shapeobj_HPP
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

typedef glm::vec3 vec3;
typedef glm::mat4 mat4;
typedef glm::vec4 vec4;


class Base{

public:
	virtual ~Base(){};
	vec3 specular;
	vec3 emission;
	
	vec3 diffuse;
	vec3 ambient;

	
	float shininess;

	mat4 transform;
	virtual vec3 getNormal(vec3 ) = 0;
	char* type;
};

//getNormal
class Sphere : public Base{
public:
	Sphere(vec3 , float );
	virtual	~Sphere();

	
	float radius;
	vec3 center;

	virtual vec3 getNormal(vec3 );
};

class Triangle : public Base{
public:
	Triangle(vec4, vec4 , vec4 );
	//Triangle(unsigned int, unsigned int , unsigned int);
	virtual ~Triangle();
	vec4 A,B,C;
	vec3 normal;


	void applyTrans();
	virtual vec3 getNormal(vec3 );

	
};

#endif
