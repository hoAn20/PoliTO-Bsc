package clinic;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.Reader;
import java.util.Collection;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.regex.Pattern;
import java.util.stream.Collectors;

/**
 * Represents a clinic with patients and doctors.
 * 
 */
public class Clinic {
	
	Map<String, Patient> patients = new HashMap<>();
	Map<Integer, Doctor> doctors = new HashMap<>();
	Map<String, Integer> patientToDoctor = new HashMap<>();

	/**
	 * Add a new clinic patient.
	 * 
	 * @param first first name of the patient
	 * @param last last name of the patient
	 * @param ssn SSN number of the patient
	 */
	public void addPatient(String first, String last, String ssn) {
		patients.put(ssn, new Patient(first, last, ssn));
	}


	/**
	 * Retrieves a patient information
	 * 
	 * @param ssn SSN of the patient
	 * @return the object representing the patient
	 * @throws NoSuchPatient in case of no patient with matching SSN
	 */
	public String getPatient(String ssn) throws NoSuchPatient {
		if (!patients.containsKey(ssn)) {
			throw new NoSuchPatient("Il paziente richiesto non risulta presente nei nostri db.");
		}
		return patients.get(ssn).toString();
	}
	
	public Collection<Patient> getPatients() {
		return this.patients.values();
	}

	/**
	 * Add a new doctor working at the clinic
	 * 
	 * @param first first name of the doctor
	 * @param last last name of the doctor
	 * @param ssn SSN number of the doctor
	 * @param docID unique ID of the doctor
	 * @param specialization doctor's specialization
	 */
	public void addDoctor(String first, String last, String ssn, int docID, String specialization) {
		doctors.put(docID, new Doctor(first, last, ssn, docID, specialization));
	}

	/**
	 * Retrieves information about a doctor
	 * 
	 * @param docID ID of the doctor
	 * @return object with information about the doctor
	 * @throws NoSuchDoctor in case no doctor exists with a matching ID
	 */
	public String getDoctor(int docID) throws NoSuchDoctor {
		if (!doctors.keySet().contains(docID)) {
			throw new NoSuchDoctor("Il dottore richiesto non risulta presente nei nostri db.");
		}
		return doctors.get(docID).toString();
	}
	
	public Collection<Doctor> getDoctors() {
		return this.doctors.values();
	}
	
	/**
	 * Assign a given doctor to a patient
	 * 
	 * @param ssn SSN of the patient
	 * @param docID ID of the doctor
	 * @throws NoSuchPatient in case of not patient with matching SSN
	 * @throws NoSuchDoctor in case no doctor exists with a matching ID
	 */
	public void assignPatientToDoctor(String ssn, int docID) throws NoSuchPatient, NoSuchDoctor {
		if (!patients.containsKey(ssn)) {
			throw new NoSuchPatient("Il paziente richiesto non risulta presente nei nostri db.");
		}
		if (!doctors.keySet().contains(docID)) {
			throw new NoSuchDoctor("Il dottore richiesto non risulta presente nei nostri db.");
		}
		patientToDoctor.put(ssn, docID);
	}
	
	/**
	 * Retrieves the id of the doctor assigned to a given patient.
	 * 
	 * @param ssn SSN of the patient
	 * @return id of the doctor
	 * @throws NoSuchPatient in case of not patient with matching SSN
	 * @throws NoSuchDoctor in case no doctor has been assigned to the patient
	 */
	public int getAssignedDoctor(String ssn) throws NoSuchPatient, NoSuchDoctor {
		if (!patients.containsKey(ssn)) {
			throw new NoSuchPatient("Il paziente richiesto non risulta presente nei nostri db.");
		}
		return patientToDoctor.get(ssn);
	}
	
	/**
	 * Retrieves the patients assigned to a doctor
	 * 
	 * @param id ID of the doctor
	 * @return collection of patient SSNs
	 * @throws NoSuchDoctor in case the {@code id} does not match any doctor 
	 */
	public Collection<String> getAssignedPatients(int id) throws NoSuchDoctor {
		if (!doctors.keySet().contains(id)) {
			throw new NoSuchDoctor("Il dottore richiesto non risulta presente nei nostri db.");
		}
		List<String> ps = new LinkedList<>();
		for (String p: patientToDoctor.keySet()) {
			if (patientToDoctor.get(p).equals(id)) {
				ps.add(p);
			}
		}
		return ps;
	}


	/**
	 * Loads data about doctors and patients from the given stream.
	 * <p>
	 * The text file is organized by rows, each row contains info about
	 * either a patient or a doctor.</p>
	 * <p>
	 * Rows containing a patient's info begin with letter {@code "P"} followed by first name,
	 * last name, and SSN. Rows containing doctor's info start with letter {@code "M"},
	 * followed by badge ID, first name, last name, SSN, and specialization.<br>
	 * The elements on a line are separated by the {@code ';'} character possibly
	 * surrounded by spaces that should be ignored.</p>
	 * <p>
	 * In case of error in the data present on a given row, the method should be able
	 * to ignore the row and skip to the next one.<br>

	 * 
	 * @param readed linked to the file to be read
	 * @throws IOException in case of IO error
	 */
	public void loadData(Reader reader) throws IOException {
		BufferedReader bReader = new BufferedReader(reader);
		
		Pattern patientPattern = Pattern.compile("P;[A-Za-z]+;[A-Za-z]+;[A-Za-z0-9]+");
		Pattern doctorPattern = Pattern.compile("M;[1-9]+;[A-Za-z]+;[A-Za-z]+;[A-Za-z0-9]+;[A-Za-z]+");
		
		String line;
		
		while ((line = bReader.readLine()) != null) {
			if (patientPattern.matcher(line).matches()) {
				String[] parts = line.split(";");
				patients.put(parts[3], new Patient(parts[1], parts[2], parts[3]));
			}
			if (doctorPattern.matcher(line).matches()) {
				String[] parts = line.split(";");
				doctors.put(Integer.parseInt(parts[1]), new Doctor(parts[2], parts[3], parts[4], 
						Integer.parseInt(parts[1]), parts[5]));
			}
		}
	}


	/**
	 * Retrieves the collection of doctors that have no patient at all.
	 * The doctors are returned sorted in alphabetical order
	 * 
	 * @return the collection of doctors' ids
	 */
	public Collection<Integer> idleDoctors(){
		return doctors.values().stream()
				.filter(d -> !patientToDoctor.values().contains(d.getDocID()))
				.map(d -> d.getDocID())
				.collect(Collectors.toList());
	}

	/**
	 * Retrieves the collection of doctors having a number of patients larger than the average.
	 * 
	 * @return  the collection of doctors' ids
	 */
	public Collection<Integer> busyDoctors(){
		Map<Integer, Long> docandnum = patientToDoctor.entrySet().stream()
				.map(e -> e.getValue())
				.collect(Collectors.groupingBy(
						id -> id,
						Collectors.counting()
						));
		float avg = 0;
		for (Long l: docandnum.values()) avg += l;
		avg /= docandnum.values().size();
		final float Avg = avg;
		return docandnum.entrySet().stream()
				.filter(e -> e.getValue() >= Avg)
				.map(e -> e.getKey())
				.collect(Collectors.toList());
	}

	/**
	 * Retrieves the information about doctors and relative number of assigned patients.
	 * <p>
	 * The method returns list of strings formatted as "{@code ### : ID SURNAME NAME}" where {@code ###}
	 * represent the number of patients (printed on three characters).
	 * <p>
	 * The list is sorted by decreasing number of patients.
	 * 
	 * @return the collection of strings with information about doctors and patients count
	 */
	public Collection<String> doctorsByNumPatients(){
		return patientToDoctor.entrySet().stream()
				.map(e -> e.getValue())
				.collect(Collectors.groupingBy(
						id -> id, 
						Collectors.counting())
				).entrySet().stream()
					.sorted((e1, e2) -> -Long.compare(e1.getValue(), e2.getValue()))
					.map(e -> String.format("%3d", e.getValue()) + " : " + e.getKey() + " " + doctors.get(e.getKey()).getSurnName())
					.collect(Collectors.toList());
	}
	
	/**
	 * Retrieves the number of patients per (their doctor's)  speciality
	 * <p>
	 * The information is a collections of strings structured as {@code ### - SPECIALITY}
	 * where {@code SPECIALITY} is the name of the speciality and 
	 * {@code ###} is the number of patients cured by doctors with such speciality (printed on three characters).
	 * <p>
	 * The elements are sorted first by decreasing count and then by alphabetic speciality.
	 * 
	 * @return the collection of strings with speciality and patient count information.
	 */
	public Collection<String> countPatientsPerSpecialization(){
		return patientToDoctor.values().stream()
				.collect(Collectors.groupingBy(
					s -> doctors.get(s).getSpecialization(),
					Collectors.counting())
				).entrySet().stream()
						.sorted((e1, e2) -> {
							if (e1.getValue() == e2.getValue()) {
								return e1.getKey().compareTo(e2.getKey());
							} else {
								return e1.getValue().compareTo(e2.getValue());
							}
						})
						.map(e -> String.format("%3d", e.getValue()) + " - " + e.getKey())
						.collect(Collectors.toList());
		
	}
	
}
