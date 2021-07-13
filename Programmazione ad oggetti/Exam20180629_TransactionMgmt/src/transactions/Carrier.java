package transactions;

import java.util.LinkedList;
import java.util.List;

public class Carrier {
	
	private String name;
	private List<String> servedRegions = new LinkedList<String>();

	public Carrier(String carrierName) {
		this.name = carrierName;
	}

	public void addServedRegion(String s) {
		this.servedRegions.add(s);
	}

	public List<String> getServedRegions() {
		return this.servedRegions;
	}

	public String getName() {
		return this.name;
	}

}
