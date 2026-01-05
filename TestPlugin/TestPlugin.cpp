#include "TestPlugin.h"
#include <iostream>

TestPlugin::TestPlugin() : 
    m_context(nullptr),
    m_counter(0.0f) {
}

TestPlugin::~TestPlugin() {
}

bool TestPlugin::init(PluginContext* context) {
    m_context = context;
    
    std::cout << "TestPlugin initialized!" << std::endl;
    std::cout << "This is a test plugin for FitsLabelVulkan." << std::endl;
    
    return true;
}

void TestPlugin::update(float deltaTime) {
    m_counter += deltaTime;
    
    if (m_counter >= 2.0f) {
        std::cout << "[TestPlugin] Hello from test plugin!" << std::endl;
        m_counter = 0.0f;
    }
}

void TestPlugin::cleanup() {
    std::cout << "TestPlugin cleaned up!" << std::endl;
    m_context = nullptr;
}

const char* TestPlugin::getName() const {
    return "TestPlugin";
}

const char* TestPlugin::getVersion() const {
    return "1.0.0";
}

// 插件导出函数
extern "C" __declspec(dllexport) IPlugin* createPlugin() {
    return new TestPlugin();
}

extern "C" __declspec(dllexport) void destroyPlugin(IPlugin* plugin) {
    delete plugin;
}
