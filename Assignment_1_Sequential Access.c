#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LEN 50
#define MAX_CATEGORY_LEN 50

// Define the product structure
typedef struct {
    int productId;
    char name[MAX_NAME_LEN];
    char category[MAX_CATEGORY_LEN];
    int quantity;
    float price;
} Product;

