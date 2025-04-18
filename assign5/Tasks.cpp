#include <iostream>
#include <vector>
#include <chrono>
#include <algorithm> // Para std::copy

/***********************************************************
 * TAREA 1.1: Lectura y discusión sobre std::move con vectores
 ***********************************************************/
void demostracion_tarea_1_1() {
    std::cout << "\n===== TAREA 1.1: Demostración de std::move con vectores =====\n";
    
    std::vector<int> v1 = {1, 2, 3, 4};
    std::vector<int> v2 = v1;        // Copy
    std::vector<int> v3 = std::move(v1); // Move

    std::cout << "v1 size: " << v1.size() << std::endl;
    std::cout << "v2 size: " << v2.size() << std::endl;
    std::cout << "v3 size: " << v3.size() << std::endl;
    
    // La salida esperada es:
    // v1 size: 0
    // v2 size: 4
    // v3 size: 4
}

/***********************************************************
 * TAREA 2.1, 2.2, 2.3: Implementación de la clase Buffer
 * con constructores y operadores de copia y movimiento
 ***********************************************************/
class Buffer {
private:
    int* data;
    size_t size;

public:
    // Constructor normal (Tarea 2.1)
    Buffer(size_t s) : size(s), data(new int[s]) {
        std::cout << "Constructor called\n";
    }

    // Destructor (Tarea 2.1)
    ~Buffer() {
        delete[] data;
        std::cout << "Destructor called\n";
    }

    // Constructor de copia (Tarea 2.2)
    Buffer(const Buffer& other) : size(other.size), data(new int[other.size]) {
        std::copy(other.data, other.data + other.size, data);
        std::cout << "Copy constructor called\n";
    }

    // Operador de asignación de copia (Tarea 2.2)
    Buffer& operator=(const Buffer& other) {
        if (this != &other) {
            delete[] data;
            size = other.size;
            data = new int[size];
            std::copy(other.data, other.data + size, data);
            std::cout << "Copy assignment called\n";
        }
        return *this;
    }

    // Constructor de movimiento (Tarea 2.3)
    Buffer(Buffer&& other) noexcept : data(other.data), size(other.size) {
        other.data = nullptr;
        other.size = 0;
        std::cout << "Move constructor called\n";
    }

    // Operador de asignación de movimiento (Tarea 2.3)
    Buffer& operator=(Buffer&& other) noexcept {
        if (this != &other) {
            delete[] data;
            data = other.data;
            size = other.size;

            other.data = nullptr;
            other.size = 0;
            std::cout << "Move assignment called\n";
        }
        return *this;
    }
    
    // Método para obtener el tamaño
    size_t getSize() const {
        return size;
    }
};

/***********************************************************
 * TAREA 3.1: Crear instancias y observar el comportamiento
 ***********************************************************/
void demostracion_tarea_3_1() {
    std::cout << "\n===== TAREA 3.1: Demostración de la clase Buffer =====\n";
    
    Buffer b1(100);
    Buffer b2 = b1;           // Debería llamar al constructor de copia
    Buffer b3 = std::move(b1); // Debería llamar al constructor de movimiento

    std::cout << "b1 size after move: " << b1.getSize() << std::endl;
    std::cout << "b2 size: " << b2.getSize() << std::endl;
    std::cout << "b3 size: " << b3.getSize() << std::endl;

    Buffer b4(50);
    b4 = b2;                  // Debería llamar a la asignación de copia
    b4 = std::move(b2);       // Debería llamar a la asignación de movimiento
    
    std::cout << "b2 size after move: " << b2.getSize() << std::endl;
    std::cout << "b4 size: " << b4.getSize() << std::endl;
}

/***********************************************************
 * TAREA 4.1: Comparación de rendimiento
 ***********************************************************/
Buffer createLargeBuffer() {
    return Buffer(10'000'000);
}

void demostracion_tarea_4_1() {
    std::cout << "\n===== TAREA 4.1: Comparación de rendimiento =====\n";
    
    const int iterations = 5;  // Reducido para que termine más rápido
    
    // Prueba de construcción por copia
    std::cout << "\nPrueba de construcción por copia:\n";
    {
        auto start = std::chrono::high_resolution_clock::now();
        
        for (int i = 0; i < iterations; i++) {
            Buffer original(10'000'000);
            Buffer copy = original;  // Construcción por copia
        }
        
        auto end = std::chrono::high_resolution_clock::now();
        std::cout << "Duración de construcción por copia: " 
                  << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() 
                  << " ms\n";
    }
    
    // Prueba de construcción por movimiento
    std::cout << "\nPrueba de construcción por movimiento:\n";
    {
        auto start = std::chrono::high_resolution_clock::now();
        
        for (int i = 0; i < iterations; i++) {
            Buffer original(10'000'000);
            Buffer moved = std::move(original);  // Construcción por movimiento
        }
        
        auto end = std::chrono::high_resolution_clock::now();
        std::cout << "Duración de construcción por movimiento: " 
                  << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() 
                  << " ms\n";
    }
    
    // Prueba de asignación por copia
    std::cout << "\nPrueba de asignación por copia:\n";
    {
        auto start = std::chrono::high_resolution_clock::now();
        
        for (int i = 0; i < iterations; i++) {
            Buffer original(10'000'000);
            Buffer copy(1);
            copy = original;  // Asignación por copia
        }
        
        auto end = std::chrono::high_resolution_clock::now();
        std::cout << "Duración de asignación por copia: " 
                  << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() 
                  << " ms\n";
    }
    
    // Prueba de asignación por movimiento
    std::cout << "\nPrueba de asignación por movimiento:\n";
    {
        auto start = std::chrono::high_resolution_clock::now();
        
        for (int i = 0; i < iterations; i++) {
            Buffer original(10'000'000);
            Buffer moved(1);
            moved = std::move(original);  // Asignación por movimiento
        }
        
        auto end = std::chrono::high_resolution_clock::now();
        std::cout << "Duración de asignación por movimiento: " 
                  << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() 
                  << " ms\n";
    }
    
    // Prueba de retorno por valor (RVO y semántica de movimiento)
    std::cout << "\nPrueba de retorno por valor:\n";
    {
        auto start = std::chrono::high_resolution_clock::now();
        
        for (int i = 0; i < iterations; i++) {
            Buffer returned = createLargeBuffer();  // Puede usar RVO o semántica de movimiento
        }
        
        auto end = std::chrono::high_resolution_clock::now();
        std::cout << "Duración de retorno por valor: " 
                  << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() 
                  << " ms\n";
    }
}

/***********************************************************
 * Función principal que ejecuta todas las demostraciones
 ***********************************************************/
int main() {
    std::cout << "PROGRAMA DE DEMOSTRACIÓN DE SEMÁNTICA DE MOVIMIENTO EN C++\n";
    std::cout << "====================================================\n";
    
    demostracion_tarea_1_1();
    demostracion_tarea_3_1();
    demostracion_tarea_4_1();
    
    std::cout << "\n====================================================\n";
    std::cout << "Programa completado exitosamente\n";
    
    return 0;
}