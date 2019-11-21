#ifndef APPSETTINGS_H
#define APPSETTINGS_H

#include <QObject>
#include <QSettings>
#include <QMetaEnum>

class AppSettings : public QObject
{
    Q_OBJECT

public:

    enum class SettingKeys {
        FastRender
    };
    Q_ENUM(SettingKeys)

    enum class SettingValues {
        Off,
        On
    };
    Q_ENUM(SettingValues)

    AppSettings() {
        load();
    }

    ~AppSettings() {
        flush();
    }

    Q_INVOKABLE static bool isEnabled(const SettingKeys key) {

        // Assumes cache and disk contents are the same
        if (m_cache.contains(key)) {
            return m_cache.value(key) == SettingValues::On;
        }

        return false;
    }

    Q_INVOKABLE static void setEnabled(const SettingKeys key, const SettingValues value) {

        // Sets the value to both in-memory structure and to disk
        const auto k = AppSettings::enumToString(key);
        const auto v = AppSettings::enumToString(value);

        QSettings s;
        s.setValue(k, v);
        m_cache[key] = value;
    }

private:

    // In-memory cache of settings with corresponding setting keys and values
    static QMap<SettingKeys, SettingValues> m_cache;

    template <class EnumClass>
    static QString enumToString(const EnumClass& key) {
        const auto metaEnum = QMetaEnum::fromType<EnumClass>();
        return metaEnum.valueToKey(static_cast<int>(key));
    }

    template <class EnumClass>
    static EnumClass stringToEnum(const QVariant& str) {
        const auto metaEnum = QMetaEnum::fromType<EnumClass>();
        const auto i = metaEnum.keyToValue(str.toString().toStdString().c_str());
        return static_cast<EnumClass>(i);
    }

    // Flushes cache to disk
    static void flush() {
        QSettings s;
        for (const auto& key: m_cache.keys()) {
            const auto k = AppSettings::enumToString<SettingKeys>(key);
            const auto v = AppSettings::enumToString<SettingValues>(m_cache.value(key));
            s.setValue(k, v);
        }
        s.sync();
    }

    // Loads values from disk to in-memory cache
    static void load() {
        QSettings s;
        for (const auto& key: s.allKeys()) {
            const auto k = AppSettings::stringToEnum<SettingKeys>(key);
            const auto v = AppSettings::stringToEnum<SettingValues>(s.value(key));
            m_cache[k] = v;
        }
    }
};


#endif // APPSETTINGS_H
