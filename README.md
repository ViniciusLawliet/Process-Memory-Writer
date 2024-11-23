# **Process Memory Writer**

A tool for manipulating memory of external processes in Windows. Designed for debugging, game modding, or real-time application control, this utility enables you to write values in process memory safely and efficiently.

---

## **Features**
- Retrieve process IDs by their name.
- Write values to memory with support for multiple data types.
- Handles runtime type conversion for seamless integration with user input.

---

## **Requirements**
- GCC (MinGW recommended for Windows).
- Windows operating system.

---

## **Compilation**
To compile the project, use the following command:

```bash
gcc memory_editor.c types.c -o memory_editor
```

---

## **Usage**
```bash
memory_editor.exe <process_name> <module_name> <hex_offset> <type> <value>
```

### **Arguments**
- **process_name**: Name of the process (e.g., `ePSXe.exe`).
- **module_name**: Name of the module to locate (e.g., `ePSXe.exe`).
- **hex_offset**: Offset in hexadecimal to calculate the target address (e.g., `AF5424`).
- **type**: Data type of the value to write. Supported types (see the table below).
- **value**: The value to write to the specified address.

---

### **Supported Data Types**

| **Type**         | **Description**                                         | **Bytes**         |
|-------------------|---------------------------------------------------------|-------------------|
| `"Binary"`        | Used for binary values (0 or 1).                        | 1 byte            |
| `"Byte"`          | Used for small integers or single bytes.                | 1 byte            |
| `"2 Bytes"`       | Used for short integers.                                | 2 bytes           |
| `"4 Bytes"`       | Used for standard integers.                             | 4 bytes           |
| `"8 Bytes"`       | Used for long integers or large values.                 | 8 bytes           |
| `"Float"`         | Used for single-precision floating-point numbers.       | 4 bytes           |
| `"Double"`        | Used for double-precision floating-point numbers.       | 8 bytes           |
| `"String"`        | Used for text values (variable size).                   | Variable size     |
| `"Array of byte"` | Used for arrays of raw bytes (variable size).           | Variable size     |

---

### **Example Command**
```bash
.\memory_editor.exe ePSXe.exe ePSXe.exe AF5424 "4 Bytes" 16
```
This command writes the integer value `16` to the memory address `AF5424` of the `ePSXe.exe` module.

---

## **How It Works**
1. Retrieves the process ID by name.
2. Locates the base address of the specified module.
3. Calculates the target address using the base address and given offset.
4. Converts the provided value to the specified data type.
5. Writes the value to the calculated memory address.

---

## **Warnings**
- Ensure you have the necessary permissions to manipulate process memory.
- Writing invalid values or to incorrect memory addresses can lead to application crashes or undefined behavior.

---

## **Author**
**Vinicius Lawliet**  
For questions, feedback, or collaboration, feel free to reach out.
