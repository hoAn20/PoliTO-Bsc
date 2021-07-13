package university;

import java.util.Arrays;
import java.util.logging.Logger;

public class UniversityExt extends University {
	
	public static final int N_TOP = 3;
	
	private final static Logger logger = Logger.getLogger("University");
	
	private Exam[] exams;
	
	private int examIndex;

	public UniversityExt(String name) {
		super(name);
		// Example of logging
		logger.info("Creating extended university object");
		this.examIndex = -1;
		exams = new Exam[MAX_COURSES * MAX_STUDENTS];
	}
	
	@Override
	public int enroll(String first, String last){
		this.students[++this.studentIndex] = new Student(first, last);
		logger.info("New student enrolled: " + this.studentIndex + ", " + first + 
				"" + last);
		return this.studentIndex + BASE_MAT;
	}
	
	@Override
	public int activate(String title, String teacher){
		this.courses[++this.courseIndex] = new Course(title, teacher);
		logger.info("New course activated: " + this.courseIndex + ", " + title);
		return this.courseIndex + BASE_COURSE;
	}
	
	@Override
	public void register(int studentID, int courseCode){
		this.students[studentID-BASE_MAT].addCourse(courseCode);
		this.courses[courseCode-BASE_COURSE].addStudent(studentID);
		logger.info("Student " + studentID + " signed up for course " + courseCode);
	}
	
	public void exam(int studentId, int courseID, int grade) {
		exams[++examIndex] = new Exam(studentId, courseID, grade);
		if (! students[studentId - BASE_MAT].attendsCourse(courseID)) {
			logger.info("Lo studente inserito non frequenta il corso indicato.");
		} else {
			logger.info("Student " + studentId + "took an exam in course " +
					courseID + " with grade " + grade);
		}
	}

	public String studentAvg(int studentId) {
		int cnt = 0;
		double somma = 0;
		
		for (int i = 0; i < examIndex + 1; ++i) {
			if (exams[i].getStudentId() == studentId) {
				cnt++;
				somma += exams[i].getGrade();
			}
		}
		
		if (cnt == 0) {
			return "Student " + studentId + " hasn't taken any exams";
		} else {
			return "Student " + studentId + ": " + String.format("%.2f", somma/cnt);
		}
	}
	
	public String courseAvg(int courseId) {
		int cnt = 0;
		double somma = 0;
		
		for (int i = 0; i < examIndex + 1; ++i) {
			if (exams[i].getCourseId() == courseId) {
				cnt++;
				somma += exams[i].getGrade();
			}
		}
		
		if (cnt == 0) {
			return "No student has taken the exam in " + courses[courseId - BASE_COURSE].getTitle();
		} else {
			return "The average for the course " + courses[courseId - BASE_COURSE] + " is: " + String.format("%.2f", somma/cnt);
		}
	}
	
	public String topThreeStudents() {
		Student[] top = new Student[N_TOP+1];
		int cnt;
		double somma;
		
		for (int i = 0; i < N_TOP; i++) {
			top[i] = new Student(-1);
		}
		
		for (int i = 0; i < studentIndex + 1; i++) {
			cnt = 0;
			somma = 0;
			for (int j = 0; j < examIndex + 1; j++) {
				if (exams[j].getStudentId() == i + BASE_MAT) {
					cnt++;
					somma += exams[j].getGrade();
				}
			}
			if (cnt == 0) {
				students[i].setPt(0.0, 0);
			} else {
				students[i].setPt(somma, cnt);
			}
			
			top[N_TOP] = students[i];
			Arrays.sort(top, (a, b) -> (int)(b.getPt() - a.getPt()));
		}
		
		StringBuffer res = new StringBuffer();
		for (int i = 0 ; i < N_TOP; i++) {
			if (top[i].getFirst() != null && top[i].getPt() > 0) {
				res.append(top[i].getPoints());
				res.append("\n");
			}
		}
		
		return res.toString();
	}
}
