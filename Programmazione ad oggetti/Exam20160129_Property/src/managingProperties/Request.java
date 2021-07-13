package managingProperties;

public class Request {
	
	static enum State {
		pending, assigned, completed
	};
	static int Id = 1;
	
	private String owner;
	private String apartment;
	private String profession;
	private String profesisonal;
	private State state;
	private int id;
	private int charge;
	
	public Request(String owner, String apartment, String profession) {
		super();
		this.owner = owner;
		this.apartment = apartment;
		this.profession = profession;
		this.state = State.pending;
		this.id = Id++;
	}

	public String getOwner() {
		return owner;
	}

	public void setOwner(String owner) {
		this.owner = owner;
	}

	public String getApartment() {
		return apartment;
	}

	public void setApartment(String apartment) {
		this.apartment = apartment;
	}

	public String getProfession() {
		return profession;
	}

	public void setProfession(String profession) {
		this.profession = profession;
	}

	public State getState() {
		return state;
	}

	public void setState(State state) {
		this.state = state;
	}

	public int getId() {
		return id;
	}

	public void setId(int id) {
		this.id = id;
	}

	public void setCharge(int amount) {
		this.charge = amount;
	}

	public int getCharge() {
		if (state.compareTo(State.completed) != 0)
			return 0;
		return charge;
	}

	public void setProfessional(String professional) {
		this.profesisonal = professional;
	}
	
	public String getProfessional() {
		return this.profesisonal;
	}
	
	public String getBuilding() {
		String[] s = this.apartment.split(":");
		return s[0];
	}

}
