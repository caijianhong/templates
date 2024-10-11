#include <bits/stdc++.h>
using namespace std;
#ifdef LOCAL
#define debug(...) fprintf(stderr, ##__VA_ARGS__)
#else
#define endl "\n"
#define debug(...) void(0)
#endif
using LL = long long;
#include "../modint/modint.hpp"
using mint = modint<int(1e9 + 7)>;
#include "./siever.hpp"
LL sqr(LL x) { return x * x; }
mint ID1(mint x) { return x * (x + 1) * 500000004; }
mint ID2(mint x) { return x * (x + 1) * (2 * x + 1) * 166666668; }
LL n, v[200010], lim;
mint f(mint pc) { return pc * (pc - 1); }
int m, idl[100010], idg[100010], idp[100010];
int id(LL x) { return x <= lim ? idl[x] : idg[n / x]; }
mint g1[200010], g2[200010], *g = g2;
siever<100010> sie;
mint dfs(LL n, int k) {
  if (n < sie.pri[k]) return 0;
  mint ans = g[id(n)] - g[idp[k - 1]];
  for (int i = k; sqr(sie.pri[i]) <= n; i++) {
    int pi = sie.pri[i];
    LL now = pi;
    for (int c = 1; now * pi <= n; c++, now *= pi) {
      ans += f(now) * dfs(n / now, i + 1) + f(now * pi);
    }
  }
  return ans;
}
int main() {
#ifndef LOCAL
  cin.tie(nullptr)->sync_with_stdio(false);  
#endif
  cin >> n;
  lim = sqrtl(n);
  for (LL l = 1, r; l <= n; l = r + 1) {
    v[++m] = n / l, r = n / v[m];
    (v[m] <= r ? idl[v[m]] : idg[r]) = m;
    g1[m] = ID1(v[m]) - 1, g2[m] = ID2(v[m]) - 1;
  }
  idp[0] = id(1);
  for (int k = 1; sqr(sie.pri[k]) <= n; k++) {
    idp[k] = id(sie.pri[k]);
    int p = sie.pri[k];
    for (int i = 1; i <= m && v[i] >= sqr(p); i++) {
      g1[i] -= p * (g1[id(v[i] / p)] - g1[idp[k - 1]]);
      g2[i] -= sqr(p) * (g2[id(v[i] / p)] - g2[idp[k - 1]]);
    }
  }
  for (int i = 1; i <= m; i++) g[i] -= g1[i];
  cout << dfs(n, 1) + 1 << endl;
  return 0;
}

