package university;

/**
 * This class represents a university education system.
 * 
 * It manages students and courses.
 *
 */
public class University {
	
	public final static int MAX_STUDENTS = 1000;
	public final static int MAX_COURSES = 50;
	public final static int BASE_MAT = 10000;
	public final static int BASE_COURSE = 10;

	protected String name;
	protected String first;
	protected String last;
	
	protected int studentIndex;
	protected int courseIndex;
	
	protected Student[] students;
	
	protected Course[] courses;
	
	/**
	 * Constructor
	 * @param name name of the university
	 */
	public University(String name){
		this.name = name;
		this.students = new Student[MAX_STUDENTS];
		this.studentIndex = -1;
		this.courses = new Course[MAX_COURSES];
		this.courseIndex = -1;
	}
	
	/**
	 * Getter for the name of the university
	 * @return name of university
	 */
	public String getName(){
		return this.name;
	}
	
	/**
	 * Defines the rector for the university
	 * 
	 * @param first
	 * @param last
	 */
	public void setRector(String first, String last){
		this.first = first;
		this.last = last;
	}
	
	/**
	 * Retrieves the rector of the university
	 * 
	 * @return
	 */
	public String getRector(){
		return this.first + " " + this.last;
	}
	
	/**
	 * Enroll a student in the university
	 * 
	 * @param first first name of the student
	 * @param last last name of the student
	 * @return
	 */
	public int enroll(String first, String last){
		this.students[++this.studentIndex] = new Student(first, last);
		return this.studentIndex + BASE_MAT;
	}
	
	/**
	 * Retrieves the information for a given student
	 * 
	 * @param id the id of the student
	 * @return information about the student
	 */
	public String student(int id){
		return id + " " + this.students[id-BASE_MAT].toString();
	}
	
	/**
	 * Activates a new course with the given teacher
	 * 
	 * @param title title of the course
	 * @param teacher name of the teacher
	 * @return the unique code assigned to the course
	 */
	public int activate(String title, String teacher){
		this.courses[++this.courseIndex] = new Course(title, teacher);
		return this.courseIndex + BASE_COURSE;
	}
	
	/**
	 * Retrieve the information for a given course
	 * 
	 * @param code unique code of the course
	 * @return information about the course
	 */
	public String course(int code){
		return code + "," + this.courses[code-BASE_COURSE].toString();
	}
	
	/**
	 * Register a student to attend a course
	 * @param studentID id of the student
	 * @param courseCode id of the course
	 */
	public void register(int studentID, int courseCode){
		this.students[studentID-BASE_MAT].addCourse(courseCode);
		this.courses[courseCode-BASE_COURSE].addStudent(studentID);
	}
	
	/**
	 * Retrieve a list of attendees
	 * 
	 * @param courseCode unique id of the course
	 * @return list of attendees separated by "\n"
	 */
	public String listAttendees(int courseCode){
		StringBuffer list = new StringBuffer();
		
		for (int i = 0; i < this.courses[courseCode-BASE_COURSE].getStudentNumber(); ++i) {
			list.append(this.student(this.courses[courseCode-BASE_COURSE].getStudentID(i)));
			if (i < this.courses[courseCode-BASE_COURSE].getStudentNumber() - 1)
				list.append("\n");
		}
		return list.toString();
	}

	/**
	 * Retrieves the study plan for a student
	 * 
	 * @param studentID id of the student
	 * @return list of courses the student is registered for
	 */
	public String studyPlan(int studentID){
		StringBuffer plan = new StringBuffer();
		
		for (int i = 0; i < this.students[studentID-BASE_MAT].getCoursesNumber(); ++i) {
			plan.append(this.course(this.students[studentID-BASE_MAT].getCourseCode(i)));
			if (i < this.students[studentID-BASE_MAT].getCoursesNumber())
				plan.append("\n");
		}
		return plan.toString();
	}
}
