package delivery;

import java.util.Comparator;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.SortedMap;
import java.util.TreeMap;
import java.util.stream.Collectors;

public class Delivery {
    
    public static enum OrderStatus { NEW, CONFIRMED, PREPARATION, ON_DELIVERY, DELIVERED } 
    
    private int CustomerId = 1;
    private int OrderId = 1;
    private int ItemId = 1;
    
    Map<Integer, Customer> customers = new HashMap<>();
    Map<Integer, MenuItem> menuItems = new HashMap<>();
    Map<Integer, Order> orders = new HashMap<>();
    
    /**
     * Creates a new customer entry and returns the corresponding unique ID.
     * 
     * The ID for the first customer must be 1.
     * 
     * 
     * @param name name of the customer
     * @param address customer address
     * @param phone customer phone number
     * @param email customer email
     * @return unique customer ID (positive integer)
     */
    public int newCustomer(String name, String address, String phone, String email) throws DeliveryException {
    	for (Customer c : customers.values())
    		if (c.getEmail().compareTo(email) == 0)
    			throw new DeliveryException("email già in uso");
    	
    	customers.put(CustomerId, new Customer(CustomerId, name, address, phone, email));
        return CustomerId++;
    }
    
    /**
     * Returns a string description of the customer in the form:
     * <pre>
     * "NAME, ADDRESS, PHONE, EMAIL"
     * </pre>
     * 
     * @param customerId
     * @return customer description string
     */
    public String customerInfo(int customerId){
        return customers.get(customerId).toString();
    }
    
    /**
     * Returns the list of customers, sorted by name
     * 
     */
    public List<String> listCustomers(){
        return customers.values().stream()
        		.sorted((c1, c2) -> c1.getName().compareTo(c2.getName()))
        		.map(c -> c.toString())
        		.collect(Collectors.toList());
    }
    
    /**
     * Add a new item to the delivery service menu
     * 
     * @param description description of the item (e.g. "Pizza Margherita", "Bunet")
     * @param price price of the item (e.g. 5 EUR)
     * @param category category of the item (e.g. "Main dish", "Dessert")
     * @param prepTime estimate preparation time in minutes
     */
    public void newMenuItem(String description, double price, String category, int prepTime){
        menuItems.put(ItemId, new MenuItem(ItemId, description, price, category, prepTime));
        ItemId++;
    }
    
    /**
     * Search for items matching the search string.
     * The items are sorted by category first and then by description.
     * 
     * The format of the items is:
     * <pre>
     * "[CATEGORY] DESCRIPTION : PRICE"
     * </pre>
     * 
     * @param search search string
     * @return list of matching items
     */
    public List<String> findItem(String search){
        return menuItems.values().stream()
        		.filter(m -> m.getDescription().toLowerCase().contains(search.toLowerCase()))
        		.sorted(Comparator.comparing(MenuItem::getCategory).thenComparing(MenuItem::getDescription))
        		.map(m -> m.toString())
        		.collect(Collectors.toList());
    }
    
    /**
     * Creates a new order for the given customer.
     * Returns the unique id of the order, a progressive
     * integer number starting at 1.
     * 
     * @param customerId
     * @return order id
     */
    public int newOrder(int customerId){
    	orders.put(OrderId, new Order(OrderId, customerId));
        return OrderId++;
    }
    
    /**
     * Add a new item to the order with the given quantity.
     * 
     * If the same item is already present in the order is adds the
     * given quantity to the current quantity.
     * 
     * The method returns the final total quantity of the item in the order. 
     * 
     * The item is found through the search string as in {@link #findItem}.
     * If none or more than one item is matched, then an exception is thrown.
     * 
     * @param orderId the id of the order
     * @param search the item search string
     * @param qty the quantity of the item to be added
     * @return the final quantity of the item in the order
     * @throws DeliveryException in case of non unique match or invalid order ID
     */
    public int addItem(int orderId, String search, int qty) throws DeliveryException {
    	if (!orders.containsKey(orderId))
    		throw new DeliveryException("codice ordine non valido");
    	if (findItem(search).size() > 1 || findItem(search).size() < 1)
    		throw new DeliveryException("la ricerca corrisponde a più di un elemento o a nessuno");
    	
    	String item = findItem(search).get(0);
    	int i = 0;
    	
    	for (MenuItem m : menuItems.values())
    		if (m.toString().compareTo(item) == 0)
    			orders.get(orderId).addItem(i = m.getId(), qty);
    	
        return orders.get(orderId).getQty(i);
    }
    
    /**
     * Show the items of the order using the following format
     * <pre>
     * "DESCRIPTION, QUANTITY"
     * </pre>
     * 
     * @param orderId the order ID
     * @return the list of items in the order
     * @throws DeliveryException when the order ID in invalid
     */
    public List<String> showOrder(int orderId) throws DeliveryException {
    	if (!orders.containsKey(orderId))
    		throw new DeliveryException("codice ordine non valido");
    	
        return orders.get(orderId).getItems(menuItems);
    }
    
    /**
     * Retrieves the total amount of the order.
     * 
     * @param orderId the order ID
     * @return the total amount of the order
     * @throws DeliveryException when the order ID in invalid
     */
    public double totalOrder(int orderId) throws DeliveryException {
    	if (!orders.containsKey(orderId))
    		throw new DeliveryException("codice ordine non valido");
        return orders.get(orderId).getTotal(menuItems);
    }
    
    /**
     * Retrieves the status of an order
     * 
     * @param orderId the order ID
     * @return the current status of the order
     * @throws DeliveryException when the id is invalid
     */
    public OrderStatus getStatus(int orderId) throws DeliveryException {
    	if (!orders.containsKey(orderId))
    		throw new DeliveryException("codice ordine non valido");
        return orders.get(orderId).getStatus();
    }
    
    /**
     * Confirm the order. The status goes from {@code NEW} to {@code CONFIRMED}
     * 
     * Returns the delivery time estimate computed as the sum of:
     * <ul>
     * <li>start-up delay (conventionally 5 min)
     * <li>preparation time (max of all item preparation time)
     * <li>transportation time (conventionally 15 min)
     * </ul>
     * 
     * @param orderId the order id
     * @return delivery time estimate in minutes
     * @throws DeliveryException when order ID invalid to status not {@code NEW}
     */
    public int confirm(int orderId) throws DeliveryException {
    	if (!orders.containsKey(orderId))
    		throw new DeliveryException("codice ordine non valido");
    	if (orders.get(orderId).getStatus() != OrderStatus.NEW)
    		throw new DeliveryException("ordine già confermato");
    	
    	orders.get(orderId).setStatus(OrderStatus.CONFIRMED);
        return orders.get(orderId).getTime(menuItems);
    }

    /**
     * Start the order preparation. The status goes from {@code CONFIRMED} to {@code PREPARATION}
     * 
     * Returns the delivery time estimate computed as the sum of:
     * <ul>
     * <li>preparation time (max of all item preparation time)
     * <li>transportation time (conventionally 15 min)
     * </ul>
     * 
     * @param orderId the order id
     * @return delivery time estimate in minutes
     * @throws DeliveryException when order ID invalid to status not {@code CONFIRMED}
     */
    public int start(int orderId) throws DeliveryException {
    	if (!orders.containsKey(orderId))
    		throw new DeliveryException("codice ordine non valido");
    	if (orders.get(orderId).getStatus() != OrderStatus.CONFIRMED)
    		throw new DeliveryException("ordine già iniziato");
    	
    	orders.get(orderId).setStatus(OrderStatus.PREPARATION);
        return orders.get(orderId).getTime(menuItems) - Order.TIME_TO_START;
    }

    /**
     * Begins the order delivery. The status goes from {@code PREPARATION} to {@code ON_DELIVERY}
     * 
     * Returns the delivery time estimate computed as the sum of:
     * <ul>
     * <li>transportation time (conventionally 15 min)
     * </ul>
     * 
     * @param orderId the order id
     * @return delivery time estimate in minutes
     * @throws DeliveryException when order ID invalid to status not {@code PREPARATION}
     */
    public int deliver(int orderId) throws DeliveryException {
    	if (!orders.containsKey(orderId))
    		throw new DeliveryException("codice ordine non valido");
    	if (orders.get(orderId).getStatus() != OrderStatus.PREPARATION)
    		throw new DeliveryException("ordine già preparato");
    	
    	orders.get(orderId).setStatus(OrderStatus.ON_DELIVERY);
        return Order.TIME_TO_SHIP;
    }
    
    /**
     * Complete the delivery. The status goes from {@code ON_DELIVERY} to {@code DELIVERED}
     * 
     * 
     * @param orderId the order id
     * @throws DeliveryException when order ID invalid to status not {@code ON_DELIVERY}
     */
    public void complete(int orderId) throws DeliveryException {
    	if (!orders.containsKey(orderId))
    		throw new DeliveryException("codice ordine non valido");
    	if (orders.get(orderId).getStatus() != OrderStatus.ON_DELIVERY)
    		throw new DeliveryException("ordine già consegnato");
    	
    	orders.get(orderId).setStatus(OrderStatus.DELIVERED);
    }
    
    /**
     * Retrieves the total amount for all orders of a customer.
     * 
     * @param customerId the customer ID
     * @return total amount
     */
    public double totalCustomer(int customerId){
        return orders.values().stream()
        		.filter(o -> Integer.compare(customerId, o.getCustomerId()) == 0 &&
        				o.getStatus()!=OrderStatus.NEW)
        		.collect(Collectors.summingDouble(o -> o.getTotal(menuItems)));
    }
    
    /**
     * Computes the best customers by total amount of orders.
     *  
     * @return the classification
     */
    public SortedMap<Double,List<String>> bestCustomers(){
        return customers.values().stream()
        		.collect(Collectors.groupingBy(
        					c -> totalCustomer(c.getId()), 
        					TreeMap::new,
        					Collectors.mapping(c -> c.toString(), Collectors.toList())
        				)
        			);
    }
    
// NOT REQUIRED
//
//    /**
//     * Computes the best items by total amount of orders.
//     *  
//     * @return the classification
//     */
//    public List<String> bestItems(){
//        return null;
//    }
//    
//    /**
//     * Computes the most popular items by total quantity ordered.
//     *  
//     * @return the classification
//     */
//    public List<String> popularItems(){
//        return null;
//    }

}
