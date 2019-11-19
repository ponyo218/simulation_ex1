#include <iostream>
#include <math.h>
#include <random>
#include <time.h>
#include <float.h>
#include "Node.h"
#include "Packet.h"

double Random_Get(double lambda);
void Evaluation(Node,Node,double);

//std::random_device rnd;
std::mt19937 mt;
std::uniform_real_distribution<> rand1(0, 1);
std::uniform_real_distribution<> rand2(0, 2);

int main(int argc, char* argv[]) {
	const int A = 400, B = 400, D = 1000000;
	double current_t = 0;
	const double lambda = 2.0;
	const double p = 0.5;
	int packet_num;

	//init
	//srand((unsigned int)time(NULL));
	Node node_A = Node(A,"A");
	Node node_B = Node(B,"B");
	packet_num = 0;

	while (D > packet_num) {
		double random_get_i = Random_Get(lambda);
		double next_arrive_t = current_t + random_get_i;
		double random_service_i = rand2(mt);
#if DEBUG_LOG
		printf("get_interval:%f service_interval%f: current_time:%f\n", random_get_i,random_service_i,current_t);
#endif
		double current_p = rand1(mt);
		Packet get_packet;
		bool packet_get = false;
		if (p > current_p) {
			get_packet = Packet(random_service_i, random_get_i, packet_num,"A");
		}
		else {
			get_packet = Packet(random_service_i, random_get_i, packet_num,"B");
		}
		while (!packet_get) {
			if (node_A.m_current_service_t >= next_arrive_t && node_B.m_current_service_t >= next_arrive_t) {
				if (get_packet.m_trans_node == "A") {
					node_A.Arrive_Event(get_packet, current_t, next_arrive_t);
				}
				else if (get_packet.m_trans_node == "B") {
					node_B.Arrive_Event(get_packet, current_t, next_arrive_t);
				}
				packet_get = true;
			}
			while (next_arrive_t > node_A.m_current_service_t || next_arrive_t > node_B.m_current_service_t) {
#if DEBUG_LOG
				std::cout << "next_arrive_t: " << next_arrive_t << std::endl;
#endif
				if (node_A.m_current_service_t <= node_B.m_current_service_t) {
					node_A.Trans_Event(current_t);
				}
				else {
					node_B.Trans_Event(current_t);
				}
			}
		}
		packet_num++;
	}
	Evaluation(node_A,node_B,current_t);
}

double Random_Get(double lambda) {
	double rand_num;
	unsigned int rand_tmp = mt();
	constexpr double mt_max = mt.max();
	if (rand_tmp == 0) {
		rand_num = (rand_tmp + 1.0) / mt_max;
	}
	else if(rand_tmp == mt_max) {
		rand_num = (rand_tmp - 1) / mt_max;
		}
	else {
		rand_num = rand_tmp / mt_max;
	}
	return -1 / lambda * std::log(1 - rand_num);
}

void Evaluation(Node node_A,Node node_B,double current_t) {
	double total_abort_rate;
	double total_ave_num_in_system;
	double total_ave_delay_in_system;
	double total_ave_delay_in_buffer;
	int get_total_num = node_A.m_get_num + node_B.m_get_num;
	double A_system = 0, B_system = 0, A_buffer = 0, B_buffer = 0;

	total_abort_rate = (double)((double)node_A.m_abort_num + (double)node_B.m_abort_num) / (double)get_total_num;
	total_ave_num_in_system = (node_A.m_system_num_dot_t + node_B.m_system_num_dot_t) / current_t;
	for (auto packet : node_A.m_link) {
		A_system += (packet.m_out_service_area_time - packet.m_get_system_time);
		A_buffer += (packet.m_into_service_area_time - packet.m_get_system_time);
	}
	for (auto packet : node_B.m_link) {
		B_system += (packet.m_out_service_area_time - packet.m_get_system_time);
		B_buffer += (packet.m_into_service_area_time - packet.m_get_system_time);
	}
	total_ave_delay_in_system = (A_system + B_system) / (double)(node_A.m_link.size() + node_B.m_link.size());
	total_ave_delay_in_buffer = (A_buffer + B_buffer) / (double)(node_A.m_link.size() + node_B.m_link.size());

	std::cout << "get_num: " << get_total_num << std::endl;
	std::cout << "total_abort_rate: " << total_abort_rate << std::endl;
	std::cout << "node_A_abort_rate: " << node_A.Get_Abort_rate() << std::endl;
	std::cout << "node_B_abort_rate: " << node_B.Get_Abort_rate() << std::endl;
	std::cout << "total_ave_num_in_system: " << total_ave_num_in_system << std::endl;
	std::cout << "node_A_num_in_system: " << node_A.Get_Ave_Num_System(current_t) << std::endl;
	std::cout << "node_B_num_in_system: " << node_B.Get_Ave_Num_System(current_t) << std::endl;
	std::cout << "total_ave_delay_in_system: " << total_ave_delay_in_system << std::endl;
	std::cout << "total_ave_delay_in_buffer: " << total_ave_delay_in_buffer << std::endl;

}