package managingProperties;

import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;
import java.util.SortedMap;
import java.util.TreeMap;
import java.util.stream.Collectors;
import java.util.stream.Stream;


public class PropertyManager {
	
	private Map<String, Building> buildings = new HashMap<String, Building>();
	private Map<String, List<String>> owners = new HashMap<>();
	private Map<String, List<String>> professionals = new HashMap<>();
	private Map<Integer, Request> requests = new HashMap<Integer, Request>();

	/**
	 * Add a new building 
	 */
	public void addBuilding(String building, int n) throws PropertyException {
		
		if (buildings.keySet().contains(building)) {
			throw new PropertyException("Building already existing");
		}
		
		if (n < 1 || n > 100) {
			throw new PropertyException("Too much or not significant num of appartments");
		}
		
		Building b = new Building(building, n);
		
		buildings.put(building, b);
		
	}

	public void addOwner(String owner, String... apartments) throws PropertyException {
		
		if (owners.containsKey(owner)) {
			throw new PropertyException("Proprietario già esistente");
		}
		
		for (String s: apartments) {
			String[] split = s.split(":");
			if (!buildings.containsKey(split[0]))
				throw new PropertyException("Edificio non esistente: " + split[0]);
			buildings.get(split[0]).assignApp(owner, Integer.parseInt(split[1]));
		}
		owners.put(owner, new LinkedList<String>());
		owners.get(owner).addAll(List.of(apartments));
		
	}

	/**
	 * Returns a map ( number of apartments => list of buildings ) 
	 * 
	 */
	public SortedMap<Integer, List<String>> getBuildings() {
		Map<Integer, List<String>> m = buildings.keySet().stream()
				.collect(Collectors.groupingBy(
						s -> buildings.get(s).getNApp(),
						Collectors.toList()
						)
					);
		SortedMap<Integer, List<String>> m2 = new TreeMap<>();
		for (Integer i : m.keySet()) {
			m2.put(i, m.get(i));
		}
		return m2;
	}

	public void addProfessionals(String profession, String... professionals) throws PropertyException {
		
		if (this.professionals.containsKey(profession)) {
			throw new PropertyException("Professione già esistente");
		}
		if(Stream.of(professionals).collect(Collectors.groupingBy(s -> s, Collectors.counting())).entrySet().size() != professionals.length)
			throw new PropertyException();
		
		for (String s: professionals) {
			for (List<String> l: this.professionals.values()) {
				if (l.contains(s))
					throw new PropertyException("Id lavoratore già inserito");
			}
		}
		
		this.professionals.put(profession, List.of(professionals));
		
	}

	/**
	 * Returns a map ( profession => number of workers )
	 *
	 */
	public SortedMap<String, Integer> getProfessions() {
		
		SortedMap<String, Integer> m = new TreeMap<String, Integer>();
		
		for (Entry<String, List<String>> e : professionals.entrySet()) {
			m.put(e.getKey(), e.getValue().size());
		}
		
		return m;
	}

	public int addRequest(String owner, String apartment, String profession) throws PropertyException {
		if (!owners.containsKey(owner)
				|| !owners.get(owner).contains(apartment) 
				|| !professionals.containsKey(profession))
			throw new PropertyException();
			
		Request r = new Request(owner, apartment, profession);
		requests.put(r.getId(), r);
		
		return r.getId();
	}

	public void assign(int requestN, String professional) throws PropertyException {
		
		if (!requests.containsKey(requestN))
			throw new PropertyException();
		if (requests.get(requestN).getState().compareTo(Request.State.pending) != 0)
			throw new PropertyException();
		if (!professionals.get(requests.get(requestN).getProfession()).contains(professional))
			throw new PropertyException();
		
		requests.get(requestN).setState(Request.State.assigned);
		requests.get(requestN).setProfessional(professional);
		
	}

	public List<Integer> getAssignedRequests() {
		return requests.values().stream()
				.filter(r -> r.getState() == Request.State.assigned)
				.map(r -> r.getId())
				.sorted()
				.collect(Collectors.toList());
	}

	
	public void charge(int requestN, int amount) throws PropertyException {
		if (!requests.containsKey(requestN))
			throw new PropertyException();
		if (requests.get(requestN).getState().compareTo(Request.State.assigned) != 0)
			throw new PropertyException();
		if (amount < 0 || amount > 1000) 
			throw new PropertyException();
		
		requests.get(requestN).setState(Request.State.completed);
		requests.get(requestN).setCharge(amount);
	}

	/**
	 * Returns the list of request ids
	 * 
	 */
	public List<Integer> getCompletedRequests() {
		
		return requests.values().stream()
				.filter(r -> r.getState() == Request.State.completed)
				.map(r -> r.getId())
				.sorted()
				.collect(Collectors.toList());
	}
	
	/**
	 * Returns a map ( owner => total expenses )
	 * 
	 */
	public SortedMap<String, Integer> getCharges() {
		return requests.values().stream()
				.filter(r -> r.getState() == Request.State.completed)
				.collect(Collectors.groupingBy(
						r -> r.getOwner(),
						TreeMap::new,
						Collectors.summingInt(r -> r.getCharge())
						)
					);
	}

	/**
	 * Returns the map ( building => ( profession => total expenses) ).
	 * Both buildings and professions are sorted alphabetically
	 * 
	 */
	public SortedMap<String, Map<String, Integer>> getChargesOfBuildings() {
		return requests.values().stream()
				.filter(r -> r.getState() == Request.State.completed)
				.collect(Collectors.groupingBy(
						r -> r.getBuilding(),
						TreeMap::new,
						Collectors.groupingBy(
								r -> r.getProfessional(),
								TreeMap::new,
								Collectors.summingInt(r->r.getCharge()))	
						)
					);
	}

}
