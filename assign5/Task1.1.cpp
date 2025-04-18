#include <iostream>
#include <vector>

int main() {
    std::vector<int> v1 = {1, 2, 3, 4};
    std::vector<int> v2 = v1;        // Copy
    std::vector<int> v3 = std::move(v1); // Move

    std::cout << "v1 size: " << v1.size() << std::endl;
    std::cout << "v2 size: " << v2.size() << std::endl;
    std::cout << "v3 size: " << v3.size() << std::endl;

    return 0;
}