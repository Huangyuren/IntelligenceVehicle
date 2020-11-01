#include <bits/stdc++.h>

using namespace std;
void printGraph(vector<vector<int>> &graph) {
    for(int i=0; i<graph.size(); ++i) {
        for(int j=0; j<graph[i].size(); ++j) {
            printf("%d, ", graph[i][j]);
        }
        printf("\n");
    }
}
void dfsCheck(vector<vector<int>> &graph, vector<int> &color, vector<pair<int, int>> &remove_edge, int currnode, int prenode) {
    color[currnode] = 1;
    for(int j=0; j<graph[currnode].size(); ++j) {
        int adjnode = graph[currnode][j];
        if(color[adjnode] == 0) {
            dfsCheck(graph, color, remove_edge, adjnode, currnode);
        }else if(color[adjnode] == 1) {
            remove_edge.emplace_back(make_pair(currnode, adjnode));
        }
    }
    color[currnode] = 2;
}
void detectCycle(vector<vector<int>> &graph, vector<int> &color, vector<pair<int, int>> &remove_edge) {
    for(int i=0; i<graph.size(); ++i) {
        if(color[i] == 0) {
            dfsCheck(graph, color, remove_edge, i, i);
        }
    }
}
int main(int argc, char* argv[]) {
    int nodes=0, edges=0, src, dst;
    cin >> nodes;
    cin >> edges;
    nodes++;
    vector<vector<int>> graph(nodes);
    vector<int> color(nodes, 0); // white: 0, gray: 1, black: 2
    vector<pair<int, int>> remove_edge;
    for(int i=0; i<nodes; ++i) {
        cin >> src;
        cin >> dst;
        graph[src].push_back(dst);
    }
    detectCycle(graph, color, remove_edge);
    for(int i=0; i<remove_edge.size(); ++i) {
        printf("(%d, %d)\n", remove_edge[i].first, remove_edge[i].second);
    }
    printf("pair count: %lu\n", remove_edge.size());
    return 0;
}
