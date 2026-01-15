#pragma once

#include "IPlugin.h"
#include "PluginContext.h"
#include <vector>
#include <string>

struct PointData {
    float x, y, z;
    float r, g, b;
};

class AstroViewerPlugin : public IPlugin {
public:
    AstroViewerPlugin();
    ~AstroViewerPlugin() override;
    
    bool init(PluginContext* context) override;
    void update(float deltaTime) override;
    void cleanup() override;
    
    const char* getName() const override;
    const char* getVersion() const override;
    
private:
    bool loadFromImage(const std::string& imagePath);
    void printPointsInfo();
    
    PluginContext* m_context;
    std::vector<PointData> m_points;
    
    // Configuration
    std::string m_imagePath;
    int m_maxPoints;
    float m_brightnessThreshold;
    float m_zScale;
    
    // Image info
    int m_imageWidth;
    int m_imageHeight;
    
    // Update timer
    float m_updateTimer;
    bool m_loaded;
};

