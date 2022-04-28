#include "base_graph.h"

//LP based methods

template <typename T>
bool DirectFeedbackSetProblem<T>::SolveMIP()
{
	lemon::Timer t(1);
	Mip mip;
	const int M = vertex_numb_;
	map<int, Mip::Col> x; //In or Out of solution
	map<int, Mip::Col> y; //Potential, against cycles
	for (lemon::ListDigraph::NodeIt u(graph_); u != lemon::INVALID; ++u)
	{
		x[graph_.id(u)] = mip.addCol();
		y[graph_.id(u)] = mip.addCol();
	}
	for (lemon::ListDigraph::NodeIt u(graph_); u != lemon::INVALID; ++u)
	{
		mip.colLowerBound(x[graph_.id(u)], 0);
		mip.colUpperBound(x[graph_.id(u)], 1);
		mip.colLowerBound(y[graph_.id(u)], 0);
		mip.colUpperBound(y[graph_.id(u)], vertex_numb_);
	}
	for (lemon::ListDigraph::NodeIt u(graph_); u != lemon::INVALID; ++u)
	{
		mip.colType(x[graph_.id(u)], Mip::INTEGER);
		//mip.colType(y[graph_.id(u)], Mip::INTEGER);
	}
	for (lemon::ListDigraph::ArcIt a(graph_); a != lemon::INVALID; ++a)
	{
		int i = graph_.id(graph_.source(a));
		int j = graph_.id(graph_.target(a));
		Mip::Expr expr = y[i] - y[j] + 1 - (1 - x[i]) * M - (1 - x[j]) * M;
		mip.addRow(expr <= 0);
	}
	Mip::Expr maxNum;
	for (lemon::ListDigraph::NodeIt u(graph_); u != lemon::INVALID; ++u)
	{
		maxNum += x[graph_.id(u)];
		//if(x[graph_.id(u)] == 1) {solution_[u] = true; ++solution_size_;}
	}
	mip.max();
	mip.obj(maxNum);
	mip.solve();
	std::cout << "IP done in " << t.userTime() << "s" << std::endl;
	// Print the results
	if (mip.type() == Mip::OPTIMAL || mip.type() == Mip::FEASIBLE)
	{
		cout << "Legkevesebb pont amit törölni kell: " << vertex_numb_ - mip.solValue() << endl;
	}
	else
	{
		std::cout << "Optimal solution not found." << std::endl;
	}
	return mip.type() == Mip::OPTIMAL || mip.type() == Mip::FEASIBLE;
}

template <typename T>
bool DirectFeedbackSetProblem<T>::SolveLP()
{
	lemon::Timer t(1);
	Mip mip;
	const int M = vertex_numb_;
	map<int, Mip::Col> x; //In or Out of solution
	map<int, Mip::Col> y; //Potential, against cycles
	for (lemon::ListDigraph::NodeIt u(graph_); u != lemon::INVALID; ++u)
	{
		x[graph_.id(u)] = mip.addCol();
		y[graph_.id(u)] = mip.addCol();
	}
	for (lemon::ListDigraph::NodeIt u(graph_); u != lemon::INVALID; ++u)
	{
		mip.colLowerBound(x[graph_.id(u)], 0);
		mip.colUpperBound(x[graph_.id(u)], 1);
		mip.colLowerBound(y[graph_.id(u)], 0);
		mip.colUpperBound(y[graph_.id(u)], vertex_numb_);
	}
	for (lemon::ListDigraph::NodeIt u(graph_); u != lemon::INVALID; ++u)
	{
		mip.colType(x[graph_.id(u)], Mip::REAL);
		mip.colType(y[graph_.id(u)], Mip::REAL);
	}
	for (lemon::ListDigraph::ArcIt a(graph_); a != lemon::INVALID; ++a)
	{
		int i = graph_.id(graph_.source(a));
		int j = graph_.id(graph_.target(a));
		Mip::Expr expr = y[i] - y[j] + 1 - (1 - x[i]) * M - (1 - x[j]) * M;
		mip.addRow(expr <= 0);
	}
	Mip::Expr maxNum;
	for (lemon::ListDigraph::NodeIt u(graph_); u != lemon::INVALID; ++u)
	{
		maxNum += x[graph_.id(u)];
	}
	mip.max();
	mip.obj(maxNum);
	mip.solve();
	std::cout << "LP done in " << t.userTime() << "s" << std::endl;
	// Print the results
	if (mip.type() == Mip::OPTIMAL || mip.type() == Mip::FEASIBLE)
	{
		cout << "Legkevesebb pont amit törölni kell: " << vertex_numb_ - mip.solValue() << endl;
	}
	else
	{
		std::cout << "Optimal solution not found." << std::endl;
	}
	int cnt = 0;
	for (lemon::ListDigraph::NodeIt u(graph_); u != lemon::INVALID; ++u)
	{
		if (mip.sol(x[graph_.id(u)]) != 1)
		{
			++cnt;
		}
	}
	cout << "Ennyi változó nem egyes: " << cnt << endl;
	return mip.type() == Mip::OPTIMAL || mip.type() == Mip::FEASIBLE;
}

//Heuristic Algorithms

template <typename T>
int DirectFeedbackSetProblem<T>::InAndOutGoingArcs(ListDigraph::Node v, int caser)
{
	int out = 0;
	for (ListDigraph::OutArcIt a(graph_, v); a != INVALID; ++a)
	{
		++out;
	}
	int in = 0;
	for (ListDigraph::InArcIt a(graph_, v); a != INVALID; ++a)
	{
		++in;
	}
	switch (caser)
	{
	case 1:
		return out + in;
		break;
	case 2:
		return in;
		break;
	case 3:
		return out;
	}
}

/*
template <typename T>
template <typename V, typename F>
int DirectFeedbackSetProblem<T>::LargestVerticesDeletion(F Fitness, int caser, int recalculate)
{
	// Fitness has two input Node and int
	//	Will delete vertices ordered by their fitnesses until the graph is acyclic.
	//	Recalculate the fitness after every "recalculate" number of vertices has been deleted
	// The fitnesses elements has type V
	//auto& Fitness = InAndOutGoingArcs; //Currently only works on this function
	while (!IsDAG())
	{
		vector<pair<V, ListDigraph::Node>> fitnesses;
		for (ListDigraph::NodeIt n(graph_); n != INVALID; ++n)
		{
			fitnesses.push_back(make_pair((this->*Fitness)(n, caser), n));
		}
		//std::transform(ListDigraph::NodeIt (graph_), INVALID, std::back_inserter(fitnesses), [&](ListDigraph::Node v){return make_pair((this->*Fitness)(v, caser), v);} );
		std::sort(all(fitnesses), [](pair<V, ListDigraph::Node> &struct1, pair<V, ListDigraph::Node> &struct2)
				  { return (struct1.first > struct2.first); });
		FOR(i, recalculate)
		{
			solution_[fitnesses[i].second] = 1;
			solution_size_++;
			graph_.erase(fitnesses[i].second);
		}
	}
	return solution_size_;
}*/

template <typename T>
bool DirectFeedbackSetProblem<T>::SolveMIPSCC()
{
	strongly_connected_num_ = lemon::stronglyConnectedComponents(graph_, strongly_connected_comp_);
	cout << "strongly_connected_num_ :" << strongly_connected_num_ << endl;
	int counter1 = 0;
	lemon::Timer t1(1);
	for (int i = 0; i < strongly_connected_num_; ++i)
	{
		int counter2 = 0;
		lemon::ListDigraph::NodeMap<bool> subgraphNode(graph_, false);
		lemon::ListDigraph::ArcMap<bool> subgraphArc(graph_, false);
		for (ListDigraph::NodeIt n1(graph_); n1 != INVALID; ++n1)
		{
			if (strongly_connected_comp_[n1] == i)
			{
				subgraphNode[n1] = true;
				++counter2;
			}
		}
		for (ListDigraph::ArcIt n1(graph_); n1 != INVALID; ++n1)
		{
			if (subgraphNode[graph_.source(n1)] && subgraphNode[graph_.target(n1)])
			{
				subgraphArc[n1] = 1;
			}
		}
		SubDigraph<ListDigraph, lemon::ListDigraph::NodeMap<bool>, lemon::ListDigraph::ArcMap<bool>> H(graph_, subgraphNode, subgraphArc);

		Mip mip;

		const int M = 1000000;

		map<int, Mip::Col> x; //In or Out of solution
		map<int, Mip::Col> y; //Potential, against cycles
		for (lemon::SubDigraph<ListDigraph, lemon::ListDigraph::NodeMap<bool>, lemon::ListDigraph::ArcMap<bool>>::NodeIt u(H); u != lemon::INVALID; ++u)
		{
			x[H.id(u)] = mip.addCol();
			y[H.id(u)] = mip.addCol();
		}

		for (lemon::SubDigraph<ListDigraph, lemon::ListDigraph::NodeMap<bool>, lemon::ListDigraph::ArcMap<bool>>::NodeIt u(H); u != lemon::INVALID; ++u)
		{
			mip.colLowerBound(x[H.id(u)], 0);
			mip.colUpperBound(x[H.id(u)], 1);
			mip.colLowerBound(y[H.id(u)], 0);
			mip.colUpperBound(y[H.id(u)], vertex_numb_);
		}

		for (lemon::SubDigraph<ListDigraph, lemon::ListDigraph::NodeMap<bool>, lemon::ListDigraph::ArcMap<bool>>::NodeIt u(H); u != lemon::INVALID; ++u)
		{
			mip.colType(x[H.id(u)], Mip::INTEGER);
			//mip.colType(y[graph_.id(u)], Mip::INTEGER);
		}

		for (lemon::SubDigraph<ListDigraph, lemon::ListDigraph::NodeMap<bool>, lemon::ListDigraph::ArcMap<bool>>::ArcIt a(H); a != lemon::INVALID; ++a)
		{
			int i = H.id(H.source(a));
			int j = H.id(H.target(a));
			Mip::Expr expr = y[i] - y[j] + 1 - (1 - x[i]) * M - (1 - x[j]) * M;

			mip.addRow(expr <= 0);
		}

		Mip::Expr maxNum;
		for (lemon::SubDigraph<ListDigraph, lemon::ListDigraph::NodeMap<bool>, lemon::ListDigraph::ArcMap<bool>>::NodeIt u(H); u != lemon::INVALID; ++u)
		{
			maxNum += x[H.id(u)];
		}

		mip.max();
		mip.obj(maxNum);
		mip.solve();

		std::cout << i + 1 << ". IP done in " << t1.userTime() << "s" << std::endl;
		// Print the results
		if (mip.type() == Mip::OPTIMAL || mip.type() == Mip::FEASIBLE)
		{
			counter1 += mip.solValue();
			cout << "Itt ennyit kell törölni: " << counter2 - mip.solValue() << endl;
		}
		else
		{
			std::cout << "Optimal solution not found." << std::endl;
		}
	}
	//return mip.type() == Mip::OPTIMAL || mip.type() == Mip::FEASIBLE;
	cout << "Legkevesebb pont amit törölni kell: " << vertex_numb_ - counter1 << endl;
	return true;
}