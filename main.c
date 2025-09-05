#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Share {
    int x, base, y;
    char value[20];
};


int toDecimal(char* str, int base) {
    int result = 0, power = 1;
    for (int i = strlen(str) - 1; i >= 0; i--) {
        result += (str[i] - '0') * power;
        power *= base;
    }
    return result;
}


int findSecret(struct Share* shares) {
   
    int x1 = shares[0].x, y1 = shares[0].y;
    int x2 = shares[1].x, y2 = shares[1].y;
    int x3 = shares[2].x, y3 = shares[2].y;
    
    
    double secret = y1 * (0-x2) * (0-x3) / ((x1-x2) * (x1-x3)) +
                   y2 * (0-x1) * (0-x3) / ((x2-x1) * (x2-x3)) +
                   y3 * (0-x1) * (0-x2) / ((x3-x1) * (x3-x2));
    
    return (int)(secret + 0.5); // Round to nearest integer
}

int main() {
    FILE* file;
    char line[100], filename[50];
    struct Share shares[10];
    int count = 0, n = 0, k = 0;
    
   
    printf("Enter JSON file name: ");
    scanf("%s", filename);
    
    file = fopen(filename, "r");
    if (!file) {
        printf("Cannot open file!\n");
        return 1;
    }
    
    printf("\nReading file...\n");
    
    // Read file line by line
    while (fgets(line, sizeof(line), file)) {
        // Get n and k values
        if (strstr(line, "\"n\"")) sscanf(line, " \"n\": %d", &n);
        if (strstr(line, "\"k\"")) sscanf(line, " \"k\": %d", &k);
        
        // Get share ID
        if (line[4] == '"' && line[5] >= '0' && line[5] <= '9') {
            shares[count].x = line[5] - '0';
            
            // Read next two lines for base and value
            fgets(line, sizeof(line), file); // base line
            sscanf(line, " \"base\": \"%d\"", &shares[count].base);
            
            fgets(line, sizeof(line), file); // value line
            sscanf(line, " \"value\": \"%[^\"]\"", shares[count].value);
            
            // Convert to decimal
            shares[count].y = toDecimal(shares[count].value, shares[count].base);
            count++;
        }
    }
    fclose(file);
    
    // Show results
    printf("\nResults:\n");
    printf("Need %d out of %d shares\n", k, n);
    printf("Found %d shares:\n", count);
    
    for (int i = 0; i < count; i++) {
        printf("Share %d: %s (base %d) = %d\n", 
               shares[i].x, shares[i].value, shares[i].base, shares[i].y);
    }
    
    if (count >= k) {
        int secret = findSecret(shares);
        printf("\nSecret: %d\n", secret);
    } else {
        printf("Need more shares!\n");
    }
    
    return 0;
}
