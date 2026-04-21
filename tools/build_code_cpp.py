#!/usr/bin/env python3
import pathlib

root = pathlib.Path(__file__).resolve().parents[1]
hdr = root/"src"/"include"/"int2048.h"
impl = root/"src"/"int2048.cpp"
out = root/"code.cpp"

header_text = hdr.read_text(encoding='utf-8')
impl_text = impl.read_text(encoding='utf-8')

# Remove local include from implementation to make single file
impl_text = impl_text.replace('#include "int2048.h"', '')

boiler = """// Generated code.cpp for ACMOJ submission
// Contains int2048 class declaration and implementation
#include <bits/stdc++.h>
using namespace std;
"""

test_main = """
#ifndef ONLINE_JUDGE
int main(){
  sjtu::int2048 a("") ; a.read("12345678901234567890");
  sjtu::int2048 b("") ; b.read("-98765432109876543210");
  cout << (a+b) << "\\n";
  cout << (b-a) << "\\n";
  cout << (a*b) << "\\n";
  cout << (b/ a) << "\\n";
  cout << (b% a) << "\\n";
  return 0;
}
#endif
"""

with out.open('w', encoding='utf-8') as f:
    f.write(boiler)
    f.write('\n')
    f.write(header_text)
    f.write('\n')
    f.write(impl_text)
    f.write('\n')
    f.write(test_main)

print(f"Wrote {out}")
