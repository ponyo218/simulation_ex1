#pragma once
#include<queue>
#include<string>
#include"Packet.h"

#define DEBUG_LOG 0

class Packet;

class Node
{
	struct Service_area {
		Packet m_service_packet;
		bool area;

		Service_area() {
			m_service_packet = Packet(0,0,0,"");
			area = false;
		};
	};
public:
	Node() {};
	Node(int buf_size, std::string name);
	void Arrive_Event(Packet packet, double & current_t, double next_arrive_t);
	void Trans_Event(double &current_t);
	void Enter_Service_Area(double current_t);
	void Exit_Service_Area(double current_t);
	void Print_Info(double current_t);
	double Get_Abort_rate();
	double Get_Ave_Num_System(double current_t);
	void Calc_System_Num_Dot_t(double interval);

	int m_buf_size;
	int m_get_num;
	int m_abort_num;
	double m_current_service_t;
	double m_system_num_dot_t;
	std::string m_name;
	std::queue<Packet> m_buffer;
	Service_area m_service_area;
	std::vector<Packet> m_link;
};

