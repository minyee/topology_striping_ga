#include "uniform_striping_decoder.h"


namespace topology_solver {
using std::pair;
double 
UniformStripingDecoder::decode(const std::vector<double> & chromosome) const {
	// Penalize constraint violation
	const double large_number = 10E10;	
	std::vector<int> es_coverings(num_electrical_switches_, 0);
	std::vector<int> os_coverings(num_optical_switches_, 0);
	std::vector<Covering> coverings = 
		transform_chromosome_to_coverings_greedy(chromosome);
	for (int i = 0; i < num_electrical_switches_; i++) {
		if (es_coverings[i] > electrical_switches_link_budget_[i]) {
			return -large_number;
		}
	}	
	for (const auto& covering : coverings) {
		for (const auto& entry : covering.striping) {
			es_coverings[entry]++;
		}
	}
	int min = 10E5;
	for (const auto& es : es_coverings) {
		min = std::min(es, min);
	}	
	std::cout << "min is: " << min << std::endl;
	return 1/min;
}

double 
UniformStripingDecoder::get_striping_goodness(const std::vector<double>& chromosome) const {
	double striping_goodness = 0;
	return striping_goodness;
}

void 
UniformStripingDecoder::initialize(const PhysicalTopologyInstance& physical_topology_instance) {
	num_optical_switches_ = physical_topology_instance.get_num_coverings();
	num_electrical_switches_ = physical_topology_instance.get_num_nodes();
	electrical_switches_link_budget_.resize(num_electrical_switches_);
	optical_switches_link_budget_.resize(num_optical_switches_);
	for (int i = 0; i < num_electrical_switches_; i++) {
		electrical_switches_link_budget_[i] = physical_topology_instance.get_node_link_counts(i);
	}
	for (int i = 0; i < num_optical_switches_; i++) {
		optical_switches_link_budget_[i] = physical_topology_instance.get_covering_link_counts(i);
	}
}

void minitrial() {
	const int src = 0;
	const int dst = 2;
	const int intermediate = 1;

	mincost::Graph g, residual;
	g.adj = new std::vector<mincost::Edge*>[3];
	residual.adj = new std::vector<mincost::Edge*>[3];

	
	// connect src with intermediate
	mincost::Edge* tmp_edge1 = mincost::genEdge(intermediate, 1, 0, 0);
	mincost::Edge* tmp_edge2 = mincost::genEdge(src, 1, 0, 0);
	tmp_edge1->counterEdge = tmp_edge2;
	tmp_edge2->counterEdge = tmp_edge1;
	g.adj[src].push_back(tmp_edge1);
	residual.adj[src].push_back(tmp_edge1);
	residual.adj[intermediate].push_back(tmp_edge2);

	// connect intermediate with dst
	mincost::Edge* tmp_edge3 = mincost::genEdge(dst, 1, 0, 0);
	mincost::Edge* tmp_edge4 = mincost::genEdge(intermediate, 1, 0, 0);
	tmp_edge3->counterEdge = tmp_edge4;
	tmp_edge4->counterEdge = tmp_edge3;
	g.adj[intermediate].push_back(tmp_edge3);
	residual.adj[intermediate].push_back(tmp_edge3);
	residual.adj[dst].push_back(tmp_edge4);

	const double cost = mincost::calcMinCostFlow(g, residual, src, dst, 1);
	std::cout << "cost from minitrial is: " << cost << std::endl;
}

bool sortbypairdescending(const std::pair<int, double>& a, const std::pair<int, double>& b) {
	return a.second > b.second;
}

std::vector<Covering>
UniformStripingDecoder::transform_chromosome_to_coverings_greedy(const std::vector<double>& chromosomes) const {
	std::vector<Covering> soln(num_optical_switches_);
	std::vector<int> optical_switch_leftover(num_optical_switches_);
	
	//Initialize leftover links for optical and electrical switches first
	for (int i = 0; i < num_optical_switches_; i++) {
		soln[i].id = i;
		optical_switch_leftover[i] = optical_switches_link_budget_[i];
	}


	for (int elec = 0; elec < num_electrical_switches_; elec++) {
		int leftover_elec_links = electrical_switches_link_budget_[elec];
		std::vector< std::pair<int, double> > scores(num_optical_switches_);
		int offset = 0;
		for (int optical = 0; optical < num_optical_switches_; optical++) {
			scores[optical] = std::make_pair(optical, chromosomes[offset + elec]);
			offset += optical_switches_link_budget_[optical];
		}
		std::sort(scores.begin(), scores.end(), sortbypairdescending);
		for (int i = 0; i < scores.size(); i++) {
			if (leftover_elec_links <= 0) {
				break;
			}
			if (optical_switch_leftover[std::get<0>(scores[i])] > 0) {
				soln[std::get<0>(scores[i])].striping.insert(elec);
				optical_switch_leftover[i]--;				
				leftover_elec_links--;
			}
		}
	}	
	for (int i = 0; i < num_optical_switches_; i++) {
		std::cout << "Optical switch : " << i << " has " << optical_switch_leftover[i] << " links leftover" << std::endl; 
	}
	return soln;
}

std::vector<Covering> 
UniformStripingDecoder::transform_chromosome_to_coverings(const std::vector<double>& chromosomes) const {
	std::vector<Covering> covering_soln(num_optical_switches_);
	// Solve the chromosomes in order to get the overall covering in the topology
	int node_id = 0;
	const int src = 0;
	const int dst = 2 + num_electrical_switches_ + num_optical_switches_ - 1;
	mincost::Graph g, residual;
	g.numVertices = 2 + num_electrical_switches_ + num_optical_switches_;
	residual.numVertices = 2 + num_electrical_switches_ + num_optical_switches_;
	g.adj = new std::vector<mincost::Edge*>[g.numVertices];
	residual.adj = new std::vector<mincost::Edge*>[residual.numVertices];
	// Generate the left half of the nodes in the bipartite graph, which are the electrical switches
	for (node_id = 1; node_id <= num_electrical_switches_; node_id++) {		
		mincost::Edge* tmp_edge1 = mincost::genEdge(node_id, electrical_switches_link_budget_[node_id - 1], electrical_switches_link_budget_[node_id - 1], 0);
		mincost::Edge* tmp_edge2 = mincost::genEdge(src, electrical_switches_link_budget_[node_id - 1], electrical_switches_link_budget_[node_id - 1], 0);
		tmp_edge1->counterEdge = tmp_edge2;
		tmp_edge2->counterEdge = tmp_edge1;
		g.adj[src].push_back(tmp_edge1);
		residual.adj[src].push_back(tmp_edge1);
		residual.adj[node_id].push_back(tmp_edge2);
	}

	// Generate the right half of the nodes in the bipartite graph, which are now the optical switches
	for (node_id = num_electrical_switches_ + 1; node_id <= num_electrical_switches_ + num_optical_switches_; node_id++) {
		const uint32_t ocs_id = node_id - num_electrical_switches_ - 1;
		for (int elec_id = 1; elec_id <= num_electrical_switches_; elec_id++) {
			// TODO (mteh) : figure out the how to retrieve allele value here
			const double score = chromosomes[ocs_id * num_electrical_switches_ + (elec_id - 1)];
			mincost::Edge* tmp_edge1 = mincost::genEdge(elec_id, 1, 1, score);
			// TODO (mteh) : figure out the how to retrieve allele value here
			mincost::Edge* tmp_edge2 = mincost::genEdge(node_id, 1, 1, -score);  
			tmp_edge1->counterEdge = tmp_edge2;
			tmp_edge2->counterEdge = tmp_edge1;
			g.adj[src].push_back(tmp_edge1);
			residual.adj[node_id].push_back(tmp_edge1);
			residual.adj[elec_id].push_back(tmp_edge2);	
		}
		mincost::Edge* tmp_edge3 = mincost::genEdge(node_id, optical_switches_link_budget_[node_id - num_electrical_switches_ - 1], 
										optical_switches_link_budget_[node_id - num_electrical_switches_ - 1], 0); // the forward cost is the value of chromosome
		mincost::Edge* tmp_edge4 = mincost::genEdge(dst, optical_switches_link_budget_[node_id - num_electrical_switches_ - 1], 
										optical_switches_link_budget_[node_id - num_electrical_switches_ - 1], 0);
		tmp_edge3->counterEdge = tmp_edge4;
		tmp_edge4->counterEdge = tmp_edge3;
		g.adj[node_id].push_back(tmp_edge3);
		residual.adj[node_id].push_back(tmp_edge3);
		residual.adj[dst].push_back(tmp_edge4);	
	}

	// now, start running the algorithm and check the results, if valid, then read the chromosomes into solution
	int total_flow = 0;
	for (int i = 0; i < num_optical_switches_; i++) {
		total_flow += optical_switches_link_budget_[i];
	}
	const double cost = mincost::calcMinCostFlow(g, residual, src, dst, total_flow);
	if (cost > 1000000) {
		std::cerr << "Invalid solution from mincost flow solver, cost is " << cost << std::endl;
	} else {
		// try to read the bipartite graph's flow
		// for all the arcs that connect the left side and right side vertices in the 
		// bipartite flow graph, read the entries that have flow
		for (int i = 0; i < num_optical_switches_; i++) {
			covering_soln[i].id = i;
		}
		for (int i = 1; i <= num_electrical_switches_; i++) {
			for (int j = 0; j < g.adj[i].size(); j++) {
				if (g.adj[i][j]->capacity - g.adj[i][j]->residualFlow >= 1) {
					const uint32_t ocs_id = g.adj[i][j]->destination - num_electrical_switches_ - 1;	
					const uint32_t eps_id = i - 1;
					covering_soln[ocs_id].striping.insert(eps_id);
				}
			}
		}
	}
	delete g.adj;
	delete residual.adj;
	return covering_soln;
}

}  // namespace topology_solver
