//
//  main.cpp
//  lab 3
//
//  Created by Максим Палёхин on 19.11.2020.
//

#include <iostream>
#include <iomanip>
#include <set>
#include <vector>
#include <string>
#include <algorithm>
#include <typeinfo>
#include <fstream>
#include <stack>
#include <queue>

using namespace std;

class Rib
{
public:
    int from, to, weight;
    Rib(int from = -1, int to = -1, int weight = 0) : from(from), to(to), weight(weight) {}
    Rib(const Rib& E)
    {
        from = E.from;
        to = E.to;
        weight = E.weight;
    }
    int operator<(Rib& E)
    {
        return (weight < E.weight);
    }
    friend ostream& operator<<(ostream& s, Rib& e);
};

ostream& operator<<(ostream& s, Rib& e)
{
    s << "от: " << e.from << ", до: " << e.to << ", вес: " << e.weight;
    return s;
}


void PrintMatrix(vector<std::vector<int> >& matrix) {
    std::for_each(matrix.begin(), matrix.end(), [](std::vector<int>& ivec)
        {
            std::for_each(ivec.begin(), ivec.end(), [](int i)
                {
                    std::cout << std::left << std::setw(5) << i;
                });
            std::cout << std::endl;
        });
}

vector<vector<int>> FromTreeToMat(vector<Rib> edges)
{
    vector<vector<int>> res(13, vector <int>(13, 0));
    for (int i = 0; i < 13; i++)
    {
        res[edges[i].from][edges[i].to] = edges[i].weight;
        res[edges[i].to][edges[i].from] = edges[i].weight;
    }
    return res;
}

vector<int> MinimumPath(vector<vector<int>> mat, int st, int V)
{
    vector<int> distance (V,0);
    int count, index;
    int i;
    vector<bool> visited(V,false);
    for (i = 0; i < V; i++)
    {
        distance[i] = INT_MAX;
        visited[i] = false;
    }
    distance[st] = 0;
    for (count = 0; count < V - 1; count++)
    {
        int min = INT_MAX;
        for (i = 0; i < V; i++)
            if (!visited[i] && distance[i] <= min)
            {
                min = distance[i];
                index = i;
            }
        visited[index] = true;
        for (i = 0; i < V; i++)
            if (!visited[i] && mat[index][i] && distance[index] != INT_MAX &&
                distance[index] + mat[index][i] < distance[i])
                distance[i] = distance[index] + mat[index][i];
    }

    return distance;
}

vector<int> DFS(vector<vector<int>> mat, int V) {
    vector<int> used(V, 0);
    for (int i = 0; i < V; i++) {
        used[i] = -2;
    }
    //-2 ничего не видели
    //-1 видели вершину, но не прошли
    //>= 0  прошли вершину - сколько ребер пройдено.
    stack<int> Stack;
    Stack.push(0);
    used[0] = 0;
    int steps = 0;
    while (!Stack.empty()) {
        int node = Stack.top();
        Stack.pop();
        if (used[node] > 0)
            continue;
        used[node] = steps;
        //проход по строке с индеком node, найти все 1.
        for (int j = V-1; j >= 0; j--) {
            if (mat[node][j]>0 && used[j] < 0) {   //есть путь и мы там не были.
                Stack.push(j);
                used[j] = -1;
            }


        }
        steps++;
        std::cout << "\nDFS пройдена " << node << " вершина";
    }
    return used;
}

vector <int> BFS(vector<vector<int>> mat, int V) {
    vector <int> used(V, -2);
    vector <int> dist(V, 10000);
    vector <int> res(V, 0);
    //-2 ничего не видели
    //-1 видели вершину, но не прошли
    //>= 0  прошли вершину - сколько ребер пройдено.
    queue<int> Queue;
    Queue.push(0);
    used[0] = 0;
    int steps = 0;
    while (!Queue.empty()) {
        int node = Queue.front();
        Queue.pop();
        if (used[node] > 0)
            continue;
        used[node] = steps;
        for (int j = V-1; j >= 0; j--)
        {
            if (mat[node][j] > 0 && used[j] > -1)
            {
                res[node]++;
            }
            if (mat[node][j] > 0 && used[j] < 0)
            {   //есть путь и мы там не были.
                Queue.push(j);
                used[j] = -1;
                res[node]++;
            }
        }
        steps++;
    }
    return res;
}

double PowAverage(vector <int> res, int V)
{
    double result = 0;
    for (int i = 0; i < V; i++)
    {
        result += res[i];
    }
    result /= V;
    return result;
}

vector<Rib> MinOstov(vector<vector<int>> mat, int V)
{
    int curWeight = 0;     //Текущий вес
    vector <int> used(V, 0); //использованные вершины
    vector<Rib> edges;        //рассматриваемые ребра
    vector<Rib> tree_edges;    //ребра в минимальном остове

    edges.push_back(Rib(0, 0, 0));


    while (!edges.empty())
    {
        //поиск минимального ребра
        Rib minedge = edges[0];
        int minindex = 0;

        for (int i = 1; i < edges.size(); i++)
        {
            Rib cur_edge = edges[i];
            if (cur_edge < minedge)
            {
                minedge = cur_edge;
                minindex = i;
            }
        }

        edges.erase(edges.begin() + minindex);
        int to = minedge.to;
        if (used[to] == 1)
        {
            continue;
        }
        //пометили обработанную вершину
        used[to] = 1;
        //нашли минимальное ребро
        curWeight += minedge.weight;
        tree_edges.push_back(minedge);

        //перечисляем все вершины, выходящие из to
        for (int i = 0; i < V; i++)
        {
            //необработанные вершины
            if (mat[to][i] > 0 && !used[i])
            {
                edges.push_back(Rib(to, i, mat[to][i]));
            }
        }
    }
    std::cout << "Минимальный вес остовного дерева: " << curWeight << endl;
    return tree_edges;
}

int main()
{

    vector<vector<int>> matrix =
    {
    { 0, 0, 0, 7, 6, 0, 1, 4, 4, 5, 6, 6, 7 },
    { 0, 0, 6, 4, 4, 1, 2, 3, 2, 0, 1, 4, 2 },
    { 0, 6, 0, 8, 8, 4, 3, 6, 5, 3, 6, 6, 5 },
    { 7, 4, 8, 0, 5, 4, 5, 8, 0, 9, 3, 6, 8 },
    { 6, 4, 8, 5, 0, 1, 4, 2, 7, 7, 7, 2, 0 },
    { 0, 1, 4, 4, 1, 0, 7, 4, 4, 2, 4, 2, 6 },
    { 1, 2, 3, 5, 4, 7, 0, 7, 1, 2, 2, 9, 8 },
    { 4, 3, 6, 8, 2, 4, 7, 0, 8, 4, 2, 3, 2 },
    { 4, 2, 5, 0, 7, 4, 1, 8, 0, 2, 5, 8, 1 },
    { 5, 0, 3, 9, 7, 2, 2, 4, 2, 0, 5, 9, 7 },
    { 6, 1, 6, 3, 7, 4, 2, 2, 5, 5, 0, 9, 6 },
    { 6, 4, 6, 6, 2, 2, 9, 3, 8, 9, 9, 0, 5 },
    { 7, 2, 5, 8, 0, 6, 8, 2, 1, 7, 6, 5, 0 },
    };

    cout << "\n\nЛаба 3.1\n\n";

    vector<Rib> treeEdges = MinOstov(matrix, 13);
    for (int i = 1; i < treeEdges.size(); i++)
    {
        cout << "\nРебро " << i << " ";
        cout << treeEdges[i];
    }

    vector<vector<int>> mat1 = FromTreeToMat(treeEdges);
    
    cout << endl << endl << "Матрица смежности для минимального остова " << endl;
    PrintMatrix(mat1);

    cout << "\n\nЛаба 3.2\n\n";

    vector<int> DFSRes = DFS(mat1, 13);
    cout << endl << "Перечисление всех вершин в минимальном остовном дереве ";
    for (int i = 0; i < DFSRes.size(); i++)
    {
        cout << endl << DFSRes[i] << " ";
    }

    cout << "\n\nЛаба 3.3\n\n";

    const int MPathNumber = 5;
    vector<int> MPath = MinimumPath(mat1, MPathNumber, 12);
    cout << "Минимальный путь между " << MPathNumber << " и всеми остальными пунктами, куда можно построить маршрут:\t\n";
    for (int i = 0; i < 12; i++)
        if (MPath[i] != INT_MAX)
            cout << MPathNumber << " > " << i << " = " << MPath[i] << endl;
        else cout << MPathNumber << " > " << i << " = " << "Маршрут недоступен" << endl;

    cout << "\n\nЛаба 3.4\n\n";

    vector <int> PowRes = BFS(mat1,12);
    cout << endl;
    for (int i = 0; i < PowRes.size(); ++i)
    {
        cout << "Степень " << i << " - " << PowRes[i] << endl;
    }
    double PowAverageRes = PowAverage(PowRes,12);
    cout << endl << "Среднее значение степени: " << PowAverageRes << endl;

    return 0;

}
