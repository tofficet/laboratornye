
CXX = g++
CXXFLAGS = -std=c++11 
TARGETS = 1 2 3 4 5 knut1 knut2 knut3 knut4 knut8 knut16

.PHONY: all clean run-tests

all: $(TARGETS)

1: 1.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

2: 2.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

3: 3.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

4: 4.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

5: 5.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

knut1: knut1.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

knut2: knut2.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

knut3: knut3.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

knut4: knut4.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

knut8: knut8.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

knut16: knut16.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<
	
clean:
	rm -f $(TARGETS)

run-interactive:
	@echo "Выберите программу для запуска (1,2,3,4,5,knut1,knut2,knut3,knut4,knut8,knut16):"
	@read prog; \
	if [ -f $$prog ]; then \
		./$$prog; \
	else \
		echo "Программа $$prog не скомпилирована. Запустите 'make' сначала."; \
	fi