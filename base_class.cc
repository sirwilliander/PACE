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
		lemon::ListDigraph g;
		vector<lemon::ListDigraph::Node> Vertices_;	
	
	public:
		void AddVertices(int n, bool add_to_memo = false) {
			FOR(i,n) {
				if(add_to_memo) Vertices_.push_back(g.addNode());
				else g.addNode();
			}
		}
		
		void ReadInput(istream& in = std::cin, bool from_memo = false) {
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
					if(from_memo) g.addArc(Vertices_[i],Vertices_[adjacent_vert-1]);
					else g.addArc(g.nodeFromId(i),g.nodeFromId(adjacent_vert-1));
				}
			}
		}
	
		void PrintEdgeList(ostream& os = std::cout, bool from_memo = false) {
			
			if(from_memo)
				FOR(i,vertex_numb_) {
					os << i << ": ";
					for (ListDigraph::OutArcIt e(g, Vertices_[i]); e != INVALID; ++e) {
						os << g.id(g.target(e)) << " ";
					}
					os << endl;
				}
			else {
				for (ListDigraph::NodeIt n(g); n != INVALID; ++n) {
					os << g.id(n) << ": ";
					for (ListDigraph::OutArcIt e(g, n); e != INVALID; ++e) {
						os << g.id(g.target(e)) << " ";
					}
					os << endl;
				}
			}
		}
		
		double AdjacentEdgeAverage() {
			return (double)edge_number_/(double)vertex_numb_;
		}
		
		//Usage of printToPdf:
		// ./testDraw | dot -Tpdf > file1.pdf
		void PrintToPdf(ostream& os = std::cout){
			os << "digraph G{" << endl;
			for(ListDigraph::ArcIt arc(g);arc!=INVALID;++arc)
			{
				os << g.id(g.source(arc)) << " -> " << g.id(g.target(arc)) << ";" << endl;
			}
			os << "}" << endl;
		}

};


int main() {
	DirectFeedbackSetProblem<int> Test;
	Test.ReadInput();
	/*if(DEBUG) cout << "READ\n";
	Test.PrintEdgeList();
	cout << "AVERAGE ADJACENT VERTICES: " << Test.AdjacentEdgeAverage() << endl;*/
	Test.PrintToPdf();
}


/*
4 5 0
2 3
3
4
1

*/
