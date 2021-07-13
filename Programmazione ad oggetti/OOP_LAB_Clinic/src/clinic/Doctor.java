package clinic;

public class Doctor {
	
	private String first;
	private String last;
	private String ssn;
	private int docID;
	private String specialization;
	
	public Doctor(String first, String last, String ssn, int docID, String specialization) {
		super();
		this.first = first;
		this.last = last;
		this.ssn = ssn;
		this.docID = docID;
		this.specialization = specialization;
	}

	@Override
	public String toString() {
		return this.last + " " + this.first + " (" + this.ssn + ")" + " [" + this.docID + "]: " + this.specialization;
	}
	
	public String getSurnName() {
		return this.last + " " + this.first;
	}

	public String getSpecialization() {
		return specialization;
	}

	public Object compareTo(Doctor doctor) {
		if (this.last.equals(doctor.last)) {
			return this.first.compareTo(doctor.first);
		} else {
			return this.last.compareTo(doctor.last);
		}
	}

	public int getDocID() {
		return docID;
	}

}
