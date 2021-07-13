package example;
import static org.junit.Assert.*;
import java.util.*;
import org.junit.Test;

import evaluation.*;

public class TestApp {
@Test
public void test() throws EvaluationsException {
	Evaluations ev = new Evaluations();
	List<String> ls = null;
	//R1

	ev.addPointsForLevels (20, 14, 10, 6);
	try {
	    ev.addPointsForLevels (20, 10, 14, 6); 
	    fail("Did not detect points not decreasing"); 
	} catch(Exception ex){} //ok
	assertEquals (ev.getPointsOfLevel(2), 14);
	
	ev.addJournal ("j1a", "d1", 1); ev.addJournal ("j2x", "d2", 2);
	ev.addJournal ("j3a", "d3", 3); ev.addJournal ("j4a", "d4", 4);
	ev.addJournal ("j2a", "d2", 2); ev.addJournal ("j2h", "d2", 2);
	
//	System.out.println(ev.countJournals());
	
	try {
	    ev.addJournal ("jx", "d1", 5); 
	    fail("Unknown level not detected");
	} catch(Exception ex){} //ok
  
    ls = ev.getJournalNamesOfAGivenDiscipline("d20");
    assertEquals ("Empty list expected", 0, ls.size());
    ls = ev.getJournalNamesOfAGivenDiscipline("d2");
    assertEquals ("[j2a, j2h, j2x]", ls.toString());
    
    //R2
	ev.addGroup("g1", "d1", "d2");
	ev.addGroup("g3", "d2", "d3");
	ev.addGroup("g2", "d2", "d3", "d4");
    try {
    	ev.addGroup("g1", "d3");
    	fail("Did not detect duplicated group name");
    } catch(Exception ex){} //ok
    
	ev.setMembers("g1", "m1c", "m1b", "m1a", "m1d");
	ev.setMembers("g2", "m2a", "m2b", "m2c");
	ev.setMembers("g3", "m3a", "m3b", "m3d");
    try {
    	ev.setMembers("g10", "m1a", "m1b", "m1c");
    	fail("group name unknown");
    } catch(Exception ex){} //ok
    
    ls = ev.getGroupNamesOfAGivenDiscipline("d20");
    assertEquals ("[]", ls.toString());
    ls = ev.getGroupNamesOfAGivenDiscipline("d3");
    assertEquals ("[g2, g3]", ls.toString());
    
    //R3
	ev.addPaper("t2", "j1a", "m1b");
	ev.addPaper("t1", "j2a", "m1c", "m1b");
	ev.addPaper("t4", "j1a", "m1d");
	ev.addPaper("t3", "j2a", "m3a", "m1b", "m2a");
    
    ls = ev.getTitlesOfAGivenAuthor ("m1b");
    assertEquals ("[t1, t2, t3]", ls.toString());
    
//    ls = ev.getTitlesOfAGivenAuthor("m2a");
//    assertEquals ("Empty list expected", 0, ls.size());
 
    try {
    	ev.addPaper("tx", "jx", "m1c");
    	fail("journal name unknown");
    } catch(Exception ex){} //ok
      
    //R4
    int points = ev.getPointsOfAGivenAuthor("m1b"); 
    assertEquals (32,points);
    
    int totalPoints = ev.evaluate();
    assertEquals (68, totalPoints);
    
    //R5
    SortedMap<String, Integer> sm = ev.pointsForGroup();
    System.out.println(sm);
    assertEquals("{g1=59, g2=5, g3=5}", sm.toString());
    
    SortedMap<Integer, List<String>> sml = ev.getAuthorNamesPerPoints ();
    System.out.println(sml);
    assertEquals("{32=[m1b], 20=[m1d], 7=[m1c], 5=[m2a, m3a]}", sml.toString());
}
}
