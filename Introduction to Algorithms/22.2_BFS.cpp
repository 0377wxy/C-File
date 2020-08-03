// 广度优先搜索

#include <iostream>
#include <vector>
#include <map>
#include <string>
using namespace std;

class Edge
{
private:
    int num;
    string key;
    int weight;

public:
    Edge(int u, string k, int w = 0)
    {
        num = u;
        key = k;
        weight = w;
    }
    friend class Figure;
};

class Point
{
    friend class Figure;

private:
    string key;
    int d = 0;
    string color;
    int father;

public:
    Point(string k)
    {
        key = k;
        color = "white";
        father = -1;
    }
};

class Figure
{
private:
    map<string, int> key_n;
    vector<Point> points;
    map<int, vector<Edge>> figure;

public:
    void add_Point(string k)
    {
        points.push_back(Point(k));
        key_n.insert(pair<string, int>(k, points.size() - 1));
        figure.insert(pair<int, vector<Edge>>(points.size() - 1, vector<Edge>()));
    }
    void add_Edge(string k1, string k2, int w = 0)
    {
        figure[key_n[k1]].push_back(Edge(key_n[k2], k2, w));
        figure[key_n[k2]].push_back(Edge(key_n[k1], k1, w));
    }
    void BFS(string k)
    {
        vector<int> Q;
        Q.push_back(key_n[k]);
        points[key_n[k]].color = "gray";
        while (Q.size() > 0)
        {
            int u = Q[0];
            for (Edge v : figure[u])
            {
                if (points[v.num].color == "white")
                {
                    points[v.num].color = "gray";
                    points[v.num].d = points[u].d + 1;
                    points[v.num].father = u;
                    Q.push_back(v.num);
                }
            }
            Q.erase(Q.begin());
            points[u].color = "black";
        }
    }
};

int main()
{
    Figure fig;
    fig.add_Point("r");
    fig.add_Point("v");
    fig.add_Point("s");
    fig.add_Point("w");
    fig.add_Point("t");
    fig.add_Point("x");
    fig.add_Point("u");
    fig.add_Point("y");
    fig.add_Edge("v", "r");
    fig.add_Edge("r", "s");
    fig.add_Edge("w", "x");
    fig.add_Edge("w", "t");
    fig.add_Edge("t", "u");
    fig.add_Edge("x", "y");
    fig.add_Edge("x", "u");
    fig.add_Edge("y", "u");
    fig.add_Edge("s", "w");
    fig.add_Edge("t", "x");
    fig.BFS("s");
    return 0;
}