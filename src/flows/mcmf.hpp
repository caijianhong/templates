template <class T>
using pqueue = priority_queue<T, vector<T>, greater<T>>;
template <class Cap, class Cst>
struct mcf_graph {
  inline static constexpr auto MAXC = numeric_limits<Cst>::max();
  struct edge {
    int v, rid;
    Cap cap;
    Cst cst;
  };
  int n;
  vector<vector<edge>> g;
  mcf_graph() = default;
  mcf_graph(int _n) : n(_n), g(_n) {}
  void add(int u, int v, Cap cap, Cst cst) {
    int fid = (int)g[u].size(), tid = (int)g[v].size() + (u == v);
    g[u].push_back({v, tid, cap, cst});
    g[v].push_back({u, fid, 0, -cst});
  }
  vector<Cst> h, dis;
  vector<int> vis;
  bool spfa(int s) {
    vis.assign(n, false);
    h.assign(n, MAXC);
    vector<int> app(n, 0);
    queue<int> q;
    q.push(s), h[s] = 0;
    while (!q.empty()) {
      int u = q.front(); q.pop();
      vis[u] = false;
      for (auto&& e : g[u]) {
        if (e.cap && h[e.v] > h[u] + e.cst) {
          h[e.v] = h[u] + e.cst;
          if (!vis[e.v]) {
            if (++app[e.v] >= n) return false;
            vis[e.v] = true, q.push(e.v);
          }
        }
      }
    }
    return true;
  }
  bool dijkstra(int s, int t) {
    vis.assign(n, false);
    dis.assign(n, MAXC);
    pqueue<pair<Cst, int>> q;
    q.emplace(dis[s] = 0, s);
    while (!q.empty()) {
      int u = q.top().second; q.pop();
      if (exchange(vis[u], true)) continue;
      for (auto&& e : g[u]) {
        if (!e.cap) continue;
        auto w = e.cst + h[u] - h[e.v];
        if (dis[e.v] > dis[u] + w) q.emplace(dis[e.v] = dis[u] + w, e.v);
      }
    }
    return dis[t] != MAXC;
  }
  vector<int> cur;
  Cap dfs(int u, Cap flw, int t) {
    if (u == t) return flw;
    Cap res = 0;
    vis[u] = true;
    for (int &i = cur[u]; i < (int)g[u].size(); i++) {
      auto &e = g[u][i];
      if (vis[e.v]) continue;
      if (e.cap && h[e.v] == h[u] + e.cst) {
        Cap run = dfs(e.v, min(flw - res, e.cap), t);
        e.cap -= run, g[e.v][e.rid].cap += run;
        res += run;
        if (res == flw) return vis[u] = false, res;
      }
    }
    return res;
  }
  pair<Cap, Cst> flow(int s, int t, Cap lim = numeric_limits<Cap>::max()) {
    if (!spfa(s)) throw logic_error("sorry, cannot solve :(");
    Cap flw = 0;
    Cst sum = 0;
    while (flw < lim && dijkstra(s, t)) {
      for (int i = 0; i < n; i++) {
        if (dis[i] != MAXC) h[i] += dis[i];
      }
      cur.assign(n, 0);
      vis.assign(n, false);
      Cap run = dfs(s, lim - flw, t);
      flw += run, sum += run * h[t];
    }
    return make_pair(flw, sum);
  }
};
