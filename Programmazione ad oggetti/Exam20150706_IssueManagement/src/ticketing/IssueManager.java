package ticketing;

import java.util.Comparator;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.SortedMap;
import java.util.TreeMap;
import java.util.stream.Collectors;
import java.util.stream.Stream;

import ticketing.Ticket.Severity;
import ticketing.Ticket.State;

public class IssueManager {

    /**
     * Eumeration of valid user classes
     */
    public static enum UserClass {
        /** user able to report an issue and create a corresponding ticket **/
        Reporter, 
        /** user that can be assigned to handle a ticket **/
        Maintainer }
    
    private Map<String, Reporter> reporters = new HashMap<>();
    private Map<String, Maintainer> maintainers = new HashMap<>();
    private Map<String, Component> components = new HashMap<>();
    private Map<Integer, Ticket> tickets = new HashMap<>();
    
    /**
     * Creates a new user
     * 
     * @param username name of the user
     * @param classes user classes
     * @throws TicketException if the username has already been created or if no user class has been specified
     */
    public void createUser(String username, UserClass... classes) throws TicketException {
    	
    	if (reporters.keySet().contains(username) || maintainers.keySet().contains(username)) {
        	throw new TicketException("Username already existing");
        }
        if (classes.length < 1) {
        	throw new TicketException("No UserClass passed");
        }
        
        for (UserClass u : classes) {
        	if (u == UserClass.Reporter && !reporters.keySet().contains(username)) {
        		reporters.put(username, new Reporter(username));
        	}
        	if (u == UserClass.Maintainer && !maintainers.keySet().contains(username)) {
        		maintainers.put(username, new Maintainer(username));
        	}
        }
    }

    /**
     * Creates a new user
     * 
     * @param username name of the user
     * @param classes user classes
     * @throws TicketException if the username has already been created or if no user class has been specified
     */
    public void createUser(String username, Set<UserClass> classes) throws TicketException {
    	
    	if (reporters.keySet().contains(username) || maintainers.keySet().contains(username)) {
        	throw new TicketException("Username already existing");
        }
        if (classes.size() < 1) {
        	throw new TicketException("No UserClass passed");
        }
        
        for (UserClass u : classes) {
        	if (u == UserClass.Reporter && !reporters.keySet().contains(username)) {
        		reporters.put(username, new Reporter(username));
        	}
        	if (u == UserClass.Maintainer && !maintainers.keySet().contains(username)) {
        		maintainers.put(username, new Maintainer(username));
        	}
        }
    }
   
    /**
     * Retrieves the user classes for a given user
     * 
     * @param username name of the user
     * @return the set of user classes the user belongs to
     */
    public Set<UserClass> getUserClasses(String username){
    	
    	HashSet<UserClass> both = new HashSet<>();
        both.add(UserClass.Reporter);
        both.add(UserClass.Maintainer);
    	
    	if (reporters.containsKey(username) && maintainers.containsKey(username)) {
    		return both;
    	} else if (reporters.keySet().contains(username)) {
    		both.remove(UserClass.Maintainer);
    		return both;
    	} else if (maintainers.keySet().contains(username)) {
    		both.remove(UserClass.Reporter);
    		return both;
    	}
    	return null;
    }
    
    /**
     * Creates a new component
     * 
     * @param name unique name of the new component
     * @throws TicketException if a component with the same name already exists
     */
    public void defineComponent(String name) throws TicketException {
        
    	if (components.containsKey(name)) {
    		throw new TicketException("Component already existing");
    	}
    	
    	components.put(name, new Component(name));
    }
    
    /**
     * Creates a new sub-component as a child of an existing parent component
     * 
     * @param name unique name of the new component
     * @param parentPath path of the parent component
     * @throws TicketException if the the parent component does not exist or 
     *                          if a sub-component of the same parent exists with the same name
     */
    public void defineSubComponent(String name, String parentPath) throws TicketException {
        
    	if (components.containsKey(name)) {
    		throw new TicketException();
    	}
    	
    	String[] parents = parentPath.split("/");
    	
    	if (!components.containsKey(parents[parents.length - 1])) {
    		throw new TicketException();
    	}
    	
    	Component parent = components.get(parents[parents.length - 1]);
    	components.put(name, new Component(name, parent));
    	parent.addSubComponent(name);
    }
    
    /**
     * Retrieves the sub-components of an existing component
     * 
     * @param path the path of the parent
     * @return set of children sub-components
     */
    public Set<String> getSubComponents(String path){
    	String[] names = path.split("/");

        return components.get(names[names.length - 1]).getSubComponents();
    }

    /**
     * Retrieves the parent component
     * 
     * @param path the path of the parent
     * @return name of the parent
     */
    public String getParentComponent(String path){
    	String[] names = path.split("/");
    	if (names.length == 2) {
    		return null;
    	}
        return Stream.of(names)
        		.limit(names.length - 1)
        		.skip(1)
        		.collect(Collectors.joining("/", "/", ""));
    }

    /**
     * Opens a new ticket to report an issue/malfunction
     * 
     * @param username name of the reporting user
     * @param componentPath path of the component or sub-component
     * @param description description of the malfunction
     * @param severity severity level
     * 
     * @return unique id of the new ticket
     * 
     * @throws TicketException if the user name is not valid, the path does not correspond to a defined component, 
     *                          or the user does not belong to the Reporter {@link IssueManager.UserClass}.
     */
    public int openTicket(String username, String componentPath, String description, Ticket.Severity severity) throws TicketException {
        
    	String[] names = componentPath.split("/");
    	
    	if (!reporters.containsKey(username)) {
    		throw new TicketException();
    	}
    	for (int i = 1; i < names.length; ++i) {
    		String s = names[i];
    		if (!components.containsKey(s)) {
    			throw new TicketException("Non esiste il componente " + s);
    		}
    	}
    	
    	Ticket ticket = new Ticket(username, componentPath, description, severity);
    	tickets.put(ticket.getId(),	ticket);
    	
    	return ticket.getId();
    }
    
    /**
     * Returns a ticket object given its id
     * 
     * @param ticketId id of the tickets
     * @return the corresponding ticket object
     */
    public Ticket getTicket(int ticketId){
        return tickets.get(ticketId);
    }
    
    /**
     * Returns all the existing tickets sorted by severity
     * 
     * @return list of ticket objects
     */
    public List<Ticket> getAllTickets(){
        return tickets.values().stream()
        		.sorted((t1, t2) -> -t1.getSeverity().compareTo(t2.getSeverity()))
        		.collect(Collectors.toList());
    }
    
    /**
     * Assign a maintainer to an open ticket
     * 
     * @param ticketId  id of the ticket
     * @param username  name of the maintainer
     * @throws TicketException if the ticket is in state <i>Closed</i>, the ticket id or the username
     *                          are not valid, or the user does not belong to the <i>Maintainer</i> user class
     */
    public void assingTicket(int ticketId, String username) throws TicketException {
        
    	if (!tickets.containsKey(ticketId) || !maintainers.containsKey(username)) {
    		throw new TicketException();
    	}
    	
    	tickets.get(ticketId).setAsAssigned();
    	maintainers.get(username).assign(ticketId);
    }

    /**
     * Closes a ticket
     * 
     * @param ticketId id of the ticket
     * @param description description of how the issue was handled and solved
     * @throws TicketException if the ticket is not in state <i>Assigned</i>
     */
    public void closeTicket(int ticketId, String description) throws TicketException {
        
    	if (!(tickets.get(ticketId).getState().compareTo(State.Assigned) == 0)) {
    		throw new TicketException();
    	}
    	tickets.get(ticketId).setAsClosed(description);
    }

    /**
     * returns a sorted map (keys sorted in natural order) with the number of  
     * tickets per Severity, considering only the tickets with the specific state.
     *  
     * @param state state of the tickets to be counted, all tickets are counted if <i>null</i>
     * @return a map with the severity and the corresponding count 
     */
    public SortedMap<Ticket.Severity,Long> countBySeverityOfState(Ticket.State state){
        Map<Ticket.Severity, Long> nps = tickets.values().stream()
        		.map(t -> t.getSeverity())
        		.collect(Collectors.groupingBy(
        				s -> s,
        				Collectors.counting()
        				)
        			);
        SortedMap<Ticket.Severity, Long> npss = new TreeMap<Ticket.Severity, Long>();
        for (Severity s : nps.keySet()) {
        	npss.put(s, nps.get(s));
        }
        
        return npss;
    }

    /**
     * Find the top maintainers in terms of closed tickets.
     * 
     * The elements are strings formatted as <code>"username:###"</code> where <code>username</code> 
     * is the user name and <code>###</code> is the number of closed tickets. 
     * The list is sorter by descending number of closed tickets and then by username.

     * @return A list of strings with the top maintainers.
     */
    public List<String> topMaintainers(){
        return maintainers.values().stream()
        		.sorted(new Comparator<Maintainer>() {

					@Override
					public int compare(Maintainer o1, Maintainer o2) {
						if (o1.getNumberOfClosedTickets(tickets) == o2.getNumberOfClosedTickets(tickets)) {
							return o1.getUsername().compareTo(o2.getUsername());
						}
						
						return -o1.getNumberOfClosedTickets(tickets).compareTo(o2.getNumberOfClosedTickets(tickets));
					}
        			
				})
        		.map(m -> m.getUsername() + ":" + m.getNumberOfClosedTickets(tickets))
        		.collect(Collectors.toList());
    }

}
