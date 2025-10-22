# version 330 core
// Do not use any version older than 330!

/* This is the fragment shader for reading in a scene description, including 
   lighting.  Uniform lights are specified from the main program, and used in 
   the shader.  As well as the material parameters of the object.  */

// Inputs to the fragment shader are the outputs of the same name of the vertex shader.
// Note that the default output, gl_Position, is inaccessible!
in vec3 mynormal; 
in vec4 myvertex; 

// You will certainly need this matrix for your lighting calculations
uniform mat4 modelview;

// This first defined output of type vec4 will be the fragment color
out vec4 fragColor;

uniform vec3 color;

const int numLights = 10; 
uniform bool enablelighting; // are we lighting at all (global).
uniform vec4 lightposn[numLights]; // positions of lights 
uniform vec4 lightcolor[numLights]; // colors of lights
uniform int numused;               // number of lights used

// Now, set the material parameters.
// I use ambient, diffuse, specular, shininess. 
// But, the ambient is just additive and doesn't multiply the lights.  

uniform vec4 ambient; 
uniform vec4 diffuse; 
uniform vec4 specular; 
uniform vec4 emission; 
uniform float shininess; 

vec4 ComputeLight (const in vec3 direction, const in vec4 lightcolor, const in vec3 normal, const in vec3 halfvec, const in vec4 mydiffuse, const in vec4 myspecular, const in float myshininess) {

        float nDotL = dot(normal, direction)  ;         
        vec4 lambert = mydiffuse * lightcolor * max (nDotL, 0.0) ;  

        float nDotH = dot(normal, halfvec) ; 
        vec4 phong = myspecular * lightcolor * pow (max(nDotH, 0.0), myshininess) ; 

        vec4 retval = lambert + phong ; 
        return retval ;            
}    

void main (void) 
{       
    if (enablelighting) {       
        vec4 finalcolor; 

        // YOUR CODE FOR HW 2 HERE
        // A key part is implementation of the fragment shader

        // They eye is always at (0,0,0) looking down -z axis 
        // Also compute current fragment position and direction to eye 

        const vec3 eyepos = vec3(0,0,0) ; 
        vec4 newPos = modelview*myvertex;
        vec3 mypos = newPos.xyz / newPos.w ; // Dehomogenize current location 
        
        vec3 eyedirn = normalize(eyepos - mypos) ; 

        // Compute normal, needed for shading. 
        mat3 MM = mat3(transpose(inverse( modelview ) ));
        // multiply it by the current normal and then normalize to a unit vector
        vec3 normal = normalize( MM * mynormal );


        vec4 lSum = vec4(1.0, 0.0, 0.0, 1.0);
        lSum = ambient + emission;

        for (int i = 0; i < numused; i++)
        {
            vec4 lightPosition = lightposn[i];
            vec3 lightdir, halfvec;


            if (lightPosition.w == 0) // Check if light is directional
            {
                lightdir = normalize(lightPosition.xyz);
                
                halfvec = normalize(lightdir + eyedirn);
            }
            else // Light is point
            {
                vec3 position = lightPosition.xyz / lightPosition.w;
                lightdir = normalize(position - mypos);
            
                halfvec = normalize(lightdir + eyedirn);
            }
            lSum += ComputeLight(lightdir, lightcolor[i], normal, halfvec, diffuse, specular, shininess);

        }
        finalcolor =  lSum;

        fragColor = finalcolor;







        // Color all pixels black for now, remove this in your implementation! 
        //changed from 0001 to 1001
        // finalcolor = vec4(1.0f, 0.0f, 0.0f, 1.0f); 

        // fragColor = finalcolor; 
    } else {
        fragColor = vec4(color, 1.0f); 
    }
}
