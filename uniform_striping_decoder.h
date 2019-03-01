#include "physical_topology_instance.h"

namespace topology_solver {

class UniformStripingDecoder {
  public:
  	UniformStripingDecoder(const PhysicalTopologyInstance& physical_topology) {
  		initialize(physical_topology);
  	};

  	virtual ~UniformStripingDecoder() {};

  	double decode(const std::vector<double>& chromosome) const;

  	virtual std::vector<Covering> get_striping(const std::vector<double>& chromosome) const;

  	virtual double get_striping_goodness(const std::vector<double>& chromosome) const;

  	virtual std::string to_string() const {
  		return "Uniform striping where we prioritize that the minimum striped switch pairs are maximized";
  	};

  protected:
  	int num_optical_switches_;

  	int num_electrical_switches_;

  	std::vector<uint32_t> electrical_switches_link_budget_;

  	std::vector<uint32_t> optical_switches_link_budget_;

  	virtual void initialize(const PhysicalTopologyInstance& physical_topology_instance);
}

}  //  namespace topology_solver

