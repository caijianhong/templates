template <class Cap>
struct mf_graph {
  struct edge {
    int v, rid;
    Cap cap;
  };
  int n;
  vector<vector<edge>> g;
  mf_graph() = default;
  mf_graph(int _n) : n(_n), g(_n) {}
  void addedge(int u, int v, Cap cap) {
    int fid = (int)g[u].size(), tid = (int)g[v].size() + (u == v);
    g[u].push_back({v, tid, cap});
    g[v].push_back({u, fid, 0});
  }
  vector<int> dep, cur;
  bool bfs(int s, int t) {
    dep.assign(n, -1);
    queue<int> q;
    dep[s] = 0;
    q.push(s);
    while (!q.empty()) {
      int u = q.front(); q.pop();
      for (auto&& e : g[u]) {
        if (e.cap && dep[e.v] == -1) {
          dep[e.v] = dep[u] + 1, q.push(e.v);
        }
      }
    }
    return dep[t] != -1;
  }
  Cap dfs(int u, Cap flw, int t) {
    if (u == t) return flw;
    Cap res = 0;
    for (int &i = cur[u]; i < (int)g[u].size(); i++) {
      auto &e = g[u][i];
      if (e.cap && dep[e.v] == dep[u] + 1) {
        Cap run = dfs(e.v, min(flw - res, e.cap), t);
        e.cap -= run, g[e.v][e.rid].cap += run;
        res += run;
        if (res == flw) return res;
      }
    }
    dep[u] = -1;
    return res;
  }
  Cap flow(int s, int t, Cap lim = numeric_limits<Cap>::max()) {
    Cap flw = 0;
    while (flw < lim && bfs(s, t)) {
      cur.assign(n, 0);
      flw += dfs(s, lim - flw, t);
    }
    return flw;
  }
};
