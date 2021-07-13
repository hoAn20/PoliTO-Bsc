package ticketing;

/**
 * Class representing the ticket linked to an issue or malfunction.
 * 
 * The ticket is characterized by a severity and a state.
 */
public class Ticket {
    
    /**
     * Enumeration of possible severity levels for the tickets.
     * 
     * Note: the natural order corresponds to the order of declaration
     */
    public enum Severity { Blocking, Critical, Major, Minor, Cosmetic };
    
    /**
     * Enumeration of the possible valid states for a ticket
     */
    public static enum State { Open, Assigned, Closed }
    
    private static int newID = 1;
    
    private int id;
    private String reporterUsername;
    private String componentPath;
    private String description;
    private Severity severity;
    private State state;
    private String solution;
    
    public Ticket(String username, String componentPath, String description, Severity severity) {
		this.reporterUsername = username;
		this.componentPath = componentPath;
		this.description = description;
		this.severity = severity;
		this.id = newID++;
		this.state = State.Open;
	}

	public int getId(){
        return this.id;
    }

    public String getDescription(){
        return this.description;
    }
    
    public Severity getSeverity() {
        return this.severity;
    }

    public String getAuthor(){
        return this.reporterUsername;
    }
    
    public String getComponent(){
        return this.componentPath;
    }
    
    public State getState(){
        return this.state;
    }
    
    public String getSolutionDescription() throws TicketException {
        return solution;
    }

	public void setAsAssigned() {
		this.state = State.Assigned;
	}

	public void setAsClosed(String solution) {
		this.state = State.Closed;
		this.solution = solution;
	}
}
