package university;

public class Exam {
	
	private int studentId;
	private int courseId;
	private int grade;

	public Exam(int studentId, int courseID, int grade) {
		this.studentId = studentId;
		this.courseId = courseID;
		this.grade = grade;
	}

	public int getStudentId() {
		return studentId;
	}
	
	public int getCourseId() {
		return courseId;
	}
	
	public int getGrade() {
		return grade;
	}

}
