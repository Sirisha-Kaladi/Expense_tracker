#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    FOOD, TRAVEL, SHOPPING, ENTERTAINMENT, OTHERS
} Category;

typedef union {
    float amount;
    int discount;
} ExtraInfo;

typedef struct {
    int id;
    char date[20];
    Category category;
    ExtraInfo info;
    char note[50];
} Expense;

char *getCategoryName(Category c) {
    switch (c) {
        case FOOD: return "Food";
        case TRAVEL: return "Travel";
        case SHOPPING: return "Shopping";
        case ENTERTAINMENT: return "Entertainment";
        default: return "Others";
    }
}

void addExpense() {
    FILE *fp = fopen("expenses.txt", "a");
    if (fp == NULL) return;
    Expense e;
    printf("Enter ID: ");
    scanf("%d", &e.id);
    printf("Enter Date (dd-mm-yyyy): ");
    scanf("%s", e.date);
    printf("1. Food\n2. Travel\n3. Shopping\n4. Entertainment\n5. Others\nEnter Category: ");
    int ch;
    scanf("%d", &ch);
    e.category = (Category)(ch - 1);
    printf("Enter Amount: ");
    scanf("%f", &e.info.amount);
    printf("Enter Note: ");
    scanf(" %[^\n]", e.note);
    fprintf(fp, "%d %s %d %.2f %s\n", e.id, e.date, e.category, e.info.amount, e.note);
    fclose(fp);
}

void viewExpenses() {
    FILE *fp = fopen("expenses.txt", "r");
    if (fp == NULL) {
        printf("No data found.\n");
        return;
    }
    Expense e;
    printf("\n%-5s %-12s %-15s %-10s %-20s\n", "ID", "Date", "Category", "Amount", "Note");
    printf("-------------------------------------------------------------\n");
    while (fscanf(fp, "%d %s %d %f %[^\n]", &e.id, e.date, (int*)&e.category, &e.info.amount, e.note) == 5) {
        printf("%-5d %-12s %-15s %-10.2f %-20s\n", e.id, e.date, getCategoryName(e.category), e.info.amount, e.note);
    }
    fclose(fp);
}

void searchExpense() {
    FILE *fp = fopen("expenses.txt", "r");
    if (fp == NULL) return;
    int id, found = 0;
    Expense e;
    printf("Enter ID to search: ");
    scanf("%d", &id);
    while (fscanf(fp, "%d %s %d %f %[^\n]", &e.id, e.date, (int*)&e.category, &e.info.amount, e.note) == 5) {
        if (e.id == id) {
            printf("Found: %d %s %.2f %s\n", e.id, e.date, e.info.amount, e.note);
            found = 1;
            break;
        }
    }
    if (!found) printf("Expense not found.\n");
    fclose(fp);
}

void deleteExpense() {
    FILE *fp = fopen("expenses.txt", "r");
    FILE *temp = fopen("temp.txt", "w");
    if (fp == NULL || temp == NULL) return;
    int id;
    Expense e;
    printf("Enter ID to delete: ");
    scanf("%d", &id);
    while (fscanf(fp, "%d %s %d %f %[^\n]", &e.id, e.date, (int*)&e.category, &e.info.amount, e.note) == 5) {
        if (e.id != id)
            fprintf(temp, "%d %s %d %.2f %s\n", e.id, e.date, e.category, e.info.amount, e.note);
    }
    fclose(fp);
    fclose(temp);
    remove("expenses.txt");
    rename("temp.txt", "expenses.txt");
}

void editExpense() {
    FILE *fp = fopen("expenses.txt", "r");
    FILE *temp = fopen("temp.txt", "w");
    if (fp == NULL || temp == NULL) return;
    int id;
    Expense e;
    printf("Enter ID to edit: ");
    scanf("%d", &id);
    while (fscanf(fp, "%d %s %d %f %[^\n]", &e.id, e.date, (int*)&e.category, &e.info.amount, e.note) == 5) {
        if (e.id == id) {
            printf("Enter new Amount: ");
            scanf("%f", &e.info.amount);
            printf("Enter new Note: ");
            scanf(" %[^\n]", e.note);
        }
        fprintf(temp, "%d %s %d %.2f %s\n", e.id, e.date, e.category, e.info.amount, e.note);
    }
    fclose(fp);
    fclose(temp);
    remove("expenses.txt");
    rename("temp.txt", "expenses.txt");
}

void showMenu() {
    int choice;
    while (1) {
        printf("\n1. Add Expense\n2. View Expenses\n3. Search Expense\n4. Edit Expense\n5. Delete Expense\n6. Exit\nEnter choice: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1: addExpense(); break;
            case 2: viewExpenses(); break;
            case 3: searchExpense(); break;
            case 4: editExpense(); break;
            case 5: deleteExpense(); break;
            case 6: exit(0);
            default: printf("Invalid choice\n");
        }
    }
}

int main() {
    showMenu();
    return 0;
}