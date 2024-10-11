// P4219 [BJOI2014] 大融合
template <int N>
struct LCT {
  int ch[N + 10][2], fa[N + 10];
  bool rev[N + 10];
  int sizl[N + 10], siz[N + 10];
  bool getson(int p) { return ch[fa[p]][1] == p; }
  bool nroot(int p) { return p && ch[fa[p]][getson(p)] == p; }
  void maintain(int p) {
    if (p) siz[p] = siz[ch[p][0]] + 1 + sizl[p] + siz[ch[p][1]];
  }
  // siz[p] 有效，当且仅当 p 被 makeroot 了
  void reverse(int p) {
    swap(ch[p][0], ch[p][1]);
    rev[p] ^= 1;
  }
  void pushdown(int p) {
    if (rev[p]) {
      reverse(ch[p][0]);
      reverse(ch[p][1]);
      rev[p] = false;
    }
  }
  void pushall(int p) {
    if (nroot(p)) pushall(fa[p]);
    pushdown(p);
  }
  void rotate(int p) {
    const auto link = [&](int p, int q, int r) {
      fa[p] = q, ch[q][r] = p;
    };
    int f = fa[p], r = getson(p);
    if (nroot(f))
      link(p, fa[f], getson(f));
    else
      fa[p] = fa[f];
    link(ch[p][r ^ 1], f, r);
    link(f, p, r ^ 1);
    maintain(f);
    maintain(p);
  }
  void splay(int p) {
    pushall(p);
    for (; nroot(p); rotate(p))
      if (nroot(fa[p])) rotate(getson(p) == getson(fa[p]) ? fa[p] : p);
  }
  int access(int p) {
    int y = 0;
    for (; p; y = p, p = fa[p]) {
      splay(p);
      sizl[p] -= siz[y];
      sizl[p] += siz[ch[p][1]];
      ch[p][1] = y;
      maintain(p);
    }
    return y;
  }
  void makeroot(int p) {
    access(p);
    splay(p);
    reverse(p);
  }
  void link(int x, int y) {
    makeroot(x);
    makeroot(y);
    fa[y] = x;
    sizl[x] += siz[y];
  }
  void cut(int x, int y) {
    makeroot(x);
    access(y), splay(y);
    fa[x] = ch[y][0] = 0;
    maintain(y);
  }
};

