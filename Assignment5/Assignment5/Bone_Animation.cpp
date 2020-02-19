#include "Bone_Animation.h"
#include <glm/gtx/euler_angles.hpp>


Bone_Animation::Bone_Animation()
{
}


Bone_Animation::~Bone_Animation()
{
}

void Bone_Animation::init()
{
    root_position = { 2.0f,1.0f,2.0f };
    
    scale_vector =
	{
		{1.0f,1.0f,1.0f},
		{0.5f,4.0f,0.5f},
		{0.5f,3.0f,0.5f},
		{0.5f,2.0f,0.5f}
	};

	rotation_degree_vector = 
	{
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f}
	};

	colors = 
	{
		{0.7f,0.0f,0.0f,1.0f},
		{0.7f,0.7f,0.0f,1.0f},
		{0.7f,0.0f,0.7f,1.0f},
		{0.0f,0.7f,0.7f,1.0f}
	};
    
    tree_depth = (int)scale_vector.size();
    
    reset();
}

void Bone_Animation::update(float delta_time)
{
    std::vector<float> angleX = { glm::radians(rotation_degree_vector[0][2]),
        glm::radians(rotation_degree_vector[1][2]),
        glm::radians(rotation_degree_vector[2][2]),
        glm::radians(rotation_degree_vector[3][2]) };
    
    std::vector<float> angleY = { glm::radians(rotation_degree_vector[0][0]),
        glm::radians(rotation_degree_vector[1][0]),
        glm::radians(rotation_degree_vector[2][0]),
        glm::radians(rotation_degree_vector[3][0]) };
    
    std::vector<float> angleZ = { glm::radians(rotation_degree_vector[0][1]),
        glm::radians(rotation_degree_vector[1][1]),
        glm::radians(rotation_degree_vector[2][1]),
        glm::radians(rotation_degree_vector[3][1]) };
    
    rotateX = { glm::eulerAngleX(angleX[0]), glm::eulerAngleX(angleX[1]), glm::eulerAngleX(angleX[2]), glm::eulerAngleX(angleX[3]) };
    rotateY = { glm::eulerAngleX(angleX[0]), glm::eulerAngleY(angleY[1]), glm::eulerAngleY(angleY[2]), glm::eulerAngleY(angleY[3]) };
    rotateZ = { glm::eulerAngleX(angleX[0]), glm::eulerAngleZ(angleZ[1]), glm::eulerAngleZ(angleZ[2]), glm::eulerAngleZ(angleZ[3]) };
    
    for (int i = 0; i < tree_depth; i++)
    {
        rotateMatrix[i] = rotateX[i] * rotateZ[i] * rotateY[i];
        
        if (i == 0)
        {
            translateMatrix_link[i] = glm::translate(glm::mat4(1.0f)  , { root_position[0], ( root_position[1] - scale_vector[i][1]/2.0f ),root_position[2] } );
            
            worldMatrix[i] = translateMatrix_link[i] * rotateMatrix[i];
            bone_mat[i] = glm::translate( glm::mat4(1.0f)  , root_position );
        }
        else
        {
            translateMatrix_origin[i] = glm::translate(glm::mat4(1.0f) , {0, scale_vector[i][1]/2.0f, 0} );
            translateMatrix_link[i] = glm::translate(glm::mat4(1.0f)  , {0, scale_vector[i-1][1], 0} );
            
            worldMatrix[i] = worldMatrix[i-1] * translateMatrix_link[i] * rotateMatrix[i];
            bone_mat[i] =  worldMatrix[i] * translateMatrix_origin[i];
        }
    }
}

void Bone_Animation::reset()
{
    rotation_degree_vector =
    {
        {0.0f,0.0f,0.0f},
        {0.0f,0.0f,0.0f},
        {0.0f,0.0f,0.0f},
        {0.0f,0.0f,0.0f}
    };
    
    bone_mat = {glm::mat4(1.0f), glm::mat4(1.0f), glm::mat4(1.0f), glm::mat4(1.0f)};
}

