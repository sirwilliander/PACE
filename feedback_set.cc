#include <bits/stdc++.h>

#include "print_template.h"
#include "base_graph.cc"
#include "base_graph.h"

using namespace std;
using namespace lemon;

int main() {
	DirectFeedbackSetProblem<int> Test;
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
	std::cout << "AFTER REDUCING\n";
	Test.PrintGraphInfos();
	//Print_vector(Test.StronglyConnectedSizes());
	/*cout << "HEURISTICS: \n";
	int h_size = Test.LargestVerticesDeletion<int>(&DirectFeedbackSetProblem<int>::InAndOutGoingArcs, 1, 10);
	cout << h_size << endl;
	Test.PrintGraphInfos();*/
	//bool b=Test.solveMIP();
	//bool c=Test.solveMIP2();
	return 0;
}

/*
4 5 0
2 3
3
4
1
*/
