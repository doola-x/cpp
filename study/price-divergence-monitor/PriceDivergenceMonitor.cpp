#include "PriceDivergenceMonitor.h"
 
PriceDivergenceMonitor::PriceDivergenceMonitor(int _threshold) {
  threshold = _threshold;
}
 
int PriceDivergenceMonitor::CalculateDivergence(int p1, int p2) {
  return std::abs(p1 - p2);
}
 
void PriceDivergenceMonitor::RegisterPair(const std::string& stockOne, const std::string& stockTwo) {
  std::cout << "registering pair " << stockOne << " -- " << stockTwo << std::endl;
  if (pairs.find({stockOne, stockTwo}) != pairs.end() || pairs.find({stockTwo, stockOne}) != pairs.end()) {
    //already tracking
    std::cout << "already tracking pair " << std::endl;
    return;
  }
  pairs.emplace(stockOne, stockTwo);
 
  //update stock mappings
  stockPairs[stockOne].emplace_back(stockOne, stockTwo);
  stockPairs[stockTwo].emplace_back(stockTwo, stockOne); // flip for stock two so affected stock always first
}
 
//void ReportDivergence(const std::string& updatedStockName, int updatedStockPrice, const std::string& otherStockName, int otherStockPrice);
void PriceDivergenceMonitor::UpdatePrice(const std::string& stockName, int newPrice) {
  std::cout << "updating stock name " << stockName << std::endl;
  prices[stockName] = newPrice;
 
  if (stockPairs.find(stockName) == stockPairs.end()) {
    //no pairs registered
    return;
  }
  //check all pairs that include affected stock
  for (const auto& pair : stockPairs[stockName]) {
    const std::string& a = pair.first;
    const std::string& b = pair.second;
 
    if (prices.find(a) == prices.end() || prices.find(b) == prices.end()) {
      //missing price for either stock
      continue;
    } 
    int p1 = prices[a];
    int p2 = prices[b];
 
    if (!p1 || !p2) continue;
 
    int divergence = CalculateDivergence(p1, p2);
 
    if (divergence > this->threshold) {
        std::cout << "rpt div called: a " << a << ", b " << b  << ", p1 " << p1 << ", p2 " << p2 << std::endl;
        ReportDivergence(a, p1, b, p2);
    } 
  }
}

void PriceDivergenceMonitor::ReportDivergence(const std::string& updatedStockName, int updatedStockPrice, const std::string& otherStockName, int otherStockPrice) {
  std::cout << "reported" << std::endl;
}