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

struct Product getProduct(struct Shop s, char* pname){
   // check the shop for a product and return it
   struct Product p;
   for (int i = 0; i < s.index; i++){
      if(strcmp(s.stock[i].product.name,pname)==0){
        p = s.stock[i].product;
      }
   }
   return p;
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

   //define the file
   FILE * fp;
   char * line = NULL;
   size_t len = 0;
   ssize_t read;

   // open the file
   fp = fopen("stock.csv", "r");
   // if the file doesn't exist - throw an error
   if (fp == NULL)
     exit(EXIT_FAILURE);

   // read the first line from the file
   double line1 = getline(&line,&len,fp);
   char* c = strtok(line, ",");
   double cash = atof(c);
   // initialise the shop with the cash from the first line in the file
   struct Shop shop = {cash};
   //loop through the remainder for the file and create the stock
   while ((read = getline(&line, &len, fp)) != -1) {
     //printf("read: %ld\n", read); 
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
     // allocate memory to the name and copy in 'n'
     char* name = malloc(sizeof(char) * 50);
     strcpy(name,n);
     // create a product and product stock with the information from the file 
     struct Product product = {name,price};
     struct ProductStock stockItem = {product,quantity};
     // increment the index and put the stock item in to the stock array
     shop.stock[shop.index++] = stockItem;
    }
    return shop; 
};

struct Customer createNewCustomer(struct Shop s){
//define the file
   FILE * fp;
   char * line = NULL;
   size_t len = 0;
   ssize_t read;

   // open the file
   fp = fopen("order.csv", "r");
   // if the file doesn't exist - throw an error
   if (fp == NULL)
     exit(EXIT_FAILURE);

   // read the first line from the file
   double line1 = getline(&line,&len,fp);
   //read the customer name as cn
   char* cn = strtok(line, ",");
   // read the customer budget as cb
   char* cb = strtok(NULL, ",");

   // allocate memory to the customer name and copy in 'cn'
   char* custName = malloc(sizeof(char) * 50);
   strcpy(custName,cn);
   // convert the budget to a double
   double custBudget = atof(cb);

   // initialise the customer 
   struct Customer customer = {custName,custBudget};

   //loop through the remainder for the file and create the shopping list
   while ((read = getline(&line, &len, fp)) != -1) {
     // read the product name as 'pn'    
     char* pn = strtok(line, ",");
     // read the productquantiy as 'qn'
     char* qn = strtok(NULL, ",");
     // convert 'qn' to int 'ordQuantity'
     int ordQuantity = atoi(qn);
     // allocate memory yto the product name and copy in 'n'
     char* prodName = malloc(sizeof(char) * 50);
     strcpy(prodName,pn);
     // create a product and product stock with the information from the file 
     struct Product product = {prodName,getProduct(s, prodName).price};
     struct ProductStock listItem = {product,ordQuantity};
     // increment the index and put the stock item in to the stock array
     customer.shoppingList[customer.index++] = listItem;
    }
    return customer;
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

void processOrder(struct Shop* s, struct Customer* c){
   // create a variable to store the total cost of the order
   double orderTotal = 0;
   // loop through the order and calculate the total cost
   for(int i = 0; i < c->index; i++){
   orderTotal = orderTotal + (c->shoppingList[i].product.price * c->shoppingList[i].quantity);
   }
   // if the total cost is greater that the budget, throw an error and terminate the transaction
   if(orderTotal > c->budget){
      printf("ERROR: order total exceeds customer budget.\n"); 
      exit(0);
   }
   // check if we have each item on the order in stock
   for(int i = 0; i < c->index; i++){
      for(int j = 0; j < s->index; j++){
         if(strcmp(c->shoppingList[i].product.name,s->stock[j].product.name)==0){
            // check do we have enough in stock
            if(c->shoppingList[i].quantity > s->stock[j].quantity){
               printf("ERROR: Not enough %s in stock to fulfil order.\n", c->shoppingList[i].product.name);
               exit(0);
            }   
            //deplete the shop stock by the ordered amount
            s->stock[j].quantity -= c->shoppingList[i].quantity;
            // update the cash in by the amount of the line of the order
            s->cash += c->shoppingList[i].product.price * c->shoppingList[i].quantity;
            // deplete the budget by the same amount
            c->budget -= c->shoppingList[i].product.price * c->shoppingList[i].quantity; 
            }
      }
   }
   //printf("Test: %.2f\n", orderTotal);  
};


int main(void)
{  // create a shop and customer
   struct Shop shop = createStockShop();
   struct Customer newCustomer = createNewCustomer(shop);
   processOrder(&shop, &newCustomer);
   return 0;
}