package it.polito.oop.milliways;

import java.util.Comparator;
import java.util.HashMap;
import java.util.LinkedHashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;
import java.util.stream.Collectors;

public class Restaurant {
	
	private Map<String, Race> races = new HashMap<String, Race>();
	private List<Party> parties = new LinkedList<Party>();
	private Map<Integer, Hall> halls = new LinkedHashMap<Integer, Hall>();

    public Restaurant() {
	}
	
	public Race defineRace(String name) throws MilliwaysException{
		if (races.containsKey(name))
			throw new MilliwaysException();
		
		Race r = new Race(name);
		races.put(name, r);
	    return r;
	}
	
	public Party createParty() {
		Party p = new Party();
		parties.add(p);
	    return p;
	}
	
	public Hall defineHall(int id) throws MilliwaysException{
		if (halls.containsKey(id))
			throw new MilliwaysException();
		
		Hall h = new Hall(id);
		halls.put(id, h);
	    return h;
	}

	public List<Hall> getHallList() {
		return halls.values().stream().collect(Collectors.toList());
	}

	public Hall seat(Party party, Hall hall) throws MilliwaysException {
		if (!hall.isSuitable(party))
			throw new MilliwaysException();
		
		hall.allocate(party.getCompanions());
        return hall;
	}

	public Hall seat(Party party) throws MilliwaysException {
		for (Hall h: halls.values()) {
			if (h.isSuitable(party)) {
				h.allocate(party.getCompanions());
				return h;
			}
		}
        throw new MilliwaysException();
	}

	public Map<Race, Integer> statComposition() {
		List<Race> l = new LinkedList<Race>();
		halls.values().stream().forEach(h -> l.addAll(h.getCompanions()));
        return l.stream()
        		.collect(Collectors.groupingBy(
        					r -> r,
        					Collectors.counting()
        				)
        			).entrySet().stream()
        				.sorted(Comparator.comparing(Entry<Race, Long>::getKey))
        				.collect(Collectors.toMap(
							e -> e.getKey(),
							e -> e.getValue().intValue()
						)
					);
	}

	public List<String> statFacility() {
		List<String> l = new LinkedList<String>();
		halls.values().stream().forEach(h -> l.addAll(h.getFacilities()));
        return l.stream()
        		.collect(Collectors.groupingBy(
        					s -> s,
        					Collectors.counting()
        				)
        			).entrySet().stream()
        				.sorted(Comparator.comparing(Entry<String, Long>::getValue).reversed().thenComparing(Entry<String, Long>::getKey))
        				.map(e -> e.getKey())
        				.collect(Collectors.toList());
	}
	
	public Map<Integer,List<Integer>> statHalls() {
		Map<Integer, List<Integer>> m = halls.keySet().stream()
        		.collect(Collectors.groupingBy(
    					i -> halls.get(i).getNumFacilities(),
    					Collectors.toList()
    				)
    			).entrySet().stream()
    				.sorted((e1, e2) -> e1.getKey().compareTo(e2.getKey()))
    				.collect(Collectors.toMap(e -> e.getKey(), e -> e.getValue()));
		for (Integer i: m.keySet())
			m.get(i).stream().sorted().collect(Collectors.toList());
        return m;
	}
	

}
