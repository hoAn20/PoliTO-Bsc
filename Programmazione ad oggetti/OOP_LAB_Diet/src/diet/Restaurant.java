package diet;
import java.util.Collections;
import java.util.LinkedList;
import java.util.List;

import diet.Order.OrderStatus;

/**
 * Represents a restaurant in the take-away system
 *
 */
public class Restaurant {
	
	class Opening {
		
		private String open;
		private String close;

		Opening(String o, String c) {
			this.open = o; 
			this.close = c;
		}
		
		public String getOpen() {
			return open;
		}

		public void setOpen(String open) {
			this.open = open;
		}

		public String getClose() {
			return close;
		}

		public void setClose(String close) {
			this.close = close;
		}
		
	}
	
	private String name;
	private Food food;
	private List<Opening> openings = new LinkedList<>();
	private List<Order> orders = new LinkedList<Order>();
	
	/**
	 * Constructor for a new restaurant.
	 * 
	 * Materials and recipes are taken from
	 * the food object provided as argument.
	 * 
	 * @param name	unique name for the restaurant
	 * @param food	reference food object
	 */
	public Restaurant(String name, Food food) {
		this.name = name;
		this.food = food;
	}
	
	/**
	 * gets the name of the restaurant
	 * 
	 * @return name
	 */
	public String getName() {
		return this.name;
	}
	
	/**
	 * Define opening hours.
	 * 
	 * The opening hours are considered in pairs.
	 * Each pair has the initial time and the final time
	 * of opening intervals.
	 * 
	 * for a restaurant opened from 8:15 until 14:00 and from 19:00 until 00:00, 
	 * is thoud be called as {@code setHours("08:15", "14:00", "19:00", "00:00")}.
	 * 
	 * @param hm a list of opening hours
	 */
	public void setHours(String ... hm) {
		for (int i = 0; i < hm.length; i +=2) {
			this.openings.add(new Opening(hm[i], hm[i+1]));
		}
	}
	
	public Menu getMenu(String name) {
		return null;
	}
	
	/**
	 * Creates a new menu
	 * 
	 * @param name name of the menu
	 * 
	 * @return the newly created menu
	 */
	public Menu createMenu(String name) {
		return this.food.createMenu(name);
	}

	/**
	 * Find all orders for this restaurant with 
	 * the given status.
	 * 
	 * The output is a string formatted as:
	 * <pre>
	 * Napoli, Judi Dench : (19:00):
	 * 	M6->1
	 * Napoli, Ralph Fiennes : (19:00):
	 * 	M1->2
	 * 	M6->1
	 * </pre>
	 * 
	 * The orders are sorted by name of restaurant, name of the user, and delivery time.
	 * 
	 * @param status the status of the searched orders
	 * 
	 * @return the description of orders satisfying the criterion
	 */
	public String ordersWithStatus(OrderStatus status) {
		StringBuffer buf = new StringBuffer();
		Collections.sort(this.orders);
		for (Order o: this.orders) {
			if (o.getStatus() == status) {
				buf.append(o.toString() + "\n");
			}
		}
		return buf.toString();
	}

	public boolean isOpened(String time) {
		int h = Integer.parseInt(time.split(":")[0]);
		int m = Integer.parseInt(time.split(":")[1]);
		
		for (Opening o: this.openings) {
			int oh = Integer.parseInt(o.getOpen().split(":")[0]);
			int om = Integer.parseInt(o.getOpen().split(":")[1]);
			if (h > oh || (h == oh && m >= om)) {
				oh = Integer.parseInt(o.getClose().split(":")[0]);
				om = Integer.parseInt(o.getClose().split(":")[1]);
				if (h < oh || (h == oh && m < om)) {
					return true;
				}
			}
		}
		return false;
	}

	public Order addOrder(User user, int h, int m) {
		Order o;
		String time = h + ":" + m;
		if (this.isOpened(new String(time))) {
			this.orders.add(o = new Order(user, this.name, time));
		} else {
			this.orders.add(o = new Order(user, this.name, nextOverture(time)));
		}
		return o;
	}
	
	private String nextOverture(String time) {
		int h = Integer.parseInt(time.split(":")[0]);
		int m = Integer.parseInt(time.split(":")[1]);
		
		for (Opening o: this.openings) {
			int oh = Integer.parseInt(o.getOpen().split(":")[0]);
			int om = Integer.parseInt(o.getOpen().split(":")[1]);
			
			if (h < oh || (h == oh && m < om)) {
				return o.getOpen();
			}
		}
		
		return this.openings.get(0).getOpen();
	}
	
}
