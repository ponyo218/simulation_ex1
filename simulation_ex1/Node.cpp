#include "Node.h"
#include "Packet.h"
#include <float.h>
#include <iostream>

Node::Node(int buf_size,std::string name) { 
	m_buf_size = buf_size;
	m_get_num = 0;
	m_abort_num = 0;
	m_current_service_t = DBL_MAX;
	m_system_num_dot_t = 0;
	m_name = name;
	m_buffer = std::queue<Packet>();
	m_service_area = Service_area();
	m_link = std::vector<Packet>();
}

void Node::Arrive_Event(Packet packet, double &current_t, double next_arrive_t) {
#if DEBUG_LOG
	std::cout << "Arrive_Event_at_" << m_name << std::endl;
#endif
	m_get_num++;
	Calc_System_Num_Dot_t(next_arrive_t - current_t);
	current_t = next_arrive_t;
	packet.m_get_system_time = current_t;
	if (m_buf_size <= (int)m_buffer.size()) {
		m_abort_num++;
#if DEBUG_LOG
		std::cout << "Abort_" << m_name << std::endl;
#endif
	}
	else {
		m_buffer.push(packet);
		if (!m_service_area.area) {
			Enter_Service_Area(current_t);
			m_current_service_t = current_t + m_service_area.m_service_packet.m_service_interval;
		}
	}
#if DEBUG_LOG
	Print_Info(current_t);
#endif
}

void Node::Trans_Event(double &current_t) {
	if (m_service_area.area) {
		Calc_System_Num_Dot_t(m_current_service_t - current_t);
		current_t = m_current_service_t;
		Exit_Service_Area(current_t);
#if DEBUG_LOG
		std::cout << "Trans_Event_at_" << m_name << " current_time: " << current_t << std::endl;
#endif
	}
	else {
#if DEBUG_LOG
		printf("can`t Trans_Event\n");
#endif
	}
	if (!m_buffer.empty()) {
		Enter_Service_Area(current_t);
		m_current_service_t = current_t + m_service_area.m_service_packet.m_service_interval;
	}
	else {
		m_current_service_t = DBL_MAX;
		m_service_area.area = false;
	}
#if DEBUG_LOG
	Print_Info(current_t);
#endif
}

void Node::Enter_Service_Area(double current_t) {
	Packet front_packet = m_buffer.front();
	m_buffer.pop();
	front_packet.m_into_service_area_time = current_t;
	m_service_area.m_service_packet = front_packet;
	m_service_area.area = true;
#if DEBUG_LOG
	std::cout << "Enter_Service_Area" << std::endl;
#endif
}

void Node::Exit_Service_Area(double current_t) {
	m_service_area.m_service_packet.m_out_service_area_time = current_t;
	m_link.emplace_back(m_service_area.m_service_packet);
	m_service_area.area = false;
}

void Node::Print_Info(double current_t) {
	std::cout << m_name << ": " << m_buffer.size() << " service_area: " << m_service_area.area << " current_time: "<< current_t << " get_num: " << m_get_num << std::endl << std::endl;
}

double Node::Get_Abort_rate() {
	return (double)m_abort_num / (double)m_get_num;
}

double Node::Get_Ave_Num_System(double current_t) {
	return m_system_num_dot_t / current_t;
}

void Node::Calc_System_Num_Dot_t(double interval) {
	int system_num = (int)m_buffer.size() + (m_service_area.area ? 1 : 0);
	m_system_num_dot_t += system_num * interval;
}