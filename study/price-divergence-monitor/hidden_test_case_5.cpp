// Duplicate Correlations Ignored
TEST_CASE( "Hidden Test Case 5", "[DivergenceMonitor]" ) {
  constexpr int threshold = 2;
  const std::string expectedOutput = 
  "1 GOOGL 103 GOOG 100\n";
 
  std::vector<pdmInput_t> actions {
    registerPairInput_t{"GOOG", "GOOGL"},
    registerPairInput_t{"GOOGL", "GOOG"},
 
    updatePriceInput_t{"GOOG", 100},
    updatePriceInput_t{"GOOGL", 103},
 
    updatePriceInput_t{"GOOG", 103},
  };
 
  runTestCase(threshold, actions, {expectedOutput});
}