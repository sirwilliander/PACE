#include <iostream>
#include <lemon/list_graph.h>
#include <math.h>
#include <lemon/graph_to_eps.h>
#include <lemon/color.h>

using namespace std;	
using namespace lemon;

void printToEps(ListDigraph& g){
  graphToEps(g,"example.eps")
    //.arcWidths(constMap<ListDigraph::Arc,double>(100.))
    //.scale(200000)
    //.hideNodes()
    //.arcColors(constMap<ListDigraph::Arc,double>(lemon::red()))
    .preScale(true)
    .autoArcWidthScale(true)
    .autoNodeScale(true)
    .run();
}
//Usage of printToPdf:
// ./testDraw | dot -Tpdf > file.pdf
void printToPdf(ListDigraph& g){
	cout<<"digraph G{"<<endl;
	for(ListDigraph::ArcIt arc(g);arc!=INVALID;++arc)
	{
		ListDigraph::Node x=g.source(arc);
		ListDigraph::Node y=g.target(arc);
		cout << g.id(x)<< " -> "<< g.id(y)<<";"<<endl;
	}
	cout<<"}"<<endl;
}


int main()
{ 
lemon::ListDigraph g;

/*Example run*/
g.addNode();
g.addNode();
g.addNode();
g.addNode(); 
g.addArc(g.nodeFromId(0),g.nodeFromId(1));
g.addArc(g.nodeFromId(0),g.nodeFromId(2));
g.addArc(g.nodeFromId(1),g.nodeFromId(3));
g.addArc(g.nodeFromId(1),g.nodeFromId(0));

printToPdf(g);
return 0;
}
