package transactions;

public class Place {
	
	private String name;
	private Region belongingRegion;

	public Place(String s, Region r) {
		this.name = s;
		this.belongingRegion = r;
	}

	String getName() {
		return name;
	}

	Region getBelongingRegion() {
		return belongingRegion;
	}
	
	

}
