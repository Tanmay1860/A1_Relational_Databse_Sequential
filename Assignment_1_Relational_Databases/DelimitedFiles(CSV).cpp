#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#pragma warning(disable: 4996) // required by Visual Studio


#define MAX_RECORDS 100
#define RECORD_SIZE 256
#define FILENAME "products.csv"

typedef struct {
    int productId;
    char name[50];
    char category[50];
    int quantity;
    float price;
} Product;

// Function Prototypes
void addProduct(Product product);
void readProducts();
void updateProduct(int productId, Product updatedProduct);
void deleteProduct(int productId);
void saveProducts(Product products[], int count);
int loadProducts(Product products[]);

int main() {
    int choice, id;
    Product product, updatedProduct;
    Product products[MAX_RECORDS];
    int count;
    

    while (1) {
        printf("\nInventory Management System\n");
        printf("\n1. Add Product\n");
        printf("2. Read Products\n");
        printf("3. Update Product\n");
        printf("4. Delete Product\n");
        printf("5. Exit\n");
        printf("Choose an option: ");
        scanf("%d", &choice);
        getchar(); // To consume the newline character

        switch (choice) {
        case 1:
            printf("Enter Product ID: ");
            scanf("%d", &product.productId);
            getchar();
            printf("Enter Name: ");
            fgets(product.name, sizeof(product.name), stdin);
            product.name[strcspn(product.name, "\n")] = 0; // Remove newline
            printf("Enter Category: ");
            fgets(product.category, sizeof(product.category), stdin);
            product.category[strcspn(product.category, "\n")] = 0;
            printf("Enter Quantity: ");
            scanf("%d", &product.quantity);
            printf("Enter Price: ");
            scanf("%f", &product.price);
            addProduct(product);
            break;

        case 2:
            readProducts();
            break;

        case 3:
            printf("Enter Product ID to update: ");
            scanf("%d", &id);
            printf("Enter new Name: ");
            getchar();
            fgets(updatedProduct.name, sizeof(updatedProduct.name), stdin);
            updatedProduct.name[strcspn(updatedProduct.name, "\n")] = 0;
            printf("Enter new Category: ");
            fgets(updatedProduct.category, sizeof(updatedProduct.category), stdin);
            updatedProduct.category[strcspn(updatedProduct.category, "\n")] = 0;
            printf("Enter new Quantity: ");
            scanf("%d", &updatedProduct.quantity);
            printf("Enter new Price: ");
            scanf("%f", &updatedProduct.price);
            updateProduct(id, updatedProduct);
            break;

        case 4:
            printf("Enter Product ID to delete: ");
            scanf("%d", &id);
            deleteProduct(id);
            break;

        case 5:
            exit(0);
            break;

        default:
            printf("\n***Invalid option! Please try again.***\n");
        }
    }

    return 0;
}

void addProduct(Product product) {
    FILE* file = fopen(FILENAME, "a");
    if (!file) {
        perror("\n***Failed to open file***");
        return;
    }
    fprintf(file, "%d,%s,%s,%d,%.2f\n", product.productId, product.name, product.category, product.quantity, product.price);
    fclose(file);
    printf("\n-> Product added successfully!\n");
}

void readProducts() {
    Product products[MAX_RECORDS];
    int count = loadProducts(products);
    if (count == 0) {
        printf("\n-> No products found!\n");
        return;
    }

    printf("\n   ProductId          Name                 Category           Quantity       Price\n");
    for (int i = 0; i < count; i++) {
        printf("|------------|----------------------|----------------------|------------|------------|\n");
        printf("|%-10d  |  %-20s| %-20s |  %-10d|  $%-9.2f|\n", products[i].productId, products[i].name, products[i].category, 
            products[i].quantity, products[i].price);
    }
    printf("|------------|----------------------|----------------------|------------|------------|\n");
}

int loadProducts(Product products[]) {
    FILE* file = fopen(FILENAME, "r");
    if (!file) {
        perror("\n***Failed to open file***");
        return 0;
    }

    int count = 0;
    while (fgets(products[count].name, sizeof(products[count].name), file) && count < MAX_RECORDS) {
        char* token = strtok(products[count].name, ",");
        products[count].productId = atoi(token);

        token = strtok(NULL, ",");
        strcpy(products[count].name, token);

        token = strtok(NULL, ",");
        strcpy(products[count].category, token);

        token = strtok(NULL, ",");
        products[count].quantity = atoi(token);

        token = strtok(NULL, ",");
        products[count].price = atof(token);

        count++;
    }
    fclose(file);
    return count;
}

void updateProduct(int productId, Product updatedProduct) {
    Product products[MAX_RECORDS];
    int count = loadProducts(products);
    int found = 0;

    for (int i = 0; i < count; i++) {
        if (products[i].productId == productId) {
            products[i] = updatedProduct; // Update product
            products[i].productId = productId; // Keep original ID
            found = 1;
            break;
        }
    }

    if (found) {
        saveProducts(products, count);
        printf("\n-> Product updated successfully!\n");
    }
    else {
        printf("\n***Product ID not found!***\n");
    }
}

void deleteProduct(int productId) {
    Product products[MAX_RECORDS];
    int count = loadProducts(products);
    int found = 0;

    for (int i = 0; i < count; i++) {
        if (products[i].productId == productId) {
            found = 1;
            // Shift remaining products left
            for (int j = i; j < count - 1; j++) {
                products[j] = products[j + 1];
            }
            count--; // Reduce the count of products
            break;
        }
    }

    if (found) {
        saveProducts(products, count);
        printf("\n-> Product deleted successfully!\n");
    }
    else {
        printf("\n***Product ID not found!***\n");
    }
}

void saveProducts(Product products[], int count) {
    FILE* file = fopen(FILENAME, "w");
    if (!file) {
        perror("\n***Failed to open file***");
        return;
    }

    for (int i = 0; i < count; i++) {
        fprintf(file, "%d,%s,%s,%d,%.2f\n", products[i].productId, products[i].name, products[i].category, products[i].quantity, products[i].price);
    }
    fclose(file);
}