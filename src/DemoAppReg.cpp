#include "ApplicationRegistration.h"
#include <QProcess>
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

    return choise;
}




int main(int argc, char *argv[])
{
    std::string AppPath, AppName, SupportedMimeTypesStr;
    QProcess* RunningDBusService;
    qint64* RunningDBusServicePID;

    std::cout << "Welcome to demo program for ApplicationRegistation framework\n";
    std::cout << "===================================================================\n";

    std::cout << "Before we start lets fill in the basic information about the application.\n\n";

    std::cout << "Enter application name (used to refer to the app): ";
    std::getline(std::cin, AppName);

    std::cout << "Enter full path to the executable: ";
    std::getline(std::cin, AppPath);

    std::cout << "Enter supported mime types for the application, separated by commas\n";
    std::cout << "(for example, \"text/plain, ...\") ";
    std::getline(std::cin, SupportedMimeTypesStr);

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



    DesktopApplication* App = new DesktopApplication (AppPath, AppName);
    App->setSupportedMimeTypes(SupportedMimeTypes);

    while(true)
    {
        switch(menu())
        {
        case 1: // Registrate application on the dbus
        {
            std::string DBusServiceName, DBusObjectPath;

            std::cout << "Enter dbus service name: ";
            std::getline(std::cin, DBusServiceName);
            //DBusServiceName = "ru.notepad";

            std::cout << "Enter dbus object path: ";
            std::getline(std::cin, DBusObjectPath);
            //DBusObjectPath = "/ru/notepad";



            RunningDBusService = new QProcess;
            RunningDBusService->setProgram("/home/scientist73/projects/ApplicationRegistration/build/DBusService");
            RunningDBusService->setArguments(   {
                                                QString::fromStdString(AppName),
                                                QString::fromStdString(AppPath),
                                                QString::fromStdString(SupportedMimeTypesStr),
                                                QString::fromStdString(DBusServiceName),
                                                QString::fromStdString(DBusObjectPath)
                                                }
                                             );
            RunningDBusService->start();

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
                if (RunningDBusService != nullptr)
                {
                    RunningDBusService->terminate();
                    delete RunningDBusService;
                    RunningDBusService = nullptr;
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
                delete App;
                return 0;
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

