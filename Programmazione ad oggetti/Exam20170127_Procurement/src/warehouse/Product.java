package warehouse;

import java.util.LinkedList;
import java.util.List;
import java.util.stream.Collectors;

public class Product {
	
	private String code;
	private String description;
	private int quantity;
	private List<Supplier> suppliers = new LinkedList<>();
	private List<Order> containedInOrders = new LinkedList<>();
	
	public Product(String code, String description) {
		super();
		this.code = code;
		this.description = description;
	}

	public String getCode(){
		// TODO: completare!
		return this.code;
	}

	public String getDescription(){
		// TODO: completare!
		return this.description;
	}
	
	public void setQuantity(int quantity){
		// TODO: completare!
		this.quantity = quantity;
	}

	public void decreaseQuantity(){
		// TODO: completare!
		this.quantity--;
	}

	public int getQuantity(){
		// TODO: completare!
		return this.quantity;
	}
	
	public void addSupplier(Supplier s) {
		this.suppliers.add(s);
	}

	public List<Supplier> suppliers(){
		// TODO: completare!
		return this.suppliers;
	}

	public List<Order> pendingOrders(){
		// TODO: completare
		return containedInOrders.stream()
				.filter(o -> !o.delivered())
				.sorted((o1, o2) -> -Integer.compare(o1.getQuantity(), o2.getQuantity()))
				.collect(Collectors.toList());
	}

	public void increaseQty(int quantity) {
		// TODO Auto-generated method stub
		this.quantity += quantity;
	}

	public void setInOrder(Order order) {
		// TODO Auto-generated method stub
		this.containedInOrders.add(order);
	}

	public List<Order> getContainedOrders() {
		// TODO Auto-generated method stub
		return this.containedInOrders;
	}

	public Long containdInNOrdersAlreadyDelivered() {
		// TODO Auto-generated method stub
		return containedInOrders.stream()
				.filter(o -> o.delivered())
				.collect(Collectors.counting());
	}
}
