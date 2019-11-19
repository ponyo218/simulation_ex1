#pragma once
#include <string>
//#include "Node.h"

//class Node;

class Packet
{
public:
	Packet() {};
	Packet(double service_interval,double random_get_interval,int packet_num, std::string trans_node);
	int m_packet_num;
	double m_service_interval;
	double m_Random_get_interval;
	double m_get_system_time;
	double m_into_service_area_time;
	double m_out_service_area_time;
	std::string m_trans_node;
};

