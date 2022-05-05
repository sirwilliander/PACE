#include "base_graph.h"
#include <bits/stdc++.h>

template <typename T>
bool DirectFeedbackSetProblem<T>::SolveIPWithoutCycles()
{
    lemon::Timer t(1);
    Mip mip;
    // const int M = vertex_numb_;
    map<int, Mip::Col> x; //In or Out of solution
    for (lemon::ListDigraph::NodeIt u(graph_); u != lemon::INVALID; ++u)
    {
        x[graph_.id(u)] = mip.addCol();
    }
    for (lemon::ListDigraph::NodeIt u(graph_); u != lemon::INVALID; ++u)
    {
        mip.colLowerBound(x[graph_.id(u)], 0);
        mip.colUpperBound(x[graph_.id(u)], 1);
    }
    for (lemon::ListDigraph::NodeIt u(graph_); u != lemon::INVALID; ++u)
    {
        mip.colType(x[graph_.id(u)], Mip::INTEGER);
    }
    for (auto &cyc : cycles_to_dodge_)
    {
        Mip::Expr expr;
        for (auto &nodeiter : cyc)
        {
            int i = graph_.id(nodeiter);
            expr = expr + (1 - x[i]);
        }
        mip.addRow(expr >= 1);
    }

    Mip::Expr maxNum;
    for (lemon::ListDigraph::NodeIt u(graph_); u != lemon::INVALID; ++u)
    {
        maxNum += x[graph_.id(u)];
    }
    mip.max();
    mip.obj(maxNum);

    bool is_there_cycle = 0;
    int iter_num = 0;
    while (iter_num < 1)
    {
        mip.solve();
        if (mip.type() == Mip::OPTIMAL || mip.type() == Mip::FEASIBLE)
        {
            lemon::ListDigraph::NodeMap<bool> VerticesIn(graph_, false);
            cout << "iternum: " << iter_num << ", mip sol: ";
            for (lemon::ListDigraph::NodeIt u(graph_); u != lemon::INVALID; ++u)
            {
                cout << mip.sol(x[graph_.id(u)]) << " ";
                if (mip.sol(x[graph_.id(u)]) > 0.9)
                {
                    //cout << graph_.id(u) << " ";
                    VerticesIn[u] = true;
                }
            }
            cout << endl;
            is_there_cycle = CycleWithDepthFirstSearch(VerticesIn);
        }

        // Print the results
        if (mip.type() == Mip::OPTIMAL || mip.type() == Mip::FEASIBLE)
        {
            std::cout << endl
                      << "IP kész, " << cycles_to_dodge_.size() << " db kör kizárva. " << t.userTime() << "s" << std::endl;
            cout << "Legkevesebb pont amit törölni kell: " << vertex_numb_ - mip.solValue() << endl;
        }
        else
        {
            std::cout << "Optimal solution not found." << std::endl;
        }

        iter_num++;
    }
    return is_there_cycle;
}

template <typename T>
bool DirectFeedbackSetProblem<T>::CycleWithDepthFirstSearch()
{
    ListDigraph::NodeMap<bool> VerticesIn(graph_, 1);
    return CycleWithDepthFirstSearch(VerticesIn);
}

template <typename T>
bool DirectFeedbackSetProblem<T>::CycleWithDepthFirstSearch(lemon::ListDigraph::NodeMap<bool> &VerticesIn)
{
    bool is_there_cycle = 0;
    // Depth First Search
    lemon::ListDigraph::NodeMap<int> eleresi_szam(graph_, -1);
    lemon::ListDigraph::NodeMap<int> befejezesi_szam(graph_, -1);
    lemon::ListDigraph::NodeMap<int> melyseg(graph_, -1);
    lemon::ListDigraph::NodeMap<ListDigraph::Node> parent(graph_, lemon::INVALID);

    int M = 0;  // Mélység számolásához
    int SZ = 0; //Elérési számhoz
    int S = 0;  //Befejezési számhoz

    // Finding a ((yet) not so random) source node
    for (lemon::ListDigraph::NodeIt u(graph_); u != lemon::INVALID; ++u)
    {
        if (VerticesIn[u] == 1 && parent[u] == lemon::INVALID)
        {
            parent[u] = u;
            // cout << "Starting from here: " << graph_.id(u) << endl;
            DepthFirstSearchStep(SZ, S, M, u, eleresi_szam, parent, befejezesi_szam, VerticesIn, melyseg);
        }
    }

    // Add new cycles
    lemon::ListDigraph::NodeMap<bool> ready(graph_, 0);
    for (lemon::ListDigraph::NodeIt node_from(graph_); node_from != lemon::INVALID; ++node_from)
    {
        if (ready[node_from] == 0)
        {
            int minlength = vertex_numb_;
            ListDigraph::Node back_edge_target = lemon::INVALID;
            for (lemon::ListDigraph::OutArcIt a(graph_, node_from); a != INVALID; ++a)
            {
                if (BackGoingArc(a, eleresi_szam, befejezesi_szam, VerticesIn))
                {
                    if (melyseg[graph_.source(a)] - melyseg[graph_.target(a)] < minlength)
                    {
                        back_edge_target = graph_.target(a);
                    }
                }
            }

            if (back_edge_target != lemon::INVALID)
            {
                is_there_cycle = 1;
                AddCycle(node_from, back_edge_target, parent, ready);
            }
            ready[node_from] = 1;
        }
    }
    return is_there_cycle;
}

template <typename T>
bool DirectFeedbackSetProblem<T>::DepthFirstSearchStep(int &SZ, int &S, int M, ListDigraph::Node current_node, lemon::ListDigraph::NodeMap<int> &eleresi_szam,
                                                       lemon::ListDigraph::NodeMap<ListDigraph::Node> &parent, lemon::ListDigraph::NodeMap<int> &befejezesi_szam,
                                                       lemon::ListDigraph::NodeMap<bool> &VerticesIn, lemon::ListDigraph::NodeMap<int> &melyseg)
{
    SZ++;
    eleresi_szam[current_node] = SZ;

    melyseg[current_node] = M;
    M++;

    //cout << graph_.id(current_node) << " " << graph_.id(parent[current_node]) << " ";
    //cout << melyseg[current_node] << " " << endl;

    for (lemon::ListDigraph::OutArcIt a(graph_, current_node); a != INVALID; ++a)
    {
        if (VerticesIn[graph_.target(a)] && parent[graph_.target(a)] == lemon::INVALID)
        {
            parent[graph_.target(a)] = current_node;
            DepthFirstSearchStep(SZ, S, M, graph_.target(a), eleresi_szam, parent, befejezesi_szam, VerticesIn, melyseg);
        }
        S++;
        befejezesi_szam[current_node] = S;
    }
    return 0;
}

template <typename T>
bool DirectFeedbackSetProblem<T>::BackGoingArc(ListDigraph::Arc &arc, ListDigraph::NodeMap<int> &eleresi_szam, ListDigraph::NodeMap<int> &befejezesi_szam,
                                               ListDigraph::NodeMap<bool> &VerticesIn)
{
    bool backgoing = (VerticesIn[graph_.source(arc)] && VerticesIn[graph_.target(arc)] &&
                      eleresi_szam[graph_.source(arc)] > eleresi_szam[graph_.target(arc)] &&
                      befejezesi_szam[graph_.source(arc)] < befejezesi_szam[graph_.target(arc)]);
    return backgoing;
}

template <typename T>
bool DirectFeedbackSetProblem<T>::AddCycle(ListDigraph::Node &node_from, ListDigraph::Node &back_edge_target,
                                           ListDigraph::NodeMap<ListDigraph::Node> &parent, ListDigraph::NodeMap<bool> &ready)
{
    vector<ListDigraph::Node> newcycle;
    ListDigraph::Node current_node = node_from;
    newcycle.push_back(node_from);
    ready[node_from] = 1;

    while (current_node != back_edge_target)
    {
        current_node = parent[current_node];
        newcycle.push_back(current_node);
        ready[current_node] = 1;
    }
    cycles_to_dodge_.push_back(newcycle);

    // Print new cycle
    cout << "New cycle: ";
    for (auto &pathiter : newcycle)
    {
        cout << graph_.id(pathiter) << " ";
    }
    cout << endl;

    return 0;
}

//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
////GENERATE RANDOM PERMUTATION
// Function to return the next random number
int getNum(vector<int> &v)
{

    // Size of the vector
    int n = v.size();

    // Generate a random number
    srand(time(NULL));

    // Make sure the number is within
    // the index range
    int index = rand() % n;

    // Get random number from the vector
    int num = v[index];

    // Remove the number from the vector
    swap(v[index], v[n - 1]);
    v.pop_back();

    // Return the removed number
    return num;
}

// Function to generate n non-repeating random numbers
void generateRandom(int n)
{
    vector<int> v(n);

    // Fill the vector with the values
    // 1, 2, 3, ..., n
    for (int i = 0; i < n; i++)
        v[i] = i + 1;

    // While vector has elements
    // get a random number from the vector and print it
    while (v.size())
    {
        cout << getNum(v) << " ";
    }
}
