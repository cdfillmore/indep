#include <iostream>               // for cout
#include <utility>                 // for pair
#include <algorithm>             // for for_each
#include <iterator>
#include <vector>
#include <map>
#include <ctime>
#include <math.h>
#include <typeinfo>

using namespace std;


// Print Vector
void print_vector(vector<int> v)
{
        for (int i=0; i<v.size(); i++)
        {
                cout << " " << v[i];
        }
        cout << "\n";
}

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

int test(map<int,int> m)
{
	m[1] = 1;
	cout << (m.begin()->first) << "\n";
	return 1;
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

int num_edges(vector<vector<int>> G)
{
	int sum;
	for (int i=0; i<G.size(); i++)
	{
		for (int j=0; j<G.size(); j++)
		{
			sum += G[i][j];
		}
	}
	return sum;
}

bool is_iso(vector<vector<int>> G, vector<vector<int>> H)
{
	if (G.size() != H.size()) return false;
	if (num_edges(G) != num_edges(H)) return false;
	
	vector<int> deg_count1 (0, G.size());
	vector<int> deg_count2 (0, G.size());
	for (int i=0; i<G.size(); i++)
	{
		int sum1;
		int sum2;
		for (int j=0; j<G.size(); j++)
		{
			sum1+= G[i][j];
			sum2+= H[i][j];
		}
		deg_count1[sum1]++;
		deg_count2[sum2]++;
	}
	if (deg_count1 != deg_count2) return false;
	
	
	vector<int> matched1 = {};
	vector<int> matched2 = {};
	for (int i=0; i<G.size(); i++)
	{
		for (int j=0; j<G.size(); j++)
		{
			
		}
	}
	return true;
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

int main()
{
	vector<vector<int>> G = { {0,1,1}, {1,0,0}, {1,0,0} };
	vector<vector<int>> H = { {0,1,1}, {1,0,0}, {1,0,0} };
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
        vector<vector<int>> dumb2 =
        {
                {0,1,0,1,1,0,0,1,0},
                {1,0,1,0,0,0,1,1,0},
                {0,1,0,1,0,1,1,0,0},
                {1,0,1,0,1,1,0,0,0},
                {1,0,0,1,0,1,0,1,0},
                {0,0,1,1,1,0,1,0,0},
                {0,1,1,0,0,1,0,1,0},
                {1,1,0,0,1,0,1,0,1},
                {0,0,0,0,0,0,0,1,0}
        };


	cout << indep(dumb1) << endl;
	cout << indep(dumb2) << endl;
	cout << indep(G) << endl;
	cout << indep(H) << endl;
	
	return 0;
}

