// Main entry gateway to interact with the brkga package

#ifndef TOPOLOGY_SOLVER_H
#define TOPOLOGY_SOLVER_H

#include <vector>
#include <algorithm>
#include "physical_topology_instance.h"
#include "uniform_striping_decoder.h"

namespace topology_solver {

// parameters for the genetics algorithm
struct GAParams {
	uint32_t p; // num chromosomes per population
	double pe; // probability of elite items into next population
	double pm; // probability of mutants introduced at each iteration into the next population
	double rhoe; // probability that an offspring inherits the allele of its elite parent
	uint32_t num_threads;
	uint32_t K;  // number of independent populations
	uint32_t reset_gen;  // reset after how many generations of not being able to improve upon a better solution
};

// By default, the fitness function will evaluate the uniform striping
class TopologySolver {
  public:
  	TopologySolver(const uint32_t iterations, const GAParams& params, 
  		 			 const PhysicalTopologyInstance& physical_instance) : 
  			max_iterations_(iterations), instance_(physical_instance) {};

  	virtual ~TopologySolver() {};  

  	void solve();

  private:
  	uint32_t max_iterations_;
  	GAParams params_;
  	PhysicalTopologyInstance instance_;
};

}  // namespace topology_solver

#endif
