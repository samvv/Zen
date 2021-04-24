#ifndef ZEN_ITERATOR_ADAPTOR_HPP
#define ZEN_ITERATOR_ADAPTOR_HPP

#include <cstddef>
#include <cstdint>
#include <iostream>
#include <iterator>
#include <type_traits>

#include "zen/meta.hpp"
#include "zen/algorithm.hpp"

ZEN_NAMESPACE_START

template<
  typename BaseT
, typename ValueT
, typename ReferenceT = std::add_lvalue_reference<ValueT>
, typename DifferenceT = std::ptrdiff_t
, typename PointerT = ValueT*
>
class iterator_adaptor;

template<typename T, typename = void>
struct has_decrement : std::false_type {};

template<typename T>
struct has_decrement<T,
    std::void_t< decltype(std::declval<T&>().decrement()) >
  > : std::true_type {};

template<typename T>
constexpr const bool has_decrement_v = has_decrement<T>::value;

template<typename T, typename = void>
struct has_increment : std::false_type {};

template<typename T>
struct has_increment<T,
    std::void_t< decltype(std::declval<T&>().increment()) >
  > : std::true_type {};

template<typename T>
constexpr const bool has_increment_v = has_increment<T>::value;

template<typename T, typename = void>
struct has_next_n : std::false_type {};

template<typename T>
struct has_next_n<T,
    std::void_t< decltype(std::declval<T&>().next_n(std::declval<std::ptrdiff_t>())) >
  > : std::true_type {};

template<typename T>
constexpr const bool has_next_n_v = has_next_n<T>::value;

template<typename T, typename = void>
struct has_prev_n : std::false_type {};

template<typename T>
struct has_prev_n<T,
    std::void_t< decltype(std::declval<T&>().prev_n(std::declval<std::ptrdiff_t>())) >
  > : std::true_type {};

template<typename T>
constexpr const bool has_prev_n_v = has_prev_n<T>::value;

template<typename T>
struct next_n_impl<T, std::enable_if_t<has_next_n_v<T>>> {
  static T apply(T& curr, typename T::difference_type n) {
    return curr.next_n(n);
  }
};

template<typename T, typename ValueT, typename ReferenceT, typename DifferenceT>
std::enable_if_t<has_prev_n_v<iterator_adaptor<T, ValueT, ReferenceT, DifferenceT>>, T> prev_n(T curr, typename T::difference_type n) {
  return curr.prev_n(n);
}

template<
  typename BaseT
, typename ValueT
, typename ReferenceT
, typename DifferenceT
, typename PointerT
>
class iterator_adaptor {
public:

  using iterator_category = std::random_access_iterator_tag;
  using value_type = ValueT;
  using reference = ReferenceT;
  using difference_type = DifferenceT;
  using pointer = PointerT;

  reference operator*() {
    return static_cast<BaseT*>(this)->dereference();
  }

  BaseT& operator++() {
    auto& base = static_cast<BaseT&>(*this);
    base.increment();
    return base;
  }

  BaseT& operator--() {
    auto& base = static_cast<BaseT&>(*this);
    base.decrement();
    return base;
  }

  BaseT operator+(std::ptrdiff_t n) {
    return ::zen::next_n(static_cast<BaseT&>(*this), n);
  }

  BaseT operator-(std::ptrdiff_t n) {
    return ::zen::prev_n(static_cast<BaseT>(*this), n);
  }

};

ZEN_NAMESPACE_END

#endif // ZEN_ITERATOR_ADAPTOR_HPP
