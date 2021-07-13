package hydraulic;

/**
 * Represents a tap that can interrupt the flow.
 * 
 * The status of the tap is defined by the method
 * {@link #setOpen(boolean) setOpen()}.
 */

public class Tap extends ElementExt {
	
	private boolean opened;

	public Tap(String name) {
		super(name);
		//TODO: complete
	}
	
	/**
	 * Defines whether the tap is open or closed.
	 * 
	 * @param open  opening level
	 */
	public void setOpen(boolean open){
		this.opened = open;
	}
	
	@Override
	public void simulate(SimulationObserver obs) {
		if (this.opened) {
			super.simulate(obs);
		} else {
			super.simulate(obs, 0.0);
		}
	}

}
