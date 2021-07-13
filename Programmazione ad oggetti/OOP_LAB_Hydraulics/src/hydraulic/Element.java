package hydraulic;

/**
 * Represents the generic abstract element of an hydraulics system.
 * It is the base class for all elements.
 *
 * Any element can be connect to a downstream element
 * using the method {@link #connect(Element) connect()}.
 */
public class Element {
	
	protected String name;
	
	protected Element next;
	
	protected double flow;
	
	/**
	 * Constructor
	 * @param name the name of the element
	 */
	public Element(String name){
		this.name = name;
	}

	/**
	 * getter method
	 * @return the name of the element
	 */
	public String getName(){
		return this.name;
	}
	
	/**
	 * Connects this element to a given element.
	 * The given element will be connected downstream of this element
	 * @param elem the element that will be placed downstream
	 */
	public void connect(Element elem){
		this.next = elem;
	}
	
	/**
	 * Retrieves the element connected downstream of this
	 * @return downstream element
	 */
	public Element getOutput(){
		return this.next;
	}
	
	public String layout() {
		String[] rows = this.next.getClass().cast(this.next)
				.layout().split("\n");
		StringBuffer layout = new StringBuffer("[" + this.name + "]" + this.getClass().getSimpleName() + " -> ");
		
		String space = new String();
		for (int i = 0; i < layout.length(); i++) {
			space += " ";
		}
		
		for (int i = 0; i < rows.length; i++) {
			if (i != 0) {
				rows[i] = space + rows[i];
				layout.append(rows[i] + '\n');
			} else {
				layout.append(rows[i] + '\n');
			}
		}
		return layout.toString();
	}
	
	public void setFlow(double flow) {
		this.flow = flow;
	}
	
	public void simulate(SimulationObserver obs) {
		obs.notifyFlow(this.getClass().getSimpleName(), this.name, this.flow, this.flow);
		this.next.getClass().cast(this.next).setFlow(this.flow);
		this.next.getClass().cast(this.next).simulate(obs);
	}
	
	public void simulate(double in, SimulationObserver obs) {
		obs.notifyFlow(this.getClass().getSimpleName(), this.name, in, this.flow);
		this.next.getClass().cast(this.next).setFlow(this.flow);
		this.next.getClass().cast(this.next).simulate(obs);
	}
	
	public void simulate(SimulationObserver obs, double out) {
		obs.notifyFlow(this.getClass().getSimpleName(), this.name, this.flow, out);
		this.next.getClass().cast(this.next).setFlow(out);
		this.next.getClass().cast(this.next).simulate(obs);
	}
	
}
