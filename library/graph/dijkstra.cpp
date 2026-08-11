// Dijkstra on a non-negative weighted graph. O((n + m) log n).
// g[u] = { {v, w}, ... }; returns dist[] with INF for unreachable nodes.
// Verified: CSES 1671 (Shortest Routes I).
vector<long long> dijkstra(const vector<vector<pair<int, int>>>& g, int src) {
    const long long INF = numeric_limits<long long>::max() / 4;
    vector<long long> dist(g.size(), INF);
    priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<>> pq;
    dist[src] = 0;
    pq.push({0, src});
    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();
        if (d != dist[u]) continue;
        for (auto [v, w] : g[u])
            if (d + w < dist[v]) pq.push({dist[v] = d + w, v});
    }
    return dist;
}
