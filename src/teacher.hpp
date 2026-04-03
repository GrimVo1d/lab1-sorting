/// \file teacher.hpp
/// \brief Доменная модель преподавателя (вариант 5 ЛР1)
///
/// Содержит структуру Teacher и перегруженные операторы сравнения
/// в порядке: факультет -> ФИО -> учёная степень -> учёное звание
#pragma once

#include <string>
#include <iosfwd>

namespace pt {

/// \brief Учёная степень; порядок задаёт ранг для сравнения
enum class Degree : int {
    None    = 0, ///< без степени
    Cand    = 1, ///< кандидат наук
    Doctor  = 2  ///< доктор наук
};

/// \brief Учёное звание; порядок задаёт ранг для сравнения
enum class Title : int {
    None       = 0, ///< без звания
    Assistant  = 1, ///< ассистент
    Docent     = 2, ///< доцент
    Professor  = 3  ///< профессор
};

/// \brief Преподаватель: ФИО, факультет, звание, степень
struct Teacher {
    std::string fio;       ///< Фамилия Имя Отчество
    std::string faculty;   ///< Факультет
    Title       title;     ///< Учёное звание
    Degree      degree;    ///< Учёная степень

    /// \brief Возвращает <0 если *this < other, 0 если равны, >0 если *this > other
    /// Порядок сравнения по варианту 5: факультет, ФИО, степень, звание
    int compare(const Teacher& other) const noexcept;
};

/// \name Перегруженные операторы сравнения
/// Реализованы через единый Teacher::compare для согласованности
/// @{
bool operator<(const Teacher& a, const Teacher& b) noexcept;
bool operator>(const Teacher& a, const Teacher& b) noexcept;
bool operator<=(const Teacher& a, const Teacher& b) noexcept;
bool operator>=(const Teacher& a, const Teacher& b) noexcept;
bool operator==(const Teacher& a, const Teacher& b) noexcept;
bool operator!=(const Teacher& a, const Teacher& b) noexcept;
/// @}

/// \brief Парсит строку "к.н."/"д.н."/"" -> Degree
Degree degree_from_string(const std::string& s);
/// \brief Парсит "ассистент"/"доцент"/"профессор"/"" -> Title
Title  title_from_string(const std::string& s);

/// \brief Имя степени для записи обратно в CSV
std::string degree_to_string(Degree d);
/// \brief Имя звания для записи обратно в CSV
std::string title_to_string(Title t);

/// \brief Печать одного преподавателя в поток (формат CSV без перевода строки)
std::ostream& operator<<(std::ostream& os, const Teacher& t);

} // namespace pt
