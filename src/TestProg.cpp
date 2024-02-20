#include <cstdlib>
#include <string>
#include <fstream>

int main(int argc, char *argv[])
{

    std::ofstream fout("/home/scientist73/projects/ApplicationRegistration/log_TestProg.txt");
    int return_value;

    if (argc == 2)
    {
        const char* dir = getenv("DIR");

        if (dir != nullptr)
        {
            std::string full_path = std::string(dir) + "/" + std::string(argv[1]);
            std::string copy_full_path = std::string(dir) + "/copy_" + std::string(argv[1]);
            system(("cp " + full_path + " " + copy_full_path).c_str());

            return_value = 0;

            fout << "The program completed successfully";
        }
        else
        {
            fout << "DIR var is not set";
            return_value = 2;
        }

    }
    else
    {
        fout << "argc != 2\n";
        return_value = 1;
    }

    fout.close();
    return return_value;
}
