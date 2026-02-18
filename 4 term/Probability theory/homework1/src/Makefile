CXX = g++
CXXFLAGS = -std=c++11 -I./include
TARGETS = Gmurman6 Gmurman8 Gmurman13 Gmurman15 Gmurman19 Gmurman20 Gmurman21 Gmurman22 Gmurman26 Gmurman27 Gmurman28 Gmurman29 Gmurman30 Gmurman32 Gmurman45

.PHONY: all clean run-interactive

all: $(TARGETS)

Gmurman6: Gmurman6.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

Gmurman8: Gmurman8.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

Gmurman13: Gmurman13.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

Gmurman15: Gmurman15.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

Gmurman19: Gmurman19.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

Gmurman20: Gmurman20.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

Gmurman21: Gmurman21.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

Gmurman22: Gmurman22.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

Gmurman26: Gmurman26.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

Gmurman27: Gmurman27.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

Gmurman28: Gmurman28.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

Gmurman29: Gmurman29.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

Gmurman30: Gmurman30.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

Gmurman32: Gmurman32.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

Gmurman45: Gmurman45.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

clean:
	rm -f $(TARGETS)

run-interactive:
	@echo "Доступные программы:"
	@echo "  Gmurman6, Gmurman8, Gmurman13, Gmurman15, Gmurman19, Gmurman20,"
	@echo "  Gmurman21, Gmurman22, Gmurman26, Gmurman27, Gmurman28, Gmurman29,"
	@echo "  Gmurman30, Gmurman32, Gmurman45"
	@echo ""
	@echo "Введите имя программы для запуска:"
	@read prog; \
	if [ -f "$$prog" ]; then \
		./$$prog; \
	elif [ -f "$$prog.exe" ]; then \
		./$$prog.exe; \
	else \
		echo "Программа $$prog не скомпилирована. Запустите 'make' сначала."; \
	fi

quick: $(TARGETS)
	@echo "Все программы скомпилированы"

run6: Gmurman6
	./Gmurman6

run8: Gmurman8
	./Gmurman8

run13: Gmurman13
	./Gmurman13

run15: Gmurman15
	./Gmurman15

run19: Gmurman19
	./Gmurman19

run20: Gmurman20
	./Gmurman20

run21: Gmurman21
	./Gmurman21

run22: Gmurman22
	./Gmurman22

run26: Gmurman26
	./Gmurman26

run27: Gmurman27
	./Gmurman27

run28: Gmurman28
	./Gmurman28

run29: Gmurman29
	./Gmurman29

run30: Gmurman30
	./Gmurman30

run32: Gmurman32
	./Gmurman32

run45: Gmurman45
	./Gmurman45