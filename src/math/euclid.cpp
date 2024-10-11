#include <bits/stdc++.h>
using namespace std;
#ifdef LOCAL
#define debug(...) fprintf(stderr, ##__VA_ARGS__)
#else
#define endl "\n"
#define debug(...) void(0)
#endif
typedef long long LL;
#include "../modint/modint.hpp"
typedef modint<998244353> mint;
// https://www.cnblogs.com/caijianhong/p/18290748/template-euclid
// 洛谷 P5170 【模板】类欧几里得算法
struct node {
  mint cu, cr, sr, su, suu, sur;
  friend node operator*(const node &S, const node &T) {
    return node{
        S.cu + T.cu, S.cr + T.cr, S.sr + T.sr + S.cr * T.cr,
        S.su + T.su + S.cu * T.cr,
        S.suu + T.suu + T.cr * S.cu * S.cu + 2 * S.cu * T.su,
        // (S.cu + ?)^2 = sum S.cu^2 + sum 2S.cu? + sum?^2
        S.sur + T.sur + T.cr * S.cr * S.cu + S.cu * T.sr + S.cr * T.su
        // (S.cr + ?)(S.cu + %) = sum S.crS.cu +sum?%+?sumS.cu+%sum S.cr
    };
  }
};
node qpow(node a, LL b) {
  node r{};
  for (; b; b >>= 1, a = a * a)
    if (b & 1) r = r * a;
  return r;
}
node solve(LL p, LL q, LL r, LL l, node U, node R) {
  // q > r
  if (!l) return {};
  if (p >= q) return solve(p % q, q, r, l, U, qpow(U, p / q) * R);
  auto div = [](__int128 p, LL q, LL r, LL l) { return (LL)((p * l + r) / q); };
  LL cntu = div(p, q, r, l);
  if (!cntu) return qpow(R, l);
  return qpow(R, (q - r - 1) / p) * U *
         solve(q, p, (q - r - 1) % p, cntu - 1, R, U) *
         qpow(R, l - div(q, p, -r - 1, cntu));
}
int main() {
#ifndef LOCAL
  cin.tie(nullptr)->sync_with_stdio(false);
#endif
  int t;
  cin >> t;
  while (t--) {
    int a, b, c, l;
    cin >> l >> a >> b >> c;
    auto ret = qpow({1, 0, 0, 0, 0, 0}, b / c) *
               solve(a, c, b % c, l, {1, 0, 0, 0, 0, 0}, {0, 1, 1, 0, 0, 0});
    cout << ret.su + b / c << " "
         << ret.suu + 1ll * (b / c) * (b / c) << " " << ret.sur
         << endl;
  }
  return 0;
}

