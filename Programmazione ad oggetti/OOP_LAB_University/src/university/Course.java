package university;

public class Course {
	
	private final static int MAX_STUDENTS = 100;
	
	private String title;
	private String teacher;
	
	private int[] attendingStudentID;
	private int index;

	public Course(String title, String teacher) {
		this.title = title;
		this.teacher = teacher;
		this.attendingStudentID = new int[MAX_STUDENTS];
	}

	public String toString() {
		return this.title + "," + this.teacher;
	}

	public void addStudent(int i) {
		this.attendingStudentID[this.index++] = i;
	}

	public int getStudentNumber() {
		return this.index;
	}

	public int getStudentID(int i) {
		return this.attendingStudentID[i];
	}
	
	public String getTitle() {
		return title;
	}

}
