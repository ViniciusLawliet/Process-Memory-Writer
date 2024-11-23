#include <windows.h>
#include <tlhelp32.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "types.h"

// Writes a value to a specific address in the memory of a given process.
// The function is designed to handle all types supported in the `convertValue` function.
void writeMemory(DWORD processID, LPCVOID address, void* value, size_t size) {
    // Open the target process with all necessary access rights
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processID);
    if (hProcess) {
        // Write the memory at the given address with the specified value
        if (WriteProcessMemory(hProcess, (LPVOID)address, value, size, NULL)) {
            printf("Value successfully written to memory.\n");
        } else {
            // If the writing fails, print the error code
            printf("Failed to write memory. Error: %lu\n", GetLastError());
        }
        // Close the handle to the process after operation
        CloseHandle(hProcess);
    } else {
        // If the process couldn't be opened, print the error code
        printf("Failed to open process. Error: %lu\n", GetLastError());
    }
}

// Retrieves the process ID (PID) by the name of the process.
// Returns the PID if found, otherwise returns 0.
DWORD GetProcessIDByName(const char* processName) {
    PROCESSENTRY32 processEntry;
    processEntry.dwSize = sizeof(PROCESSENTRY32);

    // Create a snapshot of all processes
    HANDLE hProcessSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcessSnapshot == INVALID_HANDLE_VALUE) {
        printf("Failed to create process snapshot.\n");
        return 0;
    }

    // Iterate through the processes to find the process by name
    if (Process32First(hProcessSnapshot, &processEntry)) {
        do {
            if (strcmp(processEntry.szExeFile, processName) == 0) {
                CloseHandle(hProcessSnapshot);
                return processEntry.th32ProcessID;
            }
        } while (Process32Next(hProcessSnapshot, &processEntry));
    }

    CloseHandle(hProcessSnapshot);
    return 0;
}

// Retrieves the base address of a module in a process.
// Returns the base address of the module, or 0 if not found.
DWORD_PTR GetModuleBaseAddress(DWORD processID, const char* moduleName) {
    MODULEENTRY32 moduleEntry;
    moduleEntry.dwSize = sizeof(MODULEENTRY32);

    // Create a snapshot of all modules in the process
    HANDLE hModuleSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, processID);
    if (hModuleSnapshot == INVALID_HANDLE_VALUE) {
        printf("Failed to create module snapshot.\n");
        return 0;
    }

    // Iterate through the modules to find the module by name
    if (Module32First(hModuleSnapshot, &moduleEntry)) {
        do {
            if (strcmp(moduleEntry.szModule, moduleName) == 0) {
                CloseHandle(hModuleSnapshot);
                return (DWORD_PTR)moduleEntry.modBaseAddr;
            }
        } while (Module32Next(hModuleSnapshot, &moduleEntry));
    }

    CloseHandle(hModuleSnapshot);
    return 0;
}

// Main function to execute the memory write operation
int main(int argc, char* argv[]) {
    // Validate input arguments
    if (argc != 6) {
        printf("Usage: %s <process_name> <module_name> <hex_offset> <type> <value>\n", argv[0]);
        return 1;
    }

    const char* processName = argv[1];
    const char* moduleName = argv[2];
    const char* typeName = argv[4];
    const char* valueStr = argv[5];

    // Get the process ID
    DWORD processID = GetProcessIDByName(processName);
    if (processID == 0) {
        printf("Process not found.\n");
        return 1;
    } else {
        printf("Process found! PID: %lu\n", processID);
    }

    // Get the module base address
    DWORD_PTR moduleBase = GetModuleBaseAddress(processID, moduleName);
    if (moduleBase == 0) {
        printf("Failed to get module base address. Ensure the module name is correct.\n");
        return 1;
    }
    printf("Module base address: 0x%llX\n", moduleBase);

    // Convert offset (hexadecimal string) to a number
    DWORD_PTR offset = strtoull(argv[3], NULL, 16);
    DWORD_PTR targetAddress = moduleBase + offset;

    void* value = NULL;

    // Determine the size of the type and convert the value
    int typeSize = convertValue(typeName, valueStr, &value);
    if (typeSize == -1) {
        printf("Invalid type specified.\n");
        printf("Available data types:\n");
            printf("\t1. \"Binary\"        (1 byte): Used for binary values (0 or 1).\n");
            printf("\t2. \"Byte\"          (1 byte): Used for small integers or single bytes.\n");
            printf("\t3. \"2 Bytes\"       (2 bytes): Used for short integers.\n");
            printf("\t4. \"4 Bytes\"       (4 bytes): Used for standard integers.\n");
            printf("\t5. \"8 Bytes\"       (8 bytes): Used for long integers or large values.\n");
            printf("\t6. \"Float\"         (4 bytes): Used for single-precision floating-point numbers.\n");
            printf("\t7. \"Double\"        (8 bytes): Used for double-precision floating-point numbers.\n");
            printf("\t8. \"String\"        (variable size): Used for text values.\n");
            printf("\t9. \"Array of byte\" (variable size): Used for arrays of raw bytes.\n");
        return 1;
    }

    // Write the value to memory
    printf("Writing value %s (%s) to address 0x%llX...\n", valueStr, typeName, targetAddress);
    writeMemory(processID, (LPCVOID)targetAddress, value, typeSize);

    // Free the allocated memory
    free(value);

    return 0;
}