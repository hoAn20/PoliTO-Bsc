package hydraulic;

/**
 * Represents a multisplit element, an extension of the Split that allows many outputs
 * 
 * During the simulation each downstream element will
 * receive a stream that is determined by the proportions.
 */

public class Multisplit extends Split {
	
	private double[] prop;

	/**
	 * Constructor
	 * @param name
	 * @param numOutput
	 */
	public Multisplit(String name, int numOutput) {
		super(name);
		this.nexts = new ElementExt[numOutput];
	}
	
	/**
	 * Define the proportion of the output flows w.r.t. the input flow.
	 * 
	 * The sum of the proportions should be 1.0 and 
	 * the number of proportions should be equals to the number of outputs.
	 * Otherwise a check would detect an error.
	 * 
	 * @param proportions the proportions of flow for each output
	 */
	public void setProportions(double... proportions) {
		this.prop = proportions;
	}
	
	@Override
	public void simulate(SimulationObserver obs) {
		double[] outs = new double[this.prop.length];
		for (int i = 0; i < this.prop.length; i++) {
			outs[i] = this.flow * prop[i];
		}
		obs.notifyFlow(this.getClass().toString(), this.name, this.flow, outs);
		for (int i = 0; i < outs.length; i++) {
			this.nexts[i].getClass().cast(this.nexts[i]).setFlow(outs[i]);
			this.nexts[i].getClass().cast(this.nexts[i]).simulate(obs);
		}
	}
}
