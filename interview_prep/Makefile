CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra

BINS = bin/q1_min_stack bin/q2_lru_cache bin/q3_logger_limiter \
       bin/q4_design_twitter bin/q5_time_map

.PHONY: all run clean

all: bin $(BINS)

bin:
	mkdir -p bin

bin/q1_min_stack: q1_min_stack/test.cpp q1_min_stack/solution.h
	$(CXX) $(CXXFLAGS) -I q1_min_stack -o $@ $<

bin/q2_lru_cache: q2_lru_cache/test.cpp q2_lru_cache/solution.h
	$(CXX) $(CXXFLAGS) -I q2_lru_cache -o $@ $<

bin/q3_logger_limiter: q3_logger_limiter/test.cpp q3_logger_limiter/solution.h
	$(CXX) $(CXXFLAGS) -I q3_logger_limiter -o $@ $<

bin/q4_design_twitter: q4_design_twitter/test.cpp q4_design_twitter/solution.h
	$(CXX) $(CXXFLAGS) -I q4_design_twitter -o $@ $<

bin/q5_time_map: q5_time_map/test.cpp q5_time_map/solution.h
	$(CXX) $(CXXFLAGS) -I q5_time_map -o $@ $<

run: all
	@fail=0; \
	for t in $(BINS); do \
		echo ""; \
		./$$t || fail=1; \
	done; \
	echo ""; \
	if [ $$fail -eq 0 ]; then echo "All suites passed."; else echo "One or more suites had failures."; fi; \
	exit $$fail

clean:
	rm -rf bin
