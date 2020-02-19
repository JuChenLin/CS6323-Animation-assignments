#pragma once

#include <vector>
#include <iostream>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Curve.h"

class Aircraft_Animation
{

public:
    bool is_moving;
    bool move_end;
	float total_moving_time = 10;
	float t1 = 0.3;
	float t2 = 0.7;
    float time_animation;
    float distance_animation;

private:
	
    float distance_total = 0;
    glm::mat4 m_model_mat;
	Curve* m_animation_curve = nullptr;
    
    std::vector<glm::vec4> u_length;
    std::vector<glm::vec4>::iterator current_it;
    glm::vec3 poit_current;
    glm::vec3 poit_next;
    glm::vec3 vector;

public:
	Aircraft_Animation();
	~Aircraft_Animation();

	void init();
	void init(Curve* animation_curve);

	void update(float delta_time);
    
    void move_reset();
    void arc_lenth_table();
    float time_to_length(float time_updated);
    glm::vec3 search_next(float distance);
    glm::vec3 interpolation(float distance, glm::vec4 start, glm::vec4 end);

	void reset();
	glm::mat4 get_model_mat() { return m_model_mat; };
};

