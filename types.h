#ifndef VARIABLE_TYPES_H
#define VARIABLE_TYPES_H

#include <stddef.h>

// Structure to map variable types to sizes and conversion functions
typedef struct {
    const char* typeName;            // The name of the type (e.g., "Binary", "Float")
    size_t typeSize;                 // Size of the type in bytes
    void (*convert)(void* value, const char* valueStr); // Function to convert the string to the type
} VariableTypeMap;

// Type conversion functions for different data types
void convertBinary(void* value, const char* valueStr);
void convertByte(void* value, const char* valueStr);
void convert2Bytes(void* value, const char* valueStr);
void convert4Bytes(void* value, const char* valueStr);
void convert8Bytes(void* value, const char* valueStr);
void convertFloat(void* value, const char* valueStr);
void convertDouble(void* value, const char* valueStr);
void convertString(void* value, const char* valueStr);
void convertArrayOfByte(void* value, const char* valueStr);

// Function to convert the value based on the type name
int convertValue(const char* typeName, const char* valueStr, void** value);

#endif // VARIABLE_TYPES_H