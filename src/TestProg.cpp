#include <cstdlib>
#include <string>

int main(int argc, char *argv[])
{
    if (argc == 2)
    {
        const char* dir = getenv("DIR");

        if (dir != nullptr)
        {
            std::string full_path = std::string(dir) + "/" + std::string(argv[1]);
            system(("cat " + full_path).c_str());
        }
        else
            return 1;
    }
    else
        return 1;

    return 0;
}
