package shop;

import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.*;

public class Shop {
	// a shop has a cash float and list of stock items
	private double cash;
	private ArrayList<ProductStock> stock;

	// a shop is created by reading information in from a file
	public Shop(String fileName) {
		stock = new ArrayList<>();
		List<String> lines = Collections.emptyList();
		try {
			lines = Files.readAllLines(Paths.get(fileName), StandardCharsets.UTF_8);
			// System.out.println(lines.get(0));
			cash = Double.parseDouble(lines.get(0));
			// i am removing at index 0 as it is the only one treated differently
			lines.remove(0);
			for (String line : lines) {
				String[] arr = line.split(",");
				String name = arr[0];
				double price = Double.parseDouble(arr[1]);
				int quantity = Integer.parseInt(arr[2].trim());
				Product p = new Product(name, price);
				ProductStock s = new ProductStock(p, quantity);
				stock.add(s);

			}

		}

		catch (IOException e) {

			// do something
			e.printStackTrace();
		}
	}

	// a method to export shop details to a string
	@Override
	public String toString() {
		return "Shop [cash=" + cash + ", stock=" + stock + "]";
	}

	// a method to get the cash value in the shop
	public double getCash() {
		return cash;
	}

	// a method to return a product price from the stock list by name
	public double getProductPrice(String productName) {
		// take in a product name and iterate through the shop stock
		for (ProductStock productStock : stock) {
			// if the item is in the stock shop return the shop price for the item
			if (productStock.getProduct().getName().contentEquals(productName)) {
				return productStock.getProduct().getPrice();
			}
		}
		// otherwise return 0
		return 0.0;
	}

	// method to return a product name from the stock list by name
	public String findProduct(String product) {
		// take in a shop and a string
		// loop through the shop stock
		for (ProductStock productStock : stock) {
			// check if the item entered is in stock
			if (productStock.getProduct().getName().equals(product)) {
				// if so return the product name
				return productStock.getProduct().getName();
			}
		}
		// if not return "Not in Stock"
		return "NULL";
	}

	// method to get the stock list from the shop
	public ArrayList<ProductStock> getStock() {
		return stock;
	}

	// method to update the cash value in the shop
	public void setCash(double cash) {
		this.cash = cash;
	}

	// method to update the stock list in the shop
	public void setStock(ArrayList<ProductStock> stock) {
		this.stock = stock;
	}

	// print formatted customer order details
	public static void printCustomer(Customer c) {
		// store the running total
		double totalCost = 0.0;
		// print the header
		System.out.printf("-----------------------------------------------------\n");
		System.out.printf("%s's Order:\n", c.getName());
		System.out.printf("-----------------------------------------------------\n");
		System.out.printf("| Product             |  Qty | Unit Cost | Line Cost |\n");
		System.out.printf("-----------------------------------------------------\n");
		// print the details of each product in the shop
		for (ProductStock productStock : c.getShoppingList()) {
			System.out.printf("| %-20.20s| %5d| %10.2f| %10.2f| \n", productStock.getProduct().getName(),
					productStock.getQuantity(), productStock.getProduct().getPrice(),
					(productStock.getQuantity() * productStock.getProduct().getPrice()));
			System.out.printf("-----------------------------------------------------\n");
			totalCost += productStock.getQuantity() * productStock.getProduct().getPrice();
		}
		// print the footer and bill total
		System.out.printf("%s's Total Bill :                            %.2f\n", c.getName(), totalCost);
		System.out.printf("-----------------------------------------------------\n");
	}

	// print formatted shop details
	public static void printShop(Shop s) {
		// print the cash in the shop
		System.out.printf("-----------------------------\n");
		System.out.printf("Shop has %.2f in cash\n", s.getCash());
		// display a nice header
		System.out.printf("-----------------------------\n");
		System.out.printf("| Product             |  Qty |\n");
		System.out.printf("-----------------------------\n");
		// print the details of each product in the shop
		for (ProductStock productStock : s.getStock()) {
			System.out.printf("| %-20.20s| %5d|\n", productStock.getProduct().getName(), productStock.getQuantity());
			System.out.printf("|----------------------------|\n");
		}
	}

	// process a customer order
	public void processOrder(Customer c) {
		// create a variable to store the total cost of the order
		double orderTotal = 0;
		// loop through the order and calculate the total cost
		for (ProductStock productStock : c.getShoppingList()) {
			orderTotal = orderTotal + (productStock.getProduct().getPrice() * productStock.getQuantity());
		}
		// budget will greater than 0 for csv orders and less than 0 for live
		if (c.getBudget() >= 0) {
			// if the total cost is greater that the budget, throw an error and terminate
			// the transaction
			if (orderTotal > c.getBudget()) {
				System.out.println("ERROR: order total exceeds customer budget.");
				return;
			}
		}
		// check is each item stocked
		for (ProductStock productStock : c.getShoppingList()) {
			if (findProduct(productStock.getProduct().getName()).equals("NULL")) {
				System.out.printf("\nERROR: Non Stock Item, %s, on Customer Order.\n",
						productStock.getProduct().getName());
				return;
			}

		}
		// check if we have each item on the order in stock
		for (ProductStock productStock : c.getShoppingList()) {
			for (ProductStock shopStock : stock) {
				if (productStock.getProduct().getName().equals(shopStock.getProduct().getName())) {
					// check do we have enough in stock
					if (productStock.getQuantity() > shopStock.getQuantity()) {
						System.out.printf("\nERROR: Not enough %s in stock to fulfil order.\n",
								productStock.getProduct().getName());
						return;
					}
					// deplete the shop stock by the ordered amount
					shopStock.setQuantity(shopStock.getQuantity() - productStock.getQuantity());

				}

			}

		}
		// update the cash in the shop
		setCash(getCash() + orderTotal);
		// update the customer budget
		c.setBudget(c.getBudget() - orderTotal);
		// print customer order details
		printCustomer(c);
	}

	
	// this is a method for clearing the screen
	// note that it is not possible to clear the sceen in the Eclipse console
	// This works in all other terminals
	public final void cls() {
		try {
			if (System.getProperty("os.name").contains("Windows")) {
				new ProcessBuilder("cmd", "-c", "cls").inheritIO().start().waitFor();
			} else {
				// new ProcessBuilder("bash","-c","clear").inheritIO().start().waitFor();

				final String ANSI_CLS = "\u001b[2J";
				final String ANSI_HOME = "\u001b[H";
				System.out.print(ANSI_CLS + ANSI_HOME);
				System.out.flush();
			}
		} catch (final Exception e) {

			throw new RuntimeException(e);
		}
	}

	public static void mainScreen(Shop s) {
		int choice = 0;
		String menuReturn;
		String fileName;

		// create the scanner to take in user input
		Scanner scan = new Scanner(System.in);

		// print out a splash screen asking the user to choose 1,2 or 3
		System.out.printf("WELCOME TO THE SHOP\n");
		System.out.printf("\n");
		System.out.printf("Please select one of the following options\n");
		System.out.printf("\n");
		System.out.printf("\n");
		System.out.printf("1. Import and process customer order from file\n");
		System.out.printf("\n");
		System.out.printf("\n");
		System.out.printf("2. Print Shop Status\n");
		System.out.printf("\n");
		System.out.printf("\n");
		System.out.printf("3. Create an Ad-hoc Purchase\n");
		System.out.printf("\n");
		System.out.printf("\n");
		System.out.printf("4. Exit Progam\n");

		// take the choice in a a string and convert to integer
		// this is done because .nextInt() behaves strangely!
		try {
			choice = Integer.parseInt(scan.nextLine());
		} catch (NumberFormatException e) {
			// if a non integer is entered - call the main screen
			mainScreen(s);
		}

		if (choice == 1) { // clear screen and ask user to enter file name
			s.cls();
			System.out.printf("Please enter the file for the order you'd like to import: \n");
			fileName = scan.nextLine();
			System.out.println(fileName);

			// create a new customer
			Customer newCustomer = new Customer("src/shop/" + fileName, s);

			// process the order
			s.processOrder(newCustomer);

			// hold the output until the user sees it
			System.out.printf("Enter any value to return to the main menu.\n");
			menuReturn = scan.nextLine();

			// clear screena and return to main menu
			s.cls();
			mainScreen(s);
		}

		else if (choice == 2) {
			// clear screen and print shop
			s.cls();
			printShop(s);
			// format output
			System.out.printf("\n");
			System.out.printf("\n");
			System.out.printf("Enter any value to return to the main menu.\n");
			menuReturn = scan.nextLine();
			// clear screen and return to main menu
			s.cls();
			mainScreen(s);
		}

		else if (choice == 3) { // clear screen
			s.cls();
			ArrayList<ProductStock> liveList = new ArrayList<>();

			// create the scanner to take in user input
			scan = new Scanner(System.in);

			// this is used a boolean operator in a while loop
			String continueOrder = "y";
			// keep accepting items until the users says no
			while (!continueOrder.equals("n")) {
				String item;
				int requestQty = 0;
				// ask what the user wants
				System.out.printf("What would you like to purchase?");
				item = scan.nextLine();

				// ask how many are required
				System.out.printf("How many do you need?");

				// take the choice in a a string and convert to integer
				// this is done because .nextInt() behaves strangely!
				try {
					requestQty = Integer.parseInt(scan.nextLine());
				} catch (NumberFormatException e) {
					// if a non integer is entered - call the main screen
					System.out.println("Error");
				}

				Product p = new Product(item, s.getProductPrice(item));
				ProductStock ps = new ProductStock(p, requestQty);

				liveList.add(ps);

				// ask the user do they want to enter more items
				s.cls();
				System.out.printf("\nWould you like to order anything else? (y/n)");
				continueOrder = scan.nextLine();
			}
			// create a new customer
			Customer newCustomer = new Customer(liveList);
			s.processOrder(newCustomer);
			System.out.printf("\n");
			System.out.printf("\n");
			System.out.printf("Enter any value to return to the main menu.\n");
			menuReturn = scan.nextLine();
			// clear screen and return to main menu
			s.cls();
			mainScreen(s);

		}

		else if (choice == 4) {
			// quit program
			s.cls();
			System.exit(0);
		} else {
			// for any odd entries - return to main menu
			s.cls();
			mainScreen(s);
		}

	}

	public static void main(String[] args) {
		// create a shop
		Shop shop = new Shop("src/Shop/stock.csv");
		// clear the screen and run the mainScreen
		shop.cls();
		mainScreen(shop);

	}

}