#include "Packet.h"

Packet::Packet(double service_interval, double random_get_interval, int packet_num, std::string trans_node){
	m_service_interval = service_interval;
	m_Random_get_interval = random_get_interval;
	m_packet_num = packet_num;
	m_trans_node = trans_node;
	m_get_system_time = 0;
	m_into_service_area_time = 0;
	m_out_service_area_time = 0;
}

