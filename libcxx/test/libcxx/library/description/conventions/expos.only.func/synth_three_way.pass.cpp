//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17, libcpp-no-concepts
// ADDITIONAL_COMPILE_FLAGS: -Wno-sign-compare

// constexpr auto __synth_three_way = ...;

#include <cassert>
#include <compare>
#include <limits>
#include <utility> // Includes synth-three-way via std::pair::operator<=>

#include "test_macros.h"

template <typename T> concept can_synth_three_way = requires(T t) { std::__synth_three_way(t, t); };

// A custom three-way result type
struct CustomEquality {
  friend constexpr bool operator==(const CustomEquality&, int) noexcept { return true; }
  friend constexpr bool operator<(const CustomEquality&, int) noexcept { return false; }
  friend constexpr bool operator<(int, const CustomEquality&) noexcept { return false; }
};

constexpr bool test() {
  {
    assert(std::__synth_three_way(1, 1) == std::strong_ordering::equal);
    assert(std::__synth_three_way(2, 1) == std::strong_ordering::greater);
    assert(std::__synth_three_way(1, 2) == std::strong_ordering::less);
    ASSERT_SAME_TYPE(std::strong_ordering, std::__synth_three_way_result<int, int>);
    ASSERT_SAME_TYPE(std::strong_ordering, std::__synth_three_way_result<short, long long int>);
  }
  {
    constexpr double nan = std::numeric_limits<double>::quiet_NaN();
    assert(std::__synth_three_way(1.0, 1.0) == std::partial_ordering::equivalent);
    assert(std::__synth_three_way(2.0, 1.0) == std::partial_ordering::greater);
    assert(std::__synth_three_way(1.0, 2.0) == std::partial_ordering::less);
    assert(std::__synth_three_way(nan, nan) == std::partial_ordering::unordered);
    ASSERT_SAME_TYPE(std::partial_ordering, std::__synth_three_way_result<double, double>);
    ASSERT_SAME_TYPE(std::partial_ordering, std::__synth_three_way_result<double, float>);
    ASSERT_SAME_TYPE(std::partial_ordering, std::__synth_three_way_result<double, int>);
    ASSERT_SAME_TYPE(std::partial_ordering, std::__synth_three_way_result<float, short>);
  }
  {
    struct StrongSpaceship {
      int value;
      constexpr bool operator==(const StrongSpaceship&) const = default;
      constexpr std::strong_ordering operator<=>(const StrongSpaceship& other) const { return value <=> other.value; }
    };
    assert(std::__synth_three_way(StrongSpaceship{1}, StrongSpaceship{1}) == std::strong_ordering::equal);
    assert(std::__synth_three_way(StrongSpaceship{2}, StrongSpaceship{1}) == std::strong_ordering::greater);
    assert(std::__synth_three_way(StrongSpaceship{1}, StrongSpaceship{2}) == std::strong_ordering::less);
    ASSERT_SAME_TYPE(std::strong_ordering, std::__synth_three_way_result<StrongSpaceship, StrongSpaceship>);
  }
  {
    struct WeakSpaceship {
      int value;
      constexpr bool operator==(const WeakSpaceship&) const = default;
      constexpr std::weak_ordering operator<=>(const WeakSpaceship& other) const {
        return value <=> other.value;
      }
    };
    assert(std::__synth_three_way(WeakSpaceship{1}, WeakSpaceship{1}) == std::weak_ordering::equivalent);
    assert(std::__synth_three_way(WeakSpaceship{2}, WeakSpaceship{1}) == std::weak_ordering::greater);
    assert(std::__synth_three_way(WeakSpaceship{1}, WeakSpaceship{2}) == std::weak_ordering::less);
    ASSERT_SAME_TYPE(std::weak_ordering, std::__synth_three_way_result<WeakSpaceship, WeakSpaceship>);
  }
  {
    struct PartialSpaceship {
      double value;
      constexpr bool operator==(const PartialSpaceship&) const = default;
      constexpr std::partial_ordering operator<=>(const PartialSpaceship& other) const {
        return value <=> other.value;
      }
    };
    constexpr double nan = std::numeric_limits<double>::quiet_NaN();
    assert(std::__synth_three_way(PartialSpaceship{1.0}, PartialSpaceship{1.0}) == std::partial_ordering::equivalent);
    assert(std::__synth_three_way(PartialSpaceship{2.0}, PartialSpaceship{1.0}) == std::partial_ordering::greater);
    assert(std::__synth_three_way(PartialSpaceship{1.0}, PartialSpaceship{2.0}) == std::partial_ordering::less);
    assert(std::__synth_three_way(PartialSpaceship{nan}, PartialSpaceship{nan}) == std::partial_ordering::unordered);
    ASSERT_SAME_TYPE(std::partial_ordering, std::__synth_three_way_result<PartialSpaceship, PartialSpaceship>);
  }
  {
    struct NoSpaceship {
      int value;
      constexpr bool operator==(const NoSpaceship&) const = default;
      constexpr bool operator<(const NoSpaceship& other) const { return value < other.value; }
    };
    assert(std::__synth_three_way(NoSpaceship{1}, NoSpaceship{1}) == std::weak_ordering::equivalent);
    assert(std::__synth_three_way(NoSpaceship{2}, NoSpaceship{1}) == std::weak_ordering::greater);
    assert(std::__synth_three_way(NoSpaceship{1}, NoSpaceship{2}) == std::weak_ordering::less);
    ASSERT_SAME_TYPE(std::weak_ordering, std::__synth_three_way_result<NoSpaceship, NoSpaceship>);
  }
  {
    // Types with operator<=> but no operator== are not three_way_comparable and will fall back to operator< and
    // compare as weakly ordered.
    struct SpaceshipNoEquals {
      constexpr std::strong_ordering operator<=>(const SpaceshipNoEquals&) const {
        return std::strong_ordering::equivalent;
      }
    };
    assert(std::__synth_three_way(SpaceshipNoEquals{}, SpaceshipNoEquals{}) == std::weak_ordering::equivalent);
    ASSERT_SAME_TYPE(std::weak_ordering, std::__synth_three_way_result<SpaceshipNoEquals, SpaceshipNoEquals>);
  }
  {
    // Custom three-way-comparison result types cannot satisfy standard concepts (and therefore synth-three-way)
    // because they are not understood by std::common_comparison_category, but they can still be used in
    // the same way as standard orderings to do comparisons, and thus can be used by synth-three-way to yield a
    // weakly-ordered result.
    struct CustomSpaceship {
      constexpr CustomEquality operator<=>(const CustomSpaceship&) const { return CustomEquality(); }
    };
    assert((CustomSpaceship() <=> CustomSpaceship()) == 0);
    assert(!(CustomSpaceship() < CustomSpaceship()));
    assert(std::__synth_three_way(CustomSpaceship(), CustomSpaceship()) == std::weak_ordering::equivalent);
    ASSERT_SAME_TYPE(std::weak_ordering, std::__synth_three_way_result<CustomSpaceship, CustomSpaceship>);
  }
  // SFINAE tests demonstrating synth-three-way needs three_way_comparable or operator<.
  {
    struct NoRelative {
      constexpr bool operator==(const NoRelative&) const;
    };
    static_assert(!can_synth_three_way<NoRelative>);
  }
  {
    struct NoLessThan {
      constexpr bool operator==(const NoLessThan&) const;
      constexpr bool operator>(const NoLessThan&) const;
      constexpr bool operator>=(const NoLessThan&) const;
      constexpr bool operator<=(const NoLessThan&) const;
    };
    static_assert(!can_synth_three_way<NoLessThan>);
  }
  {
    assert(std::__synth_three_way(1, 1U) == std::weak_ordering::equivalent);
    assert(std::__synth_three_way(-1, 0U) == std::weak_ordering::greater);
    // Even with the warning suppressed (-Wno-sign-compare) there should still be no <=> operator
    // between signed and unsigned types, so we should end up with a synthesized weak ordering.
    ASSERT_SAME_TYPE(std::weak_ordering, std::__synth_three_way_result<int, unsigned int>);
    // When an unsigned type can be narrowed to a larger signed type, <=> should be defined and we
    // should get a strong ordering. (This probably does not raise a warning due to safe narrowing.)
    assert((static_cast<long long int>(-1) <=> static_cast<unsigned char>(0)) == std::strong_ordering::less);
    assert(std::__synth_three_way(static_cast<long long int>(-1),
                                  static_cast<unsigned char>(0)) == std::strong_ordering::less);
    ASSERT_SAME_TYPE(std::strong_ordering, std::__synth_three_way_result<long long int, unsigned char>);
  }

  return true;
}

int main(int, char**) {
  test();
  static_assert(test());

  {
    constexpr double nan = std::numeric_limits<double>::quiet_NaN();
    assert(std::__synth_three_way(nan, 1.0) == std::partial_ordering::unordered);
  }

  return 0;
}
