package it.polito.oop.books;

import java.util.LinkedList;
import java.util.List;
import java.util.Set;
import java.util.stream.Collectors;

public class Question {
	
	private class Answer {
		String text;
		boolean isCorrect;
		
		public Answer(String text, boolean correct) {
			this.text = text;
			this.isCorrect = correct;
		}
	}
	
	private String text;
	private Topic mainTopic;
	private List<Answer> answers = new LinkedList<Question.Answer>();
	
	public Question(String question, Topic main) {
		this.text = question;
		this.mainTopic = main;
	}

	public String getQuestion() {
		return this.text;
	}
	
	public Topic getMainTopic() {
		return this.mainTopic;
	}

	public void addAnswer(String answer, boolean correct) {
		answers.add(new Answer(answer, correct));
	}
	
    @Override
    public String toString() {
        return this.text + " ("+mainTopic.toString()+")";
    }

	public long numAnswers() {
	    return answers.size();
	}

	public Set<String> getCorrectAnswers() {
		return answers.stream()
				.filter(a -> a.isCorrect)
				.map(a -> a.text)
				.collect(Collectors.toSet());
	}

	public Set<String> getIncorrectAnswers() {
		return answers.stream()
				.filter(a -> !a.isCorrect)
				.map(a -> a.text)
				.collect(Collectors.toSet());
	}
}
