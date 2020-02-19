#pragma once
#include <vector>
#include <iostream>

 #define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>

#pragma once
#include <vector>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;
using namespace std;

class Curve
{
public:
    Curve();
    ~Curve();
    
    bool curve_on = true;
    int num_of_controlPoints;
    
    void init();
    void calculate_curve();
    //void curve_reset();
    vector<vec3> catmull_rom(const vec3& P0,
                             const vec3& P1,
                             const vec3& P2,
                             const vec3& P3);
    glm::mat4 rotate_cube(glm::mat4 cube_mat,int index);
    glm::mat4 quaterion_to_matrix(glm::quat quaterion, glm::vec3 position);
    
    int count;
    
private:
    
    
public:
    
    float tau = 0.5; // Coefficient for catmull-rom spline
    int num_points_per_segment = 250;
    
    vector<vec3> control_points_pos;
    vector<vec3> curve_points_pos;
    vector<glm::quat> control_points_quaternion;
};

