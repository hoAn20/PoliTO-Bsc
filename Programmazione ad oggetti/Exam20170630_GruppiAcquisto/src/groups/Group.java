package groups;

import java.util.LinkedList;
import java.util.List;
import java.util.stream.Collectors;

public class Group {
	
	private String name;
	private String prod;
	private List<String> customers = new LinkedList<>();
	private List<String> suppliers = new LinkedList<>();
	private List<Bid> bids = new LinkedList<>();
	
	public Group(String name, String prod, List<String> customers) {
		super();
		this.name = name;
		this.prod = prod;
		this.customers = customers;
	}

	public String getName() {
		return name;
	}

	public void setName(String name) {
		this.name = name;
	}

	public String getProd() {
		return prod;
	}

	public void setProd(String prod) {
		this.prod = prod;
	}

	public List<String> getCustomers() {
		return customers;
	}

	public void setCustomers(List<String> customers) {
		this.customers = customers;
	}

	public List<String> getSuppliers() {
		return suppliers;
	}

	public void setSuppliers(List<String> suppliers) {
		this.suppliers = suppliers;
	}

	public List<Bid> getBids() {
		return bids;
	}
	
	public void addBid(Bid b) {
		this.bids.add(b);
		b.setProduct(this.prod);
	}
	
	public long getNCustromer() {
		return this.customers.size();
	}
	
	public int getBestBid() {
		return bids.stream()
				.map(b -> b.getPrice())
				.collect(Collectors.maxBy(Integer::compareTo)).orElse(0);
	}

}
