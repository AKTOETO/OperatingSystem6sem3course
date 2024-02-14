#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <filesystem>
#include <list>

using namespace std::filesystem;

std::string GetFileInfo(const std::string& filepath, size_t size)
{
    std::ifstream fin(filepath);
    char* out = new char[size];
    fin.read(out, size);
    std::string strout(out, size);
    delete[] out;
    return strout;
}

std::string GetSubString(std::istream& in, size_t size)
{
    char* out = new char[size];
    in.read(out, size);
    std::string str(out, size);
    delete[] out;
    return str;
}

int Archivate(int argc, char** argv)
{
    // если нет дополнительных аргументов
    // выдаем ошибку
    if(argc < 3)
    {
        std::cerr << "Мало аргументов\n";
        return 1;
    }

    // сохраняем путь к директории, которую надо заархивировать
    std::string arch_str(argv[1]);
    std::cout << "Директория для архивации: " << arch_str << "\n";

    // сохраняем путь к директории, в котороую нужно
    // положить заархивированный файл
    std::string dearch_str(argv[2]);
    std::cout << "Директория после архивации: " << dearch_str << "\n";

    // проверяем, существует ли такая директория
    if(!exists(arch_str))
    {
        std::cerr << "Такая директория не существует\n";
        return 2;
    }

    // проверяем директория ли это
    if(!is_directory(arch_str))
    {
        std::cerr<<"Это не директория\n";
        return 3;
    }

    // количество файлов
    size_t num_of_file = 0;

    // получаем список файлов в директории
    for (auto& entry : recursive_directory_iterator(arch_str)) 
    {
        // если это файл
        if (!is_directory(entry.path()))
        {
            // печатаем в консоль путь к файлу и его размер
            std::cout << entry.path() << 
            "\t" << entry.file_size() << "\n";
            num_of_file++;
        }
    }

    // создаем выходной файл для архивации
    std::ofstream fout(dearch_str, std::ios::binary);

    // выводим количество файлов
    fout << num_of_file << "\n";

    // вся информация из файлов
    std::string all_data;
    // весь размер информации файлов
    size_t all_size = 0;

    // выводим информацию по файлам
    for (auto& entry : recursive_directory_iterator(arch_str)) 
    {
        // если это файл
        if (!is_directory(entry.path()))
        {
            // печатаем в файл путь к файлу и его размер
            fout << entry.path() << "\n"
                 << entry.file_size() << "\n";
            all_data += GetFileInfo(entry.path().string(), entry.file_size());
            all_size += entry.file_size();
        }
    }

    // выводим информацию из файлов
    fout.write(all_data.c_str(), all_size);
    fout.close();

    return 0;
}

struct file
{
    std::string name;
    size_t size;
};


int main(int argc, char** argv)
{
    //Archivate(argc, argv);

    // если нет дополнительных аргументов
    // выдаем ошибку
    if(argc < 2)
    {
        std::cerr << "Мало аргументов\n";
        return 1;
    }

    // сохраняем путь к файлу-архиву
    std::string arch_str(argv[1]);
    std::cout << "Путь к архиву: " << arch_str << "\n";

    // проверяем, существует ли такая директория
    if(!exists(arch_str))
    {
        std::cerr << "Такая директория не существует\n";
        return 2;
    }

    // проверяем директория ли это
    if(is_directory(arch_str))
    {
        std::cerr<<"Это директория, а должен быть файл\n";
        return 3;
    }

    // открываем файл
    std::ifstream fin(arch_str, std::ios::binary);

    // количество файлов
    size_t num_of_file = 0;

    // считываем количество файлов
    fin >> num_of_file;
    fin.ignore(1000000,'\n');
    std::cout<<"Количество файлов: "<<num_of_file << "\n";

    // создаем список файлов
    std::list<file> files;
    for(size_t i = 0; i < num_of_file; i++)
    {
        //fin.ignore('\n', 1);
        std::string temp;
        file f;
        getline(fin, f.name);
        fin >> f.size;
        files.emplace_back(f);
        fin.ignore(1000000,'\n');
        std::cout<<"\tфайл: \""<<f.name<<"\" "<<f.size<<"\n";
    }

    // печать в файлы
    for(auto& el : files)
    {
        std::ofstream fout(el.name, std::ios::binary);
        if(!fout.is_open())
        {
            std::cout<<"Файл не был открыт " << el.name <<"\n";
            return 1;
        }
        fout.write(GetSubString(fin, el.size).c_str(), el.size);
        fout.close();
    }


    return 0;
}
