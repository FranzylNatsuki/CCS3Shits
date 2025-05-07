/*
Name: Franzyl Bjorn L. Macalua
Problem is a modified version of CP6, using an extra pointer making it a doubly linked-list and to adjust logic accordingly.
Date Created: April 26, 2025
Data Finished: April 27, 2025
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#define SIZE 10

typedef struct groceryItems {
	int itemCode;
	char itemName[15];
	char manufacturer[15];
	char productType[15];
	float price;
	int currentStock;
} 	GROCERY;

void menu(void);
int menuChoice(void);
void createFile(FILE *fp);
void addRecord(FILE *fp, GROCERY record);
void editRecord(FILE *fp, FILE *tempf, GROCERY record);
void typeSearch(FILE *fp, FILE *tempf, GROCERY record);
void nameSearch(FILE *fp, FILE *tempf, GROCERY record);
void DisplayAllRecords(FILE *fp, GROCERY record);
void dataEntry(FILE *fp,  GROCERY record, int *itemCode, char itemName[15], char manufacturer[15], char productType[15], float *price, int *currentStock);
void displayType(FILE *fp, GROCERY record);
void displayManufacturer(FILE *fp, GROCERY record);

void menu(void) {
	printf("MAIN MENU\n");
	printf("1. Add a Record\n");
	printf("2. Edit a Record\n");
	printf("3. Delete a Record\n");
	printf("4. Display All Records of a particular type\n");
	printf("5. Display All Records from a particular company\n");
	printf("6. Display All Records\n");
	printf("7. Exit the program\n");
}

int menuChoice(void){
	int choice;
	scanf("%d", &choice);
	return choice;
}

void createFile(FILE *fp) {
    char choice;
    fp = fopen("grocery.dat", "rb");

    if (fp != NULL) {
    // checks if there is a file already existing
        fclose(fp);

        printf("The file already exists! Would you like to re-create the file? (Re-creation will delete all information) Y/n");
        scanf(" %c", &choice);

        if (choice == 'Y' || choice == 'y') {
            fp = fopen("grocery.dat", "wb");
            if (fp == NULL) {
                printf("Error Creating File!\n");
                exit(1);
            }
            printf("File has been re-created\n");
            fclose(fp);
            return;
        }
    }
    // if no file exists
    fp = fopen("grocery.dat", "wb");
    if (fp == NULL) {
        printf("Error Creating File!\n");
        exit(1);
    }
    printf("File has been created!\n");
    fclose(fp);
}

void addRecord(FILE *fp, GROCERY record) {
    int CODE;
	char NAME[15];
	char MANU[15];
	char TYPE[15];
	float PRICE;
	int STOCK;

    fp = fopen("grocery.dat", "ab");
    if (fp == NULL) {
        printf("File Error!");
        return;
    }

    dataEntry(fp, record, &CODE, NAME, MANU, TYPE, &PRICE, &STOCK);

    record.itemCode = CODE;
    strcpy(record.itemName, NAME);
    strcpy(record.manufacturer, MANU);
    strcpy(record.productType, TYPE);
    record.price = PRICE;
    record.currentStock = STOCK;

    fwrite(&record, sizeof(GROCERY), 1, fp);
    printf("Record added!");
    fclose(fp);
}

void editRecord(FILE *fp, FILE *tempf, GROCERY record) {
	int choice;

	fp = fopen("grocery.dat", "rb");

	if (fp == NULL) {
		puts("Sorry, there are no records in the list. Press any key to continue");
		getch();
		system("cls");
		return;
	}

	fclose(fp);
	do {
		printf("Search via Item Code or Item Name:\n");
		printf("1. Item Type:\n");
		printf("2. Item Name:\n");
		scanf("%d", &choice);
		switch (choice) {
			case 1:
				typeSearch(fp, tempf, record);
				break;
			case 2:
				nameSearch(fp, tempf, record);
				break;
			default:
				printf("Invalid Option, press [Enter] to retry");
				getch();
				system("cls");
		}
	} while (choice < 1 || choice > 2);
}

void typeSearch(FILE *fp, FILE *tempf, GROCERY record){
	fp = fopen("grocery.dat", "rb");
	tempf = fopen("grocerytemp.dat", "wb");

	char search[15];
	char confirmation;
	int i, searchcheck, found = 0;;

	if (fp == NULL) {
		puts("Sorry, there are no records in the list. Press any key to continue");
		getch();
		system("cls");
		return;
	}

	printf("Item Type:");
	getchar();
    fgets(search, sizeof(search), stdin);
    search[strcspn(search, "\n")] = '\0';

	printf("Item Type: %s\n", search);
	printf("%-15s%-15s%-15s%-15s%-15s%-15s\n", "Item Code", "Item Name", "Manufacturer", "Product Type", "Price", "Stock");

	while (fread(&record, sizeof(GROCERY), 1, fp)) {
		searchcheck = strcmpi(record.productType, search);
		if (searchcheck == 0) {
			printf("%-15d%-15s%-15s%-15s%-15.2f%-15d\n", record.itemCode, record.itemName, record.manufacturer, record.productType, record.price, record.currentStock);
			found += 1;
		}
	}

	if (found == 1) {
		int choice;
		do {
			printf("Edit Element:\n");
			printf("1 Price:\n");
			printf("2 Stock:\n");
			scanf("%d", &choice);
			getchar();
			if (choice < 1 || choice >2) {
				printf("Invalid Input, Press [Enter] to Retry");
				getch();
				system("cls");
			}
		} while (choice < 1 || choice >2);

		if (choice == 1) {
			float price;
			do {
				printf("Price:");
				scanf("%f", &price);
				getchar();
				if (price <= 0) {
					printf("Price should not be zero");
					getch();
					system("cls");
				}
			} while (price <= 0);

			puts("The new data will be saved. Do you want to continue (Y/N)?");
			scanf(" %c", &confirmation);

			if (confirmation == 'Y' || confirmation == 'y') {
				record.price = price;
				system("cls");
			}
			else {
				system("cls");
				return;
			}
		}

		else if (choice == 2) {
			int stock;
			printf("Stock:");
			scanf("%d", &stock);
			getchar();

			puts("The new data will be saved. Do you want to continue (Y/N)?");
			scanf(" %c", &confirmation);

			if (confirmation == 'Y' || confirmation == 'y') {
				record.currentStock = stock;
			}
			else {
				system("cls");
				return;
			}
		}
	}

	else if (found > 1) {
		fflush(stdin);
		nameSearch(fp, tempf, record);
	}

	else if (found == 0) {
		printf("No Result Found. Press [Enter] to proceed");
		getch();
		system("cls");
	}

	rewind(fp);
	rewind(tempf);

	while (fread(&record, sizeof(GROCERY), 1, fp)) {
        fwrite(&record, sizeof(GROCERY), 1, tempf);
    }

    fclose(fp);
    fclose(tempf);

    fp = fopen("grocery.dat", "w+b");
	tempf = fopen("grocerytemp.dat", "rb");

	while (fread(&record, sizeof(GROCERY), 1, tempf)) {
        fwrite(&record, sizeof(GROCERY), 1, fp);
    }

    fclose(fp);
    fclose(tempf);
    system("cls");
}

/*
void nameSearch(FILE *fp, GROCERY record){
	char search[15];
	char confirmation;
    int i, searchcheck, found = 0;
    nd p = *head;

	if (*head == NULL) {
		puts("Sorry, there are no records in the list. Press any key to continue");
		getch();
		system("cls");
		return;
	}

    printf("Item Name: ");
    fflush(stdin);
    fgets(search, sizeof(search), stdin);
    search[strcspn(search, "\n")] = '\0';

	printf("Item Name: %s\n", search);
	printf("%-15s%-15s%-15s%-15s%-15s%-15s\n", "Item Code", "Item Name", "Manufacturer", "Product Type", "Price", "Stock");
	// use just one pointer because I know there will be no duplicates as its trapped already
	while (p != NULL) {
		searchcheck = strcmpi(p->itemName, search);
		if (searchcheck == 0) {
			printf("%-15d%-15s%-15s%-15s%-15.2f%-15d\n", p->itemCode, p->itemName, p->manufacturer, p->productType, p->price, p->currentStock);
			found = 1;
			break;
		}
		p = p->next;
	}

	if (found == 1) {
		int choice;
		do {
			printf("Edit Element:\n");
			printf("1 Price:\n");
			printf("2 Stock:\n");
			scanf("%d", &choice);
			if (choice < 1 || choice >2) {
				printf("Invalid Input, Press [Enter] to Retry");
				getch();
				system("cls");
			}
		} while (choice < 1 || choice > 2);

		if (choice == 1) {
			float price;
			do {
				printf("Price:");
				scanf("%f", &price);
				if (price <= 0) {
					printf("Price should not be zero");
					getch();
					system("cls");
				}
				system("cls");
			} while (price <= 0);

			puts("The new data will be saved. Do you want to continue (Y/N)?");
			scanf(" %c", &confirmation);

			if (confirmation == 'Y' || confirmation == 'y') {
				p->price = price;
				system("cls");
			}
			else {
				system("cls");
				return;
			}
		}

		if (choice == 2) {
			int stock;
			printf("Stock:");
			scanf("%d", &stock);
			getchar();

			puts("The new data will be saved. Do you want to continue (Y/N)?");
			scanf(" %c", &confirmation);

			if (confirmation == 'Y' || confirmation == 'y') {
				p->currentStock = stock;
				system("cls");
			}
			else {
				system("cls");
				return;
			}
		}
	}

	if (found == 0) {
		printf("No Result Found. Press [Enter] to proceed");
		getch();
		system("cls");
	}
}
*/

void DisplayAllRecords(FILE *fp, GROCERY record) {
    fp = fopen("grocery.dat", "rb");
    if (fp == NULL) {
		puts("Sorry, there are no records in the list. Press any key to continue");
		getch();
		system("cls");
		return;
	}

	printf("Displaying all records:\n");
	printf("%-15s%-15s%-15s%-15s%-15s%-15s\n", "Item Code", "Item Name", "Manufacturer", "Product Type", "Price", "Stock");

	while (fread(&record, sizeof(GROCERY), 1, fp)) {
		printf("%-15d%-15s%-15s%-15s%-15.2f%-15d\n", record.itemCode, record.itemName, record.manufacturer, record.productType, record.price, record.currentStock);
	}

	printf("Press [Enter] to proceed!");
  	getch();
  	system("cls");

  	fclose(fp);
}

void dataEntry(FILE *fp, GROCERY record, int *itemCode, char itemName[15], char manufacturer[15], char productType[15], float *price, int *currentStock) {
	fp = fopen("grocery.dat", "rb");

    int ITEMCODE;
	char ITEMNAME[15];
	char MANUFACTURER[15];
	char TYPE[15];
	float PRICE;
	int STOCK;
	int duplicateCheck = 0;
	// same trappings as the previous work
	do {
        duplicateCheck = 0;
	    printf("Item Code (10000 - 99999):");
	    scanf("%d", &ITEMCODE);
	    getchar();

		while (fread(&record, sizeof(GROCERY), 1, fp)) {
			if (ITEMCODE == record.itemCode) {
				duplicateCheck = 1;
				break;
			}
		}

	    if (ITEMCODE < 10000 || ITEMCODE > 99999) {
	        printf("Item Code not in range, press enter to retry");
	        getch();
	        system("cls");
	    }

		else if (duplicateCheck == 1) {
			printf("Already an existing item code, press enter to retry");
			getch();
			system("cls");
		}
	} while (ITEMCODE < 10000 || ITEMCODE > 99999 || duplicateCheck == 1);

	*itemCode = ITEMCODE;
	system("cls");

	do {
        duplicateCheck = 0;
	    printf("Item Name:");
	    gets(ITEMNAME);

	    while (fread(&record, sizeof(GROCERY), 1, fp)) {
			if (strcmpi(ITEMNAME, record.itemName) == 0) {
				duplicateCheck = 1;
				break;
			}
		}

	    if (strlen(ITEMNAME) == 0) {
	        printf("Item Name should not be blank");
	        getch();
	        system("cls");
	    }

        else if (duplicateCheck == 1) {
			printf("Already an existing item name, press enter to retry");
			getch();
			system("cls");
		}
	} while (strlen(ITEMNAME) == 0 || duplicateCheck == 1);

	strcpy(itemName, ITEMNAME);
	system("cls");
	fflush(stdin);

	do {
		printf("Manufacturer:");
	    gets(MANUFACTURER);

	    if (strlen(MANUFACTURER) == 0) {
	        printf("Manufacturer should not be blank");
	        getch();
	        system("cls");
	    }
	} while (strlen(MANUFACTURER) == 0);

	strcpy(manufacturer, MANUFACTURER);
	system("cls");
	fflush(stdin);

	do {
		printf("Type:");
	    gets(TYPE);

	    if (strlen(TYPE) == 0) {
	        printf("Item Type should not be blank");
	        getch();
	        system("cls");
	    }
	} while (strlen(TYPE) == 0);

	strcpy(productType, TYPE);
	system("cls");
	fflush(stdin);

    do {
		printf("Price:");
		scanf("%f", &PRICE);
		if (PRICE <= 0) {
			printf("Price should not be zero");
			getch();
			system("cls");
		}
	} while (PRICE <= 0);

	*price = PRICE;
	system("cls");

    printf("Stock:");
	scanf("%d", &STOCK);
	*currentStock = STOCK;
	system("cls");

	printf("Added Record. Press [Enter] to proceed!");
	getch();
	system("cls");
}

void displayType(FILE *fp, GROCERY record) {
	char search[15];
	int i, searchcheck, found = 0;

    fp = fopen("grocery.dat", "rb");
    if (fp == NULL) {
		puts("Sorry, there are no records in the list. Press any key to continue");
		getch();
		system("cls");
		return;
	}

	printf("Item Type:");
	getchar();
	fflush(stdin);
    gets(search);

	printf("Item Type: %s\n", search);
	printf("%-15s%-15s%-15s%-15s%-15s%-15s\n", "Item Code", "Item Name", "Manufacturer", "Product Type", "Price", "Stock");

	while (fread(&record, sizeof(GROCERY), 1, fp)) {
		searchcheck = strcmpi(record.productType, search);

		if (searchcheck == 0) {
            printf("%-15d%-15s%-15s%-15s%-15.2f%-15d\n", record.itemCode, record.itemName, record.manufacturer, record.productType, record.price, record.currentStock);
			found++;
		}
	}

	if (found == 0) {
		printf("No Result Found. Press [Enter] to proceed");
		getch();
		system("cls");
		return;
	}

	getch();
	system("cls");

	fclose(fp);
}

void displayManufacturer(FILE *fp, GROCERY record) {
	char search[15];
	int i, searchcheck, found = 0;

    fp = fopen("grocery.dat", "rb");
    if (fp == NULL) {
		puts("Sorry, there are no records in the list. Press any key to continue");
		getch();
		system("cls");
		return;
	}

	printf("Manufacturer:");
	getchar();
	fflush(stdin);
    gets(search);

	printf("Manufacturer: %s\n", search);
	printf("%-15s%-15s%-15s%-15s%-15s%-15s\n", "Item Code", "Item Name", "Manufacturer", "Product Type", "Price", "Stock");

	while (fread(&record, sizeof(GROCERY), 1, fp)) {
		searchcheck = strcmpi(record.manufacturer, search);

		if (searchcheck == 0) {
            printf("%-15d%-15s%-15s%-15s%-15.2f%-15d\n", record.itemCode, record.itemName, record.manufacturer, record.productType, record.price, record.currentStock);
			found++;
		}
	}

	if (found == 0) {
		printf("No Result Found. Press [Enter] to proceed");
		getch();
		system("cls");
		return;
	}

	getch();
	system("cls");
	fclose(fp);
}

int main(void) {
	int choice;
    FILE *fp = NULL;
    FILE *tempf = NULL;
    GROCERY record;

    createFile(fp);

	do {
	    menu();
	    choice = menuChoice();
	    switch (choice) {
	      	case 1:
		        addRecord(fp, record);
		        break;
	      	case 2:
	      		editRecord(fp, tempf, record);
		        break;
	      	case 3:
	      		//deleteRecord(fp, record);
		        break;
	      	case 4:
	      		displayType(fp, record);
		        break;
	      	case 5:
	      		displayManufacturer(fp, record);
		        break;
	      	case 6:
		        DisplayAllRecords(fp, record);
		        break;
		    case 7:
		    	return 0;
		    	break;
	    }
		if (choice != 7 && (choice < 1 || choice > 7)) {
			printf("Invalid Input, Press [Enter] to Retry");
			getch();
			system("cls");
		}
	} while (choice != 7);
	return 0;
}

