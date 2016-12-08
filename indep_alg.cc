#include <iostream>                  // for cout
#include <utility>                   // for pair
#include <algorithm>                 // for for_each
#include <iterator>
#include <vector>
#include <map>

using namespace std;

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
}

vector<vector<int>> del_v(vector<int> Vs, vector<vector<int>> G)
{
	int n = G.size();
	vector<vector<int>> H = {};
	for (int i=0; i<(n); i++)
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

int main()
{
	//int N = 4;
	vector<vector<int>> G = {{0,1,1,1},{1,0,1,1},{1,1,0,1},{1,1,1,0}};
	print_graph(G);
	cout << indep(G) << "\n";
	return 0;
}












