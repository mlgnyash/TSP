#include "head.h"

using namespace std;

double dist( pt a, pt b ){
    return sqrt( pow( a.x - b.x, 2 ) + pow( a.y - b.y, 2 ) );
}

void show_mat(vector<vector<item>> &M)
{
    int N=M.size();
    for (int i=0; i<N; i++)
    {
        for (int j=0; j<M[i].size(); j++)
        {
            cout<< M[i][j].id<<" ";
        }
        cout<<endl;
    }
    cout<<endl;
}

vector<item>& knapsack (vector <vector<vector<item>>>& used, vector<item>& it, int K)
{
    int N=it.size();
    vector<vector<int>> w(K+1, vector<int>(N+1));
    //vector <vector<vector<item>>> used(K+1, vector<vector<item>>(N+1));
    for (int i=0; i<N; i++)
    {
        for (int j=0; j<K; j++)
        {
            if (it[i].weight<=j+1)
            {
                if (w[j+1][i]>=w[j+1-it[i].weight][i]+it[i].value)
                {
                    w[j+1][i+1]=w[j+1][i];
                    used[j+1][i+1]=used[j+1][i];
                }
                else
                {
                    w[j+1][i+1]=w[j+1-it[i].weight][i]+it[i].value;
                    used[j+1][i+1]=used[j+1-it[i].weight][i];
                    used[j+1][i+1].push_back(it[i]);
                }
            }
            else
            {
                w[j+1][i+1]=w[j+1][i];
                used[j+1][i+1]=used[j+1][i];
            }
        }
    }
    return used[K][N];
}


void k_means(vector<vector<item>>& clusters, vector <item>& points, double mean)
{
    int N=points.size();
    //vector<vector<item>> clusters(2);
    for (int i=0; i<N; i++)
    {
        if (points[i].x>mean)
            clusters[0].push_back(points[i]);
        else
            clusters[1].push_back(points[i]);
    }
    clusters[0].push_back(points[0]);
    clusters[1].push_back(points[0]);
    //return clusters;
}

pair<double, int> cvrp(vector<item>& pnt, int V, int c)
{
    //каждый грузовик воображаем рюкзаком, каждую потребность - предметом, вес равен ценности
    vector<vector<item>>usedused;//наборы городов для каждого грузовика
    vector<item>p=pnt;
    double ANS=0;
    for (int i=0; i<V; i++)
    {
            int N=p.size();
            int K=c;
            vector <vector<vector<item>>> used(K+1, vector<vector<item>>(N+1));
            //vector<item>& knap=knapsack(used, p, c);
            usedused.push_back(knapsack(used, p, c));
            used.clear();
            for (int j=0; j<usedused[i].size(); j++)
                {
                    for (int k=0; k<p.size(); k++)
                    {
                        if (usedused[i][j].id==p[k].id)
                            p.erase(p.begin()+k);
                    }
                }
            if (p.size()==0)
                break;
    }
    V=V+2-usedused.size();
    double ans=0;

    for (int i=0; i<usedused.size(); i++)
    {
        Path cur;
        for (int j=0; j<usedused[i].size(); j++)
        {
             pt newone = {usedused[i][j].id, usedused[i][j].x, usedused[i][j].y};
   	         cur.add_node(newone);
        }
        cur.local_search_2();
        ans+=cur.length();
    }
    ANS+=ans;
    //cout <<endl<< "ANS="<< ans;
    return {ANS, V};
}

/*
double cvrp(vector<item>& points, int V, int c)
{
    double ANS=0;
    vector<vector<item>>usedused;//наборы городов для каждого грузовика
    vector<item>p=points;//clusters[i];
    for (int i=0; i<V; i++)
    {
            usedused.push_back(knapsack(p, c));
            for (int j=0; j<usedused[i].size(); j++)
                {
                    for (int k=0; k<p.size(); k++)
                    {
                        if (usedused[i][j].id==p[k].id)
                            p.erase(p.begin()+k);
                    }
                }
            if (p.size()==0)
                break;
    }
    V=V+2-usedused.size();
    double ans=0;

    for (int i=0; i<usedused.size(); i++)
    {
        Path cur;
        for (int j=0; j<usedused[i].size(); j++)
        {
             pt newone = {usedused[i][j].id, usedused[i][j].x, usedused[i][j].y};
   	         cur.add_node(newone);
        }
        cur.local_search_2();
        ans+=cur.length();
        //cout << cur.length() << endl;
        //cur.show();
        //cout << cur.length() << endl;
    }
    ANS+=ans;
    return ANS;
    //cout <<endl<< "ANS="<< ANS;
}*/


Path::Path(){
    num_of_nodes = 0;
    last_improvement_status = false;
}

void Path::add_node( pt &new_node ){
    if( num_of_nodes > 1 ){
        path.push_back( path[0] );
        deque<pt>::iterator it = path.begin(),
                            here = it+1;
        double best_len = dist( *it, new_node ) + dist( new_node, *(it+1) );
        it++;

        for(int i=0; i<num_of_nodes-1; i++){
            double tmp = dist( *it, new_node ) + dist( new_node, *(it+1) );
            if( tmp < best_len ){
                best_len = tmp;
                here = it+1;
            }
            it++;
        }

        path.emplace( here, new_node );
        path.pop_back();
    }
    else
        path.push_back( new_node );

    num_of_nodes++;
}

void Path::try_to_improve_2(int v1, int v2){

    if(v1>v2) swap(v1,v2);

    double init_dist =
          dist( path[v1], path[v1+1] )
        + dist( path[v2], path[v2+1] );

    // improval attempt
    double new_dist = dist( path[v1], path[v2] ) + dist( path[v1+1], path[v2+1] );
    if( init_dist > new_dist ){
        deque<pt>::iterator it1 = next( path.begin(), v1+1 );
        deque<pt>::iterator it2 = next( it1, v2-v1 );
        reverse( it1, it2 );
        last_improvement_status = true;
    }
}

double Path::length(){
    double len = 0;
    for(int i=0; i<num_of_nodes; i++)
        len += dist( path[i], path[(i+1)%num_of_nodes] );

    return len;
}

void Path::local_search_2(){
    path.push_back( path[0] );
    for(int i=0; i<num_of_nodes-2; i++)
        for(int j=i+2; j<num_of_nodes; j++){

            try_to_improve_2(i,j);

            if(last_improvement_status){
                last_improvement_status = false;
                j = i+2;
            }
        }
    path.pop_back();
}

void Path::show_a(){
    for(auto p: path)
        cout<<p.id<<": ( "<<p.x<<", "<<p.y<<" )\n";
}

void Path::show(){
    for(auto p: path)
        cout<<p.id<<" ";
}

