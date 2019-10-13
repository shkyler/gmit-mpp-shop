// The shop exercise in C adapted from the code from the lectures

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
 //a structure is a custom data type (like an object without the methods)   

//product structure
struct Product {
   // each Product has name and a price
   char* name;
   double price;
};

// product stock structure    
struct ProductStock {
   // each ProductStock has a product and a quantity
   struct Product product;
   int quantity;
};

//customer structure
struct Customer {
   // each Customer has a name, a budget and a shopping list
   char* name;
   double budget;
   struct ProductStock shoppingList[10];
   // in C we need to track the index of an array
   int index;
};

// shop structure
struct Shop {
   // a shop will have cash (this is the opening cash in the shop)
   double cash;
   // the shop will have a list of its stock
   struct ProductStock stock[20];
   // this index is used to track the shop stock array
   int index;
};

void printProduct(struct Product p){
   // take in a product and print its name and price  
   printf("PRODUCT NAME: %s\nPRODUCT PRICE: €%.2f\n", p.name, p.price);    
   printf("--------------------------\n");  
};

void printCustomer(struct Customer c){
   // take in a product and print its name and price  
   printf("CUSTOMER NAME: %s\nCUSTOMER BUDGET: €%.2f\n", c.name, c.budget);    
   printf("--------------------------\n"); 
   for(int i = 0; i < c.index; i++){
      printProduct(c.shoppingList[i].product);
      printf("%s ORDERS %d OF ABOVE PRODUCT\n", c.name, c.shoppingList[i].quantity);
      double cost = c.shoppingList[i].quantity * c.shoppingList[i].product.price;
      printf("The cost to %s will be €%.2f\n", c.name, cost);
   };
};

struct Shop createStockShop(){
      // initialise the shop with 200 in cash
      struct Shop shop = {200};

      //define the file
      FILE * fp;
      char * line = NULL;
      size_t len = 0;
      ssize_t read;

      fp = fopen("stock.csv", "r");
      if (fp == NULL)
        exit(EXIT_FAILURE);

      while ((read = getline(&line, &len, fp)) != -1) {
        // read the name as 'n'    
        char* n = strtok(line, ",");
        // read the price as 'p'
        char* p = strtok(NULL, ",");
        // read the quantiy as 'q'
        char* q= strtok(NULL, ",");
        // convert 'q' to int 'quantity'
        int quantity = atoi(q);
        // convert 'p' to double 'price'
        double price = atof(p);
        // allocate memory yto the name and copy in 'n'
        char* name = malloc(sizeof(char) * 50);
        strcpy(name,n);
        // create a product and product stock with the information from the file 
        struct Product product = {name,price};
        struct ProductStock stockItem = {product,quantity};
        // increment the index and put the stock item in to the stock array
        shop.stock[shop.index++] = stockItem;
        //printf("NAME OF PRODUCT: %s, PRICE: %.2f, QUANTITY: %d\n", name, price, quantity);
    }
    return shop; 
};

void printShop(struct Shop s){
   //print the cash in the shop
   printf("Shop has %.2f in cash\n", s.cash);
   //print the details of each product in the shop
   for(int i =0; i<s.index; i++){
      printProduct(s.stock[i].product);
      printf("The shop has %d of the above\n", s.stock[i].quantity);
   }
};

int main(void)
{
   // test the code
   //struct Customer dominic = {"Dominic", 100.0};
   //struct Product coke = {"Can Coke", 1.10};
   //struct Product bread = {"Bread", 0.7};
   //struct ProductStock cokeStock = {coke, 20};
   //struct ProductStock breadStock = {bread, 2};
   //printProduct(coke);
   //printCustomer(dominic);

   //dominic.shoppingList[dominic.index++] = cokeStock;
   //dominic.shoppingList[dominic.index++] = breadStock;
   //printCustomer(dominic);
   //printf("Customer name is %s\n", dominic.name); 
   //printf("The %s costs €%.2f\n", coke.name, coke.price);  
   //printf("The shop has %d of the product %s\n", cokeStock.quantity, cokeStock.product.name );
   struct Shop shop = createStockShop();
   printShop(shop);
   return 0;
}