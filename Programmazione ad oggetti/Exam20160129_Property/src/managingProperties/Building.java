package managingProperties;

public class Building {
	
	class Appartment {
		String p;
		
		public Appartment(String p) {
			this.p = p;
		}
	}
	
	private String id;
	private Appartment[] appartments;
	
	public Building(String id, int n) {
		this.id = id;
		this.appartments = new Appartment[n];
	}
	
	public void assignApp(String p, int n) throws PropertyException{
		if (n > appartments.length) 
			throw new PropertyException("Apparamento non esistente: " + n);
		if (this.appartments[n-1] != null)
			throw new PropertyException("Appartamento già assegnato: " + n);
		
		this.appartments[n-1] = new Appartment(p);
	}

	public String getId() {
		return id;
	}

	public void setId(String id) {
		this.id = id;
	}

	public Appartment[] getAppartments() {
		return appartments;
	}

	public void setAppartments(Appartment[] appartments) {
		this.appartments = appartments;
	}
	
	public int getNApp() {
		return this.appartments.length;
	}

}
