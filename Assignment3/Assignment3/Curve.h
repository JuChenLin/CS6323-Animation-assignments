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
    
    void init();
	void calculate_curve();
    void show_curve();
    void curve_reset();
    
    int num_of_controlPoints;
    
    vector<vec3> catmull_rom(const vec3& P0,
                     const vec3& P1,
                     const vec3& P2,
                     const vec3& P3);
    void pick_control_points();
    
private:
    
	
public:
    
    float tau = 0.5; // Coefficient for catmull-rom spline
    int num_points_per_segment = 250;

	vector<vec3> control_points_pos;
	vector<vec3> curve_points_pos;
    //vector<vec3> show_points_pos;
};

