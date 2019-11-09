package shop;

import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

public class Customer {
	private String name;
	private double budget;
	private ArrayList<ProductStock> shoppingList;

	// this is used for csv files
	public Customer(String fileName, Shop sh) {
		shoppingList = new ArrayList<>();
		List<String> lines = Collections.emptyList();
		try {
			lines = Files.readAllLines(Paths.get(fileName), StandardCharsets.UTF_8);
			String[] firstLine = lines.get(0).split(",");
			name = firstLine[0];
			budget = Double.parseDouble(firstLine[1]);

			lines.remove(0);
			for (String line : lines) {
				String[] arr = line.split(",");
				String name = arr[0];
				int quantity = Integer.parseInt(arr[1].trim());
				Product p = new Product(name, sh.getProductPrice(name));
				ProductStock s = new ProductStock(p, quantity);
				shoppingList.add(s);

			}

		} catch (IOException e) {

			// do something
			e.printStackTrace();
		}

	}

	// this is used for liver orders
	public Customer(ArrayList<ProductStock> shoppingList) {
		// this is used for processing live orders
		// we don't know the customer name or budget
		// i've set the budget to -1 to skip the budget check when processing
		this.budget = -1;
		this.name = "live";
		this.shoppingList = shoppingList;

	}

	@Override
	public String toString() {
		String ret = "Customer [name=" + name + ", budget=" + budget + ", shoppingList=\n";
		for (ProductStock productStock : shoppingList) {
			ret += productStock.getProduct().getName() + " Quantity: " + productStock.getQuantity() + "\n";
		}
		return ret + "]";
	}

	public String getName() {
		return name;
	}

	public double getBudget() {
		return budget;
	}

	public void setBudget(double budget) {
		this.budget = budget;
	}

	public ArrayList<ProductStock> getShoppingList() {
		return shoppingList;
	}

	public static void main(String[] args) {

	}

}
