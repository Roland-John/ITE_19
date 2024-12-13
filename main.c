#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

// Function prototypes
int romanToDecimal(char *roman);
void decimalToWords(int number, char *result);
int performOperation(int num1, int num2, char operator);
void processLine(char *line, FILE *outputFile);

// Main function
int main() {
    FILE *inputFile = fopen("Input.txt", "r");
    FILE *outputFile = fopen("Output.txt", "w");
    char line[256];

    if (!inputFile || !outputFile) {
        printf("Error opening file!\n");
        return 1;
    }

    while (fgets(line, sizeof(line), inputFile)) {
        processLine(line, outputFile);
    }

    fclose(inputFile);
    fclose(outputFile);

    printf("Processing complete. Check Output.txt for results.\n");
    return 0;
}

// Convert Roman numeral to decimal
int romanToDecimal(char *roman) {
    int values[256] = {0};
    values['I'] = 1; values['V'] = 5; values['X'] = 10;
    values['L'] = 50; values['C'] = 100; values['D'] = 500; values['M'] = 1000;

    int total = 0;
    int length = strlen(roman);

    for (int i = 0; i < length; i++) {
        if (values[roman[i]] < values[roman[i + 1]]) {
            total -= values[roman[i]];
        } else {
            total += values[roman[i]];
        }
    }
    return total;
}

// Convert decimal to words
void decimalToWords(int number, char *result) {
    char *ones[] = {"", "One", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine"};
    char *teens[] = {"Eleven", "Twelve", "Thirteen", "Fourteen", "Fifteen", 
                     "Sixteen", "Seventeen", "Eighteen", "Nineteen"};
    char *tens[] = {"", "", "Twenty", "Thirty", "Forty", "Fifty", "Sixty", "Seventy", "Eighty", "Ninety"};
    char *hundreds[] = {"", "One Hundred", "Two Hundred", "Three Hundred", "Four Hundred", "Five Hundred", 
                        "Six Hundred", "Seven Hundred", "Eight Hundred", "Nine Hundred"};
    char *thousands[] = {"", "Thousand", "Million"};

    if (number < 0) {
        strcpy(result, " <--- *Negative Number");
        number = -number;
    } else {
        result[0] = '\0';
    }

    int num = number, part, position = 0;
    char temp[256] = "";

    while (num > 0) {
        part = num % 1000;
        num /= 1000;

        if (part > 0) {
            char chunk[128] = "";

            if (part / 100 > 0) {
                strcat(chunk, hundreds[part / 100]);
                strcat(chunk, " ");
            }

            if (part % 100 >= 11 && part % 100 <= 19) {
                strcat(chunk, teens[part % 10 - 1]);
                strcat(chunk, " ");
            } else {
                if (part / 10 % 10 > 0) {
                    strcat(chunk, tens[part / 10 % 10]);
                    strcat(chunk, " ");
                }
                if (part % 10 > 0) {
                    strcat(chunk, ones[part % 10]);
                    strcat(chunk, " ");
                }
            }

            if (position > 0) {
                strcat(chunk, thousands[position]);
                strcat(chunk, " ");
            }

            sprintf(temp, "%s%s", chunk, result);
            strcpy(result, temp);
        }
        position++;
    }
}

// Perform arithmetic operation
int performOperation(int num1, int num2, char operator) {
    switch (operator) {
        case '+': return num1 + num2;
        case '-': return num1 - num2;
        case '*': return num1 * num2;
        case '/': return num2 != 0 ? num1 / num2 : 0;
        default: return 0;
    }
}

// Process a single line from input
void processLine(char *line, FILE *outputFile) {
    char roman1[64] = "", roman2[64] = "", operator;
    int num1, num2, result;
    char resultInWords[512] = "";

    sscanf(line, "%s %c %s", roman1, &operator, roman2);

    num1 = romanToDecimal(roman1);
    num2 = romanToDecimal(roman2);
    result = performOperation(num1, num2, operator);

    decimalToWords(result, resultInWords);

    fprintf(outputFile, "%s\n", resultInWords);
}
