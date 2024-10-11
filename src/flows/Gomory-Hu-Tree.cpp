#include <bits/stdc++.h>
using namespace std;
#ifdef LOCAL
#define debug(...) fprintf(stderr, ##__VA_ARGS__)
#else
#define endl "\n"
#define debug(...) void(0)
#endif
typedef long long LL;
#include "flow.hpp"
int n, m;
mf_graph<int> mf0;
vector<pair<int, int>> t[510];
void solve(const vector<int>& pts) {
  if (pts.size() <= 1) return ;
  auto mf = mf0;
  int s0 = pts[0], t0 = pts[1];
  int mincut = mf.flow(s0, t0);
  t[s0].emplace_back(t0, mincut);
  t[t0].emplace_back(s0, mincut);
  vector<int> vis(n);
  auto dfs = [&](auto& dfs, int u) -> void {
    vis[u] = true;
    for (auto&& e : mf.g[u]) {
      int v = e.v;
      if (e.cap && !vis[v]) dfs(dfs, v);
    }
  };
  dfs(dfs, s0);
  vector<int> pl, pr;
  for (int x : pts) if (vis[x]) pl.push_back(x); else pr.push_back(x);
  solve(pl), solve(pr);
}
int ans[510][510];
void dfs(int u, int fa, int rt, int now) {
  ans[rt][u] = now;
  for (auto&& e : t[u]) {
    int v = e.first;
    if (v != fa) dfs(v, u, rt, min(now, e.second));
  }
}
int main() {
#ifndef LOCAL
  cin.tie(nullptr)->sync_with_stdio(false);  
#endif
  cin >> n >> m, ++n;
  mf0 = decltype(mf0)(n);
  for (int i = 1; i <= m; i++) {
    int u, v, w;
    cin >> u >> v >> w;
    mf0.addedge(u, v, w);
    mf0.addedge(v, u, w);
  }
  vector<int> pts(n);
  iota(pts.begin(), pts.end(), 0);
  solve(pts);
  for (int i = 0; i < n; i++) dfs(i, -1, i, numeric_limits<int>::max());
  int q;
  cin >> q;
  while (q--) {
    int u, v;
    cin >> u >> v;
    cout << ans[u][v] << endl;
  }
  return 0;
}
