template <class T>
using must_int = enable_if_t<is_integral<T>::value, int>;
template <int id>
struct modint {/*{{{*/
  static int mod;
  static unsigned umod;
  static void setmod(int p) { mod = umod = p; }
  unsigned v;
  modint() : v(0) {}
  template <class T, must_int<T> = 0>
  modint(T x) {
    x %= mod;
    v = x < 0 ? x + mod : x;
  }
  modint operator+() const { return *this; }
  modint operator-() const { return modint() - *this; }
  friend int raw(const modint &self) { return self.v; }
  friend ostream& operator<<(ostream& os, const modint &self) { 
    return os << raw(self); 
  }
  modint &operator+=(const modint &rhs) {
    v += rhs.v;
    if (v >= umod) v -= umod;
    return *this;
  }
  modint &operator-=(const modint &rhs) {
    v -= rhs.v;
    if (v >= umod) v += umod;
    return *this;
  }
  modint &operator*=(const modint &rhs) {
    v = 1ull * v * rhs.v % umod;
    return *this;
  }
  modint &operator/=(const modint &rhs) {
    assert(rhs.v);
    return *this *= qpow(rhs, mod - 2);
  }
  template <class T, must_int<T> = 0>
  friend modint qpow(modint a, T b) {
    modint r = 1;
    for (; b; b >>= 1, a *= a)
      if (b & 1) r *= a;
    return r;
  }
  friend modint operator+(modint lhs, const modint &rhs) { return lhs += rhs; }
  friend modint operator-(modint lhs, const modint &rhs) { return lhs -= rhs; }
  friend modint operator*(modint lhs, const modint &rhs) { return lhs *= rhs; }
  friend modint operator/(modint lhs, const modint &rhs) { return lhs /= rhs; }
  bool operator==(const modint &rhs) const { return v == rhs.v; }
  bool operator!=(const modint &rhs) const { return v != rhs.v; }
};/*}}}*/
template <int id>
unsigned modint<id>::umod;
template <int id>
int modint<id>::mod;
