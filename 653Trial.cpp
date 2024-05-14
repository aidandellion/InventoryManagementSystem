#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define maximum number of prods
#define maxProducts 20
#define maxCustomers 20
#define maxTransactions 50

// Product structure
struct Product {
    char id[10];
    char name[20];
    float price;
    int quantity;
};

// Customer structure
struct Customer {
    char id[10];
    char name[20];
    char membership;
};

// Transaction structure
struct Transaction {
    struct Product prod;
    struct Customer cust;
    int quantity;
    float totalPrice;
};

// Inventory structure
struct Inventory {
    struct Product prod[maxProducts];
    int prodCount;
    struct Customer cust[maxCustomers];
    int custCount;
    struct Transaction transactions[maxTransactions];
    int transactionCount;
};

// Function prototypes
void readProductsFile(struct Inventory *inv);
void readCustomersFile(struct Inventory *inv);
void readTransactionsFile(struct Inventory *inv);
void addProduct(struct Inventory *inv);
void updateProduct(struct Inventory *inv);
void registerCustomer(struct Inventory *inv);
void recordSale(struct Inventory *inv);
void generateReports(struct Inventory *inv);
void displayMenu();
void displayCustomers(struct Inventory *inv);
void displayProducts(struct Inventory *inv);
void displayPastTransactions(struct Inventory *inv);
float totalSales = 0.00;

int main() {
	struct Customer cust;
    struct Inventory inv;
    inv.prodCount = 0;
    inv.custCount = 0;
    inv.transactionCount = 0;

	readProductsFile(&inv);
	readCustomersFile(&inv);
	readTransactionsFile(&inv);
		
    char choice;
    do {
        displayMenu();
        printf("Enter choice: ");
        scanf(" %c", &choice);

        switch (choice) {
            case 'A':
                addProduct(&inv);
                break;
            case 'B':
                updateProduct(&inv);
                break;
            case 'C':
                registerCustomer(&inv);
                break;
            case 'D':
                displayProducts(&inv);
                break;
            case 'E':
            	displayCustomers(&inv);
            	break;
            case 'F':
            	displayPastTransactions(&inv);
            	break;
            case 'G':
            	generateReports(&inv);
            	break;
            case 'Z':
                printf("END OF PROGRAM\n");
                break;
            default:
                printf("Invalid choice! Please try again.\n");
        }

    } while (choice != 'Z');

    return 0;
}

// Function to read prods from a file
void readProductsFile(struct Inventory *inv) {
    FILE *file = fopen("products.txt", "r");
    if (file != NULL) {
        while (fscanf(file, "%9[^;];%19[^;];%f;%d\n",
                      inv->prod[inv->prodCount].id,
                      inv->prod[inv->prodCount].name,
                      &inv->prod[inv->prodCount].price,
                      &inv->prod[inv->prodCount].quantity) == 4) {
            (inv->prodCount)++;
            if (inv->prodCount >= 20) {
                break;
            }
        }
        fclose(file);
        printf("Products successfully loaded from file.\n");
    } else {
        printf("Error loading prods from file.\n");
    }
}

// Function to load custs from a file
void readCustomersFile(struct Inventory *inv) {
    FILE *file = fopen("customers.txt", "r");
    if (file != NULL) {
        while (fscanf(file, "%9[^;];%19[^;];%c\n",
                      inv->cust[inv->custCount].id,
                      inv->cust[inv->custCount].name,
                      &inv->cust[inv->custCount].membership)==3) 
					  {
            				(inv->custCount)++;
            				if (inv->custCount >= maxCustomers) {
                			break;
            				}
       					}
        				fclose(file);
        				printf("Customers sucessfully loaded from file.\n");
    } 
	else {
    printf("Error loading prods from file.\n");
    }
}

void readTransactionsFile(struct Inventory *inv) {
    FILE *file = fopen("transactions.txt", "r");
    if (file != NULL) {
        while (fscanf(file, "%9[^;];%19[^;];%f;%d;%9[^;];%19[^;];%c;%f\n",
                      inv->transactions[inv->transactionCount].prod.id,
                      inv->transactions[inv->transactionCount].prod.name,
                      &inv->transactions[inv->transactionCount].prod.price,
                      &inv->transactions[inv->transactionCount].prod.quantity,
                      inv->transactions[inv->transactionCount].cust.id,
                      inv->transactions[inv->transactionCount].cust.name,
                      &inv->transactions[inv->transactionCount].cust.membership,
                      &inv->transactions[inv->transactionCount].totalPrice) == 8) {
            // Increment the transaction count
            (inv->transactionCount)++;
            // Check if the maximum number of transactions is reached
            if (inv->transactionCount >= maxTransactions) {
                break;
            }
        }
        // Close the file after reading
        fclose(file);
        printf("Transactions successfully loaded from file.\n");
    } else {
        // Display an error message if the file couldn't be opened
        printf("Error loading transactions from file.\n");
    }
}




// Function to add a new prod
void addProduct(struct Inventory *inv) {
    if (inv->prodCount < maxProducts) {
        printf("Enter prod details:\n");
        printf("Product ID: ");
        scanf("%s", inv->prod[inv->prodCount].id);
        printf("Product Name: ");
        scanf("%s", inv->prod[inv->prodCount].name);
        printf("Product Price: ");
        scanf("%f", &inv->prod[inv->prodCount].price);
        printf("Product Quantity: ");
        scanf("%d", &inv->prod[inv->prodCount].quantity);

        printf("Product added successfully!\n");
        (inv->prodCount)++;
    } else {
        printf("Maximum number of prods reached!\n");
    }
}

// Function to update prod information
void updateProduct(struct Inventory *inv) {
    char choiceID[10];
    int updateChoice;

    printf("Enter prod ID to update: ");
    scanf("%s", choiceID);

    // Search for the prod with the given ID
    int foundIndex = -1;
    for (int i = 0; i < inv->prodCount; i++) {
        if (strcmp(choiceID, inv->prod[i].id) == 0) {
            foundIndex = i;
            break;
        }
    }

    if (foundIndex != -1) {
        printf("Product ID Found!\n");
        printf("\n1- Update price \n2- Update quantity\n");
        printf("Enter option to update: ");
        scanf("%d", &updateChoice);

        if (updateChoice == 1) {
            printf("Enter new price: ");
            scanf("%f", &(inv->prod[foundIndex].price));  
            printf("Price updated successfully!\n");
        } else if (updateChoice == 2) {
            printf("Enter new quantity: ");
            scanf("%i", &(inv->prod[foundIndex].quantity));
            printf("Quantity updated successfully!\n");
        } else {
            printf("Invalid Option!\n");
        }
    } else {
        printf("Product ID not found!\n");
    }
}

void registerCustomer(struct Inventory *inventory) {
    struct Transaction t;
    char membership;
    float membershipDiscount = 1.0;  // Default discount for non-members

    if (inventory->custCount < maxCustomers) {
        printf("Enter customer details:\n");
        printf("Customer ID: ");
        scanf("%s", inventory->cust[inventory->custCount].id);
        printf("Customer Name: ");
        scanf("%s", inventory->cust[inventory->custCount].name);

        // Clear the input buffer to handle newline characters
        while (getchar() != '\n');

        printf("Membership (Y-Yes|N-No): ");
        scanf(" %c", &membership);
        inventory->cust[inventory->custCount].membership = membership;

        if (inventory->cust[inventory->custCount].membership == 'Y') {
            printf("Eligible for membership's discount (10%% off the total).\n");
            membershipDiscount = 0.9;
        }

        printf("Customer registered successfully!\n");
        (inventory->custCount)++;
    } else {
        printf("Maximum number of customers reached!\n");
    }

    printf("Enter product ID to record sales: ");
    scanf("%s", t.prod.id);

    // Search for the product with the given ID
    int foundIndex = -1;
    for (int i = 0; i < inventory->prodCount; i++) {
        if (strcmp(t.prod.id, inventory->prod[i].id) == 0) {
            foundIndex = i;
            break;
        }
    }

    if (foundIndex != -1) {
        printf("Product ID Found!\n");
        t.prod = inventory->prod[foundIndex];  // Copy product details to transaction

        printf("Enter quantity sold: ");
        scanf("%d", &(t.quantity));
        t.totalPrice = t.quantity * t.prod.price * membershipDiscount;
        inventory->prod[foundIndex].quantity -= t.quantity;
        totalSales += t.totalPrice;

        printf("Total price: %.2f\n", t.totalPrice);
        printf("\nSales recorded successfully!\n");

        // Copy customer details to transaction
        t.cust = inventory->cust[inventory->custCount - 1];

        inventory->transactions[inventory->transactionCount++] = t;
    } else {
        printf("Product ID not found!\n");
    }
}





// Function to generate reports
void generateReports(struct Inventory *inv) {
    printf("\n\nCustomer counts of the day: %i\n", inv->custCount);
    printf("Sales of the day: %.2f\n", totalSales);
    printf("----- Inventory Report -----\n");
    for (int i = 0; i < inv->prodCount; i++) {
        struct Product *prod = &(inv->prod[i]);
        printf("Product ID: %s\n", prod->id);
        printf("Name: %s\n", prod->name);
        printf("Price: %.2f\n", prod->price);
        printf("Quantity in Stock: %d\n", prod->quantity);
        printf("-----------------------------\n");
    }
}

// Function to display all prods
void displayProducts(struct Inventory *inv) {
    printf("\n\n----- Product List -----\n");

    for (int i = 0; i < inv->prodCount; i++) {
        struct Product *prod = &(inv->prod[i]);
        printf("Product ID: %s\n", prod->id);
        printf("Name: %s\n", prod->name);
        printf("Price: %.2f\n", prod->price);
        printf("Quantity: %d\n", prod->quantity);
        printf("-----------------------------\n");
    }

    printf("Total Products: %d\n", inv->prodCount);
    printf("----- End of Product List -----\n");
}

// Function to display past transactions
void displayPastTransactions(struct Inventory *inv) {
    printf("\n\n----- Past Transaction List -----\n");

    for (int i = 0; i < inv->transactionCount; i++) {
        struct Transaction *transaction = &(inv->transactions[i]);

        printf("Product ID: %s\n", transaction->prod.id);
        printf("Product Name: %s\n", transaction->prod.name);
        printf("Product Price: %.2f\n", transaction->prod.price);
        printf("Product Quantity: %d\n", transaction->prod.quantity);

        printf("Customer ID: %s\n", transaction->cust.id);
        printf("Customer Name: %s\n", transaction->cust.name);
        printf("Membership: %c\n", transaction->cust.membership);

        printf("Total Price: %.2f\n", transaction->totalPrice);

        printf("-----------------------------\n");
    }

    printf("Total Past Transactions: %d\n", inv->transactionCount);
    printf("----- End of Past Transaction List -----\n");
}



// Function to display all custs
void displayCustomers(struct Inventory *inv) {
    printf("\n\n----- Customer List -----\n");

    for (int i = 0; i < inv->custCount; i++) {
        struct Customer *cust = &(inv->cust[i]);
        printf("Customer ID: %s\n", cust->id);
        printf("Name: %s\n", cust->name);
        printf("Membership: %c\n", cust->membership);
        printf("-----------------------------\n");
    }

    printf("Total Customers: %d\n", inv->custCount);
    printf("----- End of Customer List -----\n");
}

// Function to display the menu
void displayMenu() {
    printf("\nMenu:\n");
    printf("A. Add Product\n");
    printf("B. Update Product\n");
    printf("C. Register Customer\n");
    printf("D. Display Product\n");
    printf("E. Display Customers\n");
    printf("F. Display Past Transactions\n");
    printf("G. Generate Reports\n");
    printf("Z. Exit\n");
}

