#include "AstroViewerPlugin.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <cmath>
#include <random>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

AstroViewerPlugin::AstroViewerPlugin() :
    m_context(nullptr),
    m_maxPoints(10000),
    m_brightnessThreshold(0.1f),
    m_zScale(0.05f),
    m_pointSize(5.0f),
    m_imageWidth(0),
    m_imageHeight(0),
    m_updateTimer(0.0f),
    m_loaded(false),
    m_submitted(false) {
}

AstroViewerPlugin::~AstroViewerPlugin() {
}

bool AstroViewerPlugin::init(PluginContext* context) {
    m_context = context;

    std::cout << "========================================" << std::endl;
    std::cout << "AstroViewerPlugin initialized!" << std::endl;
    std::cout << "This plugin loads image files and extracts" << std::endl;
    std::cout << "3D point data based on pixel brightness." << std::endl;
    std::cout << "========================================" << std::endl;

    // Try to load default image
    m_imagePath = "./test_image.jpg";

    std::ifstream testFile(m_imagePath);
    if (testFile.good()) {
        testFile.close();
        loadFromImage(m_imagePath);
    } else {
        std::cout << "[AstroViewer] No default image found at: " << m_imagePath << std::endl;
        std::cout << "[AstroViewer] Place an image file named 'test_image.jpg' in the executable directory." << std::endl;
    }

    return true;
}

bool AstroViewerPlugin::loadFromImage(const std::string& imagePath) {
    int width, height, channels;
    unsigned char* data = stbi_load(imagePath.c_str(), &width, &height, &channels, 3);

    if (!data) {
        std::cout << "[AstroViewer] Failed to load image: " << imagePath << std::endl;
        return false;
    }

    m_imageWidth = width;
    m_imageHeight = height;
    m_points.clear();

    int totalPixels = width * height;
    std::cout << "[AstroViewer] Image loaded: " << width << "x" << height << std::endl;
    std::cout << "[AstroViewer] Total pixels: " << totalPixels << std::endl;

    // Collect all bright pixels
    std::vector<PluginPointData> allPoints;

    // Scale factor to fit in view (normalize to reasonable range)
    float scaleX = 10.0f / width;
    float scaleY = 10.0f / height;
    float offsetX = -5.0f;
    float offsetY = -5.0f;

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int idx = (y * width + x) * 3;
            float r = data[idx] / 255.0f;
            float g = data[idx + 1] / 255.0f;
            float b = data[idx + 2] / 255.0f;

            // Calculate brightness (grayscale)
            float brightness = (r + g + b) / 3.0f;

            // Filter by brightness threshold
            if (brightness > m_brightnessThreshold) {
                PluginPointData point;
                // Scale and center the points
                point.x = x * scaleX + offsetX;
                point.y = (height - y) * scaleY + offsetY;  // Flip Y
                point.z = brightness * m_zScale * 255.0f;
                point.r = r;
                point.g = g;
                point.b = b;
                point.size = m_pointSize;
                allPoints.push_back(point);
            }
        }
    }

    stbi_image_free(data);

    std::cout << "[AstroViewer] Points after brightness filter: " << allPoints.size() << std::endl;

    // Random sampling if too many points
    if (m_maxPoints > 0 && static_cast<int>(allPoints.size()) > m_maxPoints) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::shuffle(allPoints.begin(), allPoints.end(), gen);
        allPoints.resize(m_maxPoints);
    }

    m_points = std::move(allPoints);
    m_loaded = true;
    m_submitted = false;

    std::cout << "[AstroViewer] Final points: " << m_points.size() << std::endl;

    return true;
}

void AstroViewerPlugin::submitPointsToRenderer() {
    if (!m_context || m_points.empty() || m_submitted) {
        return;
    }

    m_context->setPointCloudData(m_points);
    m_submitted = true;
    std::cout << "[AstroViewer] Submitted " << m_points.size() << " points to renderer" << std::endl;
}

void AstroViewerPlugin::printPointsInfo() {
    if (m_points.empty()) {
        std::cout << "[AstroViewer] No points loaded." << std::endl;
        return;
    }
    
    // Calculate statistics
    float minZ = m_points[0].z, maxZ = m_points[0].z;
    float avgZ = 0.0f;
    
    for (const auto& p : m_points) {
        minZ = std::min(minZ, p.z);
        maxZ = std::max(maxZ, p.z);
        avgZ += p.z;
    }
    avgZ /= m_points.size();
    
    std::cout << "[AstroViewer] Points: " << m_points.size() 
              << " | Z range: [" << minZ << ", " << maxZ << "]"
              << " | Avg Z: " << avgZ << std::endl;
}

void AstroViewerPlugin::update(float deltaTime) {
    // Submit points to renderer on first update after loading
    if (m_loaded && !m_submitted) {
        submitPointsToRenderer();
    }

    m_updateTimer += deltaTime;

    // Print info every 5 seconds
    if (m_updateTimer >= 5.0f) {
        if (m_loaded) {
            printPointsInfo();
        }
        m_updateTimer = 0.0f;
    }
}

void AstroViewerPlugin::cleanup() {
    std::cout << "AstroViewerPlugin cleaned up!" << std::endl;
    m_points.clear();
    m_context = nullptr;
}

const char* AstroViewerPlugin::getName() const {
    return "AstroViewerPlugin";
}

const char* AstroViewerPlugin::getVersion() const {
    return "1.0.0";
}

// Plugin export functions
extern "C" __declspec(dllexport) IPlugin* createPlugin() {
    return new AstroViewerPlugin();
}

extern "C" __declspec(dllexport) void destroyPlugin(IPlugin* plugin) {
    delete plugin;
}

