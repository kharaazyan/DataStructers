#ifndef GRAPH_WEIGHTED_HPP
#define GRAPH_WEIGHTED_HPP

#include <vector>
#include <list>
#include <queue>
#include <stack>
#include <algorithm>
#include <iostream>
#include <functional>
#include <limits>

#define DIRECTED true

template<bool directed = false>
class graphListWeighted {
    std::vector<std::list<std::pair<int,int>>> graph;

    void _DFSUtil(int u, std::vector<bool>& visited);
    void _DFSUtil(int u, std::vector<bool>& visited, std::stack<int>& stack);
    void _DFSUtil(std::vector<std::vector<std::pair<int,int>>>& gr,
                  int u, std::vector<bool>& visited, std::vector<int>& component);
    int  _countPath(int cur, int dst, std::vector<bool>& visited);
    bool _hasCycle(int u, int parent, std::vector<bool>& visited);
    std::vector<std::vector<std::pair<int,int>>> _transpose();

public:
    graphListWeighted() = default;
    graphListWeighted(int size);

    void addVertex();
    void addEdge(int u, int v, int w = 1);
    std::vector<int> getShortestPathBFS(int src, int dst);
    std::vector<int> getPathDFS(int src, int dest);
    int getNumberOfNodesAtAGivenLevel(int start, int levelTarget);
    int getNumberOfComponents();
    int getNumberOfPathsFromSrcToDst(int src, int dst);
    void printGraph() const;
    void transpose();
    bool hasCycle();
    std::vector<int> topologicalSort();
    std::vector<std::vector<int>> kosarajuSCC();
    std::vector<std::vector<int>> tarjanSCC();
    std::vector<int> dijkstra(int source);
};

template<bool directed = false>
class graphMatrixWeighted {
    std::vector<std::vector<std::pair<int,int>>> graph;

    void _DFSUtil(int u, std::vector<bool>& visited);
    void _DFSUtil(int u, std::vector<bool>& visited, std::stack<int>& stack);
    void _DFSUtil(std::vector<std::vector<std::pair<int,int>>>& gr,
                  int u, std::vector<bool>& visited, std::vector<int>& component);
    int  _countPath(int cur, int dst, std::vector<bool>& visited);
    bool _hasCycle(int u, int parent, std::vector<bool>& visited);
    std::vector<std::vector<std::pair<int,int>>> _transpose();

public:
    graphMatrixWeighted() = default;
    graphMatrixWeighted(int size);

    void addVertex();
    void addEdge(int u, int v, int w = 1);
    std::vector<int> getShortestPathBFS(int src, int dst);
    std::vector<int> getPathDFS(int src, int dest);
    int getNumberOfNodesAtAGivenLevel(int start, int levelTarget);
    int getNumberOfComponents();
    int getNumberOfPathsFromSrcToDst(int src, int dst);
    void printGraph() const;
    void transpose();
    bool hasCycle();
    std::vector<int> topologicalSort();
    std::vector<std::vector<int>> kosarajuSCC();
    std::vector<std::vector<int>> tarjanSCC();
    std::vector<int> dijkstra(int source);
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
void graphListWeighted<directed>::addEdge(int u, int v, int w) {
    if(u >= 0 && u < graph.size() && v >= 0 && v < graph.size()) {
        graph[u].push_back({v, w});
        if constexpr (!directed) {
            graph[v].push_back({u, w});
        }
    }
}

template<bool directed>
void graphListWeighted<directed>::printGraph() const {
    for (int i = 0; i < graph.size(); i++) {
        std::cout << i << ": ";
        for (auto &edge : graph[i]) {
            int v = edge.first;
            int w = edge.second;
            std::cout << "(" << v << ", w=" << w << ") ";
        }
        std::cout << std::endl;
    }
}

template<bool directed>
std::vector<int> graphListWeighted<directed>::dijkstra(int source){
    static_assert(directed, "dijkstra is not supported for undirected graphs");
    const int INF = std::numeric_limits<int>::max();
    int size = graph.size();
    std::vector<int> dist(size, INF);
    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<std::pair<int, int>>> pq;
    dist[source] = 0;
    pq.push({0, source});
    
    while(!pq.empty()){
        auto [d, u] = pq.top();
        pq.pop();

        if(d > dist[u]) continue;
        for(const auto [v, w] : graph[u]){
            if(dist[u] != INF && dist[u] + w < dist[v]){
                dist[v] = dist[u] + w;
                pq.push({dist[v], v});
            }
        }
    }
    return dist;
}

template<bool directed>
std::vector<std::vector<int>> graphListWeighted<directed>::tarjanSCC(){
    static_assert(directed, "tarjanSCC is not supported for undirected graphs"); 
    int size = graph.size();
    std::vector<bool> onStack(size, false);
    std::vector<int> index(size, -1);
    std::vector<int> lowLink(size, 0);
    std::stack<int> s;
    int curIndex = 0;
    std::vector<std::vector<int>> scc;

    std::function<void(int)> strongconnect = [&](int u){
        lowLink[u] = index[u] = curIndex++;
        onStack[u] = true;
        s.push(u);

        for(auto &edge : graph[u]){
            int v = edge.first;
            if(index[v] == -1){
                strongconnect(v);
                lowLink[u] = std::min(lowLink[u], lowLink[v]);
            }
            else if(onStack[v]) {
                lowLink[u] = std::min(lowLink[u], index[v]);
            }
        }

        if(lowLink[u] == index[u]){
            std::vector<int> component;
            int v = -1;
            while(v != u){
                v = s.top();
                s.pop();
                onStack[v] = false;
                component.push_back(v);
            }
            scc.push_back(component);
        }
    };

    for (int i = 0; i < size; ++i) {
        if (index[i] == -1) {
            strongconnect(i);
        }
    }
    return scc;
}

template<bool directed>
std::vector<std::vector<int>> graphListWeighted<directed>::kosarajuSCC(){
    static_assert(directed, "kosarajuSCC is not supported for undirected graphs"); 
    int size = graph.size();
    std::stack<int> s;
    std::vector<bool> visited(size, false);

    for(int u = 0; u < size; ++u){
        if(!visited[u]) {
            _DFSUtil(u, visited, s);
        }
    }

    auto g = _transpose();

    std::fill(visited.begin(), visited.end(), false);
    std::vector<std::vector<int>> scc;
    while(!s.empty()){
        int u = s.top();
        s.pop();
        if(!visited[u]){
            std::vector<int> component;
            _DFSUtil(g, u, visited, component);
            scc.push_back(component);
        }
    }
    return scc;
}

template<bool directed>
std::vector<int> graphListWeighted<directed>::topologicalSort() {
    static_assert(directed, "topologicalSort is not supported for undirected graphs"); 
    int size = graph.size();
    std::queue<int> q;
    std::vector<int> inDegree(size, 0);
    std::vector<int> order;
    
    for(int u = 0; u < size; ++u){
        for(auto &edge : graph[u]){
            int v = edge.first;
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

        for(auto &edge : graph[u]){
            int v = edge.first;
            if(--inDegree[v] == 0){
                q.push(v);
            }
        }
    }

    if (order.size() != size) return {};
    return order;
}

template<bool directed>
bool graphListWeighted<directed>::_hasCycle(int u, int parent, std::vector<bool>& visited) {
    visited[u] = true;
    for(auto &edge : graph[u]){
        int v = edge.first;
        if(v != parent){
            if(visited[v]) return true;
            if(_hasCycle(v, u, visited)) return true;   
        }
    }
    return false;
}

template<bool directed>
bool graphListWeighted<directed>::hasCycle() {
    std::vector<bool> visited(graph.size(), false);
    if(graph.empty()) return false;
    visited[0] = true;
    return _hasCycle(0, -1, visited);
}

template<bool directed>
void graphListWeighted<directed>::_DFSUtil(int u, std::vector<bool>& visited) {
    visited[u] = true;
    for(auto &edge : graph[u]){
        int v = edge.first;
        if(!visited[v]) {
            _DFSUtil(v, visited);
        }
    }
}

template<bool directed>
void graphListWeighted<directed>::_DFSUtil(int u, std::vector<bool>& visited, std::stack<int>& stack) {
    visited[u] = true;
    for(auto &edge : graph[u]){
        int v = edge.first;
        if(!visited[v]) {
            _DFSUtil(v, visited, stack);
        }
    }
    stack.push(u);
}

template<bool directed>
void graphListWeighted<directed>::_DFSUtil(
    std::vector<std::vector<std::pair<int,int>>>& gr,
    int u,
    std::vector<bool>& visited,
    std::vector<int>& component)
{
    visited[u] = true;
    component.push_back(u);
    for(auto &edge : gr[u]){
        int v = edge.first;
        if(!visited[v]) {
            _DFSUtil(gr, v, visited, component);
        }
    }
}

template<bool directed>
std::vector<std::vector<std::pair<int,int>>> graphListWeighted<directed>::_transpose() {
    int size = graph.size();
    std::vector<std::vector<std::pair<int,int>>> newGraph(size);
    for (int u = 0; u < size; ++u) {
        for (auto &edge : graph[u]) {
            int v = edge.first;
            int w = edge.second;
            newGraph[v].push_back({u, w});
        }
    }
    return newGraph;
}

template<bool directed>
void graphListWeighted<directed>::transpose() {
    static_assert(directed, "transpose is not supported for undirected graphs"); 
    graph = std::move(_transpose());
}

template<bool directed>
int graphListWeighted<directed>::getNumberOfPathsFromSrcToDst(int src, int dst) {
    std::vector<bool> visited(graph.size(), false);
    return _countPath(src, dst, visited);
}

template<bool directed>
int graphListWeighted<directed>::_countPath(int cur, int dst, std::vector<bool>& visited) {
    if(cur == dst) return 1;
    int total = 0;
    visited[cur] = true;
    for(auto &edge : graph[cur]){
        int v = edge.first;
        if(!visited[v]){
            total += _countPath(v, dst, visited);
        }
    }
    visited[cur] = false;
    return total;
}

template<bool directed>
int graphListWeighted<directed>::getNumberOfComponents(){
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
std::vector<int> graphListWeighted<directed>::getShortestPathBFS(int src, int dst){
    static_assert(!directed, "getShortestPathBFS is not supported for directed graphs"); 
    int size = graph.size();
    if(src < 0 || src >= size || dst < 0 || dst >= size) return {};
    std::queue<int> q;
    std::vector<bool> visited(size, false);
    std::vector<int> parent(size, -1);

    visited[src] = true;
    q.push(src);
    while(!q.empty()){
        int u = q.front();
        q.pop();
        if(u == dst) break;
        for(auto &edge : graph[u]){
            int v = edge.first;
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
std::vector<int> graphListWeighted<directed>::getPathDFS(int src, int dest){
    static_assert(!directed, "getPathDFS is not supported for directed graphs"); 
    int size = graph.size();
    if(src < 0 || src >= size || dest < 0 || dest >= size) return {};
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
            int v = it->first;
            if(!visited[v]){
                s.push(v);
                visited[v] = true;
                parent[v] = u;
            }
        }
    }
    if(!found) return {};
    std::vector<int> path;
    for (int cur = dest; cur != -1; cur = parent[cur]){
        path.push_back(cur);
    }
    std::reverse(path.begin(), path.end());
    return path;
}

template<bool directed>
int graphListWeighted<directed>::getNumberOfNodesAtAGivenLevel(int start, int levelTarget){
    int size = graph.size();
    if(start < 0 || start >= size || levelTarget < 0) return 0;
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
            for(auto &edge : graph[u]){
                int v = edge.first;
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
void graphMatrixWeighted<directed>::addEdge(int u, int v, int w) {
    if(u >= 0 && u < graph.size() && v >= 0 && v < graph.size()) {
        graph[u].push_back({v, w});
        if constexpr (!directed) {
            graph[v].push_back({u, w});
        }
    }
}

template<bool directed>
void graphMatrixWeighted<directed>::printGraph() const {
    for (int i = 0; i < graph.size(); i++) {
        std::cout << i << ": ";
        for (auto &edge : graph[i]) {
            int v = edge.first;
            int w = edge.second;
            std::cout << "(" << v << ", w=" << w << ") ";
        }
        std::cout << std::endl;
    }
}

template<bool directed>
void graphMatrixWeighted<directed>::_DFSUtil(int u, std::vector<bool>& visited) {
    visited[u] = true;
    for(auto &edge : graph[u]){
        int v = edge.first;
        if(!visited[v]) {
            _DFSUtil(v, visited);
        }
    }
}

template<bool directed>
void graphMatrixWeighted<directed>::_DFSUtil(int u, std::vector<bool>& visited, std::stack<int>& stack) {
    visited[u] = true;
    for(auto &edge : graph[u]){
        int v = edge.first;
        if(!visited[v]) {
            _DFSUtil(v, visited, stack);
        }
    }
    stack.push(u);
}

template<bool directed>
void graphMatrixWeighted<directed>::_DFSUtil(
    std::vector<std::vector<std::pair<int,int>>>& gr,
    int u,
    std::vector<bool>& visited,
    std::vector<int>& component)
{
    visited[u] = true;
    component.push_back(u);
    for(auto &edge : gr[u]){
        int v = edge.first;
        if(!visited[v]) {
            _DFSUtil(gr, v, visited, component);
        }
    }
}

template<bool directed>
int graphMatrixWeighted<directed>::_countPath(int cur, int dst, std::vector<bool>& visited) {
    if(cur == dst) return 1;
    int total = 0;
    visited[cur] = true;
    for(auto &edge : graph[cur]){
        int v = edge.first;
        if(!visited[v]){
            total += _countPath(v, dst, visited);
        }
    }
    visited[cur] = false;
    return total;
}

template<bool directed>
bool graphMatrixWeighted<directed>::_hasCycle(int u, int parent, std::vector<bool>& visited) {
    visited[u] = true;
    for(auto &edge : graph[u]){
        int v = edge.first;
        if(v != parent){
            if(visited[v]) return true;
            if(_hasCycle(v, u, visited)) return true;   
        }
    }
    return false;
}

template<bool directed>
bool graphMatrixWeighted<directed>::hasCycle() {
    std::vector<bool> visited(graph.size(), false);
    if(graph.empty()) return false;
    visited[0] = true;
    return _hasCycle(0, -1, visited);
}

template<bool directed>
std::vector<std::vector<std::pair<int,int>>> graphMatrixWeighted<directed>::_transpose() {
    int size = graph.size();
    std::vector<std::vector<std::pair<int,int>>> newGraph(size);
    for (int u = 0; u < size; ++u) {
        for (auto &edge : graph[u]) {
            int v = edge.first;
            int w = edge.second;
            newGraph[v].push_back({u, w});
        }
    }
    return newGraph;
}

template<bool directed>
void graphMatrixWeighted<directed>::transpose() {
    static_assert(directed, "transpose is not supported for undirected graphs");
    graph = std::move(_transpose());
}

template<bool directed>
int graphMatrixWeighted<directed>::getNumberOfPathsFromSrcToDst(int src, int dst) {
    std::vector<bool> visited(graph.size(), false);
    return _countPath(src, dst, visited);
}

template<bool directed>
int graphMatrixWeighted<directed>::getNumberOfComponents(){
    static_assert(!directed, "getNumberOfComponents is not supported for directed graphs"); 
    int size = graph.size();
    int components = 0;
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
std::vector<int> graphMatrixWeighted<directed>::getShortestPathBFS(int src, int dst){
    static_assert(!directed, "getShortestPathBFS is not supported for directed graphs"); 
    int size = graph.size();
    if(src < 0 || src >= size || dst < 0 || dst >= size) return {};
    std::queue<int> q;
    std::vector<bool> visited(size, false);
    std::vector<int> parent(size, -1);

    visited[src] = true;
    q.push(src);
    while(!q.empty()){
        int u = q.front();
        q.pop();
        if(u == dst) break;
        for(auto &edge : graph[u]){
            int v = edge.first;
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
int graphMatrixWeighted<directed>::getNumberOfNodesAtAGivenLevel(int start, int levelTarget){
    int size = graph.size();
    if(start < 0 || start >= size || levelTarget < 0) return 0;
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
            for(auto &edge : graph[u]){
                int v = edge.first;
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
std::vector<int> graphMatrixWeighted<directed>::getPathDFS(int src, int dest){
    static_assert(!directed, "getPathDFS is not supported for directed graphs"); 
    int size = graph.size();
    if(src < 0 || src >= size || dest < 0 || dest >= size) return {};
    std::stack<int> s;
    std::vector<bool> visited(size, false);
    std::vector<int> parent(size, -1);

    visited[src] = true;
    s.push(src);
    bool found = false;
    while(!s.empty() && !found){
        int u = s.top();
        s.pop();
        if(u == dest){
            found = true;
            break;
        }
        for(auto it = graph[u].rbegin(); it != graph[u].rend(); ++it){
            int v = it->first;
            if(!visited[v]){
                s.push(v);
                visited[v] = true;
                parent[v] = u;
            }
        }
    }
    if(!found) return {};
    std::vector<int> path;
    for (int cur = dest; cur != -1; cur = parent[cur]) {
        path.push_back(cur);
    }
    std::reverse(path.begin(), path.end());
    return path;
}

template<bool directed>
std::vector<int> graphMatrixWeighted<directed>::topologicalSort() {
    static_assert(directed, "topologicalSort is not supported for undirected graphs"); 
    int size = graph.size();
    std::queue<int> q;
    std::vector<int> inDegree(size, 0);
    std::vector<int> order;
    
    for(int u = 0; u < size; ++u){
        for(auto &edge : graph[u]){
            int v = edge.first;
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

        for(auto &edge : graph[u]){
            int v = edge.first;
            if(--inDegree[v] == 0){
                q.push(v);
            }
        }
    }

    if (order.size() != size) return {};
    return order;
}

template<bool directed>
std::vector<std::vector<int>> graphMatrixWeighted<directed>::tarjanSCC(){
    static_assert(directed, "tarjanSCC is not supported for undirected graphs"); 
    int size = graph.size();
    std::vector<bool> onStack(size, false);
    std::vector<int> index(size, -1);
    std::vector<int> lowLink(size, 0);
    std::stack<int> s;
    int curIndex = 0;
    std::vector<std::vector<int>> scc;

    std::function<void(int)> strongconnect = [&](int u){
        lowLink[u] = index[u] = curIndex++;
        onStack[u] = true;
        s.push(u);

        for(auto &edge : graph[u]){
            int v = edge.first;
            if(index[v] == -1){
                strongconnect(v);
                lowLink[u] = std::min(lowLink[u], lowLink[v]);
            }
            else if(onStack[v]) {
                lowLink[u] = std::min(lowLink[u], index[v]);
            }
        }

        if(lowLink[u] == index[u]){
            std::vector<int> component;
            int v = -1;
            while(v != u){
                v = s.top();
                s.pop();
                onStack[v] = false;
                component.push_back(v);
            }
            scc.push_back(component);
        }
    };

    for (int i = 0; i < size; ++i) {
        if (index[i] == -1) {
            strongconnect(i);
        }
    }
    return scc;
}

template<bool directed>
std::vector<std::vector<int>> graphMatrixWeighted<directed>::kosarajuSCC(){
    static_assert(directed, "kosarajuSCC is not supported for undirected graphs"); 
    int size = graph.size();
    std::stack<int> s;
    std::vector<bool> visited(size, false);

    for(int u = 0; u < size; ++u){
        if(!visited[u]) {
            _DFSUtil(u, visited, s);
        }
    }
    auto g = _transpose();
    std::fill(visited.begin(), visited.end(), false);
    std::vector<std::vector<int>> scc;
    while(!s.empty()){
        int u = s.top();
        s.pop();
        if(!visited[u]){
            std::vector<int> component;
            _DFSUtil(g, u, visited, component);
            scc.push_back(component);
        }
    }
    return scc;
}

template<bool directed>
std::vector<int> graphMatrixWeighted<directed>::dijkstra(int source){
    static_assert(directed, "dijkstra is not supported for undirected graphs");
    const int INF = std::numeric_limits<int>::max();
    int size = graph.size();
    std::vector<int> dist(size, INF);
    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<std::pair<int, int>>> pq;
    dist[source] = 0;
    pq.push({0, source});
    
    while(!pq.empty()){
        auto [d, u] = pq.top();
        pq.pop();

        if(d > dist[u]) continue;
        for(const auto [v, w] : graph[u]){
            if(dist[u] + w < dist[v]){
                dist[v] = dist[u] + w;
                pq.push({dist[v], v});
            }
        }
    }
    return dist;
}

#endif // GRAPH_WEIGHTED_HPP