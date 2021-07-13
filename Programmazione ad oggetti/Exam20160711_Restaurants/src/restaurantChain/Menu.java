package restaurantChain;

public class Menu {
	
	private String name;
	private double price;

	public Menu(String name, double price) {
		// TODO Auto-generated constructor stub
		this.name = name;
		this.price = price;
	}

	String getName() {
		return name;
	}

	void setName(String name) {
		this.name = name;
	}

	double getPrice() {
		return price;
	}

	void setPrice(double price) {
		this.price = price;
	}

}
