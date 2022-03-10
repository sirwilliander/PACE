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

using namespace std;
using namespace lemon;


const bool DEBUG = false; //if(DEBUG)
const long long int INF = 10000000000;

using ll = long long int;

#define all(x) begin(x), end(x)
#define FOR(i,n) for(int i = 0; i < (n); ++i)

template <class C>
void Print_vector(const C &Original) {
	for(const auto &v : Original) {
	    cout << v << " ";
	}
	cout << endl;
}

template <class C>
void Print_Matrix(const vector<vector<C>> &M, bool space = true) {
	for(int i = 0; i < (int)M.size(); ++i) {
		for(int j = 0; j < (int)M[i].size(); ++j) {
			cout << M[i][j]; if(space) cout << " ";            
			}
	    cout << endl;
	}
}

template<class T, class C>
void Print_pair(const pair<T,C> &M) {
    cout << "(" << M.first << " , " << M.second << " ) ";
}

template<class T, class C>
void Print_Matrix_pair(const vector<vector<pair<T,C>>> &M) {
	for(int i = 0; i < (int)M.size(); ++i) {
		cout << i << ": ";
		for(int j = 0; j < (int)M[i].size(); ++j) {
			Print_pair(M[i][j]);          
		}
	    cout << endl;
	}
}

template <typename T>
class DirectFeedbackSetProblem {
	
	private:
		int vertex_numb_;
		T edge_number_;  //Can be very big
		lemon::ListDigraph graph_;
	
	public:
		void AddVertices(int n) {
			FOR(i,n) {
				graph_.addNode();
			}
		}
		
		void ReadInput(istream& in = std::cin) {
			int temp;
			in >> vertex_numb_; in >> edge_number_; in >> temp;
			this->AddVertices(vertex_numb_);
			
			string t; std::getline(in, t); //There is the first endline with \n

			FOR(i,vertex_numb_) {
				string line = "";
				std::getline(std::cin, line);
				if(DEBUG) cout << "line: " << line << endl;
				stringstream sstream(line);
				int adjacent_vert;
				while (sstream >> adjacent_vert){
					if(DEBUG) cout << "Adjacent_vert: " << adjacent_vert << endl;
					graph_.addArc(graph_.nodeFromId(i),graph_.nodeFromId(adjacent_vert-1));
				}
			}
		}
	
		void PrintEdgeList(ostream& os = std::cout) {
			for (ListDigraph::NodeIt n(graph_); n != INVALID; ++n) {
				os << graph_.id(n) << ": ";
				for (ListDigraph::OutArcIt e(graph_, n); e != INVALID; ++e) {
					os << graph_.id(graph_.target(e)) << " ";
				}
				os << endl;
			}
		}
		
		double AdjacentEdgeAverage() {
			return (double)edge_number_/(double)vertex_numb_;
		}
	
		//Usage of printToPdf:
		// ./testDraw | dot -Tpdf > file1.pdf
		void PrintToPdf(ostream& os = std::cout){
			os << "digraph G{" << endl;
			for(ListDigraph::ArcIt arc(graph_);arc!=INVALID;++arc)
			{
				os << graph_.id(graph_.source(arc)) << " -> " << graph_.id(graph_.target(arc)) << ";" << endl;
			}
			os << "}" << endl;
		}
	
};


void AddVerticesToGraph(lemon::ListDigraph &g, int n) {
	FOR(i,n) {
		g.addNode();
	}
}


int main() {
	DirectFeedbackSetProblem<int> Test;
	Test.ReadInput();
	if(DEBUG) cout << "READ\n";
	if(DEBUG) Test.PrintEdgeList();
	cout << "AVERAGE ADJACENT VERTICES: " << Test.AdjacentEdgeAverage() << endl;
	
}


/*
4 5 0
2 3
3
4
1

*/
