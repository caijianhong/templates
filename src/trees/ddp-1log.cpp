#include <bits/stdc++.h>
using namespace std;
#ifdef LOCAL
#define debug(...) fprintf(stderr, __VA_ARGS__)
#else
#define endl "\n"
#define debug(...) void(0)
#endif
using LL = long long;
template <int n, int m>
struct matrix {
  int mat[n][m];
  auto operator[](int i) { return mat[i]; }
  auto operator[](int i) const { return mat[i]; }
  void fill(int v) { fill_n(&mat[0][0], n * m, v); }
};
template <int n>
matrix<n, n> I() { /*{{{*/
  matrix<n, n> res;
  res.fill(-1e9);
  for (int i = 0; i < n; i++) res[i][i] = 0;
  return res;
} /*}}}*/
template <int n, int m, int r>
matrix<n, r> operator*(const matrix<n, m>& lhs,
                       const matrix<m, r>& rhs) { /*{{{*/
  matrix<n, r> res;
  res.fill(-1e9);
  for (int i = 0; i < n; i++)
    for (int j = 0; j < m; j++)
      for (int k = 0; k < r; k++)
        res[i][k] = max(res[i][k], lhs[i][j] + rhs[j][k]);
  return res;
} /*}}}*/
template <>
matrix<2, 2> operator*(const matrix<2, 2>& lhs, const matrix<2, 2>& rhs) {
  return {max(lhs[0][0] + rhs[0][0], lhs[0][1] + rhs[1][0]),
          max(lhs[0][0] + rhs[0][1], lhs[0][1] + rhs[1][1]),
          max(lhs[1][0] + rhs[0][0], lhs[1][1] + rhs[1][0]),
          max(lhs[1][0] + rhs[0][1], lhs[1][1] + rhs[1][1])};
}
constexpr int N = 1e6 + 10;
int f[N][2], n, m, a[N];
matrix<2, 2> val[N], sum[N];
basic_string<int> g[N];
int fa[N], siz[N], son[N], dep[N], cnt, dfn[N], rnk[N], top[N], tf[N], ch[N][2];
void maintain(int p) {
  // sum[p] = sum[ch[p][0]] * val[p] * sum[ch[p][1]];
  sum[p] = val[p];
  if (ch[p][0]) sum[p] = sum[ch[p][0]] * sum[p];
  if (ch[p][1]) sum[p] = sum[p] * sum[ch[p][1]];
}
int build(int l, int r) {
  if (l > r) return 0;
  int pos = l + 1, T = siz[rnk[l]] - siz[son[rnk[r]]];
  while (pos <= r && siz[rnk[l]] - siz[son[rnk[pos]]] <= T / 2) ++pos;
  int p = rnk[--pos];
  if ((ch[p][0] = build(l, pos - 1))) tf[ch[p][0]] = p;
  if ((ch[p][1] = build(pos + 1, r))) tf[ch[p][1]] = p;
  maintain(p);
  return p;
}
void update(int p, int k) {
  debug("update(%d, %d)\n", p, k);
  val[p][1][0] += k - a[p], a[p] = k;
  while (p) {
    if (ch[tf[p]][0] != p && ch[tf[p]][1] != p) {
      val[tf[p]][0][0] -= max(sum[p][0][0], sum[p][1][0]);
      val[tf[p]][1][0] -= sum[p][0][0];
    }
    maintain(p);
    if (ch[tf[p]][0] != p && ch[tf[p]][1] != p) {
      val[tf[p]][0][0] += max(sum[p][0][0], sum[p][1][0]);
      val[tf[p]][0][1] = val[tf[p]][0][0];
      val[tf[p]][1][0] += sum[p][0][0];
    }
    p = tf[p];
  }
}
void dfs(int u, int _fa) {
  fa[u] = _fa, dep[u] = dep[_fa] + 1, siz[u] = 1;
  for (int v : g[u])
    if (v != _fa) {
      dfs(v, u), siz[u] += siz[v];
      if (siz[v] > siz[son[u]]) son[u] = v;
    }
}
void trans(int u, int v) {
  f[u][0] += max(f[v][0], f[v][1]);
  f[u][1] += f[v][0];
}
void cut(int u, int topf) {
  dfn[u] = ++cnt, rnk[cnt] = u, top[u] = topf;
  f[u][0] = 0, f[u][1] = a[u];
  val[u] = {0, 0, a[u], (int)-1e9};
  if (son[u]) cut(son[u], topf), trans(u, son[u]);
  // else tf[build(dfn[topf], dfn[u])] = fa[topf];
  for (int v : g[u])
    if (v != fa[u] && v != son[u]) {
      cut(v, v), trans(u, v);
      val[u][0][0] += max(f[v][0], f[v][1]);
      val[u][1][0] += f[v][0];
    }
  val[u][0][1] = val[u][0][0];
  debug("son[%d] = %d\n", u, son[u]);
  debug("f[%d] = {%d, %d}\n", u, f[u][0], f[u][1]);
}
int main() {
#ifndef LOCAL
  cin.tie(nullptr)->sync_with_stdio(false);
#endif
  cin >> n >> m;
  for (int i = 1; i <= n; i++) cin >> a[i];
  for (int i = 1, u, v; i < n; i++) cin >> u >> v, g[u] += v, g[v] += u;
  sum[0] = I<2>();
  dfs(1, 0), cut(1, 1);
  for (int i = 1; i <= n; i++)
    if (!son[i]) tf[build(dfn[top[i]], dfn[i])] = fa[top[i]];
  auto rt = find(tf + 1, tf + n + 1, 0) - tf;
  debug("ans0 = %d\n", max(f[1][0], f[1][1]));
  debug("ans0 = %d\n", max(sum[rt][0][0], sum[rt][1][0]));
#ifdef LOCAL
  for (int i = 1; i <= n; i++) debug("tf[%d] = %d\n", i, tf[i]);
#endif
  int lst = 0;
  while (m--) {
    int u, k;
    cin >> u >> k;
    update(u ^ lst, k);
    auto&& res = sum[rt];
    assert(res[0][1] <= res[0][0]);
    cout << (lst = max(res[0][0], res[1][0])) << endl;
  }
  return 0;
}

