#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

// Function prototypes
int romanToDecimal(char *roman); // Converts Roman numeral to decimal
void decimalToWords(int number, char *result); // Converts decimal number to its word representation
int performOperation(int num1, int num2, char operator); // Performs arithmetic operations
void processLine(char *line, FILE *outputFile); // Processes a single line from the input file

// Main function
int main() {
    // Open input and output files
    FILE *inputFile = fopen("Input.txt", "r");
    FILE *outputFile = fopen("Output.txt", "w");
    char line[256];

    // Check if files were opened successfully
    if (!inputFile || !outputFile) {
        printf("Error opening file!\n");
        return 1; // Exit with an error code
    }

    // Read each line from the input file and process it
    while (fgets(line, sizeof(line), inputFile)) {
        processLine(line, outputFile);
    }

    // Close files after processing
    fclose(inputFile);
    fclose(outputFile);

    printf("Processing complete. Check Output.txt for results.\n");
    return 0; // Exit successfully
}

// Convert Roman numeral to decimal
int romanToDecimal(char *roman) {
    int values[256] = {0}; // Array to store Roman numeral values
    values['I'] = 1; values['V'] = 5; values['X'] = 10;
    values['L'] = 50; values['C'] = 100; values['D'] = 500; values['M'] = 1000;

    int total = 0; // Stores the total decimal value
    int length = strlen(roman); // Length of the Roman numeral string

    // Loop through the Roman numeral string
    for (int i = 0; i < length; i++) {
        // Check if the current numeral is smaller than the next (e.g., IV = 4)
        if (values[roman[i]] < values[roman[i + 1]]) {
            total -= values[roman[i]]; // Subtract if smaller
        } else {
            total += values[roman[i]]; // Add if larger or equal
        }
    }
    return total; // Return the decimal equivalent
}

// Convert decimal number to its word representation
void decimalToWords(int number, char *result) {
    // Arrays for number-to-word mapping
    char *ones[] = {"", "One", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine"};
    char *teens[] = {"Eleven", "Twelve", "Thirteen", "Fourteen", "Fifteen", 
                     "Sixteen", "Seventeen", "Eighteen", "Nineteen"};
    char *tens[] = {"", "", "Twenty", "Thirty", "Forty", "Fifty", "Sixty", "Seventy", "Eighty", "Ninety"};
    char *hundreds[] = {"", "One Hundred", "Two Hundred", "Three Hundred", "Four Hundred", "Five Hundred", 
                        "Six Hundred", "Seven Hundred", "Eight Hundred", "Nine Hundred"};
    char *thousands[] = {"", "Thousand", "Million"};

    // Handle negative numbers
    if (number < 0) {
        strcpy(result, "(Negative) "); // Add "Negative" to the result
        number = -number; // Convert number to positive for processing
    } else {
        result[0] = '\0'; // Initialize result as an empty string
    }

    // Variables for processing the number
    int num = number, part, position = 0;
    char temp[256] = "";

    // Process the number in chunks of 3 digits (thousands, millions, etc.)
    while (num > 0) {
        part = num % 1000; // Extract the last three digits
        num /= 1000; // Remove the last three digits

        if (part > 0) {
            char chunk[128] = ""; // Temporary string for the current chunk

            // Process the hundreds place
            if (part / 100 > 0) {
                strcat(chunk, hundreds[part / 100]); // Add the corresponding word for hundreds
                strcat(chunk, " "); // Add a space
            }

            // Process tens and ones place
            if (part % 100 >= 11 && part % 100 <= 19) {
                strcat(chunk, teens[part % 10 - 1]); // Handle teen numbers
                strcat(chunk, " "); // Add a space
            } else {
                if (part / 10 % 10 > 0) {
                    strcat(chunk, tens[part / 10 % 10]); // Handle tens
                    strcat(chunk, " "); // Add a space
                }
                if (part % 10 > 0) {
                    strcat(chunk, ones[part % 10]); // Handle ones
                    strcat(chunk, " "); // Add a space
                }
            }

            // Add positional label (e.g., Thousand, Million)
            if (position > 0) {
                strcat(chunk, thousands[position]);
                strcat(chunk, " ");
            }

            // Combine the current chunk with the overall result
            sprintf(temp, "%s%s", chunk, result);
            strcpy(result, temp);
        }
        position++; // Move to the next position (thousands → millions, etc.)
    }
}

// Perform arithmetic operation
int performOperation(int num1, int num2, char operator) {
    // Perform the operation based on the operator
    switch (operator) {
        case '+': return num1 + num2; // Addition
        case '-': return num1 - num2; // Subtraction
        case '*': return num1 * num2; // Multiplication
        case '/': return num2 != 0 ? num1 / num2 : 0; // Division (check for division by zero)
        default: return 0; // Default case for unsupported operators
    }
}

// Process a single line from the input file
void processLine(char *line, FILE *outputFile) {
    char roman1[64] = "", roman2[64] = "", operator; // Buffers for input Roman numerals and operator
    int num1, num2, result; // Variables for decimal equivalents and result
    char resultInWords[512] = ""; // Buffer for result in words

    // Parse the line for two Roman numerals and an operator
    sscanf(line, "%s %c %s", roman1, &operator, roman2);

    // Convert Roman numerals to decimal
    num1 = romanToDecimal(roman1);
    num2 = romanToDecimal(roman2);

    // Perform the arithmetic operation
    result = performOperation(num1, num2, operator);

    // Convert the result to words
    decimalToWords(result, resultInWords);

    // Write the result in words to the output file
    fprintf(outputFile, "%s\n", resultInWords);
}
