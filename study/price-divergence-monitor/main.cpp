#include "PriceDivergenceMonitor.h"

int main() {
	PriceDivergenceMonitor* pdm = new PriceDivergenceMonitor(2);
	/*
	    registerPairInput_t{"GOOG", "GOOGL"},
    registerPairInput_t{"GOOGL", "GOOG"},
 
    updatePriceInput_t{"GOOG", 100},
    updatePriceInput_t{"GOOGL", 103},
 
    updatePriceInput_t{"GOOG", 103},*/
	pdm->RegisterPair("GOOG", "GOOGL");
	pdm->RegisterPair("GOOGL", "GOOG");

	pdm->UpdatePrice("GOOG", 100);
	pdm->UpdatePrice("GOOGL", 103);
	pdm->UpdatePrice("GOOG", 103);
	std::cout << "yes" << std::endl;
}