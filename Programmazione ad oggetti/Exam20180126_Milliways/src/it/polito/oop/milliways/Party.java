package it.polito.oop.milliways;

import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.stream.Collectors;

public class Party {
	
	private List<Race> companions = new LinkedList<Race>();

    public void addCompanions(Race race, int num) {
    	for (int i = 0; i < num; ++i)
    		companions.add(race);
	}

	public int getNum() {
        return companions.size();
	}

	public int getNum(Race race) {
	    return companions.stream()
	    		.filter(r -> race.getName().compareTo(r.getName()) == 0)
	    		.collect(Collectors.counting()).intValue();
	}

	public List<String> getRequirements() {
		List<String> l = new LinkedList<String>();
		companions.stream()
			.forEach(r -> l.addAll(r.getRequirements()));
        return l.stream()
        		.distinct()
        		.sorted()
        		.collect(Collectors.toList());
	}

    public Map<String,Integer> getDescription(){
        return companions.stream()
        		.collect(Collectors.groupingBy(
        					r -> r.getName(),
        					Collectors.counting()
        				)
        			).entrySet().stream()
        				.collect(Collectors.toMap(
        							e -> e.getKey(), 
        							e -> e.getValue().intValue()
        						)
        					);
    }
	
	List<Race> getCompanions() {
		return this.companions;
	}

}
