#include "topology_solver.h"

#include "brkgaAPI/MTRand.h" 
#include "brkgaAPI/BRKGA.h"
namespace topology_solver {

// iteratively employ the genetics algorithm
void TopologySolver::solve(const uint32_t num_iter) {
	// initialize by creating the decoder instance and also the random number generator
	const long unsigned rng_seed = 10035353;
	UniformStripingDecoder usd(instance_);
	MTRand rng(rng_seed);
	BRKGA<UniformStripingDecoder, MTRand> ga(instance_.get_num_nodes(), instance_.get_num_coverings(), 
			params_.pe, params_.pm, params_.rhoe, usd, rng, params_.K, params_.num_threads);
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
			best_chromosomes = ga.getBestChromosome();
		}

		// reset the genetics algorithm after several tries of not improving solution significantly
		if (i - best_generation  > params_.reset_gen) {
			ga.reset();
			best_generation = i;
			std::cout << "Reset at generation " << std::to_string(i) << std::endl;
		}

		// evolution strategy exchange top individuals among the populations
	}

	// now try to reconstruct the best chromosomes as the striping solution
	std::vector<Covering> striping_pattern = usd.get_striping(best_chromosomes);
	std::cout << "Best striping value has fitness value of: " 
			  << std::to_string(usd.get_striping_goodness(best_chromosomes)) << std::endl;
}

}  //  namespace topology_solver
