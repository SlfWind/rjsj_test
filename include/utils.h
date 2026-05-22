#pragma once

#include <concepts>
#include <iterator>
#include <type_traits>

namespace exercise_detail {

template <typename Container>
using container_base_t = std::remove_cvref_t<Container>;

template <typename Container>
concept HasValueType = requires {
    typename container_base_t<Container>::value_type;
};

template <typename Container>
concept MutableForwardContainer = requires(Container &container) {
    typename container_base_t<Container>::value_type;
    typename container_base_t<Container>::iterator;
    { container.begin() } -> std::same_as<typename container_base_t<Container>::iterator>;
    { container.end() } -> std::same_as<typename container_base_t<Container>::iterator>;
} && std::forward_iterator<typename container_base_t<Container>::iterator>
  && std::permutable<typename container_base_t<Container>::iterator>;

template <typename Container>
concept RangeErasable = MutableForwardContainer<Container> &&
    requires(Container &container,
             typename container_base_t<Container>::iterator first,
             typename container_base_t<Container>::iterator last) {
        { container.erase(first, last) } -> std::same_as<typename container_base_t<Container>::iterator>;
    };

template <typename Container>
concept ForwardListRangeErasable = MutableForwardContainer<Container> &&
    requires(Container &container,
             typename container_base_t<Container>::iterator first,
             typename container_base_t<Container>::iterator last) {
        { container.erase_after(first, last) } -> std::same_as<typename container_base_t<Container>::iterator>;
    };

template <typename Container>
concept SequentialContainer = MutableForwardContainer<Container> &&
    !requires { typename container_base_t<Container>::key_type; } &&
    (RangeErasable<Container> || ForwardListRangeErasable<Container>);

template <typename Elem>
concept LessComparable = requires(const std::remove_cvref_t<Elem> &lhs,
                                  const std::remove_cvref_t<Elem> &rhs) {
    { lhs < rhs } -> std::convertible_to<bool>;
};

template <typename Container, typename Elem>
concept SequentialContainerOf = SequentialContainer<Container> &&
    !std::is_reference_v<Elem> &&
    std::same_as<
        std::remove_cv_t<typename container_base_t<Container>::value_type>,
        std::remove_cv_t<Elem>>;

template <typename Compare, typename Elem>
concept SortComparator =
    std::strict_weak_order<Compare &, const Elem &, const Elem &>;

} // namespace exercise_detail
