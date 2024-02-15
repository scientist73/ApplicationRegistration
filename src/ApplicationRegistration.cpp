#include "ApplicationRegistration.h"

#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusConnectionInterface>
#include <QProcess>

#include <thread>


//=============================================DBusAdaptor=============================================//
// DBusAdaptor class
// Parent class for all dbus interfaces
// notion: Classes derived from QDBusAbstractAdaptor must be created on the heap using the new operator
//      and must not be deleted by the user (they will be deleted automatically when the object they are
//      connected to is also deleted).

// public:
DBusInterfaceAdaptor::DBusInterfaceAdaptor(QObject* Parent, QDBusConnection* DBusConnection, const std::string* DBusServiceName) :
    QDBusAbstractAdaptor(Parent),
    Parent(Parent),
    DBusConnection(DBusConnection),
    ServiceName(DBusServiceName),
    ObjectPath(nullptr),
    IsInterfaceRegistered(false)
{

}

// Register dbus interface
// These funcs is virtual, so u can override in child class if needed
bool DBusInterfaceAdaptor::registerInterface(const std::string& DBusObjectPath)
{
    this->ObjectPath = new std::string(DBusObjectPath);

    if(DBusConnection->interface()->isServiceRegistered(QString::fromStdString(*ServiceName)))
    {
        DBusConnection->registerObject(QString::fromStdString(DBusObjectPath), Parent);
        IsInterfaceRegistered = true;
        qDebug() << "New service registered\n";
        return true;
    }
    else
    {
        IsInterfaceRegistered = false;
        qDebug() << "service is not exist\n";
        return false;
    }
}
bool DBusInterfaceAdaptor::isInterfaceRegistered()
{
    return IsInterfaceRegistered;
}
bool DBusInterfaceAdaptor::unregisterInterface()
{
    if (DBusConnection != nullptr && isInterfaceRegistered())
    {
        DBusConnection->unregisterObject(QString::fromStdString(*ObjectPath));
        IsInterfaceRegistered = false;
        return true;
    }
    else
        return false;

}

const std::string* DBusInterfaceAdaptor::getObjectPath() const
{
    return ObjectPath;
}
void DBusInterfaceAdaptor::setObjectPath(const std::string& DBusObjectPath)
{

}
//=============================================DBusAdaptor=============================================//


//======================================OrgFreedesktopApplication======================================//
// OrgFreedesktopApplication class
// Implementation of org.freedesktop.Application interface
//      (for more info: https://specifications.freedesktop.org/desktop-entry-spec/1.1/ar01s07.html)

// public:
OrgFreedesktopApplication::OrgFreedesktopApplication(QObject* Parent, QDBusConnection* DBusConnection, const std::string* DBusServiceName, const std::string* AppPath, const QVector<QMimeType>* SupportedMimeTypes, const QMimeDatabase* MimeDB) :
    DBusInterfaceAdaptor(Parent, DBusConnection, DBusServiceName), AppPath(AppPath), SupportedMimeTypes(SupportedMimeTypes), MimeDB(MimeDB)
{}

// public slots:
//org.freedesktop.Application interface methods implementation
void OrgFreedesktopApplication::Activate(QVariantMap platform_data)
{
    QProcess proc;
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();

    for (QVariantMap::iterator it = platform_data.begin(); it != platform_data.end(); it++)
        env.insert(it.key().toLocal8Bit(), it.value().toByteArray());

    proc.setProgram(QString::fromStdString(*AppPath));
    proc.setProcessEnvironment(env);
    proc.startDetached();
}
void OrgFreedesktopApplication::Open(QStringList uris, QVariantMap platform_data)
{
    try {
        IsSupportedMimeTypes(uris);
    } catch (std::invalid_argument& ex) {
        QDBusMessage::createError(QDBusError::InvalidArgs, ex.what());
        return;
    }

    QProcess proc;
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();

    for (QVariantMap::iterator it = platform_data.begin(); it != platform_data.end(); it++)
        env.insert(it.key().toLocal8Bit(), it.value().toByteArray());

    proc.setProgram(QString::fromStdString(*AppPath));
    proc.setArguments(uris);
    proc.setProcessEnvironment(env);
    proc.startDetached();
}
void OrgFreedesktopApplication::ActivateAction(QString action_name, QVariantList parameter, QVariantMap platform_data)
{
    //The ActivateAction method is called when Desktop Actions are activated.
    //The action-name parameter is the name of the action.
    //It's not implemented yet, because this method needs *.desktop file and defined actions
}

// private:
// Checks if MimeTypes is valid and supported
// If even one type is not valid or not supported throws std::invalid_argument
//      with explaining error message
// Otherwise returns true
bool OrgFreedesktopApplication::IsSupportedMimeTypes(QStringList MimeTypes)
{
    QString err_mes_not_valid_types = "The following types is not valid:\n";
    QString err_mes_unsupported_types = "The following types is not supported:\n";

    bool is_err_not_valid_types = false;
    bool is_err_unsupported_types = false;


    for (QString MimeType : MimeTypes)
    {
        QMimeType x = MimeDB->mimeTypeForFile(MimeType.toLocal8Bit());

        if (!x.isValid())
        {
            is_err_not_valid_types = true;
            err_mes_not_valid_types += '\t' + MimeType + '\n';
        }
        else if(!SupportedMimeTypes->contains(x))
        {
            is_err_unsupported_types = true;
            err_mes_unsupported_types += '\t' + MimeType + '\n';
        }
    }


    if (is_err_not_valid_types && !is_err_unsupported_types)
        throw std::invalid_argument(err_mes_not_valid_types.toStdString());
    else if (!is_err_not_valid_types && is_err_unsupported_types)
        throw std::invalid_argument(err_mes_unsupported_types.toStdString());
    else if (is_err_not_valid_types && is_err_unsupported_types)
        throw std::invalid_argument((err_mes_not_valid_types + '\n' + err_mes_unsupported_types).toStdString());
    else
        return true;
}
//======================================OrgFreedesktopApplication======================================//





//=========================================DBusServiceAdaptor==========================================//
// DBusServiceAdaptor class


DBusServiceAdaptor::DBusServiceAdaptor(const DesktopApplication* ParentClass):
    RunningDBusService(nullptr), DBusConnection(new QDBusConnection(QDBusConnection::sessionBus())), IsServiceRunning(false), ParentClass(ParentClass), DBusEventLoop(new QEventLoop())
{}
DBusServiceAdaptor::~DBusServiceAdaptor()
{
    if (isServiceRunning())
        stopService();

    unregisterAllRegisteredInterfaces();
    if (isServiceRegistered())
        unregisterService();
    if (DBusEventLoop != nullptr)
    {
        delete DBusEventLoop;
        DBusEventLoop = nullptr;
    }

    if (RunningDBusService != nullptr)
    {
        delete RunningDBusService;
        RunningDBusService = nullptr;
    }
}

bool DBusServiceAdaptor::registerService(const std::string& DBusServiceName)
{
    this->ServiceName = new std::string(DBusServiceName);

    if(!isServiceRegistered())
    {
        DBusConnection->registerService(QString::fromStdString(*ServiceName));
        return true;
    }
    else
        return false;
}
bool DBusServiceAdaptor::isServiceRegistered()
{
    return DBusConnection->interface()->isServiceRegistered(QString::fromStdString(*ServiceName));
}
bool DBusServiceAdaptor::unregisterService()
{
    if (isServiceRegistered())
    {
        DBusConnection->unregisterService(QString::fromStdString(*ServiceName));
        return true;
    }
    else
        return false;
}

bool DBusServiceAdaptor::registerOrgFreedesktopApplicationInterface(const std::string& DBusObjectPath)
{
    // notion: Classes derived from QDBusAbstractAdaptor must be created on the heap using the new operator
    //      and must not be deleted by the user (they will be deleted automatically when the object they are
    //      connected to is also deleted).

    OrgFreedesktopApplication* OrgFreedesktopApplication_interface
        = new OrgFreedesktopApplication(DBusEventLoop, DBusConnection, getServiceName(), ParentClass->getAppPath(), ParentClass->_getSupportedMimeTypes(), ParentClass->_getMimeDB());

    if(OrgFreedesktopApplication_interface->registerInterface(DBusObjectPath))
    {
        DBusInterfaces["OrgFreedesktopApplication"] = OrgFreedesktopApplication_interface;
        return true;
    }
    else
        return false;
}
bool DBusServiceAdaptor::isOrgFreedesktopApplicationInterfaceRegistered()
{
    return DBusInterfaces.contains("OrgFreedesktopApplication");
}
bool DBusServiceAdaptor::unregisterOrgFreedesktopApplicationInterface()
{
    auto it = DBusInterfaces.find("OrgFreedesktopApplication");
    if (it != DBusInterfaces.end())
    {
        DBusConnection->unregisterObject(QString::fromStdString(*it.value()->getObjectPath()));
        DBusInterfaces.erase(it);
        return true;
    }
    else
        return false;
}

bool DBusServiceAdaptor::unregisterAllRegisteredInterfaces()
{
    for (auto it : DBusInterfaces)
        DBusConnection->unregisterObject(QString::fromStdString(*it->getObjectPath()));

    DBusInterfaces.clear();
    return true;
}


bool DBusServiceAdaptor::runService()
{
    RunningDBusService = new std::thread([&]()
    {
        IsServiceRunning = true;
        DBusEventLoop->exec();
    });

    return true;
}
bool DBusServiceAdaptor::isServiceRunning()
{
    return IsServiceRunning;
}
bool DBusServiceAdaptor::stopService()
{
    DBusEventLoop->quit();
    RunningDBusService->join();

    IsServiceRunning = false;

    delete RunningDBusService;
    RunningDBusService = nullptr;

    return true;
}


const std::string* DBusServiceAdaptor::getServiceName() const
{
    return ServiceName;
}

void set_DBusServiceName(const std::string& DBusServiceName)
{

}


//=========================================DBusServiceAdaptor==========================================//










//=========================================DesktopApplication==========================================//
// DesktopApplication class
// Main class for application registration
// public:
DesktopApplication::DesktopApplication(const std::string& AppPath, const std::string& AppName):
    AppPath(new std::string(AppPath)),
    AppName(new std::string(AppName)),
    MimeDB(new QMimeDatabase()),
    DBusService(new DBusServiceAdaptor(this))
{}
DesktopApplication::~DesktopApplication()
{
    exit();
}




// Sets supported Mime types for application
void DesktopApplication::setSupportedMimeTypes(const std::vector<std::string>& SupportedMimeTypes)
{
    std::string err_mes = "The following types is not valid:\n";
    bool is_err = false;

    for(std::string sup_mime_type : SupportedMimeTypes)
    {
        QMimeType x = MimeDB->mimeTypeForName(QString::fromStdString(sup_mime_type));
        if (x.isValid())
            this->SupportedMimeTypes.push_back(x);
        else
        {
            err_mes += '\t' + sup_mime_type + '\n';
            is_err = true;
        }
    }

    if(is_err)
        throw std::invalid_argument(err_mes);
}


int DesktopApplication::exit()
{
    if (MimeDB != nullptr)
    {
        delete MimeDB;
        MimeDB = nullptr;
    }
    if (DBusService != nullptr)
    {
        if (DBusService->isServiceRunning())
            DBusService->stopService();
        if (DBusService->isOrgFreedesktopApplicationInterfaceRegistered())
            DBusService->unregisterOrgFreedesktopApplicationInterface();
        if (DBusService->isServiceRegistered())
            DBusService->unregisterService();

        delete DBusService;
        DBusService = nullptr;
    }

    return 0;
}

// Getters
DBusServiceAdaptor* DesktopApplication::getDBusService() const
{
    if (DBusService != nullptr)
        return DBusService;

    throw std::exception();
}
const std::string* DesktopApplication::getAppPath() const
{
    return AppPath;
}
const std::string* DesktopApplication::getAppName() const
{
    return AppName;
}

std::vector<std::string> DesktopApplication::getSupportedMimeTypes() const
{
    std::vector<std::string> SupportedMimeTypesStr;

    for (QMimeType SupportedMimeType : SupportedMimeTypes)
        SupportedMimeTypesStr.push_back(SupportedMimeType.name().toStdString());

    return SupportedMimeTypesStr;
}


// private:
const QVector<QMimeType>* DesktopApplication::_getSupportedMimeTypes() const
{
    return &SupportedMimeTypes;
}
const QMimeDatabase* DesktopApplication::_getMimeDB() const
{
    return MimeDB;
}


//=========================================DesktopApplication==========================================//


#include "ApplicationRegistration.moc"
