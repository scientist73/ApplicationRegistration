#include "ApplicationRegistration.h"
#include <string>


// argv[1] - AppName
// argv[2] - AppPath
// argv[3] - SupportedMimeTypesStr
// argv[4] - DBusServiceName
// argv[5] - OrgFreedesktopApplicationObjectPath

int main(int argc, char *argv[])
{
    if (argc == 6)
    {
        std::string SupportedMimeTypesStr = argv[3];

        std::string delimiter = ",";
        std::vector<std::string> SupportedMimeTypes;

        for(size_t pos = SupportedMimeTypesStr.find(delimiter); ; pos = SupportedMimeTypesStr.find(delimiter))
        {
            if (pos != std::string::npos)
            {
                SupportedMimeTypes.push_back(SupportedMimeTypesStr.substr(0, pos));
                SupportedMimeTypesStr.erase(0, pos + delimiter.length());
            }
            else
            {
                if (!SupportedMimeTypesStr.empty())
                {
                    SupportedMimeTypes.push_back(SupportedMimeTypesStr);
                    SupportedMimeTypesStr.clear();
                }

                break;
            }
        }

        DesktopApplication* App = new DesktopApplication (argv[2], argv[1]);
        App->setSupportedMimeTypes(SupportedMimeTypes);

        App->getDBusService()->registerService(argv[4]);
        App->getDBusService()->registerOrgFreedesktopApplicationInterface(argv[5]);


        App->getDBusService()->runService();


        delete App;
    }

    return 0;
}
