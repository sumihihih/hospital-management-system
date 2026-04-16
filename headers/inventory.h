#ifndef INVENTORY_H
#define INVENTORY_H

void manageInventoryMenu(char currentUserName[]);
void viewInventory(void);
void viewInventoryReport(char currentUserName[]);
void searchInventory(void);
void searchInventoryByID(void);
void searchInventoryByName(void);
void filterInventory(char currentUserName[]);
void filterInventoryByCategory(char currentUserName[]);
void filterInventoryByStockLevel(char currentUserName[]);
int chooseInventoryCategory(char category[]);

#endif
