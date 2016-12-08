#include <iostream>                  // for cout
#include <utility>                   // for pair
#include <algorithm>                 // for for_each
#include <iterator>

#include <boost/graph/graph_traits.hpp>
#include "boost/graph/adjacency_list.hpp"

using namespace boost;
using namespace std;

// Create a struct to hold properties for each vertex
struct VertexProperties
{
	int VertexIntProperty;
	string VertexStringProperty;
};
 
// Create a struct to hold properties for each edge
struct EdgeProperties
{
	int num_f;
	string EdgeStringProperty;
};
 
// Define the type of the graph
typedef adjacency_list<vecS, vecS, undirectedS, VertexProperties, EdgeProperties> Graph;

void list_graph(Graph g)
{
	// get the property map for vertex indices
	typedef property_map<Graph, vertex_index_t>::type IndexMap;
	IndexMap index = get(vertex_index, g);

	cout << "vertices(g) = ";
	typedef graph_traits<Graph>::vertex_iterator vertex_iter;
	pair<vertex_iter, vertex_iter> vp;
	for (vp = vertices(g); vp.first != vp.second; ++vp.first)
		cout << index[*vp.first] <<  " ";
	cout << endl;

	cout << "edges(g) = ";
	graph_traits<Graph>::edge_iterator ei, ei_end;
	for (tie(ei, ei_end) = edges(g); ei != ei_end; ++ei)
		cout << "(" << index[source(*ei, g)] 
		     << "," << index[target(*ei, g)] << ") ";
	cout << endl;
}

int g_size(Graph g)
{
	int size = 0;
	typedef graph_traits<Graph>::vertex_iterator vertex_iter;
	pair<vertex_iter, vertex_iter> vp;
	for (vp = vertices(g); vp.first != vp.second; ++vp.first){size++;}
	return size;
}

vector<Graph> tri_constructor(Graph g, int num_v)
{
	vector<Graph> tris {};
	int size = g_size(g); 
	
	// if all vertices are added return empty list
	if (size == num_v+1){return tris;}

	Graph h = g;
	pair<Graph::edge_iterator, Graph::edge_iterator> hEdgeIterRange = edges(h);
	for (Graph::edge_iterator hEdgeIter = hEdgeIterRange.first; hEdgeIter != hEdgeIterRange.second; ++hEdgeIter)
	{
		if (h[*hEdgeIter].num_f == 2)
		{
			continue;
		}
		h[*hEdgeIter].num_f = 2;
		int s = source(*hEdgeIter, h);
		int t = target(*hEdgeIter, h);
		pair<Graph::edge_descriptor, bool> e04 = add_edge(s, i, h);
		h[edge(s,i,h).first].num_f = 1;
		pair<Graph::edge_descriptor, bool> e05 = add_edge(t, i, h);
		h[edge(t,i,h).first].num_f = 1;
		break;
	}
	g = h;
}


int main()
{
	// num vertices
	int num_v = 6;
	// array of triangulations
	vector<Graph> tris;

	// declare a graph object
	Graph g(3);
	
	// build initial triangle
	pair<Graph::edge_descriptor, bool> e01 = add_edge(0, 1, g);
	g[edge(0,1,g).first].num_f = 1;
	pair<Graph::edge_descriptor, bool> e02 = add_edge(0, 2, g);
	g[edge(0,2,g).first].num_f = 1;
	pair<Graph::edge_descriptor, bool> e03 = add_edge(1, 2, g);
	g[edge(1,2,g).first].num_f = 1;

	
	pair<Graph::edge_iterator, Graph::edge_iterator> gVertexIterRange = vertices(h);

	
	list_graph(g);
	// loop through remaining vertices adding triangles
	for (int i=3; i<num_v+1; i++)
	{
	}
	list_graph(g);

	return 0;
}












