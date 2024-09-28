#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#pragma warning(disable: 4996) // required by Visual Studio

#define MAX_RECORDS 100
#define RECORD_SIZE 256
#define FILENAME "products.dat"

typedef struct {
    int productId;
    char name[50];
    char category[50];
    int quantity;
    float price;
} Product;


//Function prototypes
void addProduct(const char* filename, int productId, Product product);
Product searchProductByPosition(const char* filename, int productId);
void updateProduct(const char* filename, int productId, Product product);
void deleteProduct(const char* filename, int productId);
int calculateHash(int productId);
void loadProducts(const char* filename);

void menuRandomAccess(const char* filename);


void menuRandomAccess(const char* filename)
{
    int choice, productId;
    Product product;

    while (1) {
        printf("\nInventory Management System\n");
        printf("1. Add Product\n");
        printf("2. View Product by ID\n");
        printf("3. Update Product\n");
        printf("4. Delete Product\n");
        printf("5. Load All Products\n");
        printf("6. Exit\n");
        printf("Choose an option: ");
        scanf("%d", &choice);
        getchar();

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
            addProduct(filename, product.productId, product);
            break;
        case 2:
            printf("Enter Product ID to search: ");
            scanf("%d", &productId);
            product = searchProductByPosition(filename, productId);
            printf("\nProduct ID: %d\nName: %s\nCategory: %s\nQuantity: %d\nPrice: %.2f\n",
                product.productId, product.name, product.category, product.quantity, product.price);
            break;
        case 3:
            printf("Enter Product ID to update: ");
            scanf("%d", &productId);
            printf("Enter new Name: ");
            getchar();
            fgets(product.name, sizeof(product.name), stdin);
            product.name[strcspn(product.name, "\n")] = 0;
            printf("Enter new Category: ");
            fgets(product.category, sizeof(product.category), stdin);
            product.category[strcspn(product.category, "\n")] = 0;
            printf("Enter new Quantity: ");
            scanf("%d", &product.quantity);
            printf("Enter new Price: ");
            scanf("%f", &product.price);
            updateProduct(filename, productId, product);
            break;
        case 4:
            printf("Enter Product ID to delete: ");
            scanf("%d", &productId);
            deleteProduct(filename, productId);
            break;
        case 5:
            loadProducts(filename);
            break;
        case 6:
            exit(0);
            break;

        default:
            printf("\nInvalid option. Please try again.\n");
        }

    }
}

void addProduct(const char* filename, int productId, Product product) {
    FILE* file = fopen(filename, "ab");
    // change postion for product Id check if the product Id is 0 and check if there is another one with the same product id.
   // (if there is nothing with that product then create the product, else return error)
    if (productId == 0) {
        printf("You cant add a product with product Id = 0\n");
        exit(EXIT_FAILURE);
    }

    if (file == NULL) {
        printf("Cannot open file");
        exit(EXIT_FAILURE);
    }
    Product existingProduct = searchProductByPosition(filename, productId);
    if (existingProduct.productId != -1) {
        printf("Product ID %d already exists.\n", productId);
        fclose(file);
        return;
    }
    int whereToWriteTheRecord = calculateHash(productId);
    if (fseek(file, whereToWriteTheRecord, SEEK_SET) != 0) {
        printf("Error seeking to the position %d. \n", whereToWriteTheRecord);
        exit(EXIT_FAILURE);
    }
    if (fwrite(&product, sizeof(Product), 1, file) != 1) {
        printf("Error writing to the file\n");
        exit(EXIT_FAILURE);
    }
    if (fclose(file) != 0) {
        printf("Error closing the file\n");
        exit(EXIT_FAILURE);
    }
    printf("Product added successfully.\n");
}

Product searchProductByPosition(const char* filename, int productId)
{
    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        printf("Cannot open file");
        exit(EXIT_FAILURE);
    }
    int position = calculateHash(productId);
    if (fseek(file, position, SEEK_SET) != 0) {
        printf("Error seeking to the position %d. \n", productId);
        exit(EXIT_FAILURE);
    }
    Product product;
    if (fread(&product, sizeof(Product), 1, file) == 1 && product.productId == productId) {
        if (fclose(file) != 0) {
            printf("Error closing the file\n");
            exit(EXIT_FAILURE);
        }
        return product;
    }//error cheking
    if (fclose(file) != 0) {
        printf("Error closing the file\n");
        exit(EXIT_FAILURE);
    }
    Product notFound = { -1,"","", 0, 0.0 };
    return product;
}

void loadProducts(const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        printf("Cannot open file \n");
        return;
    }

    Product product;
    printf("\nLoaded Products: \n");
    while (fread(&product, sizeof(Product), 1, file) == 1) { 
        if (product.productId != -1) {  // Skip deleted products 
            printf("ID: %d, Name: %s, Category: %s, Quantity: %d, Price: %.2f\n",
                product.productId, product.name, product.category, product.quantity, product.price); 
        }
    }

    if (fclose(file) != 0) { 
        printf("Error closing the file\n"); 
        exit(EXIT_FAILURE); 
    }

}

void updateProduct(const char* filename, int position, Product product) {
    FILE* file = fopen(filename, "rb+");
    if (file == NULL) {
        printf("Cannot open file");
        exit(EXIT_FAILURE);
    }
    int whereToReadTheRecord = calculateHash(position);
    if(fseek(file, whereToReadTheRecord, SEEK_SET) != 0) {
        printf("Error seeking to the position %d. \n", whereToReadTheRecord);
        exit(EXIT_FAILURE);
    }
   if (fwrite(&product, sizeof(Product), 1, file) != 1) {
       printf("Error writing to the file\n");
       exit(EXIT_FAILURE);
    }
    if (fclose(file) != 0) {
        printf("Error closing the file\n");
        exit(EXIT_FAILURE);
    }
}

void deleteProduct(const char* filename, int productId) {
    FILE* file = fopen(filename, "rb+");
    if (file == NULL) {
        printf("Cannot open file");
        exit(EXIT_FAILURE);
    }
    int position = calculateHash(productId);
   if (fseek(file, position, SEEK_SET) != 0) {
       printf("Error seeking to the position \n");
       exit(EXIT_FAILURE);
   }

    Product deletedProduct = { -1, "", "", 0, 0.0 };  // Mark as deleted
    if (fwrite(&deletedProduct, sizeof(Product), 1, file) != 1) {
        printf("Error writing to the file\n");
        exit(EXIT_FAILURE);
    }

    if (fclose(file) != 0) {
        printf("Error closing the file\n");
        exit(EXIT_FAILURE);
    }
    printf("Product at productId %d has been deleted.\n", productId);
}

int calculateHash(int productId) {
    return productId * sizeof(Product);
}




