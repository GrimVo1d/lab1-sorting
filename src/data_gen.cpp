/// \file data_gen.cpp
/// \brief Утилита генерации тестовых CSV-наборов преподавателей
///
/// Использование: ./data_gen output.csv N
#include "csv.hpp"
#include "teacher.hpp"

#include <iostream>
#include <random>
#include <string>
#include <vector>

namespace {

const std::vector<std::string> SURNAMES = {
    "Иванов","Петров","Сидоров","Кузнецов","Смирнов","Соколов","Попов","Лебедев",
    "Козлов","Новиков","Морозов","Волков","Алексеев","Лебедева","Семенов","Егоров"
};
const std::vector<std::string> NAMES_INIT = {
    "А.А.","Б.В.","В.И.","Г.Н.","Д.С.","Е.П.","Ж.Р.","З.Т.","И.К.","К.М.","Л.О.","М.Ю."
};
const std::vector<std::string> FACULTIES = {
    "ФИТ","ФПМИ","ФРТ","ФЭТ","ФФ","ФМиЕН","ФМиМ","ИТМО","ФИВТ","ФУПМ"
};

} // namespace

int main(int argc, char** argv) {
    if (argc < 3) {
        std::cerr << "Использование: " << argv[0] << " <output.csv> <N>\n";
        return 1;
    }
    std::string path = argv[1];
    std::size_t n = static_cast<std::size_t>(std::stoull(argv[2]));

    std::mt19937 rng(42 + n);
    std::uniform_int_distribution<int> sur_d(0, (int)SURNAMES.size() - 1);
    std::uniform_int_distribution<int> nm_d(0,  (int)NAMES_INIT.size() - 1);
    std::uniform_int_distribution<int> fac_d(0, (int)FACULTIES.size() - 1);
    std::uniform_int_distribution<int> tit_d(0, 3);
    std::uniform_int_distribution<int> deg_d(0, 2);

    std::vector<pt::Teacher> v;
    v.reserve(n);
    for (std::size_t i = 0; i < n; ++i) {
        pt::Teacher t;
        t.fio = SURNAMES[sur_d(rng)] + " " + NAMES_INIT[nm_d(rng)];
        t.faculty = FACULTIES[fac_d(rng)];
        t.title  = static_cast<pt::Title>(tit_d(rng));
        t.degree = static_cast<pt::Degree>(deg_d(rng));
        v.push_back(std::move(t));
    }
    pt::write_csv(path, v);
    std::cerr << "Сгенерировано " << n << " записей в " << path << "\n";
    return 0;
}
