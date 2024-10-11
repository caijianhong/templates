template <int N>
struct siever {
  int pri[N + 10], cnt;
  siever() : cnt(0) {
    fill(pri + 1, pri + N + 1, true);
    pri[0] = 1, pri[1] = false;
    for (int i = 1; i <= N; i++) {
      if (pri[i]) pri[++cnt] = i;
      for (int j = 1; j <= cnt && i * pri[j] <= N; j++) {
        pri[i * pri[j]] = false;
        if (i % pri[j] == 0) break;
      }
    }
  }
};
