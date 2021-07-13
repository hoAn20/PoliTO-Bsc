package delivery;

public class MenuItem {
	
	private int id;
	private String description;
	private double price;
	private String category;
	private int prepTime;
	
	public MenuItem(int id, String description, double price, String category, int prepTime) {
		super();
		this.id = id;
		this.description = description;
		this.price = price;
		this.category = category;
		this.prepTime = prepTime;
	}
	
	int getId() {
		return this.id;
	}

	String getDescription() {
		return description;
	}

	double getPrice() {
		return price;
	}

	String getCategory() {
		return category;
	}

	int getPrepTime() {
		return prepTime;
	}
	
	@Override
	public String toString() {
		// TODO Auto-generated method stub
		return "["+this.category+"] "+this.description+" : "+String.format("%.2f", this.price);
	}

}
