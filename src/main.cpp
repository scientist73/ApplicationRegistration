#include "ApplicationRegistration.h"


int main(int argc, char *argv[])
{
    DesktopApplication app(argc, argv, "ru.notepad", "code");
    app.setSupportedMimeTypes({"text/plain"});
    app.registerDBusInterface("org.freedesktop.Application", "/ru/notepad");
    return app.runDBusService();
}

