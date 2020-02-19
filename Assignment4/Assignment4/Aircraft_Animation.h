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
    
    int u_number;
    float distance_total = 0;
    glm::mat4 m_model_mat;
    glm::mat4 QuaterionMatrix;
    Curve* m_animation_curve = nullptr;
    
    std::vector<glm::vec4> position_entities;
    std::vector<glm::vec3> quaterion_entities;
    std::vector<glm::vec4>::iterator position_current;
    std::vector<glm::vec3>::iterator quaterion_current;
    
    glm::vec3 position;
    glm::quat quaterion;
    
    float time_to_length(float time_updated);
    
    // ---------- Calculate point location
    void table_for_position();
    glm::vec3 position_search_next(float distance);
    glm::vec3 interpolation(float distance, glm::vec4 start, glm::vec4 end);
    
    // ---------- Calculate point orientation
    void table_for_orientation();
    glm::quat quaterion_search_next(float distance);
    glm::quat slerp(float length_delta, float length_segment, glm::quat start, glm::quat end);
    
    
public:
    Aircraft_Animation();
    ~Aircraft_Animation();
    
    void init();
    void init(Curve* animation_curve);
    void reset();
    
    void update(float delta_time);
    
    glm::mat4 get_model_mat() { return m_model_mat; };
};




