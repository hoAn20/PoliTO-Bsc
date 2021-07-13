package ticketing;

import java.util.LinkedList;
import java.util.List;
import java.util.Map;

import ticketing.Ticket.State;

public class Maintainer {
	
	private String username;
	private List<Integer> assignedTickets = new LinkedList<>();

	public Maintainer(String username) {
		this.username = username;
	}

	public void assign(int ticketId) {
		this.assignedTickets.add(ticketId);
	}
	
	public Integer getNumberOfClosedTickets(Map<Integer, Ticket> tickets) {
		int n = 0;
		for (Integer i : assignedTickets) {
			if (tickets.get(i).getState().compareTo(State.Closed) == 0) n++;
		}
		
		return n;
	}

	public String getUsername() {
		return username;
	}

}
