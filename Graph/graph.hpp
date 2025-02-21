#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <vector>
#include <list>
#include <queue>
#include <stack>
#include <algorithm>
#include <iostream>
#include <stdexcept>

#define DIRECTED true

template<bool directed = false>
class graphList {
    std::vector<std::list<int>> graph;
    void _DFSUtil(int u, std::vector<bool>& visited);
    int _countPath(int cur, int dst, std::vector<bool>& visited);
    bool _hasCycle(int u, int parent, std::vector<bool>& visited);
public:
    graphList() = default;
    graphList(int size);
    void addVertex();
    void addEdge(int u, int v);
    std::vector<int> getShortestPathBFS(int src, int dst);
    std::vector<int> getPathDFS(int src, int dest);
    int getNumberOfNodesAtAGivenLevel(int start, int levelTarget);
    int getNumberOfComponents();
    int getNumberOfPathsFromSrcToDst(int src, int dst);
    void printGraph() const;
    void transpose();
    bool hasCycle();
    std::vector<int> topologicalSort();
};

template<bool directed = false>
class graphMatrix{
    std::vector<std::vector<int>> graph;
    void _DFSUtil(int u, std::vector<bool>& visited);
    int _countPath(int cur, int dst, std::vector<bool>& visited);
    bool _hasCycle(int u, int parent, std::vector<bool>& visited);
public:
    graphMatrix() = default;;
    graphMatrix(int size);
    void addVertex();
    void addEdge(int u, int v);
    std::vector<int> getShortestPathBFS(int src, int dst);
    std::vector<int> getPathDFS(int src, int dest);
    int getNumberOfNodesAtAGivenLevel(int start, int levelTarget);
    int getNumberOfComponents();
    int getNumberOfPathsFromSrcToDst(int src, int dst);
    void printGraph() const;
    void transpose();
    bool hasCycle();
    std::vector<int> topologicalSort();
};

template<bool directed>
graphList<directed>::graphList(int size) {
    while(size-- > 0){
        addVertex();
    }
}

template<bool directed>
std::vector<int> graphList<directed>::topologicalSort() {
    static_assert(directed, "topologicalSort is not supported for undirected graphs"); 
    int size = graph.size();
    std::queue<int> q;
    std::vector<int>inDegree(size, 0);
    std::vector<int> order;
    
    for(int u = 0; u < size; ++u){
        for(int v : graph[u]){
            ++inDegree[v];
        }
    }
    for(int u = 0; u < size; ++u){
        if(inDegree[u] == 0){
            q.push(u);
        }
    }
    
    while(!q.empty()){
        int u = q.front();
        q.pop();
        order.push_back(u);

        for(int v : graph[u]){
            if(--inDegree[v] == 0){
                q.push(v);
            }
        }
    }

    if (order.size() != size) {
        throw std::runtime_error("Graph has at least one cycle");
    }
    return order;
}

template<bool directed>
bool graphList<directed>::_hasCycle(int u, int parent, std::vector<bool>& visited) {
    visited[u] = true;
    for(int v : graph[u]){
        if(v != parent){
            if(visited[v]) return true;
            if(_hasCycle(v, u, visited)) return true;   
        }
    }
    return false;
}

template<bool directed>
bool graphList<directed>::hasCycle() {
    std::vector<bool> visited(graph.size(), false);
    visited[0] = true;
    return _hasCycle(0, -1, visited);
}

template<bool directed>
void graphList<directed>::_DFSUtil(int u, std::vector<bool>& visited) {
    visited[u] = true;
    for(int v : graph[u]){
        if(!visited[v]) _DFSUtil(v, visited);
    }
}

template<bool directed>
void graphList<directed>::transpose() {
    static_assert(directed, "transpose is not supported for undirected graphs"); 
    int size = graph.size();
    decltype(graph) newGraph(size);
    for (int u = 0; u < size; ++u) {
        for (int v : graph[u]) {
            newGraph[v].push_back(u);
        }
    }
    graph = std::move(newGraph);
}

template<bool directed>
int graphList<directed>::getNumberOfPathsFromSrcToDst(int src, int dst) {
    std::vector<bool> visited(graph.size(), false);
    return _countPath(src, dst, visited);
}

template<bool directed>
int graphList<directed>::_countPath(int cur, int dst, std::vector<bool>& visited) {
    if(cur == dst) return 1;
    int total = 0;
    visited[cur] = true;
    for(int v : graph[cur]){
        if(!visited[v]){
            total += _countPath(v, dst, visited);
        }
    }
    visited[cur] = false;
    return total;
}

template<bool directed>
int graphList<directed>::getNumberOfComponents(){
    static_assert(!directed, "getNumberOfComponents is not supported for directed graphs"); 
    int size = graph.size();
    int components {0};
    std::vector<bool> visited(size, false);
    for(int i = 0; i < size; ++i){
        if(!visited[i]){
            ++components;
            _DFSUtil(i, visited);
        }
    }
    return components;
}

template<bool directed>
void graphList<directed>::addVertex() {
    graph.push_back({});
}

template<bool directed>
void graphList<directed>::addEdge(int u, int v) {
    if(u >= 0 && u < graph.size() && v >= 0 && v < graph.size()) {
        graph[u].push_back(v);
        if constexpr (!directed) {
            graph[v].push_back(u);
        }
    }
}

template<bool directed>
std::vector<int> graphList<directed>::getShortestPathBFS(int src, int dst){
    static_assert(!directed, "getShortestPathBFS is not supported for directed graphs"); 
    int size = graph.size();
    std::queue<int> q;
    std::vector<bool> visited(size, false);
    std::vector<int> parent(size, -1);
    if(!(src >= 0 && src < size && dst >= 0 && dst < size)) return {};
    visited[src] = true;
    q.push(src);
    while(!q.empty()){
        int u = q.front();
        q.pop();

        if(u == dst) break;
        for(auto v : graph[u]){
            if(!visited[v]){
                visited[v] = true;
                parent[v] = u;
                q.push(v);
            }
        }
    }
    if(!visited[dst]) return {};
    std::vector<int> path;
    for(int cur = dst; cur != -1; cur = parent[cur]){
        path.push_back(cur);
    }
    std::reverse(path.begin(), path.end());
    return path;
}

template<bool directed>
std::vector<int> graphList<directed>::getPathDFS(int src, int dest){
    static_assert(!directed, "getPathDFS is not supported for directed graphs"); 
    int size = graph.size();
    std::stack<int> s;
    std::vector<bool> visited(size, false);
    std::vector<int> parent(size, -1);
    bool found = false;
    visited[src] = true;
    s.push(src);
    while(!s.empty() && !found){
        int u = s.top();
        s.pop();
        if(u == dest){
            found = true;
            break;
        }
        for(auto it = graph[u].rbegin(); it != graph[u].rend(); ++it){
            int v = *it;
            if(!visited[v]){
                s.push(v);
                visited[v] = true;
                parent[v] = u;
            }
        }
    }
    if(!found) return {};
    std::vector<int> path;
    for (int cur = dest; cur != -1; cur = parent[cur])
        path.push_back(cur);
    
    std::reverse(path.begin(), path.end());
    return path;
}

template<bool directed>
void graphList<directed>::printGraph() const {
    for (int i = 0; i < graph.size(); i++) {
        std::cout << i << ": ";
        for (int v : graph[i])
            std::cout << v << " ";
        std::cout << std::endl;
    }
}

template<bool directed>
int graphList<directed>::getNumberOfNodesAtAGivenLevel(int start, int levelTarget){
    int size = graph.size();
    if(start < 0 || levelTarget < 0 || start >= size) return 0;
    std::vector<bool> visited(size, false);
    std::queue<int> q;
    visited[start] = true;
    q.push(start);
    int currentlvl = 0;
    while(!q.empty() && currentlvl < levelTarget){
        int lvlsize = q.size();
        for(int i = 0; i < lvlsize; ++i){
            int u = q.front();
            q.pop();
            for(auto v : graph[u]){
                if(!visited[v]){
                    visited[v] = true;
                    q.push(v);

                }
            }
        }
        ++currentlvl;
    }
    return q.size();
}

template<bool directed>
graphMatrix<directed>::graphMatrix(int size) {
    while(size-- > 0){
        addVertex();
    }
}

template<bool directed>
void graphMatrix<directed>::_DFSUtil(int u, std::vector<bool>& visited) {
    visited[u] = true;
    for(int v : graph[u]){
        if(!visited[v]) _DFSUtil(v, visited);
    }
}

template<bool directed>
void graphMatrix<directed>::addVertex() {
    graph.push_back({});
}

template<bool directed>
void graphMatrix<directed>::addEdge(int u, int v) {
    if(u >= 0 && u < graph.size() && v >= 0 && v < graph.size()) {
        graph[u].push_back(v);
        if constexpr (!directed) {
            graph[v].push_back(u);
        }
    }
}

template<bool directed>
std::vector<int> graphMatrix<directed>::getShortestPathBFS(int src, int dst){
    static_assert(!directed, "getShortestPathBFS is not supported for directed graphs"); 
    int size = graph.size();
    std::queue<int> q;
    std::vector<bool> visited(size, false);
    std::vector<int> parent(size, -1);
    if(!(src >= 0 && src < size && dst >= 0 && dst < size)) return {};
    visited[src] = true;
    q.push(src);
    while(!q.empty()){
        int u = q.front();
        q.pop();

        if(u == dst) break;
        for(auto v : graph[u]){
            if(!visited[v]){
                visited[v] = true;
                parent[v] = u;
                q.push(v);
            }
        }
    }
    if(!visited[dst]) return {};
    std::vector<int> path;
    for(int cur = dst; cur != -1; cur = parent[cur]){
        path.push_back(cur);
    }
    std::reverse(path.begin(), path.end());
    return path;
}

template<bool directed>
int graphMatrix<directed>::getNumberOfNodesAtAGivenLevel(int start, int levelTarget){
    int size = graph.size();
    if(start < 0 || levelTarget < 0 || start >= size) return 0;
    std::vector<bool> visited(size, false);
    std::queue<int> q;
    visited[start] = true;
    q.push(start);
    int currentlvl = 0;
    while(!q.empty() && currentlvl < levelTarget){
        int lvlsize = q.size();
        for(int i = 0; i < lvlsize; ++i){
            int u = q.front();
            q.pop();
            for(auto v : graph[u]){
                if(!visited[v]){
                    visited[v] = true;
                    q.push(v);

                }
            }
        }
        ++currentlvl;
    }
    return q.size();
}

template<bool directed>
std::vector<int> graphMatrix<directed>::getPathDFS(int src, int dest){
    static_assert(!directed, "getPathDFS is not supported for directed graphs"); 
    int size = graph.size();
    std::stack<int> s;
    std::vector<bool> visited(size, false);
    std::vector<int> parent(size, -1);
    bool found = false;
    visited[src] = true;
    s.push(src);
    while(!s.empty() && !found){
        int u = s.top();
        s.pop();
        if(u == dest){
            found = true;
            break;
        }
        for(auto it = graph[u].rbegin(); it != graph[u].rend(); ++it){
            int v = *it;
            if(!visited[v]){
                s.push(v);
                visited[v] = true;
                parent[v] = u;
            }
        }
    }
    if(!found) return {};
    std::vector<int> path;
    for (int cur = dest; cur != -1; cur = parent[cur])
        path.push_back(cur);
    
    std::reverse(path.begin(), path.end());
    return path;
}

template<bool directed>
int graphMatrix<directed>::getNumberOfComponents(){
    static_assert(!directed, "getNumberOfComponents is not supported for directed graphs"); 
    int size = graph.size();
    int components {0};
    std::vector<bool> visited(size, false);
    for(int i = 0; i < size; ++i){
        if(!visited[i]){
            ++components;
            _DFSUtil(i, visited);
        }
    }
    return components;
}

template<bool directed>
int graphMatrix<directed>::getNumberOfPathsFromSrcToDst(int src, int dst) {
    std::vector<bool> visited(graph.size(), false);
    return _countPath(src, dst, visited);
}

template<bool directed>
int graphMatrix<directed>::_countPath(int cur, int dst, std::vector<bool>& visited) {
    if(cur == dst) return 1;
    int total = 0;
    visited[cur] = true;
    for(int v : graph[cur]){
        if(!visited[v]){
            total += _countPath(v, dst, visited);
        }
    }
    visited[cur] = false;
    return total;
}

template<bool directed>
void graphMatrix<directed>::printGraph() const {
    for (int i = 0; i < graph.size(); i++) {
        std::cout << i << ": ";
        for (int v : graph[i])
            std::cout << v << " ";
        std::cout << std::endl;
    }
}

template<bool directed>
void graphMatrix<directed>::transpose() {
    static_assert(directed, "transpose is not supported for undirected graphs"); 
    int size = graph.size();
    decltype(graph) newGraph(size);
    for (int u = 0; u < size; ++u) {
        for (int v : graph[u]) {
            newGraph[v].push_back(u);
        }
    }
    graph = std::move(newGraph);
}

template<bool directed>
bool graphMatrix<directed>::_hasCycle(int u, int parent, std::vector<bool>& visited) {
    visited[u] = true;
    for(int v : graph[u]){
        if(v != parent){
            if(visited[v]) return true;
            if(_hasCycle(v, u, visited)) return true;   
        }
    }
    return false;
}

template<bool directed>
bool graphMatrix<directed>::hasCycle() {
    std::vector<bool> visited(graph.size(), false);
    visited[0] = true;
    return _hasCycle(0, -1, visited);
}

template<bool directed>
std::vector<int> graphMatrix<directed>::topologicalSort() {
    static_assert(directed, "topologicalSort is not supported for undirected graphs"); 
    int size = graph.size();
    std::queue<int> q;
    std::vector<int>inDegree(size, 0);
    std::vector<int> order;
    
    for(int u = 0; u < size; ++u){
        for(int v : graph[u]){
            ++inDegree[v];
        }
    }
    for(int u = 0; u < size; ++u){
        if(inDegree[u] == 0){
            q.push(u);
        }
    }
    
    while(!q.empty()){
        int u = q.front();
        q.pop();
        order.push_back(u);

        for(int v : graph[u]){
            if(--inDegree[v] == 0){
                q.push(v);
            }
        }
    }

    if (order.size() != size) {
        throw std::runtime_error("Graph has at least one cycle");
    }
    return order;
}

#endif // GRAPH_HPP