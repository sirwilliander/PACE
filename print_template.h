#ifndef _DFS_PRINT_TEMPLATE_H
#define _DFS_PRINT_TEMPLATE_H

#include <bits/stdc++.h>
#include <lemon/list_graph.h>

using namespace std;
using namespace lemon;

template <class C>
void Print_vector(const C &Original);

template <class C>
void Print_Matrix(const vector<vector<C>> &M, bool space = true);

template<class T, class C>
void Print_pair(const pair<T,C> &M);

template<class T, class C>
void Print_Matrix_pair(const vector<vector<pair<T,C>>> &M);

template <typename T>
struct less_than_key
{
    inline bool operator() (const pair<T, ListDigraph::Node>& struct1, const pair<T, ListDigraph::Node>& struct2)
    {
        return (struct1.first > struct2.first);
    }
};

#endif