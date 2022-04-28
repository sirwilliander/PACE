#include <bits/stdc++.h>

#include "print_template.h"
#include "base_graph.cc"
#include "base_graph.h"
#include "lp_methods.cc"

#include <lemon/time_measure.h>


using namespace std;
using namespace lemon;

template <typename T>
void testGraph(DirectFeedbackSetProblem<T> &Test);

int main(int argc, char *argv[]) {
	if (argc == 1) {
		DirectFeedbackSetProblem<int> Test;
		testGraph(Test);
	}

	if (argc > 1) {
		cout << "\nTesting " << argc-1 << " instance(s).\n\n";
		// 
		lemon::Timer t(1);
		for (int i = 1; i <= argc-1; ++i) {
			DirectFeedbackSetProblem<int> Test(argv[i]);
			testGraph(Test);
		}	
		std::cout << endl << argc-1 << " instances were tested, in " << t.userTime() << "s" << std::endl;
	}
	return 0;
}

template <typename T>
void testGraph(DirectFeedbackSetProblem<T> &Test)
{
		/*if(DEBUG) cout << "READ\n";
		if(DEBUG) Test.PrintEdgeList();
		if(DEBUG) cout << "AVERAGE ADJACENT VERTICES: " << Test.AdjacentEdgeAverage() << endl;
		cout << "CONDENSED GRAPH HAS: " << Test.CreateCondensedGraph() << " vertices\n";
		vector<int> st_conn_sizes = Test.StronglyConnectedSizes();
		Print_vector(st_conn_sizes);
		cout << "Largest component: " << *max_element(all(st_conn_sizes)) << endl;*/
		//Test.PrintToPdf(true);
		Test.PrintGraphInfos();
		Test.ReduceGraph();
		cout << "AFTER REDUCING\n";
		Test.PrintGraphInfos();
		//Print_vector(Test.StronglyConnectedSizes());
		/*cout << "HEURISTICS: \n";
		int h_size = Test.LargestVerticesDeletion<int>(&DirectFeedbackSetProblem<int>::InAndOutGoingArcs, 1, 10);
		cout << h_size << endl;
		Test.PrintGraphInfos();*/
		//Test.SolveMIP();
		//Test.ImproveSolution();
		//bool b=Test.SolveMIP();
		//Test.SolveMIPSCC();
		Test.SolveLP();
}

/*
4 6 0
2 3
3 2
4
1
*/
