#include "topology_solver.h"

#include "brkgaAPI/MTRand.h" 

namespace topology_solver {

// iteratively employ the genetics algorithm
void TopologySolver::Solve(const uint32_t num_iter) {
	// initialize by creating the decoder instance and also the random number generator
	const long unsigned rng_seed = 10035353;
	UniformStripingDecoder usd(instance_);
	MTRand rng(rng_seed);
	BRKGA<UniformStripingDecoder, MTRand> ga(params_.n, params_.p, params_.pe, params_.pm, params_.rhoe, usd, rng, );
	// TODO(jason) : check if this is right or wrong
	double best_fitness = 1000000; 
	uint32_t best_generation = 0;
	std::vector<double> best_chromosomes;
	for (int i = 1; i <= num_iter; i++) {
		ga.evolve(); // first try to evolve population

		// find out if the current generation has the best fitness after evolving
		if (ga.getBestFitness() < best_fitness) {
			best_generation = i;
			best_fitness = ga.getBestFitness();
			best_chromosomes = ga.getBestChromosomes();
		}

		// reset the genetics algorithm after several tries of not improving solution significantly
		if (i - best_generation  > reset_after_generations) {
			ga.reset();
			best_generation = i;
		}

		// evolution strategyL exchange top individuals among the populations
	}
	// now try to reconstruct the best chromosomes as the striping solution
}

}  //  namespace topology_solver
