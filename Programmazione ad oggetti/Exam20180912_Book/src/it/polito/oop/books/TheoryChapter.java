package it.polito.oop.books;

import java.util.Comparator;
import java.util.LinkedList;
import java.util.List;
import java.util.stream.Collectors;


public class TheoryChapter {
	
	private String title;
	private int nPages;
	private String text;
	private List<Topic> topics = new LinkedList<Topic>();

	public TheoryChapter(String title, int nPages, String text) {
		super();
		this.title = title;
		this.nPages = nPages;
		this.text = text;
	}

	public String getText() {
		return this.text;
	}

    public void setText(String newText) {
    	this.text = newText;
    }


	public List<Topic> getTopics() {
        return topics.stream().distinct().sorted(Comparator.comparing(Topic::getKeyword)).collect(Collectors.toList());
	}

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
    
    public void addTopic(Topic topic) {
    	topics.addAll(topic.getWithSub());
    }
    
}
