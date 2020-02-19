#pragma once

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Animation
{
public:

	Animation();
	~Animation();
    
    int angle_a;
    int angle_b;
    float angle_a_now = 0;
    float angle_b_now = 0;

    void init();
	void update(float delta_time);
    
    //void rotateLocal(float angle);
    //void rotateGlobal(float angle);
    void rotateLocal();
    void rotateGlobal();

	void reset();
    
    glm::vec3 localAxis = glm::vec3( 1, 0, 0);
    glm::vec3 globalAxis = glm::vec3( 0, 1, 0);
    glm::vec3 vector_initial = glm::vec3(5.0, 0.0, 0.0);
    glm::vec3 vector_now = vector_initial;
    glm::mat3 nowMat3 = glm::mat3();
    glm::mat3 localMat3 = glm::mat3();
    glm::mat3 globalMat3 = glm::mat3();
    glm::mat4 model_rotated;
	glm::mat4 get_model_mat() { return m_model_mat; };

private:
	glm::mat4 m_model_mat;
    
};

