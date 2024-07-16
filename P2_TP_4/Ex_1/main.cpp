#include <iostream>
#include <fstream>
#include <vector>
#include <stdexcept>
#include <string>
#include <sstream>
#include <algorithm>


int main() 
{
    try 
    {
        LoadBinary("secret_message.txt");
    }
    catch (const std::exception& e) 
    {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}

// Función para decodificar base64
std::string base64_decode(const std::string& in) {
    std::string out;
    std::vector<int> T(256, -1);
    for (int i = 0; i < 64; i++) T["ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[i]] = i;

    int val = 0, valb = -8;
    for (unsigned char c : in) {
        if (T[c] == -1) break;
        val = (val << 6) + T[c];
        valb += 6;
        if (valb >= 0) {
            out.push_back(char((val >> valb) & 0xFF));
            valb -= 8;
        }
    }
    return out;
}

void LoadBinary(const std::string& filePath) {
    // Leer el archivo base64 como una cadena de texto
    std::ifstream file(filePath);
    if (!file) {
        throw std::runtime_error("Unable to open file " + filePath);
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string base64Content = buffer.str();
    file.close();

    // Decodificar el contenido base64
    std::string binaryContent = base64_decode(base64Content);

    // Crear un stream a partir del contenido binario decodificado
    std::istringstream binaryStream(binaryContent, std::ios::binary);

    int size;

    // Leer el encabezado (primer valor) y el tamaño
    binaryStream.read(reinterpret_cast<char*>(&size), sizeof(size));
    if (!binaryStream) {
        throw std::runtime_error("Error reading the size from " + filePath);
    }

    // Leer los valores binarios
    for (int i = 0; i < size; ++i) {
        int value;
        binaryStream.read(reinterpret_cast<char*>(&value), sizeof(value));
        if (!binaryStream) {
            throw std::runtime_error("Error reading value from " + filePath);
        }

        // Aquí podrías procesar el valor leído, por ejemplo, imprimirlo
        std::cout << "Value " << i << ": " << value << std::endl;
    }
}


