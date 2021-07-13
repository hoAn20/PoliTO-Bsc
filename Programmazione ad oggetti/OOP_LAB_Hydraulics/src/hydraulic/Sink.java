package hydraulic;

/**
 * Represents the sink, i.e. the terminal element of a system
 *
 */
public class Sink extends ElementExt {

	/**
	 * Constructor
	 * @param name
	 */
	public Sink(String name) {
		super(name);
		this.next = null;
	}
	
	@Override
	public void connect(Element elem){
		return;
	}
	
	@Override
	public void simulate(SimulationObserver obs) {
		obs.notifyFlow(this.getClass().toString(), this.name, this.flow, SimulationObserver.NO_FLOW);
	}
	
	@Override
	public String layout() {
		return "[" + this.name + "]" + this.getClass().getSimpleName() + " *";
	}
	
}
