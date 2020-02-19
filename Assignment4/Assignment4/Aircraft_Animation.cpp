#include <glm/gtx/quaternion.hpp>
#include "glm/ext.hpp"
#include "Aircraft_Animation.h"


Aircraft_Animation::Aircraft_Animation()
{
    this->m_model_mat = glm::mat4(1.0f);
}

Aircraft_Animation::~Aircraft_Animation()
{
}

void Aircraft_Animation::init()
{
    reset();
}

void Aircraft_Animation::init(Curve* animation_curve)
{
    m_animation_curve = animation_curve;
    u_number = m_animation_curve->num_points_per_segment;
    
    time_animation = 0;
    reset();
}

void Aircraft_Animation::reset()
{
    is_moving = false;
    move_end = true;
    time_animation = 0.0;
    distance_animation = 0.0;
    position_current = position_entities.begin();
    quaterion_current = quaterion_entities.begin();
    
    position = m_animation_curve->control_points_pos[0];
    quaterion = m_animation_curve->control_points_quaternion[0];
    QuaterionMatrix = m_animation_curve->quaterion_to_matrix(quaterion, position);
    m_model_mat = QuaterionMatrix * glm::mat4(1.0f);
}

void Aircraft_Animation::update(float delta_time)
{
    float time_normalized = 0.0;
    float distance_normalized = 0.0;
    
    if( m_animation_curve->curve_on == true && m_animation_curve->curve_points_pos.empty() == false && position_entities.empty() == true && quaterion_entities.empty() == true) {
        
        table_for_position();
        table_for_orientation();
        position_current = position_entities.begin();
        quaterion_current = quaterion_entities.begin();
        distance_total = position_entities[position_entities.size()-1][3];
        
    }
    
    else if (is_moving == true && position_entities.empty() == false && quaterion_entities.empty() == false) {
        
        time_animation = time_animation + delta_time;
        time_normalized = time_animation / total_moving_time;
        
        distance_normalized = time_to_length( time_normalized );
        distance_animation = distance_total * distance_normalized;
        
        
        if ( distance_animation == 0.0 || distance_animation >= distance_total) {
            position = m_animation_curve->control_points_pos[0];
            quaterion = m_animation_curve->control_points_quaternion[0];
        }
        else {
            position = position_search_next(distance_animation);
            quaterion = quaterion_search_next(distance_animation);
        }

        QuaterionMatrix = m_animation_curve->quaterion_to_matrix(quaterion, position);
        m_model_mat = QuaterionMatrix * glm::mat4(1.0f);
    }
    
    else if ( is_moving == false && distance_animation >= distance_total ) {
        reset();
    }
    
}

float Aircraft_Animation::time_to_length(float time_updated)
{
    float velocity;
    float length;
    float v0 = 2 / (1 - t1 + t2);
    
    if ( time_updated > 0 && time_updated <= t1 ) {
        velocity = v0 * (time_updated / t1);
        length =  (v0 * time_updated * time_updated) / (2 * t1);
    }
    else if ( time_updated > t1 && time_updated <= t2) {
        velocity = v0;
        length = (v0 * t1) / 2 + v0 * (time_updated - t1);
    }
    else if ( time_updated > t2 && time_updated <= 1 ) {
        velocity = v0 * (1- (time_updated - t2) / (1 - t2) );
        length = 1 - velocity * (1 - time_updated) / 2 ;
    }
    else if ( time_updated > 1) {
        velocity = 0;
        length = 1;
    }
    else {
        velocity = 0;
        length = 0;
    }
    
    return length;
}

// ---------- Calculate point location
void Aircraft_Animation::table_for_position()
{
    float piece_length;
    float arc_length = 0;
    std::vector<glm::vec3> curve_points = m_animation_curve->curve_points_pos;
    std::vector<vec3>::iterator it;
    
    for ( it = curve_points.begin() ; it != curve_points.end() ; it++) {
        
        if (it == curve_points.begin()) {
            arc_length = 0;
        }
        else {
            piece_length = sqrt(pow((*(it))[0] - (*(it-1))[0], 2) +
                                pow((*(it))[1] - (*(it-1))[1], 2) +
                                pow((*(it))[2] - (*(it-1))[2], 2) * 1.0);
            arc_length = arc_length + piece_length;
            position_entities.push_back({*it, arc_length});
        }
    }
}

glm::vec3 Aircraft_Animation::position_search_next(float distance)
{
    glm::vec3 position;
    
    if ( distance == (*position_current)[3] ) {
        position = { (*position_current)[0], (*position_current)[1], (*position_current)[2] };
    }
    else if ( distance < (*position_current)[3] ) {
        if (position_current == position_entities.begin()) {
            position = interpolation( distance, glm::vec4( 0.0, 8.5, -2.0, 0.0 ) , *position_current );
        }
        else {
            position = interpolation( distance, *(position_current-1), *position_current );
        }
    }
    else {
        position_current = position_current + 1;
        position = position_search_next(distance);
    }
    
    return position;
}

glm::vec3 Aircraft_Animation::interpolation(float distance, glm::vec4 start, glm::vec4 end)
{
    float length_total = end[3] - start[3];
    float length_delta = distance - start[3];
    float ratio = length_delta / length_total;
    glm::vec3 interpolated_poit = { (start[0] + ratio*(end[0] - start[0])),
                                    (start[1] + ratio*(end[1] - start[1])),
                                    (start[2] + ratio*(end[2] - start[2])) };
    
    return interpolated_poit;
}

// ---------- Calculate point orientation
void Aircraft_Animation::table_for_orientation()
{
    int point_index;
    int segment;
    int u_index;
    float piece_length;
    float partial_length = 0;
    float total_length = 0;
    std::vector<glm::vec3> curve_points = m_animation_curve->curve_points_pos;
    std::vector<vec3>::iterator it;
    
    for ( it = curve_points.begin() ; it != curve_points.end() ; it++) {
        
        point_index = (int) std::distance(curve_points.begin(), it);
        segment = point_index/u_number;
        u_index = segment*u_number +1;
        
        if (it == curve_points.begin()) {
            total_length = 0;
            piece_length = 0;
        }
        else {
            piece_length = sqrt(pow((*it)[0] - (*(it-1))[0], 2) +
                                pow((*it)[1] - (*(it-1))[1], 2) +
                                pow((*it)[2] - (*(it-1))[2], 2) * 1.0);
            total_length = total_length + piece_length;
        }
        
        quaterion_entities.push_back( {segment, total_length, point_index} );
    }
}

glm::quat Aircraft_Animation::quaterion_search_next(float distance)
{
    int start;
    int end;
    float segment_distance;
    float delta_distance;
    
    glm::quat orientation;
    
    if ( distance > (*quaterion_current)[1] ) {
        quaterion_current = quaterion_current + 1;
        orientation = quaterion_search_next(distance);
    }
    else {
        
        //cout << "Point Index = " << (*quaterion_current)[2] << endl;
        
        if ( (*quaterion_current)[2] == (*quaterion_current)[0]*u_number ) {
            start = (int)(*quaterion_current)[0] -1;
            end = (int)(*quaterion_current)[0];
        }
        else {
            start = (int)(*quaterion_current)[0];
            end = (int)(*quaterion_current)[0] + 1;
        }
        
        segment_distance = quaterion_entities[end*u_number][1] - quaterion_entities[start*u_number][1];
        delta_distance = distance - quaterion_entities[start*u_number][1];
        
        orientation = slerp( delta_distance,
                             segment_distance,
                             m_animation_curve->control_points_quaternion[start],
                             m_animation_curve->control_points_quaternion[(end % 8)]   );
    }
    
    return orientation;
}

glm::quat Aircraft_Animation::slerp(float length_delta, float length_segment, glm::quat start, glm::quat end)
{
    float ratio = length_delta / length_segment;
    glm::quat interpolated_quat = glm::slerp(start, end, ratio);
    
    return interpolated_quat;
}
