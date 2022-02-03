#include <iostream>
#include <fstream>
#include<vector>
#include<queue>
#include<list>

using namespace std;

#define INF 2000000000

void dijkstra(int src, const vector<list<pair<int, int>>>& adj_list, vector<int>& distance, vector<list<int>>& predecessor)
{
	int vertices = adj_list.size();
	vector<bool> visited(vertices, false);

	predecessor[src].push_back(src);
	distance[src] = 0;

	priority_queue<pair<int, int>,
		vector<pair<int, int>>,
		greater<pair<int, int>>> p_queue;

	p_queue.push({ 0, src });

	while (!p_queue.empty())
	{
		int current_node = p_queue.top().second;
		p_queue.pop();

		if (visited[current_node])
			continue;

		visited[current_node] = true;

		for (auto iterator : adj_list[current_node])
		{
			int connected_node = iterator.first;
			int weight = iterator.second;
			if (distance[current_node] + weight < distance[connected_node])
			{
				distance[connected_node] = distance[current_node] + weight;
				predecessor[connected_node].clear();
				predecessor[connected_node].push_back(current_node);
				p_queue.push({ distance[connected_node], connected_node });
			}
			else if (distance[current_node] + weight == distance[connected_node])
			{
				predecessor[connected_node].push_back(current_node);
			}
		}
	}
	
}

void calc_degree_centrality(const vector<list<pair<int, int>>>& adj_list, vector<int>& degree_centrality)
{
	int n = adj_list.size();
	for (int i = 0; i < n; i++)
	{
		int degree = 0;
		for (auto it : adj_list[i])
		{
			degree++;
		}
		degree_centrality[i] = degree;
	}
}

void calc_closeness_centrality(int n, const vector<vector<int>>& distance, vector<double>& closeness_centrality)
{
	for (int i = 0; i < n; i++)
	{
		double sum = 0;
		for (int j = 0; j < n; j++)
		{
			sum += distance[i][j];
		}
		closeness_centrality[i] = (double)(n - 1) / sum;
	}
}

void rec(int src, int des, int node, const vector<list<int>>& predecessor, int& count_paths, int& count_node_on_paths, bool& on_path)
{
	if (des == src)
	{
		count_paths++;
		if (on_path == true)
			count_node_on_paths++;
		return;
	}

	if (des == node)
		on_path = true;

	for (auto iterator : predecessor[des])
	{
		rec(src, iterator, node, predecessor, count_paths, count_node_on_paths, on_path);
	}

	if (des == node)
		on_path = false;
}

void calc_betweeness_centrality(int n, const vector<vector<list<int>>>& predecessor, vector<double>& betweenness_centrality)
{
	for (int node = 0; node < n ; node++)
	{
		for (int src = 0; src < n; src++)
		{
			if (src == node)
			{
				continue;
			}
			for (int des = 0; des < n; des++)
			{
				if (des == node || des == src)
				{
					continue;
				}
				int count_paths = 0;
				int count_node_on_paths = 0;
				bool on_path = false;
				rec(src, des, node, predecessor[src], count_paths, count_node_on_paths, on_path);
				betweenness_centrality[node] += ((double)count_node_on_paths) / ((double)count_paths);
			}
		}
		betweenness_centrality[node] = betweenness_centrality[node] / 2;
	}
}

int main(void)
{
	char z;

	cout << "If you want to visualize press 'v' else 'n': ";
	cin >> z;

	ofstream myfile;

	cout << "Enter nodes and edges: ";
	int n, m;
	cin >> n >> m;

	if (n <= 100 && z == 'v')
	{
		myfile.open("graph.dot");
		myfile << "\ngraph G {\n";
	}

	vector<list<pair<int, int>>> adj_list(n);
	for (int i = 0; i < m ;i++)
	{
		int a, b, c;
		cin >> a >> b >> c;
		adj_list[a].push_back({ b,c });
		adj_list[b].push_back({ a,c });
		
		if (n <= 100 && z == 'v')
		{
			myfile << "\t" << a << " -- " << b << " [label = \"" << c << "\"];\n";
		}
	}
	
	if (n <= 100 && z == 'v')
	{
		myfile << "}";
		myfile.close();
		system("dot -Tpng graph.dot -o graph.dot.png");
		system("graph.dot.png");
	}

	cout << "For (degree/closeness/betweenness) press ('d'/'c'/'b') : ";
	cin >> z;

	vector<vector<int>> distance(n, vector<int>(n, INF));
	vector<vector<list<int>>> predecessor(n, vector<list<int>>(n));

	if (z == 'c' || z == 'b')
	{
		for (int i = 0; i < n; i++)
		{
			dijkstra(i, adj_list, distance[i], predecessor[i]);
		}
	}

	if (z == 'd')
	{
		vector<int> degree_centrality(n);
		calc_degree_centrality(adj_list, degree_centrality);

		for (int i = 0; i < n; i++)
		{
			cout << degree_centrality[i] << "\n";
		}
	}

	if (z == 'c')
	{
		vector<double> closeness_centrality(n, 0);
		calc_closeness_centrality(n, distance, closeness_centrality);

		for (int i = 0; i < n; i++)
		{
			printf("%.12lf\n", closeness_centrality[i]);
		}
	}
	
	if (z == 'b')
	{
		vector<double> betweenness_centrality(n, 0);
		calc_betweeness_centrality(n, predecessor, betweenness_centrality);

		for (int i = 0; i < n; i++)
		{
			printf("%.12lf\n", betweenness_centrality[i]);
		}
	}
	
	
	return 0;
}
