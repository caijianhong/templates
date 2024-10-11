// https://www.luogu.com.cn/problem/P5043
// P5043 【模板】树同构（[BJOI2015]树的同构）
#include <bits/stdc++.h>
using namespace std;
#ifdef LOCAL
#define debug(...) fprintf(stderr, ##__VA_ARGS__)
#else
#define debug(...) void(0)
#endif
typedef long long LL;
using word = unsigned long long;
mt19937_64 rng{random_device{}()};
const word mask = rng();
word shift(word x) {
  x ^= mask;
  x ^= x << 13;
  x ^= x >> 7;
  x ^= x << 17;
  return x ^ mask;
}
int n, m;
basic_string<int> g[1000010];
word f[1000010], hsh[1000010];
word dfs(int u, int fa) {
  word &ans = f[u] = 1;
  for (int v : g[u]) if (v != fa) ans += shift(dfs(v, u));
  return ans;
}
void dfs2(int u, int fa) {
  for (int v : g[u]) if (v != fa) f[v] += shift(f[u] - shift(f[v])), dfs2(v, u);
}
int main() {
#ifndef LOCAL
  cin.tie(nullptr)->sync_with_stdio(false);  
#endif
  cin >> m;
  map<word, int> mp;
  for (int t = 1; t <= m; t++) {
    cin >> n;
    for (int i = 1; i <= n; i++) g[i].clear();
    for (int i = 1, x; i <= n; i++) {
      cin >> x;
      if (x) g[i] += x, g[x] += i;
    }
    dfs(1, 0);
    dfs2(1, 0);
    hsh[t] = *min_element(f + 1, f + n + 1);
    if (!mp.count(hsh[t])) mp[hsh[t]] = t;
  }
  for (int i = 1; i <= m; i++) cout << mp[hsh[i]] << endl;
  return 0;
}

