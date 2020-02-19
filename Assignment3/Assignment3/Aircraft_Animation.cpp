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
    time_animation = 0;
    poit_current = glm::vec3( 0.0, 8.5, -2.0 );
    poit_next = glm::vec3( 0.0, 8.5, -2.0 );
	reset();
}

void Aircraft_Animation::update(float delta_time)
{
    float time_normalized = 0.0;
    float distance_normalized = 0.0;
    
    if( m_animation_curve->curve_on == true && m_animation_curve->curve_points_pos.empty() == false && u_length.empty() == true) {
        arc_lenth_table();
        distance_total = u_length[u_length.size()-1][3];
        current_it = u_length.begin();
    }
    
    else if (is_moving == true && u_length.empty() == false) {

        time_animation = time_animation + delta_time;
        time_normalized = time_animation / total_moving_time;
        
        distance_normalized = time_to_length( time_normalized );
        distance_animation = distance_total * distance_normalized;
        
        
        if ( distance_animation == 0.0) {
            poit_current = glm::vec3( 0.0, 8.5, -2.0 );
            poit_next = glm::vec3( 0.0, 8.5, -2.0 );
            
        }
        else {
            poit_current = poit_next;
            poit_next = search_next(distance_animation);
        }
        
        cout << "Position : " << poit_next[0] << ", " << poit_next[1] << ", " << poit_next[2] << endl;
        
        vector = poit_next - poit_current;
        m_model_mat = glm::translate(m_model_mat, vector);
    }
    
    else if ( is_moving == false && distance_animation >= distance_total ) {
        move_reset();
    }
    
}

void Aircraft_Animation::reset()
{
	m_model_mat = glm::mat4(1.0f);

	if (m_animation_curve != nullptr && m_animation_curve->control_points_pos.size() > 0)
	{
		m_model_mat = glm::translate(m_model_mat, m_animation_curve->control_points_pos[0]);
	}
    move_reset();
}

void Aircraft_Animation::move_reset()
{
    is_moving = false;
    move_end = true;
    time_animation = 0.0;
    distance_animation = 0.0;
    current_it = u_length.begin();
    poit_current = glm::vec3( 0.0, 8.5, -2.0 );
    poit_next = glm::vec3( 0.0, 8.5, -2.0 );
}

void Aircraft_Animation::arc_lenth_table()
{
    float piece_length;
    float arc_length = 0;
    //vec4 one_entry;
    std::vector<glm::vec3> poits = m_animation_curve->curve_points_pos;
    std::vector<vec3>::iterator it;
    
    for ( it = poits.begin() ; it != poits.end() ; it++) {
        
        if (it == poits.begin()) {
            arc_length = 0;
        }
        else {
            piece_length = sqrt(pow((*(it))[0] - (*(it-1))[0], 2) +
                                pow((*(it))[1] - (*(it-1))[1], 2) +
                                pow((*(it))[2] - (*(it-1))[2], 2) * 1.0);
            arc_length = arc_length + piece_length;
            u_length.push_back({*it, arc_length});
            
            cout << "Table : " << (*it)[0] << ", " << (*it)[1] << ", " << (*it)[2] << ", " << arc_length << endl;
        }
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

glm::vec3 Aircraft_Animation::search_next(float distance)
{
    glm::vec3 position;

    if ( distance == (*current_it)[3] ) {
        position = { (*current_it)[0], (*current_it)[1], (*current_it)[2] };
    }
    else if ( distance < (*current_it)[3] ) {
        if (current_it == u_length.begin()) {
            position = interpolation( distance, glm::vec4( 0.0, 8.5, -2.0, 0.0 ) , *current_it );
        }
        else {
            position = interpolation( distance, *(current_it-1), *current_it );
        }
    }
    else {
        current_it = current_it + 1;
        position = search_next(distance);
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

