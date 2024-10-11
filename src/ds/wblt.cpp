#include <bits/stdc++.h>
using namespace std;
#ifdef LOCAL
#define debug(...) fprintf(stderr, __VA_ARGS__)
#else
#define endl "\n"
#define debug(...) void(0)
#endif
using LL = long long;
template <int N>
struct wblt {
  int ch[N << 1][2], tsh[N << 1], tot = 0, cnt = 0, siz[N << 1], val[N << 1];
  int newnode(int v) {
    int p = cnt ? tsh[cnt--] : ++tot;
    ch[p][0] = ch[p][1] = 0, siz[p] = 1, val[p] = v, maintain(p);
    return p;
  }
  bool isleaf(int p) { return !ch[p][0]; }
  void maintain(int p) { 
    if (isleaf(p)) return ;
    val[p] = val[ch[p][1]], siz[p] = siz[ch[p][0]] + siz[ch[p][1]];
  }
  int mg(int p, int q) {
    if (!p || !q) return p + q;
    int lim = 0.292 * (siz[p] + siz[q]);
    if (min(siz[p], siz[q]) >= lim) {
       int t = newnode(val[p]);
       ch[t][0] = p, ch[t][1] = q;
       return maintain(t), t;
    }
    if (siz[p] >= siz[q]) {
// pushdown(p);
      auto [x, y] = ch[tsh[++cnt] = p];
      if (siz[x] >= lim) return mg(x, mg(y, q));
// pushdown(y);
      auto [y0, y1] = ch[tsh[++cnt] = y];
      return mg(mg(x, y0), mg(y1, q));
    } else {
// pushdown(q);
      auto [x, y] = ch[tsh[++cnt] = q];
      if (siz[y] >= lim) return mg(mg(p, x), y);
      auto [x0, x1] = ch[tsh[++cnt] = x];
// pushdown(x);
      return mg(mg(p, x0), mg(x1, y));
    }
  }
  void sp(int p, int k, int& x, int& y) {
    if (!k) return x = 0, y = p, void();
    if (isleaf(p)) return x = p, y = 0, assert(k == 1);
// pushdown(p);
    if (k <= siz[ch[p][0]]) sp(ch[p][0], k, x, y), y = mg(y, ch[p][1]);
    else sp(ch[p][1], k - siz[ch[p][0]], x, y), x = mg(ch[p][0], x);
    tsh[++cnt] = p;
  }
  void spv(int p, int v, int& x, int& y) {
    if (val[p] <= v) return x = p, y = 0, void();
    if (isleaf(p)) return x = 0, y = p, void();
// pushdown(p);
    if (v < val[ch[p][0]]) spv(ch[p][0], v, x, y), y = mg(y, ch[p][1]);
    else spv(ch[p][1], v, x, y), x = mg(ch[p][0], x);
    tsh[++cnt] = p;
  }
  int getkth(int p, int k) {
    while (!isleaf(p)) {
// pushdown(p);
      if (k <= siz[ch[p][0]]) p = ch[p][0];
      else k -= siz[ch[p][0]], p = ch[p][1];
    }
    return val[p];
  }
  void dfs(int p, int &lst) {
    if (!isleaf(p)) dfs(ch[p][0], lst), dfs(ch[p][1], lst);
    else assert(exchange(lst, val[p]) <= val[p]);
  }
};
wblt<100010> t;
int main() {
#ifndef LOCAL
  cin.tie(nullptr)->sync_with_stdio(false);  
#endif
  int m, rt = 0;
  cin >> m;
  while (m--) {
    int op, v, x, y, z;
    cin >> op >> v;
    switch (op) {
      case 1:
        t.spv(rt, v, x, y);
        rt = t.mg(x, t.mg(t.newnode(v), y));
        break;
      case 2:
        t.spv(rt, v - 1, x, y);
        if (y && t.getkth(y, 1) == v) t.sp(y, 1, z, y);
        rt = t.mg(x, y);
        break;
      case 3:
        t.spv(rt, v - 1, x, y);
        cout << t.siz[x] + 1 << endl;
        rt = t.mg(x, y);
        break;
      case 4:
        cout << t.getkth(rt, v) << endl;
        break;
      case 5:
        t.spv(rt, v - 1, x, y);
        cout << t.val[x] << endl;
        rt = t.mg(x, y);
        break;
      case 6:
        t.spv(rt, v, x, y);
        cout << t.getkth(y, 1) << endl;
        rt = t.mg(x, y);
        break;
    }
#ifdef LOCAL
    int lst = -1e9;
    t.dfs(rt, lst);
#endif
  }
  return 0;
}

