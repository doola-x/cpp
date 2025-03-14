#ifndef PRICE_DIVERGENCE_MONITOR_H
#define PRICE_DIVERGENCE_MONITOR_H
 
#include <string>
#include <vector>
#include <utility>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
 
 
struct pair_hash {
  template<class T1, class T2>
  std::size_t operator()(const std::pair<T1, T2> &p) const {
    auto h1 = std::hash<T1>{}(p.first);
    auto h2 = std::hash<T2>{}(p.second);
 
    return h1 ^ h2; // may need to worry about collisions
  }
};
 
class PriceDivergenceMonitor {
public:
    // Constructor
    PriceDivergenceMonitor(int _threshold);
 
    // Register a pair of stocks to monitor
    void RegisterPair(const std::string& stockOne, const std::string& stockTwo);
 
    // Update the price of a stock
    void UpdatePrice(const std::string& stockName, int newPrice);
    
    // calculate potential divergence of two stocks
    int CalculateDivergence(int p1, int p2);
 
    // track pairs in a pair<string, string>, prices <string, int>, and to fetch pairs: string to vector of pairs
    std::unordered_set<std::pair<std::string, std::string>, pair_hash> pairs;
    std::unordered_map<std::string, int> prices;
    std::unordered_map<std::string, std::vector<std::pair<std::string, std::string>>> stockPairs;
    int threshold;
private:
    // Helper function to report divergence (Note: The testing suite WILL check that this is used)
    void ReportDivergence(const std::string& updatedStockName, int updatedStockPrice, const std::string& otherStockName, int otherStockPrice);
};
 
#endif // PRICE_DIVERGENCE_MONITOR_H