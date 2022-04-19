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

using namespace std;
using namespace lemon;

const bool DEBUG = false; //if(DEBUG)
const bool DEBUG_CONDENSED = false;
const long long int INF = 10000000000;

struct Node_pair{
	ListDigraph::Node u;
	ListDigraph::Node v;
};

bool operator <(const Node_pair& x, const Node_pair& y);
void AddVerticesToGraph(lemon::ListDigraph &g, int n);

template <typename T>
class DirectFeedbackSetProblem {
	
	private:
		int original_vertex_numb_;
		int vertex_numb_;
		T arc_numb_;  //Can be very big
		lemon::ListDigraph graph_;
		vector<lemon::ListDigraph::Node> solution_;
		lemon::ListDigraph condensed_graph_; 
		int strongly_connected_num_;
		lemon::ListDigraph::NodeMap<int> strongly_connected_comp_{graph_};   //ERROR azt hiszi függvény declarálok. () helyett {}-et kell tenni https://stackoverflow.com/questions/13734262/c-difference-between-function-declaration-and-object-initialization
		lemon::ListDigraph::ArcMap<bool> strongly_connected_arcs_{graph_, false};
	
	public:
		
		DirectFeedbackSetProblem(istream &is = std::cin);
		void PrintGraphInfos(ostream& os = std::cout);
		void AddVertices(int n);

		template <class C>
		void DeleteVertices(C to_be_deleted);
		
		void ReadInput(istream& in = std::cin);
		void PrintEdgeList(ostream& os = std::cout);
		double AdjacentEdgeAverage();
		int CreateCondensedGraph();
		vector<int> StronglyConnectedSizes();
		bool IsDAG();

		//Usage of printToPdf:
		// ./testDraw | dot -Tpdf > file1.pdf
		void PrintToPdf(bool condensed = false, ostream& os = std::cout);
		
		int DeleteSelfLoops();
		int DeleteParallelArcs();
		int DeleteDummyNodes();
		int DeleteChainingNodes();
		int PetalOne();
		void ReduceGraph();

        // LP based methods
        /*bool SolveMIP();
        bool SolveLP();
        int InAndOutGoingArcs(ListDigraph:: Node v, int caser = 1);
        int LargestVerticesDeletion(F Fitness, int caser = 1, int recalculate = 1);*/
};

#endif