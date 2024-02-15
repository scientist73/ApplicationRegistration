#include "ApplicationRegistration.h"
#include <vector>
#include <iostream>
#include <string>



size_t menu()
{
    std::cout << "Choose option:\n";
    std::cout << "\t1. Registrate application on the dbus\n";
    std::cout << "\t2. Show application info\n";
    std::cout << "\t3. Change application info\n";
    std::cout << "\t4. Remove application from the dbus\n";
    std::cout << "\t5. Exit\n";

    size_t choise;
    std::cin >> choise;
    std::cin.ignore();

    //return choise;
    return 1;
}

void init(DesktopApplication*& app)
{
    if (app != nullptr)
    {
        delete app;
        app = nullptr;
    }


    std::string AppPath, AppName, SupportedMimeTypesStr;

    std::cout << "Welcome to demo program for ApplicationRegistation framework\n";
    std::cout << "===================================================================\n";

    std::cout << "Before we start lets fill in the basic information about the application.\n\n";

    std::cout << "Enter application name (used to refer to the app): ";
    //std::getline(std::cin, AppName);
    AppName = "test";

    std::cout << "Enter full path to the executable: ";
    //std::getline(std::cin, AppPath);
    AppPath = "code";

    std::cout << "Enter supported mime types for the application, separated by commas\n";
    std::cout << "(for example, \"text/plain, ...\") ";
    //std::getline(std::cin, SupportedMimeTypesStr);
    SupportedMimeTypesStr = "text/plain";

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



    app = new DesktopApplication (AppPath, AppName);
    app->setSupportedMimeTypes(SupportedMimeTypes);
}



int main(int argc, char *argv[])
{

    DesktopApplication* App = nullptr;

    init(App);

    while(true)
    {
        switch(menu())
        {
        case 1: // Registrate application on the dbus
        {
            std::string DBusServiceName, DBusObjectPath;

            std::cout << "Enter dbus service name: ";
            //std::getline(std::cin, DBusServiceName);
            DBusServiceName = "ru.notepad";

            std::cout << "Enter dbus object path: ";
            //std::getline(std::cin, DBusObjectPath);
            DBusObjectPath = "/ru/notepad";

            App->getDBusService()->registerService(DBusServiceName);
            App->getDBusService()->registerOrgFreedesktopApplicationInterface(DBusObjectPath);

            App->getDBusService()->runService();
        }
            break;

        case 2: // Show application info
        {
            if (App != nullptr)
            {
                std::cout << "Main application info:\n";

                std::cout << "\tAppName: " << *App->getAppName() << std::endl;
                std::cout << "\tAppPath: " << *App->getAppPath() << std::endl;
                std::cout << "\tSupportedMimeTypes: ";

                std::vector<std::string> SupportedMimeTypesStr = App->getSupportedMimeTypes();
                if (SupportedMimeTypesStr.empty())
                    std::cout << "-" << std::endl;
                else
                {
                    for(size_t i = 0; i < SupportedMimeTypesStr.size() - 1; ++i)
                        std::cout << SupportedMimeTypesStr[i] << ", ";
                    std::cout << SupportedMimeTypesStr.back() << std::endl;
                }


                if (App->getDBusService()->isServiceRunning())
                {
                    std::cout << "\tDBusServiceName: " << App->getDBusService()->getServiceName() << std::endl;
                }
            }


        }
            break;
        case 3: // Change application info
        {

        }
            break;
        case 4: // Remove application from the dbus
        {
            std::cout << "Are you sure you want to remove application from the dbus (y/n, yes/no) ? ";
            std::string ans;
            std::getline(std::cin, ans);

            if (ans == "y" || ans == "yes")
            {
                if (App->getDBusService()->isServiceRunning())
                {
                    App->getDBusService()->stopService();
                    if (App->getDBusService()->isOrgFreedesktopApplicationInterfaceRegistered())
                        App->getDBusService()->unregisterOrgFreedesktopApplicationInterface();
                    if (App->getDBusService()->isServiceRegistered())
                        App->getDBusService()->unregisterService();
                }
                else
                    std::cout << "Application wasn't registered on the dbus.\n";

            }
            else if (ans == "n" || ans == "no")
                std::cout << "Return to main menu.\n";
            else
                std::cout << "Unknown answer. Return to main menu.\n";

        }
            break;
        case 5: // Exit
        {
            std::cout << "Are you sure you want to exit? Exiting will also remove application from the dbus (y/n, yes/no) ";
            std::string ans;
            std::getline(std::cin, ans);

            if (ans == "y" || ans == "yes")
            {
                std::cout << "Exiting the program...\n";
                return App->exit();
            }
            else if (ans == "n" || ans == "no")
                std::cout << "Return to main menu.\n";
            else
                std::cout << "Unknown answer. Return to main menu.\n";



        }
            break;
        default:
            std::cout << "Unknown answer. Please, enter again.\n";
        }
        std::cout << std::endl;
    }


    return 0;
}

