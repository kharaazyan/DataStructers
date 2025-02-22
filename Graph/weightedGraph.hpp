#ifndef WEIGHTED_GRAPH_LIST_HPP
#define WEIGHTED_GRAPH_LIST_HPP

#include <vector>
#include <list>
#include <queue>
#include <stack>
#include <algorithm>
#include <iostream>
#include <stdexcept>

#define DIRECTED true

template<bool directed = false>
class graphListWeighted {
    std::vector<std::list<std::pair<int,int>>> graph;

    void _DFSUtil(int u, std::vector<bool>& visited);
    int  _countPath(int cur, int dst, std::vector<bool>& visited);
    bool _hasCycle(int u, int parent, std::vector<bool>& visited);

public:
    graphListWeighted() = default;
    graphListWeighted(int size);
    void addVertex();
    void addEdge(int u, int v, int weight);
    //std::vector<int> getShortestPathBFS(int src, int dst);
    std::vector<int> getPathDFS(int src, int dest);
    int  getNumberOfNodesAtAGivenLevel(int start, int levelTarget);
    int  getNumberOfComponents();
    int  getNumberOfPathsFromSrcToDst(int src, int dst);
    void printGraph() const;
    void transpose();
    bool hasCycle();
    std::vector<int> topologicalSort();
};

template<bool directed = false>
class graphMatrixWeighted {
    std::vector<std::vector<std::pair<int,int>>> graph;

    void _DFSUtil(int u, std::vector<bool>& visited);
    int  _countPath(int cur, int dst, std::vector<bool>& visited);
    bool _hasCycle(int u, int parent, std::vector<bool>& visited);

public:
    graphMatrixWeighted() = default;
    graphMatrixWeighted(int size);
    void addVertex();
    void addEdge(int u, int v, int weight);
    //std::vector<int> getShortestPathBFS(int src, int dst);
    std::vector<int> getPathDFS(int src, int dest);
    int  getNumberOfNodesAtAGivenLevel(int start, int levelTarget);
    int  getNumberOfComponents();
    int  getNumberOfPathsFromSrcToDst(int src, int dst);
    void printGraph() const;
    void transpose();
    bool hasCycle();
    std::vector<int> topologicalSort();
};

template<bool directed>
graphListWeighted<directed>::graphListWeighted(int size) {
    while(size-- > 0){
        addVertex();
    }
}

template<bool directed>
void graphListWeighted<directed>::addVertex() {
    graph.push_back({});
}

template<bool directed>
void graphListWeighted<directed>::addEdge(int u, int v, int weight) {
    if(u >= 0 && u < (int)graph.size() && v >= 0 && v < graph.size()) {
        graph[u].push_back({v, weight});
        if constexpr (!directed) {
            graph[v].push_back({u, weight});
        }
    }
}

template<bool directed>
void graphListWeighted<directed>::printGraph() const {
    for (int i = 0; i < (int)graph.size(); i++) {
        std::cout << i << ": ";
        for (auto &edge : graph[i]) {
            std::cout << "(" << edge.first << ", w=" << edge.second << ") ";
        }
        std::cout << std::endl;
    }
}

template<bool directed>
void graphListWeighted<directed>::_DFSUtil(int u, std::vector<bool>& visited) {
    visited[u] = true;
    for(auto &edge : graph[u]) {
        int v = edge.first;
        if(!visited[v]) {
            _DFSUtil(v, visited);
        }
    }
}

template<bool directed>
int graphListWeighted<directed>::_countPath(int cur, int dst, std::vector<bool>& visited) {
    if(cur == dst) return 1;
    int total = 0;
    visited[cur] = true;
    for(auto &edge : graph[cur]) {
        int v = edge.first;
        if(!visited[v]) {
            total += _countPath(v, dst, visited);
        }
    }
    visited[cur] = false;
    return total;
}

template<bool directed>
int graphListWeighted<directed>::getNumberOfPathsFromSrcToDst(int src, int dst) {
    std::vector<bool> visited(graph.size(), false);
    return _countPath(src, dst, visited);
}

template<bool directed>
bool graphListWeighted<directed>::_hasCycle(int u, int parent, std::vector<bool>& visited) {
    visited[u] = true;
    for(auto &edge : graph[u]) {
        int v = edge.first;
        if(v == parent) continue;
        if(visited[v]) return true;
        if(_hasCycle(v, u, visited)) return true;
    }
    return false;
}

template<bool directed>
bool graphListWeighted<directed>::hasCycle() {
    static_assert(!directed, "hasCycle() is supported only for undirected graphs");
    if(graph.empty()) return false;
    std::vector<bool> visited(graph.size(), false);
    return _hasCycle(0, -1, visited);
}

template<bool directed>
void graphListWeighted<directed>::transpose() {
    static_assert(directed, "transpose() is supported only for directed graphs");
    int size = graph.size();
    decltype(graph) newGraph(size);

    for (int u = 0; u < size; ++u) {
        for (auto &edge : graph[u]) {
            int v      = edge.first;
            int weight = edge.second;
            newGraph[v].push_back({u, weight});
        }
    }
    graph = std::move(newGraph);
}

template<bool directed>
std::vector<int> graphListWeighted<directed>::topologicalSort() {
    static_assert(directed, "topologicalSort() is supported only for directed graphs");

    int size = graph.size();
    std::vector<int> inDegree(size, 0);

    for(int u = 0; u < size; ++u) {
        for(auto &edge : graph[u]) {
            ++inDegree[ edge.first ];
        }
    }

    std::queue<int> q;
    for(int u = 0; u < size; ++u) {
        if(inDegree[u] == 0) {
            q.push(u);
        }
    }

    std::vector<int> order;
    while(!q.empty()) {
        int u = q.front();
        q.pop();
        order.push_back(u);

        for(auto &edge : graph[u]) {
            int v = edge.first;
            if(--inDegree[v] == 0) {
                q.push(v);
            }
        }
    }

    if(order.size() != size) {
        throw std::runtime_error("Graph has at least one cycle");
    }
    return order;
}

template<bool directed>
int graphListWeighted<directed>::getNumberOfComponents() {
    static_assert(!directed, "getNumberOfComponents() is not supported for directed graphs");
    int size = (int)graph.size();
    int components = 0;
    std::vector<bool> visited(size, false);

    for(int i = 0; i < size; ++i) {
        if(!visited[i]) {
            ++components;
            _DFSUtil(i, visited);
        }
    }
    return components;
}

// template<bool directed>
// std::vector<int> graphListWeighted<directed>::getShortestPathBFS(int src, int dst) {
//     static_assert(!directed, "getShortestPathBFS() is not supported for directed graphs");

//     int size = (int)graph.size();
//     if(src < 0 || src >= size || dst < 0 || dst >= size) return {};

//     std::vector<bool> visited(size, false);
//     std::vector<int> parent(size, -1);
//     std::queue<int> q;

//     visited[src] = true;
//     q.push(src);

//     while(!q.empty()) {
//         int u = q.front();
//         q.pop();
//         if(u == dst) break;

//         for(auto &edge : graph[u]) {
//             int v = edge.first;
//             if(!visited[v]) {
//                 visited[v] = true;
//                 parent[v] = u;
//                 q.push(v);
//             }
//         }
//     }

//     if(!visited[dst]) return {};
//     std::vector<int> path;
//     for(int cur = dst; cur != -1; cur = parent[cur]) {
//         path.push_back(cur);
//     }
//     std::reverse(path.begin(), path.end());
//     return path;
// }

template<bool directed>
std::vector<int> graphListWeighted<directed>::getPathDFS(int src, int dest) {
    static_assert(!directed, "getPathDFS() is not supported for directed graphs");

    int size = graph.size();
    if(src < 0 || src >= size || dest < 0 || dest >= size) return {};

    std::stack<int> s;
    std::vector<bool> visited(size, false);
    std::vector<int> parent(size, -1);
    bool found = false;

    visited[src] = true;
    s.push(src);

    while(!s.empty() && !found) {
        int u = s.top();
        s.pop();
        if(u == dest) {
            found = true;
            break;
        }
        for(auto it = graph[u].rbegin(); it != graph[u].rend(); ++it) {
            int v = it->first;
            if(!visited[v]) {
                s.push(v);
                visited[v] = true;
                parent[v] = u;
            }
        }
    }

    if(!found) return {};
    std::vector<int> path;
    for(int cur = dest; cur != -1; cur = parent[cur]) {
        path.push_back(cur);
    }
    std::reverse(path.begin(), path.end());
    return path;
}

template<bool directed>
int graphListWeighted<directed>::getNumberOfNodesAtAGivenLevel(int start, int levelTarget) {
    int size = graph.size();
    if(start < 0 || start >= size || levelTarget < 0) return 0;

    std::vector<bool> visited(size, false);
    std::queue<int> q;
    visited[start] = true;
    q.push(start);

    int currentLevel = 0;
    while(!q.empty() && currentLevel < levelTarget) {
        int levelSize = (int)q.size();
        for(int i = 0; i < levelSize; ++i) {
            int u = q.front();
            q.pop();
            for(auto &edge : graph[u]) {
                int v = edge.first;
                if(!visited[v]) {
                    visited[v] = true;
                    q.push(v);
                }
            }
        }
        ++currentLevel;
    }
    return q.size();
}

template<bool directed>
graphMatrixWeighted<directed>::graphMatrixWeighted(int size) {
    while(size-- > 0){
        addVertex();
    }
}

template<bool directed>
void graphMatrixWeighted<directed>::addVertex() {
    graph.push_back({});
}

template<bool directed>
void graphMatrixWeighted<directed>::addEdge(int u, int v, int weight) {
    if(u >= 0 && u < (int)graph.size() && v >= 0 && v < graph.size()) {
        graph[u].push_back({v, weight});
        if constexpr (!directed) {
            graph[v].push_back({u, weight});
        }
    }
}

template<bool directed>
void graphMatrixWeighted<directed>::printGraph() const {
    for (int i = 0; i < (int)graph.size(); i++) {
        std::cout << i << ": ";
        for (auto &edge : graph[i]) {
            std::cout << "(" << edge.first << ", w=" << edge.second << ") ";
        }
        std::cout << std::endl;
    }
}

template<bool directed>
void graphMatrixWeighted<directed>::_DFSUtil(int u, std::vector<bool>& visited) {
    visited[u] = true;
    for(auto &edge : graph[u]) {
        int v = edge.first;
        if(!visited[v]) {
            _DFSUtil(v, visited);
        }
    }
}

template<bool directed>
int graphMatrixWeighted<directed>::_countPath(int cur, int dst, std::vector<bool>& visited) {
    if(cur == dst) return 1;
    int total = 0;
    visited[cur] = true;
    for(auto &edge : graph[cur]) {
        int v = edge.first;
        if(!visited[v]) {
            total += _countPath(v, dst, visited);
        }
    }
    visited[cur] = false;
    return total;
}

template<bool directed>
int graphMatrixWeighted<directed>::getNumberOfPathsFromSrcToDst(int src, int dst) {
    std::vector<bool> visited(graph.size(), false);
    return _countPath(src, dst, visited);
}

template<bool directed>
bool graphMatrixWeighted<directed>::_hasCycle(int u, int parent, std::vector<bool>& visited) {
    visited[u] = true;
    for(auto &edge : graph[u]) {
        int v = edge.first;
        if(v == parent) continue;
        if(visited[v]) return true;
        if(_hasCycle(v, u, visited)) return true;
    }
    return false;
}

template<bool directed>
bool graphMatrixWeighted<directed>::hasCycle() {
    static_assert(!directed, "hasCycle() is supported only for undirected graphs");
    if(graph.empty()) return false;
    std::vector<bool> visited(graph.size(), false);
    return _hasCycle(0, -1, visited);
}

template<bool directed>
void graphMatrixWeighted<directed>::transpose() {
    static_assert(directed, "transpose() is supported only for directed graphs");
    int size = graph.size();
    decltype(graph) newGraph(size);

    for (int u = 0; u < size; ++u) {
        for (auto &edge : graph[u]) {
            int v      = edge.first;
            int weight = edge.second;
            newGraph[v].push_back({u, weight});
        }
    }
    graph = std::move(newGraph);
}

template<bool directed>
std::vector<int> graphMatrixWeighted<directed>::topologicalSort() {
    static_assert(directed, "topologicalSort() is supported only for directed graphs");

    int size = graph.size();
    std::vector<int> inDegree(size, 0);

    for(int u = 0; u < size; ++u) {
        for(auto &edge : graph[u]) {
            ++inDegree[ edge.first ];
        }
    }

    std::queue<int> q;
    for(int u = 0; u < size; ++u) {
        if(inDegree[u] == 0) {
            q.push(u);
        }
    }

    std::vector<int> order;
    while(!q.empty()) {
        int u = q.front();
        q.pop();
        order.push_back(u);

        for(auto &edge : graph[u]) {
            int v = edge.first;
            if(--inDegree[v] == 0) {
                q.push(v);
            }
        }
    }

    if(order.size() != size) {
        throw std::runtime_error("Graph has at least one cycle");
    }
    return order;
}

template<bool directed>
int graphMatrixWeighted<directed>::getNumberOfComponents() {
    static_assert(!directed, "getNumberOfComponents() is not supported for directed graphs");
    int size = (int)graph.size();
    int components = 0;
    std::vector<bool> visited(size, false);

    for(int i = 0; i < size; ++i) {
        if(!visited[i]) {
            ++components;
            _DFSUtil(i, visited);
        }
    }
    return components;
}

// template<bool directed>
// std::vector<int> graphMatrixWeighted<directed>::getShortestPathBFS(int src, int dst) {
//     static_assert(!directed, "getShortestPathBFS() is not supported for directed graphs");

//     int size = (int)graph.size();
//     if(src < 0 || src >= size || dst < 0 || dst >= size) return {};

//     std::vector<bool> visited(size, false);
//     std::vector<int> parent(size, -1);
//     std::queue<int> q;

//     visited[src] = true;
//     q.push(src);

//     while(!q.empty()) {
//         int u = q.front();
//         q.pop();
//         if(u == dst) break;

//         for(auto &edge : graph[u]) {
//             int v = edge.first;
//             if(!visited[v]) {
//                 visited[v] = true;
//                 parent[v] = u;
//                 q.push(v);
//             }
//         }
//     }

//     if(!visited[dst]) return {};
//     std::vector<int> path;
//     for(int cur = dst; cur != -1; cur = parent[cur]) {
//         path.push_back(cur);
//     }
//     std::reverse(path.begin(), path.end());
//     return path;
// }

template<bool directed>
std::vector<int> graphMatrixWeighted<directed>::getPathDFS(int src, int dest) {
    static_assert(!directed, "getPathDFS() is not supported for directed graphs");

    int size = graph.size();
    if(src < 0 || src >= size || dest < 0 || dest >= size) return {};

    std::stack<int> s;
    std::vector<bool> visited(size, false);
    std::vector<int> parent(size, -1);
    bool found = false;

    visited[src] = true;
    s.push(src);

    while(!s.empty() && !found) {
        int u = s.top();
        s.pop();
        if(u == dest) {
            found = true;
            break;
        }
        for(auto it = graph[u].rbegin(); it != graph[u].rend(); ++it) {
            int v = it->first;
            if(!visited[v]) {
                s.push(v);
                visited[v] = true;
                parent[v] = u;
            }
        }
    }

    if(!found) return {};
    std::vector<int> path;
    for(int cur = dest; cur != -1; cur = parent[cur]) {
        path.push_back(cur);
    }
    std::reverse(path.begin(), path.end());
    return path;
}

template<bool directed>
int graphMatrixWeighted<directed>::getNumberOfNodesAtAGivenLevel(int start, int levelTarget) {
    int size = graph.size();
    if(start < 0 || start >= size || levelTarget < 0) return 0;

    std::vector<bool> visited(size, false);
    std::queue<int> q;
    visited[start] = true;
    q.push(start);

    int currentLevel = 0;
    while(!q.empty() && currentLevel < levelTarget) {
        int levelSize = (int)q.size();
        for(int i = 0; i < levelSize; ++i) {
            int u = q.front();
            q.pop();
            for(auto &edge : graph[u]) {
                int v = edge.first;
                if(!visited[v]) {
                    visited[v] = true;
                    q.push(v);
                }
            }
        }
        ++currentLevel;
    }
    return q.size();
}

#endif // WEIGHTED_GRAPH_LIST_HPP
