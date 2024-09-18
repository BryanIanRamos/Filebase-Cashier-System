#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <conio.h>

#define MAX 100
#define SIZE_50 50

#define RED "\033[31m"
#define RESET "\033[0m"

int prod_size = 0;
int cart_size = 0;

typedef struct Product
{
    int id;
    char name[SIZE_50];
    float price;
} Product;

typedef struct CartItem
{
    int id;
    char name[SIZE_50];
    float price;
    int qty;
} CartItem;

Product products[MAX];
CartItem cart[MAX];

Product createProduct(int id, char name[SIZE_50], float price)
{
    Product P;
    P.id = id;
    strcpy(P.name, name);
    P.price = price;
    prod_size++;
    return P;
}

CartItem createCart(int id, char name[SIZE_50], float price, int qty)
{
    CartItem C;
    C.id = id;
    strcpy(C.name, name);
    C.price = price;
    C.qty = qty;
    return C;
}

void addToCart();
void transaction();
void dashboard();

int main()
{
    char username[MAX];
    char password[MAX];
    bool first_attempt = false;
    bool is_not_login = true;

    do
    {
        system("cls");
        if (first_attempt)
        {
            printf("Invalid account or password\n");
            printf("Please Try Again\n\n");
        }

        printf("======Login Account======");
        printf("\n\nUsername: ");
        scanf("%s", username);
        printf("\nPassword: ");
        scanf("%s", password);

        if ((strcmp(username, "admin") == 0) && (strcmp(password, "admin") == 0))
            is_not_login = false;

        first_attempt = true;

    } while (is_not_login);

    system("cls");
    printf("\nLogin Successfully");
    printf("\nLoading...");
    sleep(2);

    dashboard();
    return 0;
}

void dashboard()
{
    bool invalid_input;

    do
    {
        system("cls");
        invalid_input = false;

        printf("=====================================\n");
        printf("            MiniMart System            ");
        printf("\n=====================================\n\n");

        printf("Select options:\n");
        printf("[1] Transaction\n");
        printf("[2] Add Item\n");
        printf("[3] Update Item\n");
        printf("[4] Remove Item\n");
        printf("[5] Activity Log\n");
        printf("[Esc] Exit\n");

        int input = getch();

        switch (input)
        {
        case '1':
            invalid_input = true;
            system("cls");
            printf("Transaction selected.\n");
            printf("Loading...\n");
            sleep(1);
            transaction();
            break;
        case '2':
            printf("\nAdd Item\n");
            break;
        case '3':
            printf("\nUpdate Item\n");
            break;
        case '4':
            printf("\nRemove Item\n");
            break;
        case '5':
            printf("\nActivity Log\n");
            break;
        case 27: // ESC key
            exit(0);
        }
    } while (!invalid_input);
}

void transaction()
{
    int id;
    char name[MAX];
    float price;

    FILE *prodFile = fopen("product.txt", "r");
    if (prodFile == NULL)
    {
        printf("\nFile does not exist.");
        return;
    }

    while (fscanf(prodFile, "%d %s %f", &id, name, &price) == 3)
    {
        if (id != 0)
        {
            products[prod_size] = createProduct(id, name, price);
        }
    }
    fclose(prodFile);

    int input = 0;
    int total_items = 0;
    float total_amount = 0.0;
    float amount = 0.0;
    int temp_item = 0;
    int temp_amount = 0;
    do
    {
        system("cls");
        printf("==========[ Submit a Transaction ]==========\n\n");

        printf("Overall products in cart:\n");
        total_items = 0;
        total_amount = 0;
        for (int i = 0; i < cart_size; i++)
        {
            printf("| ID: %d \t| Name: %s \t| Price: %.2f Php | Quantity: %d\n",
                   cart[i].id, cart[i].name, cart[i].price, cart[i].qty);

            if (input == 13)
            {
                total_items += cart[i].qty;

                amount = cart[i].qty * cart[i].price;
                total_amount += amount;
            }

            temp_item = total_items;
            temp_amount = amount;
        }

        printf("\n\nTotal items: %d | Total Amount: %.2f", temp_item, total_amount);
        printf("\n============================================\n");
        printf("[ENTER] Add product | [ESC] Exit");
        input = getch();

        if (input == 13)
            addToCart();

    } while (input != 27); // ESC key

    clearCart();
    dashboard();
}

void addToCart()
{
    int qty = 0;
    int input;
    bool productFound = false;

    printf("\n\n------Add to cart------");
    printf("\nInput Product ID: ");
    scanf("%d", &input);

    printf("\nInput Quantity: ");
    scanf("%d", &qty);

    // Check if product exists and add to cart
    for (int i = 0; i < prod_size; i++)
    {
        if (products[i].id == input)
        {
            cart[cart_size] = createCart(products[i].id, products[i].name, products[i].price, qty);
            cart_size++;
            productFound = true;
            break;
        }
    }
    if (!productFound)
    {
        printf(RED "\nError: Invalid product ID." RESET);
        getch();
    }
}

void clearCart()
{
    int max_cart_size = cart_size;
    cart_size = 0;

    for (int i = 0; i < max_cart_size; i++)
    {
        cart[i] = createCart(0, "", 0.0, 0);
    }
}

void saveRecipt()
{
    char fileName[20];
    int fileIndex = 1; // Starting index for file names
    FILE *file;

    // Loop to find an available file name
    while (1)
    {
        sprintf(fileName, "./recept/file%d.txt", fileIndex);
        file = fopen(fileName, "r");
        if (file == NULL)
        {
            // File does not exist, this name is available
            break;
        }
        fclose(file);
        fileIndex++;
    }

    // Create and open the file
    file = fopen(fileName, "w");
    if (file == NULL)
    {
        perror("Error opening file");
        return 1;
    }

    // Write something to the file
    // fprintf(file, "This is a test file named %s.\n", fileName);

    // Close the file
    fclose(file);

    // printf("File '%s' created successfully.\n", fileName);
}