#include <algorithm>
#include <cstdio>
#include <cstring>
using namespace std;
typedef long long LL;
template <int N, int M, class T = int>
struct graph {
  int head[N + 10], nxt[M * 2 + 10], cnt;
  struct edge {
    int u, v;
    T w;
    edge(int u = 0, int v = 0, T w = 0) : u(u), v(v), w(w) {}
  } e[M * 2 + 10];
  graph() { memset(head, cnt = 0, sizeof head); }
  edge operator[](int i) { return e[i]; }
  void add(int u, int v, T w = 0) {
    e[++cnt] = edge(u, v, w), nxt[cnt] = head[u], head[u] = cnt;
  }
  void link(int u, int v, T w = 0) { add(u, v, w), add(v, u, w); }
};
template <int N, int M, class T = int>
struct matrix {
  T a[N + 1][M + 1];
  matrix(bool f = 0) {
    memset(a, ~0x3f, sizeof a);
    for (int i = 1; f && i <= N; i++) a[i][i] = 0;
  }
  T *operator[](int i) { return a[i]; }
};
template <int N, int M, int R, class T = int>
matrix<N, R> operator*(matrix<N, M, T> a, matrix<M, R, T> b) {
  matrix<N, R, T> c = 0;
  for (int i = 1; i <= N; i++) {
    for (int j = 1; j <= R; j++) {
      for (int k = 1; k <= M; k++) {
        c[i][j] = max(c[i][j], a[i][k] + b[k][j]);
      }
    }
  }
  return c;
}
int fa[100010], siz[100010], son[100010], dfn[100010], rnk[100010], top[100010],
    cnt;
template <int N, class T = matrix<2, 2>>
struct segtree {
  T ans[N + 10];
  int ch[N + 10][2], tot;
  segtree() : tot(-1) { ans[0] = 1, newnode(); }
  int newnode() { return ++tot, ch[tot][0] = ch[tot][1] = 0, tot; }
  void maintain(int p) { ans[p] = ans[ch[p][0]] * ans[ch[p][1]]; }
  void build(T a[], int &p, int l, int r) {
    if (!p) p = newnode();
    if (l == r) return ans[p] = a[rnk[l]], void();
    int mid = (l + r) >> 1;
    build(a, ch[p][0], l, mid);
    build(a, ch[p][1], mid + 1, r);
    maintain(p);
  }
  void modify(T &v, int x, int p, int l, int r) {
    if (l == r) return ans[p] = v, void();
    int mid = (l + r) >> 1;
    if (x <= mid)
      modify(v, x, ch[p][0], l, mid);
    else
      modify(v, x, ch[p][1], mid + 1, r);
    maintain(p);
  }
  T query(int L, int R, int p, int l, int r) {
    if (L <= l && r <= R) return ans[p];
    int mid = (l + r) >> 1;
    if (R <= mid)
      return query(L, R, ch[p][0], l, mid);
    else if (mid + 1 <= L)
      return query(L, R, ch[p][1], mid + 1, r);
    else
      return query(L, R, ch[p][0], l, mid) * query(L, R, ch[p][1], mid + 1, r);
  }
};
int n, m, f[100010][2], a[100010], root, eds[100010];
matrix<2, 2> val[100010];
graph<100010, 100010, bool> g;
segtree<200010> t;
int dfs(int u, int f = 0) {
  fa[u] = f, siz[u] = 1, son[u] = 0;
  for (int i = g.head[u]; i; i = g.nxt[i]) {
    int v = g[i].v;
    if (v == fa[u]) continue;
    siz[u] += dfs(v, u);
    if (siz[v] > siz[son[u]]) son[u] = v;
  }
  return siz[u];
}
void add(int u, int v) { f[u][0] += max(f[v][0], f[v][1]), f[u][1] += f[v][0]; }
void make(int u, int v) {
  val[u][1][1] += max(f[v][0], f[v][1]), val[u][1][2] = val[u][1][1],
                                         val[u][2][1] += f[v][0];
}
void cut(int u, int topf) {
  rnk[dfn[u] = ++cnt] = u, top[u] = topf, eds[topf] = max(eds[topf], dfn[u]);
  f[u][0] = 0, f[u][1] = a[u], val[u][1][1] = val[u][1][2] = 0,
  val[u][2][1] = a[u];
  if (son[u]) cut(son[u], topf), add(u, son[u]);
  for (int i = g.head[u]; i; i = g.nxt[i]) {
    int v = g[i].v;
    if (v == fa[u] || v == son[u]) continue;
    cut(v, v);
    add(u, v), make(u, v);
  }
}
void update(int u, int k) {
  val[u][2][1] += k - a[u], a[u] = k;
  matrix<2, 2> bef, aft;
  while (u) {
    bef = t.query(dfn[top[u]], eds[top[u]], root, 1, n);
    t.modify(val[u], dfn[u], root, 1, n);
    aft = t.query(dfn[top[u]], eds[top[u]], root, 1, n);
    u = fa[top[u]];
    val[u][1][1] += max(aft[1][1], aft[2][1]) - max(bef[1][1], bef[2][1]);
    val[u][1][2] = val[u][1][1];
    val[u][2][1] += aft[1][1] - bef[1][1];
  }
}
int main() {
  //	#ifdef LOCAL
  //	 	freopen("input.in","r",stdin);
  //	#endif
  scanf("%d%d", &n, &m);
  for (int u = 1; u <= n; u++) scanf("%d", &a[u]);
  for (int i = 1, u, v; i < n; i++) scanf("%d%d", &u, &v), g.link(u, v);
  dfs(1), cut(1, 1), t.build(val, root, 1, n);
  for (int i = 1, u, k; i <= m; i++) {
    scanf("%d%d", &u, &k);
    update(u, k);
    matrix<2, 2> res = t.query(dfn[top[1]], eds[top[1]], root, 1, n);
    printf("%d\n", max(res[1][1], res[2][1]));
  }
  return 0;
}

