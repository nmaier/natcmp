/* ************************************************************************ *
 To the extent possible under law, Nils Maier has waived all copyright and
 related or neighboring rights to natcmp.
 http://creativecommons.org/publicdomain/zero/1.0/
 * ************************************************************************ */

/**
 * Yet Another Natural sorting aka. alphanum sorting aka. human sorting
 *
 * Simple templated implementation and inlines for char/wchar_t/std::string
 * in case-sensitive/case-insensitive flavors.
 *
 * Allows to create different implementations from the same template, e.g. for
 * other string classes or transformations.
 *
 * Tested with MSVC 9, icl 12 and g++ 4.4/4.5
 */

#ifndef __NATCAMP_H__
#define __NATCAMP_H__

#include <wchar.h>
#include <ctype.h>
#include <string>
#include <stdlib.h>

namespace string_natural_compare_ns {

  template<typename char_type, class func_tol>
  int impl(const char_type *s1, const char_type *s2, func_tol ToLong)
  {
    for (
      char_type c1, c2, *ep1, *ep2;
      (c1 = *s1) && (c2 = *s2);
      ++s1, ++s2
    ) {

      // Get leading numbers
      // Note: the CRT tol implementation will remove leading white space
      // This is ok for our purpose
      const long n1 = ToLong(s1, &ep1, 10),
        n2 = ToLong(s2, &ep2, 10);

      // Got numbers?
      const bool isn1 = n1 || s1 != ep1,
        isn2 = n2 || s2 != ep2;

      // Both not a number; regular character compare
      if (!isn1 && !isn2) {
        const int diff = (int)c1 - c2;
        if (diff) {
          return diff;
        }
      }

      // Both are numbers
      else if (isn1 && isn2) {
        if (n1 > n2) {
          return 1;
        }
        if (n2 > n1) {
          return -1;
        }

        // Same number, but might contain different front-padding
        // Larger front-padding is cmp-smaller
        const size_t d1 = ep1 - s1,
          d2 = ep2 - s2;
        if (d1 < d2) {
          return 1;
        }
        if (d2 < d1) {
          return -1;
        }

        // Same number
        s1 = ep1 - 1;
        s2 = ep2 - 1;
      }

      // s1 contains a number, while s2 does not
      // hence s1 < s2
      else if (isn1) {
        return -1;
      }

      // s2 contains a number, while s2 does not
      // hence s1 > s2
      else if (isn2) {
        return 1;
      }
    }

    // len(s1) > len(s2)
    if (*s1) {
      return 1;
    }
    // len(s1) < len(s2)
    if (*s2) {
      return -1;
    }

    return 0;
  }

  // version with transformations
  template<typename char_type, class func_tol, class func_transform>
  int impl(const char_type *s1, const char_type *s2, func_tol ToLong, func_transform Transform)
  {
    // For comments see implementation without transformations
    for (
      char_type c1, c2, *ep1, *ep2;
      (c1 = *s1) && (c2 = *s2);
      ++s1, ++s2
    ) {
      const long n1 = ToLong(s1, &ep1, 10),
        n2 = ToLong(s2, &ep2, 10);
      const bool isn1 = n1 || s1 != ep1,
        isn2 = n2 || s2 != ep2;
      if (!isn1 && !isn2) {
        const int diff = (int)Transform(c1) - Transform(c2);
        if (diff) {
          return diff;
        }
      }
      else if (isn1 && isn2) {
        if (n1 > n2) {
          return 1;
        }
        if (n2 > n1) {
          return -1;
        }
        const size_t d1 = ep1 - s1,
          d2 = ep2 - s2;
        if (d1 < d2) {
          return 1;
        }
        if (d2 < d1) {
          return -1;
        }
        s1 = ep1 - 1;
        s2 = ep2 - 1;
      }
      else if (isn1) {
        return -1;
      }
      else if (isn2) {
        return 1;
      }
    }
    if (*s1) {
      return 1;
    }
    if (*s2) {
      return -1;
    }
    return 0;
  }
};

/**
 * Compare two strings corresponding to lexicographically order
 *
 * @param s1 First string
 * @param s2 Second string
 * @return <0 if s1 < s2, >0 if s2 > s1 or 0 if s1 == s2
 */
inline int strnatcmp(const char* s1, const char *s2) {
  return string_natural_compare_ns::impl(s1, s2, ::strtol);
}

/**
 * Compare two strings corresponding to lexicographically order
 *
 * @param s1 First string
 * @param s2 Second string
 * @return <0 if s1 < s2, >0 if s2 > s1 or 0 if s1 == s2
 */
inline int strnatcmp(const std::string& s1, const std::string& s2) {
  return string_natural_compare_ns::impl(s1.c_str(), s2.c_str(), ::strtol);
}

/**
 * Compare two wide strings corresponding to lexicographically order
 *
 * @param s1 First string
 * @param s2 Second string
 * @return <0 if s1 < s2, >0 if s2 > s1 or 0 if s1 == s2
 */
inline int wcsnatcmp(const wchar_t* s1, const wchar_t *s2) {
  return string_natural_compare_ns::impl(s1, s2, ::wcstol);
}

/**
 * Compare two wide strings corresponding to lexicographically order
 *
 * @param s1 First string
 * @param s2 Second string
 * @return <0 if s1 < s2, >0 if s2 > s1 or 0 if s1 == s2
 */
inline int wcsnatcmp(const std::wstring& s1, const std::wstring& s2) {
  return string_natural_compare_ns::impl(s1.c_str(), s2.c_str(), ::wcstol);
}

/**
 * Compare two case-insensitive strings corresponding to lexicographically order
 *
 * @param s1 First string
 * @param s2 Second string
 * @return <0 if s1 < s2, >0 if s2 > s1 or 0 if s1 == s2
 */
inline int strnatcasecmp(const char* s1, const char *s2) {
  return string_natural_compare_ns::impl(s1, s2, ::strtol, ::tolower);
}

/**
 * Compare two case-insensitive strings corresponding to lexicographically order
 *
 * @param s1 First string
 * @param s2 Second string
 * @return <0 if s1 < s2, >0 if s2 > s1 or 0 if s1 == s2
 */
inline int strnatcasecmp(const std::string& s1, const std::string& s2) {
  return string_natural_compare_ns::impl(s1.c_str(),
                                         s2.c_str(),
                                         ::strtol,
                                         ::tolower
                                         );
}

/**
 * Compare two case-insensitive wide strings corresponding to lexicographically
 * order
 *
 * @param s1 First string
 * @param s2 Second string
 * @return <0 if s1 < s2, >0 if s2 > s1 or 0 if s1 == s2
 */
inline int wcsnatcasecmp(const wchar_t* s1, const wchar_t *s2) {
  return string_natural_compare_ns::impl(s1, s2, ::wcstol, ::towlower);
}

/**
 * Compare two case-insensitive wide strings corresponding to lexicographically
 * order
 *
 * @param s1 First string
 * @param s2 Second string
 * @return <0 if s1 < s2, >0 if s2 > s1 or 0 if s1 == s2
 */
inline int wcsnatcasecmp(const std::wstring& s1, const std::wstring& s2) {
  return string_natural_compare_ns::impl(s1.c_str(),
                                         s2.c_str(),
                                         ::wcstol,
                                         ::towlower
                                         );
}

#endif // __NATCAMP_H__
