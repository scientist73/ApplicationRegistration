# ApplicationRegistration

## Detailed description
ApplicationRegistration is a framework for registration custom 
application in linux OS. It provides the following functionality:
- register DBus service (see **DBus service** for the list of 
all available interfaces)
- creating .desktop file (see  **Desktop file**)
- some other features


## Functionality:

DesktopApplication is a main class for this framework, it uses other 
classes for app registration.
#### Member functions:
- **(constructor)** - constructs new object. Parameters:
    - *argc, argv[]* - default command line args
    - *DBusServiceName* - the name of the dbus service (example, "org.freedesktop.DBus")
    - *AppPath* - path to the application (in the file system)
- **(destructor)**
- **runDBusService** - registers dbus interface named *${BDusInterface}* 
with object path *${DBusObjectPath}*. For list off all available 
dbus interfaces see **DBusService** section.
- **setSupportedMimeTypes** - sets application supported file
formats, specified as a vector of string mime types (example,
"text/plain" instead of txt)
- **runDBusService** - runs dbus service. This func starts inf
loop, so it must be invoke last. Returns exit code if service is
terminated.
### DBus Service
For registration dbus interfaces used **runDBusService** func, which
can only registrate interface that already implemented in this
framework. 

Names of all available DBus interfaces (for *${BDusInterface}* in **runDBusService** func):
- **"org.freedesktop.Application"** - (for more info: https://specifications.freedesktop.org/desktop-entry-spec/1.1/ar01s07.html)

### Desktop file
This section is not ready yet

## Usage example
```
#include "ApplicationRegistration.h"


int main(int argc, char *argv[])
{
    DesktopApplication app(argc, argv, "ru.notepad", "/home/user/notepad");
    app.setSupportedMimeTypes({"text/plain"});
    app.registerDBusInterface("org.freedesktop.Application", "/ru/notepad");
    return app.runDBusService();
}
```

## Dependencies
List of external dependencies:
- Qt framework

## Installation
There are two ways to get ApplicationRegistration: 
1. download .deb file and install it
1. clone this repo and build it with cmake

Let's go through both ways.
### .deb file
### cmake 
This section is not ready yet

## Implementation details
This framework consists of several classes, each of which does
some specific task:
- **DBusAdaptor** - parent class for all DBus interfaces,
it registers DBus interface. 
- **${DBusInterfaceName}** - child class of **DBusAdaptor**, it 
reprecent dbus interface. To implement dbus interface needs
add Q_CLASSINFO and define correcpond function. An example would
be class **OrgFreedesktopApplication**.
- **DesktopApplication**

This implementation allows easily add new dbus interfaces if needed.


## Known issues
No known issues yet.

## Future development
Some possible future development:
1. creating and modifying .desktop file
1. implementation of **ActivateAction** func in 
**org.freedesktop.Application** interface



### TODO:
+ убедиться в том, что нет утечки памяти
- написать README.md:
    + общие хар-ки (масштабируемый, ...)
    + описать функционал
    + описать зависимости
    + описать баги и проблемы
    + детали реализации
    + дать пример использования
    + указать возможные пути развития фрейморка
    - описать процесс установки deb
- git:
    + добавить .gitignore
    - загрузить на github
+ создать deb файл (установочный)
- docker тестирование
+ добавить комментарии в CMakeLists.txt
- сделать релиз с deb файлом

- ? реализовать ActivateAction
