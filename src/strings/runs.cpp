#include <bits/stdc++.h>
using namespace std;
#ifdef LOCAL
#define debug(...) fprintf(stderr, ##__VA_ARGS__)
#else
#define endl "\n"
#define debug(...) void(0)
#endif
using LL = long long;
struct machine {// {{{
  int n;
  vector<int> rk, sa, st[30], buc, id;
  machine() = default;
  explicit machine(const string& str)
      : n((int)str.size()), rk(n + 1), sa(n), buc(max(n, 128)), id(n) {
    rk[n] = -1;
    for (int i = 0; i < n; i++) buc[rk[i] = str[i]] += 1;
    for (int i = 1; i < 128; i++) buc[i] += buc[i - 1];
    for (int i = n; i--;) sa[--buc[rk[i]]] = i;
    for (int j = 1; j <= n; j <<= 1) {
      int cur = 0;
      for (int i = n - j; i < n; i++) id[cur++] = i;
      for (int i = 0; i < n; i++)
        if (sa[i] >= j) id[cur++] = sa[i] - j;
      memset(buc.data(), 0, sizeof(int) * n);
      for (int i = 0; i < n; i++) buc[rk[i]] += 1;
      for (int i = 1; i < n; i++) buc[i] += buc[i - 1];
      for (int i = n; i--;) sa[--buc[rk[id[i]]]] = id[i];
      id[0] = 0;
      auto pred = [&](int x, int y) {
        return rk[x] != rk[y] || rk[x + j] != rk[y + j];
      };
      int pre = id[sa[0]] = 0;
      for (int i = 1; i < n; i++) id[sa[i]] = (pre += pred(sa[i - 1], sa[i]));
      memcpy(rk.data(), id.data(), sizeof(int) * n);
      if (pre + 1 == n) break;
    }
    auto& lcp = st[0];
    lcp.resize(n - 1);
    for (int i = 0, h = 0; i < n; i++) {
      if (!rk[i]) continue;
      if (h) h -= 1;
      int j = sa[rk[i] - 1];
      while (max(i, j) + h < n && str[i + h] == str[j + h]) h += 1;
      lcp[rk[i] - 1] = h;
    }
    for (int j = 1; 1 << j <= n; j++) {
      st[j].resize(n - (1 << j));
      for (int i = 0; i + (1 << j) < n; i++) {
        st[j][i] = min(st[j - 1][i], st[j - 1][i + (1 << (j - 1))]);
      }
    }
  }
  int operator()(int i, int j) const {
    if (max(i, j) == n) return 0;
    if (i == j) return n - i;
    int l = min(rk[i], rk[j]), r = max(rk[i], rk[j]) - 1;
    int k = __lg(r - l + 1);
    return min(st[k][l], st[k][r - (1 << k) + 1]);
  }
};// }}}
struct run {
  int l, r, p;
  bool operator<(const run& rhs) const {
    return l != rhs.l ? l < rhs.l : r < rhs.r;
  }
  bool operator==(const run& rhs) const {
    return l == rhs.l && r == rhs.r && p == rhs.p;
  }
};
vector<run> getRuns(string s) {
  int n = (int)s.size();
  auto lcp = machine(s);
  reverse(begin(s), end(s));
  auto lcs = machine(s);
  reverse(begin(s), end(s));
  s += '\0';
  vector<run> ans;
  for (int op : {0, 1}) {
    vector<int> stk;
    for (int i = n - 1; i >= 0; i--) {
      while (!stk.empty()) {
        int u = i, v = stk.back(), len = lcp(u, v);
        if ((s[u + len] < s[v + len]) == op) stk.pop_back();
        else break;
      }
      if (!stk.empty()) {
        int l = i, r = stk.back(), tr = lcp(l, r), tl = lcs(n - l - 1, n - r - 1);
        if (tl + tr >= r - l+1) ans.push_back({.l = l - tl + 1, .r = r + tr - 1, .p = r - l});
      }
      stk.push_back(i);
    }
  }
  auto bg = begin(ans), ed = end(ans);
  sort(bg, ed), ans.erase(unique(bg, ed), ed);
  return ans;
}
string s;
int n;
int subid, q;
int main() {
#ifndef LOCAL
  cin.tie(nullptr)->sync_with_stdio(false);
#endif
  cin >> s;
  auto runs = getRuns(s);
  cout << runs.size() << endl;
  for (auto e : runs) cout << e.l + 1 << " " << e.r + 1 << " " << e.p << endl;
  return 0;
}
