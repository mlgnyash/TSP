#include "head.h"

using namespace std;

int main()
{
    int m, l, k, p;
    cin >> m >> l >> k;
    vector <item> points(m);
    double mean = 0;
    for (p = 0; p < m; p++)
    {
        cin >> points[p].weight >> points[p].x >> points[p].y;
        points[p].value = points[p].weight;
        points[p].id = p;
        mean += points[p].x / m;
    }
    vector<vector<item>> clusters(2);
    k_means(clusters, points, mean);
    double ANS = 0;
    if (m < 250)
    {
        for (p = 0; p < clusters.size(); p++)
        {
            ANS += c(clusters[p], l, k).first;
            l = c(clusters[p], l, k).second;
        }
        cout << endl << "ANS=" << ANS;
    }
    else
    {
        ANS = c(points, l, k).first;
        cout << endl << "ANS=" << ANS;
    }
    return 0;
}