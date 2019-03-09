#include <iostream>
#include <algorithm>
#include "physical_topology_instance.h"
#include "topology_solver.h"

int main(int argc, char* argv[]) {
	if (argc > 2) {
		std::cerr << "usage: ./topol_striping argfile" << std::endl;
		return 1;
	}
	std::cout << "Hello world" << std::endl;
	topology_solver::PhysicalTopologyInstance physical_instance;
	
	// add nodes, n = 4
	physical_instance.add_node();
	physical_instance.add_node();
	physical_instance.add_node();
	physical_instance.add_node();
	physical_instance.set_node_links(0, 3);
	physical_instance.set_node_links(1, 3);
	physical_instance.set_node_links(2, 3);
	physical_instance.set_node_links(3, 3);

	// add coverings, k = 3
	physical_instance.add_covering();
	physical_instance.add_covering();
	physical_instance.add_covering();
	physical_instance.set_covering_links(0, 4);
	physical_instance.set_covering_links(1, 4);
	physical_instance.set_covering_links(2, 4);

	topology_solver::GAParams params;
	const uint32_t num_iterations = 30;
	params.pe = 0.7; // probability of elite items into next population
	params.pm = 0.2; 
	params.rhoe = 0.1;
	params.num_threads = 3;
	params.K = 3;
	params.reset_gen = 30;

	
	topology_solver::TopologySolver solver(num_iterations, params, physical_instance);
	std::cout << "Exited topology designer" << std::endl;

	// maybe each chromosome itself is a physical connectivity, and 
	// then the population just contains different stripings
	return 0;
}
