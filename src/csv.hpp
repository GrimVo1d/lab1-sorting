/// \file csv.hpp
/// \brief Простой CSV-парсер без сторонних зависимостей
#pragma once

#include "teacher.hpp"

#include <string>
#include <vector>

namespace pt {

/// \brief Читает CSV-файл с колонками fio,faculty,title,degree (с заголовком)
/// \throw std::runtime_error если файл не открылся или формат битый
std::vector<Teacher> read_csv(const std::string& path);

/// \brief Пишет вектор преподавателей в CSV (с заголовком)
void write_csv(const std::string& path, const std::vector<Teacher>& v);

} // namespace pt
