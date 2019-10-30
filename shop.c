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

//live order structure
struct liveOrder{
   struct ProductStock* liveList;
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
   //print the cash in the shop
   //system("@cls||clear");
   double totalCost = 0.0;
   printf("-----------------------------------------------------\n");
   printf("%s's Order:\n", c.name);
   printf("-----------------------------------------------------\n");
   printf("| Product             |  Qty | Cost Each | Line Cost |\n");
   printf("-----------------------------------------------------\n");
   //print the details of each product in the shop
   for(int i =0; i<c.index; i++){
      // calculate line cost
      double lineCost = c.shoppingList[i].quantity * c.shoppingList[i].product.price;
      //printProduct(s.stock[i].product);
      printf("| %-20.20s| %.5d| %10.2f| %10.2f|\n", c.shoppingList[i].product.name, c.shoppingList[i].quantity,c.shoppingList[i].product.price,lineCost);
      printf("-----------------------------------------------------\n");
      // add the line cost to the total
      totalCost += lineCost;
   };
   printf("%s's Total Bill :                  %.2f\n", c.name, totalCost);
   printf("-----------------------------------------------------\n");
};

struct Shop createStockShop(char* csvFile){

   //define the file
   FILE * fp;
   char * line = NULL;
   size_t len = 0;
   ssize_t read;

   // open the file
   fp = fopen(csvFile, "r");
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

struct Customer createNewCustomer(struct Shop s, char* csvFile){
//define the file
   FILE * fp;
   char * line = NULL;
   size_t len = 0;
   ssize_t read;

   // open the file
   fp = fopen(csvFile, "r");
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
   printf("-----------------------------\n");
   printf("Shop has %.2f in cash\n", s.cash);
   //deisplay a nice header
   printf("-----------------------------\n");
   printf("| Product             |  Qty |\n");
   printf("-----------------------------\n");
   //print the details of each product in the shop
   for(int i =0; i<s.index; i++){
      //printProduct(s.stock[i].product);
      printf("| %-20.20s| %.5d|\n", s.stock[i].product.name, s.stock[i].quantity);
      printf("|----------------------------|\n");
   }
};

char* findProduct(struct Shop* s, char *n){
   //take in a shop and a string
   // loop through the shop stock
   for(int j = 0; j < s->index; j++){
      // check if the item entered is in stock
      if(strcmp(s->stock[j].product.name,n)==0){
         // if so return the product name
         return s->stock[j].product.name;   
         }         
   }
   //if not return "Not in Stock"
   return "NULL";
}

void processOrder(struct Shop* s, struct Customer* c){
   // create a variable to store the total cost of the order
   double orderTotal = 0;
   // loop through the order and calculate the total cost
   for(int i = 0; i < c->index; i++){
   orderTotal = orderTotal + (c->shoppingList[i].product.price * c->shoppingList[i].quantity);
   }
   // if the total cost is greater that the budget, throw an error and terminate the transaction
   if(orderTotal > c->budget){
      printf("\nERROR: order total exceeds customer budget.\n"); 
      exit(0);
   }
   // check is each item stocked
   for(int i = 0; i < c->index; i++){
      if (strcmp(findProduct(s, c->shoppingList[i].product.name),"NULL")==0){
         printf("\nERROR: Non Stock Item, %s, on Customer Order.\n",c->shoppingList[i].product.name);
         exit(0);
      }
   }
   // check if we have each item on the order in stock
   for(int i = 0; i < c->index; i++){
      for(int j = 0; j < s->index; j++){
         if(strcmp(c->shoppingList[i].product.name,s->stock[j].product.name)==0){
            // check do we have enough in stock
            if(c->shoppingList[i].quantity > s->stock[j].quantity){
               printf("\nERROR: Not enough %s in stock to fulfil order.\n", c->shoppingList[i].product.name);
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
}

struct liveOrder createLiveOrder(struct Shop s){
   //create a structure for the order
   struct liveOrder order;
   order.liveList = malloc(sizeof(struct ProductStock) * 10);
   order.index = 0;
   // this is used a boolean operator a while loop
   char continueOrder;
   // keep accepting items until the users says no
   while(strcmp(&continueOrder, "n") != 0){
      //allocate memory for the inputs
      char* item = malloc(sizeof(char)*120);
      int requestQty;
  
      // ask what the user wants
      printf("What would you like to purchase?");
      scanf("\n%[^\n]%*c",item);
     
      // ask how many are required
      printf("How many do you need?");
      scanf("%d",&requestQty);
    
      // create a product and product stock with the information from the user input
      struct Product product = {item, getProduct(s, item).price};
      struct ProductStock listItem = {product,requestQty};

      // increment the index and put the stock item in to the stock array
      order.liveList[order.index] = listItem;
      order.index++;
      
      // ask the user do they want to enter more items
      //system("@cls||clear");
      printf("\nWould you like to order anything else? (y/n)");
      scanf("\n%s", &continueOrder);
   }
   return order;
}

void processLiveOrder(struct Shop* s, struct liveOrder lo){
   // create a variable to store the total cost of the order
   double orderTotal = 0;
   // loop through the order and calculate the total cost
   for(int i = 0; i <= lo.index; i++){  
      orderTotal = orderTotal + (lo.liveList[i].product.price * lo.liveList[i].quantity);
   }

   // check is each item stocked
   for(int i = 0; i < lo.index; i++){
      if (strcmp(findProduct(s, lo.liveList[i].product.name),"NULL")==0){
         printf("\nERROR: Non Stock Item, %s, on Customer Order.\n",lo.liveList[i].product.name);
         exit(0);
      }
   }
   // check if we have each item on the order in stock
   for(int i = 0; i < lo.index; i++){
      for(int j = 0; j < s->index; j++){
         if(strcmp(lo.liveList[i].product.name,s->stock[j].product.name)==0){
            // check do we have enough in stock
            if(lo.liveList[i].quantity > s->stock[j].quantity){
               printf("\nERROR: Not enough %s in stock to fulfil order.\n", lo.liveList[i].product.name);
               exit(0);
            }   
            //deplete the shop stock by the ordered amount
            s->stock[j].quantity -= lo.liveList[i].quantity;
            // update the cash in by the amount of the line of the order
            s->cash += lo.liveList[i].product.price * lo.liveList[i].quantity;
            }
      }
   }
   //format output
   printf("\n");
   printf("\n");
   printf("Order successfully processed, total cost of order: %.2f\n", orderTotal); 
}

void mainScreen(struct Shop s){
   int choice = 0;
   char menuReturn;
   char fileName;
    
   // print out a splash screen asking the user to choose 1,2 or 3
   printf("WELCOME TO THE SHOP\n");
   printf("\n");
   printf("Please select one of the following options\n");
   printf("\n");
   printf("\n");
   printf("1. Import and process customer order from file\n");
   printf("\n");
   printf("\n");
   printf("2. Print Shop Status\n");
   printf("\n");
   printf("\n");
   printf("3. Create an Ad-hoc Purchase\n");
	scanf("%d", &choice);
   if(choice == 1)
   {  //clear screen and ask user to enter file name
      system("@cls||clear");
      printf("Please enter the file for the order you'd like to import: \n");
      scanf("%s", &fileName);
      //create a new customer
      struct Customer newCustomer = createNewCustomer(s, &fileName);
      // process the order and format out put
      processOrder(&s, &newCustomer);
      system("@cls||clear");
      printf("\nOrder has successfully been processed.\n");
      printCustomer(newCustomer);
      printf("Enter any value to return to the main menu.\n");
      scanf("%s", &menuReturn);
      //clear screena and return to main menu
      system("@cls||clear");
      mainScreen(s);
   }

   else if(choice == 2)
   {
      //clear screen and print shop   
      system("@cls||clear");
      printShop(s);
      //format output
      printf("\n");
      printf("\n");
      printf("Enter any value to return to the main menu.\n");
      scanf("%s", &menuReturn);
      //clear screen and return to main menu
      system("@cls||clear");
      mainScreen(s);
   }
   else if(choice == 3)
   {
      // create a new live order
      struct liveOrder live = createLiveOrder(s);
      //process the order
      processLiveOrder(&s, live);
      //format the output
      printf("\n");
      printf("\n");
      printf("Enter any value to return to the main menu.\n");
      scanf("%s", &menuReturn);
      //clear screen and return to main menu
      system("@cls||clear");
      mainScreen(s);
   }
   else
   {
      // for any odd entries - return to main menu
      system("@cls||clear");
      mainScreen(s);
   }
   
}

int main(void)
{  // create a shop
   struct Shop shop = createStockShop("stock.csv");
   // clear the terminal screen
   system("@cls||clear");
   // call the main screen
   mainScreen(shop);

   return 0;
}