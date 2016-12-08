#include <iostream>                  // for std::cout
#include <utility>                   // for std::pair
#include <algorithm>                 // for std::for_each
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/algorithm/string.hpp>
#include <fstream>

using namespace boost;
using namespace std;

int main(int,char*[])
{
  // create a typedef for the Graph type
  typedef adjacency_list<vecS, vecS, bidirectionalS> Graph;

  // read file
  std::ifstream infile("planar_graphs.txt");
  int num_vertices;
  string strG;
  typedef vector< string > split_vector_type;
    
  split_vector_type SplitVec; // #2: Search for tokens
  while (infile >> num_vertices >> strG)
  {
    split( SplitVec, strG, is_any_of(",") ); // SplitVec == { "hello abc","ABC","aBc goodbye" }
    // process pair (a,b)
  }

  // Make convenient labels for the vertices
  enum { A, B, C, D, E, N };
  const char* name = "ABCDE";

  // writing out the edges in the graph
  typedef std::pair<int, int> Edge;
  Edge edge_array[] = 
  { Edge(A,B), Edge(A,D), Edge(C,A), Edge(D,C),
    Edge(C,E), Edge(B,D), Edge(D,E) };
  const int num_edges = sizeof(edge_array)/sizeof(edge_array[0]);

  // declare a graph object
  Graph g(num_vertices);

  // add the edges to the graph object
  for (int i = 0; i < num_edges; ++i)
    add_edge(edge_array[i].first, edge_array[i].second, g);

// get the property map for vertex indices
  typedef property_map<Graph, vertex_index_t>::type IndexMap;
  IndexMap index = get(vertex_index, g);

  std::cout << "vertices(g) = ";
  typedef graph_traits<Graph>::vertex_iterator vertex_iter;
  std::pair<vertex_iter, vertex_iter> vp;
  for (vp = vertices(g); vp.first != vp.second; ++vp.first)
    std::cout << index[*vp.first] <<  " ";
  std::cout << std::endl;
  // ...

  std::cout << "edges(g) = ";
  graph_traits<Graph>::edge_iterator ei, ei_end;
  for (tie(ei, ei_end) = edges(g); ei != ei_end; ++ei)
      std::cout << "(" << index[source(*ei, g)] 
                << "," << index[target(*ei, g)] << ") ";
  std::cout << std::endl;
    // ...eturn 0;
}
