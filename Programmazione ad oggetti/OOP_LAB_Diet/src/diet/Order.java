package diet;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

/**
 * Represents an order in the take-away system
 */
public class Order implements Comparable<Order>{
 
	/**
	 * Defines the possible order status
	 */
	public enum OrderStatus {
		ORDERED, READY, DELIVERED;
	}
	/**
	 * Defines the possible valid payment methods
	 */
	public enum PaymentMethod {
		PAID, CASH, CARD;
	}
	
	class OrderedMenus implements Comparable<OrderedMenus> {
		
		private String name;
		private int qty;
		
		public OrderedMenus(String name, int qty) {
			this.name = name;
			this.qty = qty;
		}
		
		
		public String getName() {
			return name;
		}


		@Override
		public String toString() {
			return "\t"+this.name + "->"+this.qty;
		}

		@Override
		public int compareTo(OrderedMenus o) {
			return this.name.compareTo(o.name);
		}


		public void setQty(int quantity) {
			this.qty = quantity;
		}
	}
	
	private String time;
	private User user;
	private OrderStatus status;
	private PaymentMethod method;
	private List<OrderedMenus> orderedMenus = new ArrayList<>();
	private String restaurantName;
		
	public Order(User user, String name, String time) {
		this.user = user;
		this.restaurantName = name;
		this.time = time;
		this.status = OrderStatus.ORDERED;
		this.method = PaymentMethod.CASH;
	}

	/**
	 * Total order price
	 * @return order price
	 */
	public double Price() {
		return -1.0;
	}
	
	/**
	 * define payment method
	 * 
	 * @param method payment method
	 */
	public void setPaymentMethod(PaymentMethod method) {
		this.method = method;
	}
	
	/**
	 * get payment method
	 * 
	 * @return payment method
	 */
	public PaymentMethod getPaymentMethod() {
		return this.method;
	}
	
	/**
	 * change order status
	 * @param newStatus order status
	 */
	public void setStatus(OrderStatus newStatus) {
		this.status = newStatus;
	}
	
	/**
	 * get current order status
	 * @return order status
	 */
	public OrderStatus getStatus(){
		return this.status;
	}
	
	/**
	 * Add a new menu with the relative order to the order.
	 * The menu must be defined in the {@link Food} object
	 * associated the restaurant that created the order.
	 * 
	 * @param menu     name of the menu
	 * @param quantity quantity of the menu
	 * @return this order to enable method chaining
	 */
	public Order addMenus(String menu, int quantity) {
		boolean mod = false;
		for (OrderedMenus o: this.orderedMenus) {
			if (o.getName().equals(menu)) {
				mod = true;
				o.setQty(quantity);
				break;
			}
		}
		if (!mod)
			this.orderedMenus.add(new OrderedMenus(menu,  quantity));
		return this;
	}
	
	/**
	 * Converts to a string as:
	 * <pre>
	 * RESTAURANT_NAME, USER_FIRST_NAME USER_LAST_NAME : DELIVERY(HH:MM):
	 * 	MENU_NAME_1->MENU_QUANTITY_1
	 * 	...
	 * 	MENU_NAME_k->MENU_QUANTITY_k
	 * </pre>
	 */
	@Override
	public String toString() {
		StringBuffer buf = new StringBuffer(this.restaurantName + ", " + this.user.getLastName() + " " + this.user.getFirstName()
				+ " : (" + this.time + "):");
		Collections.sort(this.orderedMenus);
		for (OrderedMenus o: this.orderedMenus) {
			buf.append("\n" + o.toString());
		}
			
		return buf.toString();
	}

	@Override
	public int compareTo(Order o) {
		if (!(this.user.compareTo(o.user) == 0)) {
			return this.user.compareTo(o.user);
		}
		Integer t1 = Integer.parseInt(this.time.split(":")[0])*60 +
				Integer.parseInt(this.time.split(":")[1]);
		Integer t2 = Integer.parseInt(o.time.split(":")[0])*60 +
				Integer.parseInt(o.time.split(":")[1]);
		return t1.compareTo(t2);
	}
	
}
