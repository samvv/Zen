#ifndef ZEN_ALGORITHM_HPP
#define ZEN_ALGORITHM_HPP

#include "zen/meta.hpp"

ZEN_NAMESPACE_START

#define ZEN_CONSTEXPR constexpr

template<typename FnT, std::size_t ...J, typename ...Ts>
ZEN_CONSTEXPR auto convert_impl(const std::tuple<Ts...>& value, std::integer_sequence<std::size_t, J...>, FnT&& op) {
  return std::tuple<std::invoke_result_t<FnT, Ts>...> { op(std::get<J>(value))... };
}

template<typename FnT, typename ...Ts>
ZEN_CONSTEXPR auto convert(const std::tuple<Ts...>& value, FnT op) {
  using seq = std::make_index_sequence<std::tuple_size_v<std::tuple<Ts...>>>;
  return convert_impl(std::tuple<Ts...>(value), seq(), std::forward<FnT>(op));
}

template<typename T1, typename T2, typename FnT>
ZEN_CONSTEXPR auto convert(const std::pair<T1, T2>& value, FnT fn) {
  return std::make_pair<T1, T2>(fn(value.first), fn(value.second));
}

template<typename T>
auto decrement(T& value) {
  --value;
}

template<typename T>
auto increment(T& value) {
  ++value;
}

template<typename T>
T prev_n(T value, meta::difference_t<T> n) {
  auto curr = value;
  for (std::ptrdiff_t i = 0; i < n; i++) {
    decrement(curr);
  }
  return curr;
}

template<typename T, typename = void>
struct next_n_impl {
  static T apply(T& value, meta::difference_t<T> n) {
    auto curr = value;
    for (std::ptrdiff_t i = 0; i < n; i++) {
      increment(curr);
    }
    return curr;
  }
};

template<typename T>
T next_n(T& value, meta::difference_t<T> n) {
  return next_n_impl<T>::apply(value, n);
}

template<typename T, typename = void>
struct zip_impl;

template<typename ...Ts>
auto zip(Ts&&...args) {
  return zip_impl<std::tuple<Ts...>>::apply(std::forward<Ts>(args)...);
}

template<typename RangeT, typename FnT, typename OutIterT>
void transform(const RangeT& range, FnT func, OutIterT iter) {
  for (auto& element: range) {
    iter = func(element);
  }
}

template<typename RangeT>
typename meta::get_element_t<RangeT> last(RangeT range) {
  auto prev = range.begin();
  auto curr = prev;
  auto end = range.end();
  for (;;) {
    curr++;
    if (curr == end) {
      break;
    }
    prev = curr;
  }
  return *prev;
}

template<typename T>
typename std::enable_if<!std::is_const_v<T>, typename T::iterator>::type start(T& container) {
  return container.begin();
}

template<typename T>
typename std::enable_if<std::is_const_v<T>, typename T::const_iterator>::type start(T& container) {
  return container.cbegin();
}

template<typename T>
typename std::enable_if<!std::is_const_v<T>, typename T::iterator>::type stop(T& container) {
  return container.end();
}

template<typename T>
typename std::enable_if<std::is_const_v<T>, typename T::const_iterator>::type stop(T& container) {
  return container.cend();
}

ZEN_NAMESPACE_END

#endif // of #ifndef ZEN_ALGORITHM_HPP
