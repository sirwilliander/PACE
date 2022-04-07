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

struct Node_pair{
	ListDigraph::Node u;
	ListDigraph::Node v;
};

bool operator <(const Node_pair& x, const Node_pair& y){
		return tie(x.u,x.v) < tie(y.u,y.v);
	}


template <typename T>
struct less_than_key
{
    inline bool operator() (const pair<T, ListDigraph::Node>& struct1, const pair<T, ListDigraph::Node>& struct2)
    {
        return (struct1.first > struct2.first);
    }
};

void AddVerticesToGraph(lemon::ListDigraph &g, int n);

template <typename T>
class DirectFeedbackSetProblem {

	private:
		int original_vertex_numb_;
		int vertex_numb_;
		T arc_numb_;  //Can be very big
		lemon::ListDigraph graph_;
		lemon::ListDigraph::NodeMap<bool> solution_{graph_};
		int solutionSize_;
		lemon::ListDigraph condensed_graph_;
		int strongly_connected_num_;
		lemon::ListDigraph::NodeMap<int> strongly_connected_comp_{graph_};   //ERROR azt hiszi fĂźggvĂŠny declarĂĄlok. () helyett {}-et kell tenni https://stackoverflow.com/questions/13734262/c-difference-between-function-declaration-and-object-initialization
		lemon::ListDigraph::ArcMap<bool> strongly_connected_arcs_{graph_, false};

	public:

		DirectFeedbackSetProblem(istream &is = std::cin) {
			ReadInput(is);
			original_vertex_numb_ = vertex_numb_;
			solutionSize_=0;
			strongly_connected_num_ = 0;
		}



		void PrintGraphInfos(ostream& os = std::cout) {
			vertex_numb_ = 0;
			for (ListDigraph::NodeIt n(graph_); n != INVALID; ++n) ++vertex_numb_;
			os << "VERTEX NUMBER: " << vertex_numb_ << endl;
			arc_numb_ = 0;
			for(ListDigraph::ArcIt arc(graph_);arc!=INVALID;++arc) ++arc_numb_;
			os << "ARC NUMBER: " << arc_numb_ << endl;
			os << "SOL and VERTEX NUMBER: " << (double)solutionSize_/original_vertex_numb_ << endl;
		}

		void AddVertices(int n) {
			FOR(i,n) {
				graph_.addNode();
			}
		}

		template <class C>
		void DeleteVertices(C to_be_deleted) {
			for(ListDigraph::Node &v : to_be_deleted) {
				graph_.erase(v);
			}
		}

		void ReadInput(istream& in = std::cin) {
			int temp;
			in >> vertex_numb_; in >> arc_numb_; in >> temp;
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
			return (double)arc_numb_/(double)vertex_numb_;
		}

		int CreateCondensedGraph() {
			strongly_connected_num_ = lemon::stronglyConnectedComponents(graph_, strongly_connected_comp_);
			AddVerticesToGraph(condensed_graph_, strongly_connected_num_);
			lemon::stronglyConnectedCutArcs(graph_, strongly_connected_arcs_);
			for(ListDigraph::ArcIt arc(graph_);arc!=INVALID;++arc) {
				if(strongly_connected_arcs_[arc]) {
					condensed_graph_.addArc(
						condensed_graph_.nodeFromId( strongly_connected_comp_[graph_.source(arc)] ),
						condensed_graph_.nodeFromId( strongly_connected_comp_[graph_.target(arc)]));
				}
			}

			return strongly_connected_num_;
		}

		vector<int> StronglyConnectedSizes() {
			if(strongly_connected_num_ == 0) CreateCondensedGraph();
			vector<int> strongly_conn_sizes(strongly_connected_num_, 0);
			for (ListDigraph::NodeIt n(graph_); n != INVALID; ++n) {
				++strongly_conn_sizes[strongly_connected_comp_[n]];
			}

			return strongly_conn_sizes;
		}

		bool IsDAG() {
			return lemon::dag(graph_);
		}

		//Usage of printToPdf:
		// ./testDraw | dot -Tpdf > file1.pdf
		void PrintToPdf(bool condensed = false, ostream& os = std::cout) {
			os << "digraph G{" << endl;
			if(!condensed) {
				for(ListDigraph::ArcIt arc(graph_);arc!=INVALID;++arc)
				{
					os << graph_.id(graph_.source(arc)) << " -> " << graph_.id(graph_.target(arc)) << ";" << endl;
				}
				os << "}" << endl;
			}
			else {
				for(ListDigraph::ArcIt arc(condensed_graph_);arc!=INVALID;++arc)
				{
					os << condensed_graph_.id(condensed_graph_.source(arc)) << " -> " << condensed_graph_.id(condensed_graph_.target(arc)) << ";" << endl;
				}
				os << "}" << endl;
			}
		}

		int DeleteSelfLoops() {
			int i = 0;
			set<int> toBeDeleted;
			for(ListDigraph::ArcIt arc(graph_);arc!=INVALID;++arc) {
				if(graph_.source(arc)==graph_.target(arc)){
					toBeDeleted.insert(graph_.id(graph_.source(arc)));
					--arc_numb_;
				}
			}
			for(set<int>::iterator it=toBeDeleted.begin(); it!=toBeDeleted.end(); ++it) {
				solution_[graph_.nodeFromId(*it)]=1;
				solutionSize_++;
				graph_.erase(graph_.nodeFromId(*it));
				--vertex_numb_;
				++i;
			}

			return i;
		}

		int DeleteParallelArcs() {
			int i = 0;
			map<Node_pair, int> m;
			Node_pair np;

			for(ListDigraph::ArcIt arc(graph_);arc!=INVALID;++arc){
					np.u=graph_.source(arc);
					np.v=graph_.target(arc);
					++m[np];
					if(m[np]>=2){
						graph_.erase(arc);
						--arc_numb_;
						++i;
					}
				}

			return i;
		}

		int DeleteDummyNodes() {
			//Nodes without ingoing or outgoing arcs
			vector<ListDigraph::Node> to_be_deleted;

			for (ListDigraph::NodeIt n(graph_); n != INVALID; ++n) {
				int out = 0;
				for (ListDigraph::OutArcIt a(graph_, n); a != INVALID; ++a) {
					++out;
				}

				int in = 0;
				for (ListDigraph::InArcIt a(graph_, n); a != INVALID; ++a) {
					++in;
				}

				if(!in || !out) {
					to_be_deleted.push_back(n);
					--vertex_numb_;
					arc_numb_ -= (in+out);
				}
			}

			DeleteVertices(to_be_deleted);

			return to_be_deleted.size();
		}

		int DeleteChainingNodes() {
			//Nodes with only one ingoing and one outgoing edges
			vector<ListDigraph::Node> to_be_deleted;

			for (ListDigraph::NodeIt n(graph_); n != INVALID; ++n) {
				int out = 0;
				for (ListDigraph::OutArcIt a(graph_, n); a != INVALID; ++a) {
					++out;
				}

				int in = 0;
				for (ListDigraph::InArcIt a(graph_, n); a != INVALID; ++a) {
					++in;
				}

				if(in == 1 && in == out) {
					to_be_deleted.push_back(n);
					--vertex_numb_;
					--arc_numb_;
				}
			}

			for(ListDigraph::Node &v : to_be_deleted) {
				ListDigraph::Arc out_arc = ListDigraph::OutArcIt (graph_, v);
				ListDigraph::Arc in_arc = ListDigraph::InArcIt (graph_, v);

				ListDigraph::Node u_next = graph_.target(out_arc);
				ListDigraph::Node u_prev = graph_.source(in_arc);

				graph_.addArc(u_prev, u_next);
				graph_.erase(v);
			}

			return to_be_deleted.size();
		}

		int PetalOne(){
			//Deletion of a node which is contained in exactly one cycle
			int i=0; // If there is a cycle which has only petal=1 nodes, then at least one of them has to stay. So we delete during the for cycle
			bool del=0;
			ListDigraph::Node deletable;
			ListDigraph::ArcMap<int> cap(graph_);
			for (ListDigraph::ArcIt arc(graph_); arc != INVALID; ++arc)
				cap[arc]=1;
			for (ListDigraph::NodeIt node(graph_); node != INVALID; ++node) {
				if(del){
					graph_.erase(deletable);
					del=0;
				}

				ListDigraph::Node u = graph_.split(node, false);

				//One way
				lemon::Preflow< ListDigraph > my_flow1(graph_,cap,u,node);
				my_flow1.init();
				my_flow1.startFirstPhase();
				int flow_value1=my_flow1.flowValue();

				//And the other
				Preflow< ListDigraph > my_flow2(graph_,cap,node,u);
				my_flow2.init();
				my_flow2.startFirstPhase();
				int flow_value2=my_flow2.flowValue();
				graph_.contract(node, u, true);

				if(flow_value1+flow_value2==1){
					int counter1=0;
					int counter2=0;
					for (ListDigraph::InArcIt x(graph_,node); x != INVALID; ++x) {
						++counter1;
						for (ListDigraph::OutArcIt y(graph_,node); y != INVALID; ++y){
							++counter2;
							graph_.addArc(graph_.source(x),graph_.target(y));
						}
					}
					deletable=node;
					del=true;
					++i;
					--vertex_numb_;
					arc_numb_=arc_numb_+counter1*counter2-counter1-counter2;
				}
			}
			if(del)
				graph_.erase(deletable);
			return i;
		}

		void ReduceGraph(){
			bool loop_free = lemon::loopFree(graph_);
			bool parallel_free = lemon::parallelFree(graph_);

			if(!loop_free){
				DeleteSelfLoops();
			}

			if(!parallel_free){
				DeleteParallelArcs();
			}

			while(DeleteDummyNodes() || DeleteChainingNodes());
			//May there be parallel arcs after chaining the nodes?
			PetalOne();

		}

		//LP based methods

    bool solveMIP(){

    lemon::Timer t(1);

    Mip mip;

    const int M = 1000000;

    map<int, Mip::Col> x;
    map<int, Mip::Col> y;
    for(lemon::ListDigraph::NodeIt u(graph_);u!=lemon::INVALID;++u){
      x[graph_.id(u)]=mip.addCol();
      y[graph_.id(u)]=mip.addCol();
      }

    for(lemon::ListDigraph::NodeIt u(graph_);u!=lemon::INVALID;++u){
  	mip.colLowerBound(x[graph_.id(u)],0);
	mip.colUpperBound(x[graph_.id(u)],1);
	mip.colLowerBound(y[graph_.id(u)],0);
	mip.colUpperBound(y[graph_.id(u)],vertex_numb_);
	}

    for(lemon::ListDigraph::NodeIt u(graph_);u!=lemon::INVALID;++u){
      mip.colType(x[graph_.id(u)], Mip::INTEGER);
    	mip.colType(y[graph_.id(u)], Mip::INTEGER);
    }

    for(lemon::ListDigraph::ArcIt a(graph_);a!=lemon::INVALID;++a){
       int i=graph_.id(graph_.source(a));
       int j=graph_.id(graph_.target(a));
       Mip::Expr expr=y[i]-y[j]+1-(1-x[i])*M -(1-x[j])*M;

     	mip.addRow(expr<=0);
     }

        Mip::Expr maxNum;
    for(lemon::ListDigraph::NodeIt u(graph_);u!=lemon::INVALID;++u){
      maxNum+=x[graph_.id(u)];
      }

	mip.max();
	mip.obj(maxNum);
	mip.solve();


      std::cout << "IP done in " << t.userTime() << "s" << std::endl;
  // Print the results
  if (mip.type() == Mip::OPTIMAL|| mip.type() == Mip::FEASIBLE) {
    cout << "Legkevesebb pont amit tĂśrĂślni kell: " << vertex_numb_-mip.solValue() << endl;

  }
   else {
    std::cout << "Optimal solution not found." << std::endl;
  }

    return mip.type() == Mip::OPTIMAL || mip.type() == Mip::FEASIBLE;
  }

  //Executes trivial improving
  bool ImproveSolution(){

     /* ListDigraph
      NodeMap<> */

      /*
    int counter=0;
    for(ListDigraph::NodeIt n(graph_); n!=INVALID;++n){
        if(solution_[n]==1){
            solution_[n]=0;
            lemon::ListDigraph::NodeMap<bool> subgraphNode(graph_);
            lemon::ListDigraph::ArcMap<bool> subgraphArc(graph_,false);
            for(ListDigraph::NodeIt n1(graph_); n1!=INVALID ;++n1)
                subgraphNode[n1]=1-solution_[n1];
            for(ListDigraph::ArcIt n1(graph_); n1!=INVALID ;++n1){
                if(subgraphNode[graph_.source(n1)]&&subgraphNode[graph_.target(n1)]){
                    subgraphArc[n1]=1;
                }
            }

            SubGraph< ListDigraph >
               ::FilterNodes<lemon::ListDigraph::NodeMap<bool> >
               ::FilterEdges<lemon::ListDigraph::ArcMap<bool> >
               ::Create
             H(graph_,subgraphNode,subgraphArc);
            if(dag(H)){
            solutionSize_--;
                counter++;
                continue;
                }
            else
            	solution_[n]=1;
        }
      cout<<"Could put back: "<<counter<<endl;
  }
  	if(counter>0)
  		return true;
  	return false;*/
  }

		//Heuristic Algorithms

		int InAndOutGoingArcs(ListDigraph:: Node v, int caser = 1) {
			int out = 0;
			for (ListDigraph::OutArcIt a(graph_, v); a != INVALID; ++a) {
				++out;
			}

			int in = 0;
			for (ListDigraph::InArcIt a(graph_, v); a != INVALID; ++a) {
				++in;
			}

			switch(caser) {
				case 1: return out+in; break;
				case 2: return in; break;
				case 3: return out;
			}
		}

		template <typename V, typename F>
		int LargestVerticesDeletion(F Fitness, int caser = 1, int recalculate = 1) {
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
					solutionSize_++;
					graph_.erase(fitnesses[i].second);
				}
			}

			return solutionSize_;
		}
};


void AddVerticesToGraph(lemon::ListDigraph &g, int n) {
	FOR(i,n) {
		g.addNode();
	}
}


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
	cout << "AFTER REDUCING\n";
	Test.PrintGraphInfos();
	//Print_vector(Test.StronglyConnectedSizes());
	/*cout << "HEURISTICS: \n";
	int h_size = Test.LargestVerticesDeletion<int>(&DirectFeedbackSetProblem<int>::InAndOutGoingArcs, 1, 10);
	cout << h_size << endl;
	Test.PrintGraphInfos();*/
	bool b=Test.solveMIP();

}
