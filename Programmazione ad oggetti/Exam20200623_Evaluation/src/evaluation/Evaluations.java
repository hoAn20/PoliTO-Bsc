package evaluation;
import java.util.*;
import java.util.stream.Collectors;


/**
 * Facade class for the research evaluation system
 *
 */
public class Evaluations {
	
	private int maxLevel = 0;
	private Map<Integer, Integer> pointPerLevel = new HashMap<>();
	private Map<String, Journal> journals = new HashMap<>();
	private Set<String> disciplines = new HashSet<>();
	private Map<String, Group> groups = new HashMap<>();
	private Map<String, Paper> papers = new HashMap<>();

    //R1
    /**
     * Define number of levels and relative points.
     * 
     * Levels are numbered from 1 on, and points must be strictly decreasing
     *  
     * @param points points for the levels
     * @throws EvaluationsException thrown if points are not decreasing
     */
    public void addPointsForLevels (int... points) throws EvaluationsException {
    	for (int i = 0; i < points.length-1; ++i) {
    		if (points[i] < points[i+1])
    			throw new EvaluationsException("Punteggio successivo maggiore di precedente.");
    	}
    	
    	for (int i = 1; i <= points.length; ++i) {
    		pointPerLevel.put(i, points[i-1]);
    	}
    	this.maxLevel = points.length;
    }

    /**
     * Retrieves the points for the given level.
     * 
     * @param level level for which points are required
     * @return points for the level
     */
    public int getPointsOfLevel (int level) {
        return pointPerLevel.get(level);
    }

    /**
     * Add a new journal for a given disciplines and provides the corresponding level.
     * 
     * The level determines the points for the article published in the journal.
     * 
     * @param name name of the new journal
     * @param discipline reference discipline for the journal
     * @param level level for the journal.
     * @throws EvaluationsException thrown if the specified level does not exist
     */
    public void addJournal (String name, String discipline, int level) throws EvaluationsException {
    	if (level > this.maxLevel || level < 1)
    		throw new EvaluationsException("Livello non esistente");
    	
    	journals.put(name, new Journal(name, discipline, level));
    	disciplines.add(discipline);
    }

    /**
     * Retrieves number of journals.
     * 
     * @return journals count
     */
    public int countJournals() {
        return this.journals.values().size();
    }

    /**
     * Retrieves all the journals for a given discipline.
     * 
     * @param discipline the required discipline
     * @return list of journals (sorted alphabetically)
     */
    public List<String> getJournalNamesOfAGivenDiscipline(String discipline) {
    	if (!disciplines.contains(discipline))
    		return new LinkedList<>();
        return journals.values().stream()
        		.filter(j -> j.getDiscipline().compareTo(discipline) == 0)
        		.map(j -> j.getName())
        		.sorted(String::compareTo)
        		.collect(Collectors.toList());
    }

    //R2
    /**
     * Add a research group and the relative disciplines.
     * 
     * @param name name of the research group
     * @param disciplines list of disciplines
     * @throws EvaluationsException thrown in case of duplicate name
     */
    public void addGroup (String name, String... disciplines) throws EvaluationsException {
    	if (groups.containsKey(name))
    		throw new EvaluationsException("Gruppo già esistente");
    	
    	groups.put(name, new Group(name, disciplines));
    }

    /**
     * Define the members for a previously defined research group.
     * 
     * @param groupName name of the group
     * @param memberNames list of group members
     * @throws EvaluationsException thrown if name not previously defined.
     */
    public void setMembers (String groupName, String... memberNames) throws EvaluationsException {
    	if (!groups.containsKey(groupName))
    		throw new EvaluationsException("Gruppo inesistente");
    	
    	groups.get(groupName).setMembers(memberNames);
    }

    /**
     * Return list of members of a group.
     * The list is sorted alphabetically.
     * 
     * @param groupName name of the group
     * @return list of members
     */
    public List<String >getMembers(String groupName){
    	if (!groups.containsKey(groupName))
    		return new LinkedList<>();
        return groups.get(groupName).getMembers().stream()
        		.sorted(String::compareTo)
        		.collect(Collectors.toList());
    }

    /**
     * Retrieves the group names working on a given discipline
     * 
     * @param discipline the discipline of interest
     * @return list of group names sorted alphabetically
     */
    public List<String> getGroupNamesOfAGivenDiscipline(String discipline) {
    	if (!disciplines.contains(discipline))
    		return new LinkedList<>();
        return groups.values().stream()
        		.filter(g -> g.getDisciplines().contains(discipline))
        		.map(g -> g.getName())
        		.sorted(String::compareTo)
        		.collect(Collectors.toList());
    }

    //R3
    /**
     * Add a new journal articles, with a given title and the list of authors.
     * 
     * The journal must have been previously defined.
     * 
     * The authors (at least one) are members of research groups.
     * 
     * @param title title of the article
     * @param journalName name of the journal
     * @param authorNames list of authors
     * @throws EvaluationsException thrown if journal not defined or no author provided
     */
    public void addPaper (String title, String journalName, String... authorNames) throws EvaluationsException {
    	if (authorNames.length < 1)
    		throw new EvaluationsException("Nessun autore indicato");
    	if (!journals.containsKey(journalName))
    		throw new EvaluationsException("Rivista non esistente");
    	
    	papers.put(title, new Paper(title, journalName, authorNames));
    }



    /**
     * Retrieves the titles of the articles authored by a member of a research group
     * 
     * @param memberName name of the group member
     * @return list of titles sorted alphabetically
     */
    public List<String> getTitlesOfAGivenAuthor (String memberName) {
        return papers.values().stream()
        		.filter(p -> p.getAuthors().contains(memberName))
        		.map(p -> p.getTitle())
        		.sorted(String::compareTo)
        		.collect(Collectors.toList());
    }


    //R4
    /**
     * Returns the points for a given group member.
     * 
     * Points are collected for each article the member authored.
     * The points are those corresponding to the level of the
     * journal where the article is published, divided by
     * the total number of authors.
     * 
     * The total points are eventually rounded to the closest integer.
     * 
     * @param memberName name of the group member
     * @return total points
     */
    public int getPointsOfAGivenAuthor (String memberName) {
    	int totPt = 0;
    	for (Paper p : papers.values()) {
    		if (p.getAuthors().contains(memberName)) {
    			String j = p.getJournal();
    			float pt = getPointsOfLevel(journals.get(j).getLevel());
    			int size = p.getAuthors().size();
    			totPt += Math.round(pt / size);
    		}
    	}
        return totPt;
    }

    /**
     * Computes the total points collected by all members of all groups
     *  
     * @return the total points
     */
    public int evaluate() {
    	int totPt = 0;
    	for (Paper p : papers.values()) {
			String j = p.getJournal();
			totPt += getPointsOfLevel(journals.get(j).getLevel());
    	}
        return totPt;
    }


    //R5 Statistiche
    
    private int groupPt(String groupName) {
    	int totPt = 0;
    	for (String s: groups.get(groupName).getMembers()) {
    		totPt += getPointsOfAGivenAuthor(s);
    	}
    	return totPt;
    }
    
    /**
     * For each group return the total points collected
     * by all the members of each research group.
     * 
     * Group names are sorted alphabetically.
     * 
     * @return the map associating group name to points
     */
    public SortedMap<String, Integer> pointsForGroup() {
    	SortedMap<String, Integer> pts = new TreeMap<>();
    	for (Group g: groups.values()) {
    		pts.put(g.getName(), groupPt(g.getName()));
    	}
        return pts;
    }

    /**
     * For each amount of points returns a list of
     * the authors (group members) that achieved such score.
     * 
     * Points are sorted in decreasing order.
     * 
     * @return the map linking the number of point to the list of authors
     */
    public SortedMap<Integer, List<String>> getAuthorNamesPerPoints () {
    	List<String> au = new LinkedList<>();
    	for (Paper p: papers.values()) {
    		au.addAll(p.getAuthors());
    	}
    	au = au.stream().distinct().collect(Collectors.toList());
    	SortedMap<Integer, List<String>> m = new TreeMap<>(Collections.reverseOrder());
    	for (String a: au) {
    		int pt = getPointsOfAGivenAuthor(a);
    		if (!m.containsKey(pt))
    			m.put(pt, new LinkedList<>());
    		
    		m.get(pt).add(a);
    		Collections.sort(m.get(pt));
    	}
    		
        return m;
    }


}