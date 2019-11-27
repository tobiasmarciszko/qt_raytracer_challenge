#include "appsettings.h"

AppSettings::AppSettings() {
    load();
}

AppSettings::~AppSettings() {
    flush();
}

bool AppSettings::fastRenderEnabled() const {
    return isEnabled(SettingKeys::FastRender);
}

void AppSettings::setFastRenderEnabled(bool isEnabled) {
    setEnabled(SettingKeys::FastRender, isEnabled);
    emit fastRenderEnabledChanged();
}

bool AppSettings::isEnabled(const SettingKeys& key) const {
    // Assumes cache and disk contents are the same
    if (m_cache.contains(key)) {
        return m_cache.value(key) == SettingValues::On;
    }

    return false;
}

void AppSettings::setEnabled(const SettingKeys& key, bool isEnabled) {
    setEnabled(key, isEnabled ? SettingValues::On : SettingValues::Off);
}

void AppSettings::setEnabled(const SettingKeys& key, const SettingValues& value) {
    // Sets the value to both in-memory structure and to disk
    const auto k = enumToString(key);
    const auto v = enumToString(value);

    m_settings.setValue(k, v);
    m_cache[key] = value;
}

void AppSettings::flush() {
    for (const auto& key: m_cache.keys()) {
        const auto k = enumToString<SettingKeys>(key);
        const auto v = enumToString<SettingValues>(m_cache.value(key));
        m_settings.setValue(k, v);
    }
    m_settings.sync();
}

void AppSettings::load() {
    for (const auto& key: m_settings.allKeys()) {
        const auto k = stringToEnum<SettingKeys>(key);
        const auto v = stringToEnum<SettingValues>(m_settings.value(key));
        m_cache[k] = v;
    }
}
