#ifndef PHYSICALTOPOLOGYINSTANCE
#define PHYSICALTOPOLOGYINSTANCE
#include <iostream>
#include <vector>
#include <set>
//  Problem instance for a topology striping problem
namespace topology_solver {

struct Covering {
	uint32_t id;
	std::set<uint32_t> striping;
};


// class that defines the physical constraints of the problem
class PhysicalTopologyInstance {
  public:
  	PhysicalTopologyInstance() {};
  
  	virtual ~PhysicalTopologyInstance() {};

  	uint32_t add_node() {
  		node_link_counts_.push_back(0);
  		return node_link_counts_.size() - 1;
  	};

  	void set_node_links(const uint32_t id, const uint32_t link_count) {
  		if (id < node_link_counts_.size()) {
  			node_link_counts_[id] = link_count;
  		}
  	};

  	// adds a covering
  	uint32_t add_covering() {
  		covering_link_counts_.push_back(0);
  		return covering_link_counts_.size() - 1;
  	};

  	void set_covering_links(const uint32_t id, const uint32_t link_count) {
  		if (id < covering_link_counts_.size()) {
  			covering_link_counts_[id] = link_count;
  		}
  	};

  	uint32_t get_num_nodes() const {
  		return node_link_counts_.size();
  	};

  	uint32_t get_num_coverings() const {
  		return covering_link_counts_.size();
  	}

  	uint32_t get_node_link_counts(const uint32_t id) const {
  		return node_link_counts_[id];
  	}

  	uint32_t get_covering_link_counts(const uint32_t id) const {
  		return covering_link_counts_[id];
  	}

  	void print_params() const {
  		std::cout << "Number of nodes: " << node_link_counts_.size() << std::endl << "lc: ";
  		for (const auto lc : node_link_counts_) {
  			std::cout << lc << ",  ";
  		}
  		std::cout << std::endl;
  		std::cout << "Number of coverings: " << covering_link_counts_.size() << std::endl << "lc: ";
		for (const auto lc : covering_link_counts_) {
  			std::cout << lc << ",  ";
  		}  		
  	}
  private:
  	std::vector<uint32_t> node_link_counts_;

  	std::vector<uint32_t> covering_link_counts_;

};

}  // namespace topology_solver

#endif
