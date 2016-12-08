#include <iostream>		  // for cout
#include <utility>		   // for pair
#include <algorithm>		 // for for_each
#include <iterator>
#include <vector>
#include <map>
#include <ctime>
#include <math.h>

using namespace std;

// Returns a vector of keys of the map
vector<vector<vector<int>>> keys(map<vector<vector<int>>,int> m)
{
	vector<vector<vector<int>>> v = {};
	for (map<vector<vector<int>>,int>::iterator it = m.begin(); it != m.end(); ++it)
	{
		v.push_back(it->first);
	}
	return v;
}

// Print Vector
void print_vector(vector<int> v)
{
	for (int i=0; i<v.size(); i++)
	{
		cout << " " << v[i];
	}
	cout << "\n\n";
}
// Print the graph
void print_graph(vector<vector<int>> G)
{
	for (int i=0; i < G.size(); i++)
	{
		for (int j=0; j<G[i].size(); j++)
		{
			cout << " " << G[i][j];
		}
		cout << "\n";
	}
	cout << "\n";
}

void merger(vector<vector<int>> G, vector<vector<int>>& H)
{
	for (int i=0; i<G.size(); i++)
	{
		for (int j=0; j<G.size(); j++)
		{
			if (G[i][j] ==1)
			{
				H[i][j] = 1;
			}
		}
	}
}

// Delete a vertex and it's neighbours
vector<vector<int>> del_v(vector<int> Vs, vector<vector<int>> G)
{
	int n = G.size();
	vector<vector<int>> H = {};
	for (int i=0; i<n; i++)
	{
		if (find(Vs.begin(), Vs.end(), i) != Vs.end())
		{
			continue;
		}
		H.push_back({});
		for (int j=0; j<n; j++)
		{
			if (find(Vs.begin(), Vs.end(), j) != Vs.end())
			{
				continue;
			}
			H[H.size()-1].push_back(G[i][j]);
		}
	}
	return H;
}

// Compute a maximum independent set in graph G
int indep( vector<vector<int>> G )
{
	int MAX = 0;
	int n = G.size();
	if (n == 0)
	{
		return 0;
	}
	if (n == 1)
	{
		return 1;
	}
	for (int i=0; i<n; i++)
	{
		vector<int> Vs = {i};
		for (int j=0; j<n; j++)
		{
			if (G[i][j]==1)
			{
				Vs.push_back(j);
			}
		}
		vector<vector<int>> H = del_v(Vs,G);
		int SUM = 1 + indep(H);
		if (SUM>MAX)
		{
			MAX = SUM;
		}
	}
	return MAX;
}

map<vector<vector<int>>, int> add_vertices(vector<int> OF, int N, map<vector<int>, map<vector<vector<int>>, int>>& face_dict)
{
	// Check we havent already seen this outer face
	if ( face_dict.find(OF) != face_dict.end() )
	{
		return face_dict[OF];
	}
	int n = *max_element(OF.begin(), OF.end());
	// Check we haven't reached target no. of vertices
	if (n+1 == N)
	{
		return map<vector<vector<int>>, int> {{vector<vector<int>> (N, vector<int> (N,0)) ,1}}; 
	}
	int n2 = OF.size();
	map<vector<vector<int>>, int> extensions;
	// Loop over all 1st connecting points
	for (int i=0;i<n2;i++)
	{
		// Shift outer face to begin at ith Vertex
		vector<int> OF2;
		for (int k=i;k<n2;k++)
		{
			OF2.push_back(OF[k]);
		}
		for (int k=0;k<i;k++)
		{
			OF2.push_back(OF[k]);
		}
		// Generate new graph with outer face and edge to first connecting point
		vector<vector<int>> H(n+2, vector<int> (n+2,0));
		for (int k=0; k<OF2.size()-1; k++)
		{
			H[OF2[k]][OF2[k+1]] = 1;
			H[OF2[k+1]][OF2[k]] = 1;
		}
		H[OF2[OF2.size()-1]][OF2[0]] = 1;
		H[OF2[0]][OF2[OF2.size()-1]] = 1;
		H[OF2[0]][n+1] = 1;
		H[n+1][OF2[0]] = 1;
		// Loop over all 2nd connecting points
		for (int j=1;j<n2;j++)
		{
			// Initialize new outer Face
			vector<int> OF3;
			OF3.push_back(OF2[0]);
			OF3.push_back(n+1);
			for (int k=j;k<n2;k++)
			{
				OF3.push_back(OF2[k]);
			}
			// Add edge to new 2nd connecting point
			H[n+1][OF2[j]] = 1;
			H[OF2[j]][n+1] = 1;
			
			// Generate all extensions
			map<vector<vector<int>>, int> temp = add_vertices(OF3, N, face_dict);
			for (map<vector<vector<int>>,int>::iterator it = temp.begin(); it != temp.end(); ++it)
			{
				// Merge extensions with current graph
				vector<vector<int>> TEMP = (it->first);
				merger(H,TEMP);
				extensions[TEMP] = 1;
			}
			extensions[H] = 1;
		}
	}
	// Update dictionary of face extensions
	for (int i=0; i<OF.size(); i++)
	{
		vector<int> key_OF = {};
		for (int j=i; j<OF.size(); j++)
		{
			key_OF.push_back(OF[j]);
		} 
		for (int j=0; j<i; j++)
		{
			key_OF.push_back(OF[j]);

		} 
		face_dict[key_OF] = extensions;
	}
	return face_dict[OF];
}

vector<int> copy_v(vector<int> v)
{
	vector<int> w(v.size(),0);
	for (int i=0; i<v.size(); i++)
	{
		w[i] = v[i];
	}
	return w;
}

vector<vector<int>> copy_G(vector<vector<int>> G)
{
	vector<vector<int>> H(G.size(), vector<int> (G.size(),0));
	for (int i=0; i<G.size(); i++)
	{
		for (int j=0; j<G.size(); j++)
		{
			H[i][j] = G[i][j];
		}
	}
	return H;
}

map<vector<vector<int>>,int> fm_loop(vector<vector<int>> G, vector<vector<int>> edges, vector<int> key, map<vector<int>, map<vector<vector<int>>,int> >& e_dict)
{
	// Check if the key is already in the dictionary
	if ( e_dict.find(key) != e_dict.end() )
	{
		return e_dict[key];
	}

	map<vector<vector<int>>,int> out = {};
	// Check if graph is still tight
	if (indep(G)!=ceil(((float)G.size())/((float)4)))
	{
		return out;
	}
	//  Loop over remaining edges
	for (int i=0; i<edges.size(); i++)
	{
		// Skip if edge already deleted
		if (key[i] == 1)
		{
			continue;
		}
		
		vector<int> new_key = copy_v(key);
		new_key[i] = 1;
		vector<vector<int>> H = copy_G(G);
		H[edges[i][0]][edges[i][1]]=0;
		H[edges[i][1]][edges[i][0]]=0;
		
		// Recurse over edges to delete
		map<vector<vector<int>>,int> smaller = fm_loop(H,edges,new_key,e_dict);
		for (map<vector<vector<int>>,int>::iterator it = smaller.begin(); it != smaller.end(); ++it)
		{
			out[it->first] = 1;
		}
		/*for (int j=0; j<out.size(); j++)
		{
			cout<< "out: \n";
			print_graph(out[j]);
		}*/
	}
	if (out.size() == 0 )
	{
		out[G] = 1;
	}
	e_dict[key] = out;
	return out;
}

map<vector<vector<int>>,int> find_minimals(vector<vector<int>> G)
{
	// Generate list of edges
	vector<vector<int>> edges = {};
	map<vector<int>, map<vector<vector<int>>,int> > e_dict;
	for (int i=0; i<G.size(); i++)
	{
		for (int j=i; j<G.size(); j++)
		{
			if (G[i][j] == 1)
			{
				edges.push_back(vector<int> {i,j});
			}
		}
	}
	// Run the loop
	return fm_loop(G, edges, vector<int> (edges.size(),0), e_dict);
}


int main()
{
	vector<vector<int>> dumb1 = 
	{
		{0,1,1,1,1,0,0,1},
		{1,0,1,0,0,0,1,1},
		{1,1,0,1,0,1,1,0},
		{1,0,1,0,1,1,0,0},
		{1,0,0,1,0,1,0,1},
		{0,0,1,1,1,0,1,0},
		{0,1,1,0,0,1,0,1},
		{1,1,0,0,1,0,1,0}
	};
	
	/*
	vector<vector<vector<int>>> fm = find_minimals(dumb1);
	for (int i=0; i<fm.size(); i++)
	{
		print_graph(fm[i]);
	}
	return 1;*/

	vector<int> dumb2 = {1,1,0,0,1,0,1,0};

	int N = 12;
	map<vector<int>, map<vector<vector<int>>, int>> face_dict;
	vector<int> OF = {0, 1};
	clock_t begin = clock();
	map<vector<vector<int>>, int> tris = add_vertices(OF, N, face_dict);
	vector<vector<vector<int>>> TRIS = keys(tris);
	clock_t end = clock();
	double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
	cout << "elapsed time: "<<elapsed_secs<<"\n";

	vector<vector<vector<int>>> tights = {};
	int biggest = 0;
	vector<vector<int>> biggest_G; 

	/*
	//vector<vector<int>> faces = {{0,1},{0,2,1},{6,7,4,5},{0,4,5,6,1},{0,4,5,2,1}};
	//vector<vector<int>> faces = {{6,7,4,5},{0,4,5,6,1},{0,4,5,2,1}};
	vector<vector<int>> faces = {{0,4,5,6,1}};
	for (int k=0; k<faces.size(); k++)
	{
		if (face_dict.find(faces[k]) != face_dict.end())
		{
			cout << "[ ";
			for (int l=0; l<faces[k].size(); l++)
			{
				cout << faces[k][l] << " ";
			}
			cout << "]: ";
			cout << face_dict[faces[k]].size() << "\n";
			//print_graph((next(face_dict[faces[k]].begin())->first));
			for (map<vector<vector<int>>,int>::iterator it = face_dict[faces[k]].begin(); it != face_dict[faces[k]].end(); ++it)
			{
				print_graph(it->first);
			}
			cout << "\n";
		}
	}
	*/
	//return 1;
	
	// Filter to tight triangulations
	for (int i=0; i<TRIS.size(); i++)
	{
		if (TRIS[i] == dumb1)
		//if (TRIS[i][7] == dumb2)
		{
			cout<<"hello\n";
			print_graph(TRIS[i]);
		}
                if (biggest < TRIS[i].size())
                {
                        biggest = TRIS[i].size();
                        biggest_G = TRIS[i];
                }
		if (indep(TRIS[i])==ceil(((float)TRIS[i].size())/((float)4)))
		{
			//print_graph(TRIS[i]);
			//cout << "\n\n";
			tights.push_back(TRIS[i]);
		}
	}
	cout << "tris: " << TRIS.size() << "\n";
	cout << "Biggest: \n";
	print_graph(biggest_G);
	biggest = 0;
	int biggest_minimal = 0;
	vector<vector<int>> biggest_minimal_G; 
	//return 1;

	// Filter to minimal tight graphs
	begin = clock();
	map<vector<vector<int>>,int> minimals = {};
	for (int i=0; i<tights.size(); i++)
	{
                if (biggest < tights[i].size())
                {
                        biggest = tights[i].size();
                        biggest_G = tights[i];
                }

		// Filter/Generate minimal graphs
		map<vector<vector<int>>,int> temp = find_minimals(tights[i]);
		for (map<vector<vector<int>>,int>::iterator it = temp.begin(); it != temp.end(); ++it)
		{
			vector<vector<int>> new_minimal = it->first; 
			if (biggest_minimal < new_minimal.size())
			{
				biggest_minimal = new_minimal.size();
				biggest_minimal_G = new_minimal;
			}
			minimals[new_minimal] = 1;
		}
	}
	cout << "tights: " << tights.size() << "\n";
	cout << "Biggest: \n";
	print_graph(biggest_G);
	biggest = 0;

	cout << "minimals: " << minimals.size() << "\n";
	cout << "Biggest: \n";
	print_graph(biggest_G);
	end = clock();
	elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
	cout << "elapsed time: "<<elapsed_secs<<"\n";

	vector<vector<int>> dumb = 
	{
		{0,1,0,1,1,0,0,1},
		{1,0,1,0,0,0,1,1},
		{0,1,0,1,0,1,1,0},
		{1,0,1,0,1,1,0,0},
		{1,0,0,1,0,1,0,1},
		{0,0,1,1,1,0,1,0},
		{0,1,1,0,0,1,0,1},
		{1,1,0,0,1,0,1,0}
	};
	//cout << indep(dumb) << "\n";
	//cout << ceil(((float)8)/((float)4)) << "\n";
	cout << "_______________\n";
	for (map<vector<vector<int>>,int>::iterator it = minimals.begin(); it != minimals.end(); ++it)
	{
		print_graph(it->first);
		cout << "_______________\n";
	}

	return 0;
}












