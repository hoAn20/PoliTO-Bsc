package delivery;

import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;
import java.util.stream.Collectors;

import delivery.Delivery.OrderStatus;

public class Order {
	
	public final static int TIME_TO_START = 5;
	public final static int TIME_TO_SHIP = 15;

	private int id;
	private int customerId;
	private Map<Integer, Integer> items;
	private Delivery.OrderStatus status;
	
	Order(int id, int customerId) {
		this.id = id;
		this.customerId = customerId;
		this.items = new HashMap<>();
		status = OrderStatus.NEW;
	}

	int getId() {
		return id;
	}

	int getCustomerId() {
		return customerId;
	}

	Map<Integer, Integer> getItems() {
		return items;
	}
	
	void addItem(int itemId, int qty) {
		if (items.containsKey(itemId))
			items.replace(itemId, items.get(itemId)+qty);
		else 
			items.put(itemId, qty);
	}

	int getQty(int i) {
		// TODO Auto-generated method stub
		return items.get(i);
	}

	List<String> getItems(Map<Integer, MenuItem> menuItems) {
		// TODO Auto-generated method stub
		return items.entrySet().stream()
				.map(e -> menuItems.get(e.getKey()).getDescription()+", "+e.getValue())
				.collect(Collectors.toList());
	}
	
	double getTotal(Map<Integer, MenuItem> menuItems) {
		double tot = 0;
		for (Entry<Integer, Integer> e: items.entrySet()) {
			tot += menuItems.get(e.getKey()).getPrice() * e.getValue();
		}
		return tot;
	}

	Delivery.OrderStatus getStatus() {
		return status;
	}

	void setStatus(Delivery.OrderStatus status) {
		this.status = status;
	}

	int getTime(Map<Integer, MenuItem> menuItems) {
		// TODO Auto-generated method stub
		return items.keySet().stream()
				.map(i -> menuItems.get(i).getPrepTime())
				.collect(Collectors.maxBy(Integer::compare)).orElse(0)
				+ TIME_TO_START + TIME_TO_SHIP;
	}
	
	
	
}
