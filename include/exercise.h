#pragma once

#include <functional>
#include <iterator>
#include <type_traits>
#include <utility>
#include <vector>

#include "utils.h"

using std::vector;

void exercise_wb_1(int, int, int);
void exercise_wb_2(int, int);
void exercise_wb_3(vector<int>);

/**
 * @brief 第一题，简单的等价类划分和边界值分析。
 * @details
 * 此函数用于百分制分数到等级制分数的转换，具体来说：
 * 1. [85, 100] => 返回A
 * 2. [75, 85)  => 返回B
 * 3. [65, 75)  => 返回C
 * 4. [60, 65)  => 返回D
 * 5. [0, 60)   => 返回F
 * 6. 其他情况，返回E代表错误
 * @param grade 百分制分数，可以有小数
 */
char exercise_bb_1(double);

/**
 * @brief 第二题，多元等价类划分和边界值分析。
 * @details
 * 此函数用于将传入的数据结构中的元素进行去重，然后进行排序。
 * Container 需要是可写的顺序访问容器，元素类型需要是 Elem，
 * Elem 需要支持小于比较以便在未指定比较函数时执行升序排序。
 * @param container 传入的容器，要求可写
 * @param cmp 传入的比较函数，作为排序的依据，缺省则默认从小到大排序
 * @return 若去重和排序正常进行，返回true，否则返回false
 */
template <typename Container, typename Elem, typename Compare>
    requires exercise_detail::SequentialContainerOf<Container, Elem> &&
             exercise_detail::LessComparable<Elem> &&
             exercise_detail::SortComparator<Compare, Elem>
bool exercise_bb_2(Container &container, Compare cmp)
{
    if constexpr (requires { static_cast<bool>(cmp); })
    {
        if (!static_cast<bool>(cmp))
        {
            return false;
        }
    }

    try
    {
        for (auto current = container.begin(); current != container.end(); ++current)
        {
            auto best = current;
            auto candidate = current;
            ++candidate;

            for (; candidate != container.end(); ++candidate)
            {
                if (cmp(*candidate, *best))
                {
                    best = candidate;
                }
            }

            if (best != current)
            {
                std::iter_swap(current, best);
            }
        }

        auto write = container.begin();
        if (write == container.end())
        {
            return true;
        }

        auto read = write;
        ++read;
        for (; read != container.end(); ++read)
        {
            const bool equivalent = !cmp(*write, *read) && !cmp(*read, *write);
            if (!equivalent)
            {
                ++write;
                if (write != read)
                {
                    *write = std::move(*read);
                }
            }
        }

        auto erase_begin = write;
        ++erase_begin;
        if constexpr (exercise_detail::RangeErasable<Container>)
        {
            container.erase(erase_begin, container.end());
        }
        else if (erase_begin != container.end())
        {
            container.erase_after(write, container.end());
        }
    }
    catch (...)
    {
        return false;
    }

    return true;
}

template <typename Container, typename Elem, typename Compare>
    requires(!exercise_detail::SequentialContainerOf<Container, Elem> ||
             !exercise_detail::LessComparable<Elem> ||
             !exercise_detail::SortComparator<Compare, Elem>)
bool exercise_bb_2(Container &, Compare)
{
    return false;
}

template <typename Container, typename Elem>
bool exercise_bb_2(Container &container, std::function<bool(const Elem &, const Elem &)> cmp)
{
    return exercise_bb_2<Container, Elem,
                         std::function<bool(const Elem &, const Elem &)>>(
        container, cmp);
}

template <typename Container, typename Elem>
bool exercise_bb_2(Container &container)
{
    if constexpr (exercise_detail::SequentialContainerOf<Container, Elem> &&
                  exercise_detail::LessComparable<Elem>)
    {
        return exercise_bb_2<Container, Elem>(
            container, std::less<std::remove_cv_t<Elem>>{});
    }
    else
    {
        return false;
    }
}

template <typename Container, typename Compare>
bool exercise_bb_2(Container &container, Compare cmp)
{
    if constexpr (exercise_detail::HasValueType<Container>)
    {
        using Elem = typename exercise_detail::container_base_t<Container>::value_type;
        return exercise_bb_2<Container, Elem>(container, cmp);
    }
    else
    {
        return false;
    }
}

template <typename Container>
bool exercise_bb_2(Container &container)
{
    if constexpr (exercise_detail::HasValueType<Container>)
    {
        using Elem = typename exercise_detail::container_base_t<Container>::value_type;
        return exercise_bb_2<Container, Elem>(container);
    }
    else
    {
        return false;
    }
}
