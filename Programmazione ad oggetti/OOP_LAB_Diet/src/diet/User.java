package diet;

/**
 * Represent a take-away system user
 *  
 */
public class User implements Comparable<User> {
	
	private String firstName;
	private String lastName;
	private String email;
	private String phoneNumber;

	public User(String firstName, String lastName, String email, String phoneNumber) {
		this.firstName = firstName;
		this.lastName = lastName;
		this.email = email;
		this.phoneNumber = phoneNumber;
	}

	/**
	 * get user's last name
	 * @return last name
	 */
	public String getLastName() {
		return this.firstName;
	}
	
	/**
	 * get user's first name
	 * @return first name
	 */
	public String getFirstName() {
		return this.lastName;
	}
	
	/**
	 * get user's email
	 * @return email
	 */
	public String getEmail() {
		return this.email;
	}
	
	/**
	 * get user's phone number
	 * @return  phone number
	 */
	public String getPhone() {
		return this.phoneNumber;
	}
	
	/**
	 * change user's email
	 * @param email new email
	 */
	public void SetEmail(String email) {
		this.email = email;
	}
	
	/**
	 * change user's phone number
	 * @param phone new phone number
	 */
	public void setPhone(String phone) {
		this.phoneNumber = phone;
	}

	@Override
	public int compareTo(User o) {
		if (this.lastName.equals(o.lastName)) {
			return this.firstName.compareTo(o.firstName);
		} else {
			return this.lastName.compareTo(o.lastName);
		}
	}
	
	@Override
	public String toString() {
		return this.firstName + " " + this.lastName;
	}
	
}
