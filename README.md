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


## Dependencies
List of external dependencies:
- qt6-base-dev (>= 6.2.4) (already in the deb package)
- libdbus-1-3
- libglib2.0-dev    


## Installation
There are two ways to get ApplicationRegistration: 
1. download .deb file and install it
1. clone this repo and build it with cmake

Let's go through both ways.
### .deb file
```
apt update
apt install wget
wget https://github.com/scientist73/ApplicationRegistration/releases/download/v1.0.0/ApplicationRegistration-1.0.0.0-linux.deb
apt install libdbus-1-3
apt install libglib2.0-dev
apt install qt6-base-dev
apt install dbus-x11
dpkg -i ApplicationRegistration-1.0.0.0-linux.deb
```
### cmake 
```
apt update
apt install git
git clone https://github.com/scientist73/ApplicationRegistration.git
cd ApplicationRegistration
apt install libdbus-1-3
apt install libglib2.0-dev
apt install qt6-base-dev
apt install dbus-x11
apt install cmake
apt install clang
cmake -S . -B build
cd build
cmake --build .
cmake --install .
```
### with script
```
apt update
apt install git
git clone https://github.com/scientist73/ApplicationRegistration.git
cd ApplicationRegistration
./BuildScript.sh
```
## Testing
To test the framework, run the following commands
```
export $(dbus-launch)
./DBusService test TestProg text/plain ru.notepad /ru/notepad &
gdbus call --session \
    --dest ru.notepad \
    --object-path /ru/notepad \
    --method org.freedesktop.Application.Open \
    "['text.txt']" "{'DIR':<'/ApplicationRegistration'>}"
```
This test program copies the file *file.** with a supported extension 
from the *DIR* directory and names it *copy_file.**.

Now there is a file in dir /ApplicationRegistration named
*copy_text.txt*.

## Implementation details
*This section is old*

This framework consists of several classes, each of which does
some specific task:
- **DBusInterfaceAdaptor** - parent class for all DBus interfaces,
it registers DBus interface. 
- **${DBusInterfaceName}** - child class of **DBusAdaptor**, it 
reprecent dbus interface. To implement dbus interface needs
add Q_CLASSINFO and define correcpond function. An example would
be class **OrgFreedesktopApplication**.
- **DBusServiceAdaptor** - main dbus class that manages all interfaces and represents a service
- **DesktopApplication**

This implementation allows easily add new dbus interfaces if needed.


## Known issues
No known issues yet.

## Future development
Some possible future development:
1. creating and modifying .desktop file
1. implementation of **ActivateAction** func in 
**org.freedesktop.Application** interface

