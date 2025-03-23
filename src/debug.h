#include <iostream>

template <typename T>
inline void println(T name) {
    std::cout << name << "\n";
}

template <typename T>
inline void print(T name) {
    std::cout << name;
}

inline void error(int name) {
    std::cerr << "\nERROR! Exited with error code:  " << name << std::endl;
    exit(name);
}