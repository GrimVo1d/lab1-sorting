/// \file main.cpp
/// \brief Точка входа ЛР1: либо сортирует CSV-файл, либо запускает бенчмарки
///
/// Режимы:
///   ./hw1 sort INPUT.csv OUTPUT.csv
///       читает input, сортирует merge_sort'ом, пишет output
///   ./hw1 bench DATA_DIR RESULTS.csv
///       прогоняет три самописные сортировки + std::sort на всех CSV-файлах
///       DATA_DIR/teachers_*.csv и пишет таблицу замеров

#include "csv.hpp"
#include "sorts.hpp"
#include "teacher.hpp"
#include "timing.hpp"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace fs = std::filesystem;

namespace {

double median3(double a, double b, double c) {
    if (a > b) std::swap(a, b);
    if (b > c) std::swap(b, c);
    if (a > b) std::swap(a, b);
    return b;
}

template <typename Sorter>
double bench_one(const std::vector<pt::Teacher>& src, Sorter sorter) {
    double r[3];
    for (int k = 0; k < 3; ++k) {
        std::vector<pt::Teacher> v = src;
        r[k] = pt::measure_ms([&] { sorter(v); });
    }
    return median3(r[0], r[1], r[2]);
}

int run_sort(const std::string& in_path, const std::string& out_path) {
    auto v = pt::read_csv(in_path);
    pt::sorts::merge_sort(v);
    pt::write_csv(out_path, v);
    std::cerr << "Отсортировано " << v.size() << " записей -> " << out_path << "\n";
    return 0;
}

int run_bench(const std::string& data_dir, const std::string& out_path) {
    std::vector<fs::path> files;
    for (auto& e : fs::directory_iterator(data_dir)) {
        if (e.is_regular_file() && e.path().extension() == ".csv") {
            files.push_back(e.path());
        }
    }
    std::sort(files.begin(), files.end(), [](const fs::path& a, const fs::path& b) {
        return fs::file_size(a) < fs::file_size(b);
    });

    std::ofstream out(out_path);
    if (!out) {
        std::cerr << "Не удалось открыть " << out_path << "\n";
        return 1;
    }
    out << "n,bubble_ms,quick_ms,merge_ms,std_sort_ms\n";
    std::cout << std::fixed;

    for (const auto& f : files) {
        auto src = pt::read_csv(f.string());
        const std::size_t n = src.size();

        // На больших массивах пузырёк катастрофически медленный, пропускаем
        const bool skip_bubble = n > 50000;

        double t_bub  = skip_bubble ? -1.0
                       : bench_one(src, [](auto& v){ pt::sorts::bubble_sort(v); });
        double t_quick = bench_one(src, [](auto& v){ pt::sorts::quick_sort(v); });
        double t_merge = bench_one(src, [](auto& v){ pt::sorts::merge_sort(v); });
        double t_std   = bench_one(src, [](auto& v){ std::sort(v.begin(), v.end()); });

        out << n << ',' << t_bub << ',' << t_quick << ','
            << t_merge << ',' << t_std << '\n';
        std::cerr << "n=" << n
                  << "  bubble=" << t_bub
                  << "  quick="  << t_quick
                  << "  merge="  << t_merge
                  << "  std::sort=" << t_std << " ms\n";
    }
    return 0;
}

} // namespace

int main(int argc, char** argv) {
    try {
        if (argc >= 4 && std::string(argv[1]) == "sort") {
            return run_sort(argv[2], argv[3]);
        }
        if (argc >= 4 && std::string(argv[1]) == "bench") {
            return run_bench(argv[2], argv[3]);
        }
        std::cerr << "Использование:\n"
                  << "  " << argv[0] << " sort  <input.csv> <output.csv>\n"
                  << "  " << argv[0] << " bench <data_dir>  <results.csv>\n";
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << "\n";
        return 2;
    }
}
