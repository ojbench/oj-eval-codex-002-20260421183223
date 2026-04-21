// Generated code.cpp for ACMOJ submission
// Contains int2048 class declaration and implementation
#include <bits/stdc++.h>
using namespace std;

#pragma once
#ifndef SJTU_BIGINTEGER
#define SJTU_BIGINTEGER

// Integer 1:
// Implement a signed big integer class that only needs to support simple addition and subtraction

// Integer 2:
// Implement a signed big integer class that supports addition, subtraction, multiplication, and division, and overload related operators

// Do not use any header files other than the following
#include <complex>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <vector>

// Do not use "using namespace std;"

namespace sjtu {
class int2048 {
private:
  // sign: 1 for non-negative, -1 for negative; zero represented by empty vec
  int sign;
  // base 10^4 per limb to keep multiplication manageable; stored little-endian
  static const int BASE = 10000;
  static const int BASE_DIG = 4;
  std::vector<int> a;

  void trim();
  static int abs_cmp(const int2048 &x, const int2048 &y);
  static int2048 abs_add(const int2048 &x, const int2048 &y);
  static int2048 abs_sub(const int2048 &x, const int2048 &y); // |x|>=|y|
  static int2048 abs_mul_simple(const int2048 &x, const int2048 &y);
  static void fft_multiply(const std::vector<int> &A, const std::vector<int> &B, std::vector<int> &C);
  static int2048 abs_mul(const int2048 &x, const int2048 &y);
  static void divmod_abs(const int2048 &u, const int2048 &v, int2048 &q, int2048 &r);

public:
  // Constructors
  int2048();
  int2048(long long);
  int2048(const std::string &);
  int2048(const int2048 &);

  // The parameter types of the following functions are for reference only, you can choose to use constant references or not
  // If needed, you can add other required functions yourself
  // ===================================
  // Integer1
  // ===================================

  // Read a big integer
  void read(const std::string &);
  // Output the stored big integer, no need for newline
  void print();

  // Add a big integer
  int2048 &add(const int2048 &);
  // Return the sum of two big integers
  friend int2048 add(int2048, const int2048 &);

  // Subtract a big integer
  int2048 &minus(const int2048 &);
  // Return the difference of two big integers
  friend int2048 minus(int2048, const int2048 &);

  // ===================================
  // Integer2
  // ===================================

  int2048 operator+() const;
  int2048 operator-() const;

  int2048 &operator=(const int2048 &);

  int2048 &operator+=(const int2048 &);
  friend int2048 operator+(int2048, const int2048 &);

  int2048 &operator-=(const int2048 &);
  friend int2048 operator-(int2048, const int2048 &);

  int2048 &operator*=(const int2048 &);
  friend int2048 operator*(int2048, const int2048 &);

  int2048 &operator/=(const int2048 &);
  friend int2048 operator/(int2048, const int2048 &);

  int2048 &operator%=(const int2048 &);
  friend int2048 operator%(int2048, const int2048 &);

  friend std::istream &operator>>(std::istream &, int2048 &);
  friend std::ostream &operator<<(std::ostream &, const int2048 &);

  friend bool operator==(const int2048 &, const int2048 &);
  friend bool operator!=(const int2048 &, const int2048 &);
  friend bool operator<(const int2048 &, const int2048 &);
  friend bool operator>(const int2048 &, const int2048 &);
  friend bool operator<=(const int2048 &, const int2048 &);
  friend bool operator>=(const int2048 &, const int2048 &);
};
} // namespace sjtu

#endif

// Implementation of int2048


namespace sjtu {

using std::cin;
using std::cout;
using std::istream;
using std::ostream;
using std::string;
using std::vector;

void int2048::trim() {
  while (!a.empty() && a.back() == 0) a.pop_back();
  if (a.empty()) sign = 1;
}

static int divceil_int(int x, int y) { return (x + y - 1) / y; }

int int2048::abs_cmp(const int2048 &x, const int2048 &y) {
  if (x.a.size() != y.a.size()) return x.a.size() < y.a.size() ? -1 : 1;
  for (int i = (int)x.a.size() - 1; i >= 0; --i) {
    if (x.a[i] != y.a[i]) return x.a[i] < y.a[i] ? -1 : 1;
  }
  return 0;
}

int2048 int2048::abs_add(const int2048 &x, const int2048 &y) {
  int2048 r;
  r.sign = 1;
  int n = (int)std::max(x.a.size(), y.a.size());
  r.a.assign(n + 1, 0);
  long long carry = 0;
  for (int i = 0; i < n || carry; ++i) {
    long long s = carry;
    if (i < (int)x.a.size()) s += x.a[i];
    if (i < (int)y.a.size()) s += y.a[i];
    if (i >= (int)r.a.size()) r.a.push_back(0);
    r.a[i] = int(s % BASE);
    carry = s / BASE;
  }
  r.trim();
  return r;
}

int2048 int2048::abs_sub(const int2048 &x, const int2048 &y) {
  // assumes |x| >= |y|
  int2048 r;
  r.sign = 1;
  r.a.assign(x.a.size(), 0);
  long long carry = 0;
  for (size_t i = 0; i < x.a.size(); ++i) {
    long long s = x.a[i] - carry - (i < y.a.size() ? y.a[i] : 0);
    if (s < 0) s += BASE, carry = 1; else carry = 0;
    r.a[i] = int(s);
  }
  r.trim();
  return r;
}

int2048 int2048::abs_mul_simple(const int2048 &x, const int2048 &y) {
  int2048 r;
  r.sign = 1;
  if (x.a.empty() || y.a.empty()) return r;
  r.a.assign(x.a.size() + y.a.size(), 0);
  for (size_t i = 0; i < x.a.size(); ++i) {
    long long carry = 0;
    for (size_t j = 0; j < y.a.size() || carry; ++j) {
      long long cur = r.a[i + j] + carry;
      if (j < y.a.size()) cur += 1LL * x.a[i] * y.a[j];
      r.a[i + j] = int(cur % BASE);
      carry = cur / BASE;
    }
  }
  r.trim();
  return r;
}

static void fft(std::vector<std::complex<double>> &a, bool invert) {
  int n = (int)a.size();
  static vector<int> rev;
  static vector<std::complex<double>> roots{ {1,0}, {1,0} };
  if ((int)rev.size() != n) {
    int k = __builtin_ctz(n);
    rev.assign(n, 0);
    for (int i = 0; i < n; ++i) {
      rev[i] = (rev[i >> 1] >> 1) | ((i & 1) << (k - 1));
    }
  }
  if ((int)roots.size() < n) {
    int k = __builtin_ctz(roots.size());
    roots.resize(n);
    while ((1 << k) < n) {
      double ang = 2 * M_PI / (1 << (k + 1));
      // Avoid <cmath>: use std::polar from <complex>
      for (int i = 1 << (k - 1); i < (1 << k); ++i) {
        roots[2 * i] = roots[i];
        roots[2 * i + 1] = roots[i] * std::polar(1.0, ang * (2 * i + 1 - (1 << k)));
      }
      ++k;
    }
  }
  for (int i = 0; i < n; ++i) if (i < rev[i]) std::swap(a[i], a[rev[i]]);
  for (int len = 1; len < n; len <<= 1) {
    for (int i = 0; i < n; i += 2 * len) {
      for (int j = 0; j < len; ++j) {
        auto u = a[i + j];
        auto v = a[i + j + len] * roots[len + j];
        a[i + j] = u + v;
        a[i + j + len] = u - v;
      }
    }
  }
  if (invert) {
    std::reverse(a.begin() + 1, a.end());
    for (auto &x : a) x /= n;
  }
}

void int2048::fft_multiply(const vector<int> &A, const vector<int> &B, vector<int> &C) {
  vector<std::complex<double>> fa(A.begin(), A.end()), fb(B.begin(), B.end());
  int n = 1;
  while (n < (int)A.size() + (int)B.size()) n <<= 1;
  fa.resize(n); fb.resize(n);
  fft(fa, false); fft(fb, false);
  for (int i = 0; i < n; ++i) fa[i] *= fb[i];
  fft(fa, true);
  C.assign(n, 0);
  long long carry = 0;
  for (int i = 0; i < n; ++i) {
    long long t = (long long) llround(fa[i].real()) + carry;
    C[i] = (int)(t % BASE);
    carry = t / BASE;
  }
  while (carry) {
    C.push_back((int)(carry % BASE));
    carry /= BASE;
  }
  while (!C.empty() && C.back() == 0) C.pop_back();
}

int2048 int2048::abs_mul(const int2048 &x, const int2048 &y) {
  int2048 r;
  r.sign = 1;
  if (x.a.empty() || y.a.empty()) return r;
  size_t n = x.a.size(), m = y.a.size();
  if (std::min(n, m) < 64) {
    return abs_mul_simple(x, y);
  }
  vector<int> c;
  fft_multiply(x.a, y.a, c);
  r.a = std::move(c);
  r.trim();
  return r;
}

void int2048::divmod_abs(const int2048 &u, const int2048 &v, int2048 &q, int2048 &r) {
  q.a.clear(); q.sign = 1; r.a.clear(); r.sign = 1;
  if (v.a.empty()) return; // undefined, but avoid crash
  if (u.a.empty()) { q.trim(); r.trim(); return; }
  if (abs_cmp(u, v) < 0) { r = u; q.a.clear(); q.sign = 1; return; }
  if (v.a.size() == 1) {
    // divide by single limb
    int d = v.a[0];
    r.a.assign(1, 0);
    long long rem = 0;
    q.a.assign(u.a.size(), 0);
    for (int i = (int)u.a.size() - 1; i >= 0; --i) {
      long long cur = u.a[i] + rem * BASE;
      q.a[i] = int(cur / d);
      rem = cur % d;
    }
    r.a[0] = (int)rem;
    q.trim(); r.trim();
    return;
  }
  // General long division (approximate quotient digits)
  int n = (int)u.a.size();
  int m = (int)v.a.size();
  int2048 normalized_u = u;
  int2048 dv = v;
  // No normalization to avoid extra headers; estimation via long double
  q.a.assign(n - m + 1, 0);
  r.a = normalized_u.a; r.trim();
  int2048 vshift;
  vshift.sign = 1;
  for (int k = n - m; k >= 0; --k) {
    // estimate qhat using leading two digits
    long long r2 = (k + m < (int)r.a.size()) ? r.a[k + m] : 0;
    long long r1 = (k + m - 1 < (int)r.a.size()) ? r.a[k + m - 1] : 0;
    long long r0 = (k + m - 2 < (int)r.a.size()) ? r.a[k + m - 2] : 0;
    long long d1 = v.a.back();
    long long d2 = (m >= 2) ? v.a[m - 2] : 0;
    long long num = r2 * BASE + r1;
    long long den = d1;
    long long qhat = num / (den ? den : 1);
    if (qhat >= BASE) qhat = BASE - 1;
    // refine qhat by checking r2*BASE^2 + r1*BASE + r0 < qhat*(d1*BASE + d2)
    while (true) {
      long long left2 = r2 * BASE + r1;
      long long right2 = qhat * d1;
      if (left2 > right2 || (left2 == right2 && r0 >= qhat * d2)) break;
      if (qhat == 0) break;
      --qhat;
    }
    // r -= qhat * v << k
    if (qhat) {
      long long carry = 0, borrow = 0;
      for (int i = 0; i < m || carry; ++i) {
        long long cur = (i < m ? 1LL * v.a[i] * qhat : 0) + carry;
        long long val = r.a[i + k] - (int)(cur % BASE) - borrow;
        carry = cur / BASE;
        if (val < 0) val += BASE, borrow = 1; else borrow = 0;
        r.a[i + k] = (int)val;
      }
      // Fix if negative
      if (borrow) {
        // add back v<<k and decrement qhat
        --qhat;
        int c = 0;
        for (int i = 0; i < m || c; ++i) {
          long long val = r.a[i + k] + (i < m ? v.a[i] : 0) + c;
          if (val >= BASE) val -= BASE, c = 1; else c = 0;
          r.a[i + k] = (int)val;
        }
      }
    }
    q.a[k] = (int)qhat;
  }
  r.trim(); q.trim();
}

// Constructors
int2048::int2048() : sign(1) {}
int2048::int2048(long long v) { *this = int2048(); if (v < 0) sign = -1, v = -v; else sign = 1; while (v) { a.push_back((int)(v % BASE)); v /= BASE; } trim(); }
int2048::int2048(const string &s) { read(s); }
int2048::int2048(const int2048 &o) = default;

void int2048::read(const string &s) {
  a.clear(); sign = 1;
  size_t i = 0; while (i < s.size() && isspace((unsigned char)s[i])) ++i;
  int sgn = 1; if (i < s.size() && (s[i] == '+' || s[i] == '-')) { if (s[i] == '-') sgn = -1; ++i; }
  while (i < s.size() && s[i] == '0') ++i;
  vector<int> tmp;
  for (size_t j = s.size(); j > i; ) {
    size_t k = (j >= BASE_DIG ? j - BASE_DIG : 0);
    if (k < i) k = i;
    int val = 0;
    for (size_t t = k; t < j; ++t) val = val * 10 + (s[t] - '0');
    tmp.push_back(val);
    j = k;
  }
  a = std::move(tmp);
  sign = a.empty() ? 1 : sgn;
  trim();
}

void int2048::print() {
  if (a.empty()) { cout << 0; return; }
  if (sign < 0) cout << '-';
  cout << a.back();
  char buf[8];
  for (int i = (int)a.size() - 2; i >= 0; --i) {
    std::snprintf(buf, sizeof(buf), "%0*d", BASE_DIG, a[i]);
    cout << buf;
  }
}

int2048 &int2048::add(const int2048 &o) {
  if (o.a.empty()) return *this;
  if (a.empty()) { *this = o; return *this; }
  if (sign == o.sign) {
    *this = abs_add(*this, o); this->sign = sign; return *this;
  } else {
    int c = abs_cmp(*this, o);
    if (c == 0) { a.clear(); sign = 1; return *this; }
    if (c > 0) { *this = abs_sub(*this, o); this->sign = sign; }
    else { *this = abs_sub(o, *this); this->sign = o.sign; }
    return *this;
  }
}

int2048 add(int2048 x, const int2048 &y) { return x.add(y); }

int2048 &int2048::minus(const int2048 &o) {
  int2048 neg = o; if (!neg.a.empty()) neg.sign = -neg.sign; return add(neg);
}

int2048 minus(int2048 x, const int2048 &y) { return x.minus(y); }

int2048 int2048::operator+() const { return *this; }
int2048 int2048::operator-() const { int2048 r = *this; if (!r.a.empty()) r.sign = -r.sign; return r; }

int2048 &int2048::operator=(const int2048 &o) = default;

int2048 &int2048::operator+=(const int2048 &o) { return add(o); }
int2048 operator+(int2048 x, const int2048 &y) { return x += y; }

int2048 &int2048::operator-=(const int2048 &o) { return minus(o); }
int2048 operator-(int2048 x, const int2048 &y) { return x -= y; }

int2048 &int2048::operator*=(const int2048 &o) {
  if (a.empty() || o.a.empty()) { a.clear(); sign = 1; return *this; }
  int s = sign * o.sign;
  int2048 r = abs_mul(*this, o);
  *this = r; this->sign = (a.empty() ? 1 : s);
  return *this;
}
int2048 operator*(int2048 x, const int2048 &y) { return x *= y; }

int2048 &int2048::operator/=(const int2048 &o) {
  if (o.a.empty()) return *this; // undefined
  if (a.empty()) return *this;
  int sgn = sign * o.sign;
  int2048 ua = *this; ua.sign = 1;
  int2048 ub = o; ub.sign = 1;
  int2048 q, r;
  divmod_abs(ua, ub, q, r);
  // floor division adjustment
  q.sign = (q.a.empty() ? 1 : sgn);
  if (sgn < 0 && !r.a.empty()) {
    // q := q - 1
    int2048 one(1);
    if (q.sign > 0) {
      q = abs_sub(q, one);
      q.sign = q.a.empty() ? 1 : 1;
    } else {
      q = abs_add(q, one);
      q.sign = -1;
    }
  }
  *this = q; trim();
  return *this;
}
int2048 operator/(int2048 x, const int2048 &y) { return x /= y; }

int2048 &int2048::operator%=(const int2048 &o) {
  if (o.a.empty()) return *this; // undefined
  if (a.empty()) return *this;
  // r = a - (a / b) * b, where / is floor division
  int2048 q = *this / o;
  int2048 r = *this - q * o;
  *this = r; trim();
  return *this;
}
int2048 operator%(int2048 x, const int2048 &y) { return x %= y; }

istream &operator>>(istream &is, int2048 &x) {
  string s; is >> s; x.read(s); return is;
}
ostream &operator<<(ostream &os, const int2048 &x) {
  if (x.a.empty()) { os << 0; return os; }
  if (x.sign < 0) os << '-';
  os << x.a.back();
  char buf[8];
  for (int i = (int)x.a.size() - 2; i >= 0; --i) {
    std::snprintf(buf, sizeof(buf), "%0*d", int2048::BASE_DIG, x.a[i]);
    os << buf;
  }
  return os;
}

bool operator==(const int2048 &x, const int2048 &y) {
  return x.sign == y.sign && x.a == y.a;
}
bool operator!=(const int2048 &x, const int2048 &y) { return !(x == y); }
bool operator<(const int2048 &x, const int2048 &y) {
  if (x.a.empty() && y.a.empty()) return false;
  if (x.sign != y.sign) return x.sign < y.sign;
  int cmp = int2048::abs_cmp(x, y);
  return x.sign > 0 ? (cmp < 0) : (cmp > 0);
}
bool operator>(const int2048 &x, const int2048 &y) { return y < x; }
bool operator<=(const int2048 &x, const int2048 &y) { return !(y < x); }
bool operator>=(const int2048 &x, const int2048 &y) { return !(x < y); }

} // namespace sjtu


#ifndef ONLINE_JUDGE
int main(){
  sjtu::int2048 a("") ; a.read("12345678901234567890");
  sjtu::int2048 b("") ; b.read("-98765432109876543210");
  cout << (a+b) << "\n";
  cout << (b-a) << "\n";
  cout << (a*b) << "\n";
  cout << (b/ a) << "\n";
  cout << (b% a) << "\n";
  return 0;
}
#endif
