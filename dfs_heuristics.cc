#include "base_graph.h"


#define all(x) begin(x), end(x)
#define FOR(i,n) for(int i = 0; i < (n); ++i)

template <typename T>
void DirectFeedbackSetProblem<T>::LoopThroughCycle(ListDigraph::Node from, ListDigraph::Node to, lemon::ListDigraph::NodeMap<ListDigraph::Node> &parents, lemon::ListDigraph::NodeMap<int>& heuristic_cycle_count)
{
	++heuristic_cycle_count[from];
	lemon::ListDigraph::Node curr = from;
	while (curr != parents[curr])
	{
		curr = parents[curr];
		++heuristic_cycle_count[curr];
	}
}

template <typename T>
void DirectFeedbackSetProblem<T>::DFSWithLoopHeuristic(lemon::ListDigraph::NodeMap<int>& heuristic_cycle_count) {
	//Does a DFS and if it finds a backward edge then goes through it
	vertex_numb_ = 0;
	for (ListDigraph::NodeIt n(graph_); n != INVALID; ++n) ++vertex_numb_;
	
	
	
	int EL = 0; //Elérési számhoz
    int BSZ = 0;  //Befejezési számhoz
	
	lemon::ListDigraph::NodeMap<int> reach(graph_, -1);
    lemon::ListDigraph::NodeMap<int> processed(graph_, -1);
    lemon::ListDigraph::NodeMap<ListDigraph::Node> parent(graph_, lemon::INVALID);
	
    
     for (lemon::ListDigraph::NodeIt u(graph_); u != lemon::INVALID; ++u)
    {
        if (parent[u] == lemon::INVALID)
        {
            parent[u] = u;
            // cout << "Starting from here: " << graph_.id(u) << endl;
            DepthFirstSearchStep(EL, BSZ, u, reach, parent, processed);
        }
    }
    
    
    for(ListDigraph::ArcIt arc(graph_);arc!=INVALID;++arc) {
		if(BackGoingArc(arc, reach, processed)) {
			/*
			++heuristic_cycle_count[graph_.source(arc)];
			++heuristic_cycle_count[graph_.target(arc)];*/
			LoopThroughCycle(graph_.source(arc), graph_.target(arc), parent, heuristic_cycle_count);
		}
	}
    
}


template <typename T>
void DirectFeedbackSetProblem<T>::DepthFirstSearchStep(int &SZ, int &S, ListDigraph::Node current_node, lemon::ListDigraph::NodeMap<int> &reach,
                                                       lemon::ListDigraph::NodeMap<ListDigraph::Node> &parent, lemon::ListDigraph::NodeMap<int> &processed)
{
    SZ++;
    reach[current_node] = SZ;


    //cout << graph_.id(current_node) << " " << graph_.id(parent[current_node]) << " ";

    for (lemon::ListDigraph::OutArcIt a(graph_, current_node); a != INVALID; ++a)
    {
        if (parent[graph_.target(a)] == lemon::INVALID)
        {
            parent[graph_.target(a)] = current_node;
            DepthFirstSearchStep(SZ, S, graph_.target(a), reach, parent, processed);
        }
        S++;
        processed[current_node] = S;
    }
}

template <typename T>
bool DirectFeedbackSetProblem<T>::BackGoingArc(ListDigraph::Arc &arc, ListDigraph::NodeMap<int> &reach, ListDigraph::NodeMap<int> &processed)
{
    return  reach[graph_.source(arc)] > reach[graph_.target(arc)] && processed[graph_.source(arc)] < processed[graph_.target(arc)];
}

template <typename T>
int DirectFeedbackSetProblem<T>::HeuristicLargestVerticesDeletion(int caser, int recalculate) {
	
	lemon::ListDigraph::NodeMap<int> heuristic_cycle_count(graph_, 0);
	DFSWithLoopHeuristic(heuristic_cycle_count);
	for (lemon::ListDigraph::NodeIt u(graph_); u != lemon::INVALID; ++u) {
		cout << heuristic_cycle_count[u] << " ";
	}
	cout << endl;
	//cout << "HERE 1:" << endl;
	int solution_size = 0;
	while(!IsDAG()) {
		vector<pair<int, ListDigraph::Node>> fitnesses;
		for (ListDigraph::NodeIt n(graph_); n != INVALID; ++n) {
			fitnesses.push_back(make_pair(heuristic_cycle_count[n], n));
		}
		//std::transform(ListDigraph::NodeIt (graph_), INVALID, std::back_inserter(fitnesses), [&](ListDigraph::Node v){return make_pair((this->*Fitness)(v, caser), v);} );
		std::sort(all(fitnesses), [](pair<int, ListDigraph::Node> &struct1, pair<int, ListDigraph::Node> &struct2){return (struct1.first > struct2.first);});
		FOR(i,recalculate) {
			solution_size++;
			graph_.erase(fitnesses[i].second);
		}
	}
	
	
	return solution_size;
}

/*
template <typename T, typename V, typename F>
int DirectFeedbackSetProblem<T>::LargestVerticesDeletion(F Fitness, int caser = 1, int recalculate = 1) {
	// Fitness has two input Node and int
	//	Will delete vertices ordered by their fitnesses until the graph is acyclic.
	//	Recalculate the fitness after every "recalculate" number of vertices has been deleted
	// The fitnesses elements has type V

	//auto& Fitness = InAndOutGoingArcs; //Currently only works on this function
	while(!IsDAG()) {
		vector<pair<V, ListDigraph::Node>> fitnesses;
		for (ListDigraph::NodeIt n(graph_); n != INVALID; ++n) {
			fitnesses.push_back(make_pair((this->*Fitness)(n, caser), n));
		}
		//std::transform(ListDigraph::NodeIt (graph_), INVALID, std::back_inserter(fitnesses), [&](ListDigraph::Node v){return make_pair((this->*Fitness)(v, caser), v);} );
		std::sort(all(fitnesses), [](pair<V, ListDigraph::Node> &struct1, pair<V, ListDigraph::Node> &struct2){return (struct1.first > struct2.first);});
		FOR(i,recalculate) {
			solution_[fitnesses[i].second]=1;
			solution_size_++;
			graph_.erase(fitnesses[i].second);
		}
	}

	return solution_size_;
}*/
