package delivery;

public class Customer {
	
	private int id;
	private String name;
	private String address;
	private String phone;
	private String email;
	
	public Customer(int id, String name, String address, String phone, String email) {
		super();
		this.id = id;
		this.name = name;
		this.address = address;
		this.phone = phone;
		this.email = email;
	}

	int getId() {
		return id;
	}

	String getName() {
		return name;
	}

	String getAddress() {
		return address;
	}

	String getPhone() {
		return phone;
	}

	String getEmail() {
		return email;
	}
	
	@Override
	public String toString() {
		// TODO Auto-generated method stub
		return this.name+", "+this.address+", "+this.phone+", "+this.email;
	}

}
