/// \file teacher.cpp
/// \brief Реализация Teacher и операторов сравнения
#include "teacher.hpp"

#include <ostream>

namespace pt {

int Teacher::compare(const Teacher& o) const noexcept {
    if (faculty != o.faculty) return faculty < o.faculty ? -1 : 1;
    if (fio     != o.fio)     return fio     < o.fio     ? -1 : 1;
    if (degree  != o.degree)  return static_cast<int>(degree) < static_cast<int>(o.degree) ? -1 : 1;
    if (title   != o.title)   return static_cast<int>(title)  < static_cast<int>(o.title)  ? -1 : 1;
    return 0;
}

bool operator<(const Teacher& a, const Teacher& b)  noexcept { return a.compare(b) <  0; }
bool operator>(const Teacher& a, const Teacher& b)  noexcept { return a.compare(b) >  0; }
bool operator<=(const Teacher& a, const Teacher& b) noexcept { return a.compare(b) <= 0; }
bool operator>=(const Teacher& a, const Teacher& b) noexcept { return a.compare(b) >= 0; }
bool operator==(const Teacher& a, const Teacher& b) noexcept { return a.compare(b) == 0; }
bool operator!=(const Teacher& a, const Teacher& b) noexcept { return a.compare(b) != 0; }

Degree degree_from_string(const std::string& s) {
    if (s == "к.н." || s == "kn" || s == "cand")   return Degree::Cand;
    if (s == "д.н." || s == "dn" || s == "doctor") return Degree::Doctor;
    return Degree::None;
}

Title title_from_string(const std::string& s) {
    if (s == "ассистент" || s == "assistant") return Title::Assistant;
    if (s == "доцент"    || s == "docent")    return Title::Docent;
    if (s == "профессор" || s == "professor") return Title::Professor;
    return Title::None;
}

std::string degree_to_string(Degree d) {
    switch (d) {
        case Degree::Cand:   return "к.н.";
        case Degree::Doctor: return "д.н.";
        default:             return "";
    }
}

std::string title_to_string(Title t) {
    switch (t) {
        case Title::Assistant: return "ассистент";
        case Title::Docent:    return "доцент";
        case Title::Professor: return "профессор";
        default:               return "";
    }
}

std::ostream& operator<<(std::ostream& os, const Teacher& t) {
    os << t.fio << ',' << t.faculty << ','
       << title_to_string(t.title) << ','
       << degree_to_string(t.degree);
    return os;
}

} // namespace pt
