mt19937_64 rng{random_device{}()};
LL qmul(LL a, LL b, LL n) { return (__int128)a * b % n; }
LL qpow(LL a, LL b, LL n) {
  LL r = 1;
  for (; b; b >>= 1, a = qmul(a, a, n)) {
    if (b & 1) r = qmul(r, a, n);
  }
  return r;
}
bool isprime(LL n) {
  if (n <= 1) return 0;
  auto MR = [&](LL a) -> bool {
    LL d = n - 1, r = 0;
    while (d % 2 == 0) d >>= 1, ++r;
    LL k = qpow(a, d, n);
    if (k == 1) return true;
    while (r--) {
      if (k == n - 1) return true;
      k = qmul(k, k, n);
    }
    return false;
  };
  constexpr int bases[] = {2, 3, 5, 7, 11, 13, 15, 17, 19, 23, 29, 31, 37};
  for (int b : bases) {
    if (n % b == 0) return n == b;
    if (!MR(b)) return false;
  }
  return true;
}
vector<LL> divide(LL n) {
  if (isprime(n)) return {n};
  if (n < 2) return {};
  static const auto find = [&](LL n) {
    auto f = [&, c = (LL)rng() % (n - 1) + 1](LL x) {
      return (qmul(x, x, n) + c) % n;
    };
    LL val = 1, s = 0, t = 0;
    for (int gal = 1;; gal <<= 1, s = t, val = 1) {
      for (int stp = 1; stp <= gal; stp++) {
        t = f(t);
        val = qmul(val, abs(t - s), n);
        if (stp % 127 == 0 || stp == gal) {
          LL d = gcd(val, n);
          if (d > 1) return d;
        }
      }
    }
    return n;
  };
  LL p = n;
  while (p == n) p = find(n);
  int cnt = 0;
  while (n % p == 0) n /= p, ++cnt;
  auto r1 = divide(n), r2 = divide(p);
  for (LL p : r2) r1.insert(r1.end(), cnt, p);
  return r1;
}

