package it.polito.oop.books;

import java.util.List;
import java.util.Set;


public class Assignment {
	
	private String id;
	private ExerciseChapter chapter;
	private double score;

    public Assignment(String iD, ExerciseChapter chapter) {
    	this.id = iD;
    	this.chapter = chapter;
    	this.score = 0;
	}

	public String getID() {
        return this.id;
    }

    public ExerciseChapter getChapter() {
        return this.chapter;
    }

    public double addResponse(Question q,List<String> answers) {
    	double as;
    	long n = q.numAnswers();
    	long fn = 0;
    	long fp = 0;
    	long dc = 0;
    	Set<String> corSet = q.getCorrectAnswers();
    	for (String s : answers) {
    		if (corSet.contains(s))
    			dc ++;
    		else 
    			fn++;
    	}
    	fp = corSet.size() - dc;
    	as = ((double)(n - fp - fn)) / n;
    	score += as;
        return as;
    }
    
    public double totalScore() {
        return score;
    }

}
