package warehouse;

import java.util.Collection;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.stream.Collectors;

public class Warehouse {
	
	private static int ordN = 1;
	
	private Map<String, Product> products = new HashMap<>();
	private Map<String, Supplier> suppliers = new HashMap<>();
	private Map<String, Order> orders = new HashMap<>();
	
	public Product newProduct(String code, String description){
		// TODO: completare
		products.put(code, new Product(code, description));
		return products.get(code);
	}
	
	public Collection<Product> products(){
		// TODO: completare
		return products.values();
	}

	public Product findProduct(String code){
		// TODO: completare
		return products.get(code);
	}

	public Supplier newSupplier(String code, String name){
		// TODO: completare
		suppliers.put(code, new Supplier(code, name));
		return suppliers.get(code);
	}
	
	public Supplier findSupplier(String code){
		// TODO: completare
		return suppliers.get(code);
	}

	public Order issueOrder(Product prod, int quantity, Supplier supp)
		throws InvalidSupplier {
		// TODO: completare
		if (!supp.supplyes(prod))
			throw new InvalidSupplier();
		String code = "ORD" + ordN;
		ordN++;
		orders.put(code, new Order(code, prod, quantity, supp));
		return orders.get(code);
	}

	public Order findOrder(String code){
		// TODO: completare
		return orders.get(code);
	}
	
	public List<Order> pendingOrders(){
		// TODO: completare
		return orders.values().stream()
				.filter(o -> !o.delivered())
				.sorted((o1, o2) -> o1.getProduct().getCode().compareTo(o2.getProduct().getCode()))
				.collect(Collectors.toList());
	}

	public Map<String,List<Order>> ordersByProduct(){
	    return orders.values().stream()
	    		.collect(Collectors.groupingBy(
	    					o -> o.getProduct().getCode(),
	    					Collectors.toList()
	    				)
	    			);
	}
	
	public Map<String,Long> orderNBySupplier(){
	    return orders.values().stream()
	    		.filter(o -> o.delivered())
	    		.map(o -> o.getSupplier().getNome())
	    		.sorted((s1, s2) -> s1.compareTo(s2))
	    		.collect(Collectors.groupingBy(
	    					s -> s, 
	    					Collectors.counting()
	    				)
	    			);
	}
	
	public List<String> countDeliveredByProduct(){
	    return products.values().stream()
	    		.sorted((p1, p2) -> -Long.compare(p1.containdInNOrdersAlreadyDelivered(), 
	    				p2.containdInNOrdersAlreadyDelivered()))
	    		.map(p -> p.getCode()+" "+p.containdInNOrdersAlreadyDelivered())
	    		.collect(Collectors.toList());
	}
}
