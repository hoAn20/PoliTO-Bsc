package it.polito.oop.books;

import java.util.Comparator;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.stream.Collectors;

public class Book {
	
	private Map<String, Topic> topics = new HashMap<String, Topic>();
	private Map<String, Question> questions = new HashMap<String, Question>();
	private List<TheoryChapter> theoryChapters = new LinkedList<TheoryChapter>();
	private List<ExerciseChapter> exerciseChapters = new LinkedList<ExerciseChapter>();
	private List<Assignment> assignments = new LinkedList<Assignment>();

    /**
	 * Creates a new topic, if it does not exist yet, or returns a reference to the
	 * corresponding topic.
	 * 
	 * @param keyword the unique keyword of the topic
	 * @return the {@link Topic} associated to the keyword
	 * @throws BookException
	 */
	public Topic getTopic(String keyword) throws BookException {
		
		if (keyword == null || keyword.equals(""))
			throw new BookException();
		
		if (!topics.containsKey(keyword))
			topics.put(keyword, new Topic(keyword));
		
	    return topics.get(keyword);
	}

	public Question createQuestion(String question, Topic mainTopic) {
		Question q = new Question(question, mainTopic);
		questions.put(question, q);
        return q;
	}

	public TheoryChapter createTheoryChapter(String title, int numPages, String text) {
		TheoryChapter t = new TheoryChapter(title, numPages, text);
        theoryChapters.add(t);
		return t;
	}

	public ExerciseChapter createExerciseChapter(String title, int numPages) {
		ExerciseChapter e = new ExerciseChapter(title, numPages);
		exerciseChapters.add(e);
        return e;
	}

	public List<Topic> getAllTopics() {
		List<Topic> l = new LinkedList<Topic>();
		theoryChapters.stream()
			.forEach(t -> l.addAll(t.getTopics()));
		exerciseChapters.stream()
			.forEach(t -> l.addAll(t.getTopics()));
		
        return l.stream()
        		.distinct()
        		.sorted(Comparator.comparing(Topic::getKeyword))
        		.collect(Collectors.toList());
	}

	public boolean checkTopics() {
		List<Topic> e = new LinkedList<Topic>();
		List<Topic> t = new LinkedList<Topic>();
		theoryChapters.stream()
				.forEach(m -> t.addAll(m.getTopics()));
		exerciseChapters.stream()
				.forEach(m -> e.addAll(m.getTopics()));
		
		for (Topic top : e) {
			if (!t.contains(top))
				return false;
		}
		
        return true;
	}

	public Assignment newAssignment(String ID, ExerciseChapter chapter) {
		Assignment a = new Assignment(ID, chapter);
		assignments.add(a);
        return a;
	}
	
    /**
     * builds a map having as key the number of answers and 
     * as values the list of questions having that number of answers.
     * @return
     */
    public Map<Long,List<Question>> questionOptions(){
        return questions.values().stream()
        		.collect(Collectors.groupingBy(
        					q -> q.numAnswers(),
        					Collectors.toList()
        				)
        			);
    }
}
