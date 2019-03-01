#include "uniform_striping_decoder.h"

namespace topology_solver {

double UniformStripingDecoder::decode(const std::vector<double> & chromosome) const {
	// Penalize constraint violation
	const double large_number = 10E10;
	std::vector<int> es_coverings(num_electrical_switches_, 0);
	std::vector<int> os_coverings(num_optical_switches_, 0);
	for (const auto& covering : coverings) {
		const uint32_t os_id = covering.id;
		for (const auto& entry : covering.striping) {
			es_coverings[entry]++;
		}
		if (covering.striping.size() > optical_switches_link_budget_[os_id]) {
			return -large_number;
		}
	}
	// check the es_coverings
	for (int i = 0; i < num_electrical_switches_; i++) {
		if (es_coverings[i] > electrical_switches_link_budge_[i]) {
			return -large_number;
		}
	}
	// find min
	int min = 10E5;
	for (const auto& es : es_coverings) {
		min = std::min(es, min);
	}
	// assuming that we 
	return min;
}

// TODO(mteh) : finish this
std::vector<Covering> 
UniformStripingDecoder::get_striping(const std::vector<double>& chromosome) const {
	std::vector<Covering> soln(num_optical_switches_);
	return soln;
}

double get_striping_goodness(const std::vector<double>& chromosome) const {
	double striping_goodness = 0;
	return striping_goodness;
}

void UniformStripingDecoder::initialize(const PhysicalTopologyInstance& physical_topology_instance) {
	num_optical_switches_ = physical_topology_instance.get_num_coverings();
	num_electrical_switches_ = physical_topology_instance.get_num_nodes();
	electrical_switches_link_budget_.resize(num_electrical_switches_);
	optical_switches_link_budget_.resize(num_optical_switches_);
	for (int i = 0; i < num_electrical_switches; i++) {
		electrical_switches_link_budget_[i] = physical_topology_instance.get_node_link_counts(i);
	}
	for (int i = 0; i < num_optical_switches_; i++) {
		optical_switches_link_budget_[i] = physical_topology_instance.get_covering_link_counts(i);
	}
}

}  // namespace topology_solver