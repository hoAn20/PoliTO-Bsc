package it.polito.oop.milliways;

import java.util.LinkedList;
import java.util.List;
import java.util.stream.Collectors;

public class Hall {
	
	private int id;
	private List<String> facilities = new LinkedList<String>();
	private List<Race> companions = new LinkedList<Race>();

	public Hall(int id) {
		this.id = id;
	}

	public int getId() {
		return this.id;
	}

	public void addFacility(String facility) throws MilliwaysException {
		if (facilities.contains(facility))
			throw new MilliwaysException();
		
		facilities.add(facility);
	}

	public List<String> getFacilities() {
        return facilities.stream().sorted().collect(Collectors.toList());
	}
	
	int getNumFacilities(){
        return facilities.size();
	}

	public boolean isSuitable(Party party) {
		List<String> pf = party.getRequirements();
		for (String s: pf)
			if (!facilities.contains(s))
				return false;
		
		return true;
	}

	public void allocate(List<Race> list) {
		companions.addAll(list);
	}

	public List<Race> getCompanions() {
		return this.companions;
	}

}
