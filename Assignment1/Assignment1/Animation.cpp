#include "Animation.h"

Animation::Animation()
{
	this->m_model_mat = glm::mat4();
}

Animation::~Animation()
{
}

void Animation::init()
{
	reset();
}

void Animation::update(float delta_time)
{
}

void Animation::reset()
{
    angle_a_now = 0;
    angle_b_now = 0;
    vector_initial = glm::vec3(5.0, 0.0, 0.0);
    vector_now = vector_initial;
    localMat3 = glm::mat3();
    globalMat3 = glm::mat3();
    m_model_mat = glm::mat4();
	m_model_mat = glm::translate(m_model_mat, vector_initial);
}

void Animation::rotateLocal()
{
    angle_a_now = angle_a_now + (float)angle_a;
    model_rotated = glm::mat4();
    
    float cos_angle_a = glm::cos(glm::radians(-angle_a_now));
    float sin_angle_a = glm::sin(glm::radians(-angle_a_now));
    float localArray[9] = { 1,        0,           0,
                            0,  cos_angle_a,  -sin_angle_a,
                            0,  sin_angle_a,  cos_angle_a};
    localMat3 = glm::make_mat3(localArray);

    model_rotated = glm::translate(model_rotated, vector_now);
    model_rotated = glm::rotate( model_rotated, glm::radians(angle_b_now), globalAxis );
    m_model_mat = glm::rotate( model_rotated, glm::radians(angle_a_now), localAxis );
    model_rotated = m_model_mat;
    
}

void Animation::rotateGlobal()
{
    angle_b_now = angle_b_now + (float)angle_b;
    model_rotated = glm::mat4();
    
    float cos_angle_b = glm::cos(glm::radians(angle_b_now));
    float sin_angle_b = glm::sin(glm::radians(angle_b_now));
    float globalArray[9] = {cos_angle_b, 0, -sin_angle_b,
                               0,       1,       0,
                           sin_angle_b, 0,  cos_angle_b};
    globalMat3 = glm::make_mat3(globalArray);
    vector_now = globalMat3*vector_initial;
    
    model_rotated = glm::translate( model_rotated, vector_now );
    model_rotated = glm::rotate( model_rotated, glm::radians(angle_b_now), globalAxis );
    m_model_mat = glm::rotate( model_rotated, glm::radians(angle_a_now), localAxis );
    model_rotated = m_model_mat;


    
}

