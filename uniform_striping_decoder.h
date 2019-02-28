namespace topology_solver {

class UniformStripingDecoder {
  public:
  	UniformStripingDecoder(const PhysicalTopologyInstance& physical_topology) {};

  	virtual ~UniformStripingDecoder() {};

  	double decode(const std::vector<double>& chromosome) const;

  private:
  	int num_optical_switches_;
  	int num_electrical_switches_;
  	std::vector<uint32_t> electrical_switches_link_budget_;
  	std::vector<uint32_t> optical_switches_link_budget_;
}

}  //  namespace topology_solver

