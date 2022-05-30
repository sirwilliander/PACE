#ifndef _DFS_BASE_GRAPH_H
#define _DFS_BASE_GRAPH_H

#include <bits/stdc++.h>
#include <lemon/list_graph.h>
#include <lemon/lgf_reader.h>
#include <lemon/lgf_writer.h>
#include <lemon/graph_to_eps.h>
#include <lemon/time_measure.h>
#include <lemon/arg_parser.h>
#include <lemon/dijkstra.h>
#include <lemon/adaptors.h>
#include <lemon/concepts/digraph.h>
#include <lemon/concepts/path.h>
#include <lemon/connectivity.h>
#include <tuple>
#include <set>
#include <lemon/preflow.h>
#include <lemon/lp.h>

#include "setcover_bc.cc"

using namespace std;
using namespace lemon;

const bool DEBUG = false; //if(DEBUG)
const bool DEBUG_CONDENSED = false;
const long long int INF = 10000000000;

struct Node_pair
{
	ListDigraph::Node u;
	ListDigraph::Node v;
};

bool operator<(const Node_pair &x, const Node_pair &y);
void AddVerticesToGraph(lemon::ListDigraph &g, int n);

template <typename T>
class DirectFeedbackSetProblem
{

private:
	int original_vertex_numb_;
	int vertex_numb_;

	T arc_numb_; //Can be very big
	lemon::ListDigraph graph_;
	vector<lemon::ListDigraph::Node> solution_;
	lemon::ListDigraph condensed_graph_;
	int strongly_connected_num_;
	lemon::ListDigraph::NodeMap<int> strongly_connected_comp_{graph_};
	lemon::ListDigraph::ArcMap<bool> strongly_connected_arcs_{graph_, false};
	vector<vector<ListDigraph::Node>> cycles_to_dodge_;
	SetCovering set_cover_solver_;

public:
	DirectFeedbackSetProblem(istream &is = std::cin);
	DirectFeedbackSetProblem(string file_name);

	void AddVertices(int n);

	template <class C>
	void DeleteVertices(C to_be_deleted);

	// Read + Print
	void ReadInput(istream &in = std::cin);
	void ReadFromFile(string file_name);
	void PrintGraphInfos(ostream &os = std::cout);
	void PrintEdgeList(ostream &os = std::cout);
	//Usage of printToPdf:
	// ./testDraw | dot -Tpdf > file1.pdf
	void PrintToPdf(bool condensed = false, ostream &os = std::cout);

	double AdjacentEdgeAverage();
	int CreateCondensedGraph();
	vector<int> StronglyConnectedSizes();
	bool IsDAG();
	int InAndOutGoingArcs(ListDigraph::Node v, int caser = 1);

	int DeleteSelfLoops();
	int DeleteParallelArcs();
	int DeleteDummyNodes();
	int DeleteChainingNodes();
	int PetalOne();
	void ReduceGraph();

	// LP based methods
	bool SolveMIP();
	bool SolveLP2();
	bool SolveLP();
	bool SolveMIP2();
	bool SolveMIPSCC();
	// int LargestVerticesDeletion(F Fitness, int caser = 1, int recalculate = 1);

	// Cutting plane methods
	bool SolveIPWithoutCycles();
	bool CycleWithDepthFirstSearch();
	bool CycleWithDepthFirstSearch(ListDigraph::NodeMap<bool> &VerticesIn);
	bool DepthFirstSearchStep(int &SZ, int &S, int M, ListDigraph::Node current_node, ListDigraph::NodeMap<int> &eleresi_szam,
							  ListDigraph::NodeMap<ListDigraph::Node> &parent, ListDigraph::NodeMap<int> &befejezesi_szam,
							  ListDigraph::NodeMap<bool> &VerticesIn, ListDigraph::NodeMap<int> &melyseg);
	bool BackGoingArc(ListDigraph::Arc &arc, ListDigraph::NodeMap<int> &eleresi_szam, ListDigraph::NodeMap<int> &befejezesi_szam,
					  ListDigraph::NodeMap<bool> &VerticesIn);
	bool AddCycle(ListDigraph::Node &node_from, ListDigraph::Node &back_edge_target,
				  ListDigraph::NodeMap<ListDigraph::Node> &parent, ListDigraph::NodeMap<bool> &ready);
	void FindBackAndForthEdges();
	void Find3LongPaths();
	int rand_numb;

	void BuildSetCoveringModel(int max_iteration);
	void SolveSetCover();


};

#endif
