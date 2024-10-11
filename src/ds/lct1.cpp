// P3690 【模板】动态树（LCT）
#include <algorithm>
#include <cstdio>
#include <cstring>
using namespace std;
typedef long long LL;
template <int N>
struct lctree {
  int val[N + 10], sum[N + 10], fa[N + 10], ch[N + 10][2], rev[N + 10];
  bool getson(int p) { return ch[fa[p]][1] == p; }
  bool isroot(int p) { return !p || ch[fa[p]][getson(p)] != p; }
  void maintain(int p) { sum[p] = val[p] ^ sum[ch[p][0]] ^ sum[ch[p][1]]; }
  void pushdown(int p) {
    if (rev[p])
      swap(ch[p][0], ch[p][1]), rev[ch[p][0]] ^= 1, rev[ch[p][1]] ^= 1,
          rev[p] ^= 1;
  }
  void update(int p) {
    if (!isroot(p)) update(fa[p]);
    pushdown(p);
  }
  void connect(int p, int q, int r) { fa[p] = q, ch[q][r] = p; }  // p->q
  void rotate(int p) {
    int f = fa[p], r = getson(p);
    if (fa[p] = fa[f], !isroot(f)) connect(p, fa[f], getson(f));
    connect(ch[p][r ^ 1], f, r), connect(f, p, r ^ 1), maintain(f), maintain(p);
  }
  void splay(int p) {
    for (update(p); !isroot(p); rotate(p))
      if (!isroot(fa[p])) rotate(getson(p) == getson(fa[p]) ? fa[p] : p);
  }
  int access(int p) {
    int y = 0;
    for (; p; p = fa[y = p]) splay(p), ch[p][1] = y, maintain(p);
    return y;
  }
  void makeroot(int p) { access(p), splay(p), rev[p] ^= 1; }
  int findroot(int p) {
    access(p), splay(p);
    while (ch[p][0]) p = ch[p][0];
    return p;
  }
  void split(int x, int y) { makeroot(x), access(y), splay(y); }
  void link(int x, int y) { makeroot(x), fa[x] = y; }
  void cut(int x, int y) {
    split(x, y);
    if (fa[x] == y && !ch[x][1]) fa[x] = ch[y][0] = 0;
    maintain(y);
  }
  void modify(int x, int y) { splay(x), val[x] = y, maintain(x); }
  int lca(int x, int y) { return access(x), access(y); }
};
int n, m;
lctree<100010> t;
int main() {
  //	#ifdef LOCAL
  //	 	freopen("input.in","r",stdin);
  //	#endif
  scanf("%d%d", &n, &m);
  for (int i = 1; i <= n; i++) scanf("%d", &t.val[i]), t.sum[i] = t.val[i];
  for (int i = 1, op, x, y; i <= m; i++) {
    scanf("%d%d%d", &op, &x, &y);
    switch (op) {
      case 0:
        t.split(x, y), printf("%d\n", t.sum[y]);
        break;
      case 1:
        if (t.findroot(x) != t.findroot(y)) t.link(x, y);
        break;
      case 2:
        if (t.findroot(x) == t.findroot(y)) t.cut(x, y);
        break;
      case 3:
        t.modify(x, y);
    }
  }
  return 0;
}


