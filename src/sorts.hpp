/// \file sorts.hpp
/// \brief Шаблонные реализации трёх сортировок вариант 5: пузырёк, быстрая, слиянием
#pragma once

#include <vector>
#include <utility>

namespace pt::sorts {

/// \brief Сортировка пузырьком с оптимизацией флагом раннего выхода
/// \tparam T тип элементов; должен поддерживать operator<
template <typename T>
void bubble_sort(std::vector<T>& a) {
    const std::size_t n = a.size();
    for (std::size_t i = 0; i + 1 < n; ++i) {
        bool swapped = false;
        for (std::size_t j = n - 1; j > i; --j) {
            if (a[j] < a[j - 1]) {
                std::swap(a[j], a[j - 1]);
                swapped = true;
            }
        }
        if (!swapped) break; // ранний выход
    }
}

namespace detail {

template <typename T>
void quick_sort_impl(std::vector<T>& a, long lo, long hi) {
    if (lo >= hi) return;
    long i = lo, j = hi;
    T pivot = a[(lo + hi) >> 1];
    do {
        while (a[i] < pivot) ++i;
        while (a[j] > pivot) --j;
        if (i <= j) {
            std::swap(a[i], a[j]);
            ++i; --j;
        }
    } while (i <= j);
    if (lo < j) quick_sort_impl(a, lo, j);
    if (i < hi) quick_sort_impl(a, i, hi);
}

template <typename T>
void merge(std::vector<T>& a, std::vector<T>& buf,
           std::size_t lo, std::size_t mid, std::size_t hi) {
    std::size_t i = lo, j = mid, k = lo;
    while (i < mid && j < hi) {
        if (!(a[j] < a[i])) buf[k++] = a[i++];
        else                buf[k++] = a[j++];
    }
    while (i < mid) buf[k++] = a[i++];
    while (j < hi)  buf[k++] = a[j++];
    for (std::size_t t = lo; t < hi; ++t) a[t] = buf[t];
}

template <typename T>
void merge_sort_impl(std::vector<T>& a, std::vector<T>& buf,
                     std::size_t lo, std::size_t hi) {
    if (hi - lo < 2) return;
    std::size_t mid = lo + (hi - lo) / 2;
    merge_sort_impl(a, buf, lo, mid);
    merge_sort_impl(a, buf, mid, hi);
    merge(a, buf, lo, mid, hi);
}

} // namespace detail

/// \brief Быстрая сортировка с pivot = центральный элемент
/// Сложность: O(n log n) средняя, O(n^2) худшая
/// In-place (стек O(log n)), неустойчивая
template <typename T>
void quick_sort(std::vector<T>& a) {
    if (a.size() < 2) return;
    detail::quick_sort_impl(a, 0, static_cast<long>(a.size()) - 1);
}

/// \brief Сортировка слиянием
/// Сложность: O(n log n) во всех случаях; память O(n) (буфер)
/// Устойчивая
template <typename T>
void merge_sort(std::vector<T>& a) {
    if (a.size() < 2) return;
    std::vector<T> buf(a.size());
    detail::merge_sort_impl(a, buf, 0, a.size());
}

} // namespace pt::sorts
