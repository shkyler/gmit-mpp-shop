package shop;

public class ProductStock {
	private Product product;
	private int quantity;

	public ProductStock(Product product, int quantity) {
		super();
		this.product = product;
		this.quantity = quantity;
	}

	public void setQuantity(int quantity) {
		this.quantity = quantity;
	}

	public Product getProduct() {
		return product;
	}

	public int getQuantity() {
		return quantity;
	}

	@Override
	public String toString() {
		return "ProductStock [product=" + product + ", quantity=" + quantity + "]";
	}

}
