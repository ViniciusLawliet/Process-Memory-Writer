#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "types.h"

// Conversion functions for different types
void convertBinary(void* value, const char* valueStr) {
    *(unsigned char*)value = (unsigned char)atoi(valueStr);
}

void convertByte(void* value, const char* valueStr) {
    *(unsigned char*)value = (unsigned char)atoi(valueStr);
}

void convert2Bytes(void* value, const char* valueStr) {
    *(short*)value = (short)atoi(valueStr);
}

void convert4Bytes(void* value, const char* valueStr) {
    *(long*)value = atoi(valueStr);
}

void convert8Bytes(void* value, const char* valueStr) {
    *(long long*)value = atoll(valueStr);
}

void convertFloat(void* value, const char* valueStr) {
    *(float*)value = (float)atof(valueStr);
}

void convertDouble(void* value, const char* valueStr) {
    *(double*)value = atof(valueStr);
}

void convertString(void* value, const char* valueStr) {
    strcpy((char*)value, valueStr);
}

void convertArrayOfByte(void* value, const char* valueStr) {
    memcpy(value, valueStr, strlen(valueStr));
}

// Type mapping table
static const VariableTypeMap typeMap[] = {
    {"Binary", 1, convertBinary},
    {"Byte", 1, convertByte},
    {"2 Bytes", 2, convert2Bytes},
    {"4 Bytes", 4, convert4Bytes},
    {"8 Bytes", 8, convert8Bytes},
    {"Float", sizeof(float), convertFloat},
    {"Double", sizeof(double), convertDouble},
    {"String", 0, convertString}, 
    {"Array of byte", 0, convertArrayOfByte},
};

// Unified memory allocation and conversion logic
int convertValue(const char* typeName, const char* valueStr, void** value) {
    for (size_t i = 0; i < sizeof(typeMap) / sizeof(VariableTypeMap); ++i) {
        if (strcmp(typeName, typeMap[i].typeName) == 0) {
            size_t size = (typeMap[i].typeSize == 0) ? strlen(valueStr) + 1 : typeMap[i].typeSize;
            *value = malloc(size);
            if (*value == NULL) {
                printf("Memory allocation failed for %s.\n", typeName);
                return -1;
            }
            if (typeMap[i].typeSize == 0 && strcmp(typeName, "Array of byte") == 0) {
                // Array of byte doesn't need null terminator
                size -= 1;
            }
            typeMap[i].convert(*value, valueStr);
            return size;
        }
    }
    return -1; // Type not found
}