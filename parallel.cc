#include <iostream>		  // for cout
#include <utility>		   // for pair
#include <algorithm>		 // for for_each
#include <iterator>
#include <vector>
#include <map>
#include <ctime>
#include <math.h>
#include <omp.h>

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
void print_v(vector<int> v)
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
		cout << G[i][0];
		for (int j=1; j<G[i].size(); j++)
		{
			cout << " " << G[i][j];
		}
		cout << "\n";
	}
	cout << "\n";
}

// No longer used
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
                vector<int> Vs ={i};
                int deg = 0; 
                for (int ele : G[i])
                {
                        if (ele == 1)
                        {
                                Vs.push_back(ele);
                                deg += ele;
                        }
                }
                if (deg <= 1)
                {
                        vector<vector<int>> H = del_v(Vs,G);
                        MAX += 1 + indep(H);
                        return MAX;
                }
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

vector<vector<int>> copy_G_p1(vector<vector<int>> G)
{
	vector<vector<int>> H(G.size()+1, vector<int> (G.size()+1,0));
	for (int i=0; i<G.size(); i++)
	{
		for (int j=0; j<G.size(); j++)
		{
			H[i][j] = G[i][j];
		}
	}
	return H;
}

bool check_deg(pair<vector<vector<int>>, vector<int>> input, int K)
{
	vector<vector<int>> G = input.first;
	vector<int> OF = input.second;
	for (int i=0; i<G.size(); i++)
	{
		if (find(OF.begin(), OF.end(), i) != OF.end())
		{
			continue;
		}
		int summ = 0;
		for (int j=0; j<G.size(); j++)
		{
			summ += G[i][j];
		}
		if (summ<K)
		{
			return false;
		}
	}
	return true;
}

map<pair<vector<vector<int>>,vector<int>>, int> add_vertices(pair<vector<vector<int>>, vector<int>> input, int n)
{
	vector<vector<int>> G = input.first;
	vector<int> OF = input.second;
	int n2 = OF.size();
	map<pair<vector<vector<int>>,vector<int>>, int> extensions;
	// Loop over all 1st connecting points
	for (int i=1;i<n2;i++)
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
		vector<vector<int>> H = copy_G_p1(G);
		H[OF2[0]][n] = 1;
		H[n][OF2[0]] = 1;
		// Loop over all 2nd connecting points
		for (int j=1;j<n2;j++)
		{
			// Initialize new outer Face
			vector<int> OF3;
			OF3.push_back(OF2[0]);
			OF3.push_back(n);
			for (int k=j;k<n2;k++)
			{
				OF3.push_back(OF2[k]);
			}
			// Add edge to new 2nd connecting point
			H[n][OF2[j]] = 1;
			H[OF2[j]][n] = 1;
			pair<vector<vector<int>>,vector<int>> output (H,OF3);
			// Check interior vertices with degree less than 4
//			if ((n-1)>4)
//			{
//				if (check_deg(output,4))
//				{
//					extensions[output] = 1;
//				}
//			}
//			else
//			{
				extensions[output] = 1;
//			}
		}
	}
	return extensions;
}

void generate_graphs(map<pair<vector<vector<int>>,vector<int>>,int> Gs, int N, int n)
//map<pair<vector<vector<int>>,vector<int>>,int> generate_graphs(map<pair<vector<vector<int>>,vector<int>>,int> Gs, int N, int n)
{
	if (n+1>N)
	{
//		cout<<"finished\n";
//		return map<pair<vector<vector<int>>,vector<int>>,int> {};
		return;
	}
	map<pair<vector<vector<int>>,vector<int>>,int> new_Gs;
	#pragma omp parallel num_threads(4)
	{
	size_t cnt = 0;
	int ithread = omp_get_thread_num();
	int nthreads = omp_get_num_threads();
        //cout<<"\nThread "<<omp_get_thread_num()<<" of "<<omp_get_num_threads()<<" on cpu "<<sched_getcpu()<<"\n";
	//if (ithread == 0) cout<< "threads: "<< nthreads<<"\n";
	for (map<pair<vector<vector<int>>,vector<int>>,int>::iterator it = Gs.begin(); it != Gs.end(); ++it)
	{
		if(cnt%nthreads != ithread) continue;
		map<pair<vector<vector<int>>,vector<int>>,int> temp = add_vertices(it->first, n);
		//print_v(it->first.second);
		//cout << "size: " << temp.size() << "\n";
		new_Gs.insert(temp.begin(),temp.end());
	}
	}

	//cout << "--------------------\n";
	//cout << n << "\n";
	//cout << "--------------------\n";
	//cout << "size: " << new_Gs.size() << "\n";
	
	for (map<pair<vector<vector<int>>,vector<int>>,int>::iterator it = new_Gs.begin(); it != new_Gs.end(); ++it)
        {
		print_graph(it->first.first);
	}

	generate_graphs(new_Gs,N,n+1);
	//map<pair<vector<vector<int>>,vector<int>>,int> other = generate_graphs(new_Gs,N,n+1);
	//new_Gs.insert(other.begin(),other.end());
	//Gs.insert(new_Gs.begin(),new_Gs.end());
	
//	return Gs;
//	return new_Gs;
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

	vector<int> dumb2 = {1,1,0,0,1,0,1,0};

	int N = 8;
	//								 G           OF     val
	map<pair<vector<vector<int>>,vector<int>>,int> Gs = {  { { {{0,1}, {1,0}}, {0,1}}  , 1} };
	clock_t begin = clock();
	//map<pair<vector<vector<int>>,vector<int>>,int> tris = generate_graphs(Gs, N, 2);
	generate_graphs(Gs, N, 2);
	//vector<vector<vector<int>>> TRIS = keys(tris);
	clock_t end = clock();
	double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
	//cout << "elapsed time: "<<elapsed_secs<<"\n";
/*
	vector<vector<vector<int>>> tights = {};
	for (map<pair<vector<vector<int>>,vector<int>>,int>::iterator it = Gs.begin(); it != Gs.end(); ++it)
	{
                if (indep(it->first.first)==ceil(((float)(it->first.first).size())/((float)4)))
                {
                        //print_graph(TRIS[i]);
                        //cout << "\n\n";
                        tights.push_back(it->first.first);
                }

	}
*/
	//cout << tights.size() <<endl;
	//print_graph(tights[0]);
	/*
	vector<vector<vector<int>>> tights = {};
	int biggest = 0;
	vector<vector<int>> biggest_G; 
	
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
	*/
	//cout << "_______________\n";
	//for (map<pair<vector<vector<int>>,vector<int>>,int>::iterator it = tris.begin(); it != tris.end(); ++it)
	//{
//		if (it->first.first.size()>8)
//		{
//			print_graph(it->first.first);
//			cout << "_______________\n";
//		}
	//}

	//cout << "elapsed time: "<<elapsed_secs<<"\n";
	//cout << "graphs: "<<tris.size()<<"\n";
	return 0;
}












