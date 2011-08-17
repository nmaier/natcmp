/* ************************************************************************ *
 To the extent possible under law, Nils Maier has waived all copyright and
 related or neighboring rights this demonstration.
 http://creativecommons.org/publicdomain/zero/1.0/
 * ************************************************************************ */

// Included first to check that the header is indeed self-contained
#include "natcmp.h"

#include <algorithm>
#include <iostream>
#include <vector>
#include <string>

using namespace std;

bool sort_sc(const wstring &a, const wstring &b) {
  return wcsnatcmp(a, b) < 0;
}
bool sort_ic(const wstring &a, const wstring &b) {
  return wcsnatcasecmp(a, b) < 0;
}

typedef vector<const wchar_t *> strvec;
typedef strvec::const_iterator strvec_cit;

const wchar_t* invec[] = {
  L"0x01",
  L"0x02",
  L"0xaf",
  L"10x01",
  L"9x01",
  L"-10",
  L"-1",
  L"1",
  L"01",
  L"001",
  L"0001",
  L"0000001",
  L"0000000001",
  L"0000000000001",
  L"000000000000000000001",
  L"01",
  L"0000000000001",
  L"10",
  L"2",
  L"hallow-1",
  L"hallow1",
  L"hallow10",
  L"hallow-10",
  L"hallow2",
  L"hallow-1foo",
  L"hallow1foo",
  L"hallow109xfoo",
  L"hallow109",
  L"hallow109",
  L"hallow109zfoo",
  L"hallow10zfoo",
  L"hallow10foobar",
  L"hallow10foo",
  L"hallow-10foo",
  L"hallow2foo",
  L"bar-1foo",
  L"BAR0foo",
  L"bar1foo",
  L"bar10foo",
  L"bar-10foo",
  L"bar101foo",
  L"bar100foo",
  L"bar200foo",
  L"bar20foo",
  L"bar2foo",
  L"44",
  L"44 (1)",
  L"44 (2)",
  L"44 (3)",
  L"44(4)",
  L"44(5)",
  // Atwood: http://www.codinghorror.com/blog/2007/12/sorting-for-humans-natural-sort-order.html
  L"z24", L"z2", L"z15", L"z1", L"z3", L"z20", L"z5", L"z11", L"z 21", L"z22"
};

#ifdef _UNICODE
#define _main wmain
#else
#define _main main
#endif

int _main() {
  strvec vsort;
  vsort.assign(invec, invec + (sizeof(invec) / sizeof(*invec)));

  wcout << "case-sensitive:" << endl;
  random_shuffle(vsort.begin(), vsort.end());
  sort(vsort.begin(), vsort.end(), sort_sc);
  for (strvec_cit i = vsort.begin(), e = vsort.end(); i != e; ++i) {
    wcout << L"\t" << *i << endl;
  }

  wcout << endl;

  wcout << "case-insensitive" << endl;
  random_shuffle(vsort.begin(), vsort.end());
  sort(vsort.begin(), vsort.end(), sort_ic);
  for (strvec_cit i = vsort.begin(), e = vsort.end(); i != e; ++i) {
    wcout << L"\t" << *i << endl;
  }

  return 0;
}
