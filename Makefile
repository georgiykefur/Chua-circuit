CXX = g++
CXXFLAGS = -std=c++11 -O2 -Wall -Wextra

all: dopri ab_ab

dopri: main.cpp chua.cpp dopri5.cpp params.h chua.h dopri5.h
	$(CXX) $(CXXFLAGS) main.cpp chua.cpp dopri5.cpp -o dopri

ab_ab: main_ab.cpp chua.cpp dopri5.cpp AB4.cpp params.h chua.h dopri5.h AB4.h
	$(CXX) $(CXXFLAGS) main_ab.cpp chua.cpp dopri5.cpp AB4.cpp -o ab_ab

run: all
	./dopri
	./ab_ab

.venv/.plot-deps: requirements.txt
	python3 -m venv .venv
	.venv/bin/python -m pip install -r requirements.txt
	touch .venv/.plot-deps

plots: run lyapunov .venv/.plot-deps
	.venv/bin/python plot_results.py

full: plots

clean:
	rm -f dopri ab_ab benettin

benettin: benettin.cpp chua.cpp dopri5.cpp AB4.cpp params.h chua.h dopri5.h AB4.h
	$(CXX) $(CXXFLAGS) benettin.cpp chua.cpp dopri5.cpp AB4.cpp -o benettin

lyapunov: benettin
	./benettin

animation: run .venv/.plot-deps
	.venv/bin/python animate.py


.PHONY: all run plots full clean lyapunov animation
