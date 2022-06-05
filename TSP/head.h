#include <bits/stdc++.h>
using namespace std;

struct pt
{
    int id;
    double x,y;
};

struct item
{
    double weight, value, x, y, id;
};

void show_mat(vector<vector<item>> &M);

vector<item>& knapsack (vector <vector<vector<item>>>& used, vector<item>& it, int K);

void k_means(vector<vector<item>>& clusters, vector <item>& points, double mean);

pair<double, int> cvrp(vector<item>& pnt, int V, int c);

double dist( pt a, pt b );

class Path{
private:
    deque<pt> path;
    int num_of_nodes;

    bool last_improvement_status;
    void try_to_improve_2(int v1, int v2);

public:
    Path();
    void add_node( pt &new_node );
    double length();
    void local_search_2();
    void show_a();
    void show();
};
