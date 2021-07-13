package restaurantChain;

import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.stream.Collectors;

public class Restaurant {
	
	private String name;
	private int tables;
	private Map<String, Menu> menus = new HashMap<>();
	private Map<String, Group> groups = new HashMap<>();
	private int refused = 0;
	
	public Restaurant(String name, int tables) {
		this.name = name;
		this.tables = tables;
	}

	public String getName(){
		return this.name;
	}
	
	public void addMenu(String name, double price) throws InvalidName{
		if (menus.containsKey(name))
			throw new InvalidName();
		
		menus.put(name, new Menu(name, price));
	}
	
	public int reserve(String name, int persons) throws InvalidName{
		int tab = (persons>4?(int)(Math.ceil((double)persons/4)):1);
		if (groups.containsKey(name))
			throw new InvalidName();
		if (tab > tables) {
			refused += persons;
			return 0;
		}
		this.tables = this.tables - tab;
		groups.put(name, new Group(name, persons));
		return tab;
	}
	
	int getTables() {
		return tables;
	}

	void setTables(int tables) {
		this.tables = tables;
	}

	Map<String, Menu> getMenus() {
		return menus;
	}

	void setMenus(Map<String, Menu> menus) {
		this.menus = menus;
	}

	Map<String, Group> getGroups() {
		return groups;
	}

	void setGroups(Map<String, Group> groups) {
		this.groups = groups;
	}

	void setName(String name) {
		this.name = name;
	}

	public int getRefused(){
		return refused;
	}
	
	public int getUnusedTables(){
		return tables;
	}
	
	public boolean order(String name, String... menu) throws InvalidName{
		
		if (!groups.containsKey(name))
			throw new InvalidName();
		for (String s : menu)
			if (!menus.containsKey(s))
				throw new InvalidName();
		
		if (menu.length < groups.get(name).getPersons())
			return false;
		
		groups.get(name).setMenu(menu);
		groups.get(name).setOrdered(true);
		return true;
	}
	
	public List<String> getUnordered(){
		return groups.values().stream()
				.filter(g -> !g.isOrdered())
				.map(g -> g.getName())
				.sorted(String::compareTo)
				.collect(Collectors.toList());
	}
	
	public double pay(String name) throws InvalidName{
		if (!groups.containsKey(name))
			throw new InvalidName();
		
		if (!groups.get(name).isOrdered())
			return 0;
		
		List<String> menu = List.of(groups.get(name).getMenu());
		groups.get(name).setPayed(true);
		groups.get(name).setTot(menu.stream().map(m -> menus.get(m)).collect(Collectors.summingDouble(Menu::getPrice)));
		return groups.get(name).getTot();
	}
	
	public List<String> getUnpaid(){
		return groups.values().stream()
				.filter(o -> !o.isPayed() && o.isOrdered())
				.map(o -> o.getName())
				.sorted(String::compareTo)
				.collect(Collectors.toList());
	}
	
	public double getIncome(){
		return groups.values().stream()
				.filter(g -> g.isPayed())
				.collect(Collectors.summingDouble(Group::getTot));
	}

}
