#include "ApplicationRegistration.h"

#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusConnectionInterface>
#include <QProcess>



//=============================================DBusAdaptor=============================================//
// DBusAdaptor class
// Parent class for all dbus interfaces
// notion: Classes derived from QDBusAbstractAdaptor must be created on the heap using the new operator
//      and must not be deleted by the user (they will be deleted automatically when the object they are
//      connected to is also deleted).

// public:
DBusAdaptor::DBusAdaptor(QObject* parent, const QString& DBusServiceName) :
    QDBusAbstractAdaptor(parent), parent(parent), DBusServiceName(&DBusServiceName), DBusObjectPath("")
{}

// Register dbus interface
// This func is virtual, so u can override in child class if needed
bool DBusAdaptor::registerInterface(const QString& DBusObjectPath)
{
    this->DBusObjectPath = DBusObjectPath;

    QDBusConnection dbusConnection = QDBusConnection::sessionBus();

    if(!dbusConnection.interface()->isServiceRegistered(*DBusServiceName))
    {
        dbusConnection.registerObject(DBusObjectPath, parent);
        dbusConnection.registerService(*DBusServiceName);
        qDebug() << "New service registered\n";
        return true;
    }
    else
    {
        qDebug() << "service is taken\n";
        return false;
    }

    return false;
}
//=============================================DBusAdaptor=============================================//


//======================================OrgFreedesktopApplication======================================//
// OrgFreedesktopApplication class
// Implementation of org.freedesktop.Application interface
//      (for more info: https://specifications.freedesktop.org/desktop-entry-spec/1.1/ar01s07.html)

// public:
OrgFreedesktopApplication::OrgFreedesktopApplication(QObject* parent, const QString& DBusServiceName, const QString& AppPath, const QVector<QMimeType>& SupportedMimeTypes, const QMimeDatabase& Mime_DB) :
    DBusAdaptor(parent, DBusServiceName), AppPath(&AppPath), SupportedMimeTypes(&SupportedMimeTypes), Mime_DB(&Mime_DB)
{}

// public slots:
//org.freedesktop.Application interface methods implementation
void OrgFreedesktopApplication::Activate(QVariantMap platform_data)
{
    QProcess proc;
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();

    for (QVariantMap::iterator it = platform_data.begin(); it != platform_data.end(); it++)
        env.insert(it.key().toLocal8Bit(), it.value().toByteArray());

    proc.setProgram(*AppPath);
    proc.setProcessEnvironment(env);
    proc.startDetached();
}
void OrgFreedesktopApplication::Open(QStringList uris, QVariantMap platform_data)
{
    try {
        is_SupportedMimeTypes(uris);
    } catch (std::invalid_argument& ex) {
        QDBusMessage::createError(QDBusError::InvalidArgs, ex.what());
        return;
    }

    QProcess proc;
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();

    for (QVariantMap::iterator it = platform_data.begin(); it != platform_data.end(); it++)
        env.insert(it.key().toLocal8Bit(), it.value().toByteArray());

    proc.setProgram(*AppPath);
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
bool OrgFreedesktopApplication::is_SupportedMimeTypes(QStringList MimeTypes)
{
    QString err_mes_not_valid_types = "The following types is not valid:\n";
    QString err_mes_unsupported_types = "The following types is not supported:\n";

    bool is_err_not_valid_types = false;
    bool is_err_unsupported_types = false;


    for (QString MimeType : MimeTypes)
    {
        QMimeType x = Mime_DB->mimeTypeForFile(MimeType.toLocal8Bit());

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


//=========================================DesktopApplication==========================================//
// DesktopApplication class
// Main class for application registration
// public:
DesktopApplication::DesktopApplication(int argc, char *argv[], const QString& DBusServiceName, const QString& AppPath):
    DBusServiceName(DBusServiceName), AppPath(AppPath)
{
    QCoreApplication* App = new QCoreApplication(argc, argv);
    this->App = App;
}
DesktopApplication::~DesktopApplication()
{
    delete App;
}

// Register interface ${BDusInterface} with object path ${DBusObjectPath}
bool DesktopApplication::registerDBusInterface(const std::string& BDusInterface, const std::string& DBusObjectPath)
{
    // notion: Classes derived from QDBusAbstractAdaptor must be created on the heap using the new operator
    //      and must not be deleted by the user (they will be deleted automatically when the object they are
    //      connected to is also deleted).

    if (BDusInterface == "org.freedesktop.Application")
    {
        OrgFreedesktopApplication* OrgFreedesktopApplication_interface
            = new OrgFreedesktopApplication(App, DBusServiceName, AppPath, SupportedMimeTypes, Mime_DB);

        if(OrgFreedesktopApplication_interface->registerInterface(QString::fromStdString(DBusObjectPath)))
        {
            dbus_interfaces[QString::fromStdString(BDusInterface)] = OrgFreedesktopApplication_interface;
            return true;
        }
        else
            return false;
    }
    else
        return false;
}

// Sets supported Mime types for application
void DesktopApplication::setSupportedMimeTypes(const std::vector<std::string>& SupportedMimeTypes)
{
    std::string err_mes = "The following types is not valid:\n";
    bool is_err = false;

    for(std::string sup_mime_type : SupportedMimeTypes)
    {
        QMimeType x = Mime_DB.mimeTypeForName(QString::fromStdString(sup_mime_type));
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

// Starts dbus service
bool DesktopApplication::runDBusService()
{
    return App->exec();
}

//=========================================DesktopApplication==========================================//


#include "ApplicationRegistration.moc"
