package restaurantChain;

public class Group {
	
	private String name;
	private int persons;
	private String[] menu;
	private boolean ordered;
	private boolean payed;
	private double tot;

	double getTot() {
		return tot;
	}


	void setTot(double tot) {
		this.tot = tot;
	}


	boolean isOrdered() {
		return ordered;
	}


	void setOrdered(boolean ordered) {
		this.ordered = ordered;
	}


	boolean isPayed() {
		return payed;
	}


	void setPayed(boolean payed) {
		this.payed = payed;
	}


	String[] getMenu() {
		return menu;
	}


	void setMenu(String[] menu) {
		this.menu = menu;
	}


	public Group(String name, int persons) {
		this.name = name;
		this.persons = persons;
		ordered = false;
		payed = false;
		
	}


	String getName() {
		return name;
	}


	void setName(String name) {
		this.name = name;
	}


	int getPersons() {
		return persons;
	}


	void setPersons(int persons) {
		this.persons = persons;
	}
	
	

}
