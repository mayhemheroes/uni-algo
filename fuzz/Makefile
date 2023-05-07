unialgo-fuzz: unialgo-fuzz.cpp
	clang++ -g -std=c++17 -fsanitize=fuzzer,address,undefined,nullability -fno-sanitize-recover=undefined,nullability -o unialgo-fuzz unialgo-fuzz.cpp ../src/data.cpp