#ifndef UNIFORMSTRIPINGDECODER
#define UNIFORMSTRIPINGDECODER

#include <iostream>
#include "physical_topology_instance.h"
#include "mincost.h"

namespace topology_solver {

class UniformStripingDecoder {
  public:
  	UniformStripingDecoder(const PhysicalTopologyInstance& physical_topology) {
  		initialize(physical_topology);
  	};

  	virtual ~UniformStripingDecoder() {};

  	double decode(const std::vector<double>& chromosome) const;

  	virtual double get_striping_goodness(const std::vector<double>& chromosome) const;

  	virtual std::string to_string() const {
  		return "Uniform striping where we prioritize that the minimum striped switch pairs are maximized";
  	};

  	virtual std::vector<Covering> transform_chromosome_to_coverings(const std::vector<double>& chromosome) const; 
  
  protected:
  	int num_optical_switches_;

  	int num_electrical_switches_;

  	std::vector<uint32_t> electrical_switches_link_budget_;

  	std::vector<uint32_t> optical_switches_link_budget_;

  	virtual void initialize(const PhysicalTopologyInstance& physical_topology_instance);
  
  private:
  	std::vector<Covering> transform_chromosome_to_coverings_greedy(const std::vector<double>& chromosome) const;
};

}  //  namespace topology_solver

#endif
