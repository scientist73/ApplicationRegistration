#ifndef APPLICATIONREGISTRATION_H
#define APPLICATIONREGISTRATION_H


#include <QtDBus/QDBusAbstractAdaptor>
#include <QCoreApplication>
#include <QMimeDatabase>
#include <QMimeType>
#include <QString>
#include <QVector>
#include <QStringList>
#include <QVariantList>
#include <QObject>

#include <string>
#include <vector>


class DBusAdaptor : public QDBusAbstractAdaptor
{
    Q_OBJECT
public:
    explicit DBusAdaptor(QObject* parent, const QString& DBusServiceName);

    virtual bool registerInterface(const QString& DBusObjectPath);

protected:
    const QString* DBusServiceName; // example, 'org.freedesktop.DBus'
    QString DBusObjectPath; // example, '/org/freedesktop/DBus'

    QObject* parent;
};

class OrgFreedesktopApplication : public DBusAdaptor
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
        QObject* parent,
        const QString& DBusServiceName,
        const QString& AppPath,
        const QVector<QMimeType>& SupportedMimeTypes,
        const QMimeDatabase& Mime_DB
        );

public slots:
    void Activate(QVariantMap platform_data);
    void Open(QStringList uris, QVariantMap platform_data);
    void ActivateAction(QString action_name, QVariantList parameter, QVariantMap platform_data);

private:
    bool is_SupportedMimeTypes(QStringList MimeTypes);


    const QString* AppPath; // the working directory to run the program in.

    const QVector<QMimeType>* SupportedMimeTypes;
    const QMimeDatabase* Mime_DB;
};

class DesktopApplication
{
public:
    DesktopApplication(int argc, char *argv[], const QString& DBusServiceName, const QString& AppPath);
    ~DesktopApplication();

    bool registerDBusInterface(const std::string& BDusInterface, const std::string& DBusObjectPath);
    void setSupportedMimeTypes(const std::vector<std::string>& SupportedMimeTypes);
    bool runDBusService();

private:
    QMap<const QString, DBusAdaptor*> dbus_interfaces;

    QString DBusServiceName;
    QString AppPath;

    QVector<QMimeType> SupportedMimeTypes;

    QMimeDatabase Mime_DB;
    QCoreApplication* App;
};


#endif // APPLICATIONREGISTRATION_H
