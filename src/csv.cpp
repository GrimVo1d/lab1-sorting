/// \file csv.cpp
/// \brief Реализация CSV-парсера
#include "csv.hpp"

#include <fstream>
#include <stdexcept>

namespace pt {

namespace {

/// Разбирает одну CSV-строку в поля; поддерживает кавычки для полей с запятой
std::vector<std::string> split_csv_line(const std::string& line) {
    std::vector<std::string> out;
    std::string cur;
    bool in_quotes = false;
    for (size_t i = 0; i < line.size(); ++i) {
        char c = line[i];
        if (c == '"') {
            if (in_quotes && i + 1 < line.size() && line[i+1] == '"') {
                cur.push_back('"');
                ++i;
            } else {
                in_quotes = !in_quotes;
            }
        } else if (c == ',' && !in_quotes) {
            out.push_back(cur);
            cur.clear();
        } else {
            cur.push_back(c);
        }
    }
    out.push_back(cur);
    return out;
}

} // namespace

std::vector<Teacher> read_csv(const std::string& path) {
    std::ifstream in(path);
    if (!in) throw std::runtime_error("Не удалось открыть файл: " + path);
    std::vector<Teacher> result;
    std::string line;
    bool first = true;
    while (std::getline(in, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back();
        if (first) { first = false; continue; } // skip header
        if (line.empty()) continue;
        auto parts = split_csv_line(line);
        if (parts.size() < 4) {
            throw std::runtime_error("Битая строка CSV: " + line);
        }
        Teacher t;
        t.fio     = parts[0];
        t.faculty = parts[1];
        t.title   = title_from_string(parts[2]);
        t.degree  = degree_from_string(parts[3]);
        result.push_back(std::move(t));
    }
    return result;
}

void write_csv(const std::string& path, const std::vector<Teacher>& v) {
    std::ofstream out(path);
    if (!out) throw std::runtime_error("Не удалось открыть для записи: " + path);
    out << "fio,faculty,title,degree\n";
    for (const auto& t : v) out << t << '\n';
}

} // namespace pt
