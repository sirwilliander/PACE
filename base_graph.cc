#include "base_graph.h"

template <typename T>
DirectFeedbackSetProblem<T>::DirectFeedbackSetProblem(istream &is)
{
	ReadInput(is);
	original_vertex_numb_ = vertex_numb_;
	strongly_connected_num_ = 0;
}

template <typename T>
DirectFeedbackSetProblem<T>::DirectFeedbackSetProblem(string file_name)
{
	ReadFromFile(file_name);
	original_vertex_numb_ = vertex_numb_;
	strongly_connected_num_ = 0;
}

template <typename T>
void DirectFeedbackSetProblem<T>::PrintGraphInfos(ostream &os)
{
	vertex_numb_ = 0;
	for (ListDigraph::NodeIt n(graph_); n != INVALID; ++n)
		++vertex_numb_;
	os << "VERTEX NUMBER: " << vertex_numb_ << endl;
	arc_numb_ = 0;
	for (ListDigraph::ArcIt arc(graph_); arc != INVALID; ++arc)
		++arc_numb_;
	os << "ARC NUMBER: " << arc_numb_ << endl;
	os << "SOL and VERTEX NUMBER: " << (double)solution_.size() / original_vertex_numb_ << endl;
}

template <typename T>
void DirectFeedbackSetProblem<T>::AddVertices(int n)
{
	for (int i = 0; i < (n); ++i)
	{
		graph_.addNode();
	}
}

template <typename T>
template <class C>
void DirectFeedbackSetProblem<T>::DeleteVertices(C to_be_deleted)
{
	for (ListDigraph::Node &v : to_be_deleted)
	{
		graph_.erase(v);
	}
}

template <typename T>
void DirectFeedbackSetProblem<T>::ReadInput(istream &in)
{
	int temp;
	in >> vertex_numb_;
	in >> arc_numb_;
	in >> temp;
	this->AddVertices(vertex_numb_);

	string t;
	std::getline(in, t); //There is the first endline with \n
	for (int i = 0; i < vertex_numb_; ++i)
	{
		string line = "";
		std::getline(in, line);
		if (DEBUG)
			cout << "line: " << line << endl;
		stringstream sstream(line);
		int adjacent_vert;
		while (sstream >> adjacent_vert)
		{
			if (DEBUG)
				cout << "Adjacent_vert: " << adjacent_vert << endl;
			graph_.addArc(graph_.nodeFromId(i), graph_.nodeFromId(adjacent_vert - 1));
		}
	}
}

template <typename T>
void DirectFeedbackSetProblem<T>::ReadFromFile(string file_name)
{
	ifstream myfile ("./contrib/examples/exact_public/"+file_name);	 	
	istream& mystream  = myfile;
	ReadInput(mystream);
}


template <typename T>
void DirectFeedbackSetProblem<T>::PrintEdgeList(ostream &os)
{
	for (ListDigraph::NodeIt n(graph_); n != INVALID; ++n)
	{
		os << graph_.id(n) << ": ";
		for (ListDigraph::OutArcIt e(graph_, n); e != INVALID; ++e)
		{
			os << graph_.id(graph_.target(e)) << " ";
		}
		os << endl;
	}
}

template <typename T>
double DirectFeedbackSetProblem<T>::AdjacentEdgeAverage()
{
	return (double)arc_numb_ / (double)vertex_numb_;
}

template <typename T>
int DirectFeedbackSetProblem<T>::CreateCondensedGraph()
{
	strongly_connected_num_ = lemon::stronglyConnectedComponents(graph_, strongly_connected_comp_);
	AddVerticesToGraph(condensed_graph_, strongly_connected_num_);
	lemon::stronglyConnectedCutArcs(graph_, strongly_connected_arcs_);
	for (ListDigraph::ArcIt arc(graph_); arc != INVALID; ++arc)
	{
		if (strongly_connected_arcs_[arc])
		{
			condensed_graph_.addArc(
				condensed_graph_.nodeFromId(strongly_connected_comp_[graph_.source(arc)]),
				condensed_graph_.nodeFromId(strongly_connected_comp_[graph_.target(arc)]));
		}
	}
	return strongly_connected_num_;
}

template <typename T>
vector<int> DirectFeedbackSetProblem<T>::StronglyConnectedSizes()
{
	if (strongly_connected_num_ == 0)
		CreateCondensedGraph();
	vector<int> strongly_conn_sizes(strongly_connected_num_, 0);
	for (ListDigraph::NodeIt n(graph_); n != INVALID; ++n)
	{
		++strongly_conn_sizes[strongly_connected_comp_[n]];
	}

	return strongly_conn_sizes;
}

template <typename T>
bool DirectFeedbackSetProblem<T>::IsDAG()
{
	return lemon::dag(graph_);
}

//Usage of printToPdf:
// ./testDraw | dot -Tpdf > file1.pdf
template <typename T>
void DirectFeedbackSetProblem<T>::PrintToPdf(bool condensed, ostream &os)
{
	os << "digraph G{" << endl;
	if (!condensed)
	{
		for (ListDigraph::ArcIt arc(graph_); arc != INVALID; ++arc)
		{
			os << graph_.id(graph_.source(arc)) << " -> " << graph_.id(graph_.target(arc)) << ";" << endl;
		}
		os << "}" << endl;
	}
	else
	{
		for (ListDigraph::ArcIt arc(condensed_graph_); arc != INVALID; ++arc)
		{
			os << condensed_graph_.id(condensed_graph_.source(arc)) << " -> " << condensed_graph_.id(condensed_graph_.target(arc)) << ";" << endl;
		}
		os << "}" << endl;
	}
}

template <typename T>
int DirectFeedbackSetProblem<T>::DeleteSelfLoops()
{
	int i = 0;
	set<int> toBeDeleted;
	for (ListDigraph::ArcIt arc(graph_); arc != INVALID; ++arc)
	{
		if (graph_.source(arc) == graph_.target(arc))
		{
			toBeDeleted.insert(graph_.id(graph_.source(arc)));
			--arc_numb_;
		}
	}
	for (set<int>::iterator it = toBeDeleted.begin(); it != toBeDeleted.end(); ++it)
	{
		solution_.push_back(graph_.nodeFromId(*it));
		graph_.erase(graph_.nodeFromId(*it));
		--vertex_numb_;
		++i;
	}

	return i;
}

template <typename T>
int DirectFeedbackSetProblem<T>::DeleteParallelArcs()
{
	int i = 0;
	map<Node_pair, int> m;
	Node_pair np;

	for (ListDigraph::ArcIt arc(graph_); arc != INVALID; ++arc)
	{
		np.u = graph_.source(arc);
		np.v = graph_.target(arc);
		++m[np];
		if (m[np] >= 2)
		{
			graph_.erase(arc);
			--arc_numb_;
			++i;
		}
	}

	return i;
}

template <typename T>
int DirectFeedbackSetProblem<T>::DeleteDummyNodes()
{
	//Nodes without ingoing or outgoing arcs
	vector<ListDigraph::Node> to_be_deleted;

	for (ListDigraph::NodeIt n(graph_); n != INVALID; ++n)
	{
		int out = 0;
		for (ListDigraph::OutArcIt a(graph_, n); a != INVALID; ++a)
		{
			++out;
		}

		int in = 0;
		for (ListDigraph::InArcIt a(graph_, n); a != INVALID; ++a)
		{
			++in;
		}

		if (!in || !out)
		{
			to_be_deleted.push_back(n);
			--vertex_numb_;
			arc_numb_ -= (in + out);
		}
	}

	DeleteVertices(to_be_deleted);

	return to_be_deleted.size();
}

template <typename T>
int DirectFeedbackSetProblem<T>::DeleteChainingNodes()
{
	//Nodes with only one ingoing and one outgoing edges
	vector<ListDigraph::Node> to_be_deleted;

	for (ListDigraph::NodeIt n(graph_); n != INVALID; ++n)
	{
		int out = 0;
		for (ListDigraph::OutArcIt a(graph_, n); a != INVALID; ++a)
		{
			++out;
		}

		int in = 0;
		for (ListDigraph::InArcIt a(graph_, n); a != INVALID; ++a)
		{
			++in;
		}

		if (in == 1 && in == out)
		{
			to_be_deleted.push_back(n);
			--vertex_numb_;
			--arc_numb_;
		}
	}

	for (ListDigraph::Node &v : to_be_deleted)
	{
		ListDigraph::Arc out_arc = ListDigraph::OutArcIt(graph_, v);
		ListDigraph::Arc in_arc = ListDigraph::InArcIt(graph_, v);

		ListDigraph::Node u_next = graph_.target(out_arc);
		ListDigraph::Node u_prev = graph_.source(in_arc);

		graph_.addArc(u_prev, u_next);
		graph_.erase(v);
	}

	return to_be_deleted.size();
}

template <typename T>
int DirectFeedbackSetProblem<T>::PetalOne()
{
	//Deletion of a node which is contained in exactly one cycle
	int i = 0; // If there is a cycle which has only petal=1 nodes, then at least one of them has to stay. So we delete during the for cycle
	bool del = 0;
	ListDigraph::Node deletable;
	ListDigraph::ArcMap<int> cap(graph_);
	for (ListDigraph::ArcIt arc(graph_); arc != INVALID; ++arc)
		cap[arc] = 1;
	for (ListDigraph::NodeIt node(graph_); node != INVALID; ++node)
	{
		if (del)
		{
			graph_.erase(deletable);
			del = 0;
		}

		ListDigraph::Node u = graph_.split(node, false);

		//One way
		lemon::Preflow<ListDigraph> my_flow1(graph_, cap, u, node);
		my_flow1.init();
		my_flow1.startFirstPhase();
		int flow_value1 = my_flow1.flowValue();

		//And the other
		Preflow<ListDigraph> my_flow2(graph_, cap, node, u);
		my_flow2.init();
		my_flow2.startFirstPhase();
		int flow_value2 = my_flow2.flowValue();
		graph_.contract(node, u, true);

		if (flow_value1 + flow_value2 == 1)
		{
			int counter1 = 0;
			int counter2 = 0;
			for (ListDigraph::InArcIt x(graph_, node); x != INVALID; ++x)
			{
				++counter1;
				for (ListDigraph::OutArcIt y(graph_, node); y != INVALID; ++y)
				{
					++counter2;
					graph_.addArc(graph_.source(x), graph_.target(y));
				}
			}
			deletable = node;
			del = true;
			++i;
			--vertex_numb_;
			arc_numb_ = arc_numb_ + counter1 * counter2 - counter1 - counter2;
		}
	}
	if (del)
		graph_.erase(deletable);
	return i;
}

template <typename T>
void DirectFeedbackSetProblem<T>::ReduceGraph()
{
	bool loop_free = lemon::loopFree(graph_);
	bool parallel_free = lemon::parallelFree(graph_);

	if (!loop_free)
	{
		DeleteSelfLoops();
	}

	if (!parallel_free)
	{
		DeleteParallelArcs();
	}

	while (DeleteDummyNodes() || DeleteChainingNodes())
		;
	//May there be parallel arcs after chaining the nodes?
	PetalOne();
}

void AddVerticesToGraph(lemon::ListDigraph &g, int n)
{
	for (int i = 0; i < (n); ++i)
	{
		g.addNode();
	}
}

bool operator<(const Node_pair &x, const Node_pair &y)
{
	return tie(x.u, x.v) < tie(y.u, y.v);
}