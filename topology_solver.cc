#include "topology_solver.h"

#include "brkgaAPI/MTRand.h" 
#include "brkgaAPI/BRKGA.h"
namespace topology_solver {

// iteratively employ the genetics algorithm
void TopologySolver::solve() {
	// initialize by creating the decoder instance and also the random number generator
	const long unsigned rng_seed = 10035353;
	UniformStripingDecoder usd(instance_);
	MTRand rng(rng_seed);
	const uint32_t num_alleles_per_chromosome = 
		instance_.get_num_coverings() * instance_.get_num_nodes();
	
	std::cout << "Printing params..." << std::endl;
	std::cout << "num individuals: " << params_.p << std::endl;
	BRKGA<UniformStripingDecoder, MTRand> ga(num_alleles_per_chromosome, params_.p, 
			params_.pe, params_.pm, params_.rhoe, usd, rng, params_.K, params_.num_threads);



	double best_fitness = 1000000; 
	uint32_t best_generation = 0;
	
	std::vector<double> best_chromosomes;

	for (int i = 1; i <= max_iterations_; i++) {
		ga.evolve(); // first try to evolve population
		// find out if the current generation has the best fitness after evolving
		if (ga.getBestFitness() < best_fitness) {
			best_generation = i;
			best_fitness = ga.getBestFitness();
			best_chromosomes = ga.getBestChromosome();
			std::cout << "Size of best_chromosomes is : " << best_chromosomes.size() << std::endl;
		}

		// reset the genetics algorithm after several tries of not improving solution significantly
		if (i - best_generation  > params_.reset_gen) {
			ga.reset();
			best_generation = i;
			std::cout << "Reset at generation " << std::to_string(i) << std::endl;
		}

		// Evolution strategy: exchange the top individuals among the populations
		if (i %  10 == 0 && best_generation != i) {
			ga.exchangeElite(3); // exchange the top 3 chromosomes
			std::cout << "Exchanging elite individuals: " << std::to_string(3) 
					  << " individuals are exchanged" << std::endl;
		}

		// evolution strategy exchange top individuals among the populations
	}
	std::cout << "Size of best_chromosomes is : " << best_chromosomes.size() << std::endl;
	std::vector<Covering> coverings = 
		usd.transform_chromosome_to_coverings_greedy(best_chromosomes);
	for (const auto& covering : coverings) {
		std::cout << "ocs_id: " <<  covering.id << std::endl << "Covers: ";
		for (const auto& entry : covering.striping) {
			std::cout << entry << " ";
		}
		std::cout << std::endl;
	}
	// now try to reconstruct the best chromosomes as the striping solution
	//std::vector<Covering> striping_pattern = usd.transform_chromosome_to_coverings(best_chromosomes);
	//std::cout << "Best striping value has fitness value of: " 
	//		  << std::to_string(usd.get_striping_goodness(best_chromosomes)) << std::endl;
}

}  //  namespace topology_solver
