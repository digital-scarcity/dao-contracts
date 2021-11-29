#pragma once

#include <document_graph/document.hpp>
#include <logger/logger.hpp>
#include <eosio/eosio.hpp>

namespace hypha {

class dao;

using eosio::checksum256;

/**
 * @brief 
 * Stores a setting document
 */
class Settings : public Document
{
public:
    Settings(dao& dao, 
            const checksum256& hash, 
            const checksum256& rootHash);
    void setSetting(const Content& setting);
    void remSetting(const string& key);
    //const Content* getSetting(const string& key);
    inline const checksum256& getRootHash() 
    {
    return m_rootHash;
    }

    template<class T>
    std::optional<T> getSettingOpt(const string& key)
    {
        TRACE_FUNCTION()
        auto [idx, content] = getContentWrapper().get(SETTINGS_IDX, key);
        if (auto p = std::get_if<T>(&content->value))
        {
            return *p;
        }

        return {};
    }

    template<class T>
    const T& getOrFail(const string& key)
    {
        TRACE_FUNCTION()
        auto [_, content] = getContentWrapper().getOrFail(SETTINGS_IDX, key, "setting " + key + " does not exist");
        return std::get<T>(content->value);
    }

    template <class T>
    T getSettingOrDefault(const string &setting, const T &def = T{})
    {
        if (auto content = getSettingOpt<T>(setting))
        {
            return *content;
        }

        return def;
    }

    //Default index for settings group
    static constexpr int64_t SETTINGS_IDX = 0;
private:
    //bool m_dirty;
    checksum256 m_rootHash;
    dao* m_dao;
};

}