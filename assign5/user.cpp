#include "user.h"
#include <iostream>
#include <algorithm>

User::User(const std::string& name)
  : _name(name), _friends(nullptr), _size(0), _capacity(0) {}

User::~User() {
    delete[] _friends;
}

User::User(const User& user) : _name(user._name), _size(user._size), _capacity(user._capacity) {
    _friends = new std::string[_capacity];
    std::copy(user._friends, user._friends + _size, _friends);
}

User& User::operator=(const User& user) {
    if (this != &user) {
        delete[] _friends;
        _name = user._name;
        _size = user._size;
        _capacity = user._capacity;
        _friends = new std::string[_capacity];
        std::copy(user._friends, user._friends + _size, _friends);
    }
    return *this;
}

void User::add_friend(const std::string& name) {
    // Si ya no hay espacio, crear un nuevo array más grande
    if (_size == _capacity) {
        size_t new_capacity = _capacity == 0 ? 1 : _capacity * 2;
        std::string* new_friends = new std::string[new_capacity];
        
        // Copiar los amigos existentes al nuevo array
        for (size_t i = 0; i < _size; ++i) {
            new_friends[i] = _friends[i];
        }
        
        // Liberar el array antiguo y actualizar los punteros/capacidad
        delete[] _friends;
        _friends = new_friends;
        _capacity = new_capacity;
    }
    
    // Añadir el nuevo amigo
    _friends[_size++] = name;
}

std::string User::get_name() const {
    return _name;
}

size_t User::size() const {
    return _size;
}

void User::set_friend(size_t index, const std::string& name) {
    if (index < _size) {
        _friends[index] = name;
    }
}

std::ostream& operator<<(std::ostream& os, const User& user) {
    os << "User(name=" << user._name << ", friends=[";
    for (size_t i = 0; i < user._size; ++i) {
        os << user._friends[i];
        if (i < user._size - 1) os << ", ";
    }
    os << "])";
    return os;
}

User& User::operator+=(User& rhs) {
    if (this != &rhs) {
        add_friend(rhs._name);
        rhs.add_friend(_name);
    }
    return *this;
}

bool User::operator<(const User& rhs) const {
    return _name < rhs._name;
}