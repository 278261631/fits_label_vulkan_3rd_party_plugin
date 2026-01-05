#pragma once

#include "IPlugin.h"
#include "PluginContext.h"

class TestPlugin : public IPlugin {
public:
    TestPlugin();
    ~TestPlugin() override;
    
    bool init(PluginContext* context) override;
    void update(float deltaTime) override;
    void cleanup() override;
    
    const char* getName() const override;
    const char* getVersion() const override;
    
private:
    PluginContext* m_context;
    float m_counter;
};
