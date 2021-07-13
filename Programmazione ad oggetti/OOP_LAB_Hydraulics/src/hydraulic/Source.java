package hydraulic;

/**
 * Represents a source of water, i.e. the initial element for the simulation.
 *
 * The status of the source is defined through the method
 * {@link #setFlow(double) setFlow()}.
 */
public class Source extends ElementExt {

	public Source(String name) {
		super(name);
	}
	
	@Override
	public void simulate(SimulationObserver obs) {
		super.simulate(SimulationObserver.NO_FLOW, obs);
	}
	
	@Override
	public void simulate(SimulationObserver obs, boolean enableMaxFlowCheck) {
		super.simulate(SimulationObserver.NO_FLOW, obs, enableMaxFlowCheck);
	}
	
	@Override
	public void setMaxFlow(double maxFlow) {
		return;
	}
	
}
