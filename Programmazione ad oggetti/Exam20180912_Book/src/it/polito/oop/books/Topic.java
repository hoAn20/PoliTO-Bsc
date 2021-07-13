package it.polito.oop.books;

import java.util.Comparator;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.stream.Collectors;

public class Topic {
	
	private String keyWord;
	private Map<String, Topic> subTopics = new HashMap<String, Topic>();
	
	Topic (String key) {
		this.keyWord = key;
	}

	public String getKeyword() {
        return this.keyWord;
	}
	
	@Override
	public String toString() {
	    return this.keyWord;
	}

	public boolean addSubTopic(Topic topic) {
		if (subTopics.containsKey(topic.getKeyword()))
			return false;
        
		subTopics.put(topic.getKeyword(), topic);
		
		return true;
	}

	/*
	 * Returns a sorted list of subtopics. Topics in the list *MAY* be modified without
	 * affecting any of the Book topic.
	 */
	public List<Topic> getSubTopics() {
        return subTopics.values().stream()
        		.sorted(Comparator.comparing(Topic::getKeyword))
        		.collect(Collectors.toList());
	}

	public List<Topic> getWithSub() {
		List<Topic> l = new LinkedList<Topic>();
		l.add(this);
		subTopics.values().stream()
			.forEach(t -> l.addAll(t.getWithSub()));
		return l;
	}
}
