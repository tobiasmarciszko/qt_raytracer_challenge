#ifndef APPSETTINGS_H
#define APPSETTINGS_H

#include <QObject>
#include <QSettings>
#include <QMetaEnum>
#include <QDebug>

class AppSettings : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("RegisterEnumClassesUnscoped", "false")

public:

    enum class SettingKeys {
        Invalid = 0,
        FastRender,
        Fireworks,
        AntiAliasing,
        Fullscreen
    };
    Q_ENUM(SettingKeys)

    enum class SettingValues {
        Invalid = 0,
        Off,
        On
    };
    Q_ENUM(SettingValues)

    ~AppSettings();

    static AppSettings& get()
    {
        static AppSettings instance;
        return instance;
    }

    // Loads values from disk to in-memory cache
    void load();

    // Flushes cache to disk, "shouldn't" be necessary since all values are written to disk when changed
    void flush();

    // Generic access to settings
    bool isEnabled(const SettingKeys& key) const;
    void setEnabled(const SettingKeys& key, bool isEnabled);
    void setEnabled(const SettingKeys& key, const SettingValues& value);

    // Explicit access to setting properties
    // FastRender
    Q_PROPERTY(bool fastRenderEnabled READ fastRenderEnabled WRITE setFastRenderEnabled NOTIFY fastRenderEnabledChanged)
    bool fastRenderEnabled() const;
    void setFastRenderEnabled(bool isEnabled);

    // FastRender
    Q_PROPERTY(bool fireworksEnabled READ fireworksEnabled WRITE setFireworksEnabled NOTIFY fireworksEnabledChanged)
    bool fireworksEnabled() const;
    void setFireworksEnabled(bool isEnabled);

    // AntiAliasing
    Q_PROPERTY(bool aaEnabled READ aaEnabled WRITE setAaEnabled NOTIFY aaEnabledChanged)
    bool aaEnabled() const;
    void setAaEnabled(bool isEnabled);

    // Fullscreen perspective
    Q_PROPERTY(bool fullscreenEnabled READ fullscreenEnabled WRITE setFullscreenEnabled NOTIFY fullscreenEnabledChanged)
    bool fullscreenEnabled() const;
    void setFullscreenEnabled(bool isEnabled);


signals:
    void fastRenderEnabledChanged();
    void fireworksEnabledChanged();
    void aaEnabledChanged();
    void fullscreenEnabledChanged();

private:
    AppSettings();
    AppSettings(AppSettings const&) = delete;
    AppSettings(AppSettings&&) = delete;

    template <class EnumClass>
    QString enumToString(const EnumClass& key) {
        const auto metaEnum = QMetaEnum::fromType<EnumClass>();
        return metaEnum.valueToKey(static_cast<int>(key));
    }

    template <class EnumClass>
    EnumClass stringToEnum(const QVariant& str) {
        const auto metaEnum = QMetaEnum::fromType<EnumClass>();
        const auto i = metaEnum.keyToValue(str.toString().toUtf8());
        return static_cast<EnumClass>(i);
    }

private:
    // In-memory cache of settings with corresponding setting keys and values
    QMap<SettingKeys, SettingValues> m_cache;
    QSettings m_settings;
};

#endif // APPSETTINGS_H
