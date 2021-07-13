package hydraulic;

public abstract class ElementExt extends Element{
	
	protected double maxFlow;

	public ElementExt(String name) {
		super(name);
	}

	public void setMaxFlow(double maxFlow) {
		this.maxFlow = maxFlow;
	}
	
	public void simulate(SimulationObserver obs, boolean maxFlowCheck) {
		obs.notifyFlow(this.getClass().getSimpleName(), this.name, this.flow, this.flow);
		this.next.getClass().cast(this.next).setFlow(this.flow);
		((ElementExt)this.next.getClass().cast(this.next)).simulate(obs, maxFlowCheck);
	}
	
	public void simulate(double in, SimulationObserver obs, boolean maxFlowCheck) {
		obs.notifyFlow(this.getClass().getSimpleName(), this.name, in, this.flow);
		this.next.getClass().cast(this.next).setFlow(this.flow);
		((ElementExt)this.next.getClass().cast(this.next)).simulate(obs, maxFlowCheck);
	}
	
	public void simulate(SimulationObserver obs, double out, boolean maxFlowCheck) {
		obs.notifyFlow(this.getClass().getSimpleName(), this.name, this.flow, out);
		this.next.getClass().cast(this.next).setFlow(out);
		((ElementExt)this.next.getClass().cast(this.next)).simulate(obs, maxFlowCheck);
	}

}
