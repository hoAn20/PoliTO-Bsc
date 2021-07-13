package groups;
import java.util.HashMap;
import java.util.HashSet;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;
import java.util.Set;
import java.util.SortedMap;
import java.util.TreeMap;
import java.util.stream.Collectors;


public class GroupHandling {
	
	private Map<String, List<String>> products = new HashMap<>();
	private Set<String> suppliers = new HashSet<>();
	private Map<String, Group> groups = new HashMap<>();
	private Set<Bid> bids = new HashSet<>();
	
//R1	
	public void addProduct (String productTypeName, String... supplierNames) 
			throws GroupHandlingException {
		if (products.containsKey(productTypeName))
			throw new GroupHandlingException();
		for (String s: supplierNames)
			if (!suppliers.contains(s))
				suppliers.add(s);
		products.put(productTypeName, List.of(supplierNames));
	}
	
	public List<String> getProductTypes (String supplierName) {
		return products.keySet().stream()
				.sorted((s1, s2) -> s1.compareTo(s2))
				.collect(Collectors.toList());
	}
	
//R2	
	public void addGroup (String name, String productName, String... customerNames) 
			throws GroupHandlingException {
		if (groups.containsKey(name))
			throw new GroupHandlingException();
		if (products.containsKey(productName))
			groups.put(name, new Group(name, productName, List.of(customerNames)));
	}
	
	public List<String> getGroups (String customerName) {
        return groups.values().stream()
        		.filter(g -> g.getCustomers().contains(customerName))
        		.map(g -> g.getName())
        		.sorted((s1, s2) -> s1.compareTo(s2))
        		.collect(Collectors.toList());
	}

//R3
	public void setSuppliers (String groupName, String... supplierNames)
			throws GroupHandlingException {
		Group g = groups.get(groupName);
		for (String s: supplierNames)
			if (!suppliers.contains(s) || !products.get(g.getProd()).contains(s))
				throw new GroupHandlingException();
		g.setSuppliers(List.of(supplierNames));
	}
	
	public void addBid (String groupName, String supplierName, int price)
			throws GroupHandlingException {
		Group g = groups.get(groupName);
		if (!g.getSuppliers().contains(supplierName))
			throw new GroupHandlingException();
		Bid bid = new Bid(supplierName, price);
		g.addBid(bid);
		bids.add(bid);
	}
	
	public String getBids (String groupName) {
        return groups.get(groupName).getBids().stream()
        		.sorted((b1, b2) -> Integer.compare(b1.getPrice(), b2.getPrice()))
        		.map(b -> b.getSupp()+":"+b.getPrice())
        		.collect(Collectors.joining(","));
	}
	
	
//R4	
	public void vote (String groupName, String customerName, String supplierName)
			throws GroupHandlingException {
		Group g = groups.get(groupName);
		boolean p = false;
		Bid bid = null;
		if (!g.getCustomers().contains(customerName))
			throw new GroupHandlingException();
		if (!g.getSuppliers().contains(supplierName))
			throw new GroupHandlingException();
		for (Bid b: g.getBids())
			if (supplierName.compareTo(b.getSupp()) == 0) {
				p = true;
				bid = b;
			}
		if (!p)
			throw new GroupHandlingException();
		bid.addVote();
	}
	
	public String  getVotes (String groupName) {
        return groups.get(groupName).getBids().stream()
        		.filter(b -> b.getVotes() >= 1)
        		.sorted((b1, b2) -> b1.getSupp().compareTo(b2.getSupp()))
        		.map(b -> b.getSupp()+":"+b.getVotes())
        		.collect(Collectors.joining(","));
	}
	
	public String getWinningBid (String groupName) {
		return groups.get(groupName).getBids().stream()
				.max((b1, b2) -> Integer.compare(b1.getVotes(), b2.getVotes()))
				.map(b -> b.getSupp()+":"+b.getVotes()).orElse(null);
	}
	
//R5
	public SortedMap<String, Integer> maxPricePerProductType() { //serve toMap
        return bids.stream()
        		.sorted((b1, b2) -> b1.getProduct().compareTo(b2.getProduct()))
        		.collect(Collectors.toMap(
        					b -> b.getProduct(),
        					b -> b.getPrice(),
        					Integer::max,
        					TreeMap::new
        				)
        			);
	}
	
	public SortedMap<Integer, List<String>> suppliersPerNumberOfBids() {
		Map <String, Long> m = bids.stream().
				map(b -> b.getSupp())
				.collect(Collectors.groupingBy(
							s -> s, 
							Collectors.counting()
						)
					);
		SortedMap<Integer, List<String>> m2 = new TreeMap<>();
		for (Entry<String, Long> e: m.entrySet()) {
			if (!m2.containsKey(e.getValue().intValue()))
				m2.put(e.getValue().intValue(), new LinkedList<>());
			m2.get(e.getValue().intValue()).add(e.getKey());
		}
		return m2;
	}
	
	public SortedMap<String, Long> numberOfCustomersPerProductType() {
        return groups.values().stream()
        		.sorted((g1, g2) -> g1.getProd().compareTo(g2.getProd()))
        		.collect(Collectors.groupingBy(
        					g -> g.getProd(), 
        					TreeMap::new, 
        					Collectors.summingLong(Group::getNCustromer)
        				)
        			);
	}
	
}
