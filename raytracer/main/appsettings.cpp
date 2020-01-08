#include "appsettings.h"

AppSettings::AppSettings() {
    load();
}

AppSettings::~AppSettings() {
    flush();
}

// Settings

// FastRender
bool AppSettings::fastRenderEnabled() const {
    return isEnabled(SettingKeys::FastRender);
}

void AppSettings::setFastRenderEnabled(bool isEnabled) {
    setEnabled(SettingKeys::FastRender, isEnabled);
    emit fastRenderEnabledChanged();
}

// Fireworks
bool AppSettings::fireworksEnabled() const {
    return isEnabled(SettingKeys::Fireworks);
}

void AppSettings::setFireworksEnabled(bool isEnabled) {
    setEnabled(SettingKeys::Fireworks, isEnabled);
    emit fireworksEnabledChanged();
}

// AntiAliasing
bool AppSettings::aaEnabled() const {
    return isEnabled(SettingKeys::AntiAliasing);
}

void AppSettings::setAaEnabled(bool isEnabled) {
    setEnabled(SettingKeys::AntiAliasing, isEnabled);
    emit aaEnabledChanged();
}

// Fullscreen perspective
bool AppSettings::fullscreenEnabled() const {
    return isEnabled(SettingKeys::Fullscreen);
}

void AppSettings::setFullscreenEnabled(bool isEnabled) {
    setEnabled(SettingKeys::Fullscreen, isEnabled);
    emit fullscreenEnabledChanged();
}

/// Boilerplate methods

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

    for (auto i = m_cache.constBegin(); i != m_cache.constEnd(); ++i) {
        const auto k = enumToString<SettingKeys>(i.key());
        const auto v = enumToString<SettingValues>(m_cache.value(i.key()));
        m_settings.setValue(k, v);
    }

    m_settings.sync();
}

void AppSettings::load() {

    foreach (const auto& key, m_settings.allKeys()) {
        const auto k = stringToEnum<SettingKeys>(key);
        const auto v = stringToEnum<SettingValues>(m_settings.value(key));
        m_cache[k] = v;
    }
}
