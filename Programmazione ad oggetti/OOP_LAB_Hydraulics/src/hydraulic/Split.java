package hydraulic;

/**
 * Represents a split element, a.k.a. T element
 * 
 * During the simulation each downstream element will
 * receive a stream that is half the input stream of the split.
 */

public class Split extends ElementExt {
	
	protected Element[] nexts;

	/**
	 * Constructor
	 * @param name
	 */
	public Split(String name) {
		super(name);
		this.nexts = new Element[2];
	}
    
	/**
	 * returns the downstream elements
	 * @return array containing the two downstream element
	 */
    public Element[] getOutputs(){
        return this.nexts;
    }

    /**
     * connect one of the outputs of this split to a
     * downstream component.
     * 
     * @param elem  the element to be connected downstream
     * @param noutput the output number to be used to connect the element
     */
	public void connect(Element elem, int noutput){
		this.next = elem;
		this.nexts[noutput] = elem;
	}
	
	@Override
	public void simulate(SimulationObserver obs) {
		obs.notifyFlow(this.getClass().toString(), this.name, this.flow, this.flow/2, this.flow/2);
		for (Element e : this.nexts) {
			e.getClass().cast(e).setFlow(this.flow/2);
			e.getClass().cast(e).simulate(obs);
		}
	}
	
	@Override
	public String layout() {
		StringBuffer layout = new StringBuffer("[" + this.name + "]" + this.getClass().getSimpleName() + " ");
		String space = new String();
		for (int i = 0; i < layout.length(); i++) {
			space += " ";
		}
		layout.append("+-> ");
		
		for (int i = 0; i < this.nexts.length; i++) {
			if (i != 0 && i < this.nexts.length - 1) {
				String[] words = this.nexts[i].getClass().cast(this.nexts[i]).layout().split("\n");
				StringBuffer rows = new StringBuffer();
				layout.append(space + "|\n" + space + "+-> " + words[0] + "\n");
				for (int j = 1; j < words.length; j++) {
					rows = new StringBuffer(words[j] + "\n");
					rows.insert(0, space + "|   ");
					layout.append(rows);
				}
			} else if (i == 0) {
				String[] words = this.nexts[i].getClass().cast(this.nexts[i]).layout().split("\n");
				StringBuffer rows = new StringBuffer();
				layout.append(words[0] + "\n");
				for (int j = 1; j < words.length; j++) {
					rows = new StringBuffer(words[j] + "\n");
					rows.insert(0, space + "|   ");
					layout.append(rows);
				}
			} else {
				String[] words = this.nexts[i].getClass().cast(this.nexts[i]).layout().split("\n");
				StringBuffer rows = new StringBuffer();
				layout.append(space + "|\n" + space + "+-> " + words[0] + "\n");
				for (int j = 1; j < words.length; j++) {
					rows = new StringBuffer(words[j] + "\n");
					rows.insert(0, space + "    ");
					layout.append(rows);
				}
			}
		}
		
		return layout.toString();
	}
}
