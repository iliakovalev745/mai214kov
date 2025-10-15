#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <filesystem>
#include <random>
#include <algorithm>
#include <cmath>

namespace fs = std::filesystem;

class PGMImage {
private:
    int width, height, maxVal;
    std::vector<std::vector<int>> pixels;

public:
    PGMImage() : width(0), height(0), maxVal(255) {}
    
    bool load(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Cannot open file: " << filename << std::endl;
            return false;
        }
        
        std::string magicNumber;
        file >> magicNumber;
        if (magicNumber != "P2") {
            std::cerr << "Unsupported PGM format: " << magicNumber << std::endl;
            return false;
        }
        
        file >> width >> height >> maxVal;
        
        pixels.resize(height, std::vector<int>(width));
        
        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j) {
                if (!(file >> pixels[i][j])) {
                    return false;
                }
                pixels[i][j] = std::max(0, std::min(255, pixels[i][j]));
            }
        }
        
        file.close();
        std::cout << "Loaded: " << filename << " (" << width << "x" << height << ")" << std::endl;
        return true;
    }
    
    bool save(const std::string& filename) {
        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Cannot create file: " << filename << std::endl;
            return false;
        }
        
        file << "P2\n" << width << " " << height << "\n" << maxVal << "\n";
        
        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j) {
                file << pixels[i][j];
                if (j < width - 1) file << " ";
            }
            file << "\n";
        }
        
        file.close();
        return true;
    }
    
    void addNoise(double noiseLevel) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis(0.0, 1.0);
        
        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j) {
                if (dis(gen) < noiseLevel) {
                    pixels[i][j] = (dis(gen) < 0.5) ? 0 : maxVal;
                }
            }
        }
    }
    
    void applyMedianFilter(int kernelSize = 3) {
        if (kernelSize % 2 == 0) return;
        
        std::vector<std::vector<int>> filteredPixels = pixels;
        int offset = kernelSize / 2;
        
        for (int i = offset; i < height - offset; ++i) {
            for (int j = offset; j < width - offset; ++j) {
                std::vector<int> window;
                
                for (int ki = -offset; ki <= offset; ++ki) {
                    for (int kj = -offset; kj <= offset; ++kj) {
                        window.push_back(pixels[i + ki][j + kj]);
                    }
                }
                
                std::sort(window.begin(), window.end());
                filteredPixels[i][j] = window[window.size() / 2];
            }
        }
        
        pixels = filteredPixels;
    }
    
    void applyGaussianFilter(int kernelSize = 3) {
        if (kernelSize % 2 == 0) return;
        
        std::vector<std::vector<int>> filteredPixels = pixels;
        int offset = kernelSize / 2;
        
        std::vector<std::vector<double>> kernel = {
            {1, 2, 1},
            {2, 4, 2},
            {1, 2, 1}
        };
        
        double sum = 16.0;
        
        for (int i = offset; i < height - offset; ++i) {
            for (int j = offset; j < width - offset; ++j) {
                double value = 0.0;
                
                for (int ki = -offset; ki <= offset; ++ki) {
                    for (int kj = -offset; kj <= offset; ++kj) {
                        value += pixels[i + ki][j + kj] * kernel[ki + offset][kj + offset];
                    }
                }
                
                filteredPixels[i][j] = std::max(0, std::min(255, static_cast<int>(value / sum)));
            }
        }
        
        pixels = filteredPixels;
    }
    
    void createTestImage(int w, int h) {
        width = w;
        height = h;
        maxVal = 255;
        pixels.resize(height, std::vector<int>(width, 128));
        
        for (int i = h/4; i < h*3/4; ++i) {
            for (int j = w/4; j < w*3/4; ++j) {
                pixels[i][j] = 200;
            }
        }
    }
    
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    int getPixel(int x, int y) const { 
        if (x >= 0 && x < width && y >= 0 && y < height) {
            return pixels[y][x];
        }
        return 0;
    }
    void setPixel(int x, int y, int value) { 
        if (x >= 0 && x < width && y >= 0 && y < height) {
            pixels[y][x] = std::max(0, std::min(255, value));
        }
    }
    bool isValid() const { return width > 0 && height > 0 && !pixels.empty(); }
};

double calculateMSE(const PGMImage& img1, const PGMImage& img2) {
    if (!img1.isValid() || !img2.isValid()) return -1.0;
    if (img1.getWidth() != img2.getWidth() || img1.getHeight() != img2.getHeight()) return -1.0;
    
    double mse = 0.0;
    int width = img1.getWidth();
    int height = img1.getHeight();
    int totalPixels = width * height;
    
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            double diff = static_cast<double>(img1.getPixel(x, y)) - static_cast<double>(img2.getPixel(x, y));
            mse += diff * diff;
        }
    }
    
    return mse / totalPixels;
}

double calculatePSNR(const PGMImage& img1, const PGMImage& img2) {
    double mse = calculateMSE(img1, img2);
    if (mse <= 0.0) return -1.0;
    if (mse < 1e-10) return 100.0;
    
    double maxVal = 255.0;
    return 10.0 * log10((maxVal * maxVal) / mse);
}

double calculateSSIM(const PGMImage& img1, const PGMImage& img2) {
    if (!img1.isValid() || !img2.isValid()) return -1.0;
    if (img1.getWidth() != img2.getWidth() || img1.getHeight() != img2.getHeight()) return -1.0;
    
    int width = img1.getWidth();
    int height = img1.getHeight();
    int totalPixels = width * height;
    if (totalPixels == 0) return -1.0;
    
    const double C1 = 6.5025, C2 = 58.5225;
    
    double mu1 = 0.0, mu2 = 0.0;
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            mu1 += img1.getPixel(x, y);
            mu2 += img2.getPixel(x, y);
        }
    }
    mu1 /= totalPixels;
    mu2 /= totalPixels;
    
    double sigma1_sq = 0.0, sigma2_sq = 0.0, sigma12 = 0.0;
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            double diff1 = img1.getPixel(x, y) - mu1;
            double diff2 = img2.getPixel(x, y) - mu2;
            sigma1_sq += diff1 * diff1;
            sigma2_sq += diff2 * diff2;
            sigma12 += diff1 * diff2;
        }
    }
    
    sigma1_sq /= (totalPixels - 1);
    sigma2_sq /= (totalPixels - 1);
    sigma12 /= (totalPixels - 1);
    
    double numerator = (2 * mu1 * mu2 + C1) * (2 * sigma12 + C2);
    double denominator = (mu1 * mu1 + mu2 * mu2 + C1) * (sigma1_sq + sigma2_sq + C2);
    
    if (denominator == 0.0) return 1.0;
    return numerator / denominator;
}

struct FilterResult {
    std::string imageName;
    std::string filterName;
    std::string parameters;
    double mse;
    double psnr;
    double ssim;
};

void processAllImages(const std::string& inputDir, const std::string& outputDir, const std::string& resultsFile) {
    std::ofstream csv(resultsFile);
    if (!csv.is_open()) {
        std::cerr << "Cannot create results file: " << resultsFile << std::endl;
        return;
    }
    
    csv << "Image,Filter,Parameters,MSE,PSNR,SSIM\n";
    fs::create_directories(outputDir);
    
    std::vector<FilterResult> allResults;
    
    for (const auto& entry : fs::directory_iterator(inputDir)) {
        if (entry.is_regular_file() && entry.path().extension() == ".pgm") {
            std::string filename = entry.path().filename().string();
            std::string baseName = entry.path().stem().string();
            
            std::cout << "Processing: " << filename << std::endl;
            
            PGMImage original;
            if (!original.load(entry.path().string())) {
                continue;
            }
            
            std::vector<int> filterSizes = {3, 5, 7};
            std::vector<double> noiseLevels = {0.01, 0.05, 0.1};
            
            for (double noiseLevel : noiseLevels) {
                PGMImage noisy = original;
                noisy.addNoise(noiseLevel);
                
                for (int filterSize : filterSizes) {
                    PGMImage filteredMedian = noisy;
                    filteredMedian.applyMedianFilter(filterSize);
                    
                    FilterResult resultMedian;
                    resultMedian.imageName = filename;
                    resultMedian.filterName = "Median";
                    resultMedian.parameters = "size=" + std::to_string(filterSize) + ",noise=" + std::to_string(noiseLevel);
                    resultMedian.mse = calculateMSE(original, filteredMedian);
                    resultMedian.psnr = calculatePSNR(original, filteredMedian);
                    resultMedian.ssim = calculateSSIM(original, filteredMedian);
                    allResults.push_back(resultMedian);
                    
                    PGMImage filteredGaussian = noisy;
                    filteredGaussian.applyGaussianFilter(filterSize);
                    
                    FilterResult resultGaussian;
                    resultGaussian.imageName = filename;
                    resultGaussian.filterName = "Gaussian";
                    resultGaussian.parameters = "size=" + std::to_string(filterSize) + ",noise=" + std::to_string(noiseLevel);
                    resultGaussian.mse = calculateMSE(original, filteredGaussian);
                    resultGaussian.psnr = calculatePSNR(original, filteredGaussian);
                    resultGaussian.ssim = calculateSSIM(original, filteredGaussian);
                    allResults.push_back(resultGaussian);
                    
                    std::string outputFile = outputDir + "/" + baseName + "_n" + 
                                           std::to_string(static_cast<int>(noiseLevel * 100)) + 
                                           "_f" + std::to_string(filterSize) + ".pgm";
                    filteredMedian.save(outputFile);
                }
            }
        }
    }
    
    if (allResults.empty()) {
        std::cout << "No PGM files found. Creating test image." << std::endl;
        PGMImage testImage;
        testImage.createTestImage(100, 100);
        testImage.save(inputDir + "/test.pgm");
        
        processAllImages(inputDir, outputDir, resultsFile);
        return;
    }
    
    for (const auto& result : allResults) {
        csv << result.imageName << "," << result.filterName << "," << result.parameters << ","
            << result.mse << "," << result.psnr << "," << result.ssim << "\n";
    }
    
    csv.close();
    std::cout << "Results saved to: " << resultsFile << std::endl;
    std::cout << "Total tests: " << allResults.size() << std::endl;
}

int main() {
    std::string inputDir = "images";
    std::string outputDir = "processed";
    std::string resultsFile = "denoising_results.csv";
    
    fs::create_directories(inputDir);
    
    std::cout << "Image Denoising Analysis" << std::endl;
    std::cout << "Input: " << inputDir << std::endl;
    std::cout << "Output: " << outputDir << std::endl;
    
    processAllImages(inputDir, outputDir, resultsFile);
    
    std::cout << "Analysis completed" << std::endl;
    return 0;
}