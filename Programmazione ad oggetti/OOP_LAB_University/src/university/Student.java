package university;

public class Student{
	
	public final static int MAX_COURSES = 25;
	
	private String first;
	private String last;
	
	private int[] attendedCoursesCode;
	private int index;
	
	private double pt;

	public Student(String first, String last) {
		this.first = first;
		this.last = last;
		this.attendedCoursesCode = new int[MAX_COURSES];
	}

	public Student(double d) {
		this.pt = d;
	}
	
	public String getFirst() {
		return first;
	}

	public void addCourse(int i) {
		this.attendedCoursesCode[this.index++] = i;
	}

	public int getCoursesNumber() {
		return this.index;
	}

	public int getCourseCode(int i) {
		return this.attendedCoursesCode[i];
	}
	
	public String toString() {
		return this.first + " " + this.last;
	}
	
	public boolean attendsCourse(int id) {
		for(int i = 0; i < index; i++) {
			if(this.attendedCoursesCode[i] == id)
				return true;
		}
		return false;
	}
	
	public void setPt(double pt, int cnt) {
		if (cnt == 0) {
			this.pt = 0;
		} else {
			this.pt = pt/cnt + (double)(cnt * 10)/index;
		}
	}
	
	public double getPt() {
		return pt;
	}
	
	public String getPoints() {
		return this.first + " " + this.last + " : " + this.pt;
	}

}
