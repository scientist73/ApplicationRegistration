#ifndef APPLICATIONREGISTRATION_H
#define APPLICATIONREGISTRATION_H


#include <QtDBus/QDBusAbstractAdaptor>
#include <QtDBus/QDBusConnection>
#include <QCoreApplication>
#include <QMimeDatabase>
#include <QMimeType>
#include <QString>
#include <QVector>
#include <QStringList>
#include <QVariantList>
#include <QEventLoop>
#include <QObject>

//#include <thread>
#include <string>
#include <vector>


class DBusInterfaceAdaptor : public QDBusAbstractAdaptor
{
    Q_OBJECT
public:
    explicit DBusInterfaceAdaptor(QObject* Parent, QDBusConnection* DBusConnection, const std::string* DBusServiceName);

    virtual bool registerInterface(const std::string& DBusObjectPath);
    virtual bool isInterfaceRegisteводчикred();
    virtual bool unregisterInterface();

    virtual const std::string* getObjectPath() const;

    virtual void setObjectPath(const std::string& DBusObjectPath);

protected:
    const std::string* ServiceName; // example, 'org.freedesktop.DBus'
    std::string* ObjectPath; // example, '/org/freedesktop/DBus'
    bool IsInterfaceRegistered;

    QDBusConnection* DBusConnection;

    QObject* Parent;
};

class OrgFreedesktopApplication : public DBusInterfaceAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.freedesktop.Application")
    Q_CLASSINFO("D-Bus Introspection", ""
       "<interface name='org.freedesktop.Application'\n>"
       "    <method name='Activate'>\n"
       "        <arg type='a{sv}' name='platform_data' direction='in'/>\n"
       "    </method>\n"
       "    <method name='Open'>"
       "        <arg type='as' name='uris' direction='in'/>\n"
       "        <arg type='a{sv}' name='platform_data' direction='in'/>\n"
       "    </method>\n"
       "    <method name='ActivateAction'>\n"
       "        <arg type='s' name='action_name' direction='in'/>\n"
       "        <arg type='av' name='parameter' direction='in'/>\n"
       "        <arg type='a{sv}' name='platform_data' direction='in'/>\n"
       "    </method>\n"
       "</interface>\n"
       "")

public:
    explicit OrgFreedesktopApplication(
        QObject* Parent,
        QDBusConnection* DBusConnection,
        const std::string* DBusServiceName,
        const std::string* AppPath,
        const QVector<QMimeType>* SupportedMimeTypes,
        const QMimeDatabase* MimeDB
        );

public slots:
    QString Activate(QVariantMap platform_data);
    QString Open(QStringList uris, QVariantMap platform_data);
    void ActivateAction(QString action_name, QVariantList parameter, QVariantMap platform_data);

private:
    bool IsSupportedMimeTypes(QStringList MimeTypes);


    const std::string* AppPath; // the working directory to run the program in.

    const QVector<QMimeType>* SupportedMimeTypes;
    const QMimeDatabase* MimeDB;
};

class DesktopApplication;


class DBusServiceAdaptor : public QObject
{
    Q_OBJECT
public:
    explicit DBusServiceAdaptor(const DesktopApplication* ParentClass);
    ~DBusServiceAdaptor();

    bool registerService(const std::string& DBusServiceName);
    bool isServiceRegistered();
    bool unregisterService();

    // OrgFreedesktopApplication
    bool registerOrgFreedesktopApplicationInterface(const std::string& DBusObjectPath);
    bool isOrgFreedesktopApplicationInterfaceRegistered();
    bool unregisterOrgFreedesktopApplicationInterface();

    bool unregisterAllRegisteredInterfaces();

    bool runService();
    bool isServiceRunning();
    bool stopService();


    const std::string* getServiceName() const;

    void setServiceName(const std::string& DBusServiceName);

public slots:
    void exit();
private:
    std::string* ServiceName;
    bool IsServiceRunning;

    QMap<const std::string, DBusInterfaceAdaptor*> DBusInterfaces;

    //std::thread* RunningDBusService;
    QDBusConnection* DBusConnection;
    QCoreApplication* DBusApp;

    const DesktopApplication* ParentClass;
};



class DesktopApplication
{
    friend bool DBusServiceAdaptor::registerOrgFreedesktopApplicationInterface(const std::string&);
public:
    DesktopApplication(const std::string& AppPath, const std::string& AppName);
    ~DesktopApplication();

    DBusServiceAdaptor* getDBusService() const;
    const std::string* getAppPath() const;
    const std::string* getAppName() const;
    std::vector<std::string> getSupportedMimeTypes() const;

    void setAppPath(const std::string& AppPath);
    void setAppName(const std::string& AppName);
    void setSupportedMimeTypes(const std::vector<std::string>& SupportedMimeTypes);

private:
    const QVector<QMimeType>* _getSupportedMimeTypes() const;
    const QMimeDatabase* _getMimeDB() const;


    std::string* AppPath;
    std::string* AppName;

    QVector<QMimeType> SupportedMimeTypes;

    QMimeDatabase* MimeDB;
    DBusServiceAdaptor* DBusService;
};


#endif // APPLICATIONREGISTRATION_H
