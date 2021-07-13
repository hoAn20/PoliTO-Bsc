package it.polito.oop.books;

import java.util.Comparator;
import java.util.LinkedList;
import java.util.List;
import java.util.stream.Collectors;


public class ExerciseChapter {

    private String title;
	private int nPages;
	private List<Question> questions = new LinkedList<Question>();

	public ExerciseChapter(String title, int numPages) {
    	this.title = title;
    	this.nPages = numPages;
	}


	public List<Topic> getTopics() {
		List<Topic> l = new LinkedList<Topic>();
		questions.stream()
			.map(q -> q.getMainTopic())
			.forEach(t -> l.add(t));
        return l.stream()
        		.distinct()
        		.sorted(Comparator.comparing(Topic::getKeyword))
        		.collect(Collectors.toList());
	};
	

    public String getTitle() {
        return this.title;
    }

    public void setTitle(String newTitle) {
    	this.title = newTitle;
    }

    public int getNumPages() {
        return this.nPages;
    }
    
    public void setNumPages(int newPages) {
    	this.nPages = newPages;
    }
    

	public void addQuestion(Question question) {
		questions.add(question);
	}	
}
